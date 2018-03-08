/**
 * @file ejercicio5b.c
 * Ejercicio 5b
 * 
 * Un único proceso padre da lugar a un conjunto de procesos hijo para i % 2 != 0. 
 * El proceso padre ha de esperar a que termine la ejecución de todos sus 
 * procesos hijo. El padre es el unico que crea procesos hijos.
 *
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 20/02/2018
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
 * @brief Punto de entrada en el programa
 */
int main (void){
    int pid;
    int i;
    
    for (i=0; i <= NUM_PROC; i++){
        if (i % 2 != 0) {
            if ((pid=fork()) <0 ){
                printf("Error haciendo fork\n");
                exit(EXIT_FAILURE);
                
            }else if (pid == 0){
                 printf("Soy hijo, mi PID = %d\t PID del padre = %d\n", getpid(), getppid());    
                 break; //Ej hijo imprime su información y sale
                 
            }else{
            	printf ("Soy padre, mi PID = %d\t PID del ultimo hijo creado = %d\n", getpid(), pid);  
            	wait(NULL); //Espera al hijo creados
            }
        }
    }

    exit(EXIT_SUCCESS);
}
