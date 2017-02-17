/*
 * VJB
 * FSU Department of Computer Science
 * Detecting Loops
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "opt.h"
#include "misc.h"
#include "analysis.h"
#include "vect.h"
#include "flow.h"

struct loopnode *loops;    /* head of linked list of loops */

void sortloops();
short getnestlvl(struct loopnode * currl);
int inloop(struct loopnode * checkl, struct loopnode * currl);
void build_loop(struct bblk *tail, struct bblk *head, struct loopnode *loop);
void resetstatus();
void mergeloops();
void removeduplicates();

/*
 * findloops - locate loops in the program and build the loop data structure
 */
void findloops()
{
   freeloops();

   extern struct bblk * top;
   struct bblk * cblk = (struct cblk*)NULL;
   struct blist * predlst = (struct blist*)NULL;
   int change = 0;

   numberblks();

   /////////////// begin: calculate dominators ///////////////
   // dom(top) = {top}
   binsert(&top->dom,top->num);

   // for each blk n in N-{top}
   // dom(n) = N
   for (cblk = top->down; cblk; cblk = cblk->down)
      bcpy(&cblk->dom,ball());

   do
   {
      change = 0;

      // for each blk n in N-{top}
      for (cblk = top->down; cblk; cblk = cblk->down)
      {
         // save dom(n) before change
         bvect _pdoms = ball();
         bclear(_pdoms);
         bcpy(&_pdoms,cblk->dom);

         bvect _preds = ball();
         bclear(_preds);
         int firstpred = 1;

         // dom(n) = intersection of all dom(P), where P
         //          are the immediate preds of n
         for(predlst = cblk->preds; predlst; predlst = predlst->next)
         {
            if( firstpred == 1 )
            {
               bunion(&_preds,predlst->ptr->dom);
               firstpred = 0;
               continue;
            }

            binter(&_preds, predlst->ptr->dom);
         }

         bclear(cblk->dom);
         bcpy(&cblk->dom,_preds);

         // dom(n) = dom(n) U {n}
         binsert(&cblk->dom,cblk->num);

         // check for change in dom(n)
         if( !bequal(_pdoms,cblk->dom) )
            change = 1;
      }
   }while(change);
   ////////////// end: calculate dominators ///////////////

   check_cf();

   ////////////// begin: calculate natural loops //////////////
   extern struct loopnode * loops;
   struct loopnode * loop = (struct loopnode*)NULL;

   // for each blk in the function
   for (cblk = top->down; cblk; cblk = cblk->down)
   {
      // if a blk is dominated by a successor
      if(
            cblk->succs
            && bin(cblk->dom,cblk->succs->ptr->num)
        )
      {
         loop = newloop();

         // add head and tail to list of loop blocks
         addtoblist(&(loop->blocks),cblk);
         addtoblist(&(loop->blocks),cblk->succs->ptr);

         // mark head as visited
         cblk->succs->ptr->status = 1;

         // recursively construct the loop
         build_loop(cblk,cblk->succs->ptr,loop);

         // set loop header
         loop->header = cblk->succs->ptr;

         // set loop field of head blk to the newly constructed loop
         cblk->succs->ptr->loop = loop;

         resetstatus();
      }
      else if(
                cblk->succs
                && cblk->succs->next
                && bin(cblk->dom,cblk->succs->next->ptr->num)
             )
      {
         loop = newloop();

         // add head and tail to list of loop blocks
         addtoblist(&(loop->blocks),cblk);
         addtoblist(&(loop->blocks),cblk->succs->next->ptr);

         // mark head as visited
         cblk->succs->next->ptr->status = 1;

         // recursively construct the loop
         build_loop(cblk,cblk->succs->next->ptr,loop);

         // set loop header
         loop->header = cblk->succs->next->ptr;

         // set loop field of head blk to the newly constructed loop
         cblk->succs->next->ptr->loop = loop;

         resetstatus();
      }
   }
   //////////////// end: calculate natural loops ////////////////////

   // merge loops with same head block
   sortloops();
   mergeloops();
   removeduplicates();

   // calculate nesting level and update loopnest field
   struct loopnode * lptr = (struct loopnode *)NULL;
   for(lptr = loops; lptr; lptr = lptr->next)
      lptr->header->loopnest = getnestlvl(lptr);

   // sort the loops by nest level
   sortloops();

   // free the loop list for the next function
   check_cf();
}

