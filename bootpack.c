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

void HariMain(void)
{
    int i;
    init_palette();
    char* p = (char*)0xa0000;
    for (i = 0; i <= 0xffff; i++) {
        p[i] = i & 0xf;
    }
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