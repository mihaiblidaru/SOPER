/**
 * Ejercicio 6b
 * Programa que crea un hijo y le envia una señal de terminación a los 40
 * segundos, cuando el hijo acaba el padre también
 * 
 * @file ejercicio6b.c
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
 * Contador usado para el trabajo del proceso hijo
 */
#define NUM_PROC 5

/**
 * Tiempo de alarma
 */
#define NUM_SEC 40


/**
 * @brief Función que imprime un mensaje cuando un proceso recibe la señal 
 * SIGTERM y finaliza
 */ 
void mensaje();

/**
 * @brief Función que imprime un mensaje cuando un proceso recibe una señal de
 * alarma
 */
void alarma();

/**
 * @brief Punto de entrada al programa principal
 */
int main (void){
    int pid, counter;
    signal(SIGALRM, alarma);

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
    
    printf("[PADRE] Ejecucion finalizada\n");
    return 0;
}


/**
 * @brief Función que imprime un mensaje cuando un proceso recibe la señal 
 * SIGTERM y finaliza
 */
void mensaje(){
    printf ("Soy %d y he recibido la señal SIGTERM\n", getpid());
    exit(EXIT_SUCCESS);
}

/**
 * @brief Función que imprime un mensaje cuando un proceso recibe una señal de
 * alarma
 */
void alarma(){
    printf ("[PADRE] Señal de alarma recibida\n");
}