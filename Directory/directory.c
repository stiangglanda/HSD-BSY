void ChangeToDir (char const* const path){
   if(chdir(path) != 0)
   {
      perror(cErrChangeDir);
      exit(EXIT_FAILURE);
   }
}

void PrintDir(){
    
}