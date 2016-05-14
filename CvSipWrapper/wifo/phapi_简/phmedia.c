/*
 * phmedia -  Phone Api media streamer
 *
 * Copyright (C) 2005-2006 WENGO SAS
 * Copyright (C) 2004 Vadim Lebedev <vadim@mbdsys.com>
 * Copyright (C) 2002,2003   Aymeric Moizard <jack@atosc.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with dpkg; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "phglobal.h"
#include "phlog.h"
#include <osip2/osip_mt.h>
#include <osip2/osip.h>
#ifdef OS_POSIX
#include <sys/ioctl.h>
#include <sys/time.h>
#endif

#include <fcntl.h>
#include <stdlib.h>
#include <ortp.h>
#include <telephonyevents.h>
#include "phapi.h"
#include "phcall.h"
#include "phmedia.h"

//#include "phcodec.h"		编解码无用，zhenHua.sun 2010-06-28
#include "mxcodec.h"		//zhenHua.sun 2010-06-28
//#include "phmstream.h"	无用,zhenHua.sun 2010-06-28

////////////////
//#include "phastream.h"	无用，zhenHua.sun 2010-06-28
#include "phmedia.h"
#include <assert.h>
#include <glib.h>
#include <glib/ghash.h>
////////////////


#ifdef _MSC_VER
#define strncasecmp strnicmp
#endif

#include "phapi-old.h"
#include "msdx0.1\ExportUtil.h"

GHashTable *graph_hwnd_map=NULL;


//std::map<HWND,int> graph_hwnd_map;
/************************************************************************///Add by zhuang 08-12-23
void parse_fmtp(char *fmtp_str,int *width,int *height,int *bitrate)
{
	char *temp;
	char *temp2;
	char *width_str;
	char *height_str;
	char *bitrate_str;
	assert(width && height && bitrate);
	if(fmtp_str==NULL)
	{
		//return default value
		*width=176;
		*height=144;
		*bitrate=1960;
		return ;
	}
	temp = strstr(fmtp_str,"CUSTOM");
	if (temp)
	{
		while (*temp!='\r'&&*temp!='\n'&&*temp!='\0'&&*temp!='=')
		{
			temp++;
		}
		temp++;
		temp2 = temp;
		while (*temp2!='\r'&&*temp2!='\n'&&*temp2!='\0'&&*temp2!=',')
		{
			temp2++;
		}
		temp2++;
		width_str = malloc(10);
		memset(width_str,0,10);
		strncpy(width_str,temp,temp2-1-temp);

		temp=temp2;
		while (*temp2!='\r'&&*temp2!='\n'&&*temp2!='\0'&&*temp2!=' '&&*temp2!=',')
		{
			temp2++;
		}
		height_str = malloc(10);
		memset(height_str,0,10);
		strncpy(height_str,temp,temp2-temp);
		*width = atoi(width_str);
		*height = atoi(height_str);
	}
	else if (temp = strstr(fmtp_str,"QCIF"))
	{
		*width = 176;
		*height =144;
	}
	else if (temp = strstr(fmtp_str,"CIF"))
	{
		*width = 352;
		*height =288;
	}
	else
	{
		*width = 176;
		*height = 144;
	}

	if (temp = strstr(fmtp_str,"MaxBR"))
	{
		while (*temp!='\r'&&*temp!='\n'&&*temp!='\0'&&*temp!='=')
		{
			temp++;
		}
		temp++;
		temp2 = temp;
		while (*temp2!='\r'&&*temp2!='\n'&&*temp2!='\0'&&*temp2!=' ')
		{
			temp2++;
		}
		bitrate_str = malloc(10);
		memset(bitrate_str,0,10);
		strncpy(bitrate_str,temp,temp2-temp);
		*bitrate = atoi(bitrate_str);
	}
	else
		*bitrate = 1960;
}
/************************************************************************/


int ph_transport_common_sendto(ph_rtp_transport_data_t *data, OWSLSocket sock, 
							   const void *buffer, size_t len, int flags, 
							   const struct sockaddr *to, socklen_t tolen)
{
	int error = 0;
	char * buf_tmp;
	int len_tmp, sent;

	buf_tmp = malloc(len);
	memcpy(buf_tmp, buffer, len);
	len_tmp = len;

	if (data->rtp_pre_send_cb)
	{
		data->rtp_pre_send_cb(data->externalID, &error, buf_tmp, &len_tmp);
	}

	sent = owsl_send(sock, buf_tmp, len_tmp, 0);

	free(buf_tmp);

	return len_tmp == sent ? len : -1;
}

int ph_rtp_transport_sendto(struct _RtpTransport *t, const void *buffer, size_t len, int flags,
							const struct sockaddr *to, socklen_t tolen)
{
	ph_rtp_transport_data_t *data = t->data;

	if (!data)
	{
		return -1;
	}

	return ph_transport_common_sendto(data, data->rtp_sock, buffer, len, flags, to, tolen);
}

int ph_rtcp_transport_sendto(struct _RtpTransport *t, const void *buffer, size_t len, int flags,
							 const struct sockaddr *to, socklen_t tolen)
{
	ph_rtp_transport_data_t *data = t->data;

	if (!data)
	{
		return -1;
	}

	return ph_transport_common_sendto(data, data->rtcp_sock, buffer, len, flags, to, tolen);
}

int ph_transport_common_recvfrom(ph_rtp_transport_data_t *data, OWSLSocket sock,
								 void *buffer, size_t len, int flags, 
								 struct sockaddr *from, socklen_t *fromlen)
{
	int i = 0;
	int ret = 0;
	int error = 0;
	
