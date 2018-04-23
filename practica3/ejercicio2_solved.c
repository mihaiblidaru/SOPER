/*
 * ejercicio2.c
 *
 *  Created on: Apr 22, 2018
 *      Author: mihai
 */


#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "semaforos.h"
#include <time.h>
#define FTOK_FILE "/bin/nc"
#define FTOK_PROJ_ID 15

#define RANDMAX 5

/*pide un programa escrito en lenguaje C, ejercicio2.c. El programa generará n procesos hijos
(n es un argumento de entrada al programa). El proceso padre reservará un bloque de
memoria, que compartirá con los procesos hijo, suficiente para una estructura del tipo

Cuando el proceso padre reciba la señal SIG_USR1 leerá de la zona de memoria compartida
e imprimirá su contenido, nombre del usuario e identificador.
Cada proceso hijo realizará los siguientes pasos:
o dormirá un tiempo aleatorio y
o solicitará al usuario que dé de alta un cliente (recoger el nombre del cliente)
o verificará de la zona de memoria compartida cuál fue el último id y lo
incrementará en una unidad.
o enviará la señal SIG_USR1 al proceso padre
o terminará correctamente.
El proceso padre terminará cuando todos los proceso hijo hayan terminado.
1. Explica en qué falla el planteamiento de este ejercicio.
2. Implementa un mecanismo ( ejercicio2_solved.c ) para solucionar este problema basado
en tu conocimiento de la asignatura
*/

typedef struct{
    char nombre[80];
    int id;
}Info;


void sig_usr1(){}

void cliente(int numHijo, int semid, Info* shbuf);




int main(int argc, char ** argv){
	Info * info = NULL;
	int semid = -1;
	int nHijos = -1, zona, i, pid;
    key_t key = -1;

    sigset_t block_set;
    sigset_t oldset;

	sigemptyset(&block_set);
	sigaddset(&block_set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &block_set, &oldset);

    if (argc < 2){
        printf("Error en el numero de argumentos:\n\t./ejercicio2_solved nHijos\n");
        return -1;
    }

    nHijos = atoi(argv[1]);
    if (nHijos <= 0){
        printf ("El numero de hijos tiene que ser positivo\n");
        return -1;
    }

    if((key = ftok(FTOK_FILE, FTOK_PROJ_ID))==-1){
        perror("ftok");
        exit(-1);
    }

    Crear_Semaforo(12344, 1, &semid);
    Up_Semaforo(semid, 0, 0);

    zona = shmget (key, sizeof(Info), IPC_CREAT |SHM_R | SHM_W);
	    if (zona == -1) {
	        fprintf (stderr, "Error creando zona de memoria compartida \n");
	        perror("shmget");
	        return -1;
	   }

	info = shmat (zona, (char *)0, 0);
	/**
	 * Limpiamos el segmento compartido.
	 */
	memset(info, 0, sizeof(Info));

	signal(SIGUSR1, sig_usr1);
    printf ("ID zona de memoria compartida: %i\n", zona);

    for(i=0; i < nHijos; i++){
        if ((pid = fork()) < 0){
            fprintf (stderr, "Error en fork\n");
            perror("fork");
            return -1;
        } else if (pid == 0){
        	cliente(i, semid,  info);
        }
    }

	while(nHijos > 0){
		sigsuspend(&oldset);

		fprintf(stdout, "Nombre usuario = %sID = %d\n", info->nombre, info->id);
		Up_Semaforo(semid, 0, 0);
		nHijos--;
	}

    shmdt ((char *)info);
    shmctl (zona, IPC_RMID, (struct shmid_ds *)NULL);
    semctl(semid, 0, IPC_RMID);
    return EXIT_SUCCESS;
}

void cliente(int numHijo, int semid, Info* shbuf){
	srand(numHijo * time(NULL));
    sleep((rand() % RANDMAX) + 1);

    Down_Semaforo(semid, 0, 0);
    printf("[Hijo %d]Intoduce nombre del cliente: ", numHijo);
    fgets(shbuf->nombre, 80, stdin);
    shbuf->id++;
    kill(getppid(), SIGUSR1);
    shmdt(shbuf);
    exit(EXIT_SUCCESS);
}

