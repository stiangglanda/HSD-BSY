#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>

#define MAX_SEGMENTS 10
#define LINE_SIZE 1024

// return codes
static short const OK = 0;
static short const NOK = 1;

// error messages
static char const* const cErrOpen = "error in fopen";
static char const* const cErrClose = "error in fclose";
static char const* const cErrMemory = "error allocating memory";
static char const* const cErrSegment = "invalid segment";
static char const* const cErrTooMany = "too many segments (max 10)";
static char const* const cErrUsage = "usage: lineout [segments] fileName";

// one segment of lines
struct Segment {
    long start;
    long end;
};

// reads a number returns NOK if invalid
static short parseNumber(char const* text, char** rest, long* value)
{
    errno = 0;
    *value = strtol(text, rest, 10);
    if (*rest == text || errno != 0 || *value < 1) {
        return NOK;
    }
    return OK;
}

// parses one part like "3", "4-9", "14-" or "-20" returns NOK if invalid
static short parseSegment(char const* text, struct Segment* segment)
{
    assert(text != NULL && segment != NULL);
    char* rest;

    if (text[0] == '-') {                          // form "-20"
        segment->start = 1;
        if (parseNumber(text + 1, &rest, &segment->end) != OK) {
            return NOK;
        }
    } else {
        if (parseNumber(text, &rest, &segment->start) != OK) {
            return NOK;
        }
        if (*rest == '\0') {                       // form "3"
            segment->end = segment->start;
        } else if (*rest == '-') {
            rest++;                                // skip the dash
            if (*rest == '\0') {                   // form "14-"
                segment->end = LONG_MAX;
            } else if (parseNumber(rest, &rest, &segment->end) != OK) {
                return NOK;                        // form "4-9" was invalid
            }
        } else {
            return NOK;                            // invalid
        }
    }

    // nothing may follow the last number and start must not be after end
    if (*rest != '\0' || segment->start > segment->end) {
        return NOK;
    }
    return OK;
}

// splits text like "3,4-9,14-" into at most max segments
static short parseSegments(char const* text, struct Segment* segments,
                           size_t max, size_t* count)
{
    assert(text != NULL && segments != NULL && count != NULL);
    *count = 0;

    // strtok changes the text, so we work on a copy
    size_t const len = strlen(text);
    char* copy = malloc(len + 1);
    if (copy == NULL) {
        perror(cErrMemory);
        return NOK;
    }
    memcpy(copy, text, len + 1);

    short result = OK;
    for (char* token = strtok(copy, ","); token != NULL; token = strtok(NULL, ",")) {
        if (*count >= max) {
            fprintf(stderr, "%s\n", cErrTooMany);
            result = NOK;
            break;
        }
        if (parseSegment(token, &segments[*count]) != OK) {
            fprintf(stderr, "%s: %s\n", cErrSegment, token);
            result = NOK;
            break;
        }
        (*count)++;
    }

    // for example an empty text
    if (result == OK && *count == 0) {
        fprintf(stderr, "%s\n", cErrSegment);
        result = NOK;
    }

    free(copy);
    return result;
}

// prints every line inside a segment, the file is read only once
static short printLines(char const* fileName, struct Segment const* segments,
                        size_t count)
{
    assert(fileName != NULL && segments != NULL);

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror(cErrOpen);
        return NOK;
    }

    char line[LINE_SIZE];                          // array, so sizeof is correct
    long number = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        number++;
        for (size_t i = 0; i < count; i++) {
            if (number >= segments[i].start && number <= segments[i].end) {
                printf("%3ld %s", number, line);
                break;                             // print the line only once
            }
        }
    }

    if (fclose(file) != 0) {
        perror(cErrClose);
        return NOK;
    }
    return OK;
}

int main(int argc, char** argv)
{
    struct Segment segments[MAX_SEGMENTS];
    size_t count = 0;
    char const* fileName;

    if (argc == 3) {
        if (parseSegments(argv[1], segments, MAX_SEGMENTS, &count) != OK) {
            return EXIT_FAILURE;
        }
        fileName = argv[2];
    } else if (argc == 2) {
        // no segments given, so print the whole file
        segments[0].start = 1;
        segments[0].end = LONG_MAX;
        count = 1;
        fileName = argv[1];
    } else {
        fprintf(stderr, "%s\n", cErrUsage);
        return EXIT_FAILURE;
    }

    return printLines(fileName, segments, count) == OK ? EXIT_SUCCESS : EXIT_FAILURE;
}