	i = owsl_recv(sock, buffer, len, 0);
	if (i < 1)
	{
		return 0;
	}

	if (data->rtp_post_recv_cb)
	{
		data->rtp_post_recv_cb(data->externalID, &error, buffer, &i);
	}

	return i;
}

int ph_rtp_transport_recvfrom(struct _RtpTransport *t, void *buffer, size_t len, int flags, 
							  struct sockaddr *from, socklen_t *fromlen)
{
	ph_rtp_transport_data_t *data = t->data;

	if (!data)
	{
		return -1;
	}

	return ph_transport_common_recvfrom(data, data->rtp_sock, buffer, len, flags, from, fromlen);
}

int ph_rtcp_transport_recvfrom(struct _RtpTransport *t, void *buffer, size_t len, int flags, 
							   struct sockaddr *from, socklen_t *fromlen)
{
	ph_rtp_transport_data_t *data = t->data;

	if (!data)
	{
		return -1;
	}

	return ph_transport_common_recvfrom(data, data->rtcp_sock, buffer, len, flags, from, fromlen);
}

int ph_media_cleanup()
{
	/************************************************************************/
	msdx_uninitial();
	/*销毁hashtable*/
	if(graph_hwnd_map!=NULL)
	{
		g_hash_table_destroy(graph_hwnd_map);
		graph_hwnd_map = NULL;					//added by zhenHua.sun 2010-06-22
	}
	
	/************************************************************************/
  return 0;
}

int ph_msession_start_2(struct ph_msession_s *s, const char *deviceid)
{
  int ret1,ret2 = -1;
  int result;
  
  DBG_MEDIA_ENGINE("MEDIA_ENGINE: entering ph_msession_start\n");

  pthread_mutex_lock(&s->critsec_mstream_init);

/**
   * 编解码无用，删除   zhenHua.sun 2010-06-23
  ret1 = ph_msession_audio_start(s, deviceid);

#ifdef PHAPI_VIDEO_SUPPORT
  ret2 = ph_msession_video_start(s, deviceid);
#endif 
 */
  pthread_mutex_unlock(&s->critsec_mstream_init);

  if (!ret1 && !ret2)
  {
    return 0;
  }

  return 0;
  return ret1 ? ret1 : ret2;
}

/************************************************************************///Add by zhuang 08-12-29
/*resize回调函数                                                        */
void ph_video_resize(HWND hwnd,int width,int height,int recvgraphid,int fullscreenflag)
{
	//if(msdx_get_send_graph()==0)
	//	msdx_set_send_preview_window(hwnd,width*2/3,height*2/3,width/3,height/3,0);
	if(recvgraphid>0)
		msdx_set_recv_display_window(recvgraphid,hwnd,0,0,width,height,fullscreenflag);
}
/************************************************************************/
int ph_msession_start(struct ph_msession_s *s, const char *deviceid)
{
	int ret1,ret2 = -1;
	int result;
	phConfig_t *cfg = phGetConfig();

	/*变量声明*/
	//phastream_t *stream;		无用，注释掉　zhenHua.sun　2010-06-28
	int codecpt;
	ph_mstream_params_t *sp_audio = &s->streams[PH_MSTREAM_AUDIO1];
	ph_mstream_params_t *sp_video = &s->streams[PH_MSTREAM_VIDEO1];
	int newstreams;
	int recvgraphid;
	int remote_video_width=0,
		remote_video_height=0,
		remote_video_bitrate=0;
	RtpProfile *profile;
	PayloadType *pt ;
	int already_sending=0;
	//设置回调函数struct//Add by zhuang 08-12-29
	media_cbk_s media_cbk;
	//窗口句柄Hwnd //Add by zhuang 08-12-26
	HWND video_hwnd;
	memset(&media_cbk,0,sizeof(media_cbk_s));
	memset(&video_hwnd,0,sizeof(HWND));
	if (s->frameDisplayCbk==NULL)
	{
		return 0;
	}
	/**********/

	DBG_MEDIA_ENGINE("MEDIA_ENGINE: entering ph_msession_start\n");

	pthread_mutex_lock(&s->critsec_mstream_init);


	//已经创建send graph
	if(msdx_get_send_graph()==-1)
		msdx_create_send_graph();
	else 
		already_sending=1; /*已经有一个send graph了*/

	//创建接收graph
	recvgraphid=msdx_create_recv_graph();

	/************************************************************************///Add by zhuang 08-12-26
	/*                                                                      */
	media_cbk.ResizeCbk= ph_video_resize;
	media_cbk.recvgraphid = recvgraphid;
	if (s->frameDisplayCbk!=NULL)
	{
		s->frameDisplayCbk(s->cbkInfo,&media_cbk);
	}

	
	while (media_cbk.video_hwnd==0)
	{
		Sleep(100);
	}

	/************************************************************************/

	newstreams = s->newstreams;
	s->newstreams = 0;

	if (!(newstreams & (1 << PH_MSTREAM_AUDIO1)) || (!sp_audio->localport || !sp_audio->remoteport))
	{
		//不支持音频，直接去视频
		goto video;
	}  

audio:   

	if(!already_sending)
	{
		result=msdx_set_send_audio_codec("PCMU");
		if(result!=0) goto ErrorOut;
	}
	result=msdx_set_recv_audio_codec(recvgraphid,"PCMU",11025,1,16);
	if(result!=0) goto ErrorOut;

video:

	if (!(newstreams & (1 << PH_MSTREAM_VIDEO1)) || !sp_video->remoteport|| 
		strcmp(cfg->video_config.video_device,"")==0)
	{
		//不支持视频，直接跳过
		goto NoCaptureVideo;
	}  
	//加入视频采集 和 编码 filterjm
	if(!already_sending)
	{
		msdx_config_set_video_device(cfg->video_config.video_device);
		//result=msdx_set_send_video_vodec("XVID",352,288);
		//指定编码的高和宽
		result=msdx_set_send_video_vodec("XVID",cfg->video_config.video_webcam_capture_width,cfg->video_config.video_webcam_capture_height);
		if(result!=0) goto ErrorOut;	
	}
NoCaptureVideo:

	if(!(newstreams & (1 << PH_MSTREAM_VIDEO1))|| !sp_video->localport )
		goto NoRecvVideo;

	//解析对方的视频分辨率  需要处理 port为0的情况
	parse_fmtp(sp_video->ipayloads[0].fmtp,&remote_video_width,&remote_video_height,&remote_video_bitrate);
	assert(remote_video_width!=0 && remote_video_height!=0);
	profile = &av_profile;
	pt = rtp_profile_get_payload(profile, sp_video->ipayloads[0].number);
	/*
	result=msdx_set_recv_video_codec(recvgraphid,pt->mime_type,remote_video_width,remote_video_height,30,16);
	*/
	result=msdx_set_recv_video_codec(recvgraphid,pt->mime_type,remote_video_width,remote_video_height,30,16);
	if(result!=0) goto ErrorOut;	

NoRecvVideo:

	//设置显示的窗口句柄
	msdx_set_recv_display_window(recvgraphid,media_cbk.video_hwnd,0,0,media_cbk.width,media_cbk.height,0);
	msdx_set_send_preview_window(media_cbk.preview_window,0,0,media_cbk.preview_width,media_cbk.preview_height,0);
	
	/**************************************************/
	if(!already_sending)
	{
		result=msdx_connect_send_graph();
		if(result!=0) goto ErrorOut;			
	}
	/*stop send graph first*/
//	if(msdx_get_graph_state(0)!=2)
//		msdx_stop_send_graph();

	result=msdx_add_send_address(sp_audio->remoteaddr,sp_audio->remoteport,sp_video->remoteaddr,sp_video->remoteport);
	if(result!=0) goto ErrorOut;	

	if(strstr(sp_video->remoteaddr,":")==NULL)
		result=msdx_set_recv_address(recvgraphid,"127.0.0.1",sp_audio->localport,"127.0.0.1",sp_video->localport);
	else
		result=msdx_set_recv_address(recvgraphid,"::1",sp_audio->localport,"::1",sp_video->localport);
	if(result!=0) goto ErrorOut;	
	result=msdx_connect_recv_graph(recvgraphid);
	if(result!=0) goto ErrorOut;
	s->recv_graphid=recvgraphid;	
	result=msdx_run_recv_graph(recvgraphid);
	if(result!=0) goto ErrorOut;
	if(msdx_get_graph_state(0)!=1)	
		result=msdx_run_send_graph();
	if(result!=0) goto ErrorOut;	
	result=msdx_set_send_video_quantizer(getVideoQuality(cfg->video_config.video_line_configuration));
	pthread_mutex_unlock(&s->critsec_mstream_init);

	return 0;

ErrorOut:

	msdx_destroy_recv_graph(recvgraphid);
	s->recv_graphid=0;

	return result;
}

