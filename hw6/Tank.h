#ifndef __Tank_h__
#define __Tank_h__

#include "basic.h"
#include "Map.h"
#include "Team.h"
#include "Missile.h"

#define MotionDelay 1
#define FiringDelay 7

class Tank{
private:
    int life;
    int position;
    Direction facing;
    Action   nextAction;
    Team m_team;
    int motionRecovering,firingRecovering;

    void draw(Map& map){
        char here;
        switch(facing){
        case U_Dir: here='^';break;
        case D_Dir: here='v';break;
        case L_Dir: here='<';break;
        case R_Dir: here='>';break;
        default:    here='^';
        }
        map[position] = m_team.colorChar(here);
    }
    bool move(Map& map){
        int nextPos = map.posMove(position,facing);
        if( !map.validPos(nextPos) ) return false;
        //else
        map[position] = ' ';
        position = nextPos;
        draw(map);
        return true;
    }
    bool turn(Map& map){
        facing = (Direction)nextAction;
        draw(map);
        return true;
    }
    bool firing(MissileList& Mlist)const{
        Mlist.push_back(Missile(position,facing));
        return true;
    }
public:
    Tank(int pos,Direction dir,Map& map,Team t,int l=5)
        :position(pos),facing(dir),m_team(t),life(l){
        nextAction=noAct;draw(map);
        motionRecovering=0;
        firingRecovering=0;
    }

    void receiveAction(const Action act){nextAction=act;}
    bool update(Map& map,MissileList& Mlist){
        if(motionRecovering>0) --motionRecovering;
        if(firingRecovering>0) --firingRecovering;

        if(nextAction==noAct){
            return true;
        }else if(nextAction==fire){
            if(firingRecovering>0) return false;
            firingRecovering+=FiringDelay;
            return firing(Mlist);
        }else if(nextAction==facing){
            if(motionRecovering>0) return false;
            motionRecovering+=MotionDelay;
            return move(map);
        }else{
            if(motionRecovering>0) return false;
            motionRecovering+=MotionDelay;
            return turn(map);
        }
    }
    void remove(Map& map){
        if(position!=-1)map[position]=' ';
        position = -1;
    }
    void hurt(int point){life-=point;if(life<0)life=0;}

    int getPos()const{return position;}
    int getLife()const{return life;}
    int getTeamID()const{return m_team.getTeamID();}
    Direction getFacing()const{return facing;}
    bool isDead()const{return life<=0;}
    bool isCleared()const{return position==-1;}

    int getmRecovering()const{return motionRecovering;}
    int getfRecovering()const{return firingRecovering;}
};


#endif // __Tank_h__
