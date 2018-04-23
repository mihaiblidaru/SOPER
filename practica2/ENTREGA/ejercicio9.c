/**
 * Ejercicio 9
 * Programa que implementa un sistema de gestión de transacciones bancarias
 * que consiste en crear procesos hijo que lleven el recuento de cada caja y
 * vayan añadiendo el dinero de 900 en 900€ a una caja total
 * 
 * @file ejercicio9.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 09/04/2018
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "semaforos.h"

/**
 * Numero limite de dinero para cada transaccion
 */
#define RAND_LIMIT 300

/**
 * Numero de operaciones de clientes de cada caja
 */
#define NUM_OP 50

/**
 * Numero maximo de cajas
 */
#define MAX_CAJAS 100

/**
 * Tiempo maximo de espera entre transacciones
 */
#define MAX_ESPERA 5

/**
 * Nombre del fichero de recuento total de cajas
 */
#define TOTAL_PADRE "cajaTotal.txt"

/**
 * @brief Función para el manejo de la señal de retirada de dinero
 * @param sig Numero de señal
 * @param siginfo Informacion de la señal
 * @param context Reserva para retorno
 */
void handler (int sig, siginfo_t *siginfo, void *context);

/**
 * @brief Función para cerrar los ficheros de texto
 * @param sig Numero de señal
 * @param siginfo Informacion de la señal
 * @param context Reserva para retorno
 */
void recoger_todo(int sig, siginfo_t *siginfo, void *context);

/**
 * @brief Función que realiza el recuento de una caja
 * @param numHijo Numero de caja en la que se va a operar
 */
void hijo(int numHijo);

/**
 * @brief Función para la creación de los ficheros con las transacciones de
 * cada caja
 * @param num_cajas Número de cajas
 * @param total Retorno del total para la comprobación
 */
int crear_ficheros_cajas(int num_cajas, int* total);

/**
 * ID de la lista de semáforos
 */
int semID = -1;

/**
 * Contabiliza el numero de cajas que no han acabado
 */
int num_cajas_activas;

/**
 * Numero inicial de cajas
*/

int num_cajas_total;

/**
 * Suma total real de todas las cajas, calculada en la funcion que genera los ficheros con los clientes.
 * Sirve para probar que el programa ha funcionado correctamente
 */ 
int total_real;

/**
 * @brief Punto de entrada al programa principañ
 */
