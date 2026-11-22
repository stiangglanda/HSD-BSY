#include "directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// Error messages
static char const* const cErrOpenDir   = "error in opendir";
static char const* const cErrAddFile   = "error in open";
static char const* const cErrAddDir    = "error in mkdir";
static char const* const cErrChangeDir = "error in chdir";
static char const* const cErrReadDir   = "error in readdir";
static char const* const cErrCloseDir  = "error in closedir";
static char const* const cErrLstat     = "error in lstat";
static char const* const cErrCloseFile = "error in close";
static char const* const cErrRemoveDir = "error in rmdir";
static char const* const cErrUnlink    = "error in unlink";

static mode_t cDefaultAccessRights = S_IRUSR | S_IWUSR | S_IXUSR;

// Internal recursive functions
static void print_directory(const char* path, int depth);
static int remove_directory(const char* path);


void ChangeToDir(char const* const path)
{
    if (chdir(path) != 0)
    {
        perror(cErrChangeDir);
    }
}


void PrintDir()
{
    print_directory(".", 0);
}


static void print_directory(const char* path, int depth)
{
    DIR* pDir = opendir(path);
    if (pDir == NULL)
    {
        perror(cErrOpenDir);
        return;
    }

    struct dirent* pDirEntry;

    while (1)
    {
        errno = 0;
        pDirEntry = readdir(pDir);

        if (pDirEntry == NULL)
        {
            if (errno != 0)
            {
                perror(cErrReadDir);
            }
            break;
        }

        if (strcmp(pDirEntry->d_name, ".") == 0 ||
            strcmp(pDirEntry->d_name, "..") == 0)
        {
            continue;
        }

        char fullPath[PATH_MAX];

        int length = snprintf(fullPath, sizeof(fullPath),
                              "%s/%s", path, pDirEntry->d_name);

        if (length < 0 || (size_t)length >= sizeof(fullPath))
        {
            fprintf(stderr, "Path too long\n");
            continue;
        }

        struct stat st;

        if (lstat(fullPath, &st) != 0)
        {
            perror(cErrLstat);
            continue;
        }

        for (int i = 0; i < depth; i++)
        {
            printf("    ");
        }

        if (S_ISDIR(st.st_mode))
        {
            printf("dir:   %s\n", pDirEntry->d_name);

            print_directory(fullPath, depth + 1);
        }
        else if (S_ISREG(st.st_mode))
        {
            printf("file:  %s\n", pDirEntry->d_name);
        }
        else
        {
            printf("other: %s\n", pDirEntry->d_name);
        }
    }

    if (closedir(pDir) != 0)
    {
        perror(cErrCloseDir);
    }
}


void AddDir(char const* const name)
{
    if (mkdir(name, cDefaultAccessRights) != 0)
    {
        perror(cErrAddDir);
    }
}


void AddFile(char const* const name)
{
    int fd = open(name, O_CREAT | O_WRONLY | O_TRUNC,
                  cDefaultAccessRights);

    if (fd == -1)
    {
        perror(cErrAddFile);
        return;
    }

    if (close(fd) != 0)
    {
        perror(cErrCloseFile);
    }
}


void RemoveAll()
{
    remove_directory(".");
}


static int remove_directory(const char* path)
{
    DIR* pDir = opendir(path);
    if (pDir == NULL)
    {
        perror(cErrOpenDir);
        return -1;
    }

    int result = 0;
    struct dirent* pDirEntry;

    while (1)
    {
        errno = 0;
        pDirEntry = readdir(pDir);

        if (pDirEntry == NULL)
        {
            if (errno != 0)
            {
                perror(cErrReadDir);
                result = -1;
            }
            break;
        }

        if (strcmp(pDirEntry->d_name, ".") == 0 ||
            strcmp(pDirEntry->d_name, "..") == 0)
        {
            continue;
        }

        char fullPath[PATH_MAX];

        int length = snprintf(fullPath, sizeof(fullPath),
                              "%s/%s", path, pDirEntry->d_name);

        if (length < 0 || (size_t)length >= sizeof(fullPath))
        {
            fprintf(stderr, "Path too long\n");
            result = -1;
            continue;
        }

        struct stat st;

        if (lstat(fullPath, &st) != 0)
        {
            perror(cErrLstat);
            result = -1;
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            if (remove_directory(fullPath) != 0)
            {
                result = -1;
                continue;
            }

            if (rmdir(fullPath) != 0)
            {
                perror(cErrRemoveDir);
                result = -1;
            }
        }
        else
        {
            if (unlink(fullPath) != 0)
            {
                perror(cErrUnlink);
                result = -1;
            }
        }
    }

    if (closedir(pDir) != 0)
    {
        perror(cErrCloseDir);
        result = -1;
    }

    return result;
}


void SetAccessRights(mode_t const mode)
{
    cDefaultAccessRights = mode;
}
