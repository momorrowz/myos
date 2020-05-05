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
    struct SHTCTL* ctl;
};
struct SHTCTL {
    unsigned char *vram, *map;
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
void sheet_updown(struct SHEET* sht, int height);
void sheet_refresh(struct SHEET* sht, int bx0, int by0, int bx1, int by1);
void sheet_refreshsub(struct SHTCTL* ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1);
void sheet_refreshmap(struct SHTCTL* ctl, int vx0, int vy0, int vx1, int vy1, int h0);
void sheet_slide(struct SHEET* sht, int vx0, int vy0);
void sheet_free(struct SHEET* sht);
#endif