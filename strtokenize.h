#ifndef __STRTOKENIZE
#define __STRTOKENIZE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Split a stirng into a number of tokens
 * @param char *str - The string to tokenize
 * @param char *delim - Delimeter
 * @param int *n - A place to store the number of tokens generated
 */

int __strtokenize( char *str, char *tokens[], char *delim );

#endif