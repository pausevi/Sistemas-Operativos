#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	struct stat fichero;

	if(lstat(fname,&fichero) == -1){
		perror("Error con lstat");
		exit(EXIT_FAILURE);
	}

	if(S_ISDIR(fichero.st_mode)){ // Si es un directorio...
		return get_size_dir(fname,blocks);
	}else{ // Si no lo es...
		// En caso de que solo sea un fichero, lo calculamos directamente gracias al tipo que nos ha dado lsat (info del archivo)
        *blocks += (fichero.st_blocks * 512) / 1024; // Convert to kilobytes
		return 0;
	}

}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	DIR *d = opendir(dname);
	if(d == NULL){
		perror("Error al abrir el directorio");
		exit(EXIT_FAILURE);
	}
	struct dirent *entrada;
	while((entrada = readdir(d)) != NULL){
		if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue; //Si es el . o .. pasa de ellos
        }
		char *path =("%s/%s",dname,entrada->d_name);
		if(get_size(path,blocks) == -1){
			closedir(d);
			perror("Error con el archivo");
			return -1;
		}
	}
	closedir(d);
	return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	if(argc < 2 ){
			printf("Usage: %s nombreFichero\n",argv[0]);
			exit(EXIT_FAILURE);
		}
	size_t total_blocks = 0, aux=0;

    for (int i = 1; i < argc; i++) {
		aux=total_blocks;
		if (get_size(argv[i], &total_blocks) == -1) {
    		fprintf(stderr, "Error processing file: %s\n", argv[i]);
		} else {
			aux=total_blocks-aux;
    		// Aquí imprimimos el tamaño y el nombre del archivo si no hay errores
    		printf("%luK %s\n", aux, argv[i]);
		}
    }

    printf("%luK total\n", total_blocks);
    return EXIT_SUCCESS;

	return 0;
}
