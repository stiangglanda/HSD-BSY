#include <sys/stat.h>   //mode_t -> Zugriffsrechte
#include <fcntl.h>      // fuer Elem IO Funktionen
#include <unistd.h>     // fuer Filedeskriptoren
#include <string.h>     // fuer strlen

static short const EXIT_FAILURE = 1;
static short const EXIT_SUCCESS = 0;

static char const * const USAGE_CMD = "usage: command filename\n";
static char const * const ERR_READ = "error in read\n";
static char const * const ERR_OPEN = "error in open\n";
static char const * const ERR_CLOSE = "error in close\n";
static char const * const ERR_WRITE = "error in write\n";

void print_number(size_t num)
{
    char buf[32];
    int i = 0;

    if (num == 0)
    {
        write(STDOUT_FILENO, "0", 1);
        return;
    }

    while (num > 0)
    {
        buf[i++] = (char)('0' + (num % 10));
        num /= 10;
    }

    // Von hinten nach vorne auf STDOUT ausgeben
    for (int j = i - 1; j >= 0; j--)
    {
        write(STDOUT_FILENO, &buf[j], 1);
    }
}

int main(int argc, char** argv) 
{
    //Argumente pruefen
    if(argc != 3)
    {
        write(STDERR_FILENO, USAGE_CMD, strlen(USAGE_CMD));
        return EXIT_FAILURE;
    }

    int const source_fd = open(argv[1], O_RDONLY);
    if(source_fd == -1)
    {
        write(STDERR_FILENO, ERR_READ, strlen(ERR_READ));
        return EXIT_FAILURE;
    }

    //Datei erzeugen mit bestimmten Zugriffsrechte
    mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
        
    int const dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, mode); // append mode
    if(dest_fd == -1)
    {
        write(STDERR_FILENO, ERR_OPEN, strlen(ERR_OPEN));
        return EXIT_FAILURE;
    }

    char buffer[512];
    ssize_t bytesRead = 0;
    ssize_t bytesWrittenTotal = 0;
    while((bytesRead = read(source_fd, buffer, sizeof(buffer))) > 0) 
    {
        ssize_t bytesWritton = write(dest_fd, buffer, bytesRead);

        if(bytesWritton == -1)
        {
            write(STDERR_FILENO, ERR_WRITE, strlen(ERR_WRITE));
            close(dest_fd);  //keine zusätzliche Fehlerpruefung notwendig
            return EXIT_FAILURE;
        }
        bytesWrittenTotal += bytesWritton;
    }

    if (bytesRead == -1)
    {
        write(STDERR_FILENO, ERR_READ, strlen(ERR_READ));
        return EXIT_FAILURE;
    }

    if(close(source_fd) == -1)
    {
        write(STDERR_FILENO, ERR_CLOSE, strlen(ERR_CLOSE));
        return EXIT_FAILURE;
    }

    if(close(dest_fd) == -1)
    {
        write(STDERR_FILENO, ERR_CLOSE, strlen(ERR_CLOSE));
        return EXIT_FAILURE;
    }

    print_number(bytesWrittenTotal);
    write(STDOUT_FILENO, "\n", 1);

    return EXIT_SUCCESS;
}