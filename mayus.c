
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>


#define BUFFSIZE 4096

int main(void){
	char buff[1];

	while(1){
		read(0, buff, 1);

		buff[0] = toupper(buff[0]);

		//printf("%c",buff[0] );
		write(1,buff,1);
		fprintf(stderr,"%d", buff[0]);
		//fprintf(stderr, "holi");
	}

	return 0;	
}