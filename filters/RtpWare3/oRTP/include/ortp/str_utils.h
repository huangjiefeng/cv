/*
  The oRTP library is an RTP (Realtime Transport Protocol - rfc3550) stack.
  Copyright (C) 2001  Simon MORLAT simon.morlat@linphone.org

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* 
	modified by qhf£¬Ìí¼Ó×¢ÊÍ
*/


#ifndef STR_UTILS_H
#define STR_UTILS_H


#include <ortp/port.h>



/************************************************************************/
/*   The msgb structure defines a message block.  A message block carries data
or information in a stream.  A STREAMS message consists of message blocks
linked through b_cont.  Each message block points to a data block descriptor, which in turn points to a data buffer.

The msgb structure is typedefed as mblk_t.  The associated data block is stored in a datab structure,
which is typedefed as dblk_t.                                                                     */
/************************************************************************/
//Defines a STREAMS message block
typedef struct msgb
{
    struct msgb *b_prev;	//A pointer to the previous message on the queue.
    struct msgb *b_next;	  //A pointer to the next message on the queue.
    struct msgb *b_cont;	// A pointer to the next message block in the message.
    struct datab *b_datap;	//A pointer to the datab structure (data block) that contains the data for the message.
    unsigned char *b_rptr;	// A pointer to the first unread data byte in the buffer.
    unsigned char *b_wptr;	// A pointer to the first unwritten data byte in the buffer.
    uint32_t reserved1;		// Additional fields that are visible within the kernel.
    uint32_t reserved2;
} mblk_t;

typedef struct datab
{
    unsigned char *db_base;	// The first byte of the buffer.
    unsigned char *db_lim;	// The last byte of the buffer, plus one.
    void (*db_freefn)(void*);
	int db_ref;		 //  The number of message blocks ( struct msgb) that reference this	data block.
} dblk_t;

typedef struct _queue
{
    mblk_t _q_stopper;
    int q_mcount;	/*number of packet in the q */
} queue_t;

#ifdef __cplusplus
extern "C" {
#endif

    void qinit(queue_t *q);

    void putq(queue_t *q, mblk_t *m);

    mblk_t * getq(queue_t *q);

    void insq(queue_t *q, mblk_t *emp, mblk_t *mp);

    void remq(queue_t *q, mblk_t *mp);

    mblk_t * peekq(queue_t *q);

    /* remove and free all messages in the q */
#define FLUSHALL 0
    void flushq(queue_t *q, int how);

    void mblk_init(mblk_t *mp);

    /* allocates a mblk_t, that points to a datab_t, that points to a buffer of size size. */
    mblk_t *allocb(int size, int unused);
#define BPRI_MED 0

    /* allocates a mblk_t, that points to a datab_t, that points to buf; buf will be freed using freefn */
    mblk_t *esballoc(uint8_t *buf, int size, int pri, void (*freefn)(void*) );

    /* frees a mblk_t, and if the datab ref_count is 0, frees it and the buffer too */
    void freeb(mblk_t *m);

    /* frees recursively (follow b_cont) a mblk_t, and if the datab
    ref_count is 0, frees it and the buffer too */
    void freemsg(mblk_t *mp);

    /* duplicates a mblk_t , buffer is not duplicated*/
    mblk_t *dupb(mblk_t *m);

    /* duplicates a complex mblk_t, buffer is not duplicated */
    mblk_t	*dupmsg(mblk_t* m);

    /* returns the size of data of a message */
    int msgdsize(const mblk_t *mp);

    /* concatenates all fragment of a complex message*/
    void msgpullup(mblk_t *mp, int len);

    /* duplicates a single message, but with buffer included */
    mblk_t *copyb(mblk_t *mp);

    /* duplicates a complex message with buffer included */
    mblk_t *copymsg(mblk_t *mp);

    mblk_t * appendb(mblk_t *mp, const char *data, int size, bool_t pad);
    void msgappend(mblk_t *mp, const char *data, int size, bool_t pad);

    mblk_t *concatb(mblk_t *mp, mblk_t *newm);

#define qempty(q) (&(q)->_q_stopper==(q)->_q_stopper.b_next)
#define qfirst(q) ((q)->_q_stopper.b_next!=&(q)->_q_stopper ? (q)->_q_stopper.b_next : NULL)
#define qbegin(q) ((q)->_q_stopper.b_next)
#define qlast(q) ((q)->_q_stopper.b_prev!=&(q)->_q_stopper ? (q)->_q_stopper.b_prev : NULL)
#define qend(q,mp)	((mp)==&(q)->_q_stopper)
#define qnext(q,mp) ((mp)->b_next)

    typedef struct _msgb_allocator
    {
        queue_t q;
    } msgb_allocator_t;

    void msgb_allocator_init(msgb_allocator_t *pa);
    mblk_t *msgb_allocator_alloc(msgb_allocator_t *pa, int size);
    void msgb_allocator_uninit(msgb_allocator_t *pa);

#ifdef __cplusplus
}
#endif

#endif
