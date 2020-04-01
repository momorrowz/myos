#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

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

void init_palette();
void set_palette(int start, int end, unsigned char* rgb);
void boxfill8(unsigned char* vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1);
void init_screen(unsigned char* vram, int xsize, int ysize);
void put_font8(char* vram, int xsize, int x, int y, char color, char* font);
void put_font8_asc(char* vram, int xsize, int x, int y, char color, unsigned char* s);
void putblock8_8(char* vram, int vxsize, int pxsize, int pysize, int px0, int py0, char* buf, int bxsize);
void init_mouse_cursor8(char* mouse, char bc);

#endif