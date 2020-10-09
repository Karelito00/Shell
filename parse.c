#include "tools.c"

void Copy_To_Line(char word_temp[], int *count_words, Command *temp_command, int *length_word_temp){
    word_temp[*length_word_temp] = 0;
    if(*count_words == 0) //it's the command
        Copy_To(word_temp, temp_command->command);

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
    Constructor_Command(&(input_process->_command[input_process->length_commands - 1]));
    Copy_To(temp->command, input_process->_command[input_process->length_commands - 1].command);
    for(int i = 0; i < temp->length_args; i++){
        if(strcmp(temp->args[i], ">") == 0){
            input_process->_command[input_process->length_commands - 1].mod1 = 1;
            input_process->_command[input_process->length_commands - 1].mod2 = 0;
            input_process->_command[input_process->length_commands - 1].output = temp->args[i + 1];
            i++;
        }
        else if(strcmp(temp->args[i], ">>") == 0){
            input_process->_command[input_process->length_commands - 1].mod1 = 0;
            input_process->_command[input_process->length_commands - 1].mod2 = 1;
            input_process->_command[input_process->length_commands - 1].output = temp->args[i + 1];
            i++;
        }
        else if(strcmp(temp->args[i], "<") == 0){
            input_process->_command[input_process->length_commands - 1].mod3 = 1;
            input_process->_command[input_process->length_commands - 1].input = temp->args[i + 1];
            i++;
        }
        else{
            input_process->_command[input_process->length_commands - 1].length_args++;
            int val = input_process->_command[input_process->length_commands - 1].length_args;
            input_process->_command[input_process->length_commands - 1].args[val - 1] = malloc(sizeof(char) * SIZE);
            Copy_To(temp->args[i], input_process->_command[input_process->length_commands - 1].args[val - 1]);
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
        Copy_Temp(&temp_command, &input_process);
    }
    return input_process;
}