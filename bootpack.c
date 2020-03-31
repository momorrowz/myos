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

/* 他のファイルで作った関数がありますとCコンパイラに教える */
// nasmfunc.asmに本体
extern void io_hlt();
extern void io_cli();
extern void io_out8(int port, int data);
extern int io_load_eflags();
extern void io_store_eflags(int eflags);

// Mainの下
void init_palette();
void set_palette(int start, int end, unsigned char* rgb);
void boxfill8(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void HariMain(void)
{
    char* vram = (char*)0xa0000;
    int xsize = 320, ysize = 200;
    init_palette();
    boxfill8(vram, xsize, bright_red, 20, 20, 120, 120);
    boxfill8(vram, xsize, bright_green, 70, 50, 170, 150);
    boxfill8(vram, xsize, bright_blue, 120, 80, 220, 180);
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
void boxfill8(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int x, y;
    for (x = x0; x <= x1; x++) {
        for (y = y0; y <= y1; y++) {
            vram[xsize * y + x] = c;
        }
    }
    return;
}