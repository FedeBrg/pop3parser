#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define STDIN 0
#define BUFFSIZE 4096

int main(void){
	char * buff = malloc(BUFFSIZE);
	int i;

	while(1){
		i = 0;
		read(STDIN, buff, BUFFSIZE);

		do{
			if(buff[i] > 96 || buff[i] < 123){
				buff[i] = buff[i] - 32;
			}

			i++;

		} while(i < BUFFSIZE);
	}

	return 0;	
}