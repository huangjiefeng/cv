#include "stdafx.h"
#include "Notifier.h"
#include <time.h>


/************************************************************************/
/* Definition of Notifier                                               */
/************************************************************************/

Notifier * Notifier::instance = NULL;

void Notifier::MuxRun()
{
    if (m_pCallbackFunc == NULL)
    {
        return;
    }
    RecStatItem item = {0};
    item.time = time(NULL);
    item.statType = REC_STAT_MUX_RUN;
    m_pCallbackFunc(m_pCallbackCaller, item);
}

void Notifier::MuxStop()
{
    if (m_pCallbackFunc == NULL)
    {
        return;
    }
    RecStatItem item = {0};
    item.time = time(NULL);
    item.statType = REC_STAT_MUX_STOP;
    m_pCallbackFunc(m_pCallbackCaller, item);
}

void Notifier::FileCreate( long job_id, const char * file )
{
    NotifyRecFileInfo(job_id, REC_STAT_FILE_CREATE, file);
}

void Notifier::FileStartWrite( long job_id, const char * file )
{
    NotifyRecFileInfo(job_id, REC_STAT_FILE_START, file);
}

void Notifier::FileAbandon( long job_id, const char * file )
{
    NotifyRecFileInfo(job_id, REC_STAT_FILE_ABANDON, file);
}

void Notifier::FileWriting( long job_id, const char * file, long long duration, long long size )
{
    if (m_pCallbackFunc == NULL)
    {
        return;
    }
    RecStatItem item = {0};
    item.statType = REC_STAT_FILE_WRITING;
    if (file != NULL)
    {
        strcpy_s(item.rec.file, sizeof(item.rec.file), file);
    }
    item.rec.job_id = job_id;
    item.rec.duration = duration;
    item.rec.size = size;
    m_pCallbackFunc(m_pCallbackCaller, item);
}

void Notifier::NotifyRecFileInfo( long job_id, RecordStatCode code, const char * file )
{
    if (m_pCallbackFunc == NULL)
    {
        return;
    }
    RecStatItem item = {0};
    item.time = time(NULL);
    item.statType = code;

    if (file != NULL)
    {
        strcpy_s(item.rec.file, sizeof(item.rec.file), file);
    }
    item.rec.job_id = job_id;
    m_pCallbackFunc(m_pCallbackCaller, item);

}

void Notifier::FileClose(long job_id, const char *file, MediaFormat *fmt)
{
    if (m_pCallbackFunc == NULL)
    {
        return;
    }

    RecStatItem item = {0};
    item.time = time(NULL);
    item.statType = REC_STAT_FILE_CLOSE;

    item.close.job_id = job_id;
    if (file != NULL)
    {
        strcpy_s(item.close.file, sizeof(item.close.file), file);
    }
    if (fmt)
    {
        item.close.start_time = fmt->start_time;
        item.close.duration = fmt->duration;
        item.close.video_width = fmt->video_width;
        item.close.video_height = fmt->video_height;
        item.close.video_fps = fmt->video_fps;
        item.close.audio_sample_rate = fmt->audio_sample_rate;
        item.close.audio_channel = fmt->audio_channel;
        item.close.audio_bits_count = fmt->audio_bits_count;
    }
    m_pCallbackFunc(m_pCallbackCaller, item);
}

void Notifier::FileCutRequest( const char * cur, const char * next )
{
    if (m_pCallbackFunc == NULL)
    {
        return;
    }
    RecStatItem item = {0};
    item.statType = REC_STAT_CUT_REQUEST;
    if (cur != NULL)
    {
        strcpy_s(item.cut.cur, sizeof(item.cut.cur), cur);
    }
    if (next != NULL)
    {
        strcpy_s(item.cut.next, sizeof(item.cut.next), next);
    }
    m_pCallbackFunc(m_pCallbackCaller, item);
}

