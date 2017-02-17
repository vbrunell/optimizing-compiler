/*
 * block bit vector manipulation functions
 */
#include <stdlib.h>
#include <stdio.h>
#include "opt.h"
#include "vect.h"

int bvectlen;   /* length of a basic block vector in integers */

/*
 * binsert - add an item to a basic block vector
 */
void binsert(bvect *sptr, unsigned int r)
{
   unsigned int *ptr, *end;
 
   /* if the bit vector has not yet been allocated */
   if (!*sptr) {

      /* allocate the space and initialize all of the words to zero */
      *sptr = (bvect) (ptr = BALLOC);
      for (end = ptr + bvectlen; ptr != end; *ptr++ = 0)
         ;
   }

   /* set the bit that corresponds to the basic block number */
   (*sptr)[r >> LOG2_INT] |= (((unsigned) 1) << (r & INT_REM));
}

/*
 * bdelete - delete an item from a basic block vector
 */
void bdelete(bvect *sptr, unsigned int r)
{
   if (*sptr)
      (*sptr)[r >> LOG2_INT] &= ~(((unsigned) 1) << (r & INT_REM));
}

/*
 * bunion - computes the union of two bvects
 */
void bunion(bvect *result, bvect vec)
{
   unsigned int *ptr1, *ptr2, *end;
 
   /* if the source vector is allocated */
   if ((ptr2 = (unsigned int *) vec)) {

      /* if the result vector is allocated, then union in all of the
         words of the source vector */
      if ((ptr1 = (unsigned int *) *result))
         for (end = ptr1 + bvectlen; ptr1 != end; *ptr1++ |= *ptr2++)
            ;

      /* else allocate space for the result vector and copy the source
         vector to it */
      else {
         *result = (bvect) (ptr1 = BALLOC);
         for (end = ptr1 + bvectlen; ptr1 != end; *ptr1++ = *ptr2++)
            ;
      }
   }
}

/*
 * binter - computes the intersection of two bvects
 */
void binter(bvect *result, bvect vec)
{
   unsigned int *ptr1, *ptr2, *end;
 
   /* if the result vector and the source vector has been allocated */
   if ((ptr1 = (unsigned int *) *result) && (ptr2 = (unsigned int *) vec))

      /* union in all of the words of the source vector */
      for (end = ptr1 + bvectlen; ptr1 != end; *ptr1++ &= *ptr2++)
         ;

   /* else clear the result vector if it has been allocated */
   else if (*result)
      bclear(*result);
}

/*
 * bin - indicate if a basic block is in a bit vector
 */
int bin(bvect ptr, unsigned int r)
{
   /* The bvect has to be allocated and the appropriate word in the
      vector has to have the appropriate bit set. */
   return (ptr && ptr[r >> LOG2_INT] & (((unsigned) 1) << (r & INT_REM)));
}

/*
 * bcpy - copy a bvect list to another one
 */
void bcpy(bvect *dst, bvect src)
{
   register unsigned int *ptr1, *ptr2, *end;
 
   /* If the src vector is allocated */
   if (src) {

      /* if the dst is not allocated, then we have to allocate
         space for it */
      if (!*dst)
         *dst = (bvect) BALLOC;

      /* copy the src vector to the dst vector */
      end = (ptr1 = (unsigned int *) *dst) + bvectlen;
      ptr2 = (unsigned int *) src;
      while (ptr1 != end)
         *ptr1++ = *ptr2++;
   }

   /* if the dst is allocated, then just clear it */
   else if (*dst)
      bclear(*dst);
}
 
/*
 * bequal - compare two bvect lists to see if all bits are equal
 */
int bequal(bvect a, bvect b)
{
   unsigned int *ptr1, *ptr2, *end;
 
   /* if both bvect have been allocated */
   if (a && b) {

      /* compare each word, if any differ then the two are not equal */
      end = (ptr1 = (unsigned int *) a) + bvectlen;
      ptr2 = (unsigned int *) b;
      while (ptr1 != end)
         if (*ptr1++ != *ptr2++)
            return FALSE ;
      return TRUE ;
   }

   /* if one exists, check that it is empty.  If neither
      exist, then they are equal. */
   if (a)
      end = (ptr1 = (unsigned int *) a) + bvectlen;
   else if (b)
      end = (ptr1 = (unsigned int *) b) + bvectlen;
   else
      return TRUE;
   while (ptr1 != end)
      if (*ptr1++)
         return FALSE ;
   return TRUE ;
}

/*
 * ball - initialize a bvect list containing all basic blocks
 */
bvect ball()
{
   unsigned int *ptr, *end;
 
   /* allocate the vector */
   end = (ptr = BALLOC) + bvectlen;

   /* set all bits of each word of the vector */
   do {
      *(--end) = -1;
   }
   while (ptr != end);

   /* return the vector */
   return((bvect) ptr);
}

/*
 * bclear - reinitialize a bvect list to contain no blocks
 */
void bclear(bvect sptr)
{
   unsigned int *ptr, *end;

   /* clear all bits in each word of the vector */
   end = (ptr = (unsigned int *) sptr) + bvectlen;
   do {
      *(--end) = 0;
   }
   while (ptr != end);
}

/*
 * bcnt - determine the number of bits set in a bvect list
 */
int bcnt(bvect sptr)
{
   int i, cnt;
   unsigned int *ptr, *end;

   if (!sptr)
      return 0;
   cnt = 0;
   for (end = (ptr = (unsigned int *) sptr) + bvectlen; ptr != end; ptr++)
      for (i = 0; i < sizeof(unsigned int)*8; i++)
         if (*ptr & (1 << i))
            cnt++;
   return cnt;
}

/*
 * bnone - initialize a bvect list containing no basic blocks
 */
bvect bnone()
{
   bvect sptr;
   void bclear(bvect);
 
   /* initialize the space for the vector */
   sptr = BALLOC;

   /* clear out all bits in the vector */
   bclear(sptr);

   /* return the vector */
   return(sptr);
}

/*
 * bfree - free up a bvect list
 */
void bfree(bvect ptr)
{
   free(ptr);
}

/*
 * bdump - dump out a bvect list in a readable fashion
 */
void bdump(FILE *fout, bvect ptr)
{
   int i;
   int bin(bvect, unsigned int);
   extern int numblks;

   for (i = 1; i <= numblks; i++)
      if (bin(ptr, i))
         fprintf(fout, " %d", i);
}
