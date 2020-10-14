#include "tools.c"

void Copy_To_Line(char word_temp[], int *count_words, Command *temp_command, int *length_word_temp){
    word_temp[*length_word_temp] = 0;
    if(*count_words == 0){ //it's the command
        Copy_To(word_temp, temp_command->name);
        temp_command->args[0] = malloc(SIZE * sizeof(char));
        Copy_To(word_temp, temp_command->args[0]);
        temp_command->length_args++;
        temp_command->args[1] = NULL;
    }

    else{ //it's a parameter
        temp_command->args[temp_command->length_args] = malloc(SIZE * sizeof(char));
        Copy_To(word_temp, temp_command->args[temp_command->length_args]);
        temp_command->length_args++;
        temp_command->args[temp_command->length_args] = NULL;
    }
    *length_word_temp = 0;
    (*count_words)++;
}

void Copy_Temp(Command *temp, Commands_Split_Pipes *split_pipes){
    int length_split = split_pipes->length_pipes;
    split_pipes->length_pipes++;
    Constructor_Command(&(split_pipes->command_by_pipes[length_split]));
    Copy_To(temp->name, split_pipes->command_by_pipes[length_split].name);

    for(int i = 0; i < temp->length_args; i++){
        if(strcmp(temp->args[i], ">") == 0){
            if(i + 1 == temp->length_args){
                split_pipes->command_by_pipes[length_split].error = 1;
                continue;
            }
            split_pipes->command_by_pipes[length_split].mod1 = 1;
            split_pipes->command_by_pipes[length_split].mod2 = 0;
            split_pipes->command_by_pipes[length_split].output = temp->args[i + 1];
            i++;
        }
        else if(strcmp(temp->args[i], ">>") == 0){
            if(i + 1 == temp->length_args){
                split_pipes->command_by_pipes[length_split].error = 1;
                continue;
            }
            split_pipes->command_by_pipes[length_split].mod1 = 0;
            split_pipes->command_by_pipes[length_split].mod2 = 1;
            split_pipes->command_by_pipes[length_split].output = temp->args[i + 1];
            i++;
        }
        else if(strcmp(temp->args[i], "<") == 0){
            if(i + 1 == temp->length_args){
                split_pipes->command_by_pipes[length_split].error = 1;
                continue;
            }
            split_pipes->command_by_pipes[length_split].mod3 = 1;
            split_pipes->command_by_pipes[length_split].input = temp->args[i + 1];
            i++;
        }
        else{
            split_pipes->command_by_pipes[length_split].length_args++;
            int val = split_pipes->command_by_pipes[length_split].length_args;
            split_pipes->command_by_pipes[length_split].args[val - 1] = malloc(sizeof(char) * SIZE);
            Copy_To(temp->args[i], split_pipes->command_by_pipes[length_split].args[val - 1]);
        }
    }
    int aux = split_pipes->command_by_pipes[length_split].length_args;
    split_pipes->command_by_pipes[length_split].args[aux] = NULL;
}

void Create_Cond(Commands_Split_Cond *split_cond, int type){
    int length_cond = split_cond->length_cond;
    split_cond->length_cond++;
    Constructor_Commands_Split_Pipes(&(split_cond->command_by_cond[length_cond + 1]));
    split_cond->Type_Cond[length_cond] = type;
}

void clean(Command *temp_command, char word_temp[], int *length_word_temp, int *count_words){
    *length_word_temp = 0;
    *count_words = 0;
    Constructor_Command(temp_command);
    length_word_temp = malloc(SIZE);
}

//Return an object that contain, the command, and arguments for the command
void Parse_Input(char line_input[], Commands_Split_Cond *split_cond){
    int reading = 0;
    char *word_temp = malloc(SIZE * sizeof(char));
    int length_word_temp = 0;
    int count_words = 0;
    Command temp_command;
    Constructor_Command(&temp_command);
    Constructor_Commands_Split_Pipes(&(split_cond->command_by_cond[0]));
    int comillas = 0;
    for(int i = 0; i < strlen(line_input); i++){
        if(line_input[i] == '#' || line_input[i] == '\n') break;
        if(line_input[i] == '\'' || line_input[i] == '\"'){
            if(comillas == 1){
                comillas = 0;
                reading = 0;
                Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
            }
            else{
                if(reading == 1){
                    Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
                    reading = 0;
                }
                comillas = 1;
            }
            continue;
        }
        if(comillas == 0 && Is_Special(line_input[i]) > 0){
            if(reading == 0) continue;
            reading = 0;
            Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
            if(line_input[i] != ' '){ //it's special character
                if(i + 1 == strlen(line_input)){
                    word_temp[length_word_temp] = line_input[i];
                    length_word_temp++;
                    Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
                    continue;
                }
                if(line_input[i] == '|'){
                    if(line_input[i + 1] == '|'){
                        Copy_Temp(&temp_command, &split_cond->command_by_cond[split_cond->length_cond]);
                        Create_Cond(split_cond, 0);
                        clean(&temp_command, word_temp, &length_word_temp, &count_words);
                        i++;
                        continue;
                    }
                    else{
                        Copy_Temp(&temp_command, &split_cond->command_by_cond[split_cond->length_cond]);
                        clean(&temp_command, word_temp, &length_word_temp, &count_words);
                        continue;
                    }
                }
                else if(line_input[i] == '&'){
                    if(line_input[i + 1] == '&'){
                        Copy_Temp(&temp_command, &split_cond->command_by_cond[split_cond->length_cond]);
                        Create_Cond(split_cond, 1);
                        clean(&temp_command, word_temp, &length_word_temp, &count_words);
                        i++;
                        continue;
                    }
                }
                else if(line_input[i] == '>'){
                    if(line_input[i + 1] == '>'){
                        i++;
                        word_temp[length_word_temp] = line_input[i];
                        length_word_temp++;
                    }
                }
                word_temp[length_word_temp] = line_input[i];
                length_word_temp++;
                Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
            }
            continue;
        }
        reading = 1;
        word_temp[length_word_temp] = line_input[i];
        length_word_temp++;
    }
    if(reading == 1 || comillas == 1){
        Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
    }
    Copy_Temp(&temp_command, &(split_cond->command_by_cond[split_cond->length_cond]));
}

void Split_Line(char line[], Split_Lines_Dotcomma *line_split){
    char *word_temp = malloc(SIZE);
    int length_word_temp = 0;
    for(int i = 0; i < strlen(line); i++){
        if(line[i] == ';'){
            if(length_word_temp == 0) continue;
            char *line_without_space = Delete_Spaces_Of_The_begin(word_temp);
            Constructor_Commands_Split_Cond(&line_split->commands_lines[line_split->length_lines_splits]);
            Parse_Input(line_without_space, &(line_split->commands_lines[line_split->length_lines_splits]));
            line_split->length_lines_splits++;
            word_temp = malloc(SIZE);
            length_word_temp = 0;
            continue;
        }
        word_temp[length_word_temp] = line[i];
        length_word_temp++;
    }
    if(length_word_temp > 0){
        char *line_without_space = Delete_Spaces_Of_The_begin(word_temp);
        Constructor_Commands_Split_Cond(&line_split->commands_lines[line_split->length_lines_splits]);
        Parse_Input(line_without_space, &(line_split->commands_lines[line_split->length_lines_splits]));
        line_split->length_lines_splits++;
        word_temp = malloc(SIZE);
        length_word_temp = 0;
    }
}