#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Es necesario especificar un archivo.\n");
		exit(EXIT_FAILURE);
	}

	int opt, N=0, last = 0;
	while((opt = getopt(argc,argv,"en:")) != -1){
		switch(opt){
			case 'n':
				N = atoi(optarg);
			break;
			case 'e':
				last=1;
			break;
			default:
			printf("Usage: %s [-n N Bytes] [-e] file\n",argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	 // Verificar que se ha pasado un archivo como argumento
    if (optind >= argc) {
        fprintf(stderr, "Error: falta el archivo.\n");
        exit(EXIT_FAILURE);
    }

    // Abrir el archivo en modo de solo lectura
    int fd = open(argv[optind], O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

	  // Obtener el tamaño del archivo
    off_t tam_archivo = lseek(fd, 0, SEEK_END);
    if (tam_archivo == -1) {
        perror("Error al calcular el tamaño del archivo");
        close(fd);
        exit(EXIT_FAILURE);
    }


// Calcular la posición de inicio en función de -n y -e
    off_t offset;
    if (last == 1) {  // Si -e está marcado
        if (N > tam_archivo) {
            fprintf(stderr, "Error: N no puede ser mayor que el tamaño del archivo\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        offset = tam_archivo - N;  // Posicionar N bytes antes del final del archivo
    } else {  // Si -e no está marcado
        if (N > tam_archivo) {
            fprintf(stderr, "Error: N no puede ser mayor que el tamaño del archivo\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        offset = N;  // Posicionar N bytes desde el inicio del archivo
    }

	// Posicionar el marcador en la posición calculada
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("Error al mover el marcador de posición");
        close(fd);
        exit(EXIT_FAILURE);
    }

	  // Leer y escribir cada byte desde el offset hasta el final del archivo
    char buffer;
    ssize_t bytes_leidos;
    while ((bytes_leidos = read(fd, &buffer, 1)) > 0) {
        if (write(STDOUT_FILENO, &buffer, 1) == -1) {
            perror("Error al escribir en la salida estándar");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    // Verificar si hubo error al leer el archivo
    if (bytes_leidos == -1) {
        perror("Error al leer el archivo");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);


	return 0;
}
