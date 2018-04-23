/**
 * @file ejercicio2.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 23 Apr 2018
 * @brief Ejercicio 2 práctica 3. Sistemas Operativos
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


/**
 * FILE para la llamada a ftok. 
 */
#define FTOK_FILE "/bin/nc"


/**
 * PROJ_ID para ftok. Sirve cualquier número
 */
#define FTOK_PROJ_ID 15

/**
 * Número máximo de segundos que un proceso cliente espera antes
 * de preguntar los datos del cliente
 */
#define RANDMAX 5

/**
 * Estructura usada para compartir datos con un segmento de memoria compartida
 */
typedef struct{
    char nombre[80]; /*!< Nombre del cliente */
    int id;   /*!< Id del cliente */
}Info;

/**
 * Manejador de señal vacio para cambiar el comportamiento por defecto de las señales
 */
void dummy_signal_handler(){}

void cliente(int numHijo, Info* shbuf);

/**
 * Punto de entrada en el programa
 */
int main(int argc, char ** argv){
	Info * info = NULL;
	
	int nHijos = -1, zona, i, pid;
    key_t key = -1;

    sigset_t block_set;
    sigset_t oldset;

	sigemptyset(&block_set);
	sigaddset(&block_set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &block_set, &oldset);

    signal(SIGUSR1, dummy_signal_handler);
    
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

    zona = shmget (key, sizeof(Info), IPC_CREAT | SHM_R | SHM_W);
	    if (zona == -1) {
	        fprintf (stderr, "Error creando zona de memoria compartida \n");
	        perror("shmget");
	        return -1;
	   }
    printf ("ID zona de memoria compartida: %i\n", zona);
	info = shmat (zona, (char *)0, 0);
	
	/* Limpiamos el segmento compartido. */
	memset(info, 0, sizeof(Info));

    for(i=0; i < nHijos; i++){
        if ((pid = fork()) < 0){
            fprintf (stderr, "Error en fork\n");
            perror("fork");
            return -1;
        } else if (pid == 0){
        	cliente(i, info);
        }
    }

	while(nHijos > 0){
	    /* cuando sigsuspend devuelve, bloquea la señal SIGUSR1, protegiendo está region
	    para no ser interumpida*/
		sigsuspend(&oldset);
		fprintf(stdout, "Nombre usuario = %sID = %d\n", info->nombre, info->id);
		nHijos--;
	}
	
	while(wait(NULL) > 0);

    shmdt ((char *)info);
    shmctl (zona, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}

/**
 * Codigo del proceso cliente
 *
 * @param numHijo id del hijo
 * @param shbuf puntero al segmento de memoria compartido
 */
void cliente(int numHijo, Info* shbuf){
	srand((numHijo << 5) * time(NULL));
    sleep((rand() % RANDMAX) + 1);
    printf("[Hijo %d]Intoduce nombre del cliente: ", numHijo);
    fgets(shbuf->nombre, 80, stdin);
    shbuf->id++;
    kill(getppid(), SIGUSR1);
    shmdt(shbuf);
    exit(EXIT_SUCCESS);
}