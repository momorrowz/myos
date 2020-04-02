#ifndef __MOUSE_H__
#define __MOUSE_H__
#include "fifo.h"

extern struct FIFO8 mousefifo;

#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4

void enable_mouse(void);

#endif