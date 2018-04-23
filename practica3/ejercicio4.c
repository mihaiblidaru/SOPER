#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void *generar_fichero(void* filename);
void *modificar_fichero(void* filename);

#define TAM_LIMITE_INFERIOR 1000
#define TAM_LIMITE_SUPERIOR 2000
#define TAM_INTERVALO TAM_LIMITE_SUPERIOR - TAM_LIMITE_INFERIOR

#define NUM_LIMITE_INFERIOR 100
#define NUM_LIMITE_SUPERIOR 1000
#define NUM_INTERVALO NUM_LIMITE_SUPERIOR - NUM_LIMITE_INFERIOR

int main(int argc, char** argv) {
	pthread_t thread;
	int thread_status;

	if(argc < 2){
		fprintf(stderr, "%s <fichero>", argv[0]);
		exit(EXIT_FAILURE);
	}

	pthread_create(&thread, NULL, generar_fichero, argv[1]);
	pthread_join(thread, (void**)&thread_status);
	printf("Thread 1 exited with status %d\n", thread_status);
	if(thread_status != 0){
		fprintf(stderr, "Job aborted");
		exit(EXIT_FAILURE);
	}

	pthread_create(&thread, NULL, modificar_fichero, argv[1]);;
	pthread_join(thread, (void**)&thread_status);
	printf("Thread 2 exited with status %d\n", thread_status);

	return EXIT_SUCCESS;
}


/**
 * Crea y escribe en un fichero un número n de números aleatorios donde n es un
 * número aleatorio del 1000 al 2000 y los números aleatorios de este fichero
 * tendrán un valor entre el 100 y el 1000 separados por comas.
 */
void *generar_fichero(void* e){
	char* filename = (char*) e;
	int num_of_nums, i;
	FILE* fp = NULL;

	if(!(fp = fopen(filename, "w"))){
		perror(filename);
		pthread_exit((void*)1);
	}

	srand(time(NULL));

	fprintf(fp, "%d", (rand() % NUM_INTERVALO) + NUM_LIMITE_INFERIOR);

	num_of_nums = (rand() % TAM_INTERVALO) + TAM_LIMITE_INFERIOR - 1;

	for(i = 0; i < num_of_nums; i++){
		fprintf(fp, ",%d", (rand() % NUM_INTERVALO) + NUM_LIMITE_INFERIOR);
	}

	fclose(fp);

	pthread_exit(NULL);
}



void *modificar_fichero(void* e){
	char* buf=NULL, *filename = (char*) e;
	struct stat finfo;
	int i, fd;

	if((fd = open(filename, O_RDWR)) == -1){
		perror("open");
		pthread_exit((void*)1);
	}

	if(fstat(fd, &finfo) == 1){
		perror("fstat");
		close(fd);
		pthread_exit((void*)1);
	}

	buf = mmap(NULL, finfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(buf == MAP_FAILED){
		perror("mmap");
		close(fd);
		pthread_exit((void*)1);
	}

	/* cambia comas por espacios */
	for(i=0; i < finfo.st_size; i++){
		if(buf[i] == ',')
			buf[i] = ' ';
	}

	if(munmap(buf, finfo.st_size) == -1){
		perror("munmap");
		close(fd);
		pthread_exit((void*)1);
	}

	close(fd);
	pthread_exit(NULL);
}
