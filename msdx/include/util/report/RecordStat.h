#ifndef RECORD_STAT_H
#define RECORD_STAT_H


// Definition of record status

enum RecordStatCode
{
    REC_STAT_MUX_RUN,
    REC_STAT_MUX_STOP,
	REC_STAT_FILE_CREATE, // rec
	REC_STAT_FILE_START, // rec
	REC_STAT_FILE_ABANDON, // rec
    REC_STAT_FILE_WRITING, // rec
    REC_STAT_FILE_CLOSE, // close
	REC_STAT_CUT_REQUEST, // cut
};


typedef struct _RecFileInfo
{
    long job_id;
    char file[256];
    unsigned long long duration; //时长，精度ms，最大2^64/86,400,000=200亿天
    unsigned long long size; //文件已写入字节数
} RecFileInfo;

typedef struct _CutFileInfo
{
	char cur[256];
	char next[256];
} CutFileInfo;

typedef struct _CloseFileInfo {
    long job_id;
    char file[256];
    unsigned long long start_time; // ms from 1970-1-1
    unsigned long long duration; // ms
    int video_width;
    int video_height;
    int video_fps;
    int audio_sample_rate;
    int audio_channel;
    int audio_bits_count;
} CloseFileInfo;

typedef struct _RecStatItem
{
    __int64 time;
	char user_name[60];	/* 媒体流成员的账号名称，如果是发送媒体流则为send*/
	RecordStatCode statType;

	union
	{
		RecFileInfo rec;
		CutFileInfo cut;
        CloseFileInfo close;
	};

} RecStatItem;


typedef void (*RecordStatCallback)(void * obj, RecStatItem &stat);



// definition of file name format

#define TX_FNFMT_DATE	"{date}" // yyyy-MM-dd
#define TX_FNFMT_DATETIME	"{datetime}" // yyyyMMddhhmmss
#define TX_FNFMT_EPISODE	"{episode}"  // eee
#define TX_FNFMT_VINFO	"{vinfo}"  // 1080p15fps


#endif