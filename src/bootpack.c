//色
#define black 0
#define bright_red 1
#define bright_green 2
#define bright_yellow 3
#define bright_blue 4
#define bright_purple 5
#define bright_light_blue 6
#define white 7
#define bright_gray 8
#define dark_red 9
#define dark_green 10
#define dark_yellow 11
#define dark_blue 12
#define dark_purple 13
#define dark_light_blue 14
#define dark_gray 15

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char* vram;
} typedef bootinfo;
struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};


// nasmfunc.asmに本体

extern void io_hlt();
extern void io_cli();
extern void io_out8(int port, int data);
extern int io_load_eflags();
extern void io_store_eflags(int eflags);

// 描画
void init_palette();
void set_palette(int start, int end, unsigned char* rgb);
void boxfill8(unsigned char* vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1);
void init_screen(unsigned char* vram, int xsize, int ysize);
void put_font8(char* vram, int xsize, int x, int y, char color, char* font);
void put_font8_asc(char* vram, int xsize, int x, int y, char color, unsigned char* s);
void putblock8_8(char* vram, int vxsize, int pxsize, int pysize, int px0, int py0, char* buf, int bxsize);
void init_mouse_cursor8(char* mouse, char bc);

//GDT関係
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR* sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR* gd, int offset, int selector, int ar);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

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

void init_gdtidt(void)
{
    struct SEGMENT_DESCRIPTOR* gdt = (struct SEGMENT_DESCRIPTOR*)0x00270000;
    struct GATE_DESCRIPTOR* idt = (struct GATE_DESCRIPTOR*)0x0026f800;
    int i;

    /* GDTの初期化 */
    for (i = 0; i < 8192; i++) {
        set_segmdesc(gdt + i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
    set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
    load_gdtr(0xffff, 0x00270000);

    /* IDTの初期化 */
    for (i = 0; i < 256; i++) {
        set_gatedesc(idt + i, 0, 0, 0);
    }
    load_idtr(0x7ff, 0x0026f800);

    return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR* sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff) {
        ar |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }
    sd->limit_low = limit & 0xffff;
    sd->base_low = base & 0xffff;
    sd->base_mid = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high = (base >> 24) & 0xff;
    return;
}

void set_gatedesc(struct GATE_DESCRIPTOR* gd, int offset, int selector, int ar)
{
    gd->offset_low = offset & 0xffff;
    gd->selector = selector;
    gd->dw_count = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high = (offset >> 16) & 0xffff;
    return;
}