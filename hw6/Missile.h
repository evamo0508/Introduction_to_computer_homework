#ifndef __Missile_h__
#define __Missile_h__

#include<list>
#include "basic.h"
#include "Map.h"

class Missile{
public:
    int position;
    Direction facing;
    bool destroy;
private:
    int updatePos(Map& map){
        if(map[position]=='.')map[position]=' ';

        int mpos = map.posMove(position,facing);
        if(mpos==position) destroy=true;//border
        else position = mpos;
        return position;
    }
    bool hitCheck(const Map& map){
        bool hit = !map.validPos(position);
        if(hit){
            destroy=true;
            return true;
        }else return false;
    }
    void draw(Map& map)const{
        if(map[position]==' ')
            map[position]='.';
    }
public:
    Missile(int pos,Direction dir):position(pos),facing(dir){destroy=false;}

    void update(Map& map){
        updatePos(map);
        hitCheck(map);
        if(!destroy) draw(map);
    }
    bool hitTank(Map& map)const{
        return map.isTank(position);
    }
    bool isDestroyed(){return destroy;}

    int getPos()const{return position;}
    Direction getFacing()const{return facing;}
};

typedef std::list<Missile> MissileList;

#endif // __Missile_h__
