#include "parse.c"

/////////////////////

void execute_command(Command *command){
    int mod1 = 0, mod2 = 0, mod3 = 0;
    char *output, *input;
    for(int i = 0; i < command->length_args; i++){
        if(strcmp(command->args[i], ">") == 0){
            mod1 = 1;
            output = command->args[i + 1];
            i++;
        }
        else if(strcmp(command->args[i], ">>") == 0){
            mod2 = 1;
            output = command->args[i + 1];
            i++;
        }
        else if(strcmp(command->args[i], "<") == 0){
            mod3 = 1;
            input = command->args[i + 1];
            i++;
        }
    }
    pid_t pid = fork();
    if(pid == 0){
        if(mod1 == 1){
            int fd = creat(output, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if(mod2 == 1){
            int fd = open(output, O_CREAT | O_WRONLY | O_APPEND, 777);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if(mod3 == 1){
            int fd = open(input, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        char *command_opt = command->command;
        char **args_opt = malloc(command->length_args + 1);
        args_opt[0] = command_opt;
        for(int i = 0; i < command->length_args; i++){
            args_opt[i + 1] = command->args[i];
        }
        args_opt[command->length_args + 1] = NULL;
        int cap = execvp(command_opt, args_opt);
        if(cap < 0) printf(Red "Error to execute \'%s\'\n" RESET, args_opt[0]);
        exit(0);
    }
    else{
    	wait(NULL);
    }
}


int execute(Command *line){
    if(strcmp(line->command, "history") == 0){
        show_history();
        return 0;
    }
    if(strcmp(line->command, "help") == 0){
        help(line);
        return 0;
    }
    /*if(strcmp(line->command, "jobs") == 0){

        return 0;
    }*/
    if(strcmp(line->command, "exit") == 0)
        return 1;
    if(strcmp(line->command, "cd") == 0){
        int success = chdir(line->args[0]);
        if(success != 0)
            printf("No such file or directory\n");
    }
    else{
        execute_command(line);
    }
    return 0;
}

int main(){
    Initial(); //Create file for history
    while(1){
        printf(Yellow "my-prompt " RESET "$ ");
        //initialize
        char *line_input_temp = malloc(SIZE * sizeof(char));
        line_input_temp[0] = 0;
        fgets(line_input_temp, SIZE, stdin);
        char *line_input = Delete_Spaces_Of_The_begin(line_input_temp);
        if(line_input[0] == '\n') continue; //if first character is endline('\n'), then we will to do another cycle.
        Commands_Split_Pipes input_process = Parse_Input(line_input);
        if(strcmp(input_process._command[0].command, "again") == 0){
            char *get_line = malloc(SIZE);
            int proof = Again_Command(&input_process._command[0], get_line);
            if(proof == 0){
                printf(Red "Error to execute command again\n" RESET);
                continue;
            }
            line_input = get_line;
            input_process = Parse_Input(get_line);
        }
        if(line_input[0] != ' ') 
            Save_History(line_input);
        
        if(execute(&input_process._command[0]))
            return 0;
    }

    return 0;
}
