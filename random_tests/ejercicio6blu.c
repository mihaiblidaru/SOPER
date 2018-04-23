/**
 * @file ejercicio6b.c
 * 
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define NUM_PROC 5

#define NUM_SEC 40

/*
 * Modifica el código del enunciado para que: 
 *  - El padre enviará la señal SIGTERM al proceso hijo cuando hayan pasado 40 
 *    segundos de la creación del hijo. 
 *  - El proceso hijo, al recibir la señal SIGTERM, imprimirá el mensaje 
 *    “Soy <PID> y he recibido la señal SIGTERM” y finalizará su ejecución. 
 *  - Cuando el hijo haya finalizado su ejecución, el padre creará el siguiente 
 *    hijo.
 */
 
 
void mensaje();
void alarma();

int main (void){
    int pid, counter;
    int i;
    signal(SIGALRM, alarma);

   // for(i=0; i < NUM_PROC; i++){
        pid = fork();
        if(pid < 0){
            printf ("Error en fork\n");
            exit(EXIT_FAILURE);
        }else if (pid == 0){
            signal(SIGTERM, mensaje);
            while(1){
                for (counter = 0; counter < NUM_PROC; counter++){
                    printf("%d\n", counter);
                    sleep(1);
                }
                sleep(3);
            }
        }else{
            alarm(NUM_SEC);
            pause();
            kill(pid, SIGTERM);
            wait(NULL);
        }
    //}
    
    return 0;
}
void mensaje(){
    printf ("Soy %d y he recibido la señal SIGTERM\n", getpid());
    exit(EXIT_SUCCESS);
}

void alarma(){
    printf ("[PADRE]Señal de alarma recibida\nEnviar señal SIGTERM");
    
}