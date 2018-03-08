/**
 * Ejercicio 12a : Ejercicio que mide el tiempo que tarda un proceso padre
 * en lanzar 100 hijos y que en cada proceso hijo se calculen los números
 * primos desde 1 hasta el argumento introducido por teclado
 * 
 * @file ejercicio12a.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 * @date 26/02/2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

/**
 * Longitud máxima de cadenas
 */
#define LENGTH 100

/**
 * Número de hijos que se van a lanzar
 */
#define NUM_HIJOS 100

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
    char cadena[LENGTH+1]; /**< Cadena de caracteres requerida */
    int num;  /**< Número requerido */
} Estructura;

/**
 * @brief Comprueba que un número n es primo.
 * @param n Número que se quiere comprobar que es primo.
 * @return FALSE si el número no es primo o < 2. TRUE si el número es primo
 */
int esPrimo(int n);


/**
 * @brief Calcula el número de primos desde 1 hasta limit
 * @param limit Número límite hasta el cual se calculan los números primos
 */
int calcular_primos(int limit);

 
/**
 * @brief Punto de entrada a la aplicacion
 * @param argc Número de parametros del programa
 * @param argv Lista de los argumentos del programa
 */
int main(int argc, char**argv){
    pid_t pid; 
    int i, limite;
    float tiempo_total;
    Estructura *e = NULL;
    struct timespec t1, t2;
    
    if (!(e= malloc (sizeof(Estructura)))){
        printf ("Error\n");
        return (EXIT_FAILURE);
    }
    
    if(argc != 2){
        printf ("Numero invalido de argumentos: se espera INT\n");
        return(EXIT_FAILURE);
    }
    
    limite = atoi(argv[argc-1]);
    
    if (limite < 0){
        printf ("Argumento incorrecto\n");
        return(EXIT_FAILURE);
    }
    
    clock_gettime(CLOCK_REALTIME, &t1);
    for(i=0; i < NUM_HIJOS; i++){
        if ((pid = fork())<0){
            printf ("Error en la creacion de hijos\n");
            return(EXIT_FAILURE);
        } else if (pid == 0){
            calcular_primos(limite);
            exit(EXIT_SUCCESS);
        } else{
            wait(NULL);
        }
    }
    
    clock_gettime(CLOCK_REALTIME, &t2);
    
    if(t1.tv_sec == t2.tv_sec){
        tiempo_total = t2.tv_nsec - t1.tv_nsec;
    }else{
        tiempo_total = (t2.tv_sec - t1.tv_sec - 1) * 1E9;
        tiempo_total += ((1E9 - t1.tv_nsec) + t2.tv_nsec);
    }
    
    tiempo_total /= 1e6;
    
    printf("Tiempo total para crear %d hijos y calcular %d primos: %f ms\n", NUM_HIJOS, limite, tiempo_total);
    
    return(EXIT_SUCCESS);
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
 * Calcula el número de primos desde 1 hasta limit
 * @param limit Número límite hasta el cual se calculan los números primos
 * @return n_primos Número de primos que hay desde 1 hasta limit. -1 si el 
 * límite es negativo.
 */
int calcular_primos(int limit){
    int i, n_primos;
    if (limit<0){
        return -1;
    }
    
    for(i=0, n_primos=0; n_primos < limit; i++){
        if(esPrimo(i)){
            n_primos++;
        }
    }
    return n_primos;
}