/**  used to join 2 sessions in a conference , 无用 ，注释掉   zhenHua.sun 2010-06-25
int ph_msession_conf_start(struct ph_msession_s *s1, struct ph_msession_s *s2, const char *device)
{
  int ret;
  DBG_MEDIA_ENGINE("MEDIA_ENGINE: entering ph_msession_conf_start\n");
  pthread_mutex_lock(&s1->critsec_mstream_init);
  pthread_mutex_lock(&s2->critsec_mstream_init);
  ret = ph_msession_audio_conf_start(s1, s2, device);
  pthread_mutex_unlock(&s2->critsec_mstream_init);
  pthread_mutex_unlock(&s1->critsec_mstream_init);
  return ret;
}
*/
/**  used to join 2 sessions in a conference , 无用 ，注释掉   zhenHua.sun 2010-06-25
int ph_msession_conf_stop(struct ph_msession_s *s1, struct ph_msession_s *s2)
{
  int ret;
  DBG_MEDIA_ENGINE("MEDIA_ENGINE: entering ph_msession_conf_stop\n");
  pthread_mutex_lock(&s1->critsec_mstream_init);
  pthread_mutex_lock(&s2->critsec_mstream_init);
  ret = ph_msession_audio_conf_stop(s1, s2);
  pthread_mutex_unlock(&s2->critsec_mstream_init);
  pthread_mutex_unlock(&s1->critsec_mstream_init);
  return ret;
}
*/

int ph_msession_suspend(struct ph_msession_s *s,  int traffictype, const char *device)
{
  DBG_MEDIA_ENGINE("MEDIA_ENGINE: entering ph_msession_suspend\n");
  pthread_mutex_lock(&s->critsec_mstream_init);

/*#ifdef PHAPI_VIDEO_SUPPORT
  ph_msession_video_stop(s);
#endif
  ph_msession_audio_suspend(s, traffictype, device);*/

  //check valid graphid
	if(s->recv_graphid >= 1)
	{
		//pause send graph
		assert(msdx_get_send_graph()==0);
		//make sure the graph is running
		if(msdx_get_graph_state(0)!=1 || msdx_get_graph_state(s->recv_graphid)!=1)
			return 0;
		msdx_pause_send_graph();		
		
		//pause recv graph		
		msdx_pause_recv_graph(s->recv_graphid);		
	}

  pthread_mutex_unlock(&s->critsec_mstream_init);
  return (0);
}

