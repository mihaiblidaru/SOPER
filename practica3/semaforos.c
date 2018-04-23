/**
 * Implementación de las funciones de gestión de semáforos
 * @file semaforos.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 09/04/2018
 * 
 */

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include "semaforos.h"
#include <stdio.h>



/**
 * Inicializar_Semaforo
 * 
 * @brief Inicializa los semaforos indicados.
 * @param semid Identificador del semaforo.
 * @param array Valores iniciales.
 * @return int OK si todo fue correcto, ERROR en caso de error.
 */
int Inicializar_Semaforo(int semid, unsigned short *array){
    union semun {
      int val;
      struct semid_ds *semstat;
      unsigned short *array;
    } arg;
    
    arg.array = array;
    
    return semctl(semid, 0, SETALL, arg);
}


/**
 * Borrar_Semaforo
 * 
 * @brief Borra un semaforo.
 * @param semid Identificador del semaforo.
 * @return int OK si todo fue correcto, ERROR en caso de error.
 */
int Borrar_Semaforo(int semid){
    if (semctl(semid, 0, IPC_RMID, NULL)==-1)
        return ERROR;
        
    return OK;
}



/**
 * Crear_Semaforo
 * 
 * @brief Crea un semaforo con la clave y el tamaño 
 * especificado. Lo inicializa a 0.
 * @param key Clave precompartida del semaforo.
 * @param size Tamaño del semaforo.
 * @param semid Identificador del semaforo.
 * @return int ERROR en caso de error, 0 si ha creado el semaforo,
 * 1 si ya estaba creado.
 */
int Crear_Semaforo(key_t key, int size, int *semid){
    int id;
    int ret = ERROR;
    if(size < 1 || semid == NULL)
        return ERROR;
        
    *semid = ERROR;
    
    id = semget(key, size, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
    if(id == -1 && errno == EEXIST){
        id = semget(key, size, SHM_R | SHM_W);
        ret = 1;
    }else{
        ret = 0;
    }
    
    if(id == -1){
        perror("semget");
        *semid = ERROR;
        return ERROR;
    }else{
        unsigned short * ini = NULL;
        ini = calloc(size, sizeof(unsigned short));
        if(ini==NULL){
         perror("calloc");
         return ERROR;
        }
        if (Inicializar_Semaforo(id, ini)==ERROR){
            free(ini);
            return ERROR;
        }
        free(ini);
        *semid = id;
        return ret;
    }
}


/**
 * Down_Semaforo
 * 
 * @brief Baja el semaforo indicado
 * @param id Identificador del semaforo.
 * @param num_sem Semaforo dentro del array.
 * @param undo Flag de modo persistente pese a finalización abrupta.
 * @return int OK si todo fue correcto, ERROR en caso de error.
 */
int Down_Semaforo(int id, int num_sem, int undo){
    if (id<0 || num_sem<0)
        return ERROR;
    
    struct sembuf op;
    op.sem_num = num_sem;
    op.sem_op = -1;
    op.sem_flg = undo;
    
    if (semop(id, &op, 1) == ERROR)
        return ERROR;
        
    return OK;
}


/**
 * DownMultiple_Semaforo
 * 
 * @brief Baja todos los semaforos del array indicado por active.
 * @param id Identificador del semaforo.
 * @param size Numero de semaforos del array.
 * @param undo Flag de modo persistente pese a finalización abrupta.
 * @param active Semaforos involucrados.
 * @return int OK si todo fue correcto, ERROR en caso de error.
 */
int DownMultiple_Semaforo(int id, int size, int undo, int * active){
    int i;
    if (id<0 || size<0 || active == NULL)
        return ERROR;
    
    for(i=0; i < size; i++){
        Down_Semaforo(id, active[i], undo);
    }
    
    return OK;
}


/**
 * @brief Sube el semaforo indicado
 * 
 * @param id: Identificador del semaforo.
 * @param num_sem: Semaforo dentro del array.
 * @param undo: Flag de modo persistente pese a finalizacion
 * 
 * @return  OK si todo fue correcto, ERROR en caso de error.
 */ 
int Up_Semaforo(int id, int num_sem, int undo){
    struct sembuf up;
    if (id<0 || num_sem < 0)
        return ERROR;
    
    up.sem_num = num_sem;
    up.sem_op = 1;
    up.sem_flg = undo;
   
    if (semop(id, &up, 1) == -1)
        return ERROR;
        
    return OK;
}


/**
 * @brief Sube todos los semaforos del array indicado por active.
 * 
 * @param id Identificador del semaforo.
 * @param size Numero de semaforos del array.
 * @param undo Flag de modo persistente pese a finalización abrupta.
 * @param active Semaforos involucrados.
 * 
 * @return OK si todo fue correcto, ERROR en caso de error.
 */
int UpMultiple_Semaforo(int id,int size,int undo, int *active){
    int i;
    if (id < 0 || size < 0 || active == NULL)
        return ERROR;
        
    
    for (i=0; i<size; i++){
        Up_Semaforo(id, active[i], undo);
    }
    
    return OK;
}