int main(int argc, char ** argv){
    int n_cajas = 0, i;
    pid_t cpid = -1;
    struct sigaction act;
    FILE* padre = NULL;
    key_t key = -1;
    memset (&act, 0, sizeof(act));
    
    if(argc != 2){
        fprintf(stderr, "Indica el número de cajas\n\n\t%s 21\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    n_cajas = atoi(argv[1]);
    
    if(n_cajas < 1 || n_cajas > MAX_CAJAS){
        fprintf(stderr, "%s es un número de cajas incorecto\n\n\t%s 21\n", argv[1], argv[0]);
        return EXIT_FAILURE;
    }
    
    if(crear_ficheros_cajas(n_cajas, &total_real) != n_cajas){
        fputs("Error creando ficheros con los clientes de cada caja", stderr);
        exit(EXIT_FAILURE);
    }
    printf("\033[0;32mTotal real esperado %d\033[0m\n", total_real);


    if((padre = fopen(TOTAL_PADRE, "w"))==NULL){
        perror(TOTAL_PADRE);
        exit(EXIT_FAILURE);
    }
    fprintf(padre, "0");
    fclose(padre);
    
    
    /* registrar los manejadores de señales */
 	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;
	sigaddset(&act.sa_mask, SIGRTMIN+7);
	sigaction(SIGRTMIN+8, &act, NULL);
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGRTMIN+8);
    act.sa_sigaction = &recoger_todo;
    sigaction(SIGRTMIN+7, &act, NULL);
    
    
    /*Mejor generar una clave en base a un archivo que hemos creado en vez de usar siempre
    una constante, por si otro programa ya lo esta usando*/
    if((key = ftok(TOTAL_PADRE, 'b'))==-1){
        perror("ftok");
        EXIT_FAILURE;
    }
    
    /* crear los semaforos*/
	Crear_Semaforo(key, n_cajas, &semID);
	
	/* inicializar todos los semaforos a 1*/
	unsigned short * vals = calloc(n_cajas, sizeof(unsigned short));
	for(i=0; i < n_cajas;i++){
	    vals[i] = 1;
	}
	Inicializar_Semaforo(semID, vals);
    
    free(vals);
    num_cajas_activas = n_cajas;
    num_cajas_total = n_cajas;
    for(i=0; i < n_cajas; i++){
        if((cpid = fork()) < 0){
            fputs("Error en el fork", stderr);
            return EXIT_FAILURE;
        }else if(cpid == 0){
           hijo(i+1);
        }
    }
    
    while(pause());
    exit(EXIT_SUCCESS);    
}

/**
 * @brief Función que realiza el recuento de una caja
 * @param numHijo Numero de caja en la que se va a operar
 */
void hijo(int numHijo){
    FILE * clientes = NULL;
    FILE* caja = NULL;
    char file_name[30];
    int cliente_actual = 0;
    int contador_clientes = 1;
    union sigval val;
    memset(&val, 0, sizeof(val));
    val.sival_int = numHijo;
    
    
    /*definir semilla para números aleatorios*/
    struct timespec seed;
    clock_gettime(CLOCK_REALTIME, &seed) ;
    srand(seed.tv_nsec);
    
    sprintf(file_name, "clientesCaja%d.txt", numHijo);
    
    if(!(clientes = fopen(file_name, "r"))){
        perror(file_name);
        exit(EXIT_FAILURE);
    }
    
    /*construir el nombre del archivo en el que va a escribir el dinero total de la caja*/
    sprintf(file_name, "recaudacionCaja%d.txt", numHijo);
    
    /*poner un 0 en el archivo*/
    if(!(caja = fopen(file_name, "wb"))){
        perror(file_name);
        exit(EXIT_FAILURE);
    }
    fwrite(&cliente_actual, sizeof(cliente_actual), 1, caja);
    fclose(caja);
    caja = NULL;
    
    while(fscanf(clientes, "%d", &cliente_actual) == 1){
        int total = 0;
        sleep((rand() % MAX_ESPERA) + 1);
        Down_Semaforo(semID, numHijo - 1,SEM_UNDO);
        /*si escribimos el numero en ascii, hay que truncar el archivo si tiene menos
        caracteres de las que tenia antes. En modo binario se leen y se escriben exactamente
        4 bytes(o el tamaño de int de cada sistema).
        */
        if(!(caja = fopen(file_name, "rb+"))){
            perror(file_name);
            exit(EXIT_FAILURE);
        }
        fread(&total, sizeof(total), 1, caja);
        total += cliente_actual;
        fseek(caja, 0, SEEK_SET);
        fwrite(&total, sizeof(total), 1, caja);
        fclose(caja);
        caja = NULL;
        Up_Semaforo(semID, numHijo - 1,SEM_UNDO);
        if(total > 1000){
            printf("\033[0;31m[Caja: %d, ultimo cliente: %d], envia señar recoger\033[0m\n", numHijo, contador_clientes);
            sigqueue(getppid(), SIGRTMIN+8, val);
        }
        contador_clientes++;
    }
   
    fclose(clientes);
    sigqueue(getppid(), SIGRTMIN+7, val);
    printf("[Caja %d] FIN\n", numHijo);
    exit(EXIT_SUCCESS);
}

/**
 * @brief Función para el manejo de la señal de retirada de dinero
 * @param sig Numero de señal
 * @param siginfo Informacion de la señal
 * @param context Reserva para retorno
 */
void handler (int sig, siginfo_t *siginfo, void *context){
    FILE * file = NULL;
    int total;
    int caja = 0;
    int restante = 0;
    char file_name[30];
    int num_caja = siginfo->si_value.sival_int; 
    sprintf(file_name, "recaudacionCaja%d.txt", num_caja);
    
    Down_Semaforo(semID, num_caja - 1, SEM_UNDO);
    if(!(file = fopen(file_name, "rb+"))){
        perror(file_name);
        exit(EXIT_FAILURE);
    }
    fread(&caja, sizeof(caja), 1, file);
    fseek(file, 0, SEEK_SET);
    if(caja > 900){
        restante = caja - 900;
        fwrite(&restante, sizeof(restante), 1, file);
    }
    fclose(file);    
    Up_Semaforo(semID, num_caja - 1, SEM_UNDO);
    
    if(restante != 0){
        if(!(file = fopen(TOTAL_PADRE, "r"))){
            perror(TOTAL_PADRE);
            exit(EXIT_FAILURE);
        }
        fscanf(file, "%d", &total);
        fclose(file);
        
        if(!(file = fopen(TOTAL_PADRE, "w"))){
            perror(TOTAL_PADRE);
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%d", total + 900);
        fclose(file);
    }
}

/**
 * @brief Función para cerrar los ficheros de texto
 * @param sig Numero de señal
 * @param siginfo Informacion de la señal
 * @param context Reserva para retorno
 */
void recoger_todo(int sig, siginfo_t *siginfo, void *context){
    int tmp = 0;
    int caja =0;
    int total;
    char file_name[30];
    FILE*file;
    int num_caja = siginfo->si_value.sival_int; 
    
    sprintf(file_name, "recaudacionCaja%d.txt", num_caja);
    Down_Semaforo(semID, num_caja - 1, SEM_UNDO);
    if(!(file = fopen(file_name, "rb+"))){
        perror(file_name);
        exit(EXIT_FAILURE);
    }
    fread(&caja, sizeof(caja), 1, file);
    fseek(file, 0, SEEK_SET);
    fwrite(&tmp, sizeof(tmp), 1, file);
    fclose(file);
    Up_Semaforo(semID, num_caja - 1, SEM_UNDO);
    file = NULL;
    
    if(!(file = fopen(TOTAL_PADRE, "r"))){
        perror(TOTAL_PADRE);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", &total);
    fclose(file);
    
    if(!(file = fopen(TOTAL_PADRE, "w"))){
        perror(TOTAL_PADRE);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d", total + caja);
    fclose(file);
    
    num_cajas_activas--;
    if(num_cajas_activas == 0){
        /*unico sitio donde realmente podemos recoger el estado de los hijos ya 
        que tenemos confirmacion de que no deberian llegar más señales. Si un proceso
        está bloqueado en el wait y le llega una señal, este se desbloquea*/
        /* tambien podriamos dejar que el OS recoja los procesos zombies*/
        
        for(tmp = 0; tmp < num_cajas_total; tmp++){
            wait(NULL);
        }
        Borrar_Semaforo(semID);
        printf("\033[0;32mTotal real: %d. Total recogido: %d \033[0m\n", total_real, total + caja);
        exit(EXIT_SUCCESS);
    }
}


/**
 * @brief Función para la creación de los ficheros con las transacciones de
 * cada caja
 * @param num_cajas Número de cajas
 * @param total Retorno del total para la comprobación
 */
int crear_ficheros_cajas(int num_cajas, int* total){
    int i,j ;
    *total = 0;
    char file_name[30];
    srand(time(NULL));
    for(i=0; i < num_cajas; i++){
        FILE* file = NULL;
        int num;
        sprintf(file_name, "clientesCaja%d.txt", i+1);
        
        if(!(file = fopen(file_name, "w"))){
            return i;
        }
        
        for(j = 0; j < NUM_OP; j++){
            num = (rand() % RAND_LIMIT) + 1;
            *total += num; 
            fprintf(file, "%d\n", num);
        }
        fclose(file);
        file = NULL;
    }
    return i;
}