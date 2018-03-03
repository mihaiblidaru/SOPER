/**
 * Ejercicio 5a
 * Programa que crea una secuencia de hijos de la forma:
 * 
 * Padre -- Hijo1 -- Hijo2 -- Hijo3
 * 
 * Cada proceso espera a que termine su unico hijo.
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 19/02/2018
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Limita el número de procesos que se generan.
 */
#define NUM_PROC 6

/**
 * Punto de entrada en el programa
 */
int main (void){
    int pid;
    int i;
    int status;
    
    for (i=0; i <= NUM_PROC; i++){
        if (i % 2 != 0) {
            if ((pid=fork()) < 0 ){
                printf("Error haciendo fork\n");
                exit(EXIT_FAILURE);
            }else if (pid == 0){
                printf("[HIJO] PID = %d -----> [PADRE] PID = %d\n", getpid(), getppid());
            }else{
                printf ("[PADRE] PID = %d ----> [HIJO CREADO] = %d\n", getpid(), pid);  
            	waitpid(pid,  &status, 0);
            	break;
            }
        }
    }
    exit(EXIT_SUCCESS);
}
