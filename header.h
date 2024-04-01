#include <fcntl.h>

#define MAX_COMMAND_LENGTH 100

void interactiveMode();
void batchMode(char* filename);
int tokenize_for_parallelCmd(char command[], char* args[]);
void process_input(char command[], char* args[], char path[][MAX_COMMAND_LENGTH]);

void exec_cmd(char *args[], char path[][MAX_COMMAND_LENGTH], char out_file[]);
void change_path(char *args[], char path[][MAX_COMMAND_LENGTH]);
void execute(char *args[], char path[][MAX_COMMAND_LENGTH], char out_file[]);
int exist_in(char* location, char* arg);
void build_path(char *args[], char path[][MAX_COMMAND_LENGTH], char temp_path[]);
void redirect_output(char out_file[]);
void run_parallel_command(int command_count, char* commands[], char* args[], char path[][MAX_COMMAND_LENGTH]);

void tokenize(char command[], char* args[]);
void tokenize_for_redirection(char command[], char* args[], char file[]);
void split_command(char command[], char* args[]);
int char_count(char** str, char ch);
int count_words(const char *str);
void deleteTabs(char *str);
void remove_spaces(char *str);

