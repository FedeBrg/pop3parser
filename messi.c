#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "messi.h"
#include <ctype.h>
#include <unistd.h>







// bool array_contains( char ** arr, char * str ) {
// 	int size = sizeof( arr )/sizeof( arr[0] );
// 	printf("SIZE CALCULADO %d\n",size );
// 	for( int i = 0; i < size; i++ ) {
// 		if( strcmp( arr[i], str ) ){
// 			return true;
// 		}
// 	}
// 	return false;

// }

char * single_line_cmds [] = { "quit", "stat", "dele", "noop", "rset", "user", "pass", "apop","capa","auth",0};

char * multi_line_cmds [] = { "list", "top", "uidl",0};

char * retr_cmd = "retr\r";

bool array_contains( char ** arr, char * str ) {

	int i = 0;
	while(arr[i] != 0){
		//printf("cmp :%s\n",arr[i] );
		fprintf(stderr,"arr[%d] = %s // cmd: %s\n", i,arr[i],str);
		if( strcmp( arr[i], str ) == 0){
			printf("TE ECONTRE\n");
			return true;
		}
		i++;
	}
	return false;

}
bool is_single( char * cmd ) {
	if( array_contains( single_line_cmds, cmd ) ) {
		return true;
	}
	return false;
}

bool is_multi( char * cmd ) {
	if(array_contains( multi_line_cmds, cmd ) ) {
		return true;
	}
	return false;
}
bool is_retr( char * cmd ) {
	if( strcmp( retr_cmd, cmd ) == 0 ) {
		return true;
	}
	return false;
}

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

enum parser_state check_cmd( char * cmd ){
	fprintf(stderr,"RECIBI %s\n",cmd );

	lowercase(cmd);
	//printf("LOWERCASE: %s\n",cmd );
	char clean[SIZE];
	clean_cmd(cmd,clean);
	//printf("CLEAN CMD%s\n", clean);
	if( strcmp(clean,"quit") == 0 ) {
		return exit_parser;
	}
	if( is_single( clean ) ) {
		return parsing_single_line;
	}
	else if( is_multi( clean ) ) {
		return parsing_multi_line;
	}
	else if( is_retr( clean ) ) {
		return parsing_retr;
	}
	else{
		return parsing_single_line;
	}

}

void clear_buffer1(char * buff){
  for(int i = 0; i<SIZE;i++){
    buff[i] = '\0';
  }
}

enum parser_state cmd_read( struct pop3_parser * p, char c ) {
	//fprintf(stderr, "%d\n",c );
	switch(p->cmd_sm.state){
		case reading:
			if( c == '\r' ){
				p->cmd_sm.state = r_found;
			}

		break;

		case r_found:
			if( c == '\n' ){
				enum parser_state st = check_cmd( p->cmd );
				p->state = st;
				p->cmd_sm.state = reading;
				clear_buffer1(p->cmd);
				return st;
			}
			else if( c == '\r' ){
				p->cmd_sm.state = r_found;
			}
			else{
				p->cmd_sm.state = reading;
			}

		break;

	}

	p->cmd[ strlen( p->cmd ) ] = c;

	return parsing_cmd;

}


enum parser_state single_line_read( struct pop3_parser * p, char c ) {

	switch(p->sl_sm.state){
		case reading:
			if( c == '\r' ){
				p->sl_sm.state = r_found;
			}

		break;

		case r_found:
			if( c == '\n' ){
								p->sl_sm.state = reading;

				return parsing_cmd;
			}
			else if( c == '\r' ){
				p->sl_sm.state = r_found;
			}
			else{
				p->sl_sm.state = reading;
			}

		break;

	}

	return parsing_single_line;
}


enum parser_state multi_line_read ( struct pop3_parser * p, char c ) {
	switch( p->ml_sm.state ) {
		case reading1:
			if( c == '\r' ) {
				p->ml_sm.state = r1_found;
			}

		break;

		case r1_found:
			if(c == '\n'){
				p->ml_sm.state = n1_found;
			}
			else if(c != '\r'){
				p->ml_sm.state = reading1;
			}
		break;

		case n1_found:
			if(c == '.'){
				p->ml_sm.state = dot_found;
			}
			else{
				p->ml_sm.state = reading1;
			}
		break;

		case dot_found:
			if( c == '\r'){
				p->ml_sm.state = r2_found;
			}
			else{
				p->ml_sm.state = reading1;
			}
		break;

		case r2_found:
			if(c == '\n'){
								p->ml_sm.state = reading1;

				return parsing_cmd;
			}
			else{
				p->ml_sm.state = reading1;
			}
		break;
	}

