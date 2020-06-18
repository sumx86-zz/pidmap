#ifndef __PIDMAP_PARSER
#define __PIDMAP_PARSER 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "strtokenize.h"

#ifdef __cplusplus
extern "C" {
#endif

#define pidmap__set_err( str, ret ){\
    sprintf( pidmap_err, "%s", str ); return ret;\
}

#ifndef NO_IMG
    #define NO_IMG "no-img\n"
#endif

char pidmap_err[0xff];

static FILE *_fp;

typedef enum {
    R = 0x1,
    W = 0x2,
    X = 0x8,
    P = 0x40,
    S = 0x1000
} vma_perm_t;

struct vma_list {
    uintptr_t start;
    uintptr_t end;
    vma_perm_t perms;
    uintptr_t offset;
    char mmi[10];
    uintptr_t inode;
    char image[0xff];
    struct vma_list *next;
};

struct vma_list * pidmap__get_maps( pid_t pid );
struct vma_list * __new_vma_entry( char *buff );
uintptr_t __hexd( const char *str, size_t size );
int __add_vma( struct vma_list *head, char *buff );
void pidmap__release( struct vma_list *head );
int pidmap__parse_line( char *buff, struct vma_list *vma );
void __define_maps_addr( struct vma_list *vma, char *buff );
void __define_perms( struct vma_list *vma_list, char *buff );
void __define_offset( struct vma_list *vma, char *buff );
void __define_mmin( struct vma_list *vma, char *buff );
void __define_inode( struct vma_list *vma, char *buff );
void __define_image( struct vma_list *vma, char *buff );
char * pidmap__get_err( void );
int iswsp( char *str );

#ifdef __cplusplus
}
#endif

#endif
