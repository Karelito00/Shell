#include "parse.c"

/////////////////////
// constant
const int TAM_PATH = 100;

//////////////////////

int execute_command(Command *command,int in,int out){
    
    if(strcmp(command->name, "true") == 0) return 0;
    if(strcmp(command->name, "false") == 0) return 1;
    
    int status = 0;
    pid_t pid = fork();

    if(pid == 0){
        dup2(in,STDIN_FILENO);
        dup2(out,STDOUT_FILENO);


        if(command->mod1 == 1){
            int fd = creat(command->output, 0644);
            dup2(fd, out);
            close(fd);
        }
        if(command->mod2 == 1){
            int fd = open(command->output, O_CREAT | O_WRONLY | O_APPEND, 777);
            dup2(fd, out);
            close(fd);
        }
        if(command->mod3 == 1){
            int fd = open(command->input, O_RDONLY);
            dup2(fd, in);
            close(fd);
        }

        if(out > 2)
            close(out);

        // debug
        // printf("LLEGO1    %d  %d\n",in,out);
        int cap = execvp(command->name, command->args);
        if(cap < 0){
            ERRORC(command->name);
            status = 1;
        }
        exit(0);
    }
    else{
        if(out > 2)
            close(out);
    	wait(&pid);
    }
    return status;
}

int execute(Command *command,int in,int out){
    if(in == -1) in = STDIN_FILENO;
    if(out == -1) out = STDOUT_FILENO;

    if(strcmp(command->name, "history") == 0){
        show_history(in,out);
        return 0;
    }
    if(strcmp(command->name, "help") == 0){
        help(command,in,out); //pending output for pipes
        return 0;
    }
    
    if(strcmp(command->name, "exit") == 0){
        //if has pipe
        if(STDIN_FILENO != in && STDOUT_FILENO != out)
            return EXIT_SUCCESS;

        exit(0);
    }
    if(strcmp(command->name, "cd") == 0){
        if(command->length_args < 2){
            ERRORC("cd");
            return 1;
        }
        //if has pipe
        if(STDIN_FILENO != in && STDOUT_FILENO != out)
            return EXIT_SUCCESS;

        int success = chdir(command->args[1]);
        if(success != 0){
            printf("No such file or directory %s\n", command->args[1]);
            return 1;
        }
        return 0;
    }
    else{
        if(command->error == 1){
            ERRORC(command->name);
            return 1;
        }
        return execute_command(command,in,out);
    }
}

int Only_One_Command(Command *command,int in,int out){
    if(strcmp(command->name, "if") == 0){
        if(command->_if == NULL || command->_then == NULL || command->error == 1){
            printf(Red"La sintaxis del if es incorrecta." RESET "\n");
            return 0;
        }
        Commands_Split_Cond split_cond;
        Constructor_Commands_Split_Cond(&split_cond);
        Parse_Input(command->_if, &split_cond);
        int status_if = 0; //IF
        for(int j = 0; j <= split_cond.length_cond; j++){ //iterating for conditions
            int status = String_Of_Commands(&split_cond.command_by_cond[j]);
            if(j == split_cond.length_cond){
                status_if = status;
                continue;
            }
            if(j == split_cond.length_cond){
                status_if = status;
                continue;
            }
            if(split_cond.Type_Cond[j] == 0){
                if(status == 1){
                    status_if = 1;
                    break;
                }
            }
            else{
                if(status == 1){
                    status_if = 1;
                    break;
                }
            }
        }
        if(status_if == 1){//ELSE
            if(command->_else != NULL){
                Constructor_Commands_Split_Cond(&split_cond);
                Parse_Input(command->_else, &split_cond);
                int status_else = 0;
                for(int j = 0; j <= split_cond.length_cond; j++){ //iterating for conditions
                    int status = String_Of_Commands(&split_cond.command_by_cond[j]);
                    if(j == split_cond.length_cond){
                        status_else = status;
                        continue;
                    }
                    if(j == split_cond.length_cond){
                        status_else = status;
                        continue;
                    }
                    if(split_cond.Type_Cond[j] == 0){
                        if(status == 1){
                            status_else = 1;
                            break;
                        }
                    }
                    else{
                        if(status == 1){
                            status_else = 1;
                            break;
                        }
                    }
                    return status_else;
                }
            }
            else{
                return 1;
            }
        }
        else{//THEN
            Constructor_Commands_Split_Cond(&split_cond);
            Parse_Input(command->_then, &split_cond);
            int status_then = 0;
            for(int j = 0; j <= split_cond.length_cond; j++){ //iterating for conditions
                int status = String_Of_Commands(&split_cond.command_by_cond[j]);
                if(j == split_cond.length_cond){
                    status_then = status;
                    continue;
                }
                if(j == split_cond.length_cond){
                    status_then = status;
                    continue;
                }
                if(split_cond.Type_Cond[j] == 0){
                    if(status == 1){
                        status_then = 1;
                        break;
                    }
                }
                else{
                    if(status == 1){
                        status_then = 1;
                        break;
                    }
                }
            }
            return status_then;
        }
    }
    else{
        int status = execute(command,in,out);
        return status;
    }

    return EXIT_SUCCESS;//never exectue
}

