#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


void print_time(char* msg){
    time_t     now;
    struct tm *ts;
    char       buf[80];
    now = time(0);
    ts = localtime(&now);
    strftime(buf, sizeof(buf), "%H:%M:%S", ts);
    printf("[%s]%s\n", buf, msg);
}

void handler(){
    print_time("ALAAAAARMAAAA");
}

int main(){
    int i;
    sigset_t lock; 
    sigset_t unlock;

    sigemptyset(&lock);
    sigemptyset(&unlock);
    
    sigaddset(&lock, SIGUSR1);
    sigaddset(&lock, SIGUSR2);
    sigaddset(&lock, SIGALRM);
    
    sigaddset(&unlock, SIGALRM);
    sigaddset(&unlock, SIGUSR1);
    
    print_time("Alarma para dentro de 20 segundos");
    signal(SIGALRM, handler);
    alarm(20);
    print_time("Bloqueando señales");
    sigprocmask(SIG_BLOCK, &lock, NULL);
    sleep(30);
    print_time("Desbloqueando señales");
    sigprocmask(SIG_UNBLOCK, &unlock, NULL);
    sleep(5);
    print_time("Saliendo");
}