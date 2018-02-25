/**
 * Ejercicio 5, apartado B
 * 
 * Un único proceso padre da lugar a un conjunto de procesos hijo para i % 2 != 0. 
 * El proceso padre ha de esperar a que termine la ejecución de todos sus 
 * procesos hijo.
 *
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 20/02/2018
 */

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


    exit(EXIT_SUCCESS);
}
