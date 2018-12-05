#include "PolicyMaker.h"

void PolicyMaker::getView(int rad){
    view.clear();
    view.copy_local(*theMap,pTank->getPos(),rad>2?2:rad);
}

void PolicyMaker::getMissileInView(int rad){
    rad = (rad>2)?2:rad;
    MsslinView.clear();//MisslinView is a vector
    int Tx,Ty,Mx,My,dx,dy;
    pos2xy(pTank->getPos(),Tx,Ty);

    MissileList::iterator it=theMissiles->begin();
    while(it != theMissiles->end()){
        pos2xy(it->getPos(),Mx,My);
        dx = Tx-Mx; dx=(dx<0)?-dx:dx;
        dy = Ty-My; dy=(dy<0)?-dy:dy;
        if(dx<=rad && dy<=rad) MsslinView.push_back(*it);
        ++it;
    }
}

void PolicyMaker::getTankInView(int rad){
    rad = (rad>2)?2:rad;
    TankinView.clear();//TankinView is a vector
    int Tx,Ty,T2x,T2y,dx,dy;
    pos2xy(pTank->getPos(),Tx,Ty);

    for(int i=0;i<tankNum;++i){
        if(pTanks[i]->getPos()==pTank->getPos()) continue;
        pos2xy(pTanks[i]->getPos(),T2x,T2y);
        dx = Tx-T2x; dx=(dx<0)?-dx:dx;
        dy = Ty-T2y; dy=(dy<0)?-dy:dy;
        if(dx<=rad && dy<=rad) TankinView.push_back(*pTanks[i]);
    }
}


