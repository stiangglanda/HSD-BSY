#ifndef PRINT_H
#define PRINT_H

// Prints text OK|NOK
void PrintResult(char const * const text, unsigned const errorCode );

// Prints header with a = underline
void PrintHeader(char const header[]);

// Prints the string array
void PrintStrArr(char const * const * const arr, unsigned const len);

// Prints the int array
void PrintIntArr(int const * const arr, unsigned const len); // const added, len unsigned

#endif