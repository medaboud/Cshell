////////  Mohamed Aboud  /////////
/////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include "header.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_ARG 10
#define ERROR_MESSAGE "An error has occurred\n"

void interactiveMode() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARG];
    char path[MAX_ARG][MAX_COMMAND_LENGTH];
    //strcpy(path[0], "/home/mohamed/CLionProjects/untitled1/cmake-build-debug/tests/p2a-test");
    //strcpy(path[0], "/home/mohamed/CLionProjects/untitled1/cmake-build-debug/tests");
    strcpy(path[0], "/bin");

    while (1) {
        for (int i = 0; i < MAX_ARG; i++) {
            args[i] = "\0";
        }
        for (int k = 0; k < MAX_COMMAND_LENGTH; k++) {
            command[k] = '\0';
        }
        printf("wish> ");
        if(fgets(command, sizeof(command), stdin) == NULL) {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
            exit(0);
        }
        // Remove newline character from the input
        command[strcspn(command, "\n")] = '\0';
        // Tokenize the input
        tokenize(command, args);
        // Execute the command
        exec_cmd(args, path);
    }
}


void batchMode(char *filename) {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARG];
    FILE *file = fopen(filename, "r");
    char path[MAX_ARG][MAX_COMMAND_LENGTH];
    strcpy(path[0], "/bin");
    for (int i = 0; i < MAX_ARG; i++) {
        args[i] = "\0";
    }
    for (int k = 0; k < MAX_COMMAND_LENGTH; k++) {
        command[k] = '\0';
    }
    if(file == NULL) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(1);
    }
    while (fgets(command, MAX_COMMAND_LENGTH, file) != NULL) {
        // Remove newline character from the input
        command[strcspn(command, "\n")] = '\0';
        // Tokenize the input
        tokenize(command, args);
        // Execute the commandX_I
        exec_cmd(args, path);
    }
    fclose(file);
}
