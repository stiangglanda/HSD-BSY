#ifndef TEST_H
#define TEST_H

#define TEST_OK  0
#define TEST_NOK 1

// All tests return TEST_OK or TEST_NOK
int TestFormatIO(); // removed extern tests printf, fgets, sscanf 

int TestString(); // tests string functions

int TestDynMem(); // tests malloc, free

int TestStruct(); // tests struct declaration and usage

int TestArray(); // tests array initialization and printing

int TestFuncPtr(); // tests function pointers, qsort

#endif