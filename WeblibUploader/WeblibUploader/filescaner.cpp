#include "filescaner.h"
#include "connectionmanager.h"
#include "weblibsetting.h"
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

FileScaner::FileScaner()
{
	m_manager = NULL;
	m_scaning = false;
	m_timer = new QTimer();
	m_timer->setInterval(FILE_SCAN_INTERVAL);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slotFileScan()));
}

FileScaner::~FileScaner()
{
	if(m_timer != NULL)
	{
		m_timer->stop();
		delete m_timer;
		m_timer = NULL;
	}
}

void FileScaner::start()
{
	slotFileScan();
	m_timer->start();
}

void FileScaner::stop()
{
	m_timer->stop();
}

QList<WebLibDirStruct *> *FileScaner::getRootDirList(int parentid)
{
	QString basedir(RELATIVE_FILE_PATH);
	if(basedir.startsWith("../"))
	{
		basedir = basedir.right(basedir.length() - 2);
		QDir curdir = QApplication::applicationDirPath();
		curdir.cdUp();
		basedir = curdir.path() + basedir;
	}
	else if(basedir.startsWith("/"))
	{
		basedir = QApplication::applicationDirPath() + basedir;
	}

	QDir rootdir(basedir);
	if(!rootdir.exists())
	{
		qDebug() << "Root Dir Not Exists";
		return NULL;
	}

	QList<WebLibDirStruct *> *list = new QList<WebLibDirStruct *>();

	QStringList *p = NULL;
	QStringList *dirlist = new QStringList();
	QStringList *tmplist = new QStringList();
	dirlist->append(rootdir.path());
	for(int i=0; i < SCAN_DIR_LEVEL; i++)
	{
		for(int j=0; j<dirlist->size(); j++)
		{
			QDir curdir(dirlist->at(j));
			if(!curdir.exists())
				continue;
			QStringList subdirlist = curdir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
			for(int k=0; k<subdirlist.size(); k++)
				tmplist->append(dirlist->at(j) + "/" + subdirlist.at(k));
		}
		p = dirlist;
		dirlist = tmplist;
		tmplist = p;
		tmplist->clear();
	}
	
	for(int i=0; i < dirlist->size(); i++)
		list->append(new WebLibDirStruct(dirlist->at(i), parentid, WEBLIB_GROUPID, parentid));

	delete tmplist;
	delete dirlist;
	return list;
}

qint64 FileScaner::getWeblibFreeSpace(int groupid)
{
	if(m_manager == NULL)
		return -1;

	QUrl url(WEBLIB_BASEURL + "/group/getResourceSize.action");
	QByteArray param(QString("groupId=%1").arg(groupid).toLatin1());
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkReply *reply = m_manager->post(request, param);
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	if(reply->error())
	{
		if(!reply->isFinished())
		{
			emit error(QString::fromLocal8Bit("获取Weblib剩余容量超时"));
		}
		else
		{
			QJsonParseError ok;
			QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();
			if(ok.error == QJsonParseError::NoError)
				emit error(result["detail"].toString());
			else
				emit error(reply->errorString());
		}
		reply->deleteLater();
		return -1;
	}

	QJsonParseError ok;
	QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();
	reply->deleteLater();
	if(ok.error != QJsonParseError::NoError)
		return -1;

	return (result["totalSize"].toDouble() - result["resourcesSize"].toDouble()) * 1024;
}


qint64 FileScaner::getWeblibSizeLimit(int groupid)
{
	if(m_manager == NULL)
		return -1;

	QUrl url(WEBLIB_BASEURL + "/group/getResourceSizeLimit.action");
	QByteArray param(QString("groupId=%1").arg(groupid).toLatin1());
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkReply *reply = m_manager->post(request, param);
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	if(reply->error())
	{
		if(!reply->isFinished())
		{
			emit error(QString::fromLocal8Bit("获取Weblib单文件大小限制超时"));
		}
		else
		{
			QJsonParseError ok;
			QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();
			if(ok.error == QJsonParseError::NoError)
				emit error(result["detail"].toString());
			else
				emit error(reply->errorString());
		}
		reply->deleteLater();
		return -1;
	}

	QJsonParseError ok;
	QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();
	reply->deleteLater();
	if(ok.error != QJsonParseError::NoError)
		return -1;

	return result["singleSize"].toDouble() * 1024;
}

