#ifndef __basic_h__
#define __basic_h__

// We advise you to build up curses library.
// You can use either PDCurses or NCurses (both are cross-platform)
// If you really don't want to use them or have big trouble on that,
// you can uncomment the following line to build LittleTank without curses.
// But in so doing, you can't enjoy keyboard agent or colors anymore.

//#define NO_CURSES

#ifndef NO_CURSES
    #include "curses.h"
#else
    #include <ctime>
    #include <cstdio>
    #include <cstdlib>

    #define COLOR_WHITE  0
    #define COLOR_GREEN  0
    #define COLOR_YELLOW 1
    #define COLOR_RED    2
    typedef char chtype;

    int printw(const char *fmt, ...);
    void mvaddch(int,int,chtype c);
    void refresh();

    inline void napms(int num){
        clock_t t = clock();
        while( clock()-t<num ){}
    }
#endif

enum Direction{
    noDir = 0,
    U_Dir,
    D_Dir,
    L_Dir,
    R_Dir
};

enum Action{
    noAct = 0,
    U_Act=U_Dir,
    D_Act=D_Dir,
    L_Act=L_Dir,
    R_Act=R_Dir,
    fire
};


#endif // __basic_h__
