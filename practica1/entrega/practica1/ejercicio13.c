/**
 * Ejercicio 13: Programa que utiliza dos hilos de ejecución para multiplicar
 * dos matrices cuadradas por dos números enteros. Tras cada iteración registra
 * por qué paso va cada hilo de ejecución.
 * 
 * @file ejercicio13.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 26/02/2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>


/**
 * Dimension máxima de la matriz cuadrada
 */
#define MAX_DIM 5


/**
 * Longitud máxima de una cadena
 */
#define LENGTH 500


/**
 * Estructura necesaria para pasar varios argumentos a un hilo
 */
typedef struct{
    int dim;                /**< Dimension de la matriz cuadrada */
    int**matriz;            /**< La matriz cuadrada */
    int mul;                /**< Número por el cual se multiplica la matriz */
    int num_hilo;           /**< Número del hilo */
    int* progreso_propio;   /**< Ultima fila de la matriz multiplicada del hilo actual */
    int* progreso_hermano;  /**< Ultima fila de la matriz multiplicada del hilo hermano*/
}ThreadArgs;



/**
 * @brief Función que va a utilizar un hilo para multiplicar una matriz por un número
 * @param args Puntero a una estructura que contiene toda la información 
 * necesaria para que se realice la multiplicación de matrices.
 * 
 */
void* MultiplicarMatriz(void *args);


/**
 * @brief Reserva memoria para una matriz cuadrada
 * @param dim Dimensión de la matriz cuadrada
 * @return El puntero a la nueva matriz creada o NULL si se produce algun error en el proceso
 */
int **reservar_matriz_cuadrada(int dim);


/**
 * @brief Libera la memoria usada por una matriz cuadrada
 * inicializada con la función reservar_matriz_cuadrada.
 * 
 * @param mat Matriz que se quiere liberar
 */
void liberar_matriz_cuadrada(int** mat);


/**
 * @brief Punto de entrada a la aplicacion
 * @param argc Número de parametros del programa
 * @param argv Lista de los argumentos del programa
 */
int main(int argc, char**argv){ 
    int dim, mul1, mul2, i, j;
    int **mat1 = NULL, **mat2 = NULL;
    pthread_t h1, h2;
    ThreadArgs argumentos1, argumentos2;
    int progreso_h1 = 0, progreso_h2 = 0;

    printf ("Introduce la dimension de la matriz cuadrada: ");
    
    if(scanf ("%d", &dim) == 1){
        if(dim > MAX_DIM || dim < 1){
            fprintf(stderr, "Error:\t 0 < dimension <= 5\n");
            exit(EXIT_FAILURE);
        }    
    }else{
        fprintf(stderr, "Lo que has introducido no es un numero valido\n");
        exit(EXIT_FAILURE);
    }
    
    
    if(!(mat1 = reservar_matriz_cuadrada(dim))){
        fprintf(stderr, "Error resrvando memoria para la primera matriz cuadrada\n");
        exit(EXIT_FAILURE);
    }
    
    
    if(!(mat2 = reservar_matriz_cuadrada(dim))){
        fprintf(stderr, "Error resrvando memoria para la segunda matriz cuadrada\n");
        liberar_matriz_cuadrada(mat1);
        exit(EXIT_FAILURE);
    }
    
    
    printf("\nIntroduzca multiplicador 1: ");
    if(scanf ("%d", &mul1) != 1){
        fprintf(stderr, "Lo que has introducido no es un numero valido\n");
        exit(EXIT_FAILURE);
    }
    
    
    printf ("\nIntroduzca multiplicador 2: ");
    if(scanf ("%d", &mul2) != 1){
        fprintf(stderr, "Lo que has introducido no es un numero valido\n");
        exit(EXIT_FAILURE);
    }
    
    
    /* Leer la primera matriz*/
    printf("Introduzca matriz 1:\n");
    for(i=0; i < dim; i++){
        for(j=0; j < dim; j++){
            if(scanf("%d", &mat1[i][j]) != 1){
                fprintf(stderr, "Lo que has introducido no es un numero valido\n");
                exit(EXIT_FAILURE);
            } 
        }
    }
    
    /* Leer la segunda matriz*/
    printf("Introduzca matriz 2:\n");
    for(i=0; i < dim; i++){
        for(j=0; j < dim; j++){
            if(scanf("%d", &mat2[i][j]) != 1){
                fprintf(stderr, "Lo que has introducido no es un numero valido\n");
                exit(EXIT_FAILURE);
            } 
        }
    }
    
    /*Configuración del hilo 1 */
    argumentos1.dim = dim;
    argumentos1.matriz = mat1;
    argumentos1.mul = mul1;
    argumentos1.num_hilo = 1;
    argumentos1.progreso_propio = &progreso_h1;
    argumentos1.progreso_hermano = &progreso_h2;
    
    
    /*Configuración del hilo 2*/
    argumentos2.dim = dim;
    argumentos2.matriz = mat2;
    argumentos2.mul = mul2;
    argumentos2.num_hilo = 2;
    argumentos2.progreso_propio = &progreso_h2;
    argumentos2.progreso_hermano = &progreso_h1;

    
    /*Crear los dos hilos*/
    pthread_create(&h1, NULL, MultiplicarMatriz, &argumentos1);
    pthread_create(&h2, NULL, MultiplicarMatriz, &argumentos2);
    
    
    /*Esperar a quea acaben los dos hilos*/
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    
    /*Liberar los recursos reservados */
    liberar_matriz_cuadrada(mat1);
    liberar_matriz_cuadrada(mat2);
    
    exit(EXIT_SUCCESS);
}