QList<int> *FileScaner::getWeblibIdList(int parentid)
{
	if(m_manager == NULL)
		return NULL;

	QUrl url(WEBLIB_BASEURL + "/group/getResources.action");
	QByteArray param(QString("parentId=%1&type=tree").arg(parentid).toLatin1());
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkReply *reply = m_manager->post(request, param);
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	QTimer::singleShot(CREATE_DIR_TIMEOUT, &loop, SLOT(quit()));
	loop.exec();
	disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	if(reply->error() || !reply->isFinished())
	{
		if(!reply->isFinished())
		{
			emit error(QString::fromLocal8Bit("获取目录资源列表超时"));
		}
		else
		{
			QJsonParseError ok;
			QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();

			if(ok.error == QJsonParseError::NoError)
				emit error(result["detail"].toString());
			else
				emit error(reply->errorString());
		}
		reply->deleteLater();
		return NULL;
	}

	QJsonParseError ok;
	QJsonArray list = QJsonDocument::fromJson(reply->readAll(), &ok).array();
	reply->deleteLater();

	if(ok.error != QJsonParseError::NoError)
		return NULL;

	QList<int> *idlist = new QList<int>();
	for(int i=0; i<list.size(); i++)
	{
		QJsonObject item = list.at(i).toObject();
		idlist->append((int)item["id"].toDouble());
	}

	return idlist;
}

//根据相对路径创建目录树
int FileScaner::createWeblibDir(int groupid, int parentid, const QString &dirpath)
{
	if(m_manager == NULL)
		return -1;

	QDir dirinfo(dirpath);
	if(!dirinfo.exists())
		return -1;

	qDebug() << "create dir: " << groupid << ""  << parentid << " " << dirpath;
	QUrl url(WEBLIB_BASEURL + "/group/createDir.action");
	QByteArray param(QString("groupId=%1&parentId=%2&name=%3").arg(groupid).arg(parentid).arg(dirinfo.dirName()).toUtf8());
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkReply *reply = m_manager->post(request, param);
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	QTimer::singleShot(CREATE_DIR_TIMEOUT, &loop, SLOT(quit()));
	loop.exec();
	disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	if(reply->error() || !reply->isFinished())
	{
		if(!reply->isFinished())
		{
			emit error(QString::fromLocal8Bit("创建文件夹超时"));
		}
		else
		{
			QJsonParseError ok;
			QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();

			if(ok.error == QJsonParseError::NoError)
				emit error(result["detail"].toString());
			else
				emit error(reply->errorString());
		}
		reply->deleteLater();
		return -1;
	}

	QJsonParseError ok;
	QJsonObject obj = QJsonDocument::fromJson(reply->readAll(), &ok).object();
	reply->deleteLater();
	if(ok.error != QJsonParseError::NoError)
		return -1;

	//创建id记录文件
	QFile idfile(dirpath + "/" + WEBLIB_TMP_FILENAME);
	if(!idfile.open(QIODevice::ReadWrite | QIODevice::Truncate))
		return -1;
	idfile.write(QByteArray::number((int)obj["id"].toDouble()));
	idfile.close();

	return (int)obj["id"].toDouble();
}

