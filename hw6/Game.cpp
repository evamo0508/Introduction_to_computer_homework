#include "basic.h"
#include "Game.h"

void Game::curses_init(){
    #ifndef NO_CURSES
    initscr();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    #endif
}
void Game::game_special0_init(int randSeed,int aiAgent){
    timer = 1000;
    srand(randSeed);
    map.clear();
    map.newMap();

    // team 0: tankNum=1,color: green
    // team 1: tankNum=1,color: yellow
    // team 2: tankNum=3,color: red

    teamMgr.newTeam(COLOR_GREEN);
    teamMgr.newTeam(COLOR_YELLOW);
    teamMgr.newTeam(COLOR_RED);
    tankNum = 5;
    teamMgr.initCURSES_Color();

    pTanks = new Tank*[tankNum];
    pTanks[0] = new Tank( 32,D_Dir,map,teamMgr.setToTeam(0));
    pTanks[1] = new Tank(117,U_Dir,map,teamMgr.setToTeam(1));
    pTanks[2] = new Tank( 42,L_Dir,map,teamMgr.setToTeam(2),2);
    pTanks[3] = new Tank( 82,U_Dir,map,teamMgr.setToTeam(2),2);
    pTanks[4] = new Tank(107,R_Dir,map,teamMgr.setToTeam(2),2);
    pAgents = new PolicyMaker*[tankNum];
    pAgents[0] = new HumanAgent;
    pAgents[1] = (*  agntsMgr.pAllNewAgentFunc[aiAgent])();
    pAgents[2] = new RandomAgent;
    pAgents[3] = new RandomAgent;
    pAgents[4] = new RandomAgent;
    //pAgents[1] = new HumanAgent('w','s','a','d','q');

    for(int i=0;i<tankNum;++i){
        pAgents[i]->setTank(  pTanks[i]  );
        pAgents[i]->linkMap( &map );
        pAgents[i]->linkMsslList( &Mlist );
        pAgents[i]->linkpTanks( pTanks,tankNum );
    }
}
void Game::game_standard_init(int GreenAgent,int YellowAgent,int randSeed){ // standard game
    timer = 1000;
    srand(randSeed);
    map.clear();
    map.newMap();

    // team 0: tankNum=1,color: green
    // team 1: tankNum=1,color: yellow
    // team 2: tankNum=3,color: red

    teamMgr.clear();
    teamMgr.newTeam(COLOR_GREEN);
    teamMgr.newTeam(COLOR_YELLOW);
    teamMgr.newTeam(COLOR_RED);
    tankNum = 5;
    teamMgr.initCURSES_Color();

    pTanks = new Tank*[tankNum];
    pTanks[0] = new Tank( 32,D_Dir,map,teamMgr.setToTeam(0));
    pTanks[1] = new Tank(117,U_Dir,map,teamMgr.setToTeam(1));
    pTanks[2] = new Tank( 42,L_Dir,map,teamMgr.setToTeam(2),2);
    pTanks[3] = new Tank( 82,U_Dir,map,teamMgr.setToTeam(2),2);
    pTanks[4] = new Tank(107,R_Dir,map,teamMgr.setToTeam(2),2);

    pAgents = new PolicyMaker*[tankNum];
    pAgents[0] = (*  agntsMgr.pAllNewAgentFunc[GreenAgent ])();
    pAgents[1] = (*  agntsMgr.pAllNewAgentFunc[YellowAgent])();
    pAgents[2] = new RandomAgent;
    pAgents[3] = new RandomAgent;
    pAgents[4] = new RandomAgent;

    for(int i=0;i<tankNum;++i){
        pAgents[i]->setTank(  pTanks[i]  );
        pAgents[i]->linkMap( &map );
        pAgents[i]->linkMsslList( &Mlist );
        pAgents[i]->linkpTanks( pTanks,tankNum );
    }
}
void Game::game_delete(){
    teamMgr.clear();
    for(int i=0;i<tankNum;++i){
        delete pTanks[i];
        delete pAgents[i];
    }
    delete [] pTanks;
    delete [] pAgents;
    tankNum = 0;
}

