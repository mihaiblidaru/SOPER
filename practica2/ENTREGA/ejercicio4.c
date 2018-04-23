/**
 * Ejercicio 4
 * Programa que crea un proceso hijo, cuando termina de trabajar manda al 
 * proceso padre una señal de relevo, y el padre le releva con un nuevo hijo
 * 
 * @file ejercicio4.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 09/04/2018
 * 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

/**
 * Nombre de la señal de relevo 
 */
#define SIGNAL_RELEVO SIGUSR1

/**
 * Número de segundos que un hijo trabaja antes de pedir un relevo 
 */
#define MAX_TIME 10 

/**
 * Número máximo de procesos que se van a generar
 */ 
#define NUM_HIJOS 4

 
/**
 * @brief Función que se ejecuta cuando el padre recibe una señal de relevo
 */
 void manejador_relevo(){
    printf ("[P]Señal de relevo recibida\n");
    return;
}

/**
 * @brief Punto de entrada en el programa
 */
int main(void){
    int cpid, old_cpid = -1, j;
    
    signal(SIGNAL_RELEVO, manejador_relevo);
    
    for(j = 0; j < NUM_HIJOS; j++ ){
        if((cpid = fork()) < 0){
            printf ("Error en fork\n");
            return EXIT_FAILURE;
        } else if (cpid==0){
            int i;
            if(old_cpid > 0){
                printf("[H]Hijo %d creado, terminar proceso %d\n", getpid(), old_cpid);
                kill(old_cpid, SIGKILL);
            }
            
            for(i=0; i < MAX_TIME; i++){
                printf ("[H]Soy %d y estoy trabajando\n", getpid());
                sleep(1);
            }
            
            printf ("[H]Enviar señal de relevo\n");
            kill(getppid(), SIGNAL_RELEVO);
            
            while(1){
                printf ("[H]Soy %d y estoy trabajando\n", getpid());
                sleep(1);
            }
            
        }else{
            
            old_cpid = cpid;
            
            /*Bloquear hasta que llegue una señal de relevo*/
            pause();  //suponemos que no se va a recibir una señal diferente a 
                      //SIGNAL_RELEVO procedente de otro proceso que no sea el hijo
        }
    }
    
    /** Mata al ultimo hijo */
    kill(old_cpid, SIGTERM);
    
    /** recoger el estado de los hijos */
    for(j=0; j < NUM_HIJOS; j++){
        wait(NULL);
    }
    
    return EXIT_SUCCESS;
}
