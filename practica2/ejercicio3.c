/**
 * Ejercicio3
 * Programa que captura la señal SIGINT y llama a la función captura
 * 
 * @file ejercicio3.c
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * @date 09/04/2018
 * 
 */ 
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Funcion de captura de señales, imprime un mensaje por pantalla
 * @param sennal Señal que captura
 */
void captura (int sennal){
    printf ("Capturada la señal %d \n", sennal);
    fflush (NULL);
    return;
}

/**
 * @brief Punto de entrada en el programa
 */
int main (int argc, char *argv [], char *env [])
{   
    if (signal (SIGINT, captura) == SIG_ERR){
        puts ("Error en la captura");
        exit (1);
    }

    while (1);
    exit (0);
}