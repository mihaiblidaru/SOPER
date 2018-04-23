/**
 * @file cadena_montaje.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 23 Apr 2018
 * @brief Ejercicio 5 práctica 3. Implementa la comunicacion entre procesos usando
 * colas de mensajes.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>


/**
 * Longitud maxima de los mensajes que se envian a las colas de mensajes
 */
#define LONGITUD_MAXIMA 2048

/**
 * Valor por defecto del parametro projid para la funcion ftok
 */
#define PROJ_ID 20

void procesoA(char*, char*);
void procesoB(int, char*, char*);
void procesoC(int, char*);
void comprobar_salida(char*, char*);


/**
 * Estructura usasa para añadir mensajes a una cola de mensajes
 */
typedef struct _Mensaje{
	long id;                     /*!< Id del mensaje */  
	int size;                    /*!< Longitud del mensaje */
	char data[LONGITUD_MAXIMA];  /*!< Contenido del mensaje */
}Mensaje;


/**
 * Punto de entrada en el programa.
 * Comprueba el numero de argumentos y lanza el primer
 * proceso de la cadena de montaje. Cuando todos los procesos acaban,
 * comprueba que la salida es la correcta.
 */
int main(int argc, char** argv){
	pid_t cpid;
	int cstatus;

	if(argc < 3 ){
		fprintf(stderr, "Parametros insuficientes\n\t%s <entrada> <salida>", argv[0]);
		exit(EXIT_FAILURE);
	}

	if(!strcmp(argv[1], argv[2])){
		fprintf(stderr, "El fichero de entrada no puede ser el mismo que el de salida\n");
		exit(EXIT_FAILURE);
	}

	if((cpid=fork()) < 0){
		perror("fork");
		exit(EXIT_FAILURE);
	}else if(cpid == 0){
		procesoA(argv[1], argv[2]);
	}

	waitpid(cpid, &cstatus, 0);
	if(WIFEXITED(cstatus)){
		printf("Proceso A terminado correctamente con codigo %d\n", WEXITSTATUS(cstatus));
	}

	comprobar_salida(argv[1], argv[2]);

	exit(EXIT_SUCCESS);
}


/**
 * Recoge la funcionalidad del proceso A. Le de un fichero y escribe en la primera cola de
 * mensajes trozos del fichero de longitud máxima 2KB.
 *
 * @param input ruta al fichero con los datos de entrada
 * @param output ruta al fichero donde se va a escribir la salida de la cadena de montaje
 */
