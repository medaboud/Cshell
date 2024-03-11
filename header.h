#define MAX_COMMAND_LENGTH 100

void interactiveMode();
void batchMode(char* filename);
void tokenize(char command[], char* args[]);
void exec_cmd(char *args[], char path[][MAX_COMMAND_LENGTH]);
void change_path(char *args[], char path[][MAX_COMMAND_LENGTH]);
void execute(char *args[], char path[][MAX_COMMAND_LENGTH]);
int exist_in(char* location, char* arg);
