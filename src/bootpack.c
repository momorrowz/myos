#include "bootpack.h"
#include "address.h"
#include "dsctbl.h"
#include "fifo.h"
#include "graphic.h"
#include "interrupt.h"
#include "keyboard.h"
#include "mouse.h"

void HariMain(void)
{
    bootinfo* binfo = (bootinfo*)ADR_BOOTINFO;
    init_gdtidt();
    init_pic();
    io_sti();
    char keybuf[32], mousebuf[128], s[40], mouse_cursor[256];
    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);
    io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
    io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

    init_keyboard();
    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    int mx = 152, my = 78;
    mysprintf(s, "(%d, %d)", mx, my);
    put_font8_asc(binfo->vram, binfo->scrnx, 0, 0, white, s);
    init_mouse_cursor8(mouse_cursor, dark_light_blue);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mouse_cursor, 16);

    struct MOUSE_DEC mdec;
    enable_mouse(&mdec);

    while (1) {
        io_cli();
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
            io_stihlt();
        } else {
            if (fifo8_status(&keyfifo) != 0) {
                int data = fifo8_get(&keyfifo);
                io_sti();
                mysprintf(s, "%x", data);
                boxfill8(binfo->vram, binfo->scrnx, dark_light_blue, 0, 16, 15, 31);
                put_font8_asc(binfo->vram, binfo->scrnx, 0, 16, white, s);
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
                    boxfill8(binfo->vram, binfo->scrnx, dark_light_blue, 32, 16, 32 + 15 * 8 - 1, 31);
                    put_font8_asc(binfo->vram, binfo->scrnx, 32, 16, white, s);
                    boxfill8(binfo->vram, binfo->scrnx, dark_light_blue, mx, my, mx + 15, my + 15);
                    mx += mdec.x;
                    my += mdec.y;
                    if (mx < 0) {
                        mx = 0;
                    }
                    if (my < 0) {
                        my = 0;
                    }
                    if (mx > binfo->scrnx - 16) {
                        mx = binfo->scrnx - 16;
                    }
                    if (my > binfo->scrny - 16) {
                        my = binfo->scrny - 16;
                    }
                    mysprintf(s, "(%d, %d)", mx, my);
                    boxfill8(binfo->vram, binfo->scrnx, dark_light_blue, 0, 0, 79, 15);
                    put_font8_asc(binfo->vram, binfo->scrnx, 0, 0, white, s);
                    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mouse_cursor, 16);
                }
            }
        }
    }
}