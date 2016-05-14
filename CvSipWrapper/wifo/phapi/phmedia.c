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
//#include "msdx0.1\ExportUtil.h"

GHashTable *graph_hwnd_map=NULL;


//std::map<HWND,int> graph_hwnd_map;
/************************************************************************///Add by zhuang 08-12-23
void parse_fmtp(char *fmtp_str,int *width,int *height,int *bitrate)
{
	char *temp;
	char *temp2;
	char width_str[10]={0};
	char height_str[10]={0};
	char bitrate_str[10] ={0};
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
		strncpy(width_str,temp,temp2-1-temp);

		temp=temp2;
		while (*temp2!='\r'&&*temp2!='\n'&&*temp2!='\0'&&*temp2!=' '&&*temp2!=',')
		{
			temp2++;
		}
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

	buf_tmp = (char*)malloc(len);
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
	ph_rtp_transport_data_t *data = (ph_rtp_transport_data_t*) (t->data);	//

	if (!data)
	{
		return -1;
	}

	return ph_transport_common_sendto(data, data->rtp_sock, buffer, len, flags, to, tolen);
}

int ph_rtcp_transport_sendto(struct _RtpTransport *t, const void *buffer, size_t len, int flags,
							 const struct sockaddr *to, socklen_t tolen)
{
	ph_rtp_transport_data_t *data = (ph_rtp_transport_data_t*) (t->data);

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
	ph_rtp_transport_data_t *data = (ph_rtp_transport_data_t*) (t->data);

	if (!data)
	{
		return -1;
	}

	return ph_transport_common_recvfrom(data, data->rtp_sock, buffer, len, flags, from, fromlen);
}

int ph_rtcp_transport_recvfrom(struct _RtpTransport *t, void *buffer, size_t len, int flags, 
							   struct sockaddr *from, socklen_t *fromlen)
{
	ph_rtp_transport_data_t *data = (ph_rtp_transport_data_t*) (t->data);

	if (!data)
	{
		return -1;
	}

	return ph_transport_common_recvfrom(data, data->rtcp_sock, buffer, len, flags, from, fromlen);
}

int ph_media_cleanup()
{
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
	return ;
}
/************************************************************************/
int ph_msession_start(struct ph_msession_s *s, const char *deviceid)
{
	return 0;
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
	return 0;
}

/**
 * @brief resume a media session
 */
int ph_msession_resume(struct ph_msession_s *s, int traffictype, const char *device)
{
	return 0;
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
	return ;
}

extern char offset127; 
extern char offset0xD5; 
extern char offset0;

/* 
 * IMPORTANT : some compiler don't support the tagged-field syntax. Those
 * macros are there to trap the problem This means that if you want to keep
 * portability, payload types must be defined with their fields in the right
 * order.
 */
#if defined(_ISOC99_SOURCE)
// ISO C99's tagged syntax
#define TYPE(val)		.type=(val)
#define CLOCK_RATE(val)		.clock_rate=(val)
#define BITS_PER_SAMPLE(val)	.bits_per_sample=(val)
#define ZERO_PATTERN(val)	.zero_pattern=(val)
#define PATTERN_LENGTH(val)	.pattern_length=(val)
#define NORMAL_BITRATE(val)	.normal_bitrate=(val)
#define MIME_TYPE(val)		.mime_type=(val)
#define FMTP(val)		.FMTP=(val)
#elif defined(__GNUC__)
// GCC's legacy tagged syntax (even old versions have it)
#define TYPE(val)		type: (val)
#define CLOCK_RATE(val)		clock_rate: (val)
#define BITS_PER_SAMPLE(val)	bits_per_sample: (val)
#define ZERO_PATTERN(val)	zero_pattern: (val)
#define PATTERN_LENGTH(val)	pattern_length: (val)
#define NORMAL_BITRATE(val)	normal_bitrate: (val)
#define MIME_TYPE(val)		mime_type: (val)
#define FMTP(val)		FMTP: (val)
#else
// No tagged syntax supported
#define TYPE(val)		(val)
#define CLOCK_RATE(val)		(val)
#define BITS_PER_SAMPLE(val)	(val)
#define ZERO_PATTERN(val)	(val)
#define PATTERN_LENGTH(val)	(val)
#define NORMAL_BITRATE(val)	(val)
#define MIME_TYPE(val)		(val)
#define FMTP(val)		(val)

#endif

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
*/

static PayloadType mspeex_nb={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(8000),
  BITS_PER_SAMPLE(320),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(15000),    // set mode MR 122 as default 
  MIME_TYPE ( CODEC_SPEEX_MIME_STRING)
};

static PayloadType mspeex_wb={
  TYPE( PAYLOAD_AUDIO_PACKETIZED),
  CLOCK_RATE(16000),
  BITS_PER_SAMPLE(0),
  ZERO_PATTERN(NULL),
  PATTERN_LENGTH(0),
  NORMAL_BITRATE(28000),
  MIME_TYPE ( CODEC_SPEEX_MIME_STRING)
};

/*
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

static PayloadType aac_lc={
  PAYLOAD_AUDIO_CONTINUOUS,
  16000,
  16,
  &offset0,
  1,
  128000,
  CODEC_AACLC_MIME_STRING
};

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
  rtp_profile_set_payload(profile,PH_MEDIA_G726WB_PAYLOAD, &g726_wb);
  */
  rtp_profile_set_payload(profile,PH_MEDIA_SPEEXNB_PAYLOAD, &mspeex_nb);
  rtp_profile_set_payload(profile,PH_MEDIA_SPEEXWB_PAYLOAD, &mspeex_wb);
  rtp_profile_set_payload(profile,PH_MEDIA_AACLC_PAYLOAD, &aac_lc);

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
	return 0;
}

void ph_media_send_stop(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort)
{
	return ;
}

int ph_media_recv_start(const char *userId,char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight,void *pClass,void *func)
{
	return 0;
}

void ph_media_recv_stop(const char *userId)
{
	return ;
}

//发送桌面共享
int ph_screen_share_send_start(char * ScreenIP,int ScreenPort,int x,int y,int width,int height)
{
	return 0;
}

//取消桌面共享
void ph_screen_share_send_stop(char * ScreenIP,int ScreenPort)
{
	return ;
}

//接收桌面共享
int ph_screen_share_recv_start(char * ScreenIP,int ScreenPort,HWND remoteHwnd,int videoWidth,int videoHeight)
{
	return 0;
}

//取消接收桌面共享
void ph_screen_share_recv_stop()
{
	return ;
}

//退出会议，关闭所有接收
void ph_exit_conference()
{
	return ;
}

//video窗口resize
void ph_resize_video_window(const char *userId,HWND remoteHwnd,int fullscreenflag)
{
	return ;
}

//禁止发言
void ph_forbid_speak(char * AudioIP,int AudioPort)
{
	return ;
}

void ph_forbid_speak2(char * userId)
{
	return ;
}

//允许发言
void ph_permit_speak(char * AudioIP,int AudioPort)
{
	return ;
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