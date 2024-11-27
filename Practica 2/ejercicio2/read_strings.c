#include <stdio.h>
#include <stdlib.h>
#include <err.h>

/** Carga una cadena terminada en '\0' desde un archivo */
char *loadstr(FILE *file)
{
    int length = 0;
    int c;

    // Leer carácter por carácter hasta encontrar el terminador '\0' o el final del archivo
    while ((c = fgetc(file)) != EOF) {
        length++;
        if (c == '\0') {
            break;
        }
    }

    if (c == EOF) {
        return NULL; // Fin del archivo
    }

    // Mover el puntero de posición del archivo al inicio de la cadena
    fseek(file, -length, SEEK_CUR);

    // Reservar memoria para la cadena y leerla completa
    char *str = (char *)malloc(length * sizeof(char));
    if (str == NULL) {
        perror("Error al reservar memoria");
        return NULL;
    }

    if (fread(str, 1, length, file) != length) {
        free(str);
        return NULL; // Error de lectura
    }

    return str;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_archivo>\n", argv[0]);
        exit(1);
    }

    // Abrir el archivo en modo binario de lectura
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        exit(2);
    }

    // Leer y mostrar cada cadena hasta el final del archivo
    char *str;
    while ((str = loadstr(file)) != NULL) {
        printf("%s\n", str);
        free(str); // Liberar la memoria de la cadena después de imprimirla
    }

    fclose(file);
    return 0;
}
