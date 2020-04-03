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

    unsigned char mouse_dbuf[3], mouse_phase = 0;
    enable_mouse();

    while (1) {
        io_cli();
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
            io_stihlt();
        } else {
            if (fifo8_status(&keyfifo) != 0) {
                unsigned char data = fifo8_get(&keyfifo);
                io_sti();
                mysprintf(s, "%x", data);
                boxfill8(binfo->vram, binfo->scrnx, dark_light_blue, 0, 16, 15, 31);
                put_font8_asc(binfo->vram, binfo->scrnx, 0, 16, white, s);
            } else if (fifo8_status(&mousefifo) != 0) {
                unsigned char data = fifo8_get(&mousefifo);
                io_sti();
                if (mouse_phase == 0) {
                    if (data == 0xfa) {
                        mouse_phase = 1;
                    }
                } else if (mouse_phase == 1) {
                    mouse_dbuf[0] = data;
                    mouse_phase = 2;
                } else if (mouse_phase == 2) {
                    mouse_dbuf[1] = data;
                    mouse_phase = 3;

                } else if (mouse_phase == 3) {
                    mouse_dbuf[2] = data;
                    mouse_phase = 1;
                    mysprintf(s, "%x %x %x", mouse_dbuf[0], mouse_dbuf[1], mouse_dbuf[2]);
                    boxfill8(binfo->vram, binfo->scrnx, dark_light_blue, 32, 16, 32 + 8 * 8 - 1, 31);
                    put_font8_asc(binfo->vram, binfo->scrnx, 32, 16, white, s);
                }
            }
        }
    }
}
