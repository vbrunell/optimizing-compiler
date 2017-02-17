/*
 * miscellaneous support functions
 */
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <setjmp.h>
#include "opt.h"
#include "io.h"
#include "misc.h"
#include "vars.h"
#include "vect.h"

/*
 * alloc - allocates space and checks the status of the allocation
 */
void *alloc(unsigned int bytes)
{
   void *ptr;

   if ((ptr = malloc(bytes)))
      return ptr;
   fprintf(stderr, "alloc: ran out of space\n");
   quit(1);

   /* should never reach here */
   return (void *) 0;
}

/*
 * allocstring - allocate space for a string and copy the string to that
 *               location
 */
char *allocstring(char *str)
{
   char *dst;

   dst = (char *) alloc(strlen(str)+1);
   strcpy(dst, str);
   return dst;
}

/*
 * replacestring - replace a dynamically allocated string
 */
void replacestring(char **s1, char *old, char *new)
{
   int i;
   char *p, *s, *d;
   char t[MAXFIELD];

   if (*s1) {
      p = strstr(*s1, old);
      for (s = *s1, d = t; s != p; *d++ = *s++)
         ;
      strcpy(d, new);
      d += strlen(new);
      for (i = 0; i < strlen(old); i++)
         s++;
      while ((*d++ = *s++))
         ;
      if (strlen(old) >= strlen(new))
         strcpy(*s1, t);
      else {
         free(*s1);
         *s1 = allocstring(t);
      }
      return;
   }
   fprintf(stderr, "replacestring - dst string not yet allocated\n");
   quit(1);
}

/*
 * createmove - create a move instruction
 */
void createmove(int type, char *src, char *dst, struct assemline *ptr)
{
   char mnem[MAXFIELD], line [MAXLINE];

   if (type == INT_TYPE)
      strcpy(mnem, "mov");
   else if (type == FLOAT_TYPE)
      strcpy(mnem, "fmovs");
   else if (type == DOUBLE_TYPE)
      strcpy(mnem, "fmovd");
   else {
      fprintf(stderr, "createmove - bad type %d\n", type);
      quit(1);
   }
   sprintf(line, "\t%s\t%s,%s", mnem, src, dst);
   free(ptr->text);
   ptr->text = allocstring(line);
   setupinstinfo(ptr);
}

/*
 * assignlabel - assigns a label to a basic block
 */
void assignlabel(struct bblk *cblk, char *label)
{
   /* assign label */
   if (cblk->label)
      free(cblk->label);
   if (label)
      cblk->label = allocstring(label);
   else
      cblk->label = (char *) NULL;
}

/*
 * newblk - allocate a new basic block
 */
struct bblk *newblk(char *label)
{
   struct bblk *tblk;
  
   /* allocate the space for the block */
   tblk = (struct bblk *) alloc(sizeof(struct bblk));

   /* initialize the fields of the block */
   tblk->label = (char *) NULL;
   assignlabel(tblk, label);
   tblk->num = 0;
   tblk->loopnest = 0;
   tblk->lines = (struct assemline *) NULL;
   tblk->lineend = (struct assemline *) NULL;
   tblk->preds = (struct blist *) NULL;
   tblk->succs = (struct blist *) NULL;
   tblk->up = (struct bblk *) NULL;
   tblk->down = (struct bblk *) NULL;
   tblk->dom = binit();
   tblk->loop = (struct loopnode *) NULL;
   varinit(tblk->uses);
   varinit(tblk->defs);
   varinit(tblk->ins);
   varinit(tblk->outs);
   tblk->status = 0;

   /* return the pointer to the block */
   return tblk;
}

/*
 * freeblk - frees up the space for a basic block
 */
void freeblk(struct bblk *cblk)
{
   struct assemline *ptr, *dptr;

   /* free label */
   if (cblk->label)
      free(cblk->label);

   /* free bit vectors */
   if (cblk->dom)
      bfree(cblk->dom);

   /* free blists */
   freeblist(cblk->preds);
   freeblist(cblk->succs);
   
   /* free assemlines */
   for (ptr = cblk->lines; ptr; ) {
      dptr = ptr;
      ptr = ptr->next;
      freeline(dptr);
   }
}

/*
 * inblist - check if a block is in a blist
 */
int inblist(struct blist *head, struct bblk *cblk)
{
   struct blist *bptr;

   for (bptr = head; bptr; bptr = bptr->next)
      if (bptr->ptr == cblk)
         return TRUE;
   return FALSE;
}

/*
 * freeblist - free up the space for a blist
 */
void freeblist(struct blist *head)
{
   struct blist *bptr, *dptr;

   for (bptr = head; bptr; ) {
      dptr = bptr;
      bptr = bptr->next;
      free(dptr);
   }
}

