#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXSZ 512

enum state{
    s_wait=0,
    s_arg,
    s_arg_end,
    s_arg_line_end,
    s_line_end,
    s_end
};


enum type{
    space=0,
    arg,
    end
};


enum type get_type_by_char(char p){
    if(p==' '){
        return space;
    }else if (p=='\n'){
        return end;
    }else{
        return arg;
    }
}

enum state tranform(enum state s,enum type next){
    switch (s)
    {
    case s_wait:
        if(next==space) return s_wait;
        if(next==end) return s_line_end;
        if(next==arg) return s_arg;
        break;
    case s_arg:
        if(next==space) return s_arg_end;
        if(next==end) return s_arg_line_end;
        if(next==arg) return s_arg;
        break;
    case s_arg_end:
    case s_arg_line_end:
    case s_line_end:
        if(next==space) return s_wait;
        if(next==end) return s_line_end;
        if(next==arg) return s_arg;
        break;
    default:
        break;
    }
    return s_end;
}


int main(int argc,char* argv[]){

    if(argc-1>MAXARG){
        fprintf(2,"too many argv!\n");
        exit(1);
    }
    enum state my_state=s_wait;
    char line[MAXSZ];
    char* p=line;
    char* my_arg[MAXARG]={0};
    for(int i=1;i<argc;++i){
        my_arg[i-1]=argv[i];
    }
    int arg_begin=0;
    int arg_end=0;
    int arg_cnt=argc-1;


    while(my_state!=s_end){
        if(read(0,p,sizeof(char))!=sizeof(char)){
            my_state=s_end;
        }else{
            my_state=tranform(my_state,get_type_by_char(*p));
        }
        if(++arg_end>=MAXSZ){
            fprintf(2,"too long\n");
            exit(1);
        }

        switch (my_state)
        {
        case s_wait:
            ++arg_begin;
            break;
        case s_arg_end:
            my_arg[arg_cnt++]=&line[arg_begin];
            arg_begin=arg_end;
            *p='\0';
            break;
        case s_arg_line_end:
            my_arg[arg_cnt++]=&line[arg_begin];
        case s_line_end:
            arg_begin=arg_end;
            *p='\0';
            if(fork()==0){
                exec(argv[1],my_arg);
            }
            arg_cnt=argc-1;
            for(int i=argc-1;i<MAXARG;++i){
                my_arg[i]=0;
            }
            wait(0);
            break;
        default:
            break;
        }

        p++;
    }

    exit(0);
}