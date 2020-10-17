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

    int not_yet = 0;
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
            
            if(not_yet == 0){
                split_pipes->command_by_pipes[length_split].name = malloc(sizeof(char) * SIZE);
                Copy_To(temp->args[i], split_pipes->command_by_pipes[length_split].name);
                not_yet = 1;
            }

            split_pipes->command_by_pipes[length_split].args[val - 1] = malloc(sizeof(char) * SIZE);
            Copy_To(temp->args[i], split_pipes->command_by_pipes[length_split].args[val - 1]);
        }
    }
    int aux = split_pipes->command_by_pipes[length_split].length_args;
    split_pipes->command_by_pipes[length_split].args[aux] = NULL;
    if(temp->_if != NULL){
        split_pipes->command_by_pipes[length_split]._if = malloc(SIZE);
        Copy_To(Delete_Spaces_Of_The_begin(temp->_if), split_pipes->command_by_pipes[length_split]._if);
    }
    if(temp->_then != NULL){
        split_pipes->command_by_pipes[length_split]._then = malloc(SIZE);
        Copy_To(Delete_Spaces_Of_The_begin(temp->_then), split_pipes->command_by_pipes[length_split]._then);
    }
    if(temp->_else != NULL){
        split_pipes->command_by_pipes[length_split]._else = malloc(SIZE);
        Copy_To(Delete_Spaces_Of_The_begin(temp->_else), split_pipes->command_by_pipes[length_split]._else);
    }
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

int compare_to(char word[], int tip, int len){
    word[len] = 0;
    if(tip == 1){
        if(strcmp(word, "then") == 0){
            return 1;
        }
        return 0;
    }
    else if(tip == 2){
        if(strcmp(word, "else") == 0){
            return 1;
        }
        else if(strcmp(word, "end") == 0){
            return 2;
        }
        return 0;
    }
    if(strcmp(word, "end") == 0){
        return 1;
    }
    return 0;
}

typedef struct IF{
    int ini_then;
    int ini_else;
    int ini_if;
    int pos_then;
    int pos_else;
    int pos_if;
}IF;

void Constructor_IF(IF *cond){
    cond->pos_then = cond->pos_else = cond->pos_if = -1;
    cond->ini_then = cond->ini_else = cond->ini_if = -1;
}

int finished(int array_cond[], int *pointer_stack, int i, IF *cond, int term){
    if(*pointer_stack == 0){
        if(array_cond[*pointer_stack] == 4){
            cond->pos_else = i - 3;
            return 1;
        }
        else if(array_cond[*pointer_stack] == 3){
            cond->pos_then = i - 4;
            cond->ini_else = term;
        }
        else if(array_cond[*pointer_stack] == 2){
            cond->pos_if= i - 4;
            cond->ini_then = term;
        }
    }
    if(array_cond[*pointer_stack] == 4){
        (*pointer_stack)--;
    }
    return 0;
}

void clean_word(char word_temp[], int *length_word_temp){
    word_temp = malloc(SIZE);
    (*length_word_temp) = 0;
}

IF processing_if(char line_input[], int pos_if){
    int *array_cond = malloc(SIZE * sizeof(int));
    int pointer_stack = 0;
    array_cond[0] = 1;

    char *word_temp = malloc(SIZE * sizeof(char));
    int length_word_temp = 0;
    char comillas = 0;
    int reading = 0;
    IF cond;
    Constructor_IF(&cond);
    cond.ini_if = pos_if;
    for(int i = pos_if; i < strlen(line_input); i++){
        if(line_input[i] == '#' || line_input[i] == '\n'){
            reading = 0;
            int aux = 0;
            if(strcmp(word_temp, "if") == 0){
                pointer_stack++;
                array_cond[pointer_stack] = 1;
                continue;
            }
            if(comillas != 0) aux = 1;
            int sum = compare_to(word_temp, array_cond[pointer_stack], length_word_temp);
            array_cond[pointer_stack] += sum;
            if(sum > 0){
                int status = finished(array_cond, &pointer_stack, i - 1 - aux, &cond, i + 1);
                if(status == 1) return cond;
            }
            break;
        }
        if(line_input[i] == '\'' || line_input[i] == '\"'){
            if(comillas == 0){
                if(strcmp(word_temp, "if") == 0){
                    pointer_stack++;
                    array_cond[pointer_stack] = 1;
                }
                else{
                    int sum = compare_to(word_temp, array_cond[pointer_stack], length_word_temp);
                    array_cond[pointer_stack] += sum;
                    if(sum > 0){
                        int status = finished(array_cond, &pointer_stack, i - 1, &cond, i + 1);
                        if(status == 1) return cond;
                    }
                }
                comillas = line_input[i];
                clean_word(word_temp, &length_word_temp);
                continue;
            }
            if(comillas == line_input[i]){
                if(strcmp(word_temp, "if") == 0){
                    pointer_stack++;
                    array_cond[pointer_stack] = 1;
                }
                else{
                    int sum = compare_to(word_temp, array_cond[pointer_stack], length_word_temp);
                    array_cond[pointer_stack] += sum;
                    if(sum > 0){
                        int status = finished(array_cond, &pointer_stack, i - 2, &cond, i + 1);
                        if(status == 1) return cond;
                    }
                }
                clean_word(word_temp, &length_word_temp);
                comillas = 0;
                continue;
            }
        } 
        if(comillas == 0){
            if(line_input[i] == ' '){
                if(reading == 0) continue;
                if(strcmp(word_temp, "if") == 0){
                    pointer_stack++;
                    array_cond[pointer_stack] = 1;
                }
                else{
                    int sum = compare_to(word_temp, array_cond[pointer_stack], length_word_temp);
                    array_cond[pointer_stack] += sum;
                    if(sum > 0){
                        int status = finished(array_cond, &pointer_stack, i - 1, &cond, i);
                        if(status == 1) return cond;
                    }
                }
                clean_word(word_temp, &length_word_temp);
                continue;
            }
        }
        reading = 1;
        word_temp[length_word_temp] = line_input[i];
        length_word_temp++;
    }
    
    if(comillas != 0 || reading != 0){
        if(strcmp(word_temp, "if") == 0){
            pointer_stack++;
            array_cond[pointer_stack] = 1;
        }
        else{
            int aux = 0;
            if(comillas != 0) aux = 1;
            int sum = compare_to(word_temp, array_cond[pointer_stack], length_word_temp);
            array_cond[pointer_stack] += sum;
            if(sum > 0)
                finished(array_cond, &pointer_stack, length_word_temp - 1 - aux, &cond, (int)strlen(line_input));
        }
    }
    return cond;
}

