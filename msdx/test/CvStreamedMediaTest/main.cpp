#include <QtCore/QCoreApplication>
#include "ChannelTest.h"
#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>
#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>
#include <dbus/channel/type/streamedMedia/client/CvStreamedMediaIf.h>
#include <dbus/channel/type/testMedia/common/testMediaServiceCommon.h>
#include <dbus/channel/type/testMedia/client/CvTestMediaIf.h>
#include <dbus/msdx/common/MsdxServiceCommon.h>

#include "qrectimer.h"
#include <fstream>
#include <string>
#include "windows.h"


struct rtpInfo_t
{
	char remoteAddr[20];
	int videoPort;
	int audioPort;
	int videoWidth;
	int videoHeight;
	char recFile[256];

	RecvGraphInfo recvInfo;
};

const int RTP_COUNT = 100;
rtpInfo_t rtpInfo[RTP_COUNT] = {0};
int episode = 0;
int rtpCount = 1;
int timerInterval = 30000;
ChannelDispatcherIf * channelDispatcherProxy = NULL;


void QRecTimerHandler::timerEvent()
{
	++episode;
	printf_s("Creating video of episode %d...\n", episode);
	for (int i = 0; i < rtpCount; ++i)
	{
		char file[256] = {0};
		sprintf_s(file, sizeof(file), rtpInfo[i].recFile, episode);
		printf_s("Creating video file %s\n", file);

		QByteArray recvByteArray;
		QDataStream out( &recvByteArray , QIODevice::WriteOnly );
		out << QString(file);
		channelDispatcherProxy->ModifyChannel(getRecvMediaID(rtpInfo[i].recvInfo.user_id)
			, QString(STREAMED_MEDIA_SERVICE_NAME), MMT_CutRecordingVideo, recvByteArray);
	}
}


