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
        //if ( vma_list->perms & R && vma_list->perms & X ) {
            printf( "%x%x\n", (uint32_t) ((vma_list->start >> 32) & 0xffffffff), (uint32_t) ((vma_list->start >> 0x0) & 0xffffffff) );
            printf( "%x%x\n", (uint32_t) ((vma_list->end   >> 32) & 0xffffffff), (uint32_t) ((vma_list->end   >> 0x0) & 0xffffffff) );
            printf( "%x\n",  vma_list->offset );
            printf( "%s\n",  vma_list->mmi );
            printf( "%x\n",  vma_list->inode );
            printf( "%s\n",  vma_list->image );
        //}
        vma_list = vma_list->next;
    }
    pidmap__release( head );
    return 0;
}
