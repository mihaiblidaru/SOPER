/**
 * Ejercicio 8: Usa la familia de funciones exec para ejecutar comandos
 * 
 * El programa puede recibir como argumentos N comandos que estén en la siguiente
 * lista: <ls, du, df> seguidos de un párametro que indica que función exec
 * hay que usar para lanzar esos comandos. Los parámetros disponobles son:
 * 
 * -l:	execl
 * -lp: eleclp
 * -v:	execv
 * -vp:	execvp
 * 
 * El progama lanza N hijos(esperando a que acabe cada uno) y sustituye la 
 * imagen del proceso de estos usando las funciones exec.
 * 
 * @file ejercicio8.c
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 * @date 26/02/2018
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


/**
 * Enumeración de los tipos de funcion exec que se van a usar en este programa. 
 * Se usa para evitar hacer 2 veces una comparación de cadenas.
 */
typedef enum{
	EXECL,
	EXECLP,
	EXECV,
	EXECVP
}ExecFunction;


/**
 * @brief Función que imprime la ayuda del programa cuando se detecta que no hay 
 * suficientes argumentos o que alguno es incorrecto.
 */
void print_help();


/**
 * @brief Punto de entrada en la aplicación.
 * Dentro de está funcion se realiza todo el trabajo. 
 */
int main (int argc, char*argv[]){
	pid_t pid;
	int i;
	ExecFunction fun = -1;
	char *arguments[2] = {NULL};
	char* ls_path = "/bin/ls";
	char* df_path = "/bin/df";
	char* du_path = "/usr/bin/du";
	
	if (argc < 3){
		print_help();
		exit(EXIT_FAILURE);
	}
	
	
	/* Comprobar que el ultimo argumento es el correcto. Si no lo es
	se le muestra un mensaje de error al usuario y se sale del programa.*/
	if(!strcmp(argv[argc-1], "-l")){
		fun = EXECL;	
	} else if(!strcmp(argv[argc-1], "-lp")){
		fun = EXECLP;
	} else if(!strcmp(argv[argc-1], "-v")){
		fun = EXECV;
	} else if(!strcmp(argv[argc-1], "-vp")){
		fun = EXECVP;
	} else{
		print_help();
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc-1; i++){
		
		/* Comprobar que los comandos introducidos están dentro de los 3 
		soportados y preparar los argumentos para las funciones execl.
		Si uno de los comando introducidos no está en la lista, se ignora y se 
		pasa al siguente. Si se usan las funciones EXECL o EXECV se preparan los
		argumentos */
		if(!strcmp(argv[i], "ls")){
			arguments[0] = (fun == EXECL || fun == EXECV) ? ls_path:argv[i];	
		}else if(!strcmp(argv[i], "du")){
			arguments[0] = (fun == EXECL || fun == EXECV) ? du_path:argv[i];	
		}else if(!strcmp(argv[i], "df")){
			arguments[0] = (fun == EXECL || fun == EXECV) ? df_path:argv[i];	
		}else{
			printf("\nEl comando \"%s\" no está soportado.\n", argv[i]);
			printf("No se va a lanzar un proceso hijo. Continuando\n\n");
			continue;
		}
		
		if((pid = fork()) < 0){
			fprintf(stderr, "Error en el fork.\n");
			exit(EXIT_FAILURE);
		}else if(pid == 0){
			switch(fun){
				case EXECL:
						  /bin/ls        /bin/ls
 					execl(arguments[0], arguments[0], (char*)NULL); break;
				
				case EXECLP:
					execlp(arguments[0], arguments[0], (char*)NULL); break;
				
				case EXECV:
					execv(arguments[0], arguments); break;
					
				case EXECVP:
					execvp(arguments[0], arguments);
			}
		}else{ 
			/*Espera a que el proceso hijo acabe antes de llamar al siguente*/
			wait(NULL);
		}
	}
	
	exit(EXIT_SUCCESS);
}


/**
 * @brief Función que imprime la ayuda del programa cuando se detecta que no hay 
 * suficientes argumentos o que alguno es incorrecto.
 */
void print_help(){
	printf("Tienes que usar una de estas funciones:\n\n");
	printf("\t-l: execl\n\t-lp: eleclp\n\t-v: execv\n\t-vp: execvp\n");
	printf("\nEjemplo:\t ./ej6 ls df du -l\n\n\n");
}