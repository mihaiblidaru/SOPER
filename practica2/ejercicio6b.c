/**
 * @file ejercicio6b.c
 * 
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 */ 

#include <stdlib.h>

/*
 * Modifica el código del enunciado para que: 
 *  - El padre enviará la señal SIGTERM al proceso hijo cuando hayan pasado 40 
 *    segundos de la creación del hijo. 
 *  - El proceso hijo, al recibir la señal SIGTERM, imprimirá el mensaje 
 *    “Soy <PID> y he recibido la señal SIGTERM” y finalizará su ejecución. 
 *  - Cuando el hijo haya finalizado su ejecución, el padre creará el siguiente 
 *    hijo.
 */
int main(){
    exit(EXIT_SUCCESS);    
}