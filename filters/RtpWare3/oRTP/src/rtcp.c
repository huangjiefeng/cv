/*
  The oRTP library is an RTP (Realtime Transport Protocol - rfc3550) stack.
  Copyright (C) 2001  Simon MORLAT simon.morlat@linphone.org

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/***************************************************************************
 *            rtcp.c
 *
 *  Wed Dec  1 11:45:30 2004
 *  Copyright  2004  Simon Morlat
 *  Email simon dot morlat at linphone dot org
 ****************************************************************************/
#include "ortp/ortp.h"
#include "ortp/rtpsession.h"
#include "ortp/rtcp.h"
#include "utils.h"
#include "rtpsession_priv.h"

#define rtcp_bye_set_ssrc(b,pos,ssrc)   (b)->ssrc[pos]=htonl(ssrc)
#define rtcp_bye_get_ssrc(b,pos)        ntohl((b)->ssrc[pos])


void rtcp_common_header_init(rtcp_common_header_t* ch, RtpSession* s, int type, int rc, int bytes_len)
{
    rtcp_common_header_set_version(ch, 2);
    rtcp_common_header_set_padbit(ch, 0);
    rtcp_common_header_set_packet_type(ch, type);
    rtcp_common_header_set_rc(ch, rc);  /* as we don't yet support multi source receiving */
    rtcp_common_header_set_length(ch, (bytes_len / 4) - 1);
}

static mblk_t* sdes_chunk_new(uint32_t ssrc)
{
    mblk_t* m = allocb(RTCP_SDES_CHUNK_DEFAULT_SIZE, 0);
    sdes_chunk_t* sc = (sdes_chunk_t*)m->b_rptr;
    sc->csrc = htonl(ssrc);
    m->b_wptr += sizeof(sc->csrc);
    return m;
}


static mblk_t* sdes_chunk_append_item(mblk_t* m, rtcp_sdes_type_t sdes_type, const char* content)
{
    if(content)
    {
        sdes_item_t si;
        si.item_type = sdes_type;
        si.len = (uint8_t)MIN(strlen(content), RTCP_SDES_MAX_STRING_SIZE);
        m = appendb(m, (char*)&si, RTCP_SDES_ITEM_HEADER_SIZE, FALSE);
        m = appendb(m, content, si.len, FALSE);
    }
    return m;
}

static void sdes_chunk_set_ssrc(mblk_t* m, uint32_t ssrc)
{
    sdes_chunk_t* sc = (sdes_chunk_t*)m->b_rptr;
    sc->csrc = htonl(ssrc);
}

#define sdes_chunk_get_ssrc(m) ntohl(((sdes_chunk_t*)((m)->b_rptr))->csrc)

static mblk_t* sdes_chunk_pad(mblk_t* m)
{
    return appendb(m, "", 1, TRUE);
}

/**
 * Set session's SDES item for automatic sending of RTCP compound packets.
 * If some items are not specified, use NULL.
**/
void rtp_session_set_source_description(RtpSession* session,
                                        const char* cname,
                                        const char* name,
                                        const char* email,
                                        const char* phone,
                                        const char* loc,
                                        const char* tool,
                                        const char* note)
{
    mblk_t* chunk = sdes_chunk_new(session->snd.ssrc);
    mblk_t* m = chunk;
    const char* _cname = cname;
    if(_cname == NULL)
    {
        _cname = "Unknown";
    }
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_CNAME, _cname);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_NAME, name);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_EMAIL, email);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_PHONE, phone);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_LOC, loc);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_TOOL, tool);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_NOTE, note);
    chunk = sdes_chunk_pad(chunk);
    if(session->sd != NULL)
        freemsg(session->sd);
    session->sd = m;
}

void rtp_session_add_contributing_source(RtpSession* session,
        uint32_t csrc,
        const char* cname,
        const char* name,
        const char* email,
        const char* phone,
        const char* loc,
        const char* tool,
        const char* note)
{
    mblk_t* chunk = sdes_chunk_new(csrc);
    mblk_t* m = chunk;
    char* _cname = (char*)cname;
    if(_cname == NULL)
    {
        _cname = "toto";
    }
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_CNAME, cname);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_NAME, name);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_EMAIL, email);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_PHONE, phone);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_LOC, loc);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_TOOL, tool);
    chunk = sdes_chunk_append_item(chunk, RTCP_SDES_NOTE, note);
    chunk = sdes_chunk_pad(chunk);
    putq(&session->contributing_sources, m);
}



