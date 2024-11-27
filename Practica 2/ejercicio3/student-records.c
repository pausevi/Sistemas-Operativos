#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"


/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

int print_text_file(char *path)
{
	/* To be completed (part A) */
	FILE * fp;
	//student_t student;
	char line[MAX_PASSWD_LINE+1];


//  Abrimos el fichero
	if((fp=fopen(path,"r"))==NULL){
		fprintf(stderr, "file could not be opened: ");
		perror(NULL);
		return EXIT_FAILURE;
	}

// Leer el número de registros (primera línea)
    if (fgets(line, sizeof(line), fp) == NULL) {
        fprintf(stderr, "Error al leer el número de registros\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    int num_records = atoi(line);
	int i=0;

	while (fgets(line, sizeof(line), fp) != NULL && i < num_records) {
        char *token = NULL;
		char *line_copy = line;

		fprintf(stdout,"[Entry #%d]\n",i);
		 
        token = strsep(&line_copy, ":");
        //student.student_id = atoi(token);
		fprintf(stdout,"\tstudent_id=%d\n",atoi(token));

        token = strsep(&line_copy, ":");
       // strncpy(student.NIF, token, sizeof(student.NIF) - 1);
	   fprintf(stdout,"\tNIF=%s\n",token);

        token = strsep(&line_copy, ":");
       // strncpy(student.first_name, token, sizeof(student.first_name) - 1);
	   fprintf(stdout,"\tfirst_name=%s\n",token);

        token = strsep(&line_copy, ":");
        //strncpy(student.last_name, token, sizeof(student.last_name) - 1);
		fprintf(stdout,"\tlast_name=%s\n",token);		
		
		i++;
	}

	return 0;
}

int print_binary_file(char *path) { //NO FUNCIONA
    FILE *fp;
    

    // Abrimos el fichero en modo binario
    if ((fp = fopen(path, "rb")) == NULL) {
        fprintf(stderr, "file could not be opened: ");
        perror(NULL);
        return EXIT_FAILURE;
    }

    // Leer el número de registros
    int num_records;
    if(fread(&num_records, sizeof(int), 1, fp) != 1){
        perror("Error al leer el número de registros");
        fclose(file);
        return 1;
    }

    for (int i = 0; i < num_records; i++) {
        student_t student;
        // Leer student_id
        fread(&student.student_id, sizeof(int), 1, fp);
        // Leer NIF
        fread(student.NIF, sizeof(student.NIF), 1, fp);

        // Leer first_name
        char buffer[MAX_PASSWD_LINE];  // Asegúrate de que el tamaño es suficiente
        fread(buffer, sizeof(char), sizeof(buffer), fp);
        student.first_name = strdup(buffer);  // Asegúrate de hacer una copia

        // Leer last_name
        fread(buffer, sizeof(char), sizeof(buffer), fp);
        student.last_name = strdup(buffer);  // Asegúrate de hacer una copia

        // Imprimir el registro del estudiante
        fprintf(stdout, "[Entry #%d]\n", i);
        fprintf(stdout, "\tstudent_id=%d\n", student.student_id);
        fprintf(stdout, "\tNIF=%s\n", student.NIF);
        fprintf(stdout, "\tfirst_name=%s\n", student.first_name);
        fprintf(stdout, "\tlast_name=%s\n", student.last_name);

        // Liberar memoria
        free(student.first_name);
        free(student.last_name);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}




int write_binary_file(char *input_file, char *output_file)
{
	/* To be completed  (part B) */
	 FILE *fp_in, *fp_out;
    student_t student;
    char line[MAXLEN_LINE_FILE + 1];

    // Abre el archivo de texto para lectura y el archivo binario para escritura
    if ((fp_in = fopen(input_file, "r")) == NULL) {
        fprintf(stderr, "Error abriendo archivo de entrada\n");
        return EXIT_FAILURE;
    }
    if ((fp_out = fopen(output_file, "wb")) == NULL) {
        fprintf(stderr, "Error abriendo archivo de salida\n");
        fclose(fp_in);
		fclose(fp_out);
        return EXIT_FAILURE;
    }

    // Leer número de registros
    if (fgets(line, sizeof(line), fp_in) == NULL) {
        fprintf(stderr, "Error al leer número de registros\n");
        fclose(fp_in);
        fclose(fp_out);
        return EXIT_FAILURE;
    }
    int num_records = atoi(line);

	 // Escribe el número de registros en el archivo binario
    fwrite(&num_records, sizeof(int), 1, fp_out);

    // Leer cada registro y escribirlo en formato binario
    for (int i = 0; i < num_records && fgets(line, sizeof(line), fp_in) != NULL; i++) {
        char *line_copy = strdup(line);

		char *token = strsep(&line_copy, ":");
        student.student_id = atoi(token);

        token = strsep(&line_copy, ":");
        strncpy(student.NIF, token, sizeof(student.NIF) - 1);

        token = strsep(&line_copy, ":");
        student.first_name = strdup(token);

        token = strsep(&line_copy, ":");
        student.last_name = strdup(token);

        // Escribir el registro en el archivo binario
        fwrite(&student.student_id, sizeof(int), 1, fp_out);
        fwrite(student.NIF, sizeof(student.NIF), 1, fp_out);
        fwrite(student.first_name, strlen(student.first_name) + 1, 1, fp_out);
        fwrite(student.last_name, strlen(student.last_name), 1, fp_out);

        free(student.first_name);
        free(student.last_name);
    }

    fclose(fp_in);
    fclose(fp_out);
    printf("%d student records written successfully to binary file %s\n", num_records, output_file);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int ret_code, opt;
	struct options options;

	/* Initialize default values for options */
	options.input_file = NULL;
	options.output_file = NULL;
	options.action = NONE_ACT;
	ret_code = 0;

	/* Parse command-line options (incomplete code!) */
	while ((opt = getopt(argc, argv, "hi:po:b")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h | -p | -i file | -c <output_file> ]\n", argv[0]);
			exit(EXIT_SUCCESS);
		case 'i':
			options.input_file = optarg;
			break;
		case 'p':
			options.action=PRINT_TEXT_ACT;
			break;
		case 'o':
			options.action=WRITE_BINARY_ACT;
			options.output_file = optarg;
			break;
		case 'b':
			options.action=PRINT_BINARY_ACT;
			break;

		default:
			exit(EXIT_FAILURE);
		}
	}

	if (options.input_file == NULL)
	{
		fprintf(stderr, "Must specify one record file as an argument of -i\n");
		exit(EXIT_FAILURE);
	}

	switch (options.action)
	{
	case NONE_ACT:
		fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
		ret_code = EXIT_FAILURE;
		break;
	case PRINT_TEXT_ACT:
		/* Part A */
		ret_code = print_text_file(options.input_file);
		break;
	case WRITE_BINARY_ACT:
		/* Part B */
		ret_code = write_binary_file(options.input_file, options.output_file);
		break;
	case PRINT_BINARY_ACT:
		/* Part C */
		ret_code = print_binary_file(options.input_file);
		break;
	default:
		break;
	}
	exit(ret_code);
}
