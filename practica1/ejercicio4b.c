/**
 * Ejercicio 4b
 * Programa que lanza un hijo e imprime el pid del proceso padre.
 * 
 * @file ejercicio4b.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 19/02/2018
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


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
    
    for (i=0; i <= NUM_PROC; i++){
        if (i % 2 == 0) {
            if ((pid=fork()) <0 ){
                printf("Error haciendo fork\n");
                exit(EXIT_FAILURE);
            }else if (pid == 0){
                printf("Soy hijo, mi PID = %d\t PID del padre = %d\n", getpid(), getppid());   
            }else{
            	printf ("Soy padre, mi PID = %d\t PID del ultimo hijo creado = %d\n", getpid(), pid);  
            }
        }
    }
    
    wait(NULL);
    exit(EXIT_SUCCESS);
}
