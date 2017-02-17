/*
 * VJB
 * FSU Department of Computer Science
 * Branch Chaining
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opt.h"
#include "misc.h"
#include "opts.h"
#include "flow.h"

int oneinst(struct bblk* blk);
void swapsuccs(struct blist** head);

/*
 * remvbranchchains - remove branch chains
 */
void remvbranchchains()
{
   extern struct bblk* top;
   struct bblk* cblk = (struct cblk*)NULL;
   struct assemline* lnptr = (struct assemline*)NULL;
   struct blist * predlst = (struct blist*)NULL;

   /*
    * Reassign branches to empty blocks
    * to target the next block down
   */
   // for each basic block in the function
   for (cblk = top; cblk; cblk = cblk->down)
   {
      // if there are no instructions in a blk with a label
      if (cblk->label && !(cblk->lineend))
      {
         // look through its preds
         for(predlst = cblk->preds; predlst; predlst = predlst->next)
         {
            // for each assemline in a pred block
            for (lnptr = predlst->ptr->lines; lnptr; lnptr = lnptr->next)
            {
               // find branches and jumps
               if(lnptr->type == BRANCH_INST || lnptr->type == JUMP_INST)
               {
                  // if they target the empty block
                  if(strcmp(cblk->label,lnptr->items[1]) == 0)
                  {
                     // construct new assemline
                     // replace branch assemline
                     char str[MAXLINE];
                     sprintf(str,"\t%s\t%s", lnptr->items[0],\
                           cblk->down->label);
                     strcpy(lnptr->items[1],cblk->down->label);
                     lnptr->text = allocstring(str);
                  }
               }
            }

            // update preds and succs
            addtoblist(&(predlst->ptr->succs),cblk->down);
            swapsuccs(&(predlst->ptr->succs));
            addtoblist(&(cblk->down->preds),predlst->ptr);
         }

         // remove the empty block
         struct bblk * tmp = cblk->down;
         deleteblk(cblk);
         cblk = tmp;

         check_cf();

         incropt(BRANCH_CHAINING);
      }
   }

   /*
    * Reassign branches to blocks which contain
    * only a jump to the target of the jump
   */
   // for each basic block in the function
   for (cblk = top; cblk; cblk = cblk->down)
   {
      // for non-empty blocks
      if( cblk->lineend )
      {
         // find jumps that occupy a block with
         // a single instruction that follows a label
         if (
               oneinst(cblk) == 1
               && cblk->lineend->type == JUMP_INST
               && cblk->label
               && cblk->up->label != cblk->label
               && cblk->down->label != cblk->label
            )
         {
            // get the target of the jump
            char * targ = allocstring(cblk->lineend->items[1]);

            // for each pred of the block with the jump
            struct blist * listptr = (struct blist*)NULL;
            for(listptr = cblk->preds; listptr; listptr = listptr->next)
            {
               // check the lines in the pred
               for (lnptr = listptr->ptr->lines; lnptr; lnptr = lnptr->next)
               {
                  // find branches and jumps
                  if(   lnptr->type == BRANCH_INST ||
                        lnptr->type == JUMP_INST )
                  {
                     // if the branch targets the label for the
                     // block containing the jump, replace it with
                     // the target of the jump
                     if(strcmp(lnptr->items[1],cblk->label) == 0)
                     {
                        // construct and replace assemline
                        char str[MAXLINE];
                        sprintf(str,"\t%s\t%s", lnptr->items[0], targ);
                        strcpy(lnptr->items[1],targ);
                        lnptr->text = allocstring(str);
                     }
                  }
               }

               // update preds and succs
               if( listptr->ptr != cblk->up )
               {
                  addtoblist(&(cblk->succs->ptr->preds),listptr->ptr);
                  addtoblist(&(listptr->ptr->succs),cblk->succs->ptr);
                  swapsuccs(&(listptr->ptr->succs));
                  delfromblist(&(listptr->ptr->succs),cblk);
                  delfromblist(&(cblk->preds),listptr->ptr);
               }
            }

            // remove cblk label
            free(cblk->label);
            cblk->label = (char*)NULL;

            incropt(BRANCH_CHAINING);
         }
      }
   }

   check_cf();
}

// Swap successor head
void swapsuccs(struct blist ** head)
{
   struct blist * tmp1;
   struct blist * tmp2;

   if(*head && (*head)->next && (*head)->next->next)
   {
      tmp1 = *head;
      tmp2 = (*head)->next->next;
      *head = (*head)->next;
      (*head)->next = tmp1;
      tmp1->next = tmp2;
   }
   else if( *head && (*head)->next )
   {
      tmp1 = *head;
      *head = (*head)->next;
      (*head)->next = tmp1;
      tmp1->next = NULL;
   }
}

// Check if only one instruction in the block
int oneinst(struct bblk * blk)
{
   struct assemline * lptr = NULL;
   int i = 0;

   for(lptr = blk->lines; lptr; lptr = lptr->next)
      if(INST(lptr->type))
         i++;

   return i;
}
