#ifndef __Map_h__
#define __Map_h__

#include "basic.h"

class Map{
private:
    int width;
    int height;
    chtype* data; // curses char type (unsigned long)

    void copy(const Map& orig);

public:
    void newMap(int w=15,int h=10);
    void copy_local(const Map& orig,int centerPos,int rad);
    Map(int w=15,int h=10){ newMap(w,h); }
    Map(const Map& orig){ copy(orig); }
    Map(const Map& orig,int centerPos,int rad){copy_local(orig,centerPos,rad);}
    Map& operator=(const Map& orig){
        clear();
        copy(orig);
        return *this;
    }
    void clear(){ delete[] data; }
    ~Map(){clear();}

    chtype& operator[](int pos){return data[pos];}
    void print()const;

    int posMove(const int pos,const Direction dir)const;
    bool validPos(const int pos)const{
        if(data[pos]==' ')return true;
        else if(data[pos]=='.')return true;
        else return false;
    }
    bool isEdge(const int pos)const;
    bool isTank(const int pos)const;
    bool isWall(const int pos)const;
    int XY2Pos(const int x,const int y)const{return y*width+x;}
    int getWidth()const{return width;}
    int getHeight()const{return height;}
};



#endif // __Map_h__
