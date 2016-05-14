#ifndef FILE_PLAY_STAT_H
#define FILE_PLAY_STAT_H

enum FilePlayStatType
{
  FILEPLAY_STAT_MEDIAINFO,
  FILEPLAY_STAT_PROGRESS
};

typedef struct _MediaFormatInfo
{
  char video_codec[16];
  long video_width;
  long video_height;
  long video_fps;
  char audio_codec[16];
  long audio_sample_rate;
  long audio_channel;
  long audio_bit_count;
  long long file_duration; //in ms unit
} MediaFormatInfo;

typedef struct _PlayProgressInfo
{
    long long current_pos; //in ms unit
    long long duration; // in ms unit
} PlayProgressInfo;

typedef struct _FilePlayStatItem
{
  char id[60];
  FilePlayStatType type;

  union
  {
    MediaFormatInfo format;
    PlayProgressInfo progress;
  };

} FilePlayStatItem;


typedef void (*FilePlayStatCallback)(void * obj, FilePlayStatItem &stat);


#endif
