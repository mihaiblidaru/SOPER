#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#define LENGTH 100
#define NUM_HILOS 100
#define TRUE 1
#define FALSE 0

typedef struct{
    char cadena[LENGTH+1];
    int tiempo_total;
} Tiempos;

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