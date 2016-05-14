#ifndef IRECEIVE_HANDLER_H
#define IRECEIVE_HANDLER_H

#include "mediastreamer2/msqueue.h"

class IFrameDeliverer
{
public:
  virtual int DeliverFrame(char *data, uint32_t timestamp) = 0;
};

#endif