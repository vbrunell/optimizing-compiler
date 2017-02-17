/*
 * VJB
 * FSU Department of Computer Science
 * Reversing Branches
 */

#include <stdio.h>
#include <string.h>
#include "opt.h"
#include "misc.h"
#include "opts.h"
#include "flow.h"

int one_inst(struct bblk* blk);
void swap_succs(struct blist** head);
char * get_revbstr(char *str);

/*
 * reversebranches - avoid jumps by reversing branches
 */
void reversebranches()
{
   extern struct bblk* top;
   struct bblk* cblk = (struct cblk*)NULL;

   // for each basic block in the function
   for (cblk = top; cblk; cblk = cblk->down)
   {
      // find blks that meet criteria for branch reversal
      if( cblk->lineend
          && cblk->lineend->type == BRANCH_INST
          && one_inst(cblk->down) == 1
          && cblk->down->lineend->type == JUMP_INST
          && cblk->succs->next->ptr == cblk->down->down
        )
      {
         // construct new assemline
         // replace branch assemline
         char * rbstr = get_revbstr(cblk->lineend->items[0]);
         char str[MAXLINE];
         sprintf(str,"\t%s\t%s", rbstr,cblk->down->lineend->items[1]);
         strcpy(cblk->lineend->items[0],rbstr);
         strcpy(cblk->lineend->items[1],cblk->down->lineend->items[1]);
         cblk->lineend->text = allocstring(str);

         // add blk->down->succs to blk->succs
         addtoblist(&(cblk->succs),cblk->down->succs->ptr);
         swap_succs(&(cblk->succs));

         // add blk to blk->down->succs->preds
         addtoblist(&(cblk->down->succs->ptr->preds),cblk);

         struct bblk * tmp = cblk->down->down;
         deleteblk(cblk->down);

         // make the previous target of the branch
         // the new fall through
         swap_succs(&(cblk->succs));

         // fix cblk pointer for loop
         cblk = tmp->up;

         // change the down to be the immediate successor
         cblk->down = cblk->succs->ptr;

         incropt(REVERSE_BRANCHES);
      }
   }

   check_cf();
}

// Swap successor head
void swap_succs(struct blist ** head)
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
int one_inst(struct bblk * blk)
{
   struct assemline * lptr = NULL;
   int i = 0;

   for(lptr = blk->lines; lptr; lptr = lptr->next)
      if(INST(lptr->type))
         i++;

   return i;
}

// Get the reverse of a branch
char * get_revbstr(char *str)
{
   char * bstr = (char*)NULL;

   if( strcmp(str,"bne") == 0)
   {
      bstr = (char *) alloc(strlen("be")+1);
      strcpy(bstr,"be");
   }
   else if( strcmp(str,"be") == 0)
   {
      bstr = (char *) alloc(strlen("bne")+1);
      strcpy(bstr,"bne");
   }
   else if( strcmp(str,"bg") == 0)
   {
      bstr = (char *) alloc(strlen("ble")+1);
      strcpy(bstr,"ble");
   }
   else if( strcmp(str,"ble") == 0)
   {
      bstr = (char *) alloc(strlen("bg")+1);
      strcpy(bstr,"bg");
   }
   else if( strcmp(str,"bl") == 0)
   {
      bstr = (char *) alloc(strlen("bge")+1);
      strcpy(bstr,"bge");
   }
   else if( strcmp(str,"bge") == 0)
   {
      bstr = (char *) alloc(strlen("bl")+1);
      strcpy(bstr,"bl");
   }
   else if( strcmp(str,"bgu") == 0)
   {
      bstr = (char *) alloc(strlen("bleu")+1);
      strcpy(bstr,"bleu");
   }
   else if( strcmp(str,"bleu") == 0)
   {
      bstr = (char *) alloc(strlen("bgu")+1);
      strcpy(bstr,"bgu");
   }
   else
      fprintf(stderr,"\n*** Branch string not found! ***\n");

   return bstr;
}
