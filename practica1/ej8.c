#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

typedef enum{
	EXECL,
	EXECLP,
	EXECV,
	EXECVP
}ExecFunction;



int main (int argc, char*argv[]){
	int pid;
	int i;
	int status; 
	char *args[4] = {NULL};

	if (argc < 3){
		exit(-1);
	}
	ExecFunction fun = -1;
	
	if(!strcmp(argv[argc-1], "-l")){
		fun = EXECL;	
	} else if(!strcmp(argv[argc-1], "-lp")){
		fun = EXECLP;
	} else if(!strcmp(argv[argc-1], "-v")){
		fun = EXECV;
	} else if(!strcmp(argv[argc-1], "-vp")){
		fun = EXECVP;
	}else{
		printf("Especifica la funcion que quieres usar\n");
		exit(EXIT_FAILURE);
	}

	for (i = 1; i <= argc; i++){
		if((pid = fork()) < 0){
			exit(EXIT_FAILURE);
		}else if(pid == 0){
			switch(fun){
				case EXECL:
					execl(argv[i],argv[i], (char*)NULL); break;
				
				case EXECLP:
					execlp(argv[i],argv[i], (char*)NULL); break;
				
				case EXECV:
					args[0] = argv[i];
					execv(argv[i], args); break;
					
				case EXECVP:
					args[0] = argv[i];
					execvp(argv[i], args);
					
				}
			} else{
			waitpid(pid, &status, 0);
		}
	}
	exit(EXIT_SUCCESS);
}