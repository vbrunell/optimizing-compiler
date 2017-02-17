/*
 * VJB
 * FSU Department of Computer Science
 * Live and Dead Variable Analysis
 */

#include <stdio.h>
#include "opt.h"
#include "misc.h"
#include "vars.h"
#include "analysis.h"
#include "opts.h"

void checkins(struct bblk * cblk);
int prior_use(struct assemline * lptr);
void rmv_priorsets(struct assemline * lptr, varstate _uses);
void delspecregs(varstate vs);
void markdeath(struct assemline * aptr, struct bblk * blk);
void marklastuse(struct assemline * aptr, struct assemline * origptr);

/*
 * calclivevars - calculate live variable information
 */
void calclivevars()
{
   extern struct bblk* top;
   struct bblk* cblk = (struct cblk*)NULL;
   struct assemline* lnptr = (struct assemline*)NULL;

   varstate _defs;
   varinit(_defs);
   varstate _uses;
   varinit(_uses);
   varstate _tmp;
   varinit(_tmp);

   //////////// calculate uses and defs /////////////
   // for each basic block in the function
   for (cblk = top; cblk; cblk = cblk->down)
   {
      // look at sets and uses for each line
      for(lnptr = cblk->lines; lnptr; lnptr = lnptr->next)
      {
         if(INST(lnptr->type))
         {
            // get block defs
            // disquailfy insts that set and use the same reg
            if(
                !prior_use(lnptr) &&
                !varcommon(lnptr->sets,lnptr->uses)
              )
               unionvar(_defs,_defs,lnptr->sets);

            // get block uses
            varinit(_tmp);
            varcopy(_tmp,lnptr->uses);
            rmv_priorsets(lnptr,_tmp);
            unionvar(_uses,_uses,_tmp);

            // don't use fp,sp,g0
            delspecregs(_uses);
            delspecregs(_defs);

            // add defs and uses info to blk
            varcopy(cblk->uses,_uses);
            varcopy(cblk->defs,_defs);
         }
      }

      // reset defs and uses
      varinit(_defs);
      varinit(_uses);
   }

   ///////////// calculate ins and outs ////////////
   int change = 0;
   struct blist * slist = (struct blist*)NULL;
   varstate _oldin;
   varinit(_oldin);
   varinit(_tmp);

   // for all B, in[B] = empty set
   for (cblk = top; cblk; cblk = cblk->down)
      varinit(cblk->ins);

   // continue updating ins and outs while change in ins
   do
   {
      change = 0;

      // for all B
      for (cblk = top; cblk; cblk = cblk->down)
      {
         // out[B] = empty set
         varinit(cblk->outs);

         // for all immediate succs S of B
         // out[B] = out[B] U in[S]
         if(cblk->succs && cblk->succs->ptr)
            for(slist = cblk->succs; slist; slist = slist->next)
               unionvar(cblk->outs,cblk->outs,slist->ptr->ins);
         else
            ;

         // oldin = in[B]
         varinit(_oldin);
         varcopy(_oldin,cblk->ins);

         // in[B] = use[B] U (out[B] - def[B])
         varcopy(_tmp,cblk->outs);
         minusvar(_tmp,_tmp,cblk->defs);
         unionvar(cblk->ins,_tmp,cblk->uses);

         // if(in[B] != oldin), change = TRUE
         if(!varcmp(cblk->ins,_oldin))
            change = 1;
      }
   }
   while(change);
}

/*
 * calcdeadvars - calculate dead variable information
 */
void calcdeadvars()
{
   extern struct bblk* top;
   struct bblk* cblk = (struct cblk*)NULL;
   struct assemline* lnptr = (struct assemline*)NULL;
   varstate tmp;

   // for each basic block in the function
   for (cblk = top; cblk; cblk = cblk->down)
   {
      // if non-empty blk
      if(cblk->lineend)
      {
         // look at each line
         for(lnptr = cblk->lines; lnptr; lnptr = lnptr->next)
         {
            if(INST(lnptr->type))
            {
               if(lnptr->type == CALL_INST)
                  continue;

               // if var set and used in same inst
               // it's dead at that inst
               if(varcommon(lnptr->sets,lnptr->uses))
               {
                  varinit(tmp);
                  intervar(tmp,lnptr->uses,lnptr->sets);
                  unionvar(lnptr->deads,lnptr->deads,tmp);
               }

               // mark the death of the var set
               if(lnptr->sets)
                  markdeath(lnptr, cblk);

               // remove sp,fp,g0
               delspecregs(lnptr->deads);
            }
         }

         // mark death of ins that have
         // a set in the blk or are not in
         // the outs of the blk
         checkins(cblk);
      }
   }
}

