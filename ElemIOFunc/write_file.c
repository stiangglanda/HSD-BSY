#include <sys/stat.h>   //mode_t -> Zugriffsrechte
#include <fcntl.h>      // fuer Elem IO Funktionen
#include <unistd.h>     // fuer Filedeskriptoren
#include <string.h>     // fuer strlen

static short const EXIT_FAILURE = 1;
static short const EXIT_SUCCESS = 0;

static char const * const USAGE_CMD = "usage: command filename\n";
static char const * const ERR_OPEN = "error in open\n";
static char const * const ERR_CLOSE = "error in close\n";
static char const * const ERR_WRITE = "error in write\n";

int main(int argc, char** argv) 
{
    //das Setzten der Einschrenkungsmaske auf 0 ermöglicht dem
    //Prozess alle Zugriffsrechte fuer eine Datei zu setzen.
    umask(0); // ->kommt spaeter noch genauer

    //Argumente pruefen
    if(argc != 2)
    {
        write(STDERR_FILENO, USAGE_CMD, strlen(USAGE_CMD));
        return EXIT_FAILURE;
    }

    //Datei erzeugen mit bestimmten Zugriffsrechte
    mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

    int const fd = open(argv[1], O_WRONLY | O_EXCL | O_CREAT, mode);
    if(fd == -1)
    {
        write(STDERR_FILENO, ERR_OPEN, strlen(ERR_OPEN));
        return EXIT_FAILURE;
    }

    // einfache Daten in die Datei schreiben
    char const* buffer = "Hello WSL-World!\n";
    ssize_t bytesWritton = write(fd, buffer, strlen(buffer));

    if(bytesWritton == -1)
    {
        write(STDERR_FILENO, ERR_WRITE, strlen(ERR_WRITE));
        close(fd);  //keine zusätzliche Fehlerpruefung notwendig
        return EXIT_FAILURE;
    }

    if(close(fd) == -1)
    {
        write(STDERR_FILENO, ERR_CLOSE, strlen(ERR_CLOSE));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}