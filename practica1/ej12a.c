#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#define LENGTH 100
#define NUM_HIJOS 100
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

int calcular_primos(int limit){
    int i, n_primos = 0;
    
    for(i=0; i < limit; i++){
        if(esPrimo(i)){
            n_primos++;
        }
    }
    return n_primos;
}

int main(int argc, char**argv){
    
    clock_t tiempo1, tiempo2;
    int z, pid, status;
    Tiempos * t = (Tiempos*) malloc (sizeof(Tiempos));
    int limite;
    
    if (!t){
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
    
    
    tiempo1 = clock();
    for(z=0; z < NUM_HIJOS; z++){
        if ((pid = fork())<0){
            printf ("Error en la creacion de hijos\n");
            return(EXIT_FAILURE);
        } else if (pid == 0){
            calcular_primos(limite);
            exit(EXIT_SUCCESS);
        } else{
            waitpid(pid, &status, 0);
            //sprintf(t->cadena[z], "%d", status);
            
        }
    }
    
    tiempo2 = clock();
    t->tiempo_total = (int)((tiempo2 - tiempo1) / (CLOCKS_PER_SEC * 100000));
    float caca = (float)((tiempo2 - tiempo1) / (float)(CLOCKS_PER_SEC/1000.0));
    printf("%d %d\n",(int)tiempo2, (int)tiempo1);
    
    printf("Tiempo total para crear %d procesos hijos: %f ms\n", NUM_HIJOS, caca);
    
    return(EXIT_SUCCESS);
    
}