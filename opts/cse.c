/*
 * VJB
 * FSU Department of Computer Science
 * Common Subexpression Elimination
 */

#include <stdio.h>
#include <string.h>
#include "opt.h"
#include "misc.h"
#include "vars.h"

// -------------------------------------------------------------
int instmatch(struct assemline * aptr, struct assemline * nptr);
// -------------------------------------------------------------

/*
 * cseinblk - perform local common subexpression elimination in a block
 */
void cseinblk(struct bblk *cblk, int *changes)
{
   extern struct instinfo insttypes[];
   struct assemline * aptr = NULL;
   struct assemline * nptr = NULL;
   int ind = 0;

   // for each line in the blk
   // check for arithmetic insts
   for(aptr = cblk->lines; aptr; aptr = aptr->next)
   {
      if(INST(aptr->type) && (aptr->type == ARITH_INST))
      {
         // look through the remaining lines for
         // a redundant computation
         for(nptr = aptr->next; nptr; nptr = nptr->next)
         {
            if(INST(nptr->type) && (nptr->type == ARITH_INST))
            {
               // if inst sets and uses the dest reg of the redundant inst
               // we can't create a move
               if(varcommon(aptr->sets,nptr->sets) && varcommon(aptr->sets,nptr->uses))
                  break;

               // if we match the mnemonic and the source
               // operands, change the matched inst to a move
               if(instmatch(aptr,nptr))
               {
                  ind = aptr->instinfonum;
                  createmove(insttypes[ind].datatype,\
                        aptr->items[aptr->numitems-1],\
                        nptr->items[nptr->numitems-1],nptr);

                  incropt(LOCAL_CSE);
                  *changes = TRUE;
               }
            }
               // if the dest reg is set again, stop
               if(varcommon(aptr->sets,nptr->sets))
                  break;
         }
      }
   }
}

/*
 * localcse - perform local common subexpression elimination
 */
void localcse(int *changes)
{
   struct bblk *cblk;
   extern struct bblk *top;

   for (cblk = top; cblk; cblk = cblk->down)
      cseinblk(cblk, changes);
}

// check if two instructions match
//    checks mnemonic and source operands
int instmatch(struct assemline * aptr, struct assemline * nptr)
{
   if(aptr->numitems != nptr->numitems)
      return 0;

   if(aptr->type != nptr->type)
      return 0;

   // check if inst sources and mnem are identical
   int i = 0;
   for(; i < aptr->numitems - 1; i++)
      if(strcmp(aptr->items[i],nptr->items[i]))
         return 0;

   return 1;
}
