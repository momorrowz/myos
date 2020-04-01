#include "bootpack.h"
#include "address.h"
#include "dsctbl.h"
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

    while (1) {
        io_hlt();
    }
}
