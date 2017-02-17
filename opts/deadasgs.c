/*
 * VJB
 * FSU Department of Computer Science
 * Dead Assignment Elimination
 */

#include <stdio.h>
#include "opt.h"
#include "vars.h"
#include "analysis.h"
#include "opts.h"
#include "misc.h"

int hasuse(varstate _set, struct assemline * lnptr);
void getuses(struct bblk *, struct bblk *, varstate, varstate *);
void unsetstatus();

/*
 * deadasgelim - perform dead assignment elimination
 */
void deadasgelim()
{
   extern struct bblk* top;
   struct bblk* cblk = NULL;
   struct assemline* tmpln = NULL;
   struct assemline* lnptr = NULL;
   varstate _ignregs;
   varinit(_ignregs);

   // set varstate of regs to ignore
   insreg("%fp",_ignregs,1);
   insreg("%sp",_ignregs,1);
   insreg("%g0",_ignregs,1);

   // for each basic block in the function
   // look at lines that set regs
   for(cblk = top; cblk; cblk = cblk->down)
      for(lnptr = cblk->lines; lnptr; )
      {
         if(      INST(lnptr->type)
               && !TOC(lnptr->type)
               && lnptr->type != SAVE_INST
               && lnptr->type != RESTORE_INST
           )
            // for each reg set that's not in ignregs
            // search the current block and the succs for a use
            // if not found, remove the line
            if(!varempty(lnptr->sets) && !varcommon(lnptr->sets,_ignregs))
            {
               if(!hasuse(lnptr->sets, lnptr))
               {
                  incropt(DEAD_ASG_ELIM);
                  tmpln = lnptr->next;
                  delline(lnptr);
                  lnptr = tmpln;
                  continue;
               }
            }

         lnptr = lnptr->next;
      }

   calclivevars();
   calcdeadvars();
}

// check for a use of the reg set
// if found return true, else return false
int hasuse(varstate _set, struct assemline * setln)
{
   struct assemline* lnptr = NULL;
   varstate _uses;
   varinit(_uses);

   unsetstatus();

   // look through the rest of the block containing
   // the reg set for a use of the reg
   if(setln && setln->next)
      for(lnptr = setln->next; lnptr; lnptr = lnptr->next)
      {
         // if use found, return true
         if(varcommon(lnptr->uses,_set))
            return 1;

         // if reg set before use, return false
         if(varcommon(lnptr->sets,_set))
            return 0;
      }

   // if the set reg is in outs, return true
   if(varcommon(setln->blk->outs,_set))
      return 1;

   // look for uses until the reg is set again
   // or until there are no more succs
   getuses(setln->blk,setln->blk,_set,&(_uses));

   // if the reg set is used, return true
   if(varcommon(_set,_uses))
      return 1;

   return 0;
}

// recursively look through succs for a set or use of the reg
void getuses(struct bblk *cb, struct bblk *ob, varstate _set, varstate *_uses)
{
   struct blist * succs = NULL;
   struct assemline* lnptr = NULL;

   // if we visited this block
   // end the recursive search
   if(cb->status)
      return;

   // mark the block as visited
   cb->status = 1;

   // if not the original block
   // look through lines for sets and uses of the reg
   if(cb != ob)
      for(lnptr = cb->lines; lnptr; lnptr = lnptr->next)
      {
         // look for uses
         if(varcommon(lnptr->uses,_set))
            unionvar(*_uses,*_uses,lnptr->uses);

         // stop looking if the reg is set
         if(varcommon(lnptr->sets,_set))
            return;
      }

   // if the set reg isn't in the outs of the block, stop
   if(!varcommon(cb->outs,_set))
      return;

   // if no successors, stop
   if(!cb->succs)
      return;

   // for each succ, look for uses of the reg
   for(succs=cb->succs; succs; succs = succs->next)
      getuses(succs->ptr,ob,_set,_uses);
}

// reset each block to not visited
void unsetstatus()
{
    struct bblk *cblk = NULL;
    extern struct bblk *top;

   // reset status
   for (cblk=top; cblk; cblk=cblk->down)
      cblk->status = 0;
}
