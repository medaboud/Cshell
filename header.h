void interactiveMode();
void batchMode(char* filename);
void tokenize(char command[], char* args[]);
char * exec_cmd(char *args[], char *path);
void change_path(char *args[], char path[]);
void execute(char *args[], char path[]);
int exist_in(char* location, char* arg);
