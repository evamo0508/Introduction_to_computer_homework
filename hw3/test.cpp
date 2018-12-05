#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main()
{
    char line[10000];
    unsigned long long int N=4108037587;
    int e=65537;
    //char N_string[20],e_string[5];
    fstream plain,public_key;
    plain.open("plain.txt",ios::in);
    plain.getline(line,sizeof(line),'\n');
    int line_length=plain.gcount();
    plain.close();


    unsigned long long int S[(int)ceil(line_length/2)],s[(int)ceil(line_length/2)];
    for(int i=0;i<ceil(line_length/2);i++)
    {
        S[i]=(int)line[2*i]*pow(2,8)+(int)line[2*i+1];
        //cout<<(unsigned long long int)pow((int)line[2*i]*pow(2,8)+(int)line[2*i+1],e)%N<<'\n';
        s[i]=1;
        unsigned long long int E=e;
        while(E>1)
        {
            if(E%2==1){s[i]=s[i]*S[i]%N;E--;}
            else {S[i]=S[i]*S[i]%N;E/=2;}
        }
        s[i]=s[i]*S[i]%N;
        cout<<s[i]<<endl;
    }
    //cout<<(int)line[0]*pow(2,8)+(int)line[1];
    fstream secret("secret.txt",ios::out);
    for(int i=0;i<ceil(line_length/2);i++)
    {
        secret<<s[i]<<'\n';
    }
    secret.close();
}

