#include "ChannelTest.h"

#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>
#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>
#include <dbus/channel/type/streamedMedia/client/CvStreamedMediaIf.h>
#include <dbus/channel/type/testMedia/common/testMediaServiceCommon.h>
#include <dbus/channel/type/testMedia/client/CvTestMediaIf.h>
#include <dbus/msdx/common/MsdxServiceCommon.h>
#include <msdx/config.h>

void ChannelTest()
{
	{
		ChannelDispatcherIf*   channelDispatcherProxy = new ChannelDispatcherIf(
			CHANNEL_DISPATCHER_SERVICE_NAME, CHANNEL_DISPATCHER_OBJECT_PATH , QDBusConnection::sessionBus());

		/** ∑¢ÀÕ≤‚ ‘ */
		char remoterAddr[50];
		strcpy( remoterAddr, "127.0.0.1");
		SendGraphInfo info;
		info.initial( 0,VIDEOCAPTRUEDEVICE , AUDIO_INPUT_DEVICE , AUDIO_OUTPUT_DEVICE, MSDX_CONF_AUDIO_INPUT_NOAEC,
            remoterAddr, 9000,"SPEEX",
			16000, 16 , 1,true, remoterAddr , 9100, 25, true, 0 , 360 ,240, "" , "");
		QByteArray bytesArray;
		QDataStream out(&bytesArray , QIODevice::WriteOnly );
		out << info;


		if( channelDispatcherProxy->isValid() )
			channelDispatcherProxy->CreateChannel( QString("send"), QString(STREAMED_MEDIA_SERVICE_NAME) , bytesArray );

		//getchar();


		//if( channelDispatcherProxy->isValid() )
		//	channelDispatcherProxy->CreateChannel( QString("send"), QString(STREAMED_MEDIA_SERVICE_NAME) , bytesArray );

		//getchar();


		///*Ω” ’≤‚ ‘*/
		//RecvGraphInfo recvInfo;
		//recvInfo.initial( "OUTPUT" , "INPUT" , "127.0.0.1" , 8500 , "SPEEX" , 16000, 16 , 1 , "127.0.0.1" , 8600,
		//	0 , 640 , 480 , 30,"user1" ,"user1",true );
		//QByteArray recvByteArray;
		//QDataStream out2( &recvByteArray , QIODevice::WriteOnly );
		//out2 << recvInfo;
		//channelDispatcherProxy->CreateChannel( userID2MediaID("user1"),QString(STREAMED_MEDIA_SERVICE_NAME) , recvByteArray);

		////channelDispatcherProxy->ReleaseChannel(  userID2MediaID("user1"),QString(STREAMED_MEDIA_SERVICE_NAME) );
		//getchar();

		//channelDispatcherProxy->CreateChannel( userID2MediaID("user1"),QString(STREAMED_MEDIA_SERVICE_NAME) , recvByteArray);

		//getchar();
		//channelDispatcherProxy->ReleaseChannel(  userID2MediaID("user1"),QString(STREAMED_MEDIA_SERVICE_NAME) );

		//getchar();
		//channelDispatcherProxy->CreateChannel( userID2MediaID("user1"),QString(STREAMED_MEDIA_SERVICE_NAME) , recvByteArray);

		/*QByteArray modifyByteArray;
		QDataStream modifyStream( &modifyByteArray , QIODevice::WriteOnly );
		modifyStream << true;
		channelDispatcherProxy->ModifyChannel("",STREAMED_MEDIA_SERVICE_NAME, MMT_EnableAutoResync,modifyByteArray);

		getchar();

		QByteArray disableAudioBytes;
		QDataStream disableAudioStream( &disableAudioBytes , QIODevice::WriteOnly );
		disableAudioStream << false;
		CvStreamedMediaIf proxy( getStreamedMediaServiceName(userID2MediaID("user1")),getStreamedMediaObjectPath(userID2MediaID("user1")),
			QDBusConnection::sessionBus() );
		proxy.SetMediaInfo( Action_ControlAudioRecv ,disableAudioBytes );*/


		//ScreenMediaInfo mediaInfo;
		//mediaInfo._ipAddress = "127.0.0.1";
		//mediaInfo._port = 31829;
		//mediaInfo._screenWnd = 0;
		//QByteArray screenRecvBytes;
		//QDataStream out3( &screenRecvBytes , QIODevice::WriteOnly );
		//out3 << screenRecvBytes;
		//channelDispatcherProxy->CreateChannel( RECV_SCREEN_MEDIA_ID , SCREEN_MEDIA_SERVICE_NAME , screenRecvBytes );

		//QByteArray screenSendBytes;
		//QDataStream out4( &screenSendBytes , QIODevice::WriteOnly );
		//out4 << screenSendBytes;
		//channelDispatcherProxy->CreateChannel( SEND_SCREEN_MEDIA_ID , SCREEN_MEDIA_SERVICE_NAME , screenSendBytes );

		getchar();



		channelDispatcherProxy->Destroy();
	}
}

void AudioTestMediaTest()
{
	ChannelDispatcherIf*   channelDispatcherProxy = new ChannelDispatcherIf(
		CHANNEL_DISPATCHER_SERVICE_NAME, CHANNEL_DISPATCHER_OBJECT_PATH , QDBusConnection::sessionBus());

	TestMediaInfo info;
	info.audioIutputDevice="Microphone (e2eSoft VAudio)";
	//info.audioIutputDevice="C:\\Users\\Public\\Music\\Sample Music\\Kalimba.mp3";
	info.audioOutputDevice="Default DirectSound Device";

	QByteArray recvByteArray;
	QDataStream out( &recvByteArray , QIODevice::WriteOnly );
	out << info;

	channelDispatcherProxy->CreateChannel(DEVICE_TEST_MEDIA_ID, TEST_MEDIA_SERVICE_NAME, recvByteArray);
	//channelDispatcherProxy->CreateChannel(FILE_TEST_MEDIA_ID, TEST_MEDIA_SERVICE_NAME, recvByteArray);
	getchar();

	channelDispatcherProxy->ReleaseChannel(DEVICE_TEST_MEDIA_ID, TEST_MEDIA_SERVICE_NAME);
	//channelDispatcherProxy->CreateChannel(FILE_TEST_MEDIA_ID, TEST_MEDIA_SERVICE_NAME, recvByteArray);

}

void VideoTestMediaTest()
{
	ChannelDispatcherIf*   channelDispatcherProxy = new ChannelDispatcherIf(
		CHANNEL_DISPATCHER_SERVICE_NAME, CHANNEL_DISPATCHER_OBJECT_PATH , QDBusConnection::sessionBus());

	VideoPreviewInfo info;
	info.videoCaptureDevice = "e2eSoft VCam";
	//info.audioIutputDevice="C:\\Users\\Public\\Music\\Sample Music\\Kalimba.mp3";
	info.width = 320;
	info.height = 240;
	info.fps = 25;
	info.preview_wnd = NULL;

	QByteArray recvByteArray;
	QDataStream out( &recvByteArray , QIODevice::WriteOnly );
	out << info;

	channelDispatcherProxy->CreateChannel(VIDEO_PREVIEW_MEDIA_ID, TEST_MEDIA_SERVICE_NAME, recvByteArray);
	getchar();

	channelDispatcherProxy->ReleaseChannel(VIDEO_PREVIEW_MEDIA_ID, TEST_MEDIA_SERVICE_NAME);

}