Command convert_if_to_command(IF *cond, char line_input[]){
    Command temp;
    Constructor_Command(&temp);
    temp.name = "if";
    if(cond->pos_else == -1){
        temp.error = 1;
        return temp;
    }
    int pos = 0;
    temp._if = malloc(SIZE);
    for(int i = cond->ini_if; i <= cond->pos_if; i++){
        temp._if[pos] = line_input[i];
        pos++;
    }
    temp._if[pos] = 0;
    pos = 0;
    temp._then = malloc(SIZE);
    int ELSE = 1;
    int then_fin = cond->pos_then;
    if(then_fin == -1){
        ELSE = 0;
        then_fin = cond->pos_else;
    }
    for(int i = cond->ini_then; i <= then_fin; i++){
        temp._then[pos] = line_input[i];
        pos++;
    }
    temp._then[pos] = 0;
    if(ELSE == 1){
        int pos = 0;
        temp._else = malloc(SIZE);
        for(int i = cond->ini_else; i <= cond->pos_else; i++){
            temp._else[pos] = line_input[i];
            pos++;
        }
        temp._else[pos] = 0;
    }
    return temp;
}

void Parse_Input(char line_input[], Commands_Split_Cond *split_cond){
    int reading = 0;
    char *word_temp = malloc(SIZE * sizeof(char));
    int length_word_temp = 0;
    int count_words = 0;
    Command temp_command;
    Constructor_Command(&temp_command);
    Constructor_Commands_Split_Pipes(&(split_cond->command_by_cond[0]));
    char comillas = 0;
    for(int i = 0; i < strlen(line_input); i++){
        if(line_input[i] == '#' || line_input[i] == '\n') break;
        if(line_input[i] == '\'' || line_input[i] == '\"'){
            if(comillas == 0){
                if(reading == 1){
                    Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
                    reading = 0;
                }
                comillas = line_input[i];
                continue;
            }
            if(comillas == line_input[i]){
                comillas = 0;
                reading = 0;
                Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
                continue;
            }
        }
        if(comillas == 0 && Is_Special(line_input[i]) > 0){
            if(count_words == 0 && reading != 0){ //we have to check if the name of command is 'if'
                if(strcmp(word_temp, "if") == 0){
                    reading = 0;
                    IF cond = processing_if(line_input, i);
                    temp_command = convert_if_to_command(&cond, line_input);
                    Copy_Temp(&temp_command, &split_cond->command_by_cond[split_cond->length_cond]);
                    clean(&temp_command, word_temp, &length_word_temp, &count_words);
                    reading = 0;
                    i = max(max(cond.pos_else, max(cond.ini_if, max(cond.ini_then, cond.pos_else))), max(cond.pos_if, cond.pos_then));
                    for(int j = i; j < strlen(line_input); j++){
                        if(j + 1 == strlen(line_input) || line_input[j] == '#' || line_input[j] == '\n'){
                            i = strlen(line_input);
                            break;
                        }
                        if(Is_Special(line_input[j]) > 0 && line_input[j] != ' '){
                            i = j - 1;
                            break;
                        }
                    }
                    continue;
                }
            }
            if(reading != 0){
                reading = 0;
                Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
            }
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
    if(reading == 1 || comillas != 0){
        Copy_To_Line(word_temp, &count_words, &temp_command, &length_word_temp);
    }
    if(temp_command.length_args != 0)
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