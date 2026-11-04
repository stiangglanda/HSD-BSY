#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// unterschiedliche Ausgabefunktionen
static void PrintArgs(char* const* argv, int const argc) {
    assert(argv != NULL && argc > 0);

    fprintf(stdout, "number = %d\n", argc);
    for (int i = 0; i < argc; ++i) {
        fputs(argv[i], stdout);
        fputc('\n', stdout);
        puts(argv[i]);
    }

    char out[257] = {};

    sprintf(out, "address of param = %p\n", (void*)&argc);
    fprintf(stderr, "%s", out);
    fputs(argv[0], stdout);
}

int main(int argc, char *argv[]) {
    puts("Hello VS Code!");
    PrintArgs(argv, argc);

    //stdout soll vollgepuffert werden
    //es wird ein interner Puffer mit entsprechender Groesze
    //erstellt.
    //Param 2 und Param 4 werdn fuer einen eigenen Puffer benoetigt.
    setvbuf(stdout, NULL, _IOFBF, 0);
    PrintArgs(argv, argc);

    //stdout wird nicht gepuffert
    setvbuf(stdout, NULL, _IONBF, 0);
    PrintArgs(argv, argc);

    //retour zu Zeilenpufferung -> standard eingestellt
    setvbuf(stdout, NULL, _IOLBF, 0); //line buffering

    return EXIT_SUCCESS;
}