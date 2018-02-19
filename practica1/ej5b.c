#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 6

int main (void){
    int pid;
    int i;
    int status;
    
    for (i=0; i <= NUM_PROC; i++){
        if (i % 2 != 0) {
            if ((pid=fork()) <0 ){
                printf("Error haciendo fork\n");
                exit(EXIT_FAILURE);
            }else if (pid == 0){
                printf("HIJO %d\n", getpid());   
                break;
            }else{
            	printf ("PADRE %d\n", getpid());  
            	waitpid(pid,  &status, WUNTRACED);
            }
        }
    }

    sleep(30);

    exit(EXIT_SUCCESS);
}
