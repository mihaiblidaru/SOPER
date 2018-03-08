/**
 * @file ejercicio4.c
 * 
 * 
 * @author Lucia Fuentes
 * @author Mihai Blidaru
 * 
 */ 

#include <stdlib.h>

/*
 * Escribe un programa en C llamado Ejercicio4.c. De forma general, en este 
 * ejercicio un padre creará un proceso hijo que realizará un trabajo. Pasado 
 * un tiempo, solicitará al padre que le releve un nuevo proceso hijo. El padre 
 * creará un nuevo proceso hijo y será, este nuevo proceso hijo, el encargado de
 * avisar al hijo anterior de que ya puede terminar. El nuevo hijo empezará a 
 * trabajar únicamente cuando el hijo anterior haya terminado. 
 * 
 * Tras crear N hijos, el padre cambiará el comportamiento al recibir la 
 * solicitud de un hijo para ser relevado. Este nuevo comportamiento consistirá 
 * en no crear un nuevo relevo, enviar una señal al proceso que solicita ser 
 * relevado para que termine y, a continuación, terminará también el padre, 
 * asegurándose de que no queda ningún proceso hijo vivo. 
 * 
 * De forma más específica, el trabajo que realizará cada hijo consistirá en: 
 *      - Imprimir el mensaje “Soy <PID> y estoy trabajando” y dormir un segundo 
 *        repetidamente. 
 * 
 *      - Pasado un tiempo de 10 segundos (tras imprimir 10 veces el mensaje), 
 *        este proceso hijo enviará una señal al proceso padre solicitando que 
 *        otro hijo le tome el relevo. Mientras tanto, seguirá imprimiendo el 
 *        mensaje cada 1 segundo.
 */
int main(){
    exit(EXIT_SUCCESS);    
}