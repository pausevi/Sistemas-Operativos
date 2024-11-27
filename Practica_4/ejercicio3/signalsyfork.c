#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 8
/*programa que temporiza la ejecución de un proceso hijo */


pid_t pid_hijo; // PID del proceso hijo, usado por el manejador de señales

// Manejador de SIGALRM: mata al proceso hijo
void manejador_alarma(int sig) {
    if (pid_hijo > 0) {
        printf("Tiempo agotado. Enviando SIGKILL al proceso hijo con PID %d\n", pid_hijo);
        kill(pid_hijo, SIGKILL);
    }
}

// Configura el padre para ignorar SIGINT
void ignorar_sigint(int sig) {
    printf("Señal SIGINT recibida, pero ignorada.\n");
}

int main(int argc, char **argv)
{
	if(argc < 2){
		printf("Usage: %s comando argumentos\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	char* comando = argv[1];
	char* argumentos[MAX_ARGS];
	argumentos[0] = comando;
//Almacenamos en argumentos los argumentos del comando en caso de que haya
	if(argc > 2){
		for(int i=2; i<argc;i++){
			argumentos[i-1] = argv[i];
		}
	}
	argumentos[argc - 1] = NULL; // Terminar la lista de argumentos



 // Crear el hijo
    pid_hijo = fork();

    if (pid_hijo == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    } else if (pid_hijo == 0) {
        // Código del hijo: ejecutar el programa indicado
        printf("Ejecutando '%s' con PID %d\n", comando, getpid());
        execvp(comando, argumentos);
        perror("Error al ejecutar execvp"); // Solo se ejecuta si execvp falla
        exit(EXIT_FAILURE);
    } else {
        // Código del padre
        // Configurar manejador para SIGALRM
        struct sigaction sa; //se usa para especificar cómo manejar una señal en un programa.
        sa.sa_handler = manejador_alarma; //Puntero a la función que maneja la señal
        sa.sa_flags = 0; //Opciones adicionales para el comportamiento del manejador
        //sa_mask // Conjunto de señales bloqueadas durante el manejador
        sigemptyset(&sa.sa_mask);// Inicializa conjunto vacio de señales bloqueadas
        sigaction(SIGALRM, &sa, NULL); //Configura el manejador para SIGALARM

        // Ignorar SIGINT en el padre
        signal(SIGINT, ignorar_sigint);

        // Programar la alarma para 5 segundos que hace al proceso recibir la señal SIGALARM
        alarm(5);

        // Esperar a que termine el hijo
        int status;
        waitpid(pid_hijo, &status, 0);

        // Evaluar cómo terminó el hijo
        if (WIFEXITED(status)) { //Esta macro verifica si un proceso hijo terminó normalmente
            printf("El hijo terminó normalmente con código de salida %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) { //Esta macro verifica si un proceso hijo terminó debido a una señal.
            printf("El hijo terminó debido a una señal: %s\n", strsignal(WTERMSIG(status)));
        }

    }

	return 0;
}
