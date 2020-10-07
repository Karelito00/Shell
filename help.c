#include "dependencies.c"

void general_help(){
    printf("Integrantes: Alben Luis Urquiza Rojas y Karel Diaz Vergara del 212\n\n");
    printf("Funcionalidades:\n");

    printf("\n");
    printf("Comandos built-in:\n");
    printf("\nTotal de puntos: x.x puntos\n\n");
}

void help(Command *input_process){
    if(input_process->length_args == 0){
        general_help();
        return;
    }
    char *param = input_process->args[0];
    printf(Red "No se econtro la ayuda para el comando: %s.\n" RESET, param);
}