mblk_t* rtp_session_create_rtcp_sdes_packet(RtpSession* session)
{
    mblk_t* mp = allocb(sizeof(rtcp_common_header_t), 0);
    rtcp_common_header_t* rtcp;
    mblk_t* tmp, * m = mp;
    queue_t* q;
    int rc = 0;
    rtcp = (rtcp_common_header_t *)mp->b_wptr;
    mp->b_wptr += sizeof(rtcp_common_header_t);

    /* concatenate all sdes chunks */
    sdes_chunk_set_ssrc(session->sd, session->snd.ssrc);
    m = concatb(m, dupmsg(session->sd));
    rc++;

    q = &session->contributing_sources;
    for(tmp = qbegin(q); !qend(q, tmp); tmp = qnext(q, mp))
    {
        m = concatb(m, dupmsg(tmp));
        rc++;
    }
    rtcp_common_header_init(rtcp, session, RTCP_SDES, rc, msgdsize(mp));
    return mp;
}


mblk_t* rtcp_create_simple_bye_packet(uint32_t ssrc, const char* reason)
{
    int packet_size;
    int strsize = 0;
    int strpadding = 0;
    mblk_t* mp;
    rtcp_bye_t* rtcp;

    packet_size = RTCP_BYE_HEADER_SIZE;
    if(reason != NULL)
    {
        strsize = (int)MIN(strlen(reason), RTCP_BYE_REASON_MAX_STRING_SIZE);
        if(strsize > 0)
        {
            strpadding = 3 - (strsize % 4);
            packet_size += 1 + strsize + strpadding;
        }
    }
    mp = allocb(packet_size, 0);

    rtcp = (rtcp_bye_t *)mp->b_rptr;
    rtcp_common_header_init(&rtcp->ch, NULL, RTCP_BYE, 1, packet_size);
    rtcp->ssrc[0] = htonl(ssrc);
    mp->b_wptr += RTCP_BYE_HEADER_SIZE;
    /* append the reason if any*/
    if(reason != NULL)
    {
        const char pad[] =
        {
            0, 0, 0
        };
        unsigned char strsize_octet = (unsigned char)strsize;

        appendb(mp, (const char*)&strsize_octet, 1, FALSE);
        appendb(mp, reason, strsize, FALSE);
        appendb(mp, pad, strpadding, FALSE);
    }
    return mp;
}

void rtp_session_remove_contributing_sources(RtpSession* session, uint32_t ssrc)
{
    queue_t* q = &session->contributing_sources;
    mblk_t* tmp;
    for(tmp = qbegin(q); !qend(q, tmp); tmp = qnext(q, tmp))
    {
        uint32_t csrc = sdes_chunk_get_ssrc(tmp);
        if(csrc == ssrc)
        {
            remq(q, tmp);
            break;
        }
    }
    tmp = rtcp_create_simple_bye_packet(ssrc, NULL);
    rtp_session_rtcp_send(session, tmp);
}

static void sender_info_init(sender_info_t* info, RtpSession* session)
{
    struct timeval tv;
    uint32_t tmp;
    gettimeofday(&tv, NULL);
    info->ntp_timestamp_msw = htonl(tv.tv_sec + 0x83AA7E80); /* 0x83AA7E80 is the number of seconds from 1900 to 1970 */
#if defined(_WIN32_WCE)
    tmp = (uint32_t)((double)tv.tv_usec * (double)(((uint64_t)1) << 32) * 1.0e-6);
#else
    tmp = (uint32_t)((double)tv.tv_usec * (double)(1LL << 32) * 1.0e-6);
#endif
    info->ntp_timestamp_lsw = htonl(tmp);
    info->rtp_timestamp = htonl(session->rtp.snd_last_ts);
    info->senders_packet_count = (uint32_t)htonl((u_long)session->rtp.stats.packet_sent);
    info->senders_octet_count = (uint32_t)htonl((u_long)session->rtp.sent_payload_bytes);
    session->rtp.last_rtcp_packet_count = session->rtp.stats.packet_sent;
}



