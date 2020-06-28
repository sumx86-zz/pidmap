#include "pidmap.h"

int __parse_line( char *buff, struct vma_list *vma )
{
    char *entries[0x6] = {0x00};
    int n = 0x00;

    if ( (n = __strtokenize( buff, entries, " " )) < 0x00 )
        goto bad;

    __define_maps_addr( vma, entries[0] );
    __define_perms(     vma, entries[1] );
    __define_offset(    vma, entries[2] );
    __define_mmin(      vma, entries[3] );
    __define_inode(     vma, entries[4] );
    __define_image(     vma, iswsp( entries[5] ) ? NO_IMG : entries[5] );

    for ( int i = 0 ; i < n ; i++ )
        free( entries[i] );
    return 0;

    bad:
        return (-1);
}

void __define_maps_addr( struct vma_list *vma, const char *buff )
{
    char hex1[25] = {0x00};
    char hex2[25] = {0x00};
    char *tmp = hex1;

    while ( *buff != '\x00' ) {
        if ( *buff == '-' )
            *tmp = '\x00', tmp = hex2;
        else
            *tmp++ = *buff;
        buff++;
    }
    vma->start = strtoul( hex1, NULL, 16 );
    vma->end   = strtoul( hex2, NULL, 16 );
}

void __define_perms( struct vma_list *vma_list, const char *buff )
{
    if ( buff[0] == 'r' )
        vma_list->perms |= R;

    if ( buff[1] == 'w' )
        vma_list->perms |= W;

    if ( buff[2] == 'x' )
        vma_list->perms |= X;

    if ( buff[3] == 'p' )
        vma_list->perms |= P;
    else
        vma_list->perms |= S;
}

void __define_offset( struct vma_list *vma, const char *buff ) {
    vma->offset = strtoul( buff, NULL, 16 );
}

void __define_mmin( struct vma_list *vma, const char *buff ) {
    strcpy( vma->mmi, buff );
}

void __define_inode( struct vma_list *vma, const char *buff ) {
    vma->inode = strtoul( buff, NULL, 16 );
}

void __define_image( struct vma_list *vma, const char *buff ) {
    strcpy( vma->image, buff );
}

struct vma_list * pidmap__get_maps( pid_t pid )
{
    char path[20] = {0x00};
    sprintf( path, "/proc/%d/maps", pid );
    
    _fp = fopen( path, "r" );
    if ( !_fp ) {
        pidmap__set_err(
            strerror( errno ),
            NULL
        );
    }
    struct vma_list *head = __new_vma_entry( NULL );
    if ( !head )
        goto bad;

    char buff[0xff] = {0x00};
    while ( fgets( buff, sizeof( buff ), _fp ) ) {
        if ( __add_vma( head, buff ) < 0 )
            goto bad;
    }
    return head;

    bad:
        return (NULL);
}

struct vma_list * __new_vma_entry( char *buff )
{
    struct vma_list *vma_entry = (struct vma_list *) malloc( sizeof(struct vma_list) );
    if ( vma_entry == NULL )
        pidmap__set_err(
            strerror( errno ),
            NULL
        );
    if ( buff != NULL ) {
        if ( __parse_line( buff, vma_entry ) == -1 && errno == ENOMEM )
            pidmap__set_err(
                strerror( errno ),
                NULL
            );
    }
    vma_entry->next = NULL;
    return vma_entry;
}

int __add_vma( struct vma_list *head, char *buff )
{
    struct vma_list *vma_entry = head;
    while ( vma_entry->next != NULL )
        vma_entry = vma_entry->next;

    if ( (vma_entry->next = __new_vma_entry( buff )) == NULL )
        return -1;

    return 0;
}

void pidmap__release( struct vma_list *head )
{
    struct vma_list *temp;
    while ( head != NULL ) {
        temp = head;
        head = head->next;
        free( temp );
    }
}

char * pidmap__get_err( void ) {
    return pidmap_err;
}

int iswsp( char *str )
{
    while ( *str != '\x00' ) {
        if ( *str > 32 ) return 0;
        str++;
    }
    return 1;
}
