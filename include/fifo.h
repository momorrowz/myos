#ifndef __FIFO_H__
#define __FIFO_H__

#define FLAGS_OVERRUN 0x001

struct FIFO8 {
    unsigned char* buf;
    int p, q, size, free, flags;
};

extern struct FIFO8 keyfifo;
void fifo8_init(struct FIFO8* fifo, int size, unsigned char* buf);
int fifo8_put(struct FIFO8* fifo, unsigned char data);
int fifo8_get(struct FIFO8* fifo);
int fifo8_status(struct FIFO8* fifo);
#endif