/*static*/
void report_block_init(report_block_t* b, RtpSession* session)
{
    int packet_loss = 0;
    uint8_t loss_fraction = 0;
    RtpStream* stream = &session->rtp;
    uint32_t delay_snc_last_sr = 0;
    uint32_t fl_cnpl;

    /* compute the statistics */
    /*printf("hwrcv_extseq.one=%u, hwrcv_seq_at_last_SR=%u hwrcv_since_last_SR=%u\n",
        stream->hwrcv_extseq.one,
        stream->hwrcv_seq_at_last_SR,
        stream->hwrcv_since_last_SR
        );*/

    /******************************************************************************************************************************/
		/* lost修改！1.除以这段时间该收到包的个数 2.这段时间没有收到包 loss_fraction=255   --  modify by cyg                          */
    /******************************************************************************************************************************/

 if(stream->hwrcv_seq_at_last_SR != 0)
    {  
       packet_loss = (stream->hwrcv_extseq - stream->hwrcv_seq_at_last_SR) - stream->hwrcv_since_last_SR;
     if(packet_loss < 0){
            packet_loss = 0;			
		}

        stream->stats.cum_packet_loss += packet_loss;

		if((stream->hwrcv_extseq == stream->hwrcv_seq_at_last_SR))
			loss_fraction= (int)(255.0 * 1.0 / 1.0);
		else
			loss_fraction = (int)(256.0 * (float)packet_loss / (float)(stream->hwrcv_extseq - stream->hwrcv_seq_at_last_SR));	
    }



    /******************************************************************************************************************************/
		/* lost修改！1.修改之前的程序代码供参考   --  modify by cyg                          */
    /******************************************************************************************************************************/
/*
    if(stream->hwrcv_seq_at_last_SR != 0)
    {
        packet_loss = (stream->hwrcv_extseq - stream->hwrcv_seq_at_last_SR) - stream->hwrcv_since_last_SR;
        if(packet_loss < 0)
            packet_loss = 0;
        stream->stats.cum_packet_loss += packet_loss;
        loss_fraction = (int)(256.0 * (float)packet_loss / (float)stream->hwrcv_since_last_SR);
    }
	*/
    /* reset them */
    stream->hwrcv_since_last_SR = 0;
    stream->hwrcv_seq_at_last_SR = stream->hwrcv_extseq;

    if(stream->last_rcv_SR_time.tv_sec != 0)
    {
        struct timeval now;
        float delay;
        gettimeofday(&now, NULL);
        delay = (float)((now.tv_sec - stream->last_rcv_SR_time.tv_sec) * 1e6) + (now.tv_usec - stream->last_rcv_SR_time.tv_usec);
        delay = (float)(delay * 65536 * 1e-6);
        delay_snc_last_sr = (uint32_t)delay;
    }

    b->ssrc = htonl(session->rcv.ssrc);
    fl_cnpl = ((loss_fraction & 0xFF) << 24) | (stream->stats.cum_packet_loss & 0xFFFFFF);
    b->fl_cnpl = htonl(fl_cnpl);
    b->interarrival_jitter = htonl((uint32_t)stream->jittctl.inter_jitter);
    b->ext_high_seq_num_rec = htonl(stream->hwrcv_extseq);
    b->lsr = htonl(stream->last_rcv_SR_ts);
    b->delay_snc_last_sr = htonl(delay_snc_last_sr);

	if(session->mode == RTP_SESSION_RECVONLY)
	{
		/************************************************************************/
		/* 接收者从自己的RR包获取丢包率、延时，方法不能用于双角色    --  modify by qhf                          */
		/************************************************************************/
		session->rtp.lost = (float)loss_fraction * 100.0 / 256.0;
		session->rtp.jitter = stream->jittctl.inter_jitter;
	}
}

//----------Modified by CHK. 2010-09-19-------------------------------
//    -     在创建SR包时，如果没含报告块(rr=0)，那么添加一个扩展块
static int rtcp_sr_init(RtpSession* session, uint8_t* buf, int size)
{
    rtcp_sr_t* sr = (rtcp_sr_t*)buf;
    //int rr=(session->rtp.stats.packet_recv>0);
    //int sr_size=sizeof(rtcp_sr_t)-sizeof(report_block_t)+(rr*sizeof(report_block_t));

    int rr = (session->rtp.stats.packet_recv > 0) ? 1 : 0;
    int sr_size = sizeof(rtcp_sr_t) + (rr - 1) * sizeof(report_block_t) + (1 - rr) * sizeof(report_extension_t);
    if(size < sr_size)
        return 0;
    rtcp_common_header_init(&sr->ch, session, RTCP_SR, rr, sr_size);
    sr->ssrc = htonl(session->snd.ssrc);
    sender_info_init(&sr->si, session);
    /*only include a report block if packets were received*/
    if(rr)
        report_block_init(&sr->rb[0], session);
    else
    {
#ifdef _DEBUG
        struct timeval now;
        uint32_t msec_snc_last_RR = 0;
        gettimeofday(&now, NULL);
        msec_snc_last_RR = (now.tv_sec - session->rtp.last_rcv_RR_time.tv_sec) * 1000 + (now.tv_usec - session->rtp.last_rcv_RR_time.tv_usec) / 1000;
        if(msec_snc_last_RR > RTCP_DEFAULT_REPORT_INTERVAL * 2000)
            session->rtp.last_rtt = 0;      //如果从上一次收到RR包到现在时间超过默认时间的2倍，重置rtp.last_rtt
#endif // _DEBUG
        sr->ext.delay = htonl(session->rtp.last_rtt / 2);
    }
    return sr_size;
}
//---------------------END--------------------------------------------

