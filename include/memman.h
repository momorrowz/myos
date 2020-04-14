#ifndef __MEMMAN_H__
#define __MEMMAN_H__

#define MEMMAN_FREES 4090
#define MEMMAN_ADDR 0x003c0000

struct FREEINFO {
    unsigned int addr, size;
};

struct MEMMAN {
    int frees, maxfrees, lostsize, losts;
    struct FREEINFO free[MEMMAN_FREES];
};

void memman_init(struct MEMMAN* man);
unsigned int memman_total(struct MEMMAN* man);
unsigned int memman_alloc(struct MEMMAN* man, unsigned int size);
int memman_free(struct MEMMAN* man, unsigned int addr, unsigned int size);

#endif