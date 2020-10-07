#include "tools.c"

void Copy_To_Line(char word_temp[], int *count_words, Command *input_process, int *length_word_temp){
    word_temp[*length_word_temp] = 0;
    
    if(*count_words == 0){ //it's the command
        for(int i = 0; i < *length_word_temp + 1; i++){
            input_process->command[i] = word_temp[i];
        }
    }
    else{ //it's a parameter
        input_process->args[*count_words - 1] = malloc(SIZE * sizeof(char));
        for(int i = 0; i < *length_word_temp + 1; i++){
            input_process->args[*count_words - 1][i] = word_temp[i];
        }
        input_process->length_args++;
    }
    *length_word_temp = 0;
    (*count_words)++;
}

//Return an object that contain, the command, and arguments for the command
Commands_Split_Pipes Parse_Input(char line_input[]){
    int reading = 0;
    char *word_temp = malloc(SIZE * sizeof(char));
    int length_word_temp = 0;
    int count_words = 0;
    Commands_Split_Pipes input_process;
    Constructor_Commands_Split_Pipes(&input_process);
    input_process.length_commands++;
    Constructor_Command(&(input_process._command[0]));
    int comillas = 0;
    for(int i = 0; i < strlen(line_input); i++){
        if(line_input[i] == '#' || line_input[i] == '\n') break;
        if(line_input[i] == '\'' && line_input[i] == '\"'){
            if(comillas == 1){
                comillas = 0;
                reading = 0;
                Copy_To_Line(word_temp, &count_words, &input_process._command[input_process.length_commands - 1], &length_word_temp);
            }
            else{
                if(reading == 1){
                    Copy_To_Line(word_temp, &count_words, &input_process._command[input_process.length_commands - 1], &length_word_temp);
                    reading = 0;
                }
                comillas = 1;
            }
            continue;
        }
        if(comillas == 0 && Is_Special(line_input[i]) > 0){
            if(reading == 0) continue;
            reading = 0;
            Copy_To_Line(word_temp, &count_words, &input_process._command[input_process.length_commands - 1], &length_word_temp);
            if(line_input[i] != ' '){ //it's special character
                if(i + 1 == strlen(line_input)){
                    word_temp[length_word_temp] = line_input[i];
                    length_word_temp++;
                    Copy_To_Line(word_temp, &count_words, &input_process._command[input_process.length_commands - 1], &length_word_temp);
                    continue;
                }
                if(line_input[i] == '|'){
                    if(line_input[i + 1] == '|'){
                        word_temp[length_word_temp] = line_input[i];
                        length_word_temp++;
                        i++;
                        word_temp[length_word_temp] = line_input[i];
                        length_word_temp++;
                        Copy_To_Line(word_temp, &count_words, &input_process._command[input_process.length_commands - 1], &length_word_temp);
                    }
                    else{
                        input_process.length_commands++;
                        Constructor_Command(&(input_process._command[input_process.length_commands - 1]));
                        continue;
                    }
                }
                else if(line_input[i] == '>'){
                    if(line_input[i + 1] == '>'){
                        i++;
                        word_temp[length_word_temp] = line_input[i];
                        length_word_temp++;
                        Copy_To_Line(word_temp, &count_words, &input_process._command[input_process.length_commands - 1], &length_word_temp);
                    }
                }
            }
            continue;
        }
        reading = 1;
        word_temp[length_word_temp] = line_input[i];
        length_word_temp++;
    }
    if(reading == 1 || comillas == 1){
        Copy_To_Line(word_temp, &count_words, &(input_process._command[input_process.length_commands - 1]), &length_word_temp);
    }
    return input_process;
}