static int rtcp_rr_init(RtpSession* session, uint8_t* buf, int size)
{
    rtcp_rr_t* rr = (rtcp_rr_t*)buf;
    if(size < sizeof(rtcp_rr_t))
        return 0;
    rtcp_common_header_init(&rr->ch, session, RTCP_RR, 1, sizeof(rtcp_rr_t));
    rr->ssrc = htonl(session->snd.ssrc);
    report_block_init(&rr->rb[0], session);

    return sizeof(rtcp_rr_t);
}

static int rtcp_app_init(RtpSession* session, uint8_t* buf, uint8_t subtype, const char* name, int size)
{
    rtcp_app_t* app = (rtcp_app_t*)buf;
    if(size < sizeof(rtcp_app_t))
        return 0;
    rtcp_common_header_init(&app->ch, session, RTCP_APP, subtype, size);
    app->ssrc = htonl(session->snd.ssrc);
    memset(app->name, 0, 4);
    strncpy(app->name, name, 4);
    return sizeof(rtcp_app_t);
}

static mblk_t* make_rr(RtpSession* session)
{
    mblk_t* cm = NULL;
    mblk_t* sdes = NULL;

    cm = allocb(sizeof(rtcp_sr_t), 0);
    cm->b_wptr += rtcp_rr_init(session, cm->b_wptr, sizeof(rtcp_rr_t));
    /* make a SDES packet */
    if(session->sd != NULL)
        sdes = rtp_session_create_rtcp_sdes_packet(session);
    /* link them */
    cm->b_cont = sdes;
    return cm;
}


static mblk_t* make_sr(RtpSession* session)
{
    mblk_t* cm = NULL;
    mblk_t* sdes = NULL;

    cm = allocb(sizeof(rtcp_sr_t), 0);
    cm->b_wptr += rtcp_sr_init(session, cm->b_wptr, sizeof(rtcp_sr_t));
    /* make a SDES packet */
    if(session->sd != NULL)
        sdes = rtp_session_create_rtcp_sdes_packet(session);
    /* link them */
    cm->b_cont = sdes;
    return cm;
}

static  const uint32_t RTCP_SR_INTERVAL = 3000;     //SR包发送间隔为3000ms
static  const uint32_t RTCP_RR_INTERVAL = 1000;     //RR包发送间隔为1000ms

void rtp_session_rtcp_process_send(RtpSession* session)
{
    RtpStream* st = &session->rtp;
    mblk_t* m;
    uint32_t currentTime = timeGetTime();
    if(currentTime - st->last_rtcp_report_snt_sr_t >= RTCP_SR_INTERVAL)
    {
        st->last_rtcp_report_snt_s = st->snd_last_ts;

        m = make_sr(session);
        /* send the compound packet */
        rtp_session_rtcp_send(session, m);
        ortp_debug("Rtcp compound message sent.");
        st->last_rtcp_report_snt_sr_t = currentTime;
    }
}




void rtp_session_rtcp_process_recv(RtpSession* session)		//该函数一般只发送RR包
{
    RtpStream* st = &session->rtp;
    mblk_t* m = NULL;

    //oRtp修改后用userts控制rtcp发送间隔的控制方式失效了，这里改成直接用时间来控制RR包发送间隔
    //...................................................
    uint32_t currentTime = timeGetTime();

    if(currentTime - st->last_rtcp_report_snt_rr_t >=  RTCP_RR_INTERVAL)
    {
        st->last_rtcp_report_snt_r = st->rcv_last_app_ts;
        /* if(session->rtp.last_rtcp_packet_count< session->rtp.stats.packet_sent)
         {
             m = make_sr(session);
             session->rtp.last_rtcp_packet_count = session->rtp.stats.packet_sent;
        	 st->last_rtcp_report_snt_sr_t = currentTimes;
         }
         else */
        if(session->rtp.stats.packet_recv > 0)
        {
            /*don't send RR when no packet are received yet*/
            m = make_rr(session);
            st->last_rtcp_report_snt_rr_t = currentTime;
        }
        if(m != NULL)
        {
            /* send the compound packet */
            rtp_session_rtcp_send(session, m);
            ortp_debug("Rtcp compound message sent.");
        }
    }
}


