#ifndef __BMPIMG_H__
#define __BMPIMG_H__

#include<iostream>
#include<string>
#include<fstream>
using namespace std;

static const int headerNum = 15;
static const int headerSize[headerNum] = {2,4,4,4,  4,4,4,2,2,4,4,4,4,4,4};
static const string headerInfo[headerNum]
    = {"Identifier","FileSize","Reserved","BitmapDataOffset",
       "BitmapHeaderSize","Width","Height","Planes",
       "BitsPerPixel","Compression","BitmapDataSize","H_Resolution",
       "V_Resolution","UsedColors","ImportantColors"};

class BMPHead{
private:
    char Identifier[2];
    unsigned int    FileSize;
    unsigned int    Reserved;
    unsigned int    BitmapDataOffset;

    unsigned int    BitmapHeaderSize;
    unsigned int    Width;
    unsigned int    Height;
    unsigned short  Planes;
    unsigned short  BitsPerPixel;
    unsigned int    Compression;
    unsigned int    BitmapDataSize;
    unsigned int    H_Resolution;
    unsigned int    V_Resolution;
    unsigned int    UsedColors;
    unsigned int    ImportantColors;
public:
    friend class BMPImg;
    void* pFlag(const int i)const{  //return pointers of flags according to its order(int)
        switch(i){
        default:
        case  0:    return (void*)Identifier;
        case  1:    return (void*)&FileSize;
        case  2:    return (void*)&Reserved;
        case  3:    return (void*)&BitmapDataOffset;
        case  4:    return (void*)&BitmapHeaderSize;
        case  5:    return (void*)&Width;
        case  6:    return (void*)&Height;
        case  7:    return (void*)&Planes;
        case  8:    return (void*)&BitsPerPixel;
        case  9:    return (void*)&Compression;
        case 10:    return (void*)&BitmapDataSize;
        case 11:    return (void*)&H_Resolution;
        case 12:    return (void*)&V_Resolution;
        case 13:    return (void*)&UsedColors;
        case 14:    return (void*)&ImportantColors;
        }
    }
};
class BMPImg{
private:
    BMPHead header;
public:
    char* data;

    int getWidth()const{return header.Width;}
    int getHeight()const{return header.Height;}
    int getPxlNum()const{return header.Width * header.Height;}
    int getBytesPerPixel()const{return header.BitsPerPixel/8;}

    BMPImg(){}
    BMPImg(const string& picPath){loadPic(picPath);}
    ~BMPImg(){delete [] data;}

    //Loading bmp file
    bool loadPic(const string& picPath)
    {
        fstream file(picPath.c_str(),ios::in|ios::out|ios::binary);//open the file
        if(file.is_open())
        {
            //set the headerinfo
            for(int i=0;i<headerNum;++i)
            {
                if(headerSize[i]==4)file.read((char*)(header.pFlag(i)),4);
                else file.read((char*)(header.pFlag(i)),2);
            }

            //set data
            data=new char [3*getPxlNum()];
            for(int i=0;i<3*getPxlNum();i++)file.get(data[i]);
            file.close();//must close the file because it's not a member data
        }
        return true;
    }
    void printHeader()const{
        for(int i=0;i<headerNum;++i){
            cout<< headerInfo[i]+":";

            if(i==0){
                cout<< header.Identifier[0] << header.Identifier[1];
            }else{
                if(headerSize[i]==4)
                    cout<< *(  (unsigned int*)(header.pFlag(i))  );
                else
                    cout<< *(  (unsigned short*)(header.pFlag(i))  );
            }
            cout<< endl;
        }
    }
    bool RGB2Y();//calc Y and store back to RGB
    bool sobelEdge();
    bool storePic(const string& outPath)//Store bmp file
    {
        fstream file2(outPath.c_str(),ios::out);
        file2.close();
        file2.open(outPath.c_str(),ios::out|ios::in|ios::binary);//must create the file and then open it, if open it with constructor, won't help create a new file
        if(file2.is_open())
        {
            //set the headerinfo
            for(int i=0;i<headerNum;++i)
            {
                //write
                if(headerSize[i]==4)file2.write((char*)(header.pFlag(i)),4);
                else file2.write((char*)(header.pFlag(i)),2);
            }

            //set data
            //data=new char [3*getPxlNum()]; //DO NOT new again!!!!otherwise the stored data would be cleared!
            for(int i=0;i<3*getPxlNum();i++)file2.put(data[i]);
            file2.close();
        }
        return true;
    }
};

#endif // __BMPIMG_H__
