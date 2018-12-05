#ifndef __PolicyMaker_h__
#define __PolicyMaker_h__

#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>
#include "basic.h"
#include "Map.h"
#include "Tank.h"
#include "Missile.h"

extern std::string DebuggingMessage;

class PolicyMaker{
private:
    Map* theMap;
    MissileList* theMissiles;
    Tank** pTanks;
    int tankNum;
    char name[32];
protected:
    const Tank* pTank;
    Map view;
    std::vector<Missile> MsslinView;
    std::vector<Tank>    TankinView;

    void pos2xy(const int pos,int& x,int& y){
        x = pos%theMap->getWidth();
        y = pos/theMap->getWidth();
    }
    void getView(int rad);
    void getMissileInView(int rad);
    void getTankInView(int rad);
public:
    PolicyMaker(const char* _name):view(5,5){strcpy(name,_name);}
    virtual ~PolicyMaker(){}

    void linkMap(Map* pmap){theMap=pmap;}
    void linkMsslList(MissileList* pML){theMissiles=pML;}
    void linkpTanks(Tank** pTs,int num){pTanks=pTs;tankNum=num;}
    void setTank(const Tank* pT){pTank=pT;}
    char* getName(){return name;}

    virtual Action actionToDo(int)=0;
};

#ifndef NO_CURSES
class HumanAgent:public PolicyMaker{
private:
    int U_key,D_key,L_key,R_key;
    int fire_key;
public:
    HumanAgent(int Uk=KEY_UP,int Dk=KEY_DOWN,int Lk=KEY_LEFT,int Rk=KEY_RIGHT,int fk=' '):PolicyMaker("HumanAgent"){
        U_key=Uk; D_key=Dk; L_key=Lk; R_key=Rk;
        fire_key=fk;
    }
    Action actionToDo(int keyPressed){
        if(keyPressed==U_key)       return U_Act;
        else if(keyPressed==D_key)  return D_Act;
        else if(keyPressed==L_key)  return L_Act;
        else if(keyPressed==R_key)  return R_Act;
        else if(keyPressed==fire_key)return fire;
        else return noAct;
    }
};
#else
    #define HumanAgent RandomAgent
#endif

class RandomAgent:public PolicyMaker{
public:
    RandomAgent():PolicyMaker("RandomAgent"){}
    Action actionToDo(int arg){
        int r = rand()%10;
        //40%: fire
        //20%: just go ahead
        //40%: 10% for each dir
        switch(r){
        case 0:
        case 1:
        case 2:
        case 3:
            return fire;
        case 4:
        case 5:
            return Action(pTank->getFacing());
        case 6:
            return U_Act;
        case 7:
            return D_Act;
        case 8:
            return L_Act;
        case 9:
            return R_Act;
        default:
            return noAct;
        }
    }
};


#endif //__PolicyMaker_h__
