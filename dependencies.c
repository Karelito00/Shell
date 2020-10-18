#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#define Red "\x1b[31m"
#define Yellow "\x1b[33m"
#define Blue "\x1b[34m"
#define Cyan "\x1b[36m"
#define RESET "\x1b[0m"
#define SIZE 100000
#define db printf(Red "debug" RESET "\n")
#define ERRORC(cad) printf(Red "Error to execute \'%s\'" RESET "\n", (cad))

int min(int a, int b){
    if(a < b) return a;
    return b;
}

int max(int a, int b){
    if(a > b) return a;
    return b;
}

typedef struct Command{
    char *name;
    char **args;
    int mod1; //for >
    int mod2; //for >>
    int mod3; //for <
    char *input, *output;
    int length_args;
    int error; //Report errors 
    char *_if;
    char *_then;
    char *_else;
}Command;

typedef struct Commands_Split_Pipes{ 
    Command *command_by_pipes;
    int length_pipes;
}Commands_Split_Pipes;

typedef struct Commands_Split_Cond{ 
    Commands_Split_Pipes *command_by_cond;
    int *Type_Cond;
    int length_cond;
}Commands_Split_Cond;

typedef struct Split_Lines_Dotcomma{ 
    Commands_Split_Cond *commands_lines;
    int length_lines_splits;
}Split_Lines_Dotcomma;

void Constructor_Command(Command *_command){
    _command->name = malloc(sizeof(char) * SIZE);
    _command->args = malloc(SIZE);
    _command->length_args = 0;
    _command->mod1 = _command->mod2 = _command->mod3 = 0;
    _command->input = malloc(sizeof(char) * SIZE);
    _command->output = malloc(sizeof(char) * SIZE);
    _command->error = 0;
    _command->_else = NULL;
    _command->_if = NULL;
    _command->_then = NULL;
}

void Constructor_Commands_Split_Pipes(Commands_Split_Pipes *input_process){
    input_process->command_by_pipes = malloc(SIZE);
    input_process->length_pipes = 0;
}

void Constructor_Commands_Split_Cond(Commands_Split_Cond *split_cond){
    split_cond->command_by_cond = malloc(SIZE);
    split_cond->length_cond = 0;
    split_cond->Type_Cond = malloc(sizeof(int) * SIZE);
}

void Constructor_Split_Lines_Dotcomma(Split_Lines_Dotcomma *lines){
    lines->commands_lines = malloc(SIZE);
    lines->length_lines_splits = 0;
}

typedef struct Global_Var{
    char *name;
    char *value;
}Global_Var;

typedef struct Linked_List{
    Global_Var *arr_var;
    int length_vars;
}Linked_List;

void Constructor_Global_Var(Global_Var *var){
    var->name = malloc(SIZE);
    var->value = malloc(SIZE);
}

void Constructor_Linked_List(Linked_List *vars){
    vars->arr_var = malloc(SIZE);
    vars->length_vars = 0;
}

char special_characters[] = {' ', '|', '>', '<', '&', ';'};

int Is_Special(char x){
    for(int i = 0; i < strlen(special_characters); i++){
        if(special_characters[i] == x)
            return 1;
    }
    return 0;
}

void Copy_To(char a[], char b[]){ //Copy from a to b
    for(int i = 0; i < strlen(a); i++)
        b[i] = a[i];
    b[strlen(a) + 1] = 0;
}

char kind_quotes[] = {'\'', '\"', '`'};

int is_a_quote(char x){
    for(int i = 0; i < strlen(kind_quotes); i++)
        if(x == kind_quotes[i]) return 1;
    return 0;
}

