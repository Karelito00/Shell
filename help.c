#include "dependencies.c"

void general_help(){
    printf("\nIntegrantes: Alben Luis Urquiza Rojas y Karel Diaz Vergara del 212\n\n");

    printf(Red"Importante: Esto es una lista de las cosas que no debes escribir en este shell:"RESET "\n");
    printf(Red"            No escribir parentesis(), ni llaves{}, ni corchetes[] con el objetivo de evitar un comportamiento inesperado.\n"RESET "\n");

    printf(Blue"Funcionalidades:"RESET "\n");
    printf("basic: funcionalidades basicas (3 puntos)\n");
    printf("multi-pipe: multiples tuberias (1 punto)\n");
    printf("help: ayuda (1 punto)\n");
    printf("spaces: cualquier cantidad de espacios entre el comando y sus argumentos (0.5 puntos)\n");
    printf("history: historial y sus funcionalidades (0.5 puntos)\n");
    printf("ctrl+c: que el programa soporte el ctrl+c sin detenerse (0.5 puntos)\n");
    printf("chain: soporta el encadenamiento de comandos (0.5 puntos)\n");
    printf("if: soporta las condicionales (1 punto)\n");
    printf("multi-if: soporta la anidacion de condiciones (0.5 puntos)\n");
    printf("variables: declaracion, eliminacion y lista de variables globales (1 punto)\n");

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
    printf("get <var>: si existe una variable asignada con el nombre <var> se mostrara su valor\n");
    printf("set <var> <args>: creara una variable con el nombre <var> y su valor <args> en caso de que no exista,\n                  de lo contrario se modificara solo su valor.\n");
    printf("unset <var>: Si existe una variable asignada con el nombre <var> sera eliminada.\n");
    printf("\n");

    printf("Keyword: if, multi-if\n");
    printf("         Descripcion: Nuestro shell soporta la secuencia if then else end, donde el else es opcional\n");
    printf("                      y ademas soporta cualquier cantidad de if anidados.\n");
    printf("Keyword: if, chain\n");
    printf("         Descripcion: Funcionan todas las formas de encadenar comandos ya sea ; || &&\n");
    printf("Keyword: Pipes\n");
    printf("         Descripcion: En desarrollo\n");
    printf("Keyword: Ctrl+C\n");
    printf("         Descripcion: Cuando un proceso esta en ejecucion, al presionar Ctrl + C se le\n");
    printf("                      envia SIGINT, si este decide ignorarlo, pues en el proximo Ctrl+c\n");
    printf("                      presionado se le enviara al proceso SIGKILL\n");
    printf("Keyword: Variables\n");
    printf("         Descripcion: Declaracion, eliminacion y lista de variables globales.(0.5 puntos)\n");

    printf("\n" Blue "Para mas informacion buscar los detalles de cada una de estas funcionalidades"RESET"\n");
    printf("help <func>: imprime la ayuda de la funcionalidad func, aqui una lista de las funcionalidades soportadas por el help:\n");
    
    
    printf("\nTotal de puntos: 9.5 puntos\n\n");

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