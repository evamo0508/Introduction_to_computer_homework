#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

int base10_asm(const char* a) {
    int result;
    asm volatile(".intel_syntax noprefix;\n"
    // ===== add your codes here =====
            //" mov   eax,  %1    ;\n"
            " mov ebx,0;\n"// initialize base as 0
            " mov edx,0;\n"// initialize result as 0
    "CHECK: shl edx,1;\n "//shift result
            " mov eax,BYTE PTR [%1+ebx];\n"//set al to a single byte of string a
            " cmp al,49;\n"//compare al w/ '1'
            " je PLUS;\n"//if equal to 1 goto PLUS
            "jmp NOPLUS;\n"
    "PLUS: inc edx;\n"//result +=1
    "NOPLUS: inc ebx;\n"//base++
            "cmp al,0;\n"//compare a1 w/ 0
            //"je FINAL;\n"
            "jne CHECK;\n"// if don't equal to 0 goto CHECK
    "FINAL: shr edx,1;\n"
    "mov   %0,   edx   ;\n"//set edx to result
    // =====---------------------=====
        ".att_syntax;\n" : "=r" (result) : "r" (a) : "%eax", "%ebx", "%ecx", "%edx"
                                        // ^ you can add more varibles/general purpose registers if you need them
    );
    return result;
}

int base10_cpp(const char* a) {
    int result = 0;
    int base = 0;

    do {
        result <<= 1;
        if(*(a + base) == '1') {
            result += 1;
        }
        base++;
    }
    while(*(a + base) != 0);

    return result;
}

#define CASENUM 10000000
#define NUM 0
void evaluate(){
    int* golden = new int[CASENUM];
    int* ans    = new int[CASENUM];
    char** s = new char*[CASENUM];
    for(int i=0,numA=NUM;i<CASENUM;++i,++numA){
        s[i] = new char[32];
        itoa(numA,s[i],2);
    }
    double cpp_time,asm_time;
    clock_t start;
    //===== run asm function  =====
    start = clock();
    for(int i=0,numA=NUM;i<CASENUM;++i,++numA){
        ans[i]=base10_asm(s[numA]);
    }
    asm_time = double(clock()-start)/CLOCKS_PER_SEC;
    //===== run cpp function =====
    start = clock();
    for(int i=0,numA=NUM;i<CASENUM;++i,++numA){
        golden[i]=base10_cpp(s[numA]);
    }
    cpp_time = double(clock()-start)/CLOCKS_PER_SEC;
    //===== checking =====
    int error = 0;
    for(int i=0;i<CASENUM;++i){
        if(ans[i]!=golden[i]) ++error;
        if(ans[i]!=golden[i] && error==1) printf("first error at %d: %d(cpp)!=%d(asm)\n",i,golden[i],ans[i]);
    }
    if(error==0){
        if(asm_time<cpp_time) printf("Congratulation!!, you pass the bonus work ^_^ \n");
        else printf("You pass the basic problem, it's near the bonus requirement >w< \n");
    }else printf("there are %d error(s) T^T \n",error);


    printf("cpp_time = %f sec for %d times\n",cpp_time,CASENUM);
    printf("asm_time = %f sec for %d times\n",asm_time,CASENUM);

    delete [] ans;
    delete [] golden;
    for(int i=0;i<CASENUM;++i){
        delete [] s[i];
    }
    delete [] s;
}

int main(){
    printf("result: %d\n",base10_asm("0101"));
    printf("result: %d\n",base10_asm("00111"));
    printf("result: %d\n",base10_asm("0011010"));
    printf("result: %d\n",base10_cpp("0101"));
    printf("result: %d\n",base10_cpp("00111"));
    printf("result: %d\n",base10_cpp("0011010"));

    for(int i=0;i<100;++i)
        evaluate();

    return 0;
}
