#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define MAX 100

typedef struct{
    int numHilo;
    char prio;
} tArg;

void *thread_usuario(void *arg) {
	tArg *argus = (tArg*)arg;
	int num = argus->numHilo;
	char prio = argus->prio;

	pthread_t pid = pthread_self();

	printf("Hilo[%d]: Prioridad = %c, con PID = %lu\n",num,prio,pid); 

	free(arg);
	return NULL;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: %s numero_de_hilos\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Obtener el número de hilos desde el argumento
    int num_hilos = atoi(argv[1]); // Convierte el argumento a un número entero
	if(num_hilos > MAX){
		printf("Error: no puedes crear tantos hilos.\n");
		exit(EXIT_FAILURE);
	}
	
	pthread_t hilos[num_hilos];


    // Inicializar los argumentos para los hilos
    for (int i = 0; i < num_hilos; i++) {
		tArg *arg = malloc(sizeof(tArg));  // Asignación de memoria dinámica para cada hilo
        if (arg == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
		arg->numHilo = i;
        arg->prio = (i % 2 == 0) ? 'P' : 'N';  // Asignación de prioridad


		if(pthread_create(&hilos[i],NULL,thread_usuario, (void*)arg) != 0 ){
			perror("Error al crear un hilo");
			free(arg);
			exit(EXIT_FAILURE);
		}
    }

	//Esperamos a que terminen los hilos...
	for(int j=0; j < num_hilos; j++){
		if(pthread_join(hilos[j],NULL)){
			perror("Error en el join");
			exit(EXIT_FAILURE);
		}
		printf("Unido el hilo %d\n", j);
	}
    return 0;
}
