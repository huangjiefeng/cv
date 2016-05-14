#include "fileuploader.h"
#include "connectionmanager.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QTextCodec>
#include <QDebug>
#include <QFileInfo>
#include <QMutexLocker>
#include <QIcon>

FileUploader::FileUploader()
{
	m_stop = false;
	m_uploadworker = new UploadWorker();
	connect(m_uploadworker, SIGNAL(reportStatus(UploadItem *)), this, SLOT(refreshItemStatus(UploadItem *)));
	connect(m_uploadworker, SIGNAL(uploadComplete(const QString &)), this, SLOT(fileComplete(const QString &)));
	connect(m_uploadworker, SIGNAL(uploadStop(const QString &)), this, SLOT(fileStop(const QString &)));

	m_scaner = new FileScaner();
	connect(m_scaner, SIGNAL(addUploadFile(const QString &, int, int)), this, SLOT(addFile(const QString &, int, int)));
	connect(m_scaner, SIGNAL(error(const QString &)), this, SIGNAL(error(const QString &)));
}

FileUploader::~FileUploader()
{
	if(m_scaner != NULL)
	{
		disconnect(m_scaner, SIGNAL(error(const QString &)), this, SIGNAL(error(const QString &)));
		disconnect(m_scaner, SIGNAL(addUploadFile(const QString &, int, int)), this, SLOT(addFile(const QString &, int, int)));
		m_scaner->stop();
		delete m_scaner;
		m_scaner = NULL;
	}

	disconnect(m_uploadworker, SIGNAL(reportStatus(UploadItem *)), this, SLOT(refreshItemStatus(UploadItem *)));
	disconnect(m_uploadworker, SIGNAL(uploadComplete(const QString &)), this, SLOT(fileComplete(const QString &)));
	disconnect(m_uploadworker, SIGNAL(uploadStop(const QString &)), this, SLOT(fileStop(const QString &)));
	if(m_uploadworker != NULL)
	{
		m_uploadworker->stopAll();
		delete m_uploadworker;
		m_uploadworker = NULL;
	}

	QMap<QString, UploadItem*>::iterator itor;
	for(itor = m_indexlist.begin(); itor != m_indexlist.end(); itor++)
		delete *itor;
}

void FileUploader::startAll()
{
	m_stop = false;
	m_scaner->start();
	m_uploadworker->startAll();
	startNextUpload(true);
}

void FileUploader::stopAll()
{
	m_stop = true;
	m_scaner->stop();
	m_uploadworker->stopAll();
}

void FileUploader::refreshItemStatus(UploadItem *iteminfo)
{
	QMutexLocker locker(&m_mutex);
	if(!m_indexlist.contains(iteminfo->filename))
		return;

	UploadItem *item = m_indexlist[iteminfo->filename];
	int index = m_seqlist.indexOf(item);
	if(index >= 0)
		m_seqlist.replace(index, iteminfo);

	delete m_indexlist[iteminfo->filename];
	m_indexlist[iteminfo->filename] = iteminfo;
	locker.unlock();

	emit signalItemStateChanged(RefreshItem, *iteminfo);


	if(iteminfo->status == Error || iteminfo->status == Failed)
		emit error(QString::fromLocal8Bit("File:%1\r\nInfo:%2").arg(iteminfo->filename).arg(iteminfo->speed));

	if(iteminfo->status == Failed)
		startNextUpload(false);
}

void FileUploader::fileStop(const QString &filename)
{
	if(!m_indexlist.contains(filename))
		return;

	UploadItem *item = m_indexlist[filename];
	item->bytesuploaded = 0;
	item->pecent = 0;
	item->speed = QString::fromLocal8Bit("停止上传");
	item->status = Pending;

	emit signalItemStateChanged(RefreshItem, *item);
}

void FileUploader::fileComplete(const QString &filename)
{
	QMutexLocker locker(&m_mutex);

	UploadItem *tagitem = checkFileContained(filename);
	if(tagitem == NULL)
		return;

	m_indexlist.remove(filename);
	m_seqlist.removeAll(tagitem);
	locker.unlock();

	//通知界面更新
	emit signalItemStateChanged(DeleteItem, *tagitem);
	delete tagitem;

	//文件重命名
	int nameindex = filename.lastIndexOf('.');
	if(nameindex == -1)
		nameindex = filename.length();
	QString newname(filename);
	newname.insert(nameindex, WEBLIB_TMP_FILENAME);
	bool ok = QFile::rename(filename, newname);
	if(!ok)
	{
		emit error(QString::fromLocal8Bit("文件%1重命名失败").arg(filename));
	}

	if(m_stop == false)
		startNextUpload(false);
}

UploadItem *FileUploader::checkFileContained(const QString &filepath)
{
	if(m_indexlist.contains(filepath))
		return m_indexlist[filepath];
	return NULL;
}

void FileUploader::addFile(const QString &filepath, int groupid, int parentid)
{
	QMutexLocker locker(&m_mutex);

	if(checkFileContained(filepath) != NULL)
		return;
	
	QFileInfo info(filepath);
	if(!info.exists())
		return;

	UploadItem *item = new UploadItem();
	item->filename = filepath;
	item->bytesuploaded = 0;
	item->groupid = groupid;
	item->parentid = parentid;
	item->pecent = 0;
	item->speed = QString::fromLocal8Bit("等待上传");
	item->size = info.size();
	item->status = Pending;

	m_indexlist.insert(filepath, item);
	m_seqlist.append(item);
	locker.unlock();

	emit signalItemStateChanged(AddItem, *item);

	startNextUpload(false);
}

void FileUploader::stopUpload(const QString &filepath)
{
	m_uploadworker->stopUploadFile(filepath);
}

bool FileUploader::removeFile(const QString &filepath)
{
	QMutexLocker locker(&m_mutex);
	UploadItem *item = checkFileContained(filepath);
	if(item == NULL)
		return false;

	//先停止文件上传
	this->stopUpload(filepath);
	m_indexlist.remove(filepath);
	m_seqlist.removeAll(item);
	locker.unlock();

	emit signalItemStateChanged(DeleteItem, *item);
	delete item;

	return true;
}

//搜索列表并上传文件，直至上传队列排满
void FileUploader::startNextUpload(bool erritem)
{
	QMutexLocker locker(&m_mutex);
	for(int i = 0; i < m_seqlist.size(); i++)
	{
		if(m_uploadworker->isFullQueue())
			break;

		UploadItem *item = m_seqlist.at(i);
		if(item->status == Pending 
			|| (erritem == true && (item->status == Error || item->status == Failed)))
		{
			ErrorInfo info = m_uploadworker->addUploadFile(item->filename, item->parentid, item->groupid);
			if(!info.error)
				item->status = Uploading;
			else
			{
				item->status = Error;
				item->speed = info.info;
			}
		}
	}
}