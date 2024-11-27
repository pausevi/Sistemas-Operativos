#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t launch_command(char** argv){
   pid_t pid = fork();

    if (pid == -1) {  // Error al crear el proceso
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Proceso hijo
        execvp(argv[0], argv);
        perror("execvp");  // Si execvp falla
        exit(EXIT_FAILURE);
    }
    // Proceso padre: devolver el PID del hijo sin esperar
    return pid;
}


// Si por ejemplo le pasamos ls -l /home , divide el string en un array que contiene a[0] = ls a[1] = -l ...
char **parse_command(const char *cmd, int* argc) {
    // Allocate space for the argv array (initially with space for 10 args)
    size_t argv_size = 10;
    const char *end;
    size_t arg_len; 
    int arg_count = 0;
    const char *start = cmd;
    char **argv = malloc(argv_size * sizeof(char *));

    if (argv == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (*start && isspace(*start)) start++; // Este bucle salta los espacios en blanco

    while (*start) { //Mientras haya caracteres...
        // Reallocate more space if needed
        if (arg_count >= argv_size - 1) {  // Reserve space for the NULL at the end
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        // Find the start of the next argument
        end = start;
        while (*end && !isspace(*end)) end++;

        // Allocate space and copy the argument
        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);

        if (argv[arg_count] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0';  // Null-terminate the argument
        arg_count++;

        // Move to the next argument, skipping spaces
        start = end;
        while (*start && isspace(*start)) start++;
    }

    argv[arg_count] = NULL; // Null-terminate the array

    (*argc)=arg_count; // Return argc

    return argv;
}


int main(int argc, char *argv[]) {
    char **cmd_argv;
    int cmd_argc;
    int i, opt, s=0, b=0;
    char *archivo;

    pid_t pid_array[100]; // Para almacenar PIDs de hasta 100 comandos
    int command_number[100]; // Asocia el número de comando al PID
    int total_commands = 0; // Lleva la cuenta de los comandos lanzados
    int commandIndex=0;


    if (argc <= 2) {
        fprintf(stderr, "Usage: %s \"command\"\n", argv[0]);
        return EXIT_FAILURE;
    }
    while ((opt = getopt(argc, argv, "x:s:b")) != -1)
    {
        switch (opt) {
               case 'x':
                    // Parsear el comando
                    cmd_argv = parse_command(optarg, &cmd_argc);

                    // Lanzar el comando
                     pid_t pid = launch_command(cmd_argv);

                     // Esperar a que el proceso hijo termine
                    int status;
                    waitpid(pid, &status, 0);
                    printf("Command terminated (pid: %d, status: %d)\n", pid, WEXITSTATUS(status));

                    // Liberar memoria
                    for (int i = 0; i < cmd_argc; i++) {
                        free(cmd_argv[i]);
                    }
                    free(cmd_argv);

                break;
                case 's':
                  s = 1;
                 archivo = optarg;
                break;
                case 'b':
                 b=1;
                break;
               default: /* '?' */
                   fprintf(stderr, "Usage: %s \"command\"\n",argv[0]);
                   exit(EXIT_FAILURE);
               }
    }

    if(s==1){
         char line[256];
        FILE *fp = fopen(archivo, "r");
        if(fp == NULL){
            perror("FILE"); 
            exit(EXIT_FAILURE);
        }
        i=0;
        while(fgets(line,256,fp) != NULL){
             if (line[strlen(line) - 1] == '\n') {
                 line[strlen(line) - 1] = '\0';  // Eliminar salto de línea
            }
            cmd_argv = parse_command(line, &cmd_argc);
            // Lanzar el comando
            printf("@@ Running Command #%d: %s\n",i,line);
            pid_t pid = launch_command(cmd_argv);
           // Esperar a que el proceso hijo termine
            if(b == 0){
                int status;
                waitpid(pid, &status, 0);
                printf("Command terminated (pid: %d, status: %d)\n\n", pid, WEXITSTATUS(status));
            }
            else{
                 pid_array[commandIndex] = pid;
                    command_number[commandIndex] = commandIndex;
                    commandIndex++;
                    total_commands++;
            }
            // Liberar memoria
            for (int i = 0; i < cmd_argc; i++) {
                free(cmd_argv[i]);
            }
            free(cmd_argv);
            i++;
        }

        if (b == 1) {
            int status;
            pid_t pid;
            int completed = 0;

            // Espera a que todos los procesos terminen, mostrando cada mensaje en cuanto finalicen
            while (completed < total_commands) {
                pid = waitpid(-1, &status, WNOHANG); // Usar -1 para esperar cualquier proceso hijo
                if (pid > 0) {
                    // Buscar el índice del comando en pid_array
                    for (int i = 0; i < total_commands; i++) {
                        if (pid_array[i] == pid) {
                            printf("@@ Command #%d terminated (pid: %d, status: %d)\n", 
                                command_number[i], pid, WEXITSTATUS(status));
                            completed++; // Incrementa el contador de procesos terminados
                            break;
                        }
                    }
                } else if (pid == 0) {
                    // Ningún proceso ha terminado, espera un momento
                    usleep(10000); // Pausa corta para evitar sobrecargar la CPU
                }
            }
}

 
    }

    return EXIT_SUCCESS;
}