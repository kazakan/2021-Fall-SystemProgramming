#include <stdio.h>

int main(void){
	for(int i=0;i<10;++i){
		for(int j=0;j<i+1;++j){
			printf("*");
		}
		printf("\n");
	}
}
