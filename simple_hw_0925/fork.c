#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fork16(){

	if(fork()){
		if(fork()){
			// parent
			// do nothing
		}else{
			//child2
			printf("Child2 : pid=%d gprp=%d\n",getpid(),getpgrp());
			while(1);
		}

	}else{
		// child1
		printf("Child1 : pid=%d gprp=%d\n",getpid(),getpgrp());
		while(1);
	}
}


void fork17(){		
	if(fork()){
		// parent
		printf("Parent : pid=%d gprp=%d\n",getpid(),getpgrp());
		while(1);
	}else{
		//child1
		printf("Child : pid=%d gprp=%d\n",getpid(),getpgrp());
		while(1);
	}

}

int main(int argc,char* argv[]){
	int fnum = atoi(argv[1]);

	switch(fnum){
		case 16:
			fork16();
			break;
		case 17:
			fork17();
			break;
		default:
			break;
	}

	return 0;
}
