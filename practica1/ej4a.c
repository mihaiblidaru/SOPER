/**
 * Ej 4
 * Programa que lanza un hijo e imprime el pid del proceso padre
 * 
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 19/02/2018
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PROC 6

int main (void){
    int pid;
    int i;
    
    for (i=0; i <= NUM_PROC; i++){
        if (i % 2 == 0) {
            if ((pid=fork()) <0 ){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }else if (pid ==0){
                printf("Soy hijo: %d\tPPID = %d\n", getpid(), getppid());     
            }/*else{
                printf ("PADRE %d \n");  
            }*/
        }
    }
    exit(EXIT_SUCCESS);
}