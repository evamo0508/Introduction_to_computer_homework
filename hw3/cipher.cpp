#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main()
{
    char line[10000];
    unsigned long long int N,e;
    //char N_string[20],e_string[5];
    fstream plain,public_key;
    plain.open("plain.txt",ios::in);
    plain.getline(line,sizeof(line),'\n');
    int line_length=plain.gcount();
    plain.close();
    public_key.open("public_key.txt",ios::in);
    //public_key.getline(N_string,sizeof(N_string),'\n');
    //int N_length=public_key.gcount();
    //public_key.getline(e_string,sizeof(e_string),'\n');
    //int e_length=public_key.gcount();
    public_key>>N>>e;
    public_key.close();

    /*int e=0;
    for(int i=0;i<e_length;i++)
    {
        e+=((int)e_string[i]-48)*pow(10,e_length-i-1);
    }
    unsigned long long int N=0;
    for(int i=0;i<N_length;i++)
    {
        N+=((int)N_string[i]-48)*pow(10,N_length-i-1);
    }*/

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
       // cout<<s[i]<<endl;
    }


    //cout<<(int)line[0]*pow(2,8)+(int)line[1];
    fstream secret("secret.txt",ios::out);
    for(int i=0;i<ceil(line_length/2);i++)
    {
        secret<<s[i]<<'\n';
    }
    secret.close();
}
