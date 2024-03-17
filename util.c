////////  Mohamed Aboud  /////////
//////////////////////////////////
// This file contains all functions that handles command execution
//////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include "header.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_ARG 10
#define ERROR_MESSAGE "An error has occurred\n"

// executes the command by pasing the arguments and the path
void exec_cmd(char *args[], char path[][MAX_COMMAND_LENGTH], char out_file[]) {
    // path command implementation
    if(strcmp(args[0], "path") == 0) {
        change_path(args, path);
    }
    // cd command implementation
    else if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) == -1) {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        }
    }
    // exit command implementation
    else if (strcmp(args[0], "exit") == 0) {
        if (args[1] == NULL) {
            exit(0);
        } else {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        }
    } else {
        execute(args, path, out_file);
    }
}

//path execution command
void change_path(char *args[], char path[][MAX_COMMAND_LENGTH]) {
    char cwd[MAX_COMMAND_LENGTH];
    for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
        cwd[i] = '\0';
    }
    for (int k = 0; k < MAX_ARG; k++) {
        for (int l = 0; l < MAX_COMMAND_LENGTH; l++) {
            path[k][l] = '\0';
        }
    }

    int pathCounter = 1;
    while(pathCounter < sizeof(*args) && args[pathCounter] != NULL)
    {   // check if path is a directory in root.
        if(exist_in("/", args[pathCounter])) {
            strcpy(cwd, "/");
        }
        else {
            getcwd(cwd, sizeof(cwd));
            strcat(cwd, "/");
        }
        strcat(cwd, args[pathCounter]);
        strcpy(path[pathCounter-1], cwd);
        pathCounter++;
    }
}

//execute command that are not 'cd' 'path' or 'exit'
void execute(char *args[], char path[][MAX_COMMAND_LENGTH], char out_file[]) {
    pid_t pid, wpid;
    int status = 0;
    pid = fork();
    if (pid < 0) {
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        char temp_path[MAX_COMMAND_LENGTH];
        //build the proper path for command if it exists in path
        build_path(args, path, temp_path);
        // if output file is provided, redirect the result of the command
        if(strcmp(out_file, " ") != 0) {
            redirect_output(out_file);
        }
        execv(temp_path, args);
        // wrote this to handle "Exec format failed"
        if(errno == 8) {
            _exit(EXIT_FAILURE);
        }
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        _exit(EXIT_FAILURE);
    }
}

// checks if the folder (arg) is in the specified directory
int exist_in(char* location, char* arg) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(location);
    while((entry = readdir(dir)) != NULL) {
        if (strcmp(arg, entry->d_name) == 0) {
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

void build_path(char *args[], char path[][MAX_COMMAND_LENGTH], char temp_path[]) {
    int pathCounter = 0;
    do {
        strcpy(temp_path, path[pathCounter]);
        strcat(temp_path, "/");
        strcat(temp_path, args[0]);
        pathCounter++;
    } while(access(temp_path, X_OK) != 0 && strcmp(path[pathCounter], "") != 0);
}

void redirect_output(char out_file[]) {
    FILE *file = fopen(out_file, "w");
    if(file == NULL) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        _exit(EXIT_FAILURE);
    }
    if(dup2(fileno(file), fileno(stdout)) == -1) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        _exit(EXIT_FAILURE);
    }
    fclose(file);
}