/**
 * Ejercicio 6
 * 
 * Ejercicio que comprueba si un proceso padre tiene acceso a la memoria del 
 * proceso hijo.
 * 
 * @file ejercicio6.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @file ejercicio6.c
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

/**
 * Longitud máxima de la cadena
 */
#define LENGTH 80

/**
 * Contiene una cadena para comprobar si se comparte la memoria.
 */
typedef struct{
    /*@{*/
    char cadena[LENGTH+1]; /**<cadena que contiene los datos */
    int numero; /**<numero */
    /*@{*/
} Estructura;


/**
 * Punto de entrada en el programa
 */
int main (void){
    int pid;

    Estructura* e = calloc (1, sizeof(Estructura));
    if (!e){
        exit(EXIT_FAILURE);
    }
    
    strcpy(e->cadena, "Contenido inicial que deberia ser sobreescrito por el hijo si comparten memoria");
    
    if((pid = fork()) <0){
        fprintf(stderr, "Error al llamar a fork()\n");
        exit(EXIT_FAILURE);
        
    }else if (pid==0){  //Codigo que ejecuta el hijo
        printf ("Inserte un nombre por teclado: ");
        fgets(e->cadena, LENGTH, stdin);
    
        printf("[HIJO] Cadena = %s\n", e->cadena);
        
        free(e);

    }else{  //Codigo que ejecuta el padre
    
        wait(NULL);
        printf("[PADRE] Cadena = %s\n", e->cadena);
        free(e);
    }

    exit(EXIT_SUCCESS);
}
