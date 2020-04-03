#ifndef __MOUSE_H__
#define __MOUSE_H__
#include "fifo.h"

extern struct FIFO8 mousefifo;
struct MOUSE_DEC {
    unsigned char buf[3], phase;
    int x, y, btn;
};

#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4

void enable_mouse(struct MOUSE_DEC* mdec);
int mouse_decode(struct MOUSE_DEC* mdec, unsigned char data);

#endif