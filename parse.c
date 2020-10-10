#include "tools.c"

void Copy_To_Line(char word_temp[], int *count_words, Command *temp_command, int *length_word_temp){
    word_temp[*length_word_temp] = 0;
    if(*count_words == 0) //it's the command
        Copy_To(word_temp, temp_command->name);

    else{ //it's a parameter
        temp_command->args[*count_words - 1] = malloc(SIZE * sizeof(char));
        Copy_To(word_temp, temp_command->args[*count_words - 1]);
        temp_command->length_args++;
    }
    *length_word_temp = 0;
    (*count_words)++;
}

void Copy_Temp(Command *temp, Commands_Split_Pipes *input_process){
    input_process->length_commands++;
    int length_split = input_process->length_commands - 1;

    Constructor_Command(&(input_process->_command[length_split]));
    Copy_To(temp->name, input_process->_command[length_split].name);
    

    for(int i = 0; i < temp->length_args; i++){
        if(strcmp(temp->args[i], ">") == 0){
            if(i + 1 == temp->length_args){
                input_process->_command[length_split].error = 1;
                continue;
            }
            input_process->_command[length_split].mod1 = 1;
            input_process->_command[length_split].mod2 = 0;
            input_process->_command[length_split].output = temp->args[i + 1];
            i++;
        }
        else if(strcmp(temp->args[i], ">>") == 0){
            if(i + 1 == temp->length_args){
                input_process->_command[length_split].error = 1;
                continue;
            }
            input_process->_command[length_split].mod1 = 0;
            input_process->_command[length_split].mod2 = 1;
            input_process->_command[length_split].output = temp->args[i + 1];
            i++;
        }
        else if(strcmp(temp->args[i], "<") == 0){
            if(i + 1 == temp->length_args){
                input_process->_command[length_split].error = 1;
                continue;
            }
            input_process->_command[length_split].mod3 = 1;
            input_process->_command[length_split].input = temp->args[i + 1];
            i++;
        }
        else{
            input_process->_command[length_split].length_args++;
            int val = input_process->_command[length_split].length_args;
            input_process->_command[length_split].args[val - 1] = malloc(sizeof(char) * SIZE);
            Copy_To(temp->args[i], input_process->_command[length_split].args[val - 1]);
        }
    }
}

//Return an object that contain, the command, and arguments for the command
Commands_Split_Pipes Parse_Input(char line_input[]){
    int reading = 0;
    char *word_temp = malloc(SIZE * sizeof(char));
    int length_word_temp = 0;
    int count_words = 0;
    Commands_Split_Pipes input_process;
    Constructor_Commands_Split_Pipes(&input_process);
    Command temp_command;
    Constructor_Command(&temp_command);
    int comillas = 0;
    for(int i = 0; i < strlen(line_input); i++){
        if(line_input[i] == '#' || line_input[i] == '\n') break;
        if(line_input[i] == '\'' && line_input[i] == '\"'){
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
                        i++;
                        word_temp[length_word_temp] = line_input[i];
                        length_word_temp++;
                    }
                    else{
                        Copy_Temp(&temp_command, &input_process);
                        Constructor_Command(&temp_command);
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
    Copy_Temp(&temp_command, &input_process);
    return input_process;
}

void Split_Line(char line[], Split_Lines_Dotcomma *line_split){
    char *word_temp = malloc(SIZE);
    int length_word_temp = 0;
    for(int i = 0; i < strlen(line); i++){
        if(line[i] == ';'){
            if(length_word_temp == 0) continue;
            char *line_without_space = Delete_Spaces_Of_The_begin(word_temp);
            line_split->commands_splits[line_split->length_commands_splits] = Parse_Input(line_without_space);
            line_split->length_commands_splits++;
            word_temp = malloc(SIZE);
            length_word_temp = 0;
            continue;
        }
        word_temp[length_word_temp] = line[i];
        length_word_temp++;
    }
    if(length_word_temp > 0){
        char *line_without_space = Delete_Spaces_Of_The_begin(word_temp);
        line_split->commands_splits[line_split->length_commands_splits] = Parse_Input(line_without_space);
        line_split->length_commands_splits++;
        word_temp = malloc(SIZE);
        length_word_temp = 0;
    }
}