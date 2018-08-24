#ifndef _screen_man_h
#define _screen_man_h


#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))
#define cursorup(x) printf("\033[%dA", (x))
#define cursordown(x) printf("\033[%dB", (x))
#define clearscr() printf("\033[H\033[J")
#define clearprev() printf("\033[3J")


#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108


int getch(void);

int kbhit(void);

int kbesc(void);

int kbget(void);

void clear_util();

#endif