#include "mouse.h"
#include "keyboard.h"

struct FIFO8 mousefifo;
void enable_mouse(void)
{
    /* マウス有効 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    return; /* うまくいくとACK(0xfa)が送信されてくる */
}
