#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

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
}


//selektiert alle Dateien im Verzeichnis
static int SelectFiles(struct dirent const* dir)
{
   assert(dir != 0);

  //to do #3
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


   return EXIT_SUCCESS;
}
