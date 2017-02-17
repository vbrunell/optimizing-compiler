/*
 * VJB
 * FSU Department of Computer Science
 * Loop Invariant Code Motion
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "opt.h"
#include "misc.h"
#include "opts.h"
#include "vars.h"
#include "vect.h"
#include "io.h"

// -------------------------------------------------------------------
struct bblk * getpreheader(struct bblk *, struct loopnode *);
int alwaysexecs(struct bblk *, struct bblk *, struct loopnode *);
int canmove(struct assemline * aptr, varstate _invregs);
int isdivide(struct assemline * aptr);
void unsetstatus2();
// -------------------------------------------------------------------

/*
 * codemotion - perform loop invariant code motion on each loop
 */
void codemotion(int *changes)
{
   extern struct loopnode * loops;
   extern struct instinfo insttypes[];
   struct assemline * aptr = NULL;
   struct assemline * tptr = NULL;
   struct loopnode * lptr;
   struct blist * bptr = NULL;
   char newreg[MAXREGCHAR];
   char tmpstr1[MAXLINE];
   char tmpstr2[MAXLINE];
   char * sptr = NULL;

   varstate _uses;
   varstate _sets;
   varstate _invregs;
   varstate _redinvs;
   varinit(_uses);
   varinit(_sets);
   varinit(_invregs);
   varinit(_redinvs);

   unsetstatus2();

   // for each loop
   for(lptr = loops; lptr; lptr = lptr->next)
   {
      // get the preheader
      lptr->preheader = getpreheader(lptr->header,lptr);

      if(!lptr->preheader)
         continue;

      varinit(_uses);
      varinit(_sets);
      varinit(_invregs);

      // ---------------- find invariant regs ------------------
      // get regs live entering the loop
      unionvar(_uses,_uses,lptr->header->ins);

      // for each blk in the loop
      for(bptr = lptr->blocks; bptr; bptr = bptr->next)
      {
         // for each line in the blk
         for(aptr = bptr->ptr->lines; aptr; aptr = aptr->next)
            if(INST(aptr->type))
            {
               // gather sets and uses
               unionvar(_uses,_uses,aptr->uses);
               unionvar(_sets,_sets,aptr->sets);

               if(aptr->type == STORE_INST || aptr->type == CALL_INST)
                  lptr->anywrites = 1;
            }

         // --- determine which blocks always execute ---
         // set status flag
         if(alwaysexecs(bptr->ptr,lptr->header,lptr))
            bptr->ptr->status = 1;
      }

      // remove sets from uses to get invregs
      minusvar(_invregs,_uses,_sets);

      // update invregs and sets
      varcopy(lptr->invregs,_invregs);
      varcopy(lptr->sets,_sets);
      // ------------------ END: find invariant regs ----------------


      // --------------- update preheader && create moves ---------------
      varstate _alloced;
      varinit(_alloced);

      unionvar(_alloced,_uses,_sets);

      // for each blk in the loop
      for(bptr = lptr->blocks; bptr; bptr = bptr->next)
      {
         // for each line in the blk
         for(aptr = bptr->ptr->lines; aptr; aptr = aptr->next)
         {
            if(INST(aptr->type))
            {
               int ind = aptr->instinfonum;

               // check for load or arith inst
               if( (aptr->type != LOAD_INST) && (aptr->type != ARITH_INST) )
                  continue;

               // if arith_inst, check that doesn't set cc
               if( (aptr->type == ARITH_INST) && (insttypes[ind].setscc) )
                  continue;

               // if load, check that loop has no writes
               if( (aptr->type == LOAD_INST) && (lptr->anywrites) )
                  continue;

               // if exception possible, check that blk always executes
               if( (aptr->type == LOAD_INST) || isdivide(aptr) )
                  if(bptr->ptr->status != 1)
                     continue;

               // check that inst sources are invregs or constants
               if(!canmove(aptr,lptr->invregs))
               {
                  continue;
               }
               else
               {
                  // if no reg to allocate, move to next line
                  if(!allocreg(insttypes[ind].datatype,_alloced,newreg))
                     continue;

                  // add allocated reg to unavailable regs
                  insreg(newreg,_alloced,calcregpos(newreg));
                  insreg(newreg,_redinvs,calcregpos(newreg));

                  strcpy(tmpstr1,aptr->items[aptr->numitems-1]);

                  // setup new assemline for preheader
                  sptr = strstr(aptr->text,aptr->items[aptr->numitems-1]);
                  strncpy(sptr,newreg,3);
                  setupinstinfo(aptr);

                  // insert inst into preheader
                  // insert into line before last if last inst is a jump
                  strcpy(tmpstr2,aptr->text);
                  if(!lptr->preheader->lineend)
                     tptr = insline(lptr->preheader,NULL,tmpstr2);
                  else if(lptr->preheader->lineend->type != JUMP_INST)
                     tptr = insline(lptr->preheader,NULL,tmpstr2);
                  else
                     tptr = insline(lptr->preheader,\
                           lptr->preheader->lineend,tmpstr2);

                  setupinstinfo(tptr);

                  // replace inst in block with move
                  createmove(insttypes[ind].datatype,newreg,tmpstr1,aptr);

                  // perform cse
                  localcse(changes);

                  // remove redundant invariant instructions
                  if(tptr->type == MOV_INST)
                  {
                     // make sure the reg we're propagating is invariant
                     unionvar(_redinvs,_invregs,_redinvs);
                     if(regexists(tptr->items[1],_redinvs))
                     {
                        ind = aptr->instinfonum;
                        createmove(insttypes[ind].datatype,\
                              tptr->items[1],aptr->items[2],aptr);
                        delline(tptr);
                     }
                  }

                  *changes = TRUE;
                  incropt(CODE_MOTION);
               }
            }
         }
      }
      // --------------- END: update preheader && create moves ---------------
   }
}

