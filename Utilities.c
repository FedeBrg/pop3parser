#include "Numbers.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


void clean_cmd(char * cmd, char * ret){
	int i = 0;
	while(cmd[i] >= 'a' && cmd[i]<='z' ){
		//printf("%c\n",cmd[i] );
		ret[i] = cmd[i];
		i++;
	}

}

void lowercase(char * cmd){
	for(int i = 0; i<SIZE;i++){
		cmd[i] = tolower(cmd[i]);
	}
	return;
}


bool array_contains( char ** arr, char * str ) {

	int i = 0;
	while(arr[i] != 0){
		//printf("cmp :%s\n",arr[i] );
		//fprintf(stderr,"arr[%d] = %s // cmd: %s\n", i,arr[i],str);
		if( strcmp( arr[i], str ) == 0){
			//printf("TE ECONTRE\n");
			return true;
		}
		i++;
	}
	return false;

}


void clear_buffer(char * buff){
  for(int i = 0; i<SIZE;i++){
    buff[i] = '\0';
  }
}