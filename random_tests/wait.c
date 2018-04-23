/**
 * @file ejercicio4.c
 * 
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>


#define SIGNAL_RELEVO 16
#define SIGNAL_KILL 17
#define ME_QUIERO_MORIR 8
#define CONTINUAR 5

#define MAX_TIME 3 //LO HE PUESTO A 3 PARA PROBAR PERO ES A 10 
#define NUM_HIJOS 4

 
int padre_espera = 1;
int old_child_pid = -1;
 
 
 /*****************************************************************/
 void new_child_signal(){
    printf ("[P]Señal de relevo\n");
    padre_espera = 0;
    return;
}


void decir_a_hijo_nuevo_que_continue(){
    kill(old_child_pid, CONTINUAR);
}


void kill_child_signal(){
    printf("[H]Hijo %d terminado\n", getpid());
    kill(getppid(), ME_QUIERO_MORIR);
    exit(EXIT_SUCCESS);
}


/*****************************************************************/

int main(void){
    int pid, i, j;
    int sig;
    sigset_t set;
    sigemptyset(&set);
    
    for(j = 0; j < NUM_HIJOS; j++ ){
        if((pid = fork()) < 0){
            printf ("Error en fork\n");
            return EXIT_FAILURE;
        } else if (pid==0){
            /* si existe un hijo anterior le manda una señal SIG_KILL*/
            if(old_child_pid != -1){
                printf("[H]Hijo %d creado, señal de matar a %d\n", getpid(), old_child_pid);
                kill(old_child_pid, SIGNAL_KILL);
                
                sigaddset(&set, CONTINUAR);
                
                /* esperar a que le llegue la señal para continuar*/
                sigwait(&set, &sig);    
            }
            
            for(i=0; i < MAX_TIME; i++){
                printf ("[H]Soy %d y estoy trabajando\n", getpid());
                sleep(1);
            }
            
            signal(SIGNAL_KILL, kill_child_signal);
            
            while(1){
                printf ("[H]Soy %d y estoy trabajando\n", getpid());
                sleep(1);
            }
            
        } else{
            old_child_pid = pid;
            padre_espera = 1;
            
            sigaddset(&set, SIGNAL_RELEVO);
            sigwait(&set, &sig);
            
            if(j == NUM_HIJOS - 1){
                kill(old_child_pid, SIGNAL_KILL);
            }
            
        }
    }
    
    return EXIT_SUCCESS;
}
