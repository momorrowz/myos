#ifndef __BOOTPACK_H__
#define __BOOTPACK_H__

struct BOOTINFO { /* 0x0ff0-0x0fff */
    char cyls;    /* ブートセクタはどこまでディスクを読んだのか */
    char leds;    /* ブート時のキーボードのLEDの状態 */
    char vmode;   /* ビデオモード  何ビットカラーか */
    char reserve;
    short scrnx, scrny; /* 画面解像度 */
    char* vram;
} typedef bootinfo;

#endif
