#ifndef MEDIA_FRAME_H
#define MEDIA_FRAME_H

#include "receive_interfaces.h"


class MediaFrame :
  public IFrame
{
public:
  MediaFrame(void);
  ~MediaFrame(void);

  virtual bool IsSyncPoint() { return is_sync_point_; }
  virtual void SetSyncPoint(bool sync) { is_sync_point_ = sync; }

  virtual bool IsDiscontinuity() {return is_discontinuity; }
  virtual void SetDiscontinuity(bool discon) { is_discontinuity = discon; } 

  virtual unsigned long GetTimestamp() { return timestamp_; }
  virtual void SetTimestamp( unsigned long ts) {timestamp_ = ts;}

  virtual unsigned long GetDataSize() { return size_; }
  virtual unsigned char * GetData() { return buf_; }
  virtual void SetData( unsigned char *buf, unsigned long sz);

private:
  unsigned char *buf_;
  unsigned long size_;
  unsigned long timestamp_;
  bool is_sync_point_;
  bool is_discontinuity;

};

#endif

