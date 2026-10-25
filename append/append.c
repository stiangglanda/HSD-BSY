#include <sys/stat.h>   // File status and permissions
#include <fcntl.h>      // File I/O
#include <unistd.h>     // File descriptors
#include <string.h>     // strlen

#define BUFFER_SIZE 512     // Read/write buffer size
#define NUMBER_BUF_SIZE 32  // Number buffer size

static short const EXIT_FAILURE = 1;
static short const EXIT_SUCCESS = 0;
static int const ERROR_RC = -1;         // I/O error value
static int const ARG_COUNT = 3;         // Program name and two file names
static int const ARG_SOURCE = 1;        // Source file index in argv
static int const ARG_DEST = 2;          // Destination file index in argv
static size_t const DECIMAL_BASE = 10;  // Decimal base

static char const * const USAGE_CMD = "usage: append source dest\n";
static char const * const ERR_READ = "error in read\n";
static char const * const ERR_OPEN_SRC = "error in open source file\n";
static char const * const ERR_OPEN_DEST = "error in open destination file\n";
static char const * const ERR_CLOSE = "error in close\n";
static char const * const ERR_WRITE = "error in write\n";
static char const * const ERR_STAT = "error in fstat\n";
static char const * const ERR_SAME = "source and destination are the same file\n";


// Prints a number to STDOUT
int print_number(size_t num)
{
    char buf[NUMBER_BUF_SIZE];
    int i = 0;

    if (num == 0)
    {
        if (write(STDOUT_FILENO, "0", 1) == ERROR_RC)
        {
            return ERROR_RC;
        }

        return 0;
    }

    while (num > 0)
    {
        buf[i++] = (char)('0' + (num % DECIMAL_BASE));
        num /= DECIMAL_BASE;
    }

    // Print digits in reverse order
    for (int j = i - 1; j >= 0; j--)
    {
        if (write(STDOUT_FILENO, &buf[j], 1) == ERROR_RC)
        {
            return ERROR_RC;
        }
    }
    return 0;
}


// Appends the source file to the destination file
int main(int argc, char** argv) 
{
    // Validate arguments.
    if(argc != ARG_COUNT)
    {
        write(STDERR_FILENO, USAGE_CMD, strlen(USAGE_CMD));
        return EXIT_FAILURE;
    }

    int const source_fd = open(argv[ARG_SOURCE], O_RDONLY);
    if(source_fd == ERROR_RC)
    {
        write(STDERR_FILENO, ERR_OPEN_SRC, strlen(ERR_OPEN_SRC));
        return EXIT_FAILURE;
    }

    // Open or create the destination file
    mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
        
    int const dest_fd = open(argv[ARG_DEST], O_WRONLY | O_CREAT | O_APPEND, mode);
    if(dest_fd == ERROR_RC)
    {
        write(STDERR_FILENO, ERR_OPEN_DEST, strlen(ERR_OPEN_DEST));
        close(source_fd);
        return EXIT_FAILURE;
    }

    // Prevent appending a file to itself
    struct stat source_info;
    struct stat dest_info;
    if(fstat(source_fd, &source_info) == ERROR_RC || fstat(dest_fd, &dest_info) == ERROR_RC)
    {
        write(STDERR_FILENO, ERR_STAT, strlen(ERR_STAT));
        close(source_fd);
        close(dest_fd);
        return EXIT_FAILURE;
    }
    if(source_info.st_dev == dest_info.st_dev && source_info.st_ino == dest_info.st_ino)
    {
        write(STDERR_FILENO, ERR_SAME, strlen(ERR_SAME));
        close(source_fd);
        close(dest_fd);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = 0;
    ssize_t bytesWrittenTotal = 0;
    while((bytesRead = read(source_fd, buffer, sizeof(buffer))) > 0) 
    {
        // Complete partial writes
        ssize_t bytesWritten = 0;
        while(bytesWritten < bytesRead)
        {
            ssize_t const n = write(dest_fd, buffer + bytesWritten, (size_t)(bytesRead - bytesWritten));
            if(n == ERROR_RC)
            {
                write(STDERR_FILENO, ERR_WRITE, strlen(ERR_WRITE));
                close(source_fd);
                close(dest_fd);
                return EXIT_FAILURE;
            }
            bytesWritten += n;
        }
        bytesWrittenTotal += bytesWritten;
    }

    if (bytesRead == ERROR_RC)
    {
        write(STDERR_FILENO, ERR_READ, strlen(ERR_READ));
        close(source_fd);
        close(dest_fd);
        return EXIT_FAILURE;
    }

    // Close the source file
    if(close(source_fd) == ERROR_RC)
    {
        write(STDERR_FILENO, ERR_CLOSE, strlen(ERR_CLOSE));
        close(dest_fd);
        return EXIT_FAILURE;
    }

    if(close(dest_fd) == ERROR_RC)
    {
        write(STDERR_FILENO, ERR_CLOSE, strlen(ERR_CLOSE));
        return EXIT_FAILURE;
    }

    if(print_number((size_t)bytesWrittenTotal) == ERROR_RC ||
       write(STDOUT_FILENO, "\n", 1) == ERROR_RC)
    {
        write(STDERR_FILENO, ERR_WRITE, strlen(ERR_WRITE));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}