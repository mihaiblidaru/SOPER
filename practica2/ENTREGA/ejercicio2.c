/**
 * Ejercicio2
 * Programa que crea procesos hijo y luego los termina con la señal SIGTERM
 * 
 * @file ejercicio2.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 09/04/2018
 * 
 */ 
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


/**
 * Número de procesos hijos que se van a crear
 */
#define NUM_HIJOS 4


/**
 * Numero de segundos que duerme el padre antes de enviar una señal SIGTERM
 * al hijo actual
 */
#define NUM_SEC_PADRE 5

/**
 * Numero de segundos que espera el hijo antes de finalizar
 */
#define NUM_SEC_HIJO 30

/**
 * @brief Punto de entrada en el programa
 */
int main(){
    pid_t pid;
    int i;
    
    for(i=0; i < NUM_HIJOS; i++){
        if ((pid = fork())<0){
            printf ("Error en fork\n");
            return(EXIT_FAILURE);
        } else if (pid == 0){
            printf ("Soy el proceso hijo %d\n", getpid());
            sleep(NUM_SEC_HIJO);
            printf ("Soy el proceso hijo %d y ya me toca terminar.\n", getpid());
            exit(EXIT_SUCCESS);
        } else{
            sleep(NUM_SEC_PADRE);
            kill(pid, SIGTERM);
            wait(NULL);
        }
    }
    
    exit(EXIT_SUCCESS);    
}