QMap<QString, QList<int> *> *FileScaner::getLocalDirInfo(QList<WebLibDirStruct *> *rootlist)
{
	if(rootlist == NULL)
		return NULL;

	QMap<QString, QList<int> *> *idlist = new QMap<QString, QList<int> *>();
	QStack<QDir> sublist;
	int rootdirlen = 0;
	int id;
	for(int i=0; i<rootlist->size(); i++)
	{
		QDir dirinfo(rootlist->at(i)->dirPath);
		rootdirlen = dirinfo.path().length();
		sublist.push(dirinfo);
		while(!sublist.isEmpty())
		{
			QDir info = sublist.pop();
			QFile file(info.path() + "/" + WEBLIB_TMP_FILENAME);
			id = -1;
			if(file.exists())
			{
				QString relpath = info.path().right(info.path().length() - rootdirlen);
				file.open(QIODevice::ReadOnly);
				id = file.readAll().toInt();
				file.close();
				if(id >= 0)
				{
					if(!idlist->contains(relpath))
					{
						QList<int> *idset = new QList<int>();
						idlist->insert(relpath, idset);
					}
					(*idlist)[relpath]->append(id);
				}
			}
			QStringList subdirlist = info.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
			foreach (QString dname, subdirlist)
				sublist.push(info.path() + "/" + dname);
		}
	}
	return idlist;
}

int FileScaner::createUserDir(int groupid, const QString &dirname)
{
	if(m_manager == NULL)
		return -1;

	QUrl url(WEBLIB_BASEURL + "/group/createDir.action");
	QByteArray param(QString("groupId=%1&parentId=0&name=%3").arg(groupid).arg(dirname).toUtf8());
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkReply *reply = m_manager->post(request, param);
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	QTimer::singleShot(CREATE_DIR_TIMEOUT, &loop, SLOT(quit()));
	loop.exec();
	disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	if(reply->error() || !reply->isFinished())
	{
		if(!reply->isFinished())
		{
			emit error(QString::fromLocal8Bit("创建用户文件夹超时"));
		}
		else
		{
			QJsonParseError ok;
			QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();
			if(ok.error == QJsonParseError::NoError)
				emit error(result["detail"].toString());
			else
				emit error(reply->errorString());
		}
		reply->deleteLater();
		return -1;
	}

	QJsonParseError ok;
	QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();
	reply->deleteLater();
	if(ok.error != QJsonParseError::NoError)
		return -1;

	return (int)result["id"].toDouble();
}

bool FileScaner::checkUserDir(int &dirId)
{
	if(m_manager == NULL)
	{
		dirId = -1;
		return false;
	}

	QUrl url(WEBLIB_BASEURL + "/group/getResources.action");
	QByteArray param(QString("parentId=%1&type=tree").arg(-WEBLIB_GROUPID).toLatin1());
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkReply *reply = m_manager->post(request, param);
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	QTimer::singleShot(CREATE_DIR_TIMEOUT, &loop, SLOT(quit()));
	loop.exec();
	disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	if(reply->error() || !reply->isFinished())
	{
		if(!reply->isFinished())
		{
			emit error(QString::fromLocal8Bit("获取目录资源列表超时"));
		}
		else
		{
			QJsonParseError ok;
			QJsonObject result = QJsonDocument::fromJson(reply->readAll(), &ok).object();
			if(ok.error == QJsonParseError::NoError)
				emit error(result["detail"].toString());
			else
				emit error(reply->errorString());
		}
		reply->deleteLater();
		dirId = -1;
		return false;
	}

	QJsonParseError ok;
	QJsonArray list = QJsonDocument::fromJson(reply->readAll(), &ok).array();
	reply->deleteLater();

	if(ok.error != QJsonParseError::NoError)
	{
		dirId = -1;
		return false;
	}

	//找到用户文件夹
	for(int i=0; i<list.size(); i++)
	{
		QJsonObject item = list.at(i).toObject();
		if(item["text"].toString() == WEBLIB_ACCOUNT)
		{
			dirId = (int)item["id"].toDouble();
			return true;
		}
	}

	dirId = -1;
	return true;
}

