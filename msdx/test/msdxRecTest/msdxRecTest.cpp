// msdxRecvTest.cpp : 定义控制台应用程序的入口点。
//

#include <conio.h>
#include <windows.h>
#include <assert.h>
#include <iostream>
using namespace std;
#include "exportutil.h"
#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"
#include "msdx/config.h"
//#include "FtpFileTrans.h"

//调用第三方工具优化文件结构用的，保留做测试
//int interleavingFile( const char * szPath )
//{
//	if (NULL == szPath || strlen(szPath) == 0)
//	{
//		return -1;
//	}
//
//	// Handle the capture file for fast pseudo-streaming
//	std::string strPostHandleProgramPath;
//	if (FALSE == CConfigMgr::GetValue(CConfigMgr::CFG_RECORD_OPTIMIZE_TOOL, CConfigMgr::TYPE_STRING, &strPostHandleProgramPath) ||
//		strPostHandleProgramPath.empty())
//	{
//		return -1;
//	}
//
//	if (PathExist(strPostHandleProgramPath))
//	{
//		strPostHandleProgramPath = GetAbsolutePath(strPostHandleProgramPath);
//		std::string strParam;
//		if (FALSE == CConfigMgr::GetValue(CConfigMgr::CFG_RECORD_OPTIMIZE_TOOL_PARAM, CConfigMgr::TYPE_STRING, &strParam) ||
//			strParam.empty())
//		{
//			return -1;
//		}
//
//		// print cmd line template
//		char szCmdLineTemp[1024] = {0};
//		sprintf_s(szCmdLineTemp, sizeof(szCmdLineTemp)-1, "\"%s\" %s"
//			, strPostHandleProgramPath.c_str()
//			, strParam.c_str());
//
//		// print cmd line param
//		char szCmdLine[1024] = {0};
//		sprintf_s(szCmdLine, sizeof(szCmdLine)-1, szCmdLineTemp, szPath);
//
//		STARTUPINFO si;
//		PROCESS_INFORMATION pi;
//
//		ZeroMemory( &si, sizeof(si) );
//		si.cb = sizeof(si);
//		ZeroMemory( &pi, sizeof(pi) );
//
//		int ret = CreateProcess(NULL, szCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
//
//		if (!ret)
//		{
//			return -1;
//		}
//		else
//		{
//			WaitForSingleObject( pi.hProcess, INFINITE );
//			CloseHandle( pi.hProcess );
//			CloseHandle( pi.hThread );
//		}
//	}
//	else
//	{
//		return -1;
//	}
//
//	return 0;
//}

void recordCallback(void * obj, RecStatItem &stat)
{
    if (stat.statType == REC_STAT_FILE_START) {
        fprintf_s(stderr, "** Start[%d]: %s\n", stat.rec.job_id, stat.rec.file);
    }
    else if (stat.statType == REC_STAT_FILE_WRITING) {
        fprintf_s(stderr, "** Recording[%d]: %I64ds, %I64dKB      \r", 
          stat.rec.job_id, stat.rec.duration/1000, stat.rec.size/1000);
    }
    else if (stat.statType == REC_STAT_FILE_CLOSE) {
        fprintf_s(stderr, "** Close[%d]: %s\n"
          "\tvideo: %dx%d@%dfps, audio: %dHz %dch %dbits\n"
          "\tstart: %I64d, dur: %I64dms\n", 
          stat.rec.job_id,
          stat.close.file, 
          stat.close.video_width, stat.close.video_height, stat.close.video_fps,
          stat.close.audio_sample_rate, stat.close.audio_channel, stat.close.audio_bits_count,
          stat.close.start_time, stat.close.duration);
    }
    else if (stat.statType == REC_STAT_MUX_STOP) {
        fprintf_s(stderr, "** Mux stopped\n");
    }
}


int main(int argc, char* argv[])
{
	char *localAddr;//监听地址
	int videoWidth = 640, videoHeight = 480, videoPort = 10000, audioPort = 10100;
	int sampleRate = 16000, channel = 1;
  char *audioCodec = MSDX_CONF_AUDIO_CODEC_SPEEX;

	if(argc < 2)
		localAddr = "127.0.0.1";
	else if (argc == 9)
	{
    localAddr = argv[1];  //可以是组播地址
    videoPort = atoi(argv[2]);
    audioPort = atoi(argv[3]);
		videoWidth = atoi(argv[4]);
		videoHeight = atoi(argv[5]);
    sampleRate = atoi(argv[6]);
    channel = atoi(argv[7]);
    audioCodec = argv[8];

    if (strcmp(audioCodec, MSDX_CONF_AUDIO_CODEC_AAC) != 0 &&
      strcmp(audioCodec, MSDX_CONF_AUDIO_CODEC_SPEEX) != 0) {
        printf_s("Error: Invalid audio codec: %s.\n", audioCodec);
        return 1;
    }
	}

	//初始化
	int result;
	result = msdx_initial();

	//创建大流的接受端
	int graphid=-1;
	const char* localAddress = "127.0.0.1";
	
	//创建大流的接受端
	//vedio graph
	assert( (graphid = msdx_create_recv_graph("rec_test"))>0 );
	
	// 必须先设置以区分类型
	int job_id = 0;
	char fileName[256] = {0};
	sprintf_s(fileName, sizeof(fileName)-1, "rec\\%s_%dk_%dch_{episode}_{vinfo}.mp4", audioCodec, sampleRate/1000, channel);
	msdx_set_record_file_name(graphid, fileName);

	result = msdx_set_recv_video_codec(graphid, MSDX_CONF_VIDEO_CODEC_H264, videoWidth,videoHeight, 25, 16);
	result = msdx_set_recv_audio_codec(graphid, audioCodec, sampleRate, channel, 16);

	assert(result==0);

	result = msdx_set_recv_address(graphid,localAddress, audioPort, localAddress, videoPort);

  result = msdx_set_record_jobid(graphid, job_id++);

	result = msdx_connect_recv_graph(graphid);
    assert(result==0);

    result = msdx_set_record_callback(graphid, NULL, (void *)recordCallback);

	result = msdx_run_recv_graph(graphid);


	char c;
	int audioRecvSwitch = 1;
	int videoRecvSwitch = 1;

	while ((c=getch()) != 13) 
	{
		switch (c)
		{
		case 'c':
			//sprintf_s(fileName, sizeof(fileName)-1, "rec\\msdxRecTest_{episode}_{vinfo}.mp4", ++episode);
			printf_s("Creating new file %s\n" ,fileName);
			msdx_record_cut_file(graphid, job_id++, fileName);
			break;
		case 'a':
			printf_s("Control audio recv: %d\n", ++audioRecvSwitch%2);
			msdx_control_audio_recv(graphid, audioRecvSwitch%2==1);
			break;
		case 'v':
			printf_s("Control video recv: %d\n", ++videoRecvSwitch%2);
			msdx_control_video_recv(graphid, videoRecvSwitch%2==1);
			break;
		default:
			printf_s("Unknown input %d\n", c);
		}
	}

	//销毁接收
	msdx_stop_recv_graph( graphid );
	msdx_destroy_recv_graph( graphid );

	//结束
	msdx_uninitial();

	printf_s("Press any key to exit...");
	getch();

	return 0;
}