#include "dependencies.c"

void general_help(){
    printf("\nIntegrantes: Alben Luis Urquiza Rojas y Karel Diaz Vergara del 212\n\n");
    printf(Blue"Funcionalidades:"RESET "\n");
    printf("basic: funcionalidades basicas (3 puntos)\n");
    printf("multi-pipe: multiples tuberias (1 punto)\n");
    printf("help: ayuda (1 punto)\n");
    printf("\n");
    printf(Blue"Comandos built-in:"RESET"\n");
    printf("cd: cambia de directorios\n");
    printf("pwd: muestra el path del directorio en que se encuentra\n");
    printf("exit: termina la ejecucion del shell\n");
    printf("help: muestra esta ayuda\n");
    printf("history: muestra las ultimas 10 lineas de commandos ejecutadas en este shell\n");
    printf("again <number>: Ejecuta la linea <number> en el history, si esta linea no existe en el history pues \n                mostraremos error al ejecutar el comando, <again> debe ser un numero entre el 1 y el\n                10 \'again\'\n");
    printf("true: comando que retorna exit status 0\n");
    printf("false: comando que retorna exit status 1\n");
    printf("\n");
    printf("Keyword: if, multi-if\n");
    printf("         Descripcion: Nuestro shell soporta la secuencia if then else end, donde el else es opcional\n");
    printf("                      y ademas soporta cualquier cantidad de if anidados.\n");
    printf("Keyword: if, chain\n");
    printf("         Descripcion: Funcionan todas las formas de encadenar comandos ya sea ; || &&\n");
    printf("Keyword: Pipes\n");
    printf("         Descripcion: En desarrollo\n");

    printf("\nTotal de puntos: 7.0 puntos\n\n");

}

int help(Command *input_process,int in,int out){
    if(input_process->length_args == 1){
        general_help();
        return 0;
    }
    char *param = input_process->args[1];
    if(strcmp(param, "")){

    }
    printf(Red "No se econtro la ayuda para el comando: %s.\n" RESET, param);
    return 1;
}