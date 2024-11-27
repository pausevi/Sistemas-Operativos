#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[])
{
	FILE *fp= NULL;
	

	if (argc < 3) {
		fprintf(stderr,"Usage: %s <file_name> <word1> <word2> ...\n",argv[0]);
		exit(1);
	}
	printf("Abriendo Archivo...\n");

	/* Open file */
	fp = fopen(argv[1], "wb");

	if (fp == NULL){
		err(2,"The input file %s could not be opened",argv[1]);	
		return 0;
	}

	for(int i = 2; i < argc; i++){
		size_t length = strlen(argv[i]) + 1; //+1 para incluir \0
		if(fwrite(argv[i],1,length,fp) != length){
			fclose(fp);
			err(3, "Error al escribir en el archivo");
		}	}
	fclose(fp);
    printf("Cadenas escritas correctamente en %s\n", argv[1]);
	return 0;
}
