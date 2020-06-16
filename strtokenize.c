#include "strtokenize.h"

int __strtokenize( char *str, char *tokens[], char *delim )
{
    if ( !str || !delim )
        return -1;

    char *tok = strtok( str, delim );
    char *copy = NULL;
    int i = 0;

    while ( tok != NULL ) {
        copy = strdup( tok );
        if ( !copy )
            goto __die;
            
        tok = strtok( NULL, delim );
        tokens[i++] = copy;
    }
    return i;

    __die:
        for ( int j = 0 ; j < i ; j++ )
            free( tokens[j] );
        return -1;
}