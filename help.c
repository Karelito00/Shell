#include "dependencies.c"

void general_help(){
    printf("\nIntegrantes: Alben Luis Urquiza Rojas y Karel Diaz Vergara del 212\n\n");
    printf("Funcionalidades:\n");

    printf("\n");
    printf("Comandos built-in:\n");
    printf("\nTotal de puntos: x.x puntos\n\n");
}

int help(Command *input_process){
    if(input_process->length_args == 1){
        general_help();
        return 0;
    }
    char *param = input_process->args[1];
    printf(Red "No se econtro la ayuda para el comando: %s.\n" RESET, param);
    return 1;
}