/**
 * @brief resume a media session
 */
int ph_msession_resume(struct ph_msession_s *s, int traffictype, const char *device)
{
  int ret = 0;

  DBG_MEDIA_ENGINE("MEDIA_ENGINE: entering ph_msession_resume\n");
  pthread_mutex_lock(&s->critsec_mstream_init);
/*  ret = ph_msession_audio_resume(s, traffictype, device);
  if (ret == 0)
  {
#ifdef PHAPI_VIDEO_SUPPORT
    ret = ph_msession_video_start(s, "");
#endif
  }*/

   //check valid graphid
	if(s->recv_graphid >= 1)
	{
		//resume send graph
		assert(msdx_get_send_graph()==0);
		//make sure the graph is pausing
		if(msdx_get_graph_state(0)!=3 || msdx_get_graph_state(s->recv_graphid)!=3)
			return 0;
		msdx_resume_send_graph();		
		
		//pause recv graph		
		msdx_resume_recv_graph(s->recv_graphid);		
	}

  pthread_mutex_unlock(&s->critsec_mstream_init);
  return ret;
}

int ph_msession_stopped(struct ph_msession_s *s)
{
	return (s->activestreams == 0);
}

int ph_msession_audio_stopped(struct ph_msession_s *s)
{
	return !(s->activestreams & (1 << PH_MSTREAM_AUDIO1));
}

int ph_msession_video_stopped(struct ph_msession_s *s)
{
	return !(s->activestreams & (1 << PH_MSTREAM_VIDEO1));
}


void ph_msession_stop(struct ph_msession_s *s, const char *adevid)
{
	ph_mstream_params_t *sp_audio = &s->streams[PH_MSTREAM_AUDIO1];
	ph_mstream_params_t *sp_video = &s->streams[PH_MSTREAM_VIDEO1];

	DBG_MEDIA_ENGINE("MEDIA_ENGINE: entering ph_msession_stop\n");
	pthread_mutex_lock(&s->critsec_mstream_init);


	//check valid graphid
	if(s->recv_graphid >= 1)
	{
		//stop send graph
		assert(msdx_get_send_graph()==0);
	//	msdx_stop_send_graph();
		msdx_delete_send_address(sp_audio->remoteaddr,sp_audio->remoteport,sp_video->remoteaddr,sp_video->remoteport);
		msdx_run_send_graph();

		//	msdx_destroy_send_graph();

		//stop recv graph
		msdx_stop_recv_graph(s->recv_graphid);
		msdx_destroy_recv_graph(s->recv_graphid);
		s->recv_graphid=0;
	}
	pthread_mutex_unlock(&s->critsec_mstream_init);
}
/**   主要支持H264  zhenHua.sun 2010-06-28
static PayloadType ilbc =
{
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(8000),
  BITS_PER_SAMPLE( 0),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH( 0),
  NORMAL_BITRATE( 13330),
  MIME_TYPE ("ILBC")
};

static PayloadType amr={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(8000),
  BITS_PER_SAMPLE(0),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(12000),    // set mode MR 122 as default s
  MIME_TYPE ("AMR")
};

static PayloadType amr_wb={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(16000),
  BITS_PER_SAMPLE(320),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(24000),   // set mode 2385 as default 
  MIME_TYPE ("AMR-WB")
};

static PayloadType cng_8={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(8000),
  BITS_PER_SAMPLE(0),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(8000),
  MIME_TYPE ("CN")
};

static PayloadType g726={
    PAYLOAD_AUDIO_PACKETIZED,
    CLOCK_RATE(8000),
    BITS_PER_SAMPLE(0),
    ZERO_PATTERN(NULL),
    PATTERN_LENGTH(0),
    NORMAL_BITRATE(32000),
    MIME_TYPE ("G726-32")
};

static PayloadType g722={
    PAYLOAD_AUDIO_PACKETIZED,
    CLOCK_RATE(16000),
    BITS_PER_SAMPLE(0),
    ZERO_PATTERN(NULL),
    PATTERN_LENGTH(0),
    NORMAL_BITRATE(60000),
    MIME_TYPE ("G722")
};


static PayloadType cng_16={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(16000),
  BITS_PER_SAMPLE(0),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(8000),
  MIME_TYPE ("CN")
};

static PayloadType mspeex_nb={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(8000),
  BITS_PER_SAMPLE(320),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(15000),    // set mode MR 122 as default 
  MIME_TYPE ("SPEEX")
};

static PayloadType mspeex_wb={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(16000),
  BITS_PER_SAMPLE(0),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(28000),
  MIME_TYPE ("SPEEX")
};

static PayloadType g726_wb={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(16000),
  BITS_PER_SAMPLE(0),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(28000),
  MIME_TYPE ("G726-64wb")
};

static PayloadType h263p={
  PAYLOAD_VIDEO,
  90000,
  0,
  NULL,
  0,
  256000,
  CODEC_H263P_MIME_STRING
};

static PayloadType h263flv1={
  PAYLOAD_VIDEO,
  90000,
  0,
  NULL,
  0,
  256000,
  CODEC_FLV1_MIME_STRING
};
*/
static PayloadType h264={
  PAYLOAD_VIDEO,
  90000,
  0,
  NULL,
  0,
  256000,
  CODEC_H264_MIME_STRING
};

static PayloadType mpeg4 ={
  PAYLOAD_VIDEO,
  90000,
  0,
  NULL,
  0,
  256000,
  CODEC_MPG4_MIME_STRING
};

/************************************************************************/
/*Mpeg2*/
static PayloadType mpeg2 ={
	PAYLOAD_VIDEO,
	90000,
	0,
	NULL,
	0,
	256000,
	CODEC_MPG2_MIME_STRING
};
/************************************************************************/

