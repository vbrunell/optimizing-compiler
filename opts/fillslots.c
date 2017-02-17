/*
 * VJB
 * FSU Department of Computer Science
 * Fill Delay Slots
 */

#include <stdio.h>
#include <string.h>
#include "opt.h"
#include "vars.h"
#include "misc.h"
#include "opts.h"
#include "io.h"

// --------------------------------------------------------------
int noconflict(struct assemline * lptr, struct assemline * tptr);
int hasdelay(struct assemline * ptr);
// --------------------------------------------------------------

/*
 * filldelayslots - fill the delay slots of the transfers of control in a
 *                  function
 */
void filldelayslots()
{
   extern struct bblk *top;
   struct bblk *cblk = NULL;
   struct assemline * lptr = NULL;
   struct assemline * tptr = NULL;
   char str[MAXLINE];

   // look through each block for branches, calls, and jumps
   // look for a line above the inst in the block that has no conflicts
   // fill the delay slot with that inst
   for(cblk = top; cblk; cblk = cblk->down)
      for(lptr = cblk->lines; lptr; lptr = lptr->next)
         if(INST(lptr->type))
            if(hasdelay(lptr))
               for(tptr = lptr->prev; tptr; tptr = tptr->prev)
                  if(INST(tptr->type))
                     if(noconflict(lptr,tptr)) {

                        // fix jump insts so they don't ignore
                        // the delay slot
                        str[0] = '\0';
                        if(lptr && tptr) {
                           if(lptr->type == JUMP_INST) {
                              sprintf(str,"\tba\t%s",lptr->items[1]);
                              strcpy(lptr->text,str);
                              setupinstinfo(lptr);
                           }

                           unhookline(tptr);
                           hookupline(cblk,NULL,tptr);

                           incropt(FILL_DELAY_SLOTS);
                        }

                        break;
                     }
}

// check if an inst can be used to fill a delay slot
int noconflict(struct assemline * lptr, struct assemline * tptr)
{
   extern struct instinfo insttypes[];
   struct assemline * nptr = NULL;
   int ind = 0;
   ind = tptr->instinfonum;

   // don't fill with save insts
   if(tptr->type == SAVE_INST)
      return 0;

   // check for conflicts
   for(nptr = tptr->next; nptr; nptr = nptr->next)
      if(INST(nptr->type))
      {
         if(nptr == lptr && (lptr->type == CALL_INST))
            break;

         if(   varcommon(tptr->sets,nptr->sets)
            || varcommon(tptr->sets,nptr->uses)
            || varcommon(tptr->uses,nptr->sets))
            return 0;

         if(insttypes[ind].setscc)
            return 0;

         if(nptr == lptr)
            break;
      }

   return 1;
}

// does the instruction have a delay slot to fill
int hasdelay(struct assemline * ptr)
{
   if(   ptr->type == BRANCH_INST
      || ptr->type == JUMP_INST
      || ptr->type == CALL_INST)
      return 1;

   return 0;
}