void procesoA(char* input, char* output){
	FILE* fp = NULL;
	Mensaje msg;
	key_t key = -1;
	pid_t cpid = -1;

	int msqid1;
	int cstatus;
	int size = 0, read = 0;

	if(!(fp = fopen(input, "r"))){
		perror(input);
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));

	key = ftok (input, PROJ_ID);
	if (key == -1){
		perror("Error al obtener clave para cola mensajes\n");
		exit(EXIT_FAILURE);
	}
	msqid1 = msgget (key, 0600 | IPC_CREAT);
	if(msqid1 == 1){
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	if((cpid=fork()) < 0){
		perror("fork");
		exit(EXIT_FAILURE);
	}else if(cpid == 0){
		procesoB(msqid1, input, output);
	}

	msg.id = 1;

	size = (rand() % LONGITUD_MAXIMA) + 1;

	while((read = fread(&msg.data, 1, size, fp)) > 0){
		msg.size = read;
		msgsnd(msqid1, &msg, sizeof(msg) - sizeof(long), 0);
		size = (rand() % LONGITUD_MAXIMA) + 1;
	}

	/**
	 * Enviar mensaje al siguiente proceso de que puede acabar
	 */
	msg.size = 0;
	msgsnd(msqid1, &msg, sizeof(msg) - sizeof(long), 0);
	fclose(fp);

	waitpid(cpid, &cstatus, 0);
	if(WIFEXITED(cstatus)){
		printf("Proceso B terminado correctamente con codigo %d\n", WEXITSTATUS(cstatus));
	}

	if(msgctl(msqid1, IPC_RMID, NULL) == -1){
		perror("msgctl, IPC_RMID");
			exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}


/**
 * Cambia cada letra del array apuntado por data por
 * la siguiente en el alfabeto. a -> b y A -> B. Si el
 * caracter no coincide con una letra [a-z] || [A-Z] se
 * deja igual.
 *
 * @param data los datos que hay que cambiar
 * @param size tamaño de los datos
 */
void procesar(char* data, int size){
	int i;
	for(i = 0; i < size; i++){
		if(data[i] >= 'A' && data[i] <= 'Z'){
			if(data[i] == 'Z'){
				data[i] = 'A';
			}else{
				data[i]++;
			}
		}else if(data[i] >= 'a' && data[i] <= 'z'){
			if(data[i] == 'z'){
				data[i] = 'a';
			}else{
				data[i]++;
			}
		}
	}
}


/**
 * Lee mensajes de la cola, cambia su contenido de acuerdo con el enunciado
 * y manda el resultado al proceso C. La cola usada para la comunicacion con el 
 * porceso C se crea en este proceso.
 * @param msqid id de la cola de mensajes de la que leer los mensajes enviados por A
 * @param input ruta del fichero de entrada usado para generar una clave con ftok
 * @param output ruta del fichero de salida para pasarlo al proceso C
 */
void procesoB(int msqid, char* input,  char* output){
	Mensaje msg;
	key_t key = -1;
	int msqid2;
	pid_t cpid = -1;
	int cstatus;

	key = ftok (input, PROJ_ID + 1);
	if (key == -1){
		perror("Error al obtener clave para cola mensajes\n");
		exit(EXIT_FAILURE);
	}

	msqid2 = msgget (key, 0600 | IPC_CREAT);
	if(msqid2 == 1){
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	if((cpid=fork()) < 0){
		perror("fork");
		exit(EXIT_FAILURE);
	}else if(cpid == 0){
		procesoC(msqid2, output);
	}

	while(1){
		msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1, 0);
		if(msg.size > 0){
			procesar(msg.data, msg.size);
			msgsnd(msqid2, &msg, sizeof(msg) - sizeof(long), 0);
		}else{
			msg.size = 0;
			msgsnd(msqid2, &msg, sizeof(msg) - sizeof(long), 0);
			break;
		}
	}

	waitpid(cpid, &cstatus, 0);
	if(WIFEXITED(cstatus)){
		printf("Proceso C terminado correctamente con codigo %d\n", WEXITSTATUS(cstatus));
	}

	if(msgctl(msqid2, IPC_RMID, NULL) == -1){
		perror("msgctl, IPC_RMID");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}


/**
 * Vuelca el contenido de los mensajes que recibe mediante una cola de mensajes
 * a un fichero.
 *
 * @param msqid id de la cola de mensajes que se usa para recibir mensajes del proceso B
 * @param output ruta al fichero de salida
 */
void procesoC(int msqid, char* output){
	Mensaje msg;
	FILE* fp = NULL;
	if(!(fp = fopen(output, "w"))){
		perror(output);
		exit(EXIT_FAILURE);
	}


	while(1){
		msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1, 0);
		if(msg.size > 0){
			fwrite(&msg.data, 1, msg.size, fp);
		}else{
			break;
		}
	}

	fclose(fp);
	exit(EXIT_SUCCESS);
}


/**
 * Comprueba que el contenido del fichero de salida es el correcto.
 * 
 * @param input ruta al fichero de entradfa
 * @param output ruta al fichero de salida
 */
void comprobar_salida(char* input, char* output){
	FILE* in = NULL, *out = NULL;
	int read_in;
	int read_out;
	char buf_in[LONGITUD_MAXIMA];
	char buf_out[LONGITUD_MAXIMA];

	if(!(in = fopen(input, "r"))){
		perror("Error comprobando salida");
		exit(EXIT_FAILURE);
	}

	if(!(out = fopen(output, "r"))){
		perror("Error comprobando salida");
		fclose(in);
		exit(EXIT_FAILURE);
	}

	while((read_in = fread(buf_in, 1, LONGITUD_MAXIMA, in)) > 0){
		read_out = fread(buf_out, 1, LONGITUD_MAXIMA, out);
		procesar(buf_in, read_in);
		if(read_in != read_out || strncmp(buf_in, buf_out, read_in)){
			fprintf(stderr, "Salida incorrecta");
			fclose(in);
			fclose(out);
			exit(EXIT_FAILURE);
		}
	}
	fclose(in);
	fclose(out);
	printf("Salida correcta\n");
}