int
ph_media_init(const char *pluginpath)
{
  static int first_time = 1;
  RtpProfile *profile;

  if (!first_time)
  {
      return 0;
  }

  ortp_init();
  ortp_set_log_level_mask(0);

  /** 编解码无用，删除   zhenHua.sun 2010-06-25
  ph_media_audio_init();
  ph_media_codecs_init(pluginpath);
  */

  /* initialize audio payloads (ortp needs this) */
  profile = &av_profile;
  rtp_profile_set_payload(profile,PH_MEDIA_DTMF_PAYLOAD, &payload_type_telephone_event);

  /** 不支持的格式  zhenHua.sun 2010-06-28
#ifdef ENABLE_ILBC
  rtp_profile_set_payload(profile,PH_MEDIA_ILBC_PAYLOAD, &ilbc);
#endif
  rtp_profile_set_payload(profile,PH_MEDIA_G722_PAYLOAD, &g722);
  rtp_profile_set_payload(profile,PH_MEDIA_G726_PAYLOAD, &g726);
#ifdef ENABLE_AMR
  rtp_profile_set_payload(profile,PH_MEDIA_AMR_PAYLOAD, &amr);
#endif

#ifdef ENABLE_AMR_WB
  rtp_profile_set_payload(profile,PH_MEDIA_AMR_WB_PAYLOAD, &amr_wb);
#endif

  rtp_profile_set_payload(profile,PH_MEDIA_CN_PAYLOAD, &cng_8);
  rtp_profile_set_payload(profile,PH_MEDIA_SPEEXNB_PAYLOAD, &mspeex_nb);
  rtp_profile_set_payload(profile,PH_MEDIA_SPEEXWB_PAYLOAD, &mspeex_wb);
  rtp_profile_set_payload(profile,PH_MEDIA_G726WB_PAYLOAD, &g726_wb);
*/

#ifdef PHAPI_VIDEO_SUPPORT
  /** 不支持的格式  zhenHua.sun 2010-06-28
  rtp_profile_set_payload(profile,PH_MEDIA_H263P_PAYLOAD, &h263p);
  rtp_profile_set_payload(profile,PH_MEDIA_H263FLV1_PAYLOAD, &h263flv1);
  */
  rtp_profile_set_payload(profile,PH_MEDIA_H264_PAYLOAD, &h264);
  rtp_profile_set_payload(profile,PH_MEDIA_MPEG4_PAYLOAD, &mpeg4);
  rtp_profile_set_payload(profile,PH_MEDIA_MPEG2_PAYLOAD, &mpeg2);
#endif

  //ortp_scheduler_init();
  ortp_set_log_file(NULL);

  msdx_initial();

  first_time = 0;
  if (graph_hwnd_map==NULL)
  {
	  graph_hwnd_map = g_hash_table_new_full(g_str_hash, g_str_equal,g_free, g_free);
  }
  return 0;
}

int ph_media_supported_payload(ph_media_payload_t *pt, const char *ptstring)
{
  PayloadType *rtppt;
  RtpProfile *profile = &av_profile;

  pt->number = rtp_profile_get_payload_number_from_rtpmap(profile, ptstring);
  if (pt->number == -1)
  {
    return 0;
  }

  rtppt = rtp_profile_get_payload(profile, pt->number);

  strncpy(pt->string, rtppt->mime_type, sizeof(pt->string));
  pt->rate = rtppt->clock_rate;

  return 1;
}

/**
 * 编解码无用，删除
 * zhenHua.sun 2010-06-23
phcodec_t *ph_media_lookup_codec_bymime(const char *mime, int rate)
{
  phcodec_t *codec = ph_codec_list;
  int plen = strlen(mime);

  plen = strlen(mime);
  DBG_CODEC_LOOKUP("we need to match against : <%s/%d> in phcodec list\n", mime, rate);

  while(codec)
  {
    int mlen;

    DBG_CODEC_LOOKUP("....trying <%s/%d>\n", codec->mime, codec->clockrate);
    mlen = strlen(codec->mime);
    if(mlen == plen)
    {
      if (!strncasecmp(codec->mime, mime, mlen))
      {
        if (!codec->clockrate || !rate)
        {
          DBG_CODEC_LOOKUP("....<%s/%d> matched\n", codec->mime, codec->clockrate);
          return codec;
        }

        if (codec->clockrate == rate)
        {
          DBG_CODEC_LOOKUP("....<%s/%d> matched\n", codec->mime, codec->clockrate);
          return codec;
        }
      }
    }
    codec = codec->next;
  }

  DBG_CODEC_LOOKUP("could not find phapi codec ref <%s> in phcodec list\n", mime);
  return 0;
}
*/
/** 
 * 编解码无用，删除
 * zhenHua.sun 2010-06-23
// find a codec object corresponding to given payload 
phcodec_t *ph_media_lookup_codec(int payload)
{
  RtpProfile *profile = &av_profile;
  PayloadType *pt = rtp_profile_get_payload(profile, payload);

  DBG_CODEC_LOOKUP("CODEC LOOKUP: ph_media_lookup_codec\n");

  if (!pt)
  {
    DBG_CODEC_LOOKUP("could not find payload %d in ortp profile list\n", payload);
    return 0;
  }

  if (!pt->mime_type)
  {
    DBG_CODEC_LOOKUP("fatal error - NULL mime type for codec %d in ortp\n", payload);
    return 0;
  }

  return ph_media_lookup_codec_bymime(pt->mime_type, pt->clock_rate);
}
*/
/*
 given the mime string of type ENCODING/RATE return TRUE if we have a codec able to handle this
 payload
*/
int ph_media_can_handle_payload(const char *mime)
{
  const char *slash;
  char mime2[64];
  int mlen;

  if (!mime)
  {
    return 0;
  }

  slash = strchr(mime, '/');
  if (!slash)
  {
	return 0;
	/**
	* 编解码无用，删除
	* zhenHua.sun  2010-06-23
	return 0 != ph_media_lookup_codec_bymime(mime2, atoi(slash+1));
	*/
  }

  mlen = slash - mime;
  if (mlen > (sizeof(mime2) - 1))
  {
    return 0;
  }

  strncpy(mime2, mime, mlen);
  mime2[mlen] = 0;
  return 0;
 /**
   * 编解码无用，删除
   * zhenHua.sun  2010-06-23
  return 0 != ph_media_lookup_codec_bymime(mime2, atoi(slash+1));
  */
}

