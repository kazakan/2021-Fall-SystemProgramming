#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <mqueue.h>

#define MQ_NAME "/mymymy"
#define PRIO 0
#define MSG_SIZE sizeof(int)
#define MAX_MSG 10

#include "ku_ps_input.h"

int doCount(const int proc_num,const int i, const int a1, const int a2){
    int count =0;
    
    for(int idx = proc_num;idx<NUMS;idx+=i){
        if(a1 <= input[idx] && input[idx]<=a2){
            ++count;
        }    
    }

    return count;
}

void perProcessJob(const int proc_num,const int i,const int a1,const int a2){

    const int count = doCount(proc_num,i,a1,a2);

    // send result to parent
    struct mq_attr attr;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MAX_MSG;

    mqd_t mqdes = mq_open(MQ_NAME,O_CREAT|O_WRONLY,0600,&attr);
    if(mqdes == -1){
        perror("mq_open()");
        exit(0);
    }

    while(mq_send(mqdes,(char*)&count,MSG_SIZE,PRIO)==-1);

    mq_close(mqdes);
    exit(0);
}


int main(int argc,char** argv){
    const int a1 = atoi(argv[1]);
    const int a2 = atoi(argv[2]);
    const int i = atoi(argv[3]);

    for(int proc_num=0;proc_num<i;++proc_num){
        pid_t pid=0;
        if((pid=fork())==0){
            perProcessJob(proc_num,i,a1,a2); // calls exit inside
        }else if(pid==-1){
            perror("fork()");
            exit(0);
        }
    }

    int result = 0;
    
    struct mq_attr attr;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MAX_MSG;

    mqd_t mqdes = mq_open(MQ_NAME,O_CREAT|O_RDONLY,0600,&attr);

    if(mqdes == -1){
        perror("mq_open()");
        exit(0);
    }

    for(int chunk_num=0;chunk_num<i;++chunk_num){
        int tmp=0;
        pid_t pid = wait(0);

        if(pid == -1){
            perror("wait()");
            mq_close(mqdes);
            mq_unlink(MQ_NAME);
            exit(0);
        }

        if(mq_receive(mqdes,(char*)&tmp,MSG_SIZE,PRIO) == -1){
            perror("mq_receive()");
            mq_close(mqdes);
            mq_unlink(MQ_NAME);
            exit(0);
        }
        result+=tmp;
        
    }

    mq_close(mqdes);
    mq_unlink(MQ_NAME);
    
    printf("%d",result);
    return 0;
}
