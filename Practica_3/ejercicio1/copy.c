#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

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

int main(int argc, char *argv[])
{
	if(argc < 3){
		printf("ERROR! Usage: ./nombrePrograma ficheroOrigen ficheroDestino.\n");
		return 0;
	}
	int fdo = open(argv[1], O_RDONLY);

	if(fdo == -1){
		perror("Error al abrir el fichero origen.");
		return 1;
	}

	int fdd = open(argv[2], O_WRONLY | O_CREAT | O_CREAT, 0644);

	if(fdd == -1){
		perror("Error al abrir el fichero destino.");
		return 1;
	}

	copy(fdo,fdd);

	close(fdo);
	close(fdd);
	return 0;
}
