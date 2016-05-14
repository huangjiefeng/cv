#include "terminal_description.h"
#include "msdx/config.h"

#include <cassert>

#include <QStringList>

bool TerminalDescription::IsSameWithoutTotalNumCompared( const TerminalDescription &other ) const {
  assert(uri == other.uri);
  assert(virtual_uri == other.virtual_uri);
  assert(virtual_index == other.virtual_index);

  // TODO: 当终端序号是0时，才比较:
  // is_speaking, is_handup, is_main_speaker,

  return (
    is_available == other.is_available &&
    is_speaking == other.is_speaking &&
    is_handup == other.is_handup &&
    is_main_speaker == other.is_main_speaker &&
    virtual_count == other.virtual_count &&
    name == other.name &&
    has_video == other.has_video &&
    has_audio == other.has_audio &&
    video_port == other.video_port &&
    audio_port == other.audio_port &&
    small_video_port == other.small_video_port &&
    sdp == other.sdp &&
    version == other.version &&
    dns == other.dns &&
    location == other.location &&
    model == other.model &&
    multicast_address == other.multicast_address);
}

void TerminalHelper::GetMediaInfoFromSDP(const QString &sdp, 
                                         int &video_width, 
                                         int &video_height,
                                         int &video_fps, 
                                         QString &audio_codec,
                                         int &audio_rate, 
                                         int &audio_bits,
                                         int &audio_channel) {
  // video
  if (!sdp.contains("m=video")) {
    video_width = 0;
    video_height = 0;
    video_fps = 0;
  } else {
    int index1 = sdp.indexOf("CUSTOM=");
    int index2 = sdp.indexOf(" MaxBR", index1);
    QString temp1 = sdp.mid(index1 + 7, index2 - index1 - 7).trimmed();
    int index3 = temp1.indexOf(",");
    QString swidth = temp1.left(index3);
    QString sheight = temp1.mid(index3 + 1);

    video_width = swidth.toInt();
    video_height = sheight.toInt();

    index1 = sdp.indexOf("FPS=");
    if( index1 > 0 ) {
      index1 += strlen("FPS=");
      index2 = sdp.indexOf("a", index1);
      temp1 = sdp.mid(index1, index2 - index1).trimmed();
      video_fps = temp1.toInt();
    } else {
      // 默认值
      video_fps = 30;
    }
  }

  // audio
  // example:
  /*
  ...
  m=audio  RTP/AVP 116
  a=ptime:20
  a=rtpmap:116 AAC_LC/44100/2
  m=video  RTP/AVP 34 
  ...
  */
  //初始化默认值
  audio_codec = MSDX_CONF_AUDIO_CODEC_SPEEX;
  audio_rate = 16000;
  audio_bits = 16;
  audio_channel = 1;
  if (sdp.contains("m=audio")) {
    int index1 = sdp.indexOf("m=audio");
    int index2 = sdp.indexOf("m=video", index1);
    int index3 = sdp.indexOf("a=rtpmap:", index1);
    if (index2 > index3) { // 确保是audio的rtpmap
      int index_start = sdp.indexOf(" ", index3) + 1;
      int index_end = sdp.indexOf("=", index_start) - 1;
      QString audio_rtpmap = sdp.mid(index_start, 
        index_end - index_start).trimmed(); // 查找AAC_LC/44100/2
      QStringList audio_param = audio_rtpmap.split("/");
      if (audio_param.length() >= 2) {
        audio_codec = audio_param[0];
        audio_rate = audio_param[1].toInt();
      }
      if (audio_param.length() >= 3) {
        audio_channel = audio_param[2].toInt();
      }
    }
  }
}
