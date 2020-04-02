#include "bootpack.h"
#include "address.h"
#include "dsctbl.h"
#include "fifo.h"
#include "graphic.h"
#include "interrupt.h"

void HariMain(void)
{
    bootinfo* binfo = (bootinfo*)ADR_BOOTINFO;
    init_gdtidt();
    init_pic();
    io_sti();

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    int mx = 152, my = 78;
    char s[40], mouse_cursor[256];
    mysprintf(s, "(%d, %d)", mx, my);
    put_font8_asc(binfo->vram, binfo->scrnx, 0, 0, white, s);
    init_mouse_cursor8(mouse_cursor, dark_light_blue);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mouse_cursor, 16);

    io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
    io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

    char keybuf[32];
    fifo8_init(&keyfifo, 32, keybuf);
    while (1) {
        io_cli();
        if (fifo8_status(&keyfifo) == 0) {
            io_stihlt();
        } else {
            unsigned char data = fifo8_get(&keyfifo);
            io_sti();
            mysprintf(s, "%x", data);
            boxfill8(binfo->vram, binfo->scrnx, dark_light_blue, 0, 16, 15, 31);
            put_font8_asc(binfo->vram, binfo->scrnx, 0, 16, white, s);
        }
    }
}
