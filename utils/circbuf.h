#ifndef _CIRCBUF_H
#define _CIRCBUF_H

#include "globdefs.h"

/* Opaque buffer element type.  This would be defined by the application. */
typedef u8 ElemType;


/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
    ElemType   *elems;  /* vector of elements                   */
} CircularBuffer;


int   cb_init(CircularBuffer*, int);
void  cb_free(CircularBuffer*);
void  cb_clear(CircularBuffer *);
int   cb_is_full(CircularBuffer*);
int   cb_is_empty(CircularBuffer*);
void  cb_write(CircularBuffer *, ElemType *);
void  cb_read(CircularBuffer *, ElemType *);


#endif /*  circbuf.h  */

