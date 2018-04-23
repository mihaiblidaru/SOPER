#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include "semaforos.h"
#define FTOK_FILE "/bin/nc" /* */
#define FTOK_PROJ_ID 15

#define BUFFER_SIZE 1000
#define LIMITE 9999

/*
Ejercicio 3. Realizar un programa que implemente el
problema del productor-consumidor. Para ello, se debe crear una región de memoria
compartida que alojará el búfer en el que el productor escribirá los elementos que produzca,
letras en orden alfabético y posteriormente números del 0 al 9, y del que el consumidor
extraerá dichas letras y números mostrándolos en pantalla. Los procesos productor y
consumidor se ejecutarán concurrentemente y la sincronización entre los procesos debe
seguir realizándose a través de semáforos, prestando especial atención para evitar el
interbloqueo entre los procesos. Para implementarlo se debe hacer uso de la biblioteca de
semáforos realizada en la práctica anterior.
*/

int mutex = -1;
int fillCount = -1;
int emptyCount = -1;

char *buf = NULL;
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include "semaforos.h"
#define FTOK_FILE "/bin/nc" /* */
#define FTOK_PROJ_ID 15


#define LIMITE 50
#define BUFFER_SIZE sizeof(char)*LIMITE

#define MUTEX 0
#define FILLCOUNT 1
#define EMPTYCOUNT 2

int mutex = -1;
int fillCount = -1;
int emptyCount = -1;
int consumidos = 0;
int producidos = 0;

int semaforos;

char *buf = NULL;

char producirItem(int i){
    char letra = i;
    return letra;
}

void consumirItem(){
    int j;
    for(j = 0; j < BUFFER_SIZE; j++){
        if (buf[j] != '\0'){
            printf ("Consumido item: %c\n", buf[j]);
            buf[j] = '\0';
            consumidos++;
            return;
        }
    }
}

void addItem(char item){
    int j;
    for (j=0; j < BUFFER_SIZE; j++){
        if (buf[j] == '\0'){
            buf[j] = item;
            printf ("+ [%c]\n", buf[j]);
            producidos++;
            return;
        }
    }
}

int main(int argc, char ** argv){
    key_t key = -1;
    int zona, pid, i;
    //int k;

    //crear key
    if((key = ftok(FTOK_FILE, FTOK_PROJ_ID))==-1){
        perror("ftok");
        exit(-1);
    }
    
    
    zona = shmget (key, BUFFER_SIZE, IPC_CREAT | IPC_EXCL |SHM_R | SHM_W);
    if (zona == -1) {
        fprintf (stderr, "Error creando zona de memoria compartida \n");
        perror("shmget");
        return -1;
    }
    
    buf = shmat (zona, (char *)0, 0);
    memset(buf, 0, BUFFER_SIZE);
    
    
    /**
     * que sentido tiene crear 3 semaforos cuando puedes tener un array de 
     * 3 semaforos ????
     */
    //tres semaforos: un mutex, un fillcount y un emptycount
    //mutex
    if (Crear_Semaforo(key, 1, &semaforos) == ERROR){
        printf ("Error en la creacion de semaforos\n");
        return EXIT_FAILURE;
    }
    unsigned short * valor = calloc(3, sizeof(unsigned short));
	valor[MUTEX] = 1;
	valor[FILLCOUNT] = 0;
	valor[EMPTYCOUNT] = BUFFER_SIZE;
	Inicializar_Semaforo(semaforos, valor);
	free(valor);
	
	if ((pid = fork()) < -1){
	    printf ("Error en fork\n");
	    return EXIT_FAILURE;
	} else if (pid == 0){
	    //CONSUMIDOR
	   while(1){
    	    Down_Semaforo(semaforos, FILLCOUNT, SEM_UNDO); 
    	    Down_Semaforo(semaforos, MUTEX, SEM_UNDO);
    	    consumirItem();
    	    Up_Semaforo(semaforos, MUTEX, SEM_UNDO);
    	    Up_Semaforo(semaforos, EMPTYCOUNT, SEM_UNDO);
    	    
    	    if (consumidos>=LIMITE){
    	        printf ("LIMITE CONSUMIDOS\n");
    	        while(wait(NULL)>0);
                shmdt ((char *)buf);
                shmctl (zona, IPC_RMID, (struct shmid_ds *)NULL);
                Borrar_Semaforo(semaforos);
                exit(EXIT_SUCCESS);
    	    }
    	}
	 } else{
    	//PRODUCTOR
    	while(1){
    	//letras A-Z
        	for(i='A'; i <= 'Z'; i++){
        	    char item = i;
        	    Down_Semaforo(semaforos, EMPTYCOUNT, SEM_UNDO);
        	    Down_Semaforo(semaforos, MUTEX, SEM_UNDO);
        	    addItem(item);
        	    Up_Semaforo(semaforos, MUTEX, SEM_UNDO);
        	    Up_Semaforo(semaforos, FILLCOUNT, SEM_UNDO);
        	    if (producidos>=LIMITE){
        	        printf ("LIMITE PRODUCIDOS\n");
        	        break;
        	    }
            }
            if (producidos>=LIMITE)
        	        break;
            
            //numeros 0-9
            for(i='0'; i<='9'; i++){
                char item = i;
        	    Down_Semaforo(semaforos, EMPTYCOUNT, SEM_UNDO);
        	    Down_Semaforo(semaforos, MUTEX, SEM_UNDO);
        	    addItem(item);
        	    Up_Semaforo(semaforos, MUTEX, SEM_UNDO);
        	    Up_Semaforo(semaforos, FILLCOUNT, SEM_UNDO);
        	    if (producidos>=LIMITE){
        	        printf ("LIMITE PRODUCIDOS\n");
        	        break;
        	    }
            }
            if (producidos>=LIMITE)
        	        break;
    	}
	 }
	
	while(wait(NULL)>0);
    shmdt ((char *)buf);
    shmctl (zona, IPC_RMID, (struct shmid_ds *)NULL);
    Borrar_Semaforo(semaforos);
    exit(EXIT_SUCCESS);
}
char producirItem(int i){
    char letra = i;
    return letra;
}