// sort loops by nest level
void sortloops()
{
   extern struct loopnode * loops;
   struct loopnode * lptr = (struct loopnode *)NULL;
   struct loopnode * tmp;
   tmp = (struct loopnode *) alloc(sizeof(struct loopnode));
   int change = 0;

   // bubble sort loops in descending order
   // by loop nest level
   if( loops && loops->next )
   {
      // sort by nest level
      do
      {
         change = 0;

         for(lptr = loops; lptr->next; lptr = lptr->next)
         {
            if(lptr->header->loopnest < lptr->next->header->loopnest)
            {
               tmp->header = lptr->header;
               tmp->blocks = lptr->blocks;
               lptr->blocks = lptr->next->blocks;
               lptr->header = lptr->next->header;
               lptr->next->blocks = tmp->blocks;
               lptr->next->header = tmp->header;

               change = 1;
            }
         }

      }while(change);

      // break nest ties by placing lower block number first
      do
      {
         change = 0;

         for(lptr = loops; lptr->next; lptr = lptr->next)
         {
            if(
                  (lptr->header->loopnest == lptr->next->header->loopnest)
                  && (lptr->header->num > lptr->next->header->num)
               )
            {
               tmp->header = lptr->header;
               tmp->blocks = lptr->blocks;
               lptr->blocks = lptr->next->blocks;
               lptr->header = lptr->next->header;
               lptr->next->blocks = tmp->blocks;
               lptr->next->header = tmp->header;

               change = 1;
            }
         }

      }while(change);
   }
}

// merge loops with the same head into one loop
void mergeloops()
{
   extern struct loopnode * loops;
   struct loopnode * lptr;
   struct loopnode * llptr = (struct loopnode *)NULL;
   struct loopnode * tmp;
   tmp = (struct loopnode *) alloc(sizeof(struct loopnode));
   struct blist * bptr = NULL;

   if( loops && loops->next )
   {
      // for each loop
      for(lptr = loops; lptr->next; lptr = lptr->next)
      {
         // check for other loops with same head
         for(llptr = lptr->next; llptr; llptr = llptr->next)
         {
            // merge the blocks into one loop
            if(lptr->header->num == llptr->header->num)
            {
               for(bptr = llptr->blocks; bptr; bptr = bptr->next)
                  addtoblist(&(lptr->blocks),bptr->ptr);
            }
         }
      }
   }
}

// remove loops with duplicate head blocks
void removeduplicates()
{
   extern struct loopnode * loops;
   struct loopnode * lptr = loops;
   struct loopnode * nxtnxt;

   if(!lptr)
      return;

   // loop through loops and remove nodes with
   // duplicate head block
   while(lptr->next)
   {
      if(lptr->header->num == lptr->next->header->num)
      {
         nxtnxt = lptr->next->next;
         freeblist(lptr->next->blocks);
         free(lptr->next);
         lptr->next = nxtnxt;
      }
      else
         lptr = lptr->next;
   }
}

// get nest level of a loop
short getnestlvl(struct loopnode * currl)
{
   extern struct loopnode * loops;
   struct loopnode * lptr = (struct loopnode *)NULL;
   short nstlvl = 0;

   // for each loop in the function
   // check if the current loop is contained
   // in another loop
   // if it is, increase nest level
   for(lptr = loops; lptr; lptr = lptr->next)
      if( (currl != lptr) && inloop(lptr,currl) )
         nstlvl++;

   return nstlvl;
}

// check to see if all a loop's blocks are contained in
// another loop
int inloop(struct loopnode * checkl, struct loopnode * currl)
{
   struct blist * lstptr = (struct blist*)NULL;
   int found = 1;

   // check that the current loop's blocks are all contained
   // in the loop we're checking
   for(lstptr = currl->blocks;  lstptr; lstptr = lstptr->next)
   {
      if(!inblist(checkl->blocks,lstptr->ptr))
      {
         found = 0;
         break;
      }
   }

   return found;
}

// build the loop
void build_loop(struct bblk * tail, struct bblk * head, struct loopnode * loop)
{
   struct blist * preds = NULL;

   // if we reach the loop head
   // or if we've already visited this block
   // end the recursive search
   if( (tail == head) || (tail->status) )
      return;

   // if there are preds
   if( tail->preds )
   {
      tail->status = 1;

      // for each pred, add it to the loop block list
      // search the preds of the pred for more blocks to add
      for(preds=tail->preds; preds; preds = preds->next)
      {
         addtoblist(&(loop->blocks),preds->ptr);
         build_loop(preds->ptr, head, loop);
      }
   }
}

// reset each block to not visited
void resetstatus()
{
    struct bblk *cblk;
    extern struct bblk *top;

    for (cblk=top; cblk; cblk=cblk->down)
        cblk->status = 0;
}
