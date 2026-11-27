#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

// valgrind --leak-check=full ./ScanDir /home

//Fehlerkonstanten und -texte
static short const SEL_OK = 1;
static short const SEL_NOK = 0;
static short const GREATER = 1;
static short const EQUAL = 0;
static short const LESS = -1;

static char const* const cUsageCmd = "usage: scandir [directory]\n";
static char const* const cErrScanDir = "error in scandir";


//Implementierung einer Select-Funktion: Filtert den aktuellen (.) und den
//uebergeordneten Verzeichniseintrag (..) heraus
static int Select(struct dirent const* dir)
{
   assert(dir != 0);

   //to do #2
   if(strcmp(dir->d_name, ".") == EQUAL || strcmp(dir->d_name, "..") == EQUAL)
   {
      return SEL_NOK;
   }
   return SEL_OK;
}


//selektiert alle Dateien im Verzeichnis
static int SelectFiles(struct dirent const* dir)
{
   assert(dir != 0);
   return (dir->d_type == DT_REG) ? SEL_OK : SEL_NOK;
}


//Implementierung einer Sortierfunktion: sortiert die Eintraege umgekehrt
//nach der Laenge des Dateinamens
//allgemein: left < right -> -1
//           left = right ->  0
//           left > right ->  1
static int SortByLen(struct dirent const** left,
                     struct dirent const** right)
{
   assert(left != 0 && right != 0 && *left != 0 && *right != 0);

   //to do #4
   int const lenLeft = (int)strlen((*left)->d_name);
   int const lenRight = (int)strlen((*right)->d_name);

   return lenRight - lenLeft;
}


int main (int argc, char **argv)
{
   char const * dirName = ".";  //aktuelles Working-Verzeichnis;

   if (argc > 2)
   {
      fputs(cUsageCmd,stderr);
      return EXIT_FAILURE;
   }

   //lesen des Kommandozeilen-Parameters
   if (argc == 2)
   {
      dirName = argv[1];
   }

   //Lesen des angegebenen Verzeichnisses
   errno = 0;
   struct dirent **namelist = 0;

   //to do #1
   // Variante 1
   //int numOfEntries = scandir(dirName, &namelist, nullptr, alphasort);

   // Variante 2 -> mit function pointer auf 'Select'
   //int numOfEntries = scandir(dirName, &namelist, Select, alphasort);

   // Variante 3 -> mit function pointer auf 'Select' und SortByLen
   int numOfEntries = scandir(dirName, &namelist, Select, SortByLen);

   // Variante 4 -> mit function pointer auf 'SelectFiles' und SortByLen
   int numOfEntries = scandir(dirName, &namelist, SelectFiles, SortByLen);
   if(errno){
      perror(cErrScanDir);
      return EXIT_FAILURE;
   }

   printf("Anzahl der Eintraege: %s: %d\n", dirName, numOfEntries);

   //Ausgabe der Eintraege
   if(numOfEntries > 0)
   {
      puts("Die Eintraege:");
      for(int i = 0; i < numOfEntries; ++i)
      {
         printf(" %s -> %ld\n", namelist[i]->d_name, namelist[i]->d_ino);
         free(namelist[i]);
      }
      free(namelist);
   }

   return EXIT_SUCCESS;
}
