#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


char* signames[] = {"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS",
                    "SIGFPE","SIGKILL","SIGUSR1","SIGSEGV","SIGUSR2","SIGPIPE","SIGALRM","SIGTERM",
                    "SIGSTKFLT","SIGCHLD","SIGCONT","SIGSTOP","SIGTSTP","SIGTTIN","SIGTTOU","SIGURG","SIGXCPU",
                    "SIGXFSZ","SIGVTALRM","SIGPROF","SIGWINCH","SIGIO","SIGPWR"};
                    

void blocked_signals(){
    int i = 0;
    
    sigset_t blocked_signals_set;
    sigfillset (&blocked_signals_set);
    sigset_t mySet;
    sigemptyset (&mySet);
    
    sigprocmask(SIG_BLOCK, &mySet, &blocked_signals_set);
 
    for(i=0; i < 29; i++){
        if(sigismember(&blocked_signals_set, i+1)){
            printf("%s\n", signames[i]);
        }
        
    }
}

void handler(){
    
}

int main(){
    
    
    signal(SIGUSR2, handler);
    sigset_t mySet;
    int a = 1, b =2, c =3;
    
    blocked_signals();
    printf("PID: %d\n", getpid());
    sleep(3000);
    a = b + c;
    sigemptyset (&mySet);
    a = b + c;
    sigfillset (&mySet);
    a = b + c;
    return 0;
}