#include "terminaldetail.h"

TerminalDetail::TerminalDetail(QVariantMap v) {
  QVariantMap variantMap = v;
  QVariant uri = variantMap["uri"];
  uriStr = uri.toString();

  virtualCount = variantMap["virtualCount"].toInt();
  virtualIndex = variantMap["virtualIndex"].toInt();

  QVariant virtualUri = variantMap["virtualUri"];
  virtualUriStr = virtualUri.toString();

  QVariant location = variantMap["location"];
  locationStr = location.toString();

  QVariant name = variantMap["name"];
  nameStr = name.toString();

  QVariant status = variantMap["status"];
  statusStr = status.toString();

  QVariant handup = variantMap["isHandUp"];
  isHandUp = handup.toString();

  QVariant isspeaking = variantMap["isSpeaking"];
  isSpeaking = isspeaking.toString();

  QVariant ischairmanterminal = variantMap["isChairmanTerminal"];
  isChairmanTerminal =ischairmanterminal.toString();

  QVariant ismainspeaker = variantMap["isMainSpeaker"];
  isMainSpeaker = ismainspeaker.toString();

  QVariant sdp = variantMap["sdp"];
  ParseSDP(sdp.toString());

  QVariant muladdress = variantMap["multicastAddress"];
  muladdressStr = muladdress.toString();

  QVariant videoport = variantMap["videoPort"];
  videoportStr = videoport.toString().toInt();

  QVariant audioport = variantMap["audioPort"];
  audioportStr = audioport.toString().toInt();

  this->isShown = false;
  this->isReceivedAudio = false;
}

TerminalDetail::TerminalDetail( const TerminalInfo &terminal ) {
  uriStr = QString::fromUtf8(terminal.uri.c_str());
  virtualCount = terminal.virtualCount;
  virtualIndex = terminal.virtualIndex;
  virtualUriStr = QString::fromUtf8(terminal.virtualUri.c_str());
  nameStr = QString::fromUtf8(terminal.name.c_str());
  locationStr = QString::fromUtf8(terminal.location.c_str());
  statusStr = QString::fromUtf8(terminal.status.c_str());
  
  ParseSDP(QString::fromUtf8(terminal.sdp.c_str()));

  videoportStr = terminal.videoPort;
  audioportStr = terminal.audioPort;
  muladdressStr = QString::fromUtf8(terminal.multicastAddress.c_str());
  isHandUp = QString::fromUtf8(terminal.isHandUp.c_str());
  isSpeaking = QString::fromUtf8(terminal.isSpeaking.c_str());
  isMainSpeaker = QString::fromUtf8(terminal.isMainSpeaker.c_str());
  isChairmanTerminal = QString::fromUtf8(terminal.isChairmanTerminal.c_str());

  isShown = false;
  isReceivedAudio = false;
}

void TerminalDetail::ParseSDP( const QString &&sdp ) {
  hasVideo = sdp.contains("m=video");
  hasAudio = sdp.contains("m=audio");

  //// 获取视频的分辨率
  //QString vwidth, vheight;
  //if(sdp.contains("m=video")) {
  //  videoStr = QString::fromLocal8Bit("视");
  //  int index1 = sdp.indexOf("CUSTOM=");
  //  int index2 = sdp.indexOf(" MaxBR");
  //  QString temp1 = temp.remove(0, index1+7);
  //  QString temp2 = temp1.left(index2-index1-7);
  //  int index3 = temp2.indexOf(",");
  //  vwidth = temp2.left(index3);
  //  vheight = temp2.remove(0,index3+1);
  //} else {
  //  vwidth = vheight = "";
  //}
}