/*
 * newline - allocate a new assembly line
 */
struct assemline *newline(char *text)
{
   struct assemline *tline;

   /* allocate space for the assembly line */
   tline = (struct assemline *) alloc(sizeof(struct assemline));

   /* initialize the other fields of the assembly line */
   tline->text = allocstring(text);
   tline->next = tline->prev = (struct assemline *) NULL;
   varinit(tline->sets);
   varinit(tline->uses);
   varinit(tline->deads);
   tline->blk = (struct bblk *) NULL;

   /* return the pointer to the assembly line */
   return tline;
}

/*
 * hookupline - hook up the assembly line within the basic block
 */
void hookupline(struct bblk *cblk, struct assemline *ptr,
                struct assemline *line)
{
   /* hook this assembly line into the basic block before ptr */
   if (!ptr) {
      if (!cblk->lineend) {
         cblk->lines = cblk->lineend = line;
         line->prev = line->next = (struct assemline *) NULL;
      }
      else {
         cblk->lineend->next = line;
         line->prev = cblk->lineend;
         line->next = (struct assemline *) NULL;
         cblk->lineend = line;
      }
   }
   else {
      line->next = ptr;
      if (!(line->prev = ptr->prev))
         cblk->lines = line;
      else
         line->prev->next = line;
      ptr->prev = line;
   }
   line->blk = cblk;
}

/*
 * unhookline - unhook an assembly line from a basic block
 */
void unhookline(struct assemline *ptr)
{
   if (ptr->prev)
      ptr->prev->next = ptr->next;
   else
      ptr->blk->lines = ptr->next;
   if (ptr->next)
      ptr->next->prev = ptr->prev;
   else
      ptr->blk->lineend = ptr->prev;
   ptr->next = ptr->prev = (struct assemline *) NULL;
}

/*
 * insline - insert the line before the one in the argument
 */
struct assemline *insline(struct bblk *cblk, struct assemline *ptr, char *text)
{
   struct assemline *tline;

   /* allocate the assembly line */
   tline = newline(text);

   /* hook up the assembly line into the basic block */
   hookupline(cblk, ptr, tline);

   /* return the inserted assembly line */
   return tline;
}

/*
 * delline - delete the specified line in the basic block
 */
void delline(struct assemline *ptr)
{
   unhookline(ptr);
   freeline(ptr);
}

/*
 * freeline - deallocate an assembly line
 */
void freeline(struct assemline *ptr)
{
   int i;

   for (i = 0; i < ptr->numitems; i++)
      free(ptr->items[i]);
   free(ptr->text);
   free(ptr);
}

/*
 * addtoblist - add a basic block to a blist
 */
void addtoblist(struct blist **head, struct bblk *cblk)
{
   struct blist *bptr;

   /* first check that the basic block is not already in the blist */
   for (bptr = *head; bptr; bptr = bptr->next)
      if (bptr->ptr == cblk)
         return;

   /* allocate the space for the blist element */
   bptr = (struct blist *) alloc(sizeof(struct blist));

   /* link in the block at the head of the list */
   bptr->ptr = cblk;
   bptr->next = *head;
   *head = bptr;
}

/*
 * sortblist - sort the blocks in blist by the block number
 */
void sortblist(struct blist *head)
{
   struct blist *bptr, *bptr2;
   struct bblk *tblk;

   for (bptr = head; bptr; bptr = bptr->next)
      for (bptr2 = bptr->next; bptr2; bptr2 = bptr2->next)
         if (bptr->ptr->num > bptr2->ptr->num) {
            tblk = bptr->ptr;
            bptr->ptr = bptr2->ptr;
            bptr2->ptr = tblk;
         }
}

/*
 * orderpreds - make the predecessors of each block be in ascending order
 */
void orderpreds()
{
   struct bblk *cblk;
   extern struct bblk *top;

   for (cblk = top; cblk; cblk = cblk->down)
      sortblist(cblk->preds);
}

/*
 * deleteblk - delete a basic block from the list of basic blocks
 */
void deleteblk(struct bblk *cblk)
{
   extern struct bblk *bot;

   /* update bottom block if needed */
   if (cblk == bot)
      bot = cblk->up;

   /* unhook the "up" and "down" pointers */
   unlinkblk(cblk);

   /* unhook preds */
   delfrompreds_succs(cblk);

   /* unhook succs */
   delfromsuccs_preds(cblk);

   /* free up the memory */
   freeblk(cblk);
}

/*
 * unlinkblk - unhook a basic block from the list of basic blocks
 */
