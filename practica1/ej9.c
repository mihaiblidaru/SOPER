#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define OPERANDO_1 6
#define OPERANDO_2 5
#define MAX_LENGTH 50


/**
 * Implementación no recursiva de la funcion matematica factorial.
 * 
 * @param num El número del que se quiere calcular el factorial >= 0
 * 
 * @return El factorial del numero recibido como parametro. -1 Si el número
 * es negativo.
 */
int fact(int num){
    if(num < 0){
        return -1;
    }
    
    int result = 1;
    
    while(num > 1){
        result *= num;
        num--:
    }
    return result;
}

int factorial(int num1, int num2){
    return (num1 >= 0) ? fact(num1))/ num2 : -1;
}

int permutacion(int num1, int num2){
    if (num1<0 || num2<0){
        return -1;
    }
    return fact(num1)/fact(num1-num2);
}


typedef enum{
    POWER,
    FACT,
    PERM,
    ABS
} Function;


int main(int argc, char**argv){

    int pipe1[2] = {0, 0};
    int pipe2[2] = {0, 0};
    
    int pid = -1;
    char message[MAX_LENGTH];
    char received[MAX_LENGTH];
    int result;
    int num1, num2, i, nbytesread;
    
    if (pipe(pipe1) < 0){
        fprintf(stderr, "Error creating pipe1\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe2) < 0){
        fprintf(stderr, "Error creating pipe2\n");
        exit(EXIT_FAILURE);
    }
    
    
    sprintf(message, "%d,%d", OPERANDO_1, OPERANDO_2);
    
    for(i=0; i < 4; i++){
        if ((pid = fork()) == -1){
            printf ("Error in fork\n");
            exit(EXIT_FAILURE);
        }else if (pid == 0){
            printf("\nLanzado hijo %d\n", i);
            /*HIJO*/
            close(pipe1[1]);   
            
            
            
            memset(received, 0, MAX_LENGTH);
            nbytesread = read(pipe1[0],	received, MAX_LENGTH);
            
            printf("\nBytes recibidos: %d\n", nbytesread);
            
            printf("Hijo recibe: %s", received);
            if(sscanf(received, "%d, %d",  &num1, &num2) != 2){
                printf("El mensaje enviado no contiene 2 numeros: %s\n", received);
                exit(EXIT_FAILURE);
            }
            
            switch(i){
                case POWER : result = (int)pow(num1, num2); break;
                
                case FACT : result = factorial(num1, num2); break;
                
                case PERM : result = permutacion(num1, num2); break;
                
                case ABS : result = (int)(abs(num1) + abs(num2)); break;
                
                default : break;
            }
            printf ("\nRESULTADO %d : %d\n", i, result);
            
            memset(message, 0, MAX_LENGTH);
            sprintf(message, "%d", result);
            if(i==0){
                //close(pipe2[0]);    
            }
        
        write(pipe2[1], message, strlen(message));
        
        
        
        exit(EXIT_SUCCESS);
        
        }else{ /* Padre */
            printf("\nMensaje que el padre va a enviar: %s\n", message);
            if(i==0){
               // close(pipe1[0]);
            }
            write(pipe1[1], message, strlen(message));

            //close(pipe2[1]);
            
            memset(received, 0, MAX_LENGTH);
            read(pipe2[0], received, MAX_LENGTH);
            printf("PADRE: Received %s", received);
            
            
            wait(NULL);
        }
        
        
    }

    
    
    exit(EXIT_SUCCESS);
    
    
}



