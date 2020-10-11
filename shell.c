#include "parse.c"

/////////////////////

void execute_command(Command *command){
    if(command->error == 1){
        ERRORC(command->name);
        return;
    }
    pid_t pid = fork();
    if(pid == 0){
        if(command->mod1 == 1){
            int fd = creat(command->output, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if(command->mod2 == 1){
            int fd = open(command->output, O_CREAT | O_WRONLY | O_APPEND, 777);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if(command->mod3 == 1){
            int fd = open(command->input, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        char *command_opt = command->name;
        char **args_opt = malloc(command->length_args + 1);
        args_opt[0] = command_opt;
        for(int i = 0; i < command->length_args; i++){
            args_opt[i + 1] = command->args[i];
        }
        args_opt[command->length_args + 1] = NULL;
        int cap = execvp(command_opt, args_opt);
        if(cap < 0) ERRORC(args_opt[0]);
        exit(0);
    }
    else{
    	wait(NULL);
    }
}


int execute(Command *command){
    if(strcmp(command->name, "history") == 0){
        show_history();
        return 0;
    }
    if(strcmp(command->name, "help") == 0){
        help(command);
        return 0;
    }
    
    if(strcmp(command->name, "exit") == 0)
        return 1;
    if(strcmp(command->name, "cd") == 0){
        int success = chdir(command->args[0]);
        if(success != 0)
            printf("No such file or directory\n");
    }
    else{
        execute_command(command);
    }
    return 0;
}

int String_Of_Commands(Commands_Split_Pipes *commands_pipes){
    int status = execute(&(commands_pipes->_command[0]));
    if(status == 1) return 1;
}

int main(){
    Initial(); //Create file for history
    while(1){
        printf(Yellow "my-prompt " RESET "$ ");
        //initialize
        char *line_input = malloc(SIZE * sizeof(char));
        fgets(line_input, SIZE, stdin); //get line

        if(Is_Only_Spaces(line_input)) continue;

        //First we have to change every command 'again' for the right command on history
        char *new_line = malloc(SIZE);
        Change_Command_Again(line_input, new_line);
        
        Split_Lines_Dotcomma line_split;
        Constructor_Split_Lines_Dotcomma(&line_split);
        Split_Line(new_line, &line_split);

        Save_History(new_line);
        for(int i = 0; i < line_split.length_commands_splits; i++){
            int status = String_Of_Commands(&line_split.commands_splits[i]);
            if(status == 1) return 0;
        }
    }

    return 0;
}
