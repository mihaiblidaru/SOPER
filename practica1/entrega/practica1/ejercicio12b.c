/**
 * Ejercicio 12a : Ejercicio que mide el tiempo que tarda un proceso padre
 * en lanzar 100 hijos y que en cada proceso hijo se calculen los números
 * primos desde 1 hasta el argumento introducido por teclado
 * 
 * @file ejercicio12b.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 * @date 26/02/2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

/**
 * Longitud máxima de cadenas
 */
#define LENGTH 100

/**
 * Numero de hilos
 */
#define NUM_HILOS 100


/**
 * Constante empleada en un tipo de dato Bool equivalente a verdadero
 */
#define TRUE 1

/**
 * Constante empleada en un tipo de dato Bool equivalente a falso
 */
#define FALSE 0

/**
 * Estructura pedida por el enunciado que almacena una cadena y el tiempo total
 * de ejecución
 */
typedef struct{
    char cadena[LENGTH+1];  /**< Cadena de caracteres requerida */
    int num;  /**< Almacena el limite de numero de primos*/
} Estructura;


/**
 * Comprueba que un número n es primo.
 * @param n Número que se quiere comprobar que es primo.
 * @return FALSE si el número no es primo o < 2. TRUE si el número es primo
 */
int esPrimo(int n);


/**
 * Calcula el número de primos desde 1 hasta limit
 * @param e Estructura usada para pasarle limites al hilo
 */
void *calcular_primos(void* e);


/**
 * @brief Punto de entrada a la aplicacion
 * @param argc Número de parametros del programa
 * @param argv Lista de los argumentos del programa
 */
int main(int argc, char**argv){
    pthread_t hilos[NUM_HILOS];
    int i, limite;
    float tiempo_total;
    Estructura *e = NULL;
    struct timespec t1, t2;
    
    if(argc != 2){
        printf ("Numero invalido de argumentos: se espera INT\n");
        return(EXIT_FAILURE);
    }
    
    limite = atoi(argv[argc-1]);
    
    if (limite < 0){
        printf ("Argumento incorrecto\n");
        return(EXIT_FAILURE);
    }
    
    
    if (!(e = malloc (sizeof(Estructura)))){
        printf ("Error\n");
        return (EXIT_FAILURE);
    }
    
    e->num = limite;
    
    clock_gettime(CLOCK_REALTIME, &t1);
    
    for(i=0; i<NUM_HILOS; i++){
        pthread_create(&hilos[i], NULL, calcular_primos, e);
        pthread_join(hilos[i], NULL);
    }
    
    clock_gettime(CLOCK_REALTIME, &t2);
    
    if(t1.tv_sec == t2.tv_sec){
        tiempo_total = t2.tv_nsec - t1.tv_nsec;
    }else{
        tiempo_total = (t2.tv_sec - t1.tv_sec - 1) * 1E9;
        tiempo_total += ((1E9 - t1.tv_nsec) + t2.tv_nsec);
    }
    
    tiempo_total /= 1e6;
    
    printf("Tiempo total para crear %d hilos: %f ms\n", NUM_HILOS, tiempo_total);
    
    free(e);
    exit(EXIT_SUCCESS);
}


/**
 * @brief Comprueba que un número n es primo.
 * @param n Número que se quiere comprobar que es primo.
 * @return FALSE si el número no es primo o < 2. TRUE si el número es primo
 */
int esPrimo(int n){
    int i, raiz = (int)sqrt(n);
    if(n < 2){
        return FALSE;    
    }
    if (n==2)
        return TRUE;
        
    for(i = 2; i <= raiz; i++){
        if (n % i == 0){
            return FALSE;
        }
    }
    return TRUE;
}


/**
 * @brief Calcula el número de primos desde 1 hasta limit
 * @param e Estructura usada para pasarle limites al hilo
 */
void *calcular_primos(void* e){
    Estructura* args = (Estructura*)e;
    int i, n_primos;
    int final = args->num;
    
    
    for(i=0, n_primos=0; n_primos < final; i++){
        if(esPrimo(i)){
            n_primos++;
        }
    }
    pthread_exit(NULL);
}
