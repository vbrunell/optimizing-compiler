/*
 * perform control flow analysis functions
 */
#include <stdio.h>
#include <string.h>
#include "opt.h"
#include "flow.h"
#include "misc.h"

int numblks = 0;         /* number of basic blocks in the function */

/*
 * numberblks - number the basic blocks in the function
 */
void numberblks()
{
   int num;
   struct bblk *cblk;
   extern struct bblk *top;
   extern int bvectlen;

   /* number each basic block in the function */
   num = 0;
   for (cblk = top; cblk; cblk = cblk->down)
      cblk->num = ++num;
   num++;

   /* reset the length in integer words for the basic block bit vectors */
   bvectlen = num >> LOG2_INT;
   if (num & INT_REM)
      bvectlen++;

   /* set the number of blocks for the program */
   numblks = num-1;
}

/*
 * findblk - find the block with the specified label
 */
struct bblk *findblk(char *label)
{
   struct bblk *cblk;
   extern struct bblk *top;

   /* search for the block */
   for (cblk = top; cblk; cblk = cblk->down)
      if (cblk->label && strcmp(cblk->label, label) == 0)
         return cblk;

   /* could not find the block */
   return (struct bblk *) NULL;
}

/*
 * setupcontrolflow - setup the control flow within the function
 */
void setupcontrolflow()
{
   struct bblk *cblk, *tblk;
   extern struct bblk *top;

   /* for each basic block in the function */
   for (cblk = top; cblk; cblk = cblk->down) {

      /* if there are one or more instructions in the block */
      if (cblk->lineend) {

         /* if the last instruction is a jump, then establish the
            control flow between the current block and the target
            and continue with the next block */
         if (cblk->lineend->type == JUMP_INST) {
            if ((tblk = findblk(cblk->lineend->items[1]))) {
               addtoblist(&cblk->succs, tblk);
               addtoblist(&tblk->preds, cblk);
            }
            else {
               fprintf(stderr,
                      "setupcontrolflow - could not find block with label %s\n",
                       cblk->lineend->items[1]);
               quit(1);
            }
            continue;
         }

         /* if the last instruction is a branch, then establish the
            control flow between the current block and the target */
         else if (cblk->lineend->type == BRANCH_INST) {
            if ((tblk = findblk(cblk->lineend->items[1]))) {
               addtoblist(&cblk->succs, tblk);
               addtoblist(&tblk->preds, cblk);
            }
            else {
               fprintf(stderr,
                      "setupcontrolflow - could not find block with label %s\n",
                       cblk->lineend->items[1]);
               quit(1);
            }
         }

         /* if the last instruction is a return, then continue with
            the next block since a return has no successors */
         else if (cblk->lineend->prev &&
                  cblk->lineend->prev->type == RETURN_INST)
            continue;
      }

      /* establish control flow between the current block and the
         block following it since the current block could fall into it */
      addtoblist(&cblk->succs, cblk->down);
      if (cblk->down)
         addtoblist(&cblk->down->preds, cblk);
   }
   check_cf();
}

/*
 * clearstatus - clear the status field in the basic block indicating
 *               that it has not been visited yet
 */
void clearstatus()
{
    struct bblk *cblk;
    extern struct bblk *top;

    for (cblk=top; cblk; cblk=cblk->down)
        cblk->status &= ~DONE;
}

/*
 * check_cf - check the control flow for inconsistencies
 */
void check_cf()
{
   struct bblk *cblk, *tblk;
   struct blist *bptr;
   struct assemline *ptr;
   extern struct bblk *top;

   /* go through every block */
   numberblks();
   for (cblk = top; cblk; cblk = cblk->down) {

       /* check that up matches down */
       if (cblk->up && cblk->up->down != cblk) {
	  fprintf(stderr, "check_cf - up does not match down at block %d\n",
                  cblk->up->num);
          quit(1);
       }

       /* check that down matches up */
       if (cblk->down && cblk->down->up != cblk) {
	  fprintf(stderr, "check_cf - down does not match up at block %d\n",
                  cblk->down->num);
          quit(1);
       }

       /* check if all predecessors match */
       for (bptr = cblk->preds; bptr; bptr = bptr->next)
          if (!inblist(bptr->ptr->succs, cblk)) {
             fprintf(stderr, "check_cf - pred %d does not have %d as succ\n",
                     bptr->ptr->num, cblk->num);
             quit(1);
          }

       /* check if all successors match */
       for (bptr = cblk->succs; bptr; bptr = bptr->next)
          if (!inblist(bptr->ptr->preds, cblk)) {
             fprintf(stderr, "check_cf - succ %d does not have %d as pred\n",
                     bptr->ptr->num, cblk->num);
             quit(1);
          }

       /* check that if two successors, then the first successor is the
          fall-through successor */
       if (cblk->succs && cblk->succs->next &&
           cblk->succs->ptr != cblk->down) {
          fprintf(stderr,
                  "check_cf - first succ of branch should be fallthru\n");
          quit(1);
       }


       /* check if target of jump or branch instruction is to the 
          correct block */
       if (cblk->lineend) {
          if (cblk->lineend->type == JUMP_INST && 
              !((tblk = cblk->succs->ptr) && tblk->label &&
                strcmp(cblk->lineend->items[1], tblk->label) == 0)) {
             fprintf(stderr, 
                     "check_cf - target of jump in block %d incorrect\n",
                     cblk->num);
             quit(1);
          }
          else if (cblk->lineend->type == BRANCH_INST && 
                   !((tblk = cblk->succs->next->ptr) && tblk->label &&
                     strcmp(cblk->lineend->items[1], tblk->label) == 0)) {
             fprintf(stderr, 
                     "check_cf - target of branch in block %d incorrect\n",
                     cblk->num);
             quit(1);
          }
       }
             
       /* check that assembly lines are appropriately linked */
       if (cblk->lines && cblk->lines->prev) {
          fprintf(stderr,
                  "check_cf - first line in %d should not have a prev line\n",
                  cblk->num);
          quit(1);
       }
       for (ptr = cblk->lines; ptr; ptr = ptr->next) {
          if (ptr->next && ptr->next->prev != ptr) {
	     fprintf(stderr,
                     "check_cf - next does not match prev at block %d\n",
                     cblk->num);
             quit(1);
          }
          if (ptr->prev && ptr->prev->next != ptr) {
	     fprintf(stderr,
                     "check_cf - prev does not match next at block %d\n",
                     cblk->num);
             quit(1);
          }
       }
       if (cblk->lineend && cblk->lineend->next) {
          fprintf(stderr, "last line in %d should not have a next line\n",
                  cblk->num);
          quit(1);
       }
   }
}
