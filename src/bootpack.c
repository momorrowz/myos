#include "dsctbl.h"
#include "graphic.h"

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char* vram;
} typedef bootinfo;

// nasmfunc.asmに本体
//extern void io_hlt();
//extern void io_cli();
//extern void io_out8(int port, int data);
//extern int io_load_eflags();
//extern void io_store_eflags(int eflags);

//void load_gdtr(int limit, int addr);
//void load_idtr(int limit, int addr);

extern char hankaku[4096];

void HariMain(void)
{
    bootinfo* binfo = (bootinfo*)0x0ff0;
    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    char* s;
    int mx = 152, my = 78;
    mysprintf(s, "(%d, %d)", mx, my);
    put_font8_asc(binfo->vram, binfo->scrnx, 0, 0, white, s);
    char mouse_cursor[256];
    init_mouse_cursor8(mouse_cursor, dark_light_blue);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mouse_cursor, 16);
    while (1) {
        io_hlt();
    }
}
