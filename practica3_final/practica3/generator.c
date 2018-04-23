/*
 * generator.c
 *
 *  Created on: Apr 21, 2018
 *      Author: mihai
 */

#include <stdio.h>
#include <stdlib.h>

char random_char();

int main(int argc, char** argv){
	FILE* fp = NULL;
	int size = 0;
	int i;
	if(argc < 3){
		fprintf(stderr, "Parametros insuficientes");
		exit(EXIT_FAILURE);
	}

	size = atoi(argv[2]);

	fp = fopen(argv[1], "w");

	for(i=0; i < size; i++){
		putc(random_char(), fp);
	}
	fclose(fp);
	return EXIT_SUCCESS;

}


char random_char(){
	char num  = rand() % (26 * 2);
	if(num < 26){
		return num + 'A';
	}else{
		return num + 'a' - 26;
	}
}

