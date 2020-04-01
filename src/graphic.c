#include "graphic.h"

extern char hankaku[4096];

void init_palette()
{
    static unsigned char table_rgb[16 * 3] = {
        // clang-format off
		0x00,0x00,0x00, //黒
		0xff,0x00,0x00, //明赤
		0x00,0xff,0x00, //明緑
		0xff,0xff,0x00, //明黄
		0x00,0x00,0xff, //明青
		0xff,0x00,0xff, //ggrks
		0x00,0xff,0xff,
		0xff,0xff,0xff,
		0xc6,0xc6,0xc6,
		0x84,0x00,0x00,
		0x00,0x84,0x00,
		0x84,0x84,0x00, 
		0x00,0x00,0x84, 
		0x84,0x00,0x84,
		0x00,0x84,0x84,
		0x84,0x84,0x84,
        // clang-format on
    };
    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char* rgb)
{
    int i, eflags;
    eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
    return;
}
void boxfill8(unsigned char* vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1)
{
    int x, y;
    for (x = x0; x <= x1; x++) {
        for (y = y0; y <= y1; y++) {
            vram[xsize * y + x] = color;
        }
    }
    return;
}
void init_screen(unsigned char* vram, int x, int y)
{
    boxfill8(vram, x, dark_light_blue, 0, 0, x - 1, y - 29);
    boxfill8(vram, x, bright_gray, 0, y - 28, x - 1, y - 28);
    boxfill8(vram, x, white, 0, y - 27, x - 1, y - 27);
    boxfill8(vram, x, bright_gray, 0, y - 26, x - 1, y - 1);

    boxfill8(vram, x, white, 3, y - 24, 59, y - 24);
    boxfill8(vram, x, white, 2, y - 24, 2, y - 4);
    boxfill8(vram, x, dark_gray, 3, y - 4, 59, y - 4);
    boxfill8(vram, x, dark_gray, 59, y - 23, 59, y - 5);
    boxfill8(vram, x, black, 2, y - 3, 59, y - 3);
    boxfill8(vram, x, black, 60, y - 24, 60, y - 3);

    boxfill8(vram, x, dark_gray, x - 47, y - 24, x - 4, y - 24);
    boxfill8(vram, x, dark_gray, x - 47, y - 23, x - 47, y - 4);
    boxfill8(vram, x, white, x - 47, y - 3, x - 4, y - 3);
    boxfill8(vram, x, white, x - 3, y - 24, x - 3, y - 3);
    return;
}

void put_font8(char* vram, int xsize, int x, int y, char color, char* font)
{
    int i;
    char *p, d;
    for (i = 0; i < 16; i++) {
        p = vram + (y + i) * xsize + x;
        d = font[i];
        if ((d & 0b10000000) != 0) {
            p[0] = color;
        }
        if ((d & 0b01000000) != 0) {
            p[1] = color;
        }
        if ((d & 0b00100000) != 0) {
            p[2] = color;
        }
        if ((d & 0b00010000) != 0) {
            p[3] = color;
        }
        if ((d & 0b00001000) != 0) {
            p[4] = color;
        }
        if ((d & 0b00000100) != 0) {
            p[5] = color;
        }
        if ((d & 0b00000010) != 0) {
            p[6] = color;
        }
        if ((d & 0b00000001) != 0) {
            p[7] = color;
        }
    }
}

void put_font8_asc(char* vram, int xsize, int x, int y, char color, unsigned char* s)
{
    for (; *s != 0x00; s++) {
        put_font8(vram, xsize, x, y, color, hankaku + *s * 16);
        x += 8;
    }
}

void init_mouse_cursor8(char* mouse, char bc)
{
    static char cursor[16][16] = {
        "**************..",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOOO*......",
        "*OOOOOOO*.......",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OOO*..*OOO*....",
        "*OO*....*OOO*...",
        "*O*......*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***"};
    int x, y;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = black;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = white;
            }
            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = bc;
            }
        }
    }
    return;
}

void putblock8_8(char* vram, int vxsize, int pxsize, int pysize, int px0, int py0, char* buf, int bxsize)
{
    int x, y;
    for (y = 0; y < pysize; y++) {
        for (x = 0; x < pxsize; x++) {
            vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
        }
    }
    return;
}
