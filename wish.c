////////  Mohamed Aboud  /////////
//////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"
#include "signal.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_ARG 10
#define ERROR_MESSAGE "An error has occurred\n"

void interactiveMode() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARG];
    char path[MAX_ARG][MAX_COMMAND_LENGTH];

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
            exit(EXIT_FAILURE);
        }
        if(strchr(command, '&') != NULL) { // if the input includes a "&"
            command[strcspn(command, "\n")] = '\0'; // Remove newline character from the input
            char *commands[MAX_ARG];
            for (int i = 0; i < MAX_ARG; i++) {
                commands[i] = "\0";
            }
            int command_count = tokenize_for_parallelCmd(command, commands);
            run_parallel_command(command_count, commands, args, path);

        } else {
            process_input(command, args, path);
        }
    }
}

void batchMode(char *filename) {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARG];
    FILE *file = fopen(filename, "r");
    char path[MAX_ARG][MAX_COMMAND_LENGTH];

    strcpy(path[0], "/bin");
    if(file == NULL) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_ARG; i++) {
        args[i] = "\0";
    }
    for (int k = 0; k < MAX_COMMAND_LENGTH; k++) {
        command[k] = '\0';
    }
    while (fgets(command, MAX_COMMAND_LENGTH, file) != NULL) {

        for (int i = 0; i < MAX_ARG; i++) {
            args[i] = "\0";
        }
        if(strchr(command, '&') != NULL) { // if the input includes a "&"
            command[strcspn(command, "\n")] = '\0'; // Remove newline character from the input
            char *commands[MAX_ARG];
            for (int i = 0; i < MAX_ARG; i++) {
                commands[i] = "\0";
            }
            int command_count = tokenize_for_parallelCmd(command, commands);
            run_parallel_command(command_count, commands, args, path);

        } else {
            process_input(command, args, path);
        }
    }
    fclose(file);
}

void process_input(char command[], char* args[], char path[][MAX_COMMAND_LENGTH]) {
    command[strcspn(command, "\n")] = '\0'; // Remove newline character from the input
    deleteTabs(command);    //delete tabs from the user input

    if(strchr(command, '>') != NULL) {      // if the input includes a ">"
        char cmd[MAX_COMMAND_LENGTH];
        strcpy(cmd, command);
        split_command(cmd, args);

        // get the index of the last argument in user input
        int last_arg_position = 0;
        while(last_arg_position < MAX_ARG && strcmp(args[last_arg_position],"\0") != 0) {
            last_arg_position++;
        }
        // check if ">" is leading the command (no command to run) or there are more than one ">"
        // or if ">" is trailing the command (no output file)
        if(strcmp(args[0], ">") == 0 || char_count(args, '>') > 1
            || (last_arg_position != 0 && strcmp(args[last_arg_position - 1], ">") == 0)) {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        }
        // in case the command is a proper redirection
        // checking that there is only one output file is done in "tokenize_for_redirection"
        else {
            char output_file[MAX_COMMAND_LENGTH] = "";
            for (int i = 0; i < MAX_ARG; i++) {
                args[i] = "\0";
            }
            tokenize_for_redirection(command, args, output_file);
            // exit with error if user provide more than one output file
            if(strcmp(output_file, "TOO_MANY_FILES") == 0) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                _exit(EXIT_SUCCESS);
            }
            remove_spaces(output_file);
            exec_cmd(args, path, output_file);
        }
    }
    else {      // if the input does not include a ">" output file will be standard output
        tokenize(command, args);        // Tokenize the input
        if(args[0] != NULL) {
            exec_cmd(args, path, " ");      // Execute the command
        }
    }
}

// this function create multiple child processes to run commanads in parallel
void run_parallel_command(int command_count, char* commands[], char* args[], char path[][MAX_COMMAND_LENGTH]) {
    pid_t processes[command_count];
    for(int i=0; i<command_count; i++)
    {
        pid_t child = fork();
        if(child == 0)
        {
            process_input(commands[i], args, path);
            _exit(0);
        }
        else if (child < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        processes[i] = child;
    }

    for(int i=0; i < command_count; i++)
    {
        pid_t child = processes[i];
        int status = 0;
        waitpid(child, &status, WUNTRACED);
    }
}