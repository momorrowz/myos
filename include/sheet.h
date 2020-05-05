#ifndef __SHEET_H__
#define __SHEET_H__

#include "memman.h"
#include "memory.h"

#define MAX_SHEETS 256
#define SHEET_USE 1

struct SHEET {
    unsigned char* buf;
    int bxsize, bysize, vx0, vy0, col_inv, height, flags;
    // シートの大きさ、画面上の位置、色、高さ、諸情報
};
struct SHTCTL {
    unsigned char* vram;
    int xsize, ysize, top;
    //画面サイズ、一番上のsheetの高さ
    struct SHEET* sheets[MAX_SHEETS];
    //sheets0のアドレス
    struct SHEET sheets0[MAX_SHEETS];
    // sheet MAX_SHEETS分
};

struct SHTCTL* shtctl_init(struct MEMMAN* man, unsigned char* vram, int xsize, int ysize);
struct SHEET* sheet_alloc(struct SHTCTL* ctl);
void sheet_setbuf(struct SHEET* sht, unsigned char* buf, int xsize, int ysize, int col_inv);
void sheet_updown(struct SHTCTL* ctl, struct SHEET* sht, int height);
void sheet_refresh(struct SHTCTL* ctl);

void sheet_slide(struct SHTCTL* ctl, struct SHEET* sht, int vx0, int vy0);
void sheet_free(struct SHTCTL* ctl, struct SHEET* sht);
#endif