//从上至下顺序构建目录树，从底向上查找parentid不为0的父节点，从该节点开始创建。
//若至根目录为止parentid依然为-1，则以根目录为起点进行创建，此时parentid为rootid
int FileScaner::checkAndCreateWeblibDirTree(int groupid, int parentid, const QString &dirpath, int rootid, int rootlen, QMap<QString, int> *idlist)
{
	if(idlist == NULL)
		return -1;

	QList<WebLibDirStruct *> dirlist;
	dirlist.append(new WebLibDirStruct(dirpath, parentid, groupid, -1));
	QString curpath = dirpath;
	//当查找到目录parentid不为-1时即停止
	while(parentid == -1)
	{
		int index = curpath.lastIndexOf("/");
		if(index >= 0 && index > rootlen)
		{
			curpath = curpath.left(index);
			QString prelpath = curpath.right(curpath.length() - rootlen);
			if(idlist->contains(prelpath))
				parentid = (*idlist)[prelpath];
			else
				parentid = -1;
		}
		else
			parentid = rootid;
		
		if(parentid == -1)
			dirlist.append(new WebLibDirStruct(curpath, parentid, groupid, -1));
		else
			dirlist.back()->parentid = parentid;
	}
	
	for(int i=dirlist.size() - 1; i >= 0; i--)
	{
		WebLibDirStruct *item = dirlist.at(i);
		item->weblibid = createWeblibDir(item->groupid, item->parentid, item->dirPath);
		if(item->weblibid == -1)
		{
			emit error(QString::fromLocal8Bit("创建目录%1失败").arg(item->dirPath));
			break;
		}

		QString relpath = item->dirPath.right(item->dirPath.length() - rootlen);
		QMap<QString, int>::iterator itor = idlist->find(relpath);
		if(itor != idlist->end())
			*itor = item->weblibid;
		else
			idlist->insert(relpath, item->weblibid);

		if(i > 0)
			dirlist.at(i-1)->parentid = item->weblibid;
	}

	int result = dirlist.at(0)->weblibid;
	for(int i=0; i<dirlist.size(); i++)
		delete dirlist.at(i);

	return result;
}

