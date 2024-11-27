#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define BUFFER_SIZE 1024  // Tamaño del bloque para leer/escribir

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	size_t bytesRead;
	unsigned char buffer[BUFFER_SIZE];

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "rb")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte 
	while ((c = getc(file)) != EOF) {*/
	while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0){
		/* Print byte to stdout */
		
		/*ret=putc((unsigned char) c, stdout);
		if (ret==EOF){
			fclose(file);
			err(3,"putc() failed!!");
		}*/
		if (fwrite(buffer, 1, bytesRead, stdout) != bytesRead) {
				fclose(file);
			err(3, "fwrite() failed!!");
		}
	}

	fclose(file);
	return 0;
}