void consumirItem(){
    int j;
    for(j = 0; j < BUFFER_SIZE-1; j++){
        if (buf[j] != '\0'){
            printf ("Consumido item: %c\n", buf[j]);
            consumidos++;
            buf[j] = '\0';
            return;
        }
    }
}

void addItem(char item){
    int j;
    for (j=0; j < BUFFER_SIZE; j++){
        if (buf[j] == '\0'){
            buf[j] = item;
            printf ("+ [%c]\n", buf[j]);
            return;
        }
    }
}


int main(int argc, char ** argv){
    key_t key = -1;
    int zona, pid, i;
    //int k;
    /*
    buf = (char*)calloc(BUFFER_SIZE, sizeof(char));
    memset(buf, 0, sizeof(*buf));
    esto es totalmente inecesario
    */
    
    //crear key
    if((key = ftok(FTOK_FILE, FTOK_PROJ_ID))==-1){
        perror("ftok");
        exit(-1);
    }
    
    
    //crear zona de memoria
    /*
    zona = shmget (key, sizeof(*buf), IPC_CREAT | IPC_EXCL |SHM_R | SHM_W);
    esto está mal. sizeof(*buf) = 1
    */
    zona = shmget (key, BUFFER_SIZE, IPC_CREAT | IPC_EXCL |SHM_R | SHM_W);
    if (zona == -1) {
        fprintf (stderr, "Error creando zona de memoria compartida \n");
        perror("shmget");
        return -1;
    }
    
    buf = shmat (zona, (char *)0, 0);
    memset(buf, 0, BUFFER_SIZE);
    
    
    /**
     * que sentido tiene crear 3 semaforos cuando puedes tener un array de 
     * 3 semaforos ????
     */
    //tres semaforos: un mutex, un fillcount y un emptycount
    //mutex
    if (Crear_Semaforo(key, 1, &mutex) == ERROR){
        printf ("Error en la creacion de semaforos\n");
        return EXIT_FAILURE;
    }
    unsigned short * valor = calloc(1, sizeof(unsigned short));
	valor[0] = 1;
	Inicializar_Semaforo(mutex, valor);
	
	//fillcount, se inicializa a 0 = numero de objetos en el buffer
    if (Crear_Semaforo(key, 1, &fillCount) == ERROR){
        printf ("Error en la creacion de semaforos\n");
        return EXIT_FAILURE;
    }
    unsigned short * fill = calloc(1, sizeof(unsigned short));
    fill[0] = 0;
	Inicializar_Semaforo(fillCount, fill);
	
	//emptycount, se inicializa a buffer_size(-1?) = numero de espacios donde se pueden meter objetos
    if (Crear_Semaforo(key, 1, &emptyCount) == ERROR){
        printf ("Error en la creacion de semaforos\n");
        return EXIT_FAILURE;
    }
    unsigned short * empty = calloc(1, sizeof(unsigned short));
    empty[0] = BUFFER_SIZE;
	Inicializar_Semaforo(emptyCount, empty);
	
	
	if ((pid = fork()) < -1){
	    printf ("Error en fork\n");
	    return EXIT_FAILURE;
	} else if (pid == 0){
	    //CONSUMIDOR
	   while(1){
        	    Down_Semaforo(fillCount, 0, SEM_UNDO); 
        	    Down_Semaforo(mutex, 0, SEM_UNDO);
        	    consumirItem();
        	    if (consumidos>=LIMITE)
        	        exit(EXIT_SUCCESS);
        	    Up_Semaforo(mutex, 0, SEM_UNDO);
        	    Up_Semaforo(emptyCount, 0, SEM_UNDO);
    	}
	 } else{
    	//PRODUCTOR
    	//letras A-Z
    	for(i=65; i<91; i++){
    	    char item = producirItem(i);
    	    Down_Semaforo(emptyCount, 0, SEM_UNDO);
    	    Down_Semaforo(mutex, 0, SEM_UNDO);
    	    addItem(item);
    	    Up_Semaforo(mutex, 0, SEM_UNDO);
    	    Up_Semaforo(fillCount, 0, SEM_UNDO);
        }
        
        //numeros 0-9
        for(i=48; i<58; i++){
            char item = producirItem(i);
    	    Down_Semaforo(emptyCount, 0, SEM_UNDO);
    	    Down_Semaforo(mutex, 0, SEM_UNDO);
    	    addItem(item);
    	    Up_Semaforo(mutex, 0, SEM_UNDO);
    	    Up_Semaforo(fillCount, 0, SEM_UNDO);
        }
	 }
	 while(wait(NULL)>0);
    free(valor);
    free(empty);
    free(fill);
    shmdt ((char *)buf);
    shmctl (zona, IPC_RMID, (struct shmid_ds *)NULL);
    Borrar_Semaforo(mutex);
    Borrar_Semaforo(fillCount);
    Borrar_Semaforo(emptyCount);
    
    return EXIT_SUCCESS;
}