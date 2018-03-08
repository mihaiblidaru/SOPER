/**
 * Ejercicio 4a
 * Programa que lanza un hijo e imprime el pid del proceso padre.
 *
 * @file ejercicio4a.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 19/02/2018
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 * Limita el número de procesos que se generan
 */
#define NUM_PROC 6


/**
 * @brief Punto de entrada en el programa
 */
int main (void){
    int pid;
    int i;
    
    for (i=0; i <= NUM_PROC; i++){
        if (i % 2 == 0) {
            if ((pid=fork()) < 0){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }else if (pid ==0){
                /* Cada hijo imprime su pid y el pid de su padre */
                printf("Soy hijo, mi PID = %d\t PID del padre = %d\n", getpid(), getppid());     
            
            }else{
                printf ("Soy padre, mi PID = %d\t PID del ultimo hijo creado = %d\n", getpid(), pid); 
                
            }
        }
    }
    exit(EXIT_SUCCESS);
}