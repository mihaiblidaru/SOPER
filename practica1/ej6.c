#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define LENGTH 80


typedef struct{
        char cadena[LENGTH+1];
        int numero;
} Estructura;


int main (void){
    int pid;
    int status;

    Estructura* e = (Estructura*) calloc (1, sizeof(Estructura));
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
    
        printf("HIJO: Cadena = %s\n", e->cadena);
        
        free(e);

    }else{  //Codigo que ejecuta el padre
    
        waitpid(pid, &status, WUNTRACED);
        printf("PADRE: Cadena = %s\n", e->cadena);
        free(e);
    }

    exit(EXIT_SUCCESS);
}
