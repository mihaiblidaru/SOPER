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
 * En este ejercicio se implementará un sistema de gestión de transacciones 
 * bancarias en un hipermercado. Así, tendremos una cuenta global (guardada 
 * por el proceso padre) que contabiliza el total de dinero recaudado por todas 
 * las cajas. Habrá N cajas que serán las encargadas de realizar las 
 * transacciones con los clientes. Para simular las operaciones de los clientes,
 * el proceso padre preparará un fichero para cada caja (clientesCaja1.txt, 
 * clientesCaja2.txt …) donde rellenará un listado de 50 operaciones aleatorias 
 * de compra, consistentes en escribir en cada línea del fichero una cantidad 
 * aleatoria entera entre 0 y 300€. 
 * 
 * El proceso padre generará tantos procesos hijo (cajeros) como cajas, y cada 
 * hijo guardará el total de dinero existente en su caja mediante un fichero, 
 * incrementando la cantidad en cada una de las operaciones realizadas con 
 * clientes. Para ello, realizará lecturas consecutivas del fichero de 
 * operaciones de clientes asociado a esa caja e irá incrementando su balance en
 * función de la cantidad pagada por el cliente. Para simular el tiempo de 
 * gestión de cada cliente, cuando un cajero haga una lectura del fichero 
 * realizará también una espera aleatoria de entre 1 y 5 segundos. 
 * 
 * Las cajas, al superar los 1000€, solicitarán que se les retire dinero por 
 * seguridad. El cajero seguirá realizando operaciones y el proceso padre, 
 * cuando tenga la primera oportunidad (la cuenta de la caja no esté siendo 
 * modificada), retirará 900€ de la caja y se los llevará a la cuenta global. 
 * También avisarán al proceso padre de que han procesado a todos los clientes y
 * que, el remanente en la caja debe ser retirado. Las operaciones de retirada 
 * de dinero de las diferentes cajas se realizarán por parte del proceso padre.
 * 
 * Siempre que se realice una operación de lectura de un fichero, el fichero se 
 * abrirá y al realizar la operación de lectura o escritura correspondiente, 
 * el fichero se cerrará. 
 * 
 * Se deberán utilizar señales para avisar de los distintos eventos entre 
 * proceso padre y procesos hijos (por ejemplo, un proceso hijo avisa al padre 
 * de que tiene más de 1000€ en caja) y semáforos para controlar el acceso a los
 * ficheros, de forma que nunca un fichero se modifique por dos procesos 
 * simultáneamente. 
 * 
 * El uso de semáforos se realizará mediante la biblioteca implementada en el 
 * ejercicio anterior.
 */ 
int main(){
    exit(EXIT_SUCCESS);    
}