	return parsing_multi_line;
}

enum parser_state retr_read ( struct pop3_parser * p, char c ) {
	switch( p->r_sm.state ) {
		case reading1:
			if( c == '\r' ) {
				p->r_sm.state = r1_found;
			}

		break;

		case r1_found:
			if(c == '\n'){
				p->r_sm.state = n1_found;
			}
			else if(c != '\r'){
				p->r_sm.state = reading1;
			}
		break;

		case n1_found:
			if(c == '.'){
				p->r_sm.state = dot_found;
			}
			else{
				p->r_sm.state = reading1;
			}
		break;

		case dot_found:
			if( c == '\r'){
				p->r_sm.state = r2_found;
			}
			else{
				p->r_sm.state = reading1;
			}
		break;

		case r2_found:
			if(c == '\n'){
								p->r_sm.state = reading1;

				return parsing_cmd;
			}
			else{
				p->r_sm.state = reading1;
			}
		break;
	}

	return parsing_multi_line;
}


// void enum_to_str(int i){
// 	switch(i){
// 		case parsing_cmd:
// 			printf("PARSING CMD\n");
// 		break;

// 		case parsing_single_line:
// 			printf("PARSING SINLE LINE\n");
// 		break;

// 		case parsing_multi_line:
// 			printf("PARSING MULTI LINE\n");
// 		break;

// 		case parsing_retr:
// 			printf("PARSING RETR\n");
// 		break;

// 		case parser_error:
// 			printf("PARSER ERROR");
// 		break;



// 	}
// }

// void main(void){
// 	printf("Hola\n");

// 	char * retr = "retr\r\n.\r\n";

// 	struct single_line_state_machine slsm = {
// 		.state = reading,
// 	};

// 	struct multi_line_state_machine mlsm = {
// 		.state = reading1,
// 	};

// 	struct cmd_state_machine cmsm = {
// 		.state = reading,
// 	};

// 	struct pop3_parser p = {
// 		.sl_sm = slsm,
// 		.ml_sm = mlsm,
// 		.cmd_sm = cmsm,
// 		.state = parsing_cmd,
// 		.cmd = "\0\0\0\0\0\0\0\0\0\0",
// 	};

// 	printf("%s\n", p.cmd);
// 	int i = 0;
// 	int ret;
// 	while(true){
// 		if( ( ret = multi_line_read( &p, *(retr+i) ) ) != parsing_multi_line ){
// 			break;
// 		}
// 		i++;
// 	}

// 	enum_to_str(ret);

// }


//HAY QUE HACER UNA FUNCION QUE LIMPIE p->cmd DE LOS CARACTERES Q NO SEAN PARTE DEL COMANDO, UNA ESPECIE DE SPLIT EN LOS ESPACIOS Y DESPUES VER QUE NO LO RODEE NADA RARO TIPO \r O ESAS COSAS
//FALTA MANEJO DE ERRORES EJ SI MIENTRAS PARSEA MULTILINE NO ENCUENTRA LOS CARACTERES TERMINALES TE TIRA SEG FAULT

// ESTARIA BUENO PODER UNIFICAR CMD_SM Y SINGLE_LINE_SM PORQUE HACEN EXACTAMENTE LO MISMO
// HAY Q VER SI EL RETR NECESITA UNA MAQUINA DE ESTADOS PARA EL MISMO PARA IDENTIFICAR QUE PARTES SON DE SUBJECT BODY Y TODO ESO O SE PUEDE USAR DIRECTAMENTE MULTILINE CON ALGUN FLAG

// AL PARECER YA PUEDE IDENTIFICAR TODOS LOS ESTADOS  Y RETORNA BIEN SEGUN A QUE ESTADO HAY Q PASAR