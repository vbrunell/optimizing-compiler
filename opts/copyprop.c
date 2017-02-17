/*
 * VJB
 * FSU Department of Computer Science
 * Local Copy Propagation
 */

#include <stdio.h>
#include <string.h>
#include "opt.h"
#include "opts.h"
#include "misc.h"
#include "vars.h"
#include "io.h"

void propagate(char *, char *, struct bblk *, struct assemline *);

/*
 * localcopyprop - perform copy propagation
 */
void localcopyprop(int *changes)
{
   extern struct bblk* top;
   struct bblk* cblk = (struct cblk*)NULL;
   struct assemline* lnptr = (struct assemline*)NULL;

   // for each basic block in the function
   // look at each line
   // find move instructions
   // propagate the src reg if possible
   for (cblk = top; cblk; cblk = cblk->down)
      for (lnptr = cblk->lines; lnptr; lnptr = lnptr->next)
         if(lnptr->type == MOV_INST)
            if(isreg(lnptr->items[1]))
               if(strcmp(lnptr->items[1],lnptr->items[2]) != 0)
                  propagate(lnptr->items[1], lnptr->items[2], cblk, lnptr);

   calclivevars();
   calcdeadvars();
}

// propagate newreg to instructoins in the block that use oldreg
void propagate(char *new, char *old, struct bblk *cblk, struct assemline *mvln)
{
   struct assemline *lnptr = (struct assemline*)NULL;

   // look at uses of oldreg and attempt to replace them with newreg
   if(mvln && mvln->next)
      for (lnptr = mvln->next; lnptr; lnptr = lnptr->next)
      {
         // if the reg we want to replace is used
         // replace it with newreg
         if(    regexists(old,lnptr->uses)
             && lnptr->type != CALL_INST
             && lnptr->type != RETURN_INST )
         {
            incropt(COPY_PROPAGATION);
            replacestring(&(lnptr->text),old,new);
            setupinstinfo(lnptr);
         }

         // if either register is changed, stop propagating
         if(regexists(old,lnptr->sets) || regexists(new,lnptr->sets))
            break;
      }
}
