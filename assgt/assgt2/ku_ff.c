/**
 * @file ku_ff.c
 * @author JunHyung Do (dojh99@gmail.com)
 * @date 2021-11-25
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

// global variables
int num=0;
int n_thread;
int a1;
int a2;
int fd_in;
int fd_out;
char* fullfile;
char** contents;

// thread job
void* perThreadJob(void* p_args){
    const int thread_num= *((int*) p_args);
    int* count = (int*) malloc(sizeof(int));
    *count = 0;

    if(thread_num > num){
        return count;
    }
    
    char buffer[6];

    for(int idx = thread_num;idx<num;idx+=n_thread){
        memcpy(buffer,fullfile + 6*idx,6);
        //pread(fd_in,buffer,6,6*(idx+1)); //not using pread in thread is faster...?
        int number = atoi(buffer);

        if(a1 <=number && number<=a2){
            memcpy(contents[thread_num] + 6*(*count),buffer,6);
            ++(*count);
            //write(fd_out,buffer,6); //not using write in loop is much faster (x5~7)
        }    
    }

    write(fd_out,contents[thread_num],(*count)*6);
    return count;
}


int main(int argc,char** argv){
    char buffer[7]={0,};
    int result = 0; 
    pthread_t* tids;
    int* args;
    
    if(argc <=6){
        return 1;    
    }
    
    //args
    a1 = atoi(argv[1]);
    a2 = atoi(argv[2]);
    n_thread = atoi(argv[3]);
    char* input_path = argv[4];
    char* output_path = argv[5];

    // open files
    fd_in = open(input_path,O_RDONLY);
    fd_out = open(output_path,O_WRONLY|O_CREAT|O_TRUNC,0666);

    if(fd_in == -1){
        perror("open(input_path)");
        return 1;
    }

    if(fd_out == -1){
        perror("open(output_path)");
        return 1;
    }

    // get NUMS
    read(fd_in,buffer,6);
    num = atoi(buffer);

    // read files
    int fsize = sizeof(char)*6*num;
    fullfile = malloc(fsize);
    read(fd_in,fullfile,fsize);
    
    // set outputfile offset 
    lseek(fd_out,6,SEEK_SET);

    // create array
    tids = (pthread_t*) malloc(sizeof(pthread_t)*n_thread);
    args = malloc(sizeof(int)*n_thread); 
    contents = malloc(sizeof(char*)*n_thread);

    // create thread
    for(int thread_num=0;thread_num<n_thread;++thread_num){
            // create array(2d)
            contents[thread_num] = malloc(6*(num/n_thread+1)); 

            //set arg
            args[thread_num]=thread_num;

            // create thread
            pthread_create(&tids[thread_num],NULL,perThreadJob,&args[thread_num]);
    }
    
    // join threads and get results
    int** results = (int**) malloc(sizeof(int**)*n_thread);

    for(int thread_num=0;thread_num<n_thread;++thread_num){
        int status = pthread_join(tids[thread_num],(void**)&(results[thread_num]));
        //write(fd_out,contents[thread_num],6*(*results[thread_num]));
        result+=*results[thread_num];
    }

    // write N which is number of elements in range
    sprintf(buffer,"%5d\n",result);
    pwrite(fd_out,buffer,6,0);

    // free
    free(args);

    for(int i=0;i<n_thread;++i){
        free(results[i]);
        free(contents[i]);
    }

    free(results);
    free(contents);

    free(tids);
    free(fullfile);

    return 0;
}
