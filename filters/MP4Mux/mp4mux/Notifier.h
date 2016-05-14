#pragma once

#include "RecordStat.h"

class Notifier
{
	friend class Mpeg4Mux;

	// callback
	void * m_pCallbackCaller;
	RecordStatCallback m_pCallbackFunc;

	static Notifier * instance;

	Notifier():m_pCallbackCaller(NULL), m_pCallbackFunc(NULL) {}

public:
    struct MediaFormat {
        int video_width;
        int video_height;
        int video_fps;
        int audio_sample_rate;
        int audio_channel;
        int audio_bits_count;
        long long start_time; // ms from 1970-1-1
        long long duration; // ms
    };

public:

	static Notifier * getInstance()
	{
		if (instance == NULL)
		{
			instance = new Notifier;
		}
		return instance;
	}

	void MuxRun();
    void MuxStop();
    void FileCreate(long job_id, const char * file);
    void FileStartWrite(long job_id, const char * file);
    void FileWriting(long job_id, const char * file, long long duration, long long size);
    void FileAbandon(long job_id, const char * file);
    void FileClose(long job_id, const char * file, MediaFormat *fmt);
	void FileCutRequest(const char * cur, const char * next);

private:
	void NotifyRecFileInfo(long job_id, RecordStatCode code, const char * file);
};