/**
 * @brief Función que va a utilizar un hilo para multiplicar una matriz por un número
 * 
 * @param args Puntero a una estructura que contiene toda la información 
 * necesaria para que se realice la multiplicación de matrices.
 */
void* MultiplicarMatriz(void *args){
    ThreadArgs *argumentos = (ThreadArgs*)args;
    char aux[LENGTH]= "";
    int i, j;
    for(i=0; i < argumentos->dim; i++){
        
        sprintf (aux, "Hilo %d multiplicando fila %d resultado ", argumentos->num_hilo, i);
        for(j=0; j < argumentos->dim; j++){
            argumentos->matriz[i][j] *= argumentos->mul;
            sprintf(aux, "%s %d ", aux, argumentos->matriz[i][j]);
        }
        
        *(argumentos->progreso_propio) += 1;
        
        if(*argumentos->progreso_hermano >= argumentos->dim){
            printf("%s - el Hilo %d ha terminado\n", aux, argumentos->num_hilo == 1 ? 2:1);
        }else{
            printf ("%s - el Hilo %d va por la fila %d\n",aux, argumentos->num_hilo == 1 ? 2:1, *argumentos->progreso_hermano);    
        }
        usleep (1000000); 
    }
    pthread_exit(NULL);
}


/**
 * @brief Reserva memoria para una matriz cuadrada.
 * 
 * La reserva se realiza de forma que todos los datos de la matriz esten 
 * contiguos en memoria.
 * @param dim Dimensión de la matriz cuadrada
 * @return El puntero a la nueva matriz creada o NULL si se produce algun error en el proceso
 */
int **reservar_matriz_cuadrada(int dim){
    int** mat = NULL;
    int i;
    if(!(mat = malloc(dim * sizeof(int*)))){
        return NULL;
    }
    
    if(!(mat[0] = malloc(dim * dim * sizeof(int)))){
        free(mat);
        return NULL;
    }
    
    for(i=1; i < dim; i++){
        mat[i] = &mat[0][dim *i];
    }
    return mat;
}



/**
 * @brief Libera la memoria usada por una matriz cuadrada
 * inicializada con la función reservar_matriz_cuadrada.
 * 
 * @param mat Matriz que se quiere liberar
 */
void liberar_matriz_cuadrada(int** mat){
    if(mat){
        free(mat[0]);
        free(mat);
    }
}
