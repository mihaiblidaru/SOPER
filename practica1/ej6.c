#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 6

typedef struct{
        char cadena[80];
        int numero;

} Estructura;


int main (void){
    int pid;
    int status;


    Estructura* e = (Estructura*) calloc (1, sizeof(Estructura));
    if (!e){
        return -1;
    }


    if((pid = fork()) <0){
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, WUNTRACED);
    if (pid==0){
        printf ("Inserte un nombre por teclado: ");
        fgets(e->cadena,80, stdin);
        printf("Cadena = %s\n", e->cadena);
    }

    printf("%s\n", e->cadena);

    


    exit(EXIT_SUCCESS);
}
