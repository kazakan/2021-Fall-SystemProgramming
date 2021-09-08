#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef FPRKFN
#define FORKFN 0
#endif

void fork1(){
	int x = 0;
	pid_t pid = fork();
	if(pid == 0){
		printf("Child x = %d\n",++x);
	}else{
	
		printf("Parent x = %d\n",--x);
	}
	printf("bye bye from process %d with x=%d\n",getpid(),x);

}

void fork2(){
	printf("L0\n");
	fork();
	printf("L1\n");
	fork();
	printf("byebye fork2\n");
}

void fork3(){
	printf("L0\n");
	fork();
	printf("L1\n");
	fork();
	printf("L2\n");
	fork();
	printf("byebye fork2\n");
}

void fork4(){
	printf("L0\n");
	if(fork()!=0){
		printf("L1\n");
		if(fork() !=0){
			printf("L2\n");
			fork();
		}
	}
	printf("Bye Bye Bye\n");
}

void fork5(){
	printf("L0\n");
	if(fork()==0){
		printf("L1\n");
		if(fork() ==0){
			printf("L2\n");
			fork();
		}
	}
	printf("Bye Bye Bye\n");
}

void cleaning(){
	printf("Cleaning\n");
}

void fork6(){
	atexit(cleaning);
	fork();
	exit(0);
}

int main(void){
	switch(FORKFN){
		case 1:
			fork1();
			break;
		case 2:
			fork2();
			break;
		case 3:
			fork3();
			break;
		case 4:
			fork4();
			break;
		case 5:
			fork5();
			break;
		case 6:
			fork6();
			break;
		default:
			printf("Set FORKFN to intetger between 1~6 using gcc option\n");
	}
	return 0;
}
