#include "kernel/types.h"

#include "user/user.h"


void primes(int p[2]){
    close(p[1]);
    int first;

    if(read(p[0],&first,sizeof(first))==0){
        exit(0);
    }
    int my_p[2];
    pipe(my_p);
    printf("prime %d\n",first);
    int data;
    while(read(p[0],&data,sizeof(data))>0){
        if(data%first==0){
            continue;
        }else{
            write(my_p[1],&data,sizeof(data));
        }
    }
    if(fork()==0){
        primes(my_p);
    }else{
        close(my_p[0]);
        close(my_p[1]);
        wait(0);
    }
    exit(0);
}


int main(int argc, char* argv[]){
    int p[2];
    pipe(p);

    for(int i=2;i<=35;++i){
        write(p[1],&i,sizeof(i));
    }
    if(fork()==0){
        primes(p);
    }else{
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}