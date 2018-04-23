/**
 * @file ejercicio6b.c
 * 
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define RAND_LIMIT 300
#define NUM_OP 2
#define MAX_CAJAS 100
#define MAX_ESPERA 2
#define TOTAL_PADRE "cajaTotal.txt"

#define KEY 113412

void handler (int sig, siginfo_t *siginfo, void *context);

//void handler ();
int main(int argc, char ** argv){
    int n_cajas = 5, i;
    pid_t cpid = -1;
    
    
    struct sigaction act;
    memset (&act, 0, sizeof(act));
 	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	

    for(i=0; i < n_cajas; i++){
        if((cpid = fork()) < 0){
            fputs("Error en el fork", stderr);
            return EXIT_FAILURE;
        }else if(cpid == 0){
            printf("Hijo %d creado. Pid = %d \n", i+1, getpid());
            sleep(i+1);
            kill(getppid(), SIGUSR1);
            exit(EXIT_SUCCESS);
        }
    }
    
    int status;
    int pid;
    while (wait(NULL));
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    
    while(!sigwait(&set, &status));
    printf("PadreAcaba\n");
    
    exit(EXIT_SUCCESS);    
}





void  handler (int sig, siginfo_t *siginfo, void *context){
    printf ("Sending PID: %ld, UID: %ld\n",
			(long)siginfo->si_pid, (long)siginfo->si_uid);
	usleep(10000000);
}