int main(int argc, char *argv[])
{

	QCoreApplication a(argc, argv);

	//ChannelTest();
	//AudioTestMediaTest();
	//VideoTestMediaTest();
	//return 0;

	// Input

	if (argc < 2)
	{
		printf_s("Too less arguments.\n"
			"Usage: timer_in_sec recv_count [ip video_port audio_port video_width video_height rec_file_name] ...\n"
			"e.g. CvStreamedMediaTest.EXE 2 127.0.0.1 10000 10100 320 240 TestVideo1-%03d.mp4 127.0.0.1 11000 11100 320 240 TestVideo2-%03d.mp4\n");
		printf_s("Looking for CvRecParams.txt...\n");

		std::ifstream  fin("CvRecParams.txt", std::ios::in);  
		if (!fin)
		{
			printf_s("No CvRecParams.txt found. exit!\n");
			return 0;
		}

		char  line[1024]={0};  
		int linesize = 0;

		fin.getline(line, sizeof(line));
		sscanf_s(line, "%d %d", &timerInterval, &rtpCount);
		timerInterval *= 1000;
		printf_s("Timer set to %ds\n", timerInterval/1000);

		if (rtpCount > RTP_COUNT || rtpCount <= 0)
		{
			printf_s("recv_count must between 0 and %d\n", RTP_COUNT);
			return 1;
		}

		for (int i = 0; i < rtpCount; ++i)
		{
			if (!fin.getline(line, sizeof(line)))
			{
				printf_s("Insufficient parameters. Please check CvRecParams.txt.\n");
				return 1;
			}
			sscanf_s(line, "%s %d %d %d %d %s", rtpInfo[i].remoteAddr, sizeof(rtpInfo[i].remoteAddr)
				, &rtpInfo[i].videoPort, &rtpInfo[i].audioPort, &rtpInfo[i].videoWidth, &rtpInfo[i].videoHeight
				, rtpInfo[i].recFile, sizeof(rtpInfo[i].recFile));
		}

		fin.clear();
		fin.close();
	}
	else
	{
		timerInterval = atoi(argv[1]) * 1000;
		printf_s("Timer set to %ds\n", timerInterval/1000);

		rtpCount = atoi(argv[2]);
		if (rtpCount > RTP_COUNT || rtpCount <= 0)
		{
			printf_s("recv_count must between 0 and 4\n");
			return 1;
		}

		const int PARAM_INDEX_BASE = 3;

		for (int i = 0; i < rtpCount; ++i)
		{
			strcpy(rtpInfo[i].remoteAddr, argv[PARAM_INDEX_BASE + i * 6]);
			rtpInfo[i].videoPort = atoi(argv[PARAM_INDEX_BASE + 1 + i * 6]);
			rtpInfo[i].audioPort = atoi(argv[PARAM_INDEX_BASE + 2 + i * 6]);
			rtpInfo[i].videoWidth = atoi(argv[PARAM_INDEX_BASE + 3 + i * 6]);
			rtpInfo[i].videoHeight = atoi(argv[PARAM_INDEX_BASE + 4 + i * 6]);
			strcpy(rtpInfo[i].recFile, argv[PARAM_INDEX_BASE + 5 + i * 6]);
		}
	}

	// Running

	channelDispatcherProxy = new ChannelDispatcherIf(
		CHANNEL_DISPATCHER_SERVICE_NAME, CHANNEL_DISPATCHER_OBJECT_PATH , QDBusConnection::sessionBus());

	while (true)
	{
		for (int i = 0; i < rtpCount; ++i)
		{
			char userId[16] = {0};
			sprintf_s(userId, sizeof(userId), "User%02d", i);

			char file[256] = {0};
			sprintf_s(file, sizeof(file), rtpInfo[i].recFile, episode);

			printf_s("Creating channel of %s: IP:%s V:%d A:%d @ %dx%d\n\t Recording %s\n"
				, userId, rtpInfo[i].remoteAddr, rtpInfo[i].videoPort, rtpInfo[i].audioPort, rtpInfo[i].videoWidth, rtpInfo[i].videoHeight
				, rtpInfo[i].recFile);

			rtpInfo[i].recvInfo.initial( "OUTPUT" , "INPUT" , rtpInfo[i].remoteAddr , rtpInfo[i].audioPort , "SPEEX" , 16000, 16 , 1 
				, rtpInfo[i].remoteAddr, rtpInfo[i].videoPort, NULL , rtpInfo[i].videoWidth , rtpInfo[i].videoHeight , 30
				, userId, userId, true, file);
      rtpInfo[i].recvInfo.audio_info.enable = true;

			QByteArray recvByteArray;
			QDataStream out( &recvByteArray , QIODevice::WriteOnly );
			out << rtpInfo[i].recvInfo;
			channelDispatcherProxy->CreateChannel( getRecvMediaID(userId), QString(STREAMED_MEDIA_SERVICE_NAME) , recvByteArray);
			Sleep(200);
			printf_s("[OK]\n");
		}


		//自动分割计时器，在线程QThread中运行;
		QRecTimerHandler handler;
		if (timerInterval > 0)
		{
			handler.startTimer(timerInterval);
			printf_s("Timer started.\n");
		}

        {
            getchar();
            // 测试控制台显示与隐藏
            QByteArray recvByteArray;
            QDataStream out( &recvByteArray , QIODevice::WriteOnly );
            out << false;
            channelDispatcherProxy->ModifyChannel("", "", MMT_ShowConsoleWindow, recvByteArray);

            getchar();

            recvByteArray.clear();
            out << true;
            channelDispatcherProxy->ModifyChannel("", "", MMT_ShowConsoleWindow, recvByteArray);
        }


		printf_s("Press q to quit, other key to release channel...\n");
		char input = getchar();

		//handler.stopTimer();

		if ('q' == input)
		{
			break;
		}

		for (int i = 0; i < rtpCount; ++i)
		{
			printf_s("Releasing channel of %s\n", qPrintable(rtpInfo[i].recvInfo.user_id));
			channelDispatcherProxy->ReleaseChannel(getRecvMediaID(rtpInfo[i].recvInfo.user_id), QString(STREAMED_MEDIA_SERVICE_NAME));
			printf_s("[OK]\n");
		}

		printf_s("Press q to quit, other key to create channel again...\n");
		input = getchar();
		if ('q' == input)
		{
			break;
		}
	}

	channelDispatcherProxy->Destroy();
	printf_s("You can close this console now.");

	a.exec(); // 为方便操作，计时器在单独线程中运行，不需要主线程处理其时间，见qrectimer实现 - Liaokz
}