void unlinkblk(struct bblk *cblk)
{
   extern struct bblk *top;

   /* relink a backward pointer to bypass the block to be deleted */
   if (cblk->down)
      cblk->down->up = cblk->up;

   /* set a forward pointer to bypass the block to be deleted */
   if (!cblk->up)
      top = cblk->down;
   else
      cblk->up->down = cblk->down;
}

/*
 * delfrompreds_succs - delete cblk from the successor list of all the
 *                      predecessors of cblk
 */
void delfrompreds_succs(struct bblk *cblk)
{
   struct blist *curpred;

   for (curpred=cblk->preds; curpred; curpred=curpred->next)
      if (!delfromblist(&(curpred->ptr->succs), cblk)) {
         fprintf(stderr, "delfrompreds_succs(), basic block not found.\n");
         quit(1);
      }
}

/*
 * delfromsuccs_preds - delete cblk from the predecessor list of all the  
 *                      successors of cblk
 */
void delfromsuccs_preds(struct bblk *cblk)
{
    struct blist *cursucc;

    for (cursucc=cblk->succs; cursucc; cursucc=cursucc->next)
       if (!delfromblist(&(cursucc->ptr->preds), cblk)) {
          fprintf(stderr, "delfromsuccs_preds(), basic block not found.\n");
          quit(1);
       }
}

/*
 * delfromblist - deletes a block from a blist
 */
struct bblk *delfromblist(struct blist **head, struct bblk *cblk)
{
   struct bblk *tblk;
   struct blist *bptr, *pbptr;

   /* check if the basic block already has been allocated */
   tblk = (struct bblk *) NULL;
   pbptr = (struct blist *) NULL;
   for (bptr = *head; bptr; pbptr = bptr, bptr = bptr->next)
      if (bptr->ptr == cblk) {
         tblk = bptr->ptr;
         if (pbptr)
            pbptr->next = bptr->next;
         else
            *head = bptr->next;
         free(bptr);
         break;
      }
   return tblk;
}

/*
 * newloop - allocate a new loop
 */
struct loopnode *newloop()
{
   struct loopnode *loop;
   extern struct loopnode *loops;

   loop = (struct loopnode *) alloc(sizeof(struct loopnode));
   loop->header = (struct bblk *) NULL;
   loop->preheader = (struct bblk *) NULL;
   loop->blocks = (struct blist *) NULL;
   varinit(loop->invregs);
   varinit(loop->sets);
   loop->anywrites = 0;
   loop->next = loops;
   loops = loop;
   return loop;
}

/*
 * freeloops - free up the loop structures
 */
void freeloops()
{
   struct bblk *cblk;
   struct loopnode *lptr, *dlptr;
   extern struct bblk *top;
   extern struct loopnode *loops;

   /* clean up loop information associated with each basic block */
   for (cblk = top; cblk; cblk = cblk->down) {
      cblk->loop = (struct loopnode *) NULL;
      cblk->loopnest = 0;
      bfree(cblk->dom);
   }

   /* free up information associated with each loop */
   for (lptr = loops; lptr; ) {
      freeblist(lptr->blocks);
      dlptr = lptr;
      lptr = lptr->next;
      free(dlptr);
   }

   /* indicate there is currently no loop information calculated */
   loops = (struct loopnode *) NULL;
}

/*
 * dumploops - dumps the loops in the function to the specified file pointer
 */
void dumploops(FILE *fout)
{
   struct loopnode *lptr;
   void dumploop(FILE *, struct loopnode *);
   extern struct loopnode *loops;

   fprintf(fout, "! loops in function\n");
   for (lptr = loops; lptr; lptr = lptr->next)
      dumploop(fout, lptr);
   fprintf(fout, "\n");
}

/*
 * dumploop - dump a loop in the program to the specified file pointer
 */
void dumploop(FILE *fout, struct loopnode *lptr)
{
   struct blist *bptr;

   fprintf(fout, "!   loop: head = %d\n", lptr->header->num);
   fprintf(fout, "!         blocks =");
   sortblist(lptr->blocks);
   for (bptr = lptr->blocks; bptr; bptr = bptr->next)
      fprintf(fout, " %d", bptr->ptr->num);
   fprintf(fout, "\n");
}

/*
 * incropt - Increment the count of transformations for an optimization.
 *           Calls to this function should be placed immediately before
 *           a transformation for an optimization is about to be applied.
 *           
 */
void incropt(enum opttype opt)
{
   extern int moreopts;
   extern jmp_buf my_env;
   extern struct optinfo totopts[];

   if (totopts[opt].count == totopts[opt].max) {
      moreopts = FALSE;
      longjmp(my_env, 1);
   }
   totopts[opt].count++;
   return;
}

/*
 * quit - exits the program
 */
void quit(int flag)
{
   exit(flag);
}
