#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "header.h"

int main(int argc, char *argv[]) {

    if (argc == 1) {
        // No command line arguments, run in interactive mode
        interactiveMode();
    } else if (argc == 2) {
        // One command line argument, assume it's the batch file name
        batchMode(argv[1]);
    } else {
        // Incorrect number of arguments
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
        return 1;
    }

    return 0;
}
