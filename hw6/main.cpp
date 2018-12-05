#include<cstdlib>
#include<ctime>

#include "Game.h"

int main(){
    Game game;
    // TA: choose one of the following mode to start the game: humanGame,singleGame,battleAll

    //game.humanGame(4,1);
        // TA: parameters: int randSeed=4,int aiAgent=0,int viewL=0
        // TA: suggested random seed: 4,9,80,7352,8632

    //game.singleGame(time(0),0,1);
        // TA: parameters: int randSeed=4,int GreenAgent=0,int YellowAgent=1,bool showGame=true

    game.battleAll("test.csv",true);
        // TA: parameters: const char* dumpFileName,bool showGame=false

    #ifndef NO_CURSES
    printw("Please press q to exit\n");
    while( getch()!='q' ){}
    #endif

    return 0;
}
