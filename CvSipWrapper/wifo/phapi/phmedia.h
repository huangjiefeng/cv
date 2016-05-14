/*
 * phmedia -  Phone Api media streamer
 *
 * Copyright (C) 2005-2006 WENGO SAS
 * Copyright (C) 2004 Vadim Lebedev <vadim@mbdsys.com>
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
#ifndef __PHMEDIA_H__
#define __PHMEDIA_H__

#include <pthread.h>
#include <owsl.h>

#define PH_MEDIA_DTMF_PAYLOAD 101
#define PH_MEDIA_SPEEXNB_PAYLOAD 105
#define PH_MEDIA_G726WB_PAYLOAD 102
#define PH_MEDIA_SPEEXWB_PAYLOAD 103
#define PH_MEDIA_ILBC_PAYLOAD 111
#define PH_MEDIA_AMR_PAYLOAD 123
#define PH_MEDIA_AMR_WB_PAYLOAD 124
#define PH_MEDIA_G72632_PAYLOAD 2
#define PH_MEDIA_G729_PAYLOAD 18
#define PH_MEDIA_H263_PAYLOAD 34
#define PH_MEDIA_H263FLV1_PAYLOAD 96
#define PH_MEDIA_H263P_PAYLOAD 97
#define PH_MEDIA_MPEG4_PAYLOAD 98
//Mpeg2 ts
#define PH_MEDIA_MPEG2_PAYLOAD 33
#define PH_MEDIA_H264_PAYLOAD 40
#define PH_MEDIA_CN_16000_PAYLOAD 125
#define PH_MEDIA_G72632_PAYLOAD 2
#define PH_MEDIA_CN_PAYLOAD 13
#define PH_MEDIA_G722_PAYLOAD 9
#define PH_MEDIA_G726_PAYLOAD 112
#define PH_MEDIA_AACLC_PAYLOAD 116

#define PH_MEDIA_CN_PT_STR "13"

#define PH_MSTREAM_PARAMS_MAX_PAYLOADS	16


struct ph_media_payload_s
{
  int  number;       /* payload code for rtp packets*/
  char string[32];   /* mime string (ex: ILBC/8000/1) */
  int  rate;         /* sampling rate */
  int  chans;        /* number of channels */
  int  ptime;        /* media duration in one packet in msecs */
  int  psize;        /* (optional) size of payload in one packet */
  int  mode;         /* (optional) codec specific mode parameter */
  char* fmtp;        /* value part of a fmtp attribute (ex: 111 mode=30) */
};



enum ph_media_types {
  PH_MEDIA_TYPE_AUDIO = 1,
  PH_MEDIA_TYPE_VIDEO,
  PH_MEDIA_TYPE_OTHER
};

/* traffic type of a streamer */
enum ph_mstream_traffic_type {
  PH_MSTREAM_TRAFFIC_NONE = 0,
  PH_MSTREAM_TRAFFIC_IN = 1, // 1<<0 = RX
  PH_MSTREAM_TRAFFIC_OUT = 2, // 1<<1 = TX
  PH_MSTREAM_TRAFFIC_IO = 3 // RX | TX
};


enum ph_mstream_flags {
  PH_MSTREAM_FLAG_VAD = 1,
  PH_MSTREAM_FLAG_CNG = 2,
  PH_MSTREAM_FLAG_AEC = 4,
  // SPIKE_HDX
  PH_MSTREAM_FLAG_MICHDX = 8,  /* half duplex mode */
  PH_MSTREAM_FLAG_SPKHDX = 16,  /* half duplex mode */
  PH_MSTREAM_FLAG_RUNNING = 32,
  PH_MSTREAM_FLAG_TUNNEL = 64,

};

/**
 * @struct description of 1 media stream
 */
struct ph_mstream_params_s
{
    /* local IP port on which the stream will be received */
	int  localport;
	int  mediatype;
	int  traffictype;
	int  flags;
	int  jitter;
	int  ptime;
    /* IP of the remote peer */
	//char remoteaddr[16];
	char remoteaddr[64];//modify by zhuang 09-07-06
	int  remoteport;
	int  vadthreshold;
	int  videoconfig;
	struct ph_media_payload_s ipayloads[PH_MSTREAM_PARAMS_MAX_PAYLOADS];
	struct ph_media_payload_s opayloads[PH_MSTREAM_PARAMS_MAX_PAYLOADS];
	/* structure dedicated to the underlying streamer engine (audio, video..)
		cf phastream, phvstream
	*/
	void *streamerData;
};

typedef struct ph_mstream_params_s ph_mstream_params_t;


#define PH_MSESSION_MAX_STREAMS  4
#define PH_MSTREAM_AUDIO1 0
#define PH_MSTREAM_VIDEO1 1
#define PH_MSTREAM_AUDIO2 2
#define PH_MSTREAM_VIDEO2 3

enum ph_mession_conf_flags
  {
    PH_MSESSION_CONF_NOCONF = 0,
    PH_MSESSION_CONF_MEMBER = 1,
    PH_MSESSION_CONF_MASTER = 2
  };


/**
 * @brief session englobes up to 2 audio and 2 video streams
 * allocation sequence :
 *   - fill with 0
 *   - pthread_mutex_init on the critsec_mstream_init
 */
struct ph_msession_s
{
	/* bit mask of active streams */
	int    activestreams;

