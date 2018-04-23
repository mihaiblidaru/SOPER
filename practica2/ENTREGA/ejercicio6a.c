/**
 * Ejercicio 6a
 * Programa que crea un proceso hijo y establece una señal de alarma
 * mientras  alterna el bloqueo y desbloqueo de la señal de alarma
 * 
 * @file ejercicio6a.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 09/04/2018
 * 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

/**
 * Sirve como contador para el trabajo del proceso hijo
 */
#define NUM_PROC 5

/**
 * Tiempo de alarma
 */
#define NUM_SEC 40



/**
 * @brief Mensaje de impresión cuando un proceso recibe la señal de alarma
 */
void alarma();


/**
 * @brief Punto de entrada en el programa
 */
int main (void){
    int pid, counter;
    sigset_t lock; //conjunto de señales a bloquear
    sigset_t unlock; //conjunto de señales para desbloquear
    sigset_t oset; //mascara antigua de señales bloqueadas
    
    
    sigemptyset(&lock);
    sigemptyset(&unlock);
    
    sigaddset(&lock, SIGUSR1);
    sigaddset(&lock, SIGUSR2);
    sigaddset(&lock, SIGALRM);
    
    sigaddset(&unlock, SIGALRM);
    sigaddset(&unlock, SIGUSR1);
    
    
    pid = fork();
    if (pid == 0){
        alarm(NUM_SEC+1);
        signal(SIGALRM, alarma);
        
        while(1){
            printf ("Lock\n");
            sigprocmask(SIG_BLOCK, &lock, &oset);
            
            for (counter = 0; counter < NUM_PROC; counter++){
                printf("%d\n", counter+1);
                sleep(1);
            }
            printf ("Unblock\n");
            sigprocmask(SIG_UNBLOCK, &unlock, &oset);
            printf("Espera +3\n");
            sleep(3);
        }
    }
    
    while(wait(NULL) > 0);
    
    return EXIT_SUCCESS;
}


/**
 * @brief Mensaje de impresión cuando un proceso recibe la señal de alarma
 */
void alarma(){
    printf("[%d]Recibo la señal de alarma\n", getpid());
}