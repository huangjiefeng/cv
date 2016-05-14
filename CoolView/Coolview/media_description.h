#ifndef MEDIA_DESCRIPTION_H
#define MEDIA_DESCRIPTION_H

enum MediaStreamType {
  kUnknowType = -1,
  kStreamType,
  kSmallStreamType,
  kScreenType,
};

enum MediaControlType {
  kUnknowControlType = -1,
  kAudioControlType = 0,
  kVideoControlType = 1,
  kScreenControlType = 2,
  kRecordControlType = 3,
};

enum AudioDeviceType {
  kSpeaker,
  kMicrophone,
};

class MediaHelper {
public:
  static void RegisterMetaType() {
    qRegisterMetaType<MediaStreamType>("MediaStreamType");
    qRegisterMetaType<MediaControlType>("MediaControlType");
    qRegisterMetaType<AudioDeviceType>("AudioDeviceType");
  }
};

#endif // MEDIA_DESCRIPTION_H