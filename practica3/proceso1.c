#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#define FILEKEY "/bin/cat" /*Util para ftok */
#define KEY 1300
#define MAXBUF 10

int main (int argc, char *argv[]) {
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
     
     /* We create the shared memory */
     id_zone = shmget (key, sizeof(int)*MAXBUF, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
     
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
    
     for (i = 0; i < MAXBUF; i++)
       buffer[i] = i;
     
     /* The daemon executes until press some character */
    
     c = getchar();
     
     /* Free the shared memory */
     shmdt ((char *)buffer);
     shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
     
     return 0;
}