void Game::plot(int viewL){// viewL==0 : global view, else agent0's view
    if(viewL>0){
        Map view(map,pTanks[0]->getPos(),viewL);
        view.print();
    }else{
        map.print();
    }
    printw("\n");
    printw("Time: %d\n",timer);

    for(int i=0;i<tankNum;++i){
        printw("Tank[%d]'s Life:%d\n",i,pTanks[i]->getLife());
    }

    for(int i=0,s=teamMgr.getTeamCount();i<s;++i){
        printw("Team[%d] has %d tank(s)\n",i,teamMgr.getTankNum(i));
    }

    printw("%s",DebuggingMessage.c_str());

    #ifdef NO_CURSES
    for(int i=0;i<3;++i) printw("\n");
    #endif
    refresh();
}
bool Game::update(bool showGame,int viewL){
    Action keyAction;
    #ifndef NO_CURSES
    int keyPress=getch();
    #else
    int keyPress=0;
    #endif

    //Tanks update
    for(int i=0;i<tankNum;++i){
        if(pTanks[i]->isDead()){
            if( !pTanks[i]->isCleared() )
                teamMgr.teamGet_1_Killed( pTanks[i]->getTeamID() );
            pTanks[i]->remove(map);
            continue;
        }
        pTanks[i]->receiveAction(  pAgents[i]->actionToDo(keyPress)  );
        pTanks[i]->update(map,Mlist);
    }

    //Missles update;
    MissileList::iterator it=Mlist.begin();
    while(it != Mlist.end()){
        it->update(map);
        if(it->hitTank(map)){
            for(int i=0;i<tankNum;++i){
                if(  pTanks[i]->getPos()==it->position  ){
                    pTanks[i]->hurt(1);
                    break;
                }
            }
        }
        if(it->isDestroyed()){
            it = Mlist.erase(it);
        }else ++it;
    }

    if(showGame)plot(viewL);

    // check end of the game?  return toContinue
    int teamsRemain = teamMgr.getTeamCount();
    for(int i=0,s=teamsRemain;i<s;++i)
        if(teamMgr.getTankNum(i)<=0) --teamsRemain;
    if(teamsRemain<=1)
        return false;
    else{
        --timer;
        return timer>0 ;
    }
}

void Game::humanGame(int randSeed,int aiAgent,int viewL){
    game_special0_init(randSeed,aiAgent);
    playing(true,viewL);
    showEndingMsg();
    game_delete();
}
void Game::singleGame(int randSeed,int GreenAgent,int YellowAgent,bool showGame){
    game_standard_init(GreenAgent,YellowAgent,randSeed);
    playing(showGame);
    showEndingMsg();
    game_delete();
}
void Game::battleAll(const char* dumpFileName,bool showGame){
    static int gameStageCount = 5;
    static int gameStage[5] = {4,9,80,7352,8632};//1543

    int size = agntsMgr.pAllNewAgentFunc.size();
    int score_p0,score_p1;
    for(int i=0;i<size;++i)
        for(int j=0;j<size;++j)
            agntsMgr.scores[i][j] = 0;

    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            //if(showGame==false)
            //    printw("agent No %d v.s. agent No %d",i,j);
            for(int k=0;k<gameStageCount;++k){
                game_standard_init(i,j,gameStage[k]);
                playing(showGame);

                //calc scores:
                if(teamMgr.getTankNum(0)>0) agntsMgr.scores[i][j] += (timer<=0)?400:(900+timer);
                if(teamMgr.getTankNum(1)>0) agntsMgr.scores[j][i] += (timer<=0)?400:(900+timer);

                if(i==0 && k==0) agntsMgr.agentName.push_back(pAgents[1]->getName());
                //system("pause");
                game_delete();
            }
        }
    }
    agntsMgr.dump(dumpFileName);
}
void Game::showEndingMsg(){
    int win=-1;
    int score_p0=0,score_p1=0;

    for(int i=0,s=teamMgr.getTeamCount();i<s;++i){
        if(teamMgr.getTankNum(i)>0){
            win = i;
            break;
        }
    }
    if(timer<=0) win = -1;
    if(teamMgr.getTankNum(0)>0) score_p0 = (timer<=0)?400:(900+timer);
    if(teamMgr.getTankNum(1)>0) score_p1 = (timer<=0)?400:(900+timer);

    if(win==-1) printw("Match game!!\t(team 0: %d p;team 1: %d p)\n",score_p0,score_p1);
    else        printw("team %d wins!!\t(team 0: %d p;team 1: %d p)\n",win,score_p0,score_p1);
}


