#include <sys/stat.h>   // mode_t -> Zugriffsrechte, umask
#include <fcntl.h>      // fuer Elem IO Funktionen (open, O_RDONLY, etc.)
#include <unistd.h>     // fuer Filedeskriptoren und Systemcalls (read, write, close)
#include <string.h>     // fuer strlen

static short const EXIT_FAILURE = 1;
static short const EXIT_SUCCESS = 0;

static char const * const USAGE_CMD      = "usage: ./append src_file dst_file\n";
static char const * const ERR_OPEN_SRC   = "error: cannot open source file\n";
static char const * const ERR_OPEN_DST   = "error: cannot open/create destination file\n";
static char const * const ERR_READ       = "error in read\n";
static char const * const ERR_WRITE      = "error in write\n";
static char const * const ERR_CLOSE      = "error in close\n";
static char const * const MSG_APPENDED   = "Appended bytes: ";

static size_t const BUFFER_SIZE = 512;

static void print_number(size_t num)
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
    if (argc != 3)
    {
        write(STDERR_FILENO, USAGE_CMD, strlen(USAGE_CMD));
        return EXIT_FAILURE;
    }

    int const fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1)
    {
        write(STDERR_FILENO, ERR_OPEN_SRC, strlen(ERR_OPEN_SRC));
        return EXIT_FAILURE;
    }

    mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    int const fd_dst = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, mode);
    if (fd_dst == -1)
    {
        write(STDERR_FILENO, ERR_OPEN_DST, strlen(ERR_OPEN_DST));
        close(fd_src);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = 0;
    size_t totalBytesAppended = 0;

    while ((bytesRead = read(fd_src, buffer, BUFFER_SIZE)) > 0)
    {
        ssize_t bytesWritten = 0;
        // Schleife stellt sicher, dass alle gelesenen Bytes geschrieben werden
        while (bytesWritten < bytesRead)
        {
            ssize_t const res = write(fd_dst, buffer + bytesWritten, (size_t)(bytesRead - bytesWritten));
            if (res == -1)
            {
                write(STDERR_FILENO, ERR_WRITE, strlen(ERR_WRITE));
                close(fd_src);
                close(fd_dst);
                return EXIT_FAILURE;
            }
            bytesWritten += res;
        }
        totalBytesAppended += (size_t)bytesRead;
    }

    if (bytesRead == -1)
    {
        write(STDERR_FILENO, ERR_READ, strlen(ERR_READ));
        close(fd_src);
        close(fd_dst);
        return EXIT_FAILURE;
    }

    if (close(fd_src) == -1 || close(fd_dst) == -1)
    {
        write(STDERR_FILENO, ERR_CLOSE, strlen(ERR_CLOSE));
        return EXIT_FAILURE;
    }

    write(STDOUT_FILENO, MSG_APPENDED, strlen(MSG_APPENDED));
    print_number(totalBytesAppended);
    write(STDOUT_FILENO, "\n", 1);

    return EXIT_SUCCESS;
}