int String_Of_Commands(Commands_Split_Pipes *commands_pipes){
    // int f[2];
    // if (pipe(f) < 0) 
    //     exit(1); 

    char *temp_out = malloc(TAM_PATH);
    char *temp_in = malloc(TAM_PATH);

    int in,out;

    for(int i = 0;i < commands_pipes->length_pipes;i++){
        // debug
        // printf("%d\n",i);
        // printf("%s ",commands_pipes->command_by_pipes[i].name);
        // for(int j = 0;j < commands_pipes->command_by_pipes[i].length_args;j++)
        //     printf("%s ",commands_pipes->command_by_pipes[i].args[j]);
        // printf("\n");

        // int in = f[0];
        // int out = fd;

        if(i == 0)
            in = -1;
        else
            in = open(temp_in,O_RDONLY);

        if(i + 1 == commands_pipes->length_pipes)
            out = -1;
        else{
            getcwd(temp_out,TAM_PATH);
            strcat(temp_out,"/.temporal_output");
            char *num = malloc(5);
            num[0] = (char)('0' + i);
            num[1] = '\0';
            strcat(temp_out,num);

            out = creat(temp_out,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        }

        if(Only_One_Command(&(commands_pipes->command_by_pipes[i]),in,out) == 1){
            printf("Error en la ejecucion del comando\n");
            return EXIT_FAILURE;
        }

        //delete archive old
        if(i > 0)
            remove(temp_in);
        //otput this command is the input of the next
        if(i + 1 < commands_pipes->length_pipes)
            strcpy(temp_in,temp_out);
    }

    return EXIT_SUCCESS;
}

int main(){
    char *path_initial = malloc(TAM_PATH);
    getcwd(path_initial, TAM_PATH);
    strcat(path_initial,"/file_h");

    Initial(path_initial,TAM_PATH); //Create file for history
    while(1){
        printf(Yellow "my-prompt " RESET "$ ");
        //initialize
        char *line_input = malloc(SIZE * sizeof(char));
        fgets(line_input, SIZE, stdin); //get line

        if(Is_Only_Spaces(line_input)) continue;

        //First we have to change every command 'again' for the right command on history
        char *new_line = malloc(SIZE);
        Change_Command_Again(line_input, new_line);
        Save_History(new_line);
        
        if(Have_Background(new_line) != 0){
            printf(Red "Nuestro shell no soporta el keyword: background" RESET "\n");
            continue;
        }

        Split_Lines_Dotcomma line_split;
        Constructor_Split_Lines_Dotcomma(&line_split);
        Split_Line(new_line, &line_split);

        for(int i = 0; i < line_split.length_lines_splits; i++){ //lines independent for ;
            for(int j = 0; j <= line_split.commands_lines[i].length_cond; j++){ //iterating for conditions

                int status = String_Of_Commands(&line_split.commands_lines[i].command_by_cond[j]);

                if(j == line_split.commands_lines[i].length_cond) continue;
                if(line_split.commands_lines[i].Type_Cond[j] == 0){
                    if(status == 0) break;
                }
                else{
                    if(status == 1) break;
                }
            }
        }
    }

    return 0;
}
