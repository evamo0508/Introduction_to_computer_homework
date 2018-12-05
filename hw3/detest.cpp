#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main()
{
    unsigned long long int N=4108037587,d=3835874873;
    unsigned long long int s[100];
    //char N_string[20],e_string[5];
    fstream secret,private_key;
    secret.open("secret.txt",ios::in);
    int s_length=0;
    while(!secret.eof()){secret>>s[s_length];s_length++;}
    s_length--;//eof()會多算一次
    secret.close();

    //for(int i=0;i<s_length;i++)cout<<s[i]<<'\n';
    //cout<<s_length;

    unsigned long long int m[s_length];
    for(int i=0;i<s_length;i++)
    {
        m[i]=1;
        unsigned long long int D=d;
        while(D>1)
        {
            if(D%2==1){m[i]=m[i]*s[i]%N;D--;}
            else {s[i]=s[i]*s[i]%N;D/=2;}
        }
        m[i]=m[i]*s[i]%N;
        cout<<m[i]<<endl;
    }

    char mes[s_length*2];
    for(int i=0;i<s_length;i++)
    {
        mes[2*i+1]=(char)m[i]%256;
        mes[2*i]=(char)((m[i]-mes[2*i+1])/256);
    }

    fstream message;
    message.open("message.txt",ios::out);
    for(int i=0;i<s_length*2;i++){message<<mes[i];}
    message.close();
}


