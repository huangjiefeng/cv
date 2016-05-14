#include <dbus/core/client/CvCoreIf.h>
#include <dbus/core/common/CvCoreServiceCommon.h>
#include "dbus/performance/monitor/common/CvPerfMonitorServiceCommon.h"

#include "MediaMonitorThread.h"


MediaMonitorThread::MediaMonitorThread()
{
	this->_endMark=false;
}

void MediaMonitorThread::run()
{
	while(!_endMark )
	{

		int processNum = this->mediaMonitoredProcessList.size();
		for(int i=0;i<processNum;i++)
		{
			int processid = mediaMonitoredProcessList[i]->processID;
			//如果当前进程不存在，需要重启进程
			if(!_perforUtil.isProcessRunningByID(processid))
			{
				if(mediaMonitoredProcessList[i]->mediaType == STREAM_MEDIA_RECV)
				{
	
						MediaMonitoredProcess* mProcess = mediaMonitoredProcessList[i];
						
						mediaMonitoredProcessList.remove(i);

						QByteArray output_bytes;
						QDataStream out( &output_bytes, QIODevice::WriteOnly );
						out.setVersion(QDataStream::Qt_4_4 );
						out<< *mProcess;
						CvCoreIf coreProxy( CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , QDBusConnection::sessionBus() );
						coreProxy.TeleCommand(CoreAction_RecoveryMeidaProcess,output_bytes);

				}
			}

		}

		
		//监控线程睡眠10秒
		msleep( 10000 );
	}
	_endMark = true;
}

int MediaMonitorThread::searchMeidaMonitoredProcess(int& processid)
{
	if(mediaMonitoredProcessList.isEmpty())
	{
		return -1;
	}

	for(int i = 0; i < mediaMonitoredProcessList.size() ; i ++)
	{
		if(mediaMonitoredProcessList.at(i)->processID == processid)
		{
			return i;
		}
	}

	 return -1;
}

void MediaMonitorThread::addMediaMonitoredProcess(MediaMonitoredProcess &mediaProcess)
{
	int index = searchMeidaMonitoredProcess(mediaProcess.processID);

	if(index>=0)
	{
		return;
	}
	else
	{
		MediaMonitoredProcess * mProcess = new MediaMonitoredProcess();
		*mProcess =mediaProcess;
		mediaMonitoredProcessList.push_back(mProcess);
	}
}

void MediaMonitorThread::removeMediaMonitoredProcess(int &processID )
{
	int index = searchMeidaMonitoredProcess(processID);
	
	if(index >=0)
	{
		MediaMonitoredProcess * mProcess =mediaMonitoredProcessList[index];
		mediaMonitoredProcessList.remove(index);
		delete mProcess;
	}
	else
	{
	   return;
	}

}