// find a unique preheader
struct bblk * getpreheader(struct bblk * head, struct loopnode * lptr)
{
   struct bblk * phblk = NULL;
   struct blist * bptr = NULL;
   int cnt = 0;

   // look for preds that are not in the loop blocks
   for(bptr = head->preds; bptr; bptr = bptr->next)
      if(!inblist(lptr->blocks,bptr->ptr))
      {
         phblk = bptr->ptr;
         cnt++;
      }

   // if more than one pred to header outside the loop, fail
   if(cnt > 1)
      return NULL;

   // if preheader has more than one succs, fail
   if(phblk->succs && phblk->succs->next)
      return NULL;

   return phblk;
}

// check if a block always executes or not
int alwaysexecs(struct bblk * cblk, struct bblk * header, struct loopnode * lp)
{
   struct blist * pptr = NULL;
   struct blist * bptr = NULL;
   struct blist * sptr = NULL;

   // check header->preds for the tails
   // the pred must be in the loop to be a tail
   // if the block doesn't dominate a tail, fail
   for(pptr = header->preds; pptr; pptr = pptr->next)
      if(inblist(lp->blocks,pptr->ptr))
         if(!bin(pptr->ptr->dom,cblk->num))
            return 0;

   // for each block in the loop,
   // check its successors for blocks not in the loop
   // these are the exit transition blocks
   // if the cblk doesn't dominate the exit, fail
   for(bptr = lp->blocks; bptr; bptr = bptr->next)
      for(sptr = bptr->ptr->succs; sptr; sptr = sptr->next)
         if(!inblist(lp->blocks,sptr->ptr))
               if(!bin(bptr->ptr->dom,cblk->num))
                  return 0;

   return 1;
}

// check if inst sources are all invregs
int canmove(struct assemline * aptr, varstate _invregs)
{
   varstate _invcheck;
   varinit(_invcheck);

   varcopy(_invcheck,aptr->uses);
   minusvar(_invcheck,_invcheck,_invregs);

   if(!varempty(_invcheck))
      return 0;

   return 1;
}

// check if inst is a divide
int isdivide(struct assemline * aptr)
{
   if(strstr(aptr->items[0],"div"))
      return 1;

   return 0;
}

// reset each block to not visited
void unsetstatus2()
{
    struct bblk *cblk = NULL;
    extern struct bblk *top;

   // reset status
   for (cblk=top; cblk; cblk=cblk->down)
      cblk->status = 0;
}
