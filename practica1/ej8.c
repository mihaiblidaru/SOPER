#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main (int argc, char*argv[]){
	int pid;
	int i;
	char funcion[20] = "\0";

	if (argc < 3){
		exit(-1);
	}

	if (strcmp(argv[argc-1], "-l") && strcmp(argv[argc-1], "-lp") && 
				strcmp(argv[argc-1], "-v") && strcmp(argv[argc-1], "-vp")){

		printf("Especifica la funcion que quieres usar\n");
	}

	for (i = 1; i <= argc; i++){
		pid = fork();
		if(pid == 0){
			execl("/bin/", "ls");
		}
	}
}