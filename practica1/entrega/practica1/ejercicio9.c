/**
 * Ejercicio 9: El proceso padre lanza cuatro hijos y pasa unos números a través
 * de tuberías. Los hijos realizarán operaciones con los números que ha pasado 
 * el padre y devolverán, mediante tuberías, el resultado al padre.
 * Las operaciones que realizan son las siguentes:
 *   Hijo 1: O1^O2
 *   Hijo 2: O1! / O2
 *   Hijo 3: O1! / (O1-O2)!
 *   Hijo 4: |O1| + |O2|

 * @file ejercicio9.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 * @date 26/02/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/**
 * Indica que una funcion ha acabado correctamente
 */ 
#define OK 1

/**
 * Indica que una funcion NO ha acabado correctamente
 */ 
#define ERROR 0

/**
 * Longitud máxima del mensaje enviado por una tuberia
 */ 
#define MAX_LENGTH 500


/**
 * Enumeración utilizada para decidir qué operación realizará el proceso hijo.
 */
typedef enum{
    POWER = 0,
    FACT,
    PERM,
    ABS
} Function;


/**
 * Implementación no recursiva de la funcion matematica factorial.
 * 
 * @param num El número del que se quiere calcular el factorial >= 0
 * 
 * @return El factorial del numero recibido como parametro. -1 Si el número
 * es negativo.
 */
int fact(int num);


/**
 * @brief Implementación de la función factorial pedida en el enunciado,
 * que calcula el factorial del primero y lo divide entre el segundo.
 * 
 * @param num1 El número del que se quiere calcular el factorial >= 0
 * @param num2 El número por el que se va a dividir el factorial del primero
 * @param status Puntero por el cual se devuelve el estado de la operación OK o ERROR
 * 
 * @return El factorial del número 1 dividido entre el segundo número. 
 * -1 Si el número es negativo.
 */
int factorial(int num1, int num2, int *status);


/**
 * @brief Implementación de la función que realiza la permutación de num1 elementos
 * tomados de num2 en num2.
 * 
 * @param num1 Número de elementos de la permutación
 * @param num2 Las permutaciones se toman de num2 en num2
 * @param status Puntero por el cual se devuelve el estado de la operación OK o ERROR
 * 
 * @return La permutación de num1 elementos de num2 en num2 
 * -1 Si el número es negativo.
 */
int permutacion(int num1, int num2, int* status);



/**
 * @brief Punto de entrada en la aplicación.
 * Dentro de está funcion se realiza todo el trabajo. 
 */
