#include "bootpack.h"

void init();

void HariMain(void)
{
    bootinfo* binfo = (bootinfo*)ADR_BOOTINFO;
    init_gdtidt();
    init_pic();
    io_sti();

    //キーボードマウスの初期化
    char keybuf[32], mousebuf[128];
    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);
    io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
    io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */
    init_keyboard();
    struct MOUSE_DEC mdec;
    enable_mouse(&mdec);

    //メモリマネージャ
    struct MEMMAN* memman = (struct MEMMAN*)MEMMAN_ADDR;
    unsigned int memtotal = memtest(0x00400000, 0xbfffffff);
    memman_init(memman);
    memman_free(memman, 0x1000, 0x9e000);  //0x01000-0x9efff
    memman_free(memman, 0x400000, memtotal - 0x400000);

    //画面の初期化
    struct SHTCTL* shtctl;
    struct SHEET *sht_back, *sht_mouse, *sht_win;
    unsigned char *buf_back, buf_mouse[256], *buf_win;
    char s[40], mouse_cursor[256];
    unsigned int count = 0;
    init_palette();
    shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
    sht_back = sheet_alloc(shtctl);
    sht_mouse = sheet_alloc(shtctl);
    sht_win = sheet_alloc(shtctl);
    buf_back = (unsigned char*)memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
    buf_win = (unsigned char*)memman_alloc_4k(memman, 160 * 52);
    sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1);
    sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
    sheet_setbuf(sht_win, buf_win, 160, 52, -1);
    init_screen(buf_back, binfo->scrnx, binfo->scrny);
    init_mouse_cursor8(buf_mouse, 99);
    make_window8(buf_win, 160, 52, "counter");
    sheet_slide(sht_back, 0, 0);
    int mx = (binfo->scrnx - 16) / 2;
    int my = (binfo->scrny - 28 - 16) / 2;
    sheet_slide(sht_mouse, mx, my);
    sheet_slide(sht_win, 80, 72);
    sheet_updown(sht_back, 0);
    sheet_updown(sht_win, 1);
    sheet_updown(sht_mouse, 2);
    mysprintf(s, "(%d, %d)", mx, my);
    put_font8_asc(buf_back, binfo->scrnx, 0, 0, white, s);
    mysprintf(s, "memory %dMB   free : %dKB", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    put_font8_asc(buf_back, binfo->scrnx, 0, 32, white, s);
    sheet_refresh(sht_back, 0, 0, binfo->scrnx, 48);

    while (1) {
        //counter表示
        count++;
        mysprintf(s, "%d", count);
        boxfill8(buf_win, 160, bright_gray, 40, 28, 119, 43);
        put_font8_asc(buf_win, 160, 40, 28, black, s);
        sheet_refresh(sht_win, 40, 28, 120, 44);
        //マウスとキーボード
        io_cli();
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
            //io_stihlt();
            io_sti();
        } else {
            if (fifo8_status(&keyfifo) != 0) {
                int data = fifo8_get(&keyfifo);
                io_sti();
                mysprintf(s, "%x", data);
                boxfill8(buf_back, binfo->scrnx, dark_light_blue, 0, 16, 15, 31);
                put_font8_asc(buf_back, binfo->scrnx, 0, 16, white, s);
                sheet_refresh(sht_back, 0, 16, 16, 32);
            } else if (fifo8_status(&mousefifo) != 0) {
                int data = fifo8_get(&mousefifo);
                io_sti();
                if (mouse_decode(&mdec, data) != 0) {
                    mysprintf(s, "[lcr %d %d]", mdec.x, mdec.y);
                    if ((mdec.btn & 0x01) != 0) {
                        s[1] = 'L';
                    }
                    if ((mdec.btn & 0x02) != 0) {
                        s[3] = 'R';
                    }
                    if ((mdec.btn & 0x04) != 0) {
                        s[2] = 'C';
                    }
                    boxfill8(buf_back, binfo->scrnx, dark_light_blue, 32, 16, 32 + 15 * 8 - 1, 31);
                    put_font8_asc(buf_back, binfo->scrnx, 32, 16, white, s);
                    sheet_refresh(sht_back, 32, 16, 32 + 15 * 8, 32);
                    mx += mdec.x;
                    my += mdec.y;
                    if (mx < 0) {
                        mx = 0;
                    }
                    if (my < 0) {
                        my = 0;
                    }
                    if (mx > binfo->scrnx - 1) {
                        mx = binfo->scrnx - 1;
                    }
                    if (my > binfo->scrny - 1) {
                        my = binfo->scrny - 1;
                    }
                    mysprintf(s, "(%d, %d)", mx, my);
                    boxfill8(buf_back, binfo->scrnx, dark_light_blue, 0, 0, 79, 15);
                    put_font8_asc(buf_back, binfo->scrnx, 0, 0, white, s);
                    sheet_refresh(sht_back, 0, 0, 80, 16);
                    sheet_slide(sht_mouse, mx, my);
                }
            }
        }
    }
}