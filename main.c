#include "pidmap.h"

static void __die( const char *str ) {
    printf( "%s\n", str );
    exit( 0x02 );
}

int main( int argc, char **argv )
{
    pid_t pid = getpid();
    
    struct vma_list *vma_list, *head;
    if ( (vma_list = pidmap__get_maps( pid )) == NULL ) {
        __die( pidmap__get_err() );
    }

    head = vma_list;

    while ( vma_list != NULL ) {
        if ( vma_list->perms & R && vma_list->perms & X ) {
            printf( "%lu\n", vma_list->start );
            printf( "%lu\n", vma_list->end );
            printf( "%lu\n", vma_list->offset );
            printf( "%s\n",  vma_list->mmi );
            printf( "%lu\n", vma_list->inode );
            printf( "%s\n",  vma_list->image );
        }
        vma_list = vma_list->next;
    }
    pidmap__release( head );
    return 0;
}