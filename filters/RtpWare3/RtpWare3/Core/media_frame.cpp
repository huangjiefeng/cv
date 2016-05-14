#include "media_frame.h"
#include <memory.h>

MediaFrame::MediaFrame(void)
  : buf_(nullptr)
  , size_(0)
  , timestamp_(0)
  , is_sync_point_(true)
  , is_discontinuity(false)
{
}

MediaFrame::~MediaFrame(void)
{
  if (buf_) {
    delete buf_;
    buf_ = nullptr;
  }
}

void MediaFrame::SetData( unsigned char *src, unsigned long sz)
{
  if (!src || sz == 0) return;
  if (buf_) delete []buf_;
  size_ = sz;
  buf_ = new unsigned char[size_];
  if (!buf_) {
    //内存分配失败？！太夸张了！出现这个错误一定有内存泄露
    size_ = 0;
    return;
  }
  memcpy_s(buf_, size_, src, size_);
}
