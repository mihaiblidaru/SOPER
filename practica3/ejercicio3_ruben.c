#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include <time.h>
#include "semaforos.h"

#define SEMKEY1 75798
#define KEY 5763
#define NAMESIZE 80 /*Tamaño máximo para el nombre*/
#define FILEKEY "/bin/cat"
#define PRODUCTOS 100

int contador1=0;
int contador2=0;
int mutex=0;
int fill=0;
int empty=0;
char *cadena;


void aniadirItem(char obj){
    int i=0;
    for(i=0; i<PRODUCTOS; i++){
        if(cadena[i] == '\0'){
            cadena[i] = obj;
            contador1++;
            printf("Se ha producido el item %c y es el item %d\n", obj, contador1);
            return;
        }
    }
    return;
    
}

void leerItem(void){
    int i=0;
    char obj;
    for(i=0; i<PRODUCTOS; i++){
        if(cadena[i] != '\0'){
            obj=cadena[i];
            cadena[i] = '\0';
            contador2++;
            printf("Se ha consumido el item %c y es el item %d\n", obj, contador2);
            return;
        }
    }
    return;
    
}


int main (int argc, char * argv[]){
    
    int key, id_zone, pid;
    int i = 0;
    key_t key1 = ftok(FILEKEY, SEMKEY1);
    unsigned short *mutexSem;
    unsigned short *emptySem;
    unsigned short *fillSem;
    char objeto;
    
    mutexSem = (unsigned short*)calloc(1,sizeof(unsigned short));
    mutexSem[0]=1;
    emptySem = (unsigned short*)calloc(1,sizeof(unsigned short));
    emptySem[0]=PRODUCTOS;
    fillSem = (unsigned short*)calloc(1,sizeof(unsigned short));
    fillSem[0]=0;
    
    
    /*La key identifica unívocamente el semgento compartido de memoria*/
    key = ftok(FILEKEY, KEY);
    if (key == -1) {
      perror ( "Error con la clave para memoria compartida.\n");
      exit(EXIT_FAILURE);
    }
    
    /*Reservando la zona de memoria compartida*/
    id_zone = shmget (key, sizeof(char)*PRODUCTOS, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if (id_zone == -1) {
      perror ("Error creando la zona de memoria compartida.\n");
      exit(EXIT_FAILURE);
    }
    
    cadena = shmat(id_zone, (char *) 0, 0);
    if (cadena == NULL ) { /*Donde está la memoria, tipo de dato, donde empieza*/
      perror("Imposible añadir la cadena a la memoria compartida.\n");
      exit(EXIT_FAILURE);
    }
    
    if(Crear_Semaforo(key1, 1, &mutex) == -1){
        fprintf(stdout, "Error al crear el semaforo\n");
        return -1;
    }
    if(Inicializar_Semaforo(mutex, mutexSem)==-1){
        fprintf(stdout, "Error al inicializar el semaforo\n");
        return -1;
    }
     if(Crear_Semaforo(key1, 1, &fill) == -1){
        fprintf(stdout, "Error al crear el semaforo\n");
        return -1;
    }
    if(Inicializar_Semaforo(fill, fillSem)==-1){
        fprintf(stdout, "Error al inicializar el semaforo\n");
        return -1;
    }
     if(Crear_Semaforo(key1, 1, &empty) == -1){
        fprintf(stdout, "Error al crear el semaforo\n");
        return -1;
    }
    if(Inicializar_Semaforo(empty, emptySem)==-1){
        fprintf(stdout, "Error al inicializar el semaforo\n");
        return -1;
    }
    
    pid = fork();
    
    if(pid < 0){
        perror("Imposible realizar fork");
        exit(EXIT_FAILURE);
    }
    if(pid > 0){  /*Estaremos en el padre, productor*/
        
        while(1){
            
            for(i=0; i<36; i++){
                sleep(1);
                if(i<26){/*Generar las letras a-z*/
                    objeto=i+97;
                }
                else{/*Generar los numeros 0-9*/
                   objeto = i+22;
                }
                
                Down_Semaforo(empty, 0, 1);
                Down_Semaforo(mutex, 0, 1);
                aniadirItem(objeto);
                Up_Semaforo(mutex, 0, 1);    
                Up_Semaforo(fill, 0, 1); 
                 
                if(contador1>=PRODUCTOS){
                    fprintf(stdout, "Hemos llegado al maximo de productos a producir: %d\n", contador1);
                    while(wait(NULL)>0);
                    free(mutexSem);
                    free(emptySem);
                    free(fillSem);
                    shmdt ((char *)cadena);
                    shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
                    Borrar_Semaforo(mutex);
                    Borrar_Semaforo(fill);
                    Borrar_Semaforo(empty);
                    exit(EXIT_SUCCESS); 
                }    
                    
            }
              
            
        }
    }
    if(pid == 0){   /*Estaremos en el hijo, consumidor*/
        while(1){
            if(contador2>=PRODUCTOS){
                fprintf(stdout, "Hemos llegado al maximo de productos a consumir: %d\n", contador2);
                break;
            }
            else{
                    Down_Semaforo(fill, 0, 1);
                    Down_Semaforo(mutex, 0, 1);
                    leerItem();
                    Up_Semaforo(mutex, 0, 1);    
                    Up_Semaforo(empty, 0, 1);
            }
        }
    }
    
    
    while(wait(NULL)>0);
    free(mutexSem);
    free(emptySem);
    free(fillSem);
    shmdt ((char *)cadena);
    shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
    Borrar_Semaforo(mutex);
    Borrar_Semaforo(fill);
    Borrar_Semaforo(empty);
    exit(EXIT_SUCCESS); 
}