void ph_tvsub(register struct timeval *out, register struct timeval *in)
{
  out->tv_usec -= in->tv_usec;

  while(out->tv_usec < 0)
  {
    --out->tv_sec;
    out->tv_usec += 1000000;
  }

  out->tv_sec -= in->tv_sec;
}

void
ph_tvdiff(register struct timeval *diff, register struct timeval *out, register struct timeval *in)
{
  diff->tv_sec = out->tv_sec;
  diff->tv_usec = out->tv_usec;

  diff->tv_usec -= in->tv_usec;

  while(diff->tv_usec < 0)
  {
    --diff->tv_sec;
    diff->tv_usec += 1000000;
  }

  diff->tv_sec -= in->tv_sec;
}

int
ph_timeval_substract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec)
  {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000)
  {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

int ph_media_get_clock_rate(int payload)
{
  RtpProfile *profile = &av_profile;
  PayloadType *pt;
#ifdef SPEEX_OVER_G729_HACK
  if (payload == PH_MEDIA_G729_PAYLOAD)
  {
    payload = PH_MEDIA_SPEEXWB_PAYLOAD;
  }
#endif
  pt  = rtp_profile_get_payload(profile, payload);
  return pt->clock_rate;
}


// deprecated branch of code (before msession & conf addition)
#if 0
int ph_media_start(phcall_t *ca, int port, int videoport,
           void (*dtmfCallback)(phcall_t *ca, int event),
           void (*endCallback)(phcall_t *ca, int event),
               phFrameDisplayCbk frameDisplay,
       const char * deviceId, unsigned vad, int cng, int jitter, int noaec)

{

  int ret;

  ret = ph_media_audio_start(ca, port, dtmfCallback, endCallback, deviceId, vad, cng, jitter, noaec);
/**
   * 编解码无用，删除   zhenHua.sun 2010-06-23
#ifdef PHAPI_VIDEO_SUPPORT
  if (!ret && (frameDisplay != 0))
{
    ph_media_video_start(ca, videoport, frameDisplay);
}
  else
  {
    ph_media_video_stop(ca);
  }
#endif
*/
  return ret;

}

void ph_media_stop(phcall_t *ca)
{
  int i;

#ifdef PHAPI_VIDEO_SUPPORT
  ph_media_video_stop(ca);
#endif
  ph_media_audio_stop(ca);
}
#endif  //if 0

void ph_clear_msession_streams_fmtps(struct ph_msession_s *s)
{
	int iStreamsIndex;
	int iPayloadsIndex;
	
	if (s)
	{
		for (iStreamsIndex = 0; 
				iStreamsIndex < PH_MSESSION_MAX_STREAMS; iStreamsIndex++)
		{
			for (iPayloadsIndex = 0; 
					iPayloadsIndex < PH_MSTREAM_PARAMS_MAX_PAYLOADS; iPayloadsIndex++)
			{
				if (s->streams[iStreamsIndex].ipayloads[iPayloadsIndex].fmtp)
				{
					osip_free(s->streams[iStreamsIndex].ipayloads[iPayloadsIndex].fmtp);
					s->streams[iStreamsIndex].ipayloads[iPayloadsIndex].fmtp = NULL;
				}
				
				if (s->streams[iStreamsIndex].opayloads[iPayloadsIndex].fmtp)
				{
					osip_free(s->streams[iStreamsIndex].opayloads[iPayloadsIndex].fmtp);
					s->streams[iStreamsIndex].opayloads[iPayloadsIndex].fmtp = NULL;
				}
			}
		}
	}
}

/************************************************************************/
/*会议音视频*/
int ph_media_send_start( char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,HWND previewHwnd,void *pClass,void *func,int width,int height)
{
	int result;
	RECT WindowRect;
	phConfig_t *cfg = phGetConfig();
	int already_sending=0;
	//已经创建send graph
	if(msdx_get_send_graph()==-1)
		msdx_create_send_graph();
	else 
		already_sending=1; /*已经有一个send graph了*/
  
	if(!already_sending)
	{
		result=msdx_set_send_audio_codec("PCMU");
		if(result!=0) goto ErrorOut;
	}

//	strcpy(cfg->video_config.video_device,"USB 视频设备");
	if (VideoPort==0||strcmp(cfg->video_config.video_device,"")==0)
	{
		//不支持视频，直接跳过
		goto NoCaptureVideo;
	}  
	//加入视频采集 和 编码 filterjm
	if(!already_sending)
	{
		msdx_config_set_video_device(cfg->video_config.video_device);
		//视频量化系数
		result=msdx_set_send_video_vodec("XVID",width,height);
		if(result!=0) goto ErrorOut;	
	}
NoCaptureVideo:
	//设置显示的窗口句柄
	if (previewHwnd)
	{
		GetWindowRect(previewHwnd,&WindowRect);
		msdx_set_send_preview_window(previewHwnd,0,0,WindowRect.right-WindowRect.left,WindowRect.bottom-WindowRect.top,0);
	}
	

	/**************************************************/
	//加入屏幕共享的Filter
	if(!already_sending)
	{
		result=msdx_set_send_screen_codec("Snap_Screen");
	}

	if(!already_sending)
	{
		result=msdx_connect_send_graph();
		if(result!=0) goto ErrorOut;			
	}
	/*stop send graph first*/
	//	if(msdx_get_graph_state(0)!=2)
	//		msdx_stop_send_graph();

	result=msdx_add_send_address(AudioIP,AudioPort,VideoIP,VideoPort);
	if(result!=0) goto ErrorOut;
//	msdx_set_Rtcp_Callbak_func(pClass,func);
	result=msdx_set_Rtcp_Recv_Filter(VideoIP,VideoPort);
	if(msdx_get_graph_state(0)!=1)	
		result=msdx_run_send_graph();
	if(result!=0) goto ErrorOut;
	if (strcmp(cfg->video_config.video_device,"")!=0)
	{
		result=msdx_set_send_video_quantizer(getVideoQuality(cfg->video_config.video_line_configuration));
	}
	return 0;

ErrorOut:
	return result;
}

void ph_media_send_stop(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort)
{
	assert(msdx_get_send_graph()==0);
	//	msdx_stop_send_graph();
	msdx_delete_send_address(AudioIP,AudioPort,VideoIP,VideoPort);
	//msdx_destroy_send_graph();
//	msdx_run_send_graph();
}

int ph_media_recv_start(const char *userId,char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight,void *pClass,void *func)
{
	int xpos = 0,ypos = 0,width = 0,height = 0;
	int result;
	phConfig_t *cfg = phGetConfig();
	int already_sending=0;
	int *_pRecvGraphID = malloc(sizeof(int)); 
	int recvgraphid;
	RECT WindowRect;
	//创建接收graph
	recvgraphid=msdx_create_recv_graph2(pClass,func);
	if (AudioPort==0)
	{
		//不支持音频，直接去视频
		goto video;
	}

	result=msdx_set_recv_audio_codec(recvgraphid,"PCMU",11025,1,16);
	if(result!=0) goto ErrorOut;

video:

	if(VideoPort==0)
		goto NoRecvVideo;

	result=msdx_set_recv_video_codec(recvgraphid,codecMIME,videoWidth,videoHeight,30,16);
	if(result!=0) goto ErrorOut;

NoRecvVideo:

	//设置显示的窗口句柄
	if (remoteHwnd)
	{
		GetWindowRect(remoteHwnd,&WindowRect);
		width = WindowRect.right-WindowRect.left;
		height = WindowRect.bottom-WindowRect.top;
		if (width!=0&&height!=0)
		{
			if ((width/height)>=(176/144))
			{
				xpos = (width-height*176/144)/2;
				width = height*176/144;
			}
			else
			{
				ypos = (height-width*144/176)/2;
				height = width*144/176;
			}
		}
		msdx_set_recv_display_window(recvgraphid,remoteHwnd,xpos,ypos,width,height,0);
	}
	

	result=msdx_set_recv_address(recvgraphid,AudioIP,AudioPort,VideoIP,VideoPort);
	if(result!=0) goto ErrorOut;	
	result=msdx_connect_recv_graph(recvgraphid);
	if(result!=0) goto ErrorOut;
	result=msdx_run_recv_graph(recvgraphid);
	if(result!=0) goto ErrorOut;

//	graph_hwnd_map.insert(make_pair(remoteHwnd,recvgraphid));
//	g_hash_table_insert(graph_hwnd_map, &remoteHwnd, &recvgraphid);
	*_pRecvGraphID = recvgraphid;
	g_hash_table_insert(graph_hwnd_map, g_strdup(userId), _pRecvGraphID);
	return 0;

ErrorOut:

	msdx_destroy_recv_graph(recvgraphid);

	return result;
}

void ph_media_recv_stop(const char *userId)
{
	//check valid graphid
	int graphID = 0;
	int *value = NULL;
	//map<HWND,int>::iterator it = graph_hwnd_map.find(hwnd);
	value = (int *)g_hash_table_lookup(graph_hwnd_map, userId);
	if (value==NULL)
	{
		return;
	}
	graphID = *value;
	//if (it!=graph_hwnd_map.end())
	//{
	//	graphID = it->second;
	//}
	if(graphID > 0)
	{
		//stop recv graph
		msdx_stop_recv_graph(graphID);
		msdx_destroy_recv_graph(graphID);
		g_hash_table_steal(graph_hwnd_map,userId);
	}
}

//发送桌面共享
int ph_screen_share_send_start(char * ScreenIP,int ScreenPort,int x,int y,int width,int height)
{
	int result;
	msdx_add_send_address2(ScreenIP,0,ScreenIP,0,ScreenIP,ScreenPort);
	result = msdx_set_screen_filter_config(x,y,width,height);
	return result;
}

//取消桌面共享
void ph_screen_share_send_stop(char * ScreenIP,int ScreenPort)
{
	msdx_delete_screen_send_address(ScreenIP,ScreenPort);
}

//接收桌面共享
int ph_screen_share_recv_start(char * ScreenIP,int ScreenPort,HWND remoteHwnd,int videoWidth,int videoHeight)
{
	int xpos = 0,ypos = 0,width = 0,height = 0;
	int result;
	int *_pRecvGraphID = malloc(sizeof(int)); 
	int recvgraphid;
	RECT WindowRect;
	//创建接收graph
	recvgraphid=msdx_create_recv_graph();

	result=msdx_set_recv_screen_codec(recvgraphid,"JPEG",videoWidth,videoHeight,2,24);
	if(result!=0) goto ErrorOut;

	//设置显示的窗口句柄
	if (remoteHwnd)
	{
		GetWindowRect(remoteHwnd,&WindowRect);
		width = WindowRect.right-WindowRect.left;
		height = WindowRect.bottom-WindowRect.top;
		//if (width!=0&&height!=0)
		//{
		//	if ((width/height)>=(176/144))
		//	{
		//		xpos = (width-height*176/144)/2;
		//		width = height*176/144;
		//	}
		//	else
		//	{
		//		ypos = (height-width*144/176)/2;
		//		height = width*144/176;
		//	}
		//}
		msdx_set_recv_display_window(recvgraphid,remoteHwnd,0,0,width,height,0);
	}

	msdx_set_recv_screen_address(recvgraphid,ScreenIP,ScreenPort);
	if(result!=0) goto ErrorOut;	
	result=msdx_connect_recv_graph(recvgraphid);
	if(result!=0) goto ErrorOut;
	result=msdx_run_recv_graph(recvgraphid);
	if(result!=0) goto ErrorOut;

	*_pRecvGraphID = recvgraphid;
	g_hash_table_insert(graph_hwnd_map, g_strdup("screen_share"), _pRecvGraphID);
	return 0;

ErrorOut:

	msdx_destroy_recv_graph(recvgraphid);

	return result;
}

//取消接收桌面共享
void ph_screen_share_recv_stop()
{
	ph_media_recv_stop("screen_share");
}

//退出会议，关闭所有接收
void ph_exit_conference()
{
	
	GList *graph_id_list = g_hash_table_get_values(graph_hwnd_map);
	if (graph_id_list!=NULL)
	{
		while (graph_id_list)
		{
			int *pGraphID = (int *)graph_id_list->data;
			int graphid = *pGraphID;
			if(graphid>0)
			{
				msdx_stop_recv_graph(graphid);
				msdx_destroy_recv_graph(graphid);
			}
			graph_id_list = graph_id_list->next;
//			g_hash_table_steal(graph_hwnd_map,pGraphID);
//			free(pGraphID);
		}
		g_hash_table_steal_all(graph_hwnd_map);
	}
	
}

//video窗口resize
void ph_resize_video_window(const char *userId,HWND remoteHwnd,int fullscreenflag)
{
	int xpos = 0,ypos = 0,width = 0,height = 0;
	int graphID = 0;
	int *value = NULL;
	RECT WindowRect;
	if(strcmp(userId,"")==0||userId==NULL) //Add by hexianfu 2010-3-4
	{
		return;
	}
	if (strcmp(userId,"unknown")==0)
	{
		return;
	}
	if (remoteHwnd)
	{
		GetWindowRect(remoteHwnd,&WindowRect);
		width = WindowRect.right-WindowRect.left;
		height = WindowRect.bottom-WindowRect.top;
		if (width==0||height==0)
		{
			return;
		}
		if (strcmp(userId,"screen_share")!=0)
		{
			if ((width/height)>=(176/144))
			{
				xpos = (width-height*176/144)/2;
				width = height*176/144;
			}
			else
			{
				ypos = (height-width*144/176)/2;
				height = width*144/176;
			}
		}
		if((int)g_hash_table_size(graph_hwnd_map)>0) //Add by hexianfu 2010-3-4
		{
			value = (int *)g_hash_table_lookup(graph_hwnd_map, userId);
			if (value==NULL)
			{
				return;
			}
			graphID = *value;
			if(graphID > 0)
			{
				//resize
				msdx_set_recv_display_window(graphID,remoteHwnd,xpos,ypos,width,height,fullscreenflag);
			}
		}
		
	}

}

//禁止发言
void ph_forbid_speak(char * AudioIP,int AudioPort)
{
	assert(msdx_get_send_graph()==0);
	msdx_delete_send_address2(AudioIP,AudioPort);
	msdx_run_send_graph();
}

void ph_forbid_speak2(char * userId)
{
	//check valid graphid
	int graphID = 0;
	int *value = NULL;
	//map<HWND,int>::iterator it = graph_hwnd_map.find(hwnd);
	value = (int *)g_hash_table_lookup(graph_hwnd_map, userId);
	if (value==NULL)
	{
		return;
	}
	graphID = *value;
	//if (it!=graph_hwnd_map.end())
	//{
	//	graphID = it->second;
	//}
	if(graphID > 0)
	{
		//clear recv buffer
		msdx_clear_recv_buf(graphID);
	}
}

//允许发言
void ph_permit_speak(char * AudioIP,int AudioPort)
{
	assert(msdx_get_send_graph()==0);
	msdx_add_send_address(AudioIP,AudioPort,NULL,0);
	msdx_run_send_graph();
}

//视频质量
int getVideoQuality(int video_line_configuration) {

	switch(video_line_configuration) {
	case PHAPI_VIDEO_LINE_128KBPS:
		return 5;

	case PHAPI_VIDEO_LINE_256KBPS:
		return 4;

	case PHAPI_VIDEO_LINE_512KBPS:
		return 3;

	case PHAPI_VIDEO_LINE_1024KBPS:
		return 2;

	case PHAPI_VIDEO_LINE_2048KBPS:
		return 0;

	case PHAPI_VIDEO_LINE_AUTOMATIC:
		return 0;

	default:
		return 0;
	}
}
/************************************************************************/