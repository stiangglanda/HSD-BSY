#include "directory.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

//error messages
static char const* const cErrOpenDir  = "error in opendir";
static char const* const cErrAddFile  = "error in fopen";
static char const* const cErrAddDir   = "error in mkdir";
static char const* const cErrChangeDir = "error in chdir";
static char const* const cErrReadDir  = "error in readdir!";
static char const* const cErrCloseDir = "error in closedir!";

static mode_t cDefaultAccessRights = S_IRUSR | S_IWUSR | S_IXUSR; // Default access rights for new directories and files

static void remove_directory(const char *path)
{
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip current and parent directory
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Construct full path
        char full_path[PATH_MAX];

        snprintf(full_path, sizeof(full_path),
                 "%s/%s", path, entry->d_name);

        struct stat st;

        if (lstat(full_path, &st) == -1)
        {
            perror("lstat");
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            // Recursively remove subdirectory
            remove_directory(full_path);

            // Remove now-empty directory
            if (rmdir(full_path) == -1)
            {
                perror("rmdir");
            }
        }
        else
        {
            // Remove file (also handles symbolic links)
            if (unlink(full_path) == -1)
            {
                perror("unlink");
            }
        }
    }

    closedir(dir);
}

void ChangeToDir (char const* const path){
   if(chdir(path) != 0)
   {
      perror(cErrChangeDir);
      exit(EXIT_FAILURE);
   }
}

void PrintDir(){
   char* dirName = ".";  //current working direktory


   //explizites Ruecksetzten der errno-Variable vor einem Systemaufruf
   errno = 0;

   //Verzeichnis oeffnen
   DIR* pDir = opendir(dirName);
   if (pDir == nullptr)
   {
      perror(cErrOpenDir);
   }

   //lesen der einzelnen Eintraege
   errno = 0;
   struct dirent* pDirEntry = nullptr;
   while ((pDirEntry = readdir(pDir)) != nullptr)
   {
      switch (pDirEntry->d_type)
      {
         case DT_REG:
            printf("file:  ");
            break;
         case DT_DIR:
            printf("dir:   ");
            break;
         default:
            printf("other: ");
            break;
      }
      printf("%20s [inode: %8ld] [len: %d]\n", pDirEntry->d_name, 
                                               (long)pDirEntry->d_ino, 
                                               pDirEntry->d_reclen);
   }

   //es koennte auch ein Lesefehler passiert sein -> errno pruefen
   if (errno != 0)
   {
      perror(cErrReadDir);
      errno = 0;
      closedir(pDir);
   }

   //Verzeichnis schliessen
   if (closedir(pDir) != 0)
   {
      perror(cErrCloseDir);
   }
}

void AddDir (char const* const name){
   if(mkdir(name, cDefaultAccessRights) != 0)
   {
      perror(cErrAddDir);
      exit(EXIT_FAILURE);
   }
}

void AddFile (char const* const name){
   FILE* pFile = fopen(name, "w");
   if(pFile == nullptr)
   {
      perror(cErrAddFile);
      exit(EXIT_FAILURE);
   }
   fclose(pFile);
}

void RemoveAll(){
   //to do
   remove_directory(".");
}

void SetAccessRights (mode_t const mode) {
   cDefaultAccessRights = mode;
}