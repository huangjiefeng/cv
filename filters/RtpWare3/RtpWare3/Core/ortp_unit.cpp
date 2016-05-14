#include "ortp_unit.h"
#include <ortp/str_utils.h>

OrtpUnit::OrtpUnit(mblk_t *m)
{
  m_ = dupmsg(m);
  msgpullup(m_, -1); //整合数据块链表，否则不能用GetData取得完整数据
}

OrtpUnit::OrtpUnit( ISendUnit *u )
{
  if (!u) {
    m_ = nullptr;
    return;
  }
  m_ = allocb(u->GetSize()+10, 0);
  if (m_) {
    memcpy(m_->b_wptr, u->GetData(), u->GetSize());
    m_->b_wptr += u->GetSize();
  }
}

OrtpUnit::OrtpUnit( const OrtpUnit &u )
{
  m_ = dupmsg(u.m_);
}

OrtpUnit::~OrtpUnit()
{
  if (m_) {
    freemsg(m_);
    m_ = nullptr;
  }
}

unsigned char * OrtpUnit::GetData()
{
  if (!m_) return nullptr;
  return m_->b_rptr;
}

unsigned long OrtpUnit::GetSize()
{
  if (!m_) return 0;
  return msgdsize(m_);
}

mblk_t * OrtpUnit::GetMsgb()
{
  if (!m_) return nullptr;
  //注意：该函数不会复制实际数据，只会增加一份引用，并重新创建相关指针
  return dupmsg(m_);
}
