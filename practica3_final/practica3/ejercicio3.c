/**
 * @file ejercicio3.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 23 Apr 2018
 * @brief Ejercicio 3 práctica 3. Implementa el problema del productor-consumidor
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>
#include "semaforos.h"


/**
 * FILE para la llamada a ftok. 
 */
#define FTOK_FILE "/bin/ls"


/**
 * PROJ_ID para ftok. Sirve cualquier número
 */
#define FTOK_PROJ_ID 15


/**
 * Limite de caracteres que se va a producir. Sirve para que el programa
 * pueda parar. Si el limite es mayor que el número de caracteres, se vuelven
 * a producir por el principio empezando por la letra A
 */
#define LIMITE 100


/**
 * Tamaño del segmento de memoria compartida. Para ilustrar el problema del productor-consumidor
 * su valor tiene que ser menor que el LIMITE, de lo contrario solo haria falta un semaforo
 */
#define BUFFER_SIZE 20


/**
 * Indice del semaforo MUTEX dentro del array de semaforos
 */
#define MUTEX 0


/**
 * Indice del semaforo CONTADOR_OCUPADOS dentro del array de semaforos
 */
#define CONTADOR_OCUPADOS 1


/**
 * Indice del semaforo CONTADOR_VACIOS dentro del array de semaforos
 */
#define CONTADOR_VACIOS 2


/**
 * Número total de caracteres que se van a 'producir'
 */
#define NUM_CARACTERES 36


/**
 * Consume el primer caracter distinto de 0 del buffer apuntado por buf.
 * Consumir un caracter supone sustituir el valor actual por cero.
 * 
 * @param buf buffer que contiene los caracteres
 * @param consumidos buffer de caracteres consumidos
 */
void consumirItem(char* buf, int* consumidos){
    int j;
    for(j = 0; j < BUFFER_SIZE; j++){
        if (buf[j] != '\0'){
            printf ("Consumido item: %c\n", buf[j]);
            buf[j] = '\0';
            (*consumidos)++;
            return;
        }
    }
}


/**
 * Añade un caracter a un buffer en el primer hueco
 * que encuentra disponible. Una posicion se considera hueco 
 * si su contenido es un cero
 * @param buf buffer donde se va a añadir el caracter
 * @param item caracter que se va a añadir
 * @param producidos contador de caracteres añadidos
 */
void addItem(char* buf, char item, int* producidos){
    int j;
    for (j=0; j < BUFFER_SIZE; j++){
        if (buf[j] == '\0'){
            buf[j] = item;
            printf ("+ [%c]\n", buf[j]);
            (*producidos)++;
            return;
        }
    }
}

/**
 * Punto de entrada en la aplicacion.
 * 
 * Crea la zona de memoria compartida crea un hijo que actua como consumidor
 * mientras que el padre actua como productor
 */ 
int main(int argc, char ** argv){
    key_t key = -1;
    int zona, pid, i;
    int semid = -1;
    char *buf = NULL;
    ushort valores[3];
    
    //crear key
    if((key = ftok(FTOK_FILE, FTOK_PROJ_ID))==-1){
        perror("ftok");
        exit(-1);
    }
    
    zona = shmget (key, BUFFER_SIZE, IPC_CREAT | SHM_R | SHM_W);
    if (zona == -1) {
        fprintf (stderr, "Error creando zona de memoria compartida \n");
        perror("shmget");
        return -1;
    }
    
    buf = shmat (zona, NULL, 0);
    memset(buf, 0, BUFFER_SIZE);
    
    if (Crear_Semaforo(key, 1, &semid) == ERROR){
        printf ("Error en la creacion de semaforos\n");
        return EXIT_FAILURE;
    }
    
    valores[MUTEX] = 1;
	valores[CONTADOR_OCUPADOS] = 0;
	valores[CONTADOR_VACIOS] = BUFFER_SIZE;
	
	Inicializar_Semaforo(semid, valores);

	if ((pid = fork()) < -1){
	    printf ("Error en fork\n");
	    return EXIT_FAILURE;
	} else if (pid == 0){
	    int consumidos = 0;
	    while(consumidos < LIMITE){
    	    Down_Semaforo(semid, CONTADOR_OCUPADOS, SEM_UNDO); 
    	    Down_Semaforo(semid, MUTEX, SEM_UNDO);
    	    consumirItem(buf, &consumidos);
    	    Up_Semaforo(semid, MUTEX, SEM_UNDO);
    	    Up_Semaforo(semid, CONTADOR_VACIOS, SEM_UNDO);
    	}
    	shmdt (buf);
    	printf ("LIMITE CONSUMIDOS\n");
    	exit(EXIT_SUCCESS);
	} else {
	    //PRODUCTOR
	    int producidos = 0;
        char caracteres[NUM_CARACTERES] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    	
    	while(producidos < LIMITE){
    	    for(i=0; i < NUM_CARACTERES && producidos < LIMITE; i++){
        	    Down_Semaforo(semid, CONTADOR_VACIOS, SEM_UNDO);
        	    Down_Semaforo(semid, MUTEX, SEM_UNDO);
        	    addItem(buf, caracteres[i], &producidos);
        	    Up_Semaforo(semid, MUTEX, SEM_UNDO);
        	    Up_Semaforo(semid, CONTADOR_OCUPADOS, SEM_UNDO);
            }
    	}
    	printf ("LIMITE PRODUCIDOS\n");
	}
	
	while(wait(NULL)>0);
    shmdt (buf);
    shmctl (zona, IPC_RMID, NULL);
    Borrar_Semaforo(semid);
    exit(EXIT_SUCCESS);
}