//本方法假设每次扫描的过程中weblib的目录结构不会变化，若在方法执行过程中通过其它客
//户端删除了weblib相应目录，则本方法无法处理
void FileScaner::slotFileScan()
{
	if(m_scaning == true)
		return;

	m_manager = ConnectionManager::getInstance().getAvailableManager();

	qint64 freespace = getWeblibFreeSpace(WEBLIB_GROUPID);
	if(freespace <= 0)
	{
		ConnectionManager::getInstance().releaseManager(m_manager);
		m_manager = NULL;
		emit error(QString::fromLocal8Bit("Weblib空间不足"));
		return;
	}

	qint64 singlelim = getWeblibSizeLimit(WEBLIB_GROUPID);
	if(singlelim <= 0)
		singlelim = LONG_MAX;

	m_scaning = true;

	int parentId = 0;
	if(WEBLIB_ISCREATEUSERDIR == true)
	{
		if(!checkUserDir(parentId))
		{
			ConnectionManager::getInstance().releaseManager(m_manager);
			m_manager = NULL;
			emit error(QString::fromLocal8Bit("查找Weblib用户文件夹失败"));
			return;
		}

		if(parentId < 0)
			parentId = createUserDir(WEBLIB_GROUPID, WEBLIB_ACCOUNT);

		if(parentId < 0)
		{
			ConnectionManager::getInstance().releaseManager(m_manager);
			m_manager = NULL;
			emit error(QString::fromLocal8Bit("创建Weblib用户文件夹失败"));
			return;
		}
	}

	QList<WebLibDirStruct*> *list = getRootDirList(parentId);
	if(list == NULL)
		return;

	//先获取本地已保存的id记录，防止重复创建相对路径相同的目录
	QMap<QString, QList<int> *> *localidset = getLocalDirInfo(list);
	
	//构建上传目录用的列表
	QMap<QString, int> *localidlist = new QMap<QString, int>();

	long curfilesize = 0;
	QList<int> *idlist = NULL;
	QStack<WebLibDirStruct *> dstack;
	int rootlen, rootid;
	//对根目录进行处理，可能同时存在多个根目录
	for(int i=0; i<list->size(); i++)
	{
		//根路径长度，使用深度优先遍历，用于计算目录的相对路径
		rootlen = list->at(i)->dirPath.length();
		rootid = list->at(i)->weblibid;
		dstack.push(list->at(i));

		while(!dstack.isEmpty())
		{
			WebLibDirStruct *curitem = dstack.pop();
			//获取weblib已有目录id列表
			if(curitem->weblibid != -1)
			{
				int tagid = curitem->weblibid == 0 ? -curitem->groupid : curitem->weblibid; 
				idlist = getWeblibIdList(tagid);
			}

			QDir curdir(curitem->dirPath);
			//检查所有子目录id对应项，构建目录树，为避免上传空目录，将创建目录动作置于文件检测过程
			QStringList sublist = curdir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
			for(int j=0; j < sublist.size(); j++)
			{
				int weblibid = -1;
				QString curpath = (curitem->dirPath + "/" + sublist.at(j));
				QString relpath = curpath.right(curpath.length() - rootlen);
				//解决相对路径相同问题
				if(localidlist->contains(relpath))
					weblibid = (*localidlist)[relpath];
				else
				{
					//若weblib无对应id记录则当前id无效，需重建目录
					if(idlist != NULL && localidset->contains(relpath))
					{
						QList<int> *idset = (*localidset)[relpath];
						for(int m=0; m<idset->size(); m++)
						{
							if(idlist->contains(idset->at(m)))
							{
								weblibid = idset->at(m);
								localidlist->insert(relpath, weblibid);
								break;
							}
						}
					}
				}

				qDebug() << "add dir list: " << curitem->groupid << " " << curitem->weblibid << " " << weblibid << " " << curitem->dirPath + "/" + sublist.at(j);
				WebLibDirStruct *item = new WebLibDirStruct(curitem->dirPath + "/" + sublist.at(j), curitem->weblibid, curitem->groupid, weblibid);
				dstack.push(item);
			}

			//上传目录文件
			QStringList filelist = curdir.entryList(QDir::Files);
			for(int j=0; j<filelist.size(); j++)
			{
				QFile fileinfo(filelist.at(j));

				if(fileinfo.size() > singlelim)
				{
					emit error(QString::fromLocal8Bit("超过单文件最大限制"));
					continue;
				}

				if(curfilesize + fileinfo.size() > freespace)
				{
					emit error(QString::fromLocal8Bit("Weblib空间不足"));
					continue;
				}

				//检查已上传文件标记
				if(filelist.at(j).indexOf(WEBLIB_TMP_FILENAME) != -1)
				{
					continue;
				}

				//检查上传目录id是否有效，无效则需创建新目录
				if(curitem->weblibid == -1)
				{
					curitem->weblibid = checkAndCreateWeblibDirTree(curitem->groupid, curitem->parentid, curitem->dirPath, rootid, rootlen, localidlist);
					if(curitem->weblibid == -1)
						continue;
				}

				curfilesize += fileinfo.size();
				emit addUploadFile(curitem->dirPath + "/" + filelist.at(j), curitem->groupid, curitem->weblibid);
			}

			delete curitem;

			if(idlist != NULL)
			{
				delete idlist;
				idlist = NULL;
			}
		}
	}
	delete list;

	QMap<QString, QList<int> *>::iterator itor;
	for(itor = localidset->begin() ; itor != localidset->end(); itor++)
		delete *itor;
	delete localidset;

	if(localidlist != NULL)
		delete localidlist;

	ConnectionManager::getInstance().releaseManager(m_manager);
	m_manager = NULL;
	m_scaning = false;
}