/************************************************************************/
/* ortp中原来的SR、RR发送间隔控制 函数           */
void rtp_session_rtcp_process_send_2(RtpSession *session)
{
    RtpStream *st = &session->rtp;
    mblk_t *m;
    if (st->rcv_last_app_ts - st->last_rtcp_report_snt_r > st->rtcp_report_snt_interval
            || st->snd_last_ts - st->last_rtcp_report_snt_s > st->rtcp_report_snt_interval)
    {
        st->last_rtcp_report_snt_r = st->rcv_last_app_ts;
        st->last_rtcp_report_snt_s = st->snd_last_ts;
        m = make_sr(session);
        /* send the compound packet */
        rtp_session_rtcp_send(session, m);
        ortp_debug("Rtcp compound message sent.");
    }
}

void rtp_session_rtcp_process_recv_2(RtpSession *session)
{
    RtpStream *st = &session->rtp;
    mblk_t *m = NULL;
    if (st->rcv_last_app_ts - st->last_rtcp_report_snt_r > st->rtcp_report_snt_interval
            || st->snd_last_ts - st->last_rtcp_report_snt_s > st->rtcp_report_snt_interval)
    {
        st->last_rtcp_report_snt_r = st->rcv_last_app_ts;
        st->last_rtcp_report_snt_s = st->snd_last_ts;

        if (session->rtp.last_rtcp_packet_count < session->rtp.stats.packet_sent)
        {
            m = make_sr(session);
            session->rtp.last_rtcp_packet_count = session->rtp.stats.packet_sent;
        }
        else if (session->rtp.stats.packet_recv > 0)
        {
            /*don't send RR when no packet are received yet*/
            m = make_rr(session);
        }
        if (m != NULL)
        {
            /* send the compound packet */
            rtp_session_rtcp_send(session, m);
            ortp_debug("Rtcp compound message sent.");
        }
    }
}
/************************************************************************/



void rtp_session_send_rtcp_APP(RtpSession* session, uint8_t subtype, const char* name, const uint8_t* data, int datalen)
{
    mblk_t* h = allocb(sizeof(rtcp_app_t), 0);
    mblk_t* d;
    h->b_wptr += rtcp_app_init(session, h->b_wptr, subtype, name, datalen + sizeof(rtcp_app_t));
    d = esballoc((uint8_t *)data, datalen, 0, NULL);
    h->b_cont = d;
    rtp_session_rtcp_send(session, h);
}

/**
 * Sends a RTCP bye packet.
 *@param session RtpSession
 *@param reason the reason phrase.
**/
int rtp_session_bye(RtpSession* session, const char* reason)
{
    mblk_t* cm;
    mblk_t* sdes = NULL;
    mblk_t* bye = NULL;
    int ret;

    /* Make a BYE packet (will be on the end of the compund packet). */
    bye = rtcp_create_simple_bye_packet(session->snd.ssrc, reason);

    /* SR or RR is determined by the fact whether stream was sent*/
    if(session->rtp.stats.packet_sent > 0)
    {
        cm = allocb(sizeof(rtcp_sr_t), 0);
        cm->b_wptr += rtcp_sr_init(session, cm->b_wptr, sizeof(rtcp_sr_t));
        /* make a SDES packet */
        sdes = rtp_session_create_rtcp_sdes_packet(session);
        /* link them */
        concatb(concatb(cm, sdes), bye);
    }
    else if(session->rtp.stats.packet_recv > 0)
    {
        /* make a RR packet */
        cm = allocb(sizeof(rtcp_rr_t), 0);
        cm->b_wptr += rtcp_rr_init(session, cm->b_wptr, sizeof(rtcp_rr_t));
        /* link them */
        cm->b_cont = bye;
    }
    else
        cm = bye;

    /* Send compound packet. */
    ret = rtp_session_rtcp_send(session, cm);

    return ret;
}