// Mark the death of the ins that are
// set or are not in the outs of the blk
void checkins(struct bblk * cblk)
{
   struct assemline * nptr = (struct assemline*)NULL;
   varstate inscpy;
   varinit(inscpy);
   varstate tmp1;
   varinit(tmp1);
   varstate tmp2;
   varinit(tmp2);

   varcopy(inscpy,cblk->ins);

   // check for a set of an in
   // if found, mark the last use before
   // the set as the death
   for(nptr = cblk->lines; nptr; nptr = nptr->next)
   {
      if(INST(nptr->type))
         if(varcommon(nptr->sets,inscpy))
         {
            minusvar(inscpy,inscpy,nptr->sets);
            marklastuse(nptr,nptr);
         }
   }

   // get ins live at end of blk
   intervar(tmp1,inscpy,cblk->outs);

   // subtract intersection of ins and outs from ins
   // to get the regs that will die in the blk
   minusvar(tmp2,inscpy,tmp1);

   varinit(tmp1);

   // for each in that does not have a set
   // in the blk and is not in the outs
   // search from the bottom of the blk for a use
   for(nptr = cblk->lineend; nptr; nptr = nptr->prev)
   {
      if(INST(nptr->type))
      {
         // if use found, get ins used in the inst
         // add them to the deads for the inst
         // remove the vars from the remaining ins to check
         if(varcommon(nptr->uses,tmp2))
         {
            intervar(tmp1,nptr->uses,tmp2);
            unionvar(nptr->deads,nptr->deads,tmp1);
            minusvar(tmp2,tmp2,tmp1);
         }
      }
   }
}

// Check for prior uses of a set reg/var
int prior_use(struct assemline * lptr)
{
   struct assemline * ptr = (struct assemline*)NULL;

   // for each line above lptr
   for(ptr = lptr->prev; ptr; ptr = ptr->prev)
   {
      // if the reg is used, return true
      if(INST(ptr->type))
         if(varcommon(lptr->sets,ptr->uses))
            return 1;
   }
   return 0;
}

// Remove prior sets of used regs/vars from a varstate
void rmv_priorsets(struct assemline * lptr, varstate _uses)
{
   struct assemline * ptr = (struct assemline*)NULL;

   // Check previous sets with varcommon()
   // If there's a var in common, subtract the set from uses
   for(ptr = lptr->prev; ptr; ptr = ptr->prev)
   {
      if(INST(ptr->type))
         if(varcommon(_uses,ptr->sets))
            minusvar(_uses,_uses,ptr->sets);
   }
}

// Remove the fp, sp, and g0 regs
void delspecregs(varstate vs)
{
   varstate rmregs;

   varinit(rmregs);
   insreg("%fp",rmregs,1);
   insreg("%sp",rmregs,1);
   insreg("%g0",rmregs,1);
   minusvar(vs,vs,rmregs);
}

// mark the death of a reg/var if found
void markdeath(struct assemline * aptr, struct bblk * blk)
{
   struct assemline * nptr = (struct assemline*)NULL;

   // for each line following the line we're checking
   for(nptr = aptr->next; nptr; nptr = nptr->next)
   {
      // locate next set in the blk
      if(varcommon(aptr->sets,nptr->sets))
      {
         // mark the last use as the death
         marklastuse(nptr, aptr);
         return;
      }
   }

   // if no second set in the blk, compare reg/var with outs
   // if in outs, do nothing; reg/var is live leaving the blk
   // else, mark last use in blk as death
   if(varcommon(aptr->sets,blk->outs))
      return;
   else
      marklastuse(blk->lineend, aptr);
}

// mark the last use in a blk as the death
void marklastuse(struct assemline * aptr, struct assemline * origptr)
{
   struct assemline * pptr = (struct assemline*)NULL;

   // for each line above the line we're checking
   for(pptr = aptr; pptr; pptr = pptr->prev)
   {
      // mark the last use in the blk as death
      if(varcommon(origptr->sets,pptr->uses))
      {
         // mark pptr->death
         unionvar(pptr->deads,pptr->deads,origptr->sets);
         return;
      }
   }
}
