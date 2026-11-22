#include "directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

static int passed = 0;
static int failed = 0;

#define TEST(name, condition)                  \
    do {                                       \
        if (condition) {                       \
            printf("[PASS] %s\n", name);       \
            passed++;                          \
        } else {                               \
            printf("[FAIL] %s\n", name);       \
            failed++;                          \
        }                                      \
    } while (0)

static int exists(const char* path)
{
    struct stat st;
    return stat(path, &st) == 0;
}

int main(void)
{
    char originalDir[PATH_MAX];
    char testDir[] = "directory_tests";

    if (getcwd(originalDir, sizeof(originalDir)) == NULL ||
        mkdir(testDir, 0700) == -1)
    {
        perror("Test setup");
        return EXIT_FAILURE;
    }

    chdir(testDir);

    printf("--- Directory Module Tests ---\n\n");

    // Directory and file creation
    AddDir("test_dir");
    TEST("Create directory", exists("test_dir"));

    AddFile("test.txt");
    TEST("Create file", exists("test.txt"));

    // Nested directories
    AddDir("test_dir/level1");
    AddDir("test_dir/level1/level2");
    AddFile("test_dir/level1/test.txt");

    TEST("Create nested directories",
         exists("test_dir/level1/level2"));

    TEST("Create file in nested directory",
         exists("test_dir/level1/test.txt"));

    // Change directory
    ChangeToDir("test_dir");

    char cwd[4096];
    getcwd(cwd, sizeof(cwd));

    TEST("ChangeToDir", strstr(cwd, "test_dir") != NULL);

    ChangeToDir("..");

    // Print directory
    printf("\n--- PrintDir ---\n");
    PrintDir();
    printf("--- End PrintDir ---\n\n");

    // Duplicate directory
    AddDir("test_dir");
    TEST("Duplicate directory", exists("test_dir"));

    // File truncation
    int fd = open("test.txt", O_WRONLY | O_TRUNC);
    if (fd >= 0) {
        write(fd, "test content", 12);
        close(fd);
    }

    AddFile("test.txt");

    struct stat st;
    TEST("Existing file gets truncated",
         stat("test.txt", &st) == 0 && st.st_size == 0);

    // Access rights
    mode_t oldMask = umask(0);

    SetAccessRights(0700);
    AddDir("permission_dir");
    AddFile("permission.txt");

    TEST("Directory permissions",
         stat("permission_dir", &st) == 0 &&
         (st.st_mode & 0777) == 0700);

    TEST("File permissions",
         stat("permission.txt", &st) == 0 &&
         (st.st_mode & 0777) == 0700);

    umask(oldMask);

    // Symbolic link
    char externalPath[PATH_MAX + sizeof("/external.txt")];
    int externalPathLength = snprintf(externalPath, sizeof(externalPath),
                                      "%s/external.txt", originalDir);

    if (externalPathLength < 0 ||
        (size_t)externalPathLength >= sizeof(externalPath))
    {
        fprintf(stderr, "External path is too long\n");
        chdir(originalDir);
        return EXIT_FAILURE;
    }

    fd = open(externalPath, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd >= 0)
        close(fd);

    AddDir("link_dir");
    symlink(externalPath, "link_dir/external_link");

    TEST("Create symbolic link", lstat("link_dir/external_link", &st) == 0 &&
         S_ISLNK(st.st_mode));

    // Remove everything recursively
    RemoveAll();

    TEST("RemoveAll removes files", !exists("test.txt"));
    TEST("RemoveAll removes nested directories", !exists("test_dir"));
    TEST("RemoveAll removes symbolic links", !exists("link_dir"));
    TEST("RemoveAll removes permission files", !exists("permission.txt"));

    TEST("External symlink target remains",
         exists(externalPath));

    // Cleanup
    chdir(originalDir);
    rmdir(testDir);
    unlink(externalPath);

    printf("\n--- Results ---\n");
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
