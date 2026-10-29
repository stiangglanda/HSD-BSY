#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <limits.h>
#include <assert.h>

//error code
static short const OK = 0;
static short const NOK = 1;

//error and info messages
static char const* const cErrOpen = "error in fopen";
static char const* const cErrClose = "error in fclose";
static char const* const cErrWrite = "error in fwrite";
static char const* const cErrRead = "error in fgetc";
static char const* const cErrUsage = "error in commandline -> \
./readWrite fileName";
static char const* const cFileEnd = "end of file reached";

struct Abschnitt { 
    long von; 
    long bis; 
};

struct Abschnitt abschnitte[10];
static size_t abschnitte_index = 0;

void printAbschnitt(struct Abschnitt* abschnitt) {
    if (abschnitt->bis == LONG_MAX) {
        printf("Abschnitt: %ld bis unbegrenzt\n", abschnitt->von);
    } else if (abschnitt->von == abschnitt->bis) {
        printf("Abschnitt: %ld\n", abschnitt->von);
    } else {
        printf("Abschnitt: %ld bis %ld\n", abschnitt->von, abschnitt->bis);
    }
}

struct Abschnitt parseSegment(char const* const segment) 
{
    struct Abschnitt abschnitt;
    char *ende;
    if (segment[0] == '-') {              // Form "-20"
        abschnitt.von = 1;
        abschnitt.bis = strtol(segment + 1, &ende, 10);
    } else {
        abschnitt.von = strtol(segment, &ende, 10);
        if (*ende == '\0')             // Form "3"
            abschnitt.bis = abschnitt.von;
        else if (*ende == '-' && *(ende + 1) == '\0')   // Form "14-"
            abschnitt.bis = LONG_MAX;
        else if (*ende == '-')         // Form "4-9"
            abschnitt.bis = strtol(ende + 1, &ende, 10);
    }

    printAbschnitt(&abschnitt);
    return abschnitt;
}

void parseSegments(char const* const segments) {
    size_t const len = strlen(segments);
    char* input = malloc(len + 1);
    if (input == NULL) {
        perror("error allocating memory");
        return;
    }
    memcpy(input, segments, len + 1);

    // Returns first token
    char* token = strtok(input, " , ");

    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL) {
        printf(" %s\n", token);
        abschnitte[abschnitte_index++]=parseSegment(token);
        token = strtok(NULL, " , ");
    }

    free(input);
}

void printLines(char const* const fileName) {
    assert(fileName != NULL);

    FILE* file = fopen(fileName, "r");
    if(file == NULL)
    {
        perror(cErrOpen);
        return;
    }

    char* zeile = malloc(1024);

    long nr = 0;
    while (fgets(zeile, sizeof(zeile), file) != NULL) {
        nr++;
        for (int i = 0; i < abschnitte_index; i++) {
            if (nr >= abschnitte[i].von && nr <= abschnitte[i].bis) {
                printf("%3ld %s", nr, zeile);
                break;   // Zeile nur einmal ausgeben
            }
        }
    }

    free(zeile);
}


//main program
int main(int argc, char** argv)
{
    if(argc == 3) {
        parseSegments(argv[1]);
        printf("%s\n", argv[2]);
    } else if(argc == 2) {
        printf("%s\n", argv[1]);
    } else {
        fprintf(stderr, "%s\n", cErrUsage);
        return EXIT_FAILURE;
    }

    printLines(argv[2]);

   return EXIT_SUCCESS;
}
