#include "window.h"
#include "graphic.h"

void make_window8(unsigned char* buf, int xsize, int ysize, char* title)
{
    static char closebtn[14][16] = {
        "OOOOOOOOOOOOOOO@",
        "OQQQQQQQQQQQQQ$@",
        "OQQQQQQQQQQQQQ$@",
        "OQQQ@@QQQQ@@QQ$@",
        "OQQQQ@@QQ@@QQQ$@",
        "OQQQQQ@@@@QQQQ$@",
        "OQQQQQQ@@QQQQQ$@",
        "OQQQQQ@@@@QQQQ$@",
        "OQQQQ@@QQ@@QQQ$@",
        "OQQQ@@QQQQ@@QQ$@",
        "OQQQQQQQQQQQQQ$@",
        "OQQQQQQQQQQQQQ$@",
        "O$$$$$$$$$$$$$$@",
        "@@@@@@@@@@@@@@@@"};
    int x, y;
    char c;
    boxfill8(buf, xsize, bright_gray, 0, 0, xsize - 1, 0);
    boxfill8(buf, xsize, white, 1, 1, xsize - 2, 1);
    boxfill8(buf, xsize, bright_gray, 0, 0, 0, ysize - 1);
    boxfill8(buf, xsize, white, 1, 1, 1, ysize - 2);
    boxfill8(buf, xsize, dark_gray, xsize - 2, 1, xsize - 2, ysize - 2);
    boxfill8(buf, xsize, black, xsize - 1, 0, xsize - 1, ysize - 1);
    boxfill8(buf, xsize, bright_gray, 2, 2, xsize - 3, ysize - 3);
    boxfill8(buf, xsize, dark_blue, 3, 3, xsize - 4, 20);
    boxfill8(buf, xsize, dark_gray, 1, ysize - 2, xsize - 2, ysize - 2);
    boxfill8(buf, xsize, black, 0, ysize - 1, xsize - 1, ysize - 1);
    put_font8_asc(buf, xsize, 24, 4, white, title);
    for (y = 0; y < 14; y++) {
        for (x = 0; x < 16; x++) {
            c = closebtn[y][x];
            if (c == '@') {
                c = black;
            } else if (c == '$') {
                c = dark_blue;
            } else if (c == 'Q') {
                c = bright_gray;
            } else {
                c = white;
            }
            buf[(5 + y) * xsize + (xsize - 21 + x)] = c;
        }
    }
    return;
}
