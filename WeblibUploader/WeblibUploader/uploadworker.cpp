#include "uploadworker.h"
#include "connectionmanager.h"
#include "weblibsetting.h"
#include <QDebug>
#include <QSet>
#include <QList>
#include <QTimer>

UploadWorker::UploadWorker()
{
	m_uploading = false;
	m_stop = false;
}

UploadWorker::~UploadWorker()
{
	
}

void UploadWorker::startAll()
{
	m_stop = false;
	scheduleTransfer();
}

void UploadWorker::stopAll()
{
	m_stop = true;
}

ErrorInfo UploadWorker::addUploadFile(const QString &filepath, int parentid, int groupid)
{
	ErrorInfo info;

	if(m_uploadlist.size() >= UPLOAD_THREAD_LIMIT)
	{
		info.error = true;
		info.info = QString::fromLocal8Bit("上传队列已满");
		return info;
	}

	qDebug() << "worker addfile :" << filepath;

	UploadOperator *oper = new UploadOperator(filepath, parentid, groupid);
	if(!oper->initializeConnect(ConnectionManager::getInstance().getCookies()))
	{
		oper->closeUploader();
		info.error = true;
		info.info = oper->getErrorInfo();
		delete oper;
		return info;
	}

	m_uploadlist.append(oper);
	connect(oper, SIGNAL(refreshStatus(UploadOperator*)), this, SLOT(refreshStatus(UploadOperator*)));
	connect(oper, SIGNAL(uploadFinished(bool,bool, UploadOperator *)),
		this, SLOT(uploadFinished(bool,bool, UploadOperator *)));
	connect(oper, SIGNAL(readyToTransfer()), this, SLOT(scheduleTransfer()));

	info.error = false;
	info.info = "OK";
	return info;
}

void UploadWorker::stopUploadFile(const QString &filepath)
{
	qDebug() << "worker stop file: " << filepath;

	for(int i=0; i<m_uploadlist.size(); i++)
	{
		if(m_uploadlist.at(i)->getFileName() == filepath)
		{
			uploadFinished(false, true, m_uploadlist.at(i));
			break;
		}
	}
}

void UploadWorker::scheduleTransfer()
{
	if(m_uploading == true || m_stop == true)
		return;
	m_uploading = true;
	QTimer::singleShot(50, this, SLOT(transfer()));
}

void UploadWorker::transfer()
{
	m_uploading = false;
	qint64 mesc = 1000;
	if(stopWatch.isValid())
		mesc = qMin(mesc, stopWatch.elapsed());

	qint64 speed = UPLOAD_SPEED_LIMIT <= 0 ? 0x7FFFFFFF : UPLOAD_SPEED_LIMIT;
	qint64 bytestowrite = (double)(speed * mesc) / 1000;
	if(bytestowrite == 0)
	{
		scheduleTransfer();
		return;
	}

	QSet<UploadOperator *> pendinglist;
	foreach(UploadOperator *oper, m_uploadlist)
	{
		if(oper->canTransferMore())
			pendinglist.insert(oper);
	}

	if(pendinglist.isEmpty())
		return;
	stopWatch.start();

	bool canTransferMore, canContinue;
	do 
	{
		canTransferMore = false;
		canContinue = false;
		qint64 writeChunk = qMax(qint64(1), bytestowrite / pendinglist.size());
		QSetIterator<UploadOperator*> itor(pendinglist);
		while(itor.hasNext() && (bytestowrite > 0))
		{
			UploadOperator *item = itor.next();
			if(speed * 2 > item->bytesToWrite())
			{
				qint64 buffersize = qMin(writeChunk, bytestowrite);
				qint64 sendsize = qMin(buffersize, speed * 2 - item->bytesToWrite());
				if(sendsize > 0)
				{
					bool bufferfull = false;
					qint64 sendedsize = item->sendBuffer(sendsize, bufferfull);
					if(sendedsize > 0)
					{
						bytestowrite -= sendedsize;
						canContinue = true;
					}
					if(bufferfull == true)
						canContinue = false;
				}
				item->updateStatus(sendsize);
			}
			else
				item->updateStatus(0);

			if(item->canTransferMore())
				canTransferMore = true;
			else
				pendinglist.remove(item);
		}
	} while (canTransferMore && canContinue && bytestowrite > 0 && !pendinglist.isEmpty());
	
	if(canTransferMore)
		scheduleTransfer();
}

void UploadWorker::refreshStatus(UploadOperator *sender)
{
	UploadItem *newstate = new UploadItem();
	newstate->filename = sender->getFileName();
	newstate->size = sender->getSize();
	newstate->groupid = sender->getGroupId();
	newstate->parentid = sender->getParentId();
	newstate->bytesuploaded = sender->getBytesUploaded();
	newstate->pecent = sender->getPecent();
	newstate->lefttime = sender->getLeftTime();
	newstate->speed = sender->getSpeed();
	newstate->status = sender->getStatus();
	emit reportStatus(newstate);
}

void UploadWorker::uploadFinished(bool error, bool abored, UploadOperator *sender)
{
	if(error == false)
	{
		QString filename = sender->getFileName();
		disconnect(sender, SIGNAL(refreshStatus(UploadOperator*)), this, SLOT(refreshStatus(UploadOperator*)));
		disconnect(sender, SIGNAL(uploadFinished(bool, bool, UploadOperator *)),
			this, SLOT(uploadFinished(bool, bool, UploadOperator *)));
		disconnect(sender, SIGNAL(readyToTransfer()), this, SLOT(scheduleTransfer()));
		m_uploadlist.removeAll(sender);
		sender->closeUploader();
		sender->deleteLater();
		if(abored == false)
		{
			qDebug() << "worker file finished: " << filename;
			emit uploadComplete(filename);
		}
		else
		{
			qDebug() << "worker file stop: " << filename;
			emit uploadStop(filename);
		}
	}
	else
	{
		qDebug() << "worker file error: " << sender->getFileName();

		//发生错误时的处理，newstate对象的释放由fileuploader负责，因此此处不执行delete
		UploadItem *newstate = new UploadItem();
		newstate->filename = sender->getFileName();
		newstate->size = sender->getSize();
		newstate->bytesuploaded = sender->getBytesUploaded();
		newstate->pecent = sender->getPecent();
		newstate->parentid = sender->getParentId();
		newstate->groupid = sender->getGroupId();
		newstate->lefttime = sender->getLeftTime();
		newstate->speed = sender->getSpeed();
		newstate->status = sender->getStatus();
		int retimes = sender->getRetrys();
		if(retimes >= UPLOAD_RETRY_TIMES)
		{
			//重试次数超过阈值，放弃此文件
			newstate->status = Failed;

			disconnect(sender, SIGNAL(uploadFinished(bool, bool, UploadOperator *)),
				this, SLOT(uploadFinished(bool, bool, UploadOperator *)));
			disconnect(sender, SIGNAL(refreshStatus(UploadOperator *)),
				this, SLOT(refreshStatus(UploadOperator *)));
			disconnect(sender, SIGNAL(readyToTransfer()), this, SLOT(scheduleTransfer()));
			
			m_uploadlist.removeAll(sender);
			sender->closeUploader();
			sender->deleteLater();
		}
		else
		{
			//重传
			QTimer::singleShot(10000, sender, SLOT(restartUpload()));
		}
		emit reportStatus(newstate);
	}
}