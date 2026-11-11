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

static void EmptyStdIn(){
    int ch = EOF;
    do{
        ch = getchar();
    }
    while(ch != EOF && ch != '\n');
}

static void ScanFromStdIn(){
    char const* cErrMsg = "Error reading ...";
    size_t const cSize = 5;
    char buffer[cSize] = {};

    puts("\nread from stdin");
    int ret = scanf("%s", buffer);
    buffer[cSize] = '\0';
    ret == 1 ? puts(buffer) : puts(cErrMsg);

    // noch ein lesevorgang
    EmptyStdIn();

    int const cLineSize = 20;
    char line[cLineSize] = {};
    puts("\nread line from stdin");
    if(fgets(line, cLineSize, stdin) == NULL){
        perror("error fgets");
        return;
    }

    memset(buffer, 0, cSize);
    ret = scanf(line, "%4s", buffer);
    
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