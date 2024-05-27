#include "kernel/types.h"
#include "user/user.h"


int main(int argc,char* argv[]){

    int My_pipe[2];
    int My_pipe_son[2];
    pipe(My_pipe);//创建了一个管道  分别是0：读  和   1：写
    pipe(My_pipe_son);
   
    int pid=fork();

    if(pid>0){
        close(My_pipe[1]);
        close(My_pipe_son[0]);
        char buf='a';
        read(My_pipe[0],&buf,1);
        printf("%d:recive ping\n",getpid());
        write(My_pipe_son[1],&buf,1);
        close(My_pipe[0]);
        close(My_pipe_son[1]);
        exit(0);
    }
    close(My_pipe[0]);
    close(My_pipe_son[1]);
    char buf='a';
    write(My_pipe[1],&buf,1);
    read(My_pipe_son[0],&buf,1);
    printf("%d:recive pong\n",getpid());
    int state;
    wait(&state);
    close(My_pipe[1]);
    close(My_pipe_son[0]);
    return 0;
}