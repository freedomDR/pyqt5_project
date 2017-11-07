
#ifndef PORTING_APIS_H
#define PORTING_APIS_H

#include <stdarg.h>

#define T_minus    (1<<0)
#define T_plus     (1<<1)
#define T_alt      (1<<2)
#define T_space    (1<<3)
#define T_zero     (1<<4)
#define T_done     (1<<5)

#define T_char     (1<<6)
#define T_short    (1<<7)
#define T_long     (1<<8)
#define T_llong    (1<<9)
#define T_intmax   (1<<10)
#define T_size     (1<<11)
#define T_ptrdiff  (1<<12)
#define T_intptr   (1<<13)

#define T_ldouble  (1<<14)

#define T_lower    (1<<15)
#define T_unsigned (1<<16)

#define T_TYPES (T_char | T_short | T_long | T_llong | T_intmax \
                | T_size | T_ptrdiff | T_intptr)

struct status_t
{
    int              base;   /* base to which the value shall be converted   */
    int 			 flags;  /* flags and length modifiers                */
    unsigned         n;      /* print: maximum characters to be written (snprintf) */
                             /* scan:  number matched conversion specifiers  */
    unsigned         i;      /* number of characters read/written            */
    unsigned         current;/* chars read/written in the CURRENT conversion */
    unsigned         width;  /* specified field width                        */
    int              prec;   /* specified field precision                    */

    void *           ctx;    /* context for callback */

    size_t (*write) (void *p, const char *buf, size_t size);

	va_list  arg;    /* argument stack                               */
};

struct state {
    size_t bufrem;
    char *bufp;
};

int i_vsnprintf(char *, size_t, const char *, va_list ap);
int i_snprintf( char *, size_t, const char *, ...);

const void * i_bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)( const void *, const void * ) );
void i_qsort(void *base, size_t nmemb, size_t size, int (*compar)( const void *, const void *));
int i_atoi(char *);

#endif // PORTING_APIS_H
