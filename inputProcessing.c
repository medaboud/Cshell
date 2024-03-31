////////  Mohamed Aboud  /////////
//////////////////////////////////
// This file contains all functions that handles user input
//////////////////////////////////

#include <string.h>
#include <ctype.h>
#include "header.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_ARG 10
#define ERROR_MESSAGE "An error has occurred\n"

// tokenize the line read from file or stdin
void tokenize(char command[], char* args[]) {
    int arg_count = 0;
    char* token = strtok(command, " ");

    while (token != NULL && arg_count < MAX_ARG -1) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;
}

// get commands to be run in parallel from the global command the user provided
void tokenize_for_parallelCmd(char command[], char* args[]) {
    int arg_count = 0;
    char* token = strtok(command, "&");

    while (token != NULL && arg_count < MAX_ARG -1) {
        args[arg_count++] = token;
        token = strtok(NULL, "&");
    }
    args[arg_count] = NULL;

}

// get arguments for command and output file for redirection
void tokenize_for_redirection(char command[], char* args[], char file[]) {
    int arg_count = 0;
    char* token = strtok(command, ">");

    while (token != NULL && arg_count < MAX_ARG -1) {
        args[arg_count++] = token;
        token = strtok(NULL, ">");
    }
    args[arg_count] = NULL;
    // get the output file from user input
    strcpy(file,args[arg_count - 1]);
    if(count_words(file) > 1) {
        strcpy(file,"TOO_MANY_FILES");
    }
    // get the command to be executed from user input
    tokenize(args[0], args);
}

// splits a command into multiple arguments
void split_command(char command[], char* args[]) {
    int arg_count = 0;
    char* token = strtok(command, " ");

    while (token != NULL && arg_count < MAX_ARG -1) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
}

// return the number of occurrences of a certain character
int char_count(char** arr, char ch) {
    int count = 0;
    for(int i = 0; i < MAX_ARG; i++) {
        const char* str = arr[i];
        if (*str == ch){
            count++;
        }
    }
    return count;
}

// return the number of words in a string
int count_words(const char *str) {
    int count = 0;
    int in_word = 0; // Flag to track whether we are inside a word

    while (*str != '\0') {
        // If the current character is a whitespace and we were in a word before
        // then increment the word count and reset the flag
        if (isspace(*str) && in_word) {
            count++;
            in_word = 0;
        }
        // If the current character is not a whitespace, set the in_word flag
        else if (!isspace(*str)) {
            in_word = 1;
        }
        str++; // Move to the next character
    }
    // If the last character is not a whitespace and we were in a word before
    // then increment the word count to include the last word
    if (in_word) {
        count++;
    }
    return count;
}

// delete tabs from a command
void deleteTabs(char *str) {
    int i, j;
    int len;
    len = strlen(str);

    for (i = 0, j = 0; i < len; i++) {
        if (str[i] != '\t') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0'; // Null terminate the string at the new length
}