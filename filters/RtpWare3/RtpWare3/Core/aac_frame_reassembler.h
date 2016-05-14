#ifndef AAC_FRAME_REASSEMBLER_H
#define AAC_FRAME_REASSEMBLER_H

#include "generic_frame_reassembler.h"


class AACFrameReassembler :
  public GenericFrameReassembler
{
public:
  AACFrameReassembler(ILogger *logger);
  ~AACFrameReassembler(void);

  unsigned long ReassembleFrameData( 
    unsigned short begin_pos, unsigned short end_pos );

};


#endif // !AAC_FRAME_REASSEMBLER_H
