#include<cmath>
#include "BMPImg.h"
#include<iostream>
#include<fstream>

using namespace std;

bool BMPImg::RGB2Y()
{
    int y[getPxlNum()];
    for(int i=0;i<getPxlNum();i++)y[i]=0.299*(double)data[i*3+2]+0.587*(double)data[i*3+1]+0.114*(double)data[i*3];
    for(int i=0;i<3*getPxlNum();i++)data[i]=y[i/3];
    return true;
}

/*bool BMPImg::sobelEdge(){
    return true;
}*/

int main(){
BMPImg img;
img.loadPic("liver.bmp");
//img.printHeader();
img.storePic("result1.bmp");
img.RGB2Y();
img.storePic("result2.bmp");
return 0;
}




