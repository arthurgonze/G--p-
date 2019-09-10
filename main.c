#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zconf.h>

#define BUFSIZE 4096
#define PERMS 0666 /*RW for owner, group, others */
#define NALLOC 1024 /* minimum #units to request */

#undef getchar
#undef malloc
#undef sbrk

typedef long Align; /* for aligment to long boundary*/
union header
{
    struct
    {
        union header *ptr; /* next block if on free list */
        unsigned size; /* size of this block */
    } s;
    Align x;/* force alignment of blocks */
};

typedef union header Header;

static Header base; /* empty list to get started */
static Header *freep = NULL; /* start of free list */
static Header *morecore(unsigned nu);

int getchar(void);
void error(char *, ...);
void free(void *ap);

void *malloc(unsigned nbytes);

/**
 * cp: copy f1 to f2
 * @return
 */
int main(int argc, char *argv[])
{
    int f1, f2, n;
    char buf[BUFSIZE];

    if (argc==1)
    {
        while ((n = read(0, buf, BUFSIZE)) > 0)
        {
            write(1, buf, n);
        }
    }
    else
    {
        if (argc!=3)
        {
            error("Usage: cp from to \n");
        }
        else if (argc==3)
        {

            if ((f1 = open(argv[1], O_RDONLY, 0))==-1)
            {
                error("cp: can't open %s \n", argv[1]);
            }

            if ((f2 = creat(argv[2], PERMS))==-1)
            {
                error("cp: can't creat %s, mode %03o \n", argv[2], PERMS);
            }

            while ((n = read(f1, buf, BUFSIZE)) > 0)
            {
                if (write(f2, buf, n)!=n)
                {
                    error("cp: write error on file %s \n", argv[2]);
                }
            }

        }
        return 0;
    }
}

/**
 * getchar: simple buffere version
 * @return
 */
int getchar(void)
{
    static char buf[BUFSIZE];
    static char *bufp = buf;
    static int n = 0;

    /* Buffer is empty */
    if (n==0)
    {
        n = read(0, buf, sizeof(buf));
        bufp = buf;
    }

    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

/**
 * error: print an error message and die
 * @param fmt
 * @param ...
 */
void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vprintf(fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

/**
 * malloc: general-purpose storage allocator
 * @param nbytes
 * @return
 */
void *malloc(unsigned nbytes)
{
    Header *p, *prevp;
    Header *moreroce(unsigned);
    unsigned nunits;

    nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header/*header*/) + 1;
    /* no free list yet */
    if ((prevp = freep)==NULL)
    {
        base.s.ptr = freep/*freeptr*/ = prevp/*prevptr*/ = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr)
    {
        if (p->s.size >= nunits)/* big enough */
        {
            if (p->s.size==nunits)/* exactly */
            {
                prevp->s.ptr = p->s.ptr;
            }
            else /* allocate tail end */
            {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *) (p + 1);
        }
    }
}

/**
 * morecore: ask system for more memory
 * @param nu
 * @return
 */
static Header *morecore(unsigned nu)
{
    char *cp, *sbrk(int);
    Header *up;

    if (nu < NALLOC)
    {
        nu = NALLOC;
    }
    cp = sbrk(nu*sizeof(Header));
    if (cp==(char *) -1) /* no space at all */
    {
        return NULL;
    }
    up = (Header *) cp;
    up->s.size = nu;
    free((void *) (up + 1));
    return freep;
}

/**
 * free: put block ap in free list
 * @param ap
 */
void free(void *ap)
{
    Header *bp, *p;
    bp = (Header *) ap - 1; /* point to block header */
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
        {
            break;  /* freed block at start or end of arena */
        }
    }

    if (bp + bp->s.size/*size*/ == p->s.ptr)  /* join to upper nbr */
    {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    }else
    {
        bp->s.ptr = p->s.ptr;
    }

    if(p + p->s.size/*size*/ == bp)   /* join to lower nbr */
    {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
    {
        p->s.ptr = bp;
    }
    freep = p;
}