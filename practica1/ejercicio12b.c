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
 * Constante utilizada para el tipo de dato Boolean
 */
#define TRUE 1
/**
 * Constante utilizada para el tipo de dato Boolean
 */
#define FALSE 0

/**
 * Estructura pedida por el enunciado que almacena una cadena y el tiempo total
 * de ejecución
 */
typedef struct{
    char cadena[LENGTH+1];  /**< Cadena de caracteres requerida */
    int tiempo_total;  /**< Almacena el tiempo total de ejecución*/
} Tiempos;


/**
 * Comprueba que un número n es primo.
 * @param n Número que se quiere comprobar que es primo.
 * @return FALSE si el número no es primo o < 2. TRUE si el número es primo
 */
int esPrimo(int n){
    int i, raiz = (int)sqrt(n);
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
 * Calcula el número de primos desde 1 hasta limit
 * @param limit Número límite hasta el cual se calculan los números primos
 */
void *calcular_primos(void* limit){
    int i, n_primos = 0;
    int final = *(int*)limit;
    for(i=0; i < final; i++){
        if(esPrimo(i)){
            n_primos++;
        }
    }
    pthread_exit(NULL);
}

/**
 * Punto de entrada a la aplicacion
 * @param argc Número de parametros del programa
 * @param argv Lista de los argumentos del programa
 */
int main(int argc, char**argv){
    pthread_t hilos[NUM_HILOS];
    clock_t tiempo1, tiempo2;
    int z, limite;
    float tiempo_total;
    
    if(argc != 2){
        printf ("Numero invalido de argumentos: se espera INT\n");
        return(EXIT_FAILURE);
    }
    
    limite = atoi(argv[argc-1]);
    
    if (limite < 0){
        printf ("Argumento incorrecto\n");
        return(EXIT_FAILURE);
    }
    
    Tiempos * t = (Tiempos*) malloc (sizeof(Tiempos));
    
    if (!t){
        printf ("Error\n");
        return (EXIT_FAILURE);
    }
    
    tiempo1 = clock();
    for(z=0; z<NUM_HILOS; z++){
        
        pthread_create(&hilos[z], NULL, calcular_primos, &limite);
        pthread_join(hilos[z], NULL);
    }
    
    tiempo2 = clock();
    
    /*for(z=0; z < NUM_HILOS; z++){
        pthread_join(hilos[z], NULL);
    */
    
    tiempo_total = (float)((tiempo2 - tiempo1) / (CLOCKS_PER_SEC/1000.0));
    
    printf("Tiempo total para crear %d hilos: %f ms\n", NUM_HILOS, tiempo_total);
    
    free(t);
    exit(EXIT_SUCCESS);
}