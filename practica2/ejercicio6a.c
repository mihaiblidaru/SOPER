/**
 * @file ejercicio6a.c
 * 
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 */ 

#include <stdlib.h>

/*
 * Modifica el código del enunciado para que: 
 *  - El hijo, justo después de ser creado, establecerá una alarma para ser 
 *    recibida dentro de 40 segundos. 
 *  - Justo antes de comenzar cada bucle de imprimir números en un proceso hijo,
 *    las señales SIGUSR1, SIGUSR2 y SIGALRM deben quedar bloqueadas. 
 *  - Al finalizar el bucle de impresión de números, y antes de la espera de 3 
 *    segundos, se desbloqueará la señal SIGALRM y SIGUSR1. 
 * 
 *  - ¿Qué sucede cuando el hijo recibe la señal de alarma?
 */
int main(){
    exit(EXIT_SUCCESS);    
}