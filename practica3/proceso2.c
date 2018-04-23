#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h> /* shm* */
#define FILEKEY "/bin/cat"
#define KEY 1300
#define MAXBUF 10



int main () {
    int *buffer; /* shared buffer */
    int key, id_zone;
    int i;
    char c;
    
    /* Key to shared memory */
    key = ftok(FILEKEY, KEY);
    if (key == -1) {
        fprintf (stderr, "Error with key \n");
        return -1;
    }
    
    /* we create the shared memory */
    id_zone = shmget (key, sizeof(int)*MAXBUF, IPC_CREAT | IPC_EXCL|SHM_R | SHM_W);
    if (id_zone == -1) {
        fprintf (stderr, "Error with id_zone \n");
        return -1;
    }
    printf ("ID zone shared memory: %i\n", id_zone);
    /* we declared to zone to share */
    buffer = shmat (id_zone, (char *)0, 0);
    if (buffer == NULL) {
        fprintf (stderr, "Error reserve shared memory \n");
        return -1;
    }
    printf ("Pointer buffer shared memory: %p\n", buffer);
    
        /* Write the values of shared memory */
    
    for (i = 0; i < MAXBUF; i++)
        printf ("%i\n", buffer[i]);
    
    
    
    //en teoria esto no estaba     
    shmdt ((char *)buffer);
    shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
    
        
    return 0;
}