	/* bit mask of new streams to be activated */
	int    newstreams;

	/* potential role of the session in a conference. cf ph_mession_conf_flags enum */
	int    confflags;

    /* partner session for a conference (the other session)*/
	struct ph_msession_s *confsession;

    /* array of potential streams */
	struct ph_mstream_params_s streams[PH_MSESSION_MAX_STREAMS];

	/* mutex used to avoid threading issues when operating on the streams */
	pthread_mutex_t critsec_mstream_init;

	void (*dtmfCallback)(void *info, int event);
	void (*endCallback)(void  *info, int event);
	void (*frameDisplayCbk)(void *info, void *event);

    /*
        holder for the userdata "info" element of the callbacks prototypes
    */
	void *cbkInfo;

	int recv_graphid;
};

typedef struct ph_msession_s ph_msession_t;

typedef struct ph_media_payload_s  ph_media_payload_t;

typedef void (*RtpTransportPrePostActionCallback)(int cid, void *error, void *buffer, int *len);
/**
 * @brief structure associated with a rtp flow to manage 
 * data just before/after it is sent/received.
 * Also manage owsl sockets.
 */
typedef struct ph_rtp_transport_data_s
{
	int externalID;

	OWSLSocket rtp_sock;
	OWSLSocket rtcp_sock;

	RtpTransportPrePostActionCallback rtp_pre_send_cb; /* Callback for pre-send actions */
	RtpTransportPrePostActionCallback rtp_post_recv_cb; /* Callback for post-recv actions */

}	ph_rtp_transport_data_t;

/************************************************************************///Add by zhuang 08-12-29
/*                                                                      */
struct MediaCallbk
{
	HWND preview_window;
	HWND video_hwnd;
	int preview_width;
	int preview_height;
	int width;
	int height;
	int recvgraphid;
	void (*ResizeCbk)(HWND hwnd,int width,int height,int recvgraphid,int fullscreenflag);

};
typedef struct MediaCallbk media_cbk_s;
/************************************************************************/

int ph_media_init(const char *pluginpath);
int ph_media_cleanup(void);

int ph_media_supported_payload(ph_media_payload_t *pt, const char *ptstring);
int ph_media_can_handle_payload(const char *mime);

/**
 * @brief start or (modify and restart) the necessary device/streaming engines of the session
 */
int ph_msession_start(struct ph_msession_s *s, const char *adeviceId);
/**
 * @brief stop the necessary device/streaming engines of the session
 */
void ph_msession_stop(struct ph_msession_s *s, const char *adeviceId);
int ph_msession_stopped(struct ph_msession_s *s);
#define ph_msession_stream_active(s, n)  (s->activestreams & (1 << n))
int ph_msession_set_recvol(struct ph_msession_s *s,  int level);
int ph_msession_set_playvol(struct ph_msession_s *s,  int level);
//int ph_msession_send_sound_file(struct ph_msession_s *s, const char *filename);	该函数负责发送音频文件，无用，注释掉 zhenHua.sun 2010-06-25
//int ph_msession_send_dtmf(struct ph_msession_s *s, int dtmf, int mode);		dtmf，无用，删除    zhenHua.sun 2010-06-25
//int ph_msession_conf_start(struct ph_msession_s *s1, struct ph_msession_s *s2, const char *adevice);	used to join 2 sessions in a conference , 无用 ，注释掉   zhenHua.sun 2010-06-25
//int ph_msession_conf_stop(struct ph_msession_s *s1, struct ph_msession_s *s2);
int ph_msession_suspend(struct ph_msession_s *s,  int traffictype, const char *adevice);
int ph_msession_resume(struct ph_msession_s *s, int traffictype, const char *adevice);

/************************************************************************/
/*会议音视频*/
int ph_media_send_start(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,HWND previewHwnd,void *pClass,void *func,int width,int height);
void ph_media_send_stop(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort);
int ph_media_recv_start(const char *userId,char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight,void *pClass,void *func);
void ph_media_recv_stop(const char *userId);
void ph_exit_conference();
//屏幕共享
int ph_screen_share_send_start(char * ScreenIP,int ScreenPort,int x,int y,int width,int height);
//取消桌面共享
void ph_screen_share_send_stop(char * ScreenIP,int ScreenPort);
//接收桌面共享
int ph_screen_share_recv_start(char * ScreenIP,int ScreenPort,HWND remoteHwnd,int videoWidth,int videoHeight);
//取消接收桌面共享
void ph_screen_share_recv_stop();
//video窗口resize
void ph_resize_video_window(const char *userId,HWND remoteHwnd,int fullscreenflag);

//禁止发言
void ph_forbid_speak(char * AudioIP,int AudioPort);
//允许发言
void ph_permit_speak(char * AudioIP,int AudioPort);

//视频质量
int getVideoQuality(int video_line_configuration);

void ph_forbid_speak2(char * userId);
/************************************************************************/


struct timeval;
void ph_tvdiff(struct timeval *diff,  struct timeval *out,  struct timeval *in);
void ph_tvsub( struct timeval *out,  struct timeval *in);
int ph_timeval_substract (struct timeval *result, struct timeval *x, struct timeval *y);



void ph_clear_msession_streams_fmtps(struct ph_msession_s *s);


/* */
#endif
