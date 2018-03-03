#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAX_DIM 5
#define LENGTH 500

typedef struct{
    int dim;
    int**matriz;
    int mul;
    int num_hilo;
    int* progreso_propio;
    int* progreso_hermano;
}ThreadArgs;

void* SuperFuncion(void *args){
    ThreadArgs *argumentos = (ThreadArgs*)args;
    char aux[LENGTH]= "";
    int i, j;
    for(i=0; i < argumentos->dim; i++){
        
        sprintf (aux, "Hilo %d multiplicando fila %d resultado ", argumentos->num_hilo, i);
        for(j=0; j<argumentos->dim; j++){
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

int **reservar_matriz_cuadrada(int dim){
    int** mat = NULL;
    int i;
    if(!(mat = malloc(dim * sizeof(int*)))){
        return NULL;
    }
    
    if(!(mat[0] = malloc(dim * dim * sizeof(int)))){
        return NULL;
    }
    
    for(i=1; i < dim; i++){
        mat[i] = &mat[0][dim *i];
    }
    return mat;
}

void liberar_matriz_cuadrada(int** mat){
    if(mat){
        free(mat[0]);
        free(mat);
    }
}

int main(int argc, char**argv){ 
    int dim, mul1, mul2, i, j;
    int **mat1 = NULL, **mat2 = NULL;
    ThreadArgs argumentos1, argumentos2;
    pthread_t h1, h2;
    int progreso1 = 0, progreso2 = 0;

    printf ("Introduce la dimension de la matriz cuadrada: ");
    scanf ("%d", &dim);
    
    if(dim > MAX_DIM || dim < 1){
        fprintf(stderr, "La 0 < dimension <= 5\n");
        exit(EXIT_FAILURE);
    }
    
    mat1 = reservar_matriz_cuadrada(dim);
    if(!mat1){
        exit(EXIT_FAILURE);
    }
    
    mat2 = reservar_matriz_cuadrada(dim);
    if(!mat2){
        exit(EXIT_FAILURE);
    }
    
    printf ("\nIntroduzca multiplicador 1: ");
    scanf ("%d", &mul1);
    
    printf ("\nIntroduzca multiplicador 2: ");
    scanf ("%d", &mul2);
    
    if (mul1<0 || mul2<0){
        printf ("Multiplicadores positivos\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Introduzca matriz 1:\n");
    for(i=0; i < dim; i++){
        for(j=0; j < dim; j++){
            scanf("%d", &mat1[i][j]); 
        }
    }
    fflush(stdin);
    
    printf("Introduzca matriz 2:\n");
    for(i=0; i < dim; i++){
        for(j=0; j < dim; j++){
            scanf("%d", &mat2[i][j]); 
        }
    }
    
    argumentos1.dim = dim;
    argumentos1.matriz = mat1;
    argumentos1.mul = mul1;
    argumentos1.num_hilo = 1;
    argumentos1.progreso_propio = &progreso1;
    argumentos1.progreso_hermano = &progreso2;
    
    argumentos2.dim = dim;
    argumentos2.matriz = mat2;
    argumentos2.mul = mul2;
    argumentos2.num_hilo = 2;
    argumentos2.progreso_propio = &progreso2;
    argumentos2.progreso_hermano = &progreso1;

    
    pthread_create(&h1, NULL, SuperFuncion, &argumentos1);
    pthread_create(&h2, NULL, SuperFuncion, &argumentos2);
    
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    
    liberar_matriz_cuadrada(mat1);
    liberar_matriz_cuadrada(mat2);
    
    exit(EXIT_SUCCESS);
}