int main(int argc, char**argv){
    pid_t pid = -1;
    int pipe1[2], pipe2[2];
    int num1, num2, i;
    double result;
    int op_status = OK;
    char *func_as_str[4] = {"Potencia", "Factorial", "Permutaciones", "Valor absoluto"};
    char message[MAX_LENGTH];
    char received[MAX_LENGTH];
    /* podriamos usar num1 y num2 pero para que se vea claro que los numeros se 
        pasan a traves de tuberias usamos estas variables*/
    int op1, op2; 
    
    if(argc != 3){
        fprintf(stderr, "Argumentos insuficientes\n Ej: %s 5 6\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if(sscanf(argv[1], "%d", &op1) != 1 || sscanf(argv[2], "%d", &op2) != 1){
        fprintf(stderr, "Alguno de los argumentos indicados no es correcto\n Ej: %s 5 6\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if (pipe(pipe1) < 0){
        fprintf(stderr, "Error creando la primera tuberia\n");
        exit(EXIT_FAILURE);
    }
    
    if (pipe(pipe2) < 0){
        fprintf(stderr, "Error creando la segunda tuberia. Cerrando las tuberias abiertas\n");
        close(pipe1[0]);
        close(pipe1[1]);
        exit(EXIT_FAILURE);
    }
    
    /*El mensaje es comun para todos los hijos */
    sprintf(message, "%d, %d", op1, op2);
    
    for(i=0; i < 4; i++){
        if ((pid = fork()) == -1){
            printf ("Error in fork\n");
            exit(EXIT_FAILURE);
            
        }else if (pid == 0){
            
            memset(received, 0, MAX_LENGTH); // limpiar el buffer usado para recibir datos
            memset(message, 0, MAX_LENGTH); // limpiar el buffer usado para mandar datos
            
            /* Cerrar extremos no usados*/
            close(pipe2[0]); /*Cerrar extremo de lectura no usado */ 
            close(pipe1[1]); /*Cerrar extremo de escritura no usado */  

            read(pipe1[0], received, MAX_LENGTH);
            close(pipe1[0]); /* Cerrar el extremo de lectura despues de usarlo */
            
            if(sscanf(received, "%d, %d",  &num1, &num2) != 2){
                printf("El mensaje enviado no contiene 2 numeros: %s\n", received);
                exit(EXIT_FAILURE);
            }
            
            switch(i){
                case POWER : result = pow(num1, num2); break;
            
                case FACT : result = factorial(num1, num2, &op_status); break;
                
                case PERM : result = permutacion(num1, num2, &op_status); break;
                
                case ABS : result = (int)(abs(num1) + abs(num2)); break;
                
                default : break;
            }
            sprintf(message, "Datos enviados a traves de la tubería por el proceso "
                             "PID=%d. Operando 1:%d Operando 2:%d\t%d.%s: ", 
                              getpid(), num1, num2, i+1, func_as_str[i]);
                              
            if(op_status == ERROR){
                sprintf(message, "%s No se puede calcular con los operandos %d %d\n", message, num1, num2);    
            }else{
                sprintf(message, "%s %lf\n", message, result);    
            }
            

            write(pipe2[1], message, strlen(message));
            close(pipe2[1]); 
        
            exit(EXIT_SUCCESS);
        
        }else{ /* Padre */
            write(pipe1[1], message, strlen(message));
            memset(received, 0, MAX_LENGTH);
            read(pipe2[0], received, MAX_LENGTH);
            printf("%s\n", received);
            wait(NULL);
        }
    }
    
    /*Cerrar las pipes en el proceso padre antes de salir. Este es el unico
    momento donde podemos cerrar los pipes ya que si las cerramos algun extremo
    en el bucle(en el padre), el siguiente hijo que se va a crear no va a tener
    disponibles esos extemos. */
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    
    exit(EXIT_SUCCESS);
}


/**
 * @brief Implementación no recursiva de la funcion matematica factorial.
 * 
 * @param num El número del que se quiere calcular el factorial >= 0
 * 
 * @return El factorial del numero recibido como parametro. -1 Si el número
 * es negativo.
 */
int fact(int num){
    if(num < 0){
        return ERROR;
    }
    
    int result = 1;
    
    while(num > 1){
        result *= num;
        num--;
    }
    return result;
}

/**
 * @brief Implementación de la función factorial pedida en el enunciado,
 * que calcula el factorial del primero y lo divide entre el segundo.
 * 
 * @param num1 El número del que se quiere calcular el factorial >= 0
 * @param num2 El número por el que se va a dividir el factorial del primero
 * @param status Puntero por el cual se devuelve el estado de la operación OK o ERROR
 * 
 * @return El factorial del número 1 dividido entre el segundo número. 
 * -1 Si el número es negativo.
 */
int factorial(int num1, int num2, int *status){
    if (num1 < 0 || num2 == 0){
        *status = ERROR;
        return -1;
    }
        
    *status = OK;    
    return (fact(num1))/ num2;
}


/**
 * @brief Implementación de la función que realiza la permutación de num1 elementos
 * tomados de num2 en num2.
 * 
 * @param num1 Número de elementos de la permutación
 * @param num2 Las permutaciones se toman de num2 en num2
 * @param status Puntero por el cual se devuelve el estado de la operación OK o ERROR
 * 
 * @return La permutación de num1 elementos de num2 en num2 
 * -1 Si el número es negativo.
 */
int permutacion(int num1, int num2, int* status){
    if (num1 < 0 || num2 < 0 || num2 > num1){
        *status = ERROR;
        return -1;
    }
    
    *status = OK;   
    return fact(num1) / fact(num1-num2);
}
