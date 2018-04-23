/**
 * @file semaforos.h
 * Wrapper basico de las funciones del sistema que permiten trabajar con semaforos.
 * Permite el uso de semaforos de una forma mas intuitiva ocultando ciertos detalles
 * de su funcionamiento.
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 */ 

#ifndef SEMAFOROS_H
#define SEMAFOROS_H

/**
 * Constante de ERROR
 */
#define ERROR -1
/**
 * Constante de OK
 */
#define OK 0

/**
 * Implementación de las funciones de gestión de semáforos
 * @file semaforos.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 09/04/2018
 * 
 */


/**
 * Inicializar_Semaforo
 * 
 * @brief Inicializa los semaforos indicados.
 * @param semid Identificador del semaforo.
 * @param array Valores iniciales.
 * @return int OK si todo fue correcto, ERROR en caso de error.
 */
int Inicializar_Semaforo(int semid, unsigned short *array);


/**
 * Borrar_Semaforo
 * 
 * @brief Borra un semaforo.
 * @param semid Identificador del semaforo.
 * @return int OK si todo fue correcto, ERROR en caso de error.
 */
int Borrar_Semaforo(int semid);


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
int Crear_Semaforo(key_t key, int size, int *semid);


/**
 * Down_Semaforo
 * 
 * @brief Baja el semaforo indicado
 * @param id Identificador del semaforo.
 * @param num_sem Semaforo dentro del array.
 * @param undo Flag de modo persistente pese a finalización abrupta.
 * @return int OK si todo fue correcto, ERROR en caso de error.
 */
int Down_Semaforo(int id, int num_sem, int undo);


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
int DownMultiple_Semaforo(int id, int size, int undo, int * active);


/**
 * @brief Sube el semaforo indicado
 * 
 * @param id: Identificador del semaforo.
 * @param num_sem: Semaforo dentro del array.
 * @param undo: Flag de modo persistente pese a finalizacion
 * 
 * @return  OK si todo fue correcto, ERROR en caso de error.
 */ 
int Up_Semaforo(int id, int num_sem, int undo);


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
int UpMultiple_Semaforo(int id, int size, int undo, int *active);


#endif