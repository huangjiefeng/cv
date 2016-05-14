#ifndef MIME_TEXT_PARSER_AND_BUILDER_H
#define MIME_TEXT_PARSER_AND_BUILDER_H

#include <cassert>

#include <QString>
#include <QStringList>

#include "media_description.h"

class MimeTextBuilder {
public:
  MimeTextBuilder(const QString &uri) 
      : uri_(uri) {
    stream_type_ = MediaStreamType::kUnknowType;
  }
  
  void AddVirtualURI(const QString &vuri) {
    vuris_.push_back(vuri);
  }

  void SetMediaStreamType(MediaStreamType type) {
    stream_type_ = type;
  }

  QString GetText() const {
    // coolview \t type \t uri \t vuri_count \t
    QString text("coolview\t");
    text += QString("%1\t").arg(static_cast<int>(stream_type_));
    text += QString("%1\t%2\t").arg(uri_).arg(vuris_.size());
    // vuri0 \t vuri1 \t ...
    for (const QString &vuri : vuris_) {
      text += QString("%1\t").arg(vuri);
    }
    return text;
  }

private:
  QString uri_;
  QList<QString> vuris_;
  MediaStreamType stream_type_;
};

class MimeTextParser {
public:
  MimeTextParser(const QString &text) {
    // 基础的信息有coolview标识、媒体流类型、uri、虚拟终端数量，共4个
    QStringList strs = text.split('\t');
    if (strs.isEmpty() || strs[0] != "coolview" || strs.size() < 4) {
      is_coolview_mime_text_ = false;
      return;
    }

    bool res = false;
    stream_type_ = static_cast<MediaStreamType>(strs[1].toInt(&res));
    if (res == false) {
      is_coolview_mime_text_ = false;
      return;
    }
    
    uri_ = strs[2];

    const int vuri_count = strs[3].toInt(&res);
    if (res == false) {
      is_coolview_mime_text_ = false;
      return;
    }
    
    if (strs.size() <= vuri_count + 4) {
      is_coolview_mime_text_ = false;
      return;
    }

    vuris_.reserve(vuri_count);
    for (int i = 0; i < vuri_count; ++i) {
      vuris_.push_back(strs[4+i]);
    }

    is_coolview_mime_text_ = true;
  }

  bool is_coolview_mime_text() const {
    return is_coolview_mime_text_;
  }
  
  const QString& GetURI() const {
    return uri_;
  }

  MediaStreamType GetMediaStreamType() const {
    return stream_type_;
  }

  const QList<QString>& GetVURIs() {
    return vuris_;
  }

private:
  QString uri_;
  QList<QString> vuris_;
  MediaStreamType stream_type_;
  bool is_coolview_mime_text_;
};

#endif // MIME_TEXT_PARSER_AND_BUILDER_H