#ifndef __BOOTPACK_H__
#define __BOOTPACK_H__

#include "address.h"
#include "dsctbl.h"
#include "fifo.h"
#include "graphic.h"
#include "interrupt.h"
#include "keyboard.h"
#include "memman.h"
#include "memory.h"
#include "mouse.h"
#include "sheet.h"

struct BOOTINFO { /* 0x0ff0-0x0fff */
    char cyls;    /* ブートセクタはどこまでディスクを読んだのか */
    char leds;    /* ブート時のキーボードのLEDの状態 */
    char vmode;   /* ビデオモード  何ビットカラーか */
    char reserve;
    short scrnx, scrny; /* 画面解像度 */
    char* vram;
} typedef bootinfo;

#endif
