#include "help.c"

void Initial(){
    //Para crear el archivo file_h en caso de que no exista que va a contener el historial
    FILE *cf = fopen("file_h", "r");
    if(cf == NULL){
        cf = fopen("file_h", "w");
        fprintf(cf, "%d\n", 0);
    }
    fclose(cf);
}

int Again_Command(Command *input_process, char get_line[]){
    if(input_process->length_args == 0) return 0;
    int number = 0;
    char *first_arg = input_process->args[0];
    for(int i = 0; i < strlen(first_arg); i++){
        if(first_arg[i] < '0' || first_arg[i] > '9') return 0;
        number = (number * 10) + (first_arg[i] - '0');
    }
    
    if(number == 0) return 0;
    FILE *file_h = fopen("file_h", "a+");
    int lines_history = 0;
    char *hist = malloc(SIZE);
    fscanf(file_h, "%d", &lines_history);
    if(lines_history < number){
        fclose(file_h);
        return 0;
    }
    fgets(hist, SIZE, file_h); //Discard first line because it's the number of lines of history
    for(int j = 0; j < lines_history; j++){
        fgets(hist, SIZE, file_h);
        if(number == j + 1){
            int pos = 0;
            for(int i = 0; i < strlen(hist); i++){
                if(hist[i] == '\n')
                    break;
                get_line[i] = hist[i];
                pos = i;
            }
            get_line[pos + 1] = 0;
            break;
        }
    }
    fclose(file_h);
    return 1;
}

void Change_Command_Again(char line[], char new_line[]){

    int len = strlen(line);
    char aux[] = {'a', 'g', 'a', 'i', 'n'};
    int length_new_line = 0;
    int i = 0;
    for(; i + 4 < len; i++){
        int sub_string = 1;
        for(int j = i; j < i + 5; j++){
            if(aux[j - i] != line[j])
                sub_string = 0;
        }
        if(i != 0 && Is_Special(line[i - 1]) == 0) sub_string = 0;
        if(i + 5 != len && line[i + 5] != ' ') sub_string = 0;
        if(sub_string == 0){
            new_line[length_new_line] = line[i];
            length_new_line++;
            continue;
        }

        int point = i + 5;
        while(point < len && line[point] == ' '){
            point++;
        }

        char *param = malloc(SIZE);
        int pos = 0;
        while(point < len && Is_Special(line[point]) == 0 && line[point] != '\n'){
            param[pos] = line[point];
            point++;
            pos++;
        }
        param[pos] = 0;
        Command aux_command;  //Begin Parse To Command
        Constructor_Command(&aux_command);
        char *line_of_history = malloc(SIZE);
        aux_command.length_args = 1;
        aux_command.args[0] = malloc(SIZE);
        Copy_To(param, aux_command.args[0]);
        aux_command.name = malloc(SIZE);
        Copy_To(aux, aux_command.name); //End Parse To Command

        int response = Again_Command(&aux_command, line_of_history);
        if(response == 0){
            new_line[length_new_line] = line[i];
            length_new_line++;
            continue;
        }
        i = point - 1;
        for(int j = 0; j < strlen(line_of_history); j++){
            new_line[length_new_line] = line_of_history[j];
            length_new_line++;
        }
    }
    while(i < len){
        new_line[length_new_line] = line[i];
        length_new_line++;
        i++;
    }
    new_line[length_new_line] = 0;
}

int Have_Background(char line[]){
    int len = strlen(line);
    for(int i = 0; i < len; i++){
        if(line[i] == '#' || line[i] == '\n') break;
        if(line[i] == '&'){
            int aux = 0;
            if(i > 0 && line[i - 1] == '&')
                aux++;
            if(i < len - 1 && line[i + 1] == '&')
                aux++;
            if(aux == 0) return 1;
        }
    }
    return 0;
}

void Save_History(char *line){
    FILE *file_h = fopen("file_h", "a+");
    int lines = 0;
    char *hist = malloc(SIZE);
    char *file_history = malloc(SIZE);
    int count = 0;
    int lines_history;
    fscanf(file_h, "%d", &lines_history);
    fgets(file_history, SIZE, file_h); //Discard first line because it's the number of lines of history
    for(int j = 0; j < lines_history; j++){
        fgets(file_history, SIZE, file_h);
        if(lines_history == 10 && j == 0) continue;
        for(int i = 0; i < strlen(file_history); i++){
            hist[count] = file_history[i];
            count++;
            if(file_history[i] == '\n') break;
        }
    }

    for(int i = 0; i < strlen(line); i++){
        hist[count] = line[i];
        count++;
        if(line[i] == '\n') break;
    }
    fclose(file_h);
    file_h = fopen("file_h", "w");
    fprintf(file_h, "%d\n", min(lines_history + 1, 10));
    fputs(hist, file_h);
    fclose(file_h);
}

void show_history(){
    FILE *file_h = fopen("file_h", "r");
    char *history_line = malloc(100);
    int lines_history;
    fscanf(file_h, "%d", &lines_history);
    fgets(history_line, 100, file_h);
    for(int j = 0; j < lines_history; j++){
        fgets(history_line, 100, file_h);
        printf("%d: ", j + 1);
        printf("%s", history_line);
    }
    fclose(file_h);
}

char* Delete_Spaces_Of_The_begin(char line_input_temp[]){
    char *line_input = malloc(SIZE * sizeof(char));
    int space = 0;
    int length_line_input = 0;
    for(int i = 0; i < strlen(line_input_temp); i++){
        if(space == 0 && line_input_temp[i] == ' ') continue;
        space = 1;
        line_input[length_line_input] = line_input_temp[i];
        length_line_input++;
    }
    return line_input;
}

int Is_Only_Spaces(char line[]){
    for(int i = 0; i < strlen(line); i++){
        if(line[i] != ' ' && line[i] != '\n')
            return 0;
    }
    return 1;
}