#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


#define TAM_BLOQUE  512

void copy(int fdo, int fdd)
{
char buffer[TAM_BLOQUE];
ssize_t bytes;

while((bytes = read(fdo,buffer,TAM_BLOQUE)) > 0){
	ssize_t escritos = write(fdd, buffer, bytes);
	if(escritos == -1){
		perror("Error al escribir en el fichero destino.");
		exit(1);
	}
}

}

void copy_regular(char* origen, char*destino){
	int fdo = open(origen, O_RDONLY);

	if(fdo == -1){
		perror("Error al abrir el fichero origen.");
		exit(1);
	}

	int fdd = open(destino, O_WRONLY | O_CREAT , 0644);

	if(fdd == -1){
		perror("Error al abrir el fichero destino.");
		exit(1);
	}
	copy(fdo,fdd);

	close(fdo);
	close(fdd);
}
void copy_link(char* origen, char*destino){
	struct stat archivo;

	if(lstat(origen, &archivo) == -1){
		perror("Error con lstat");
		exit(EXIT_FAILURE);
	}	
	char *buff = malloc(archivo.st_size+1);
	if(buff == -1){
		perror("Error al reservar memoria.");
		exit(EXIT_FAILURE);
	}

	 ssize_t link = readlink(origen, buff, archivo.st_size);
    if (link == -1) {
        perror("Error al leer el enlace simbólico.");
        free(buff);
        exit(EXIT_FAILURE);
    }
    buff[link] = '\0';  // Añadir el carácter null al final

    // Ahora `buff` contiene la ruta a la que apunta el enlace simbólico original.
    if (symlink(buff, destino) == -1) {  // Argumentos en orden correcto
        perror("Error al crear el enlace simbólico.");
        free(buff);
        exit(EXIT_FAILURE);
    }

}

int main(int argc, char *argv[])
{
	if(argc < 3){
		printf("ERROR! Usage: ./nombrePrograma ficheroOrigen ficheroDestino.\n");
		return 0;
	}

	struct stat archivo;

	if(lstat(argv[1], &archivo) == -1){
		perror("Error con lstat");
		exit(EXIT_FAILURE);
	}	
	 if (S_ISLNK(archivo.st_mode)) {  // Verifica si es un enlace simbólico
	 printf("Enlace simbolico.\n");
        copy_link(argv[1], argv[2]);
    } else if (S_ISREG(archivo.st_mode)) {  // Verifica si es un archivo regular
	printf("Enlace regular.\n");
        copy_regular(argv[1], argv[2]);
    } else {
        fprintf(stderr, "Error: archivo no válido.\n");
        exit(EXIT_FAILURE);
    }
	return 0;
}
