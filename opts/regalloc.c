/*
 * VJB
 * FSU Department of Computer Science
 * Register Allocation
 */

#include <stdio.h>
#include <strings.h>
#include <malloc.h>
#include "opt.h"
#include "misc.h"
#include "vars.h"
#include "opts.h"

void replacememrefs(char * varstr, char * newreg, short itype);
short getvartype(char * varstr);
int isindirect(char * varstr);

/*
 * regalloc - perform register allocation
 */
void regalloc(int *changes)
{
   extern struct bblk* top;
   struct bblk* cblk = (struct cblk*)NULL;
   struct assemline* lnptr = (struct assemline*)NULL;
   char varstr[MAXLINE], newreg[MAXREGCHAR],
        commstr[MAXLINE], regstr[MAXLINE];
   int ind;
   short itype;

   // varstate of unavailable registers with scratch regs
   varstate _usedregs;
   varinit(_usedregs);

   // gather the regs already used in the function
   for (cblk = top; cblk; cblk = cblk->down)
      if(cblk->lineend)
         for(lnptr = cblk->lines; lnptr; lnptr = lnptr->next)
            if(INST(lnptr->type))
            {
               unionvar(_usedregs,_usedregs,lnptr->sets);
               unionvar(_usedregs,_usedregs,lnptr->uses);
            }

   // for each basic block in the function
   for (cblk = top; cblk; cblk = cblk->down)
   {
      // find define lines and replace them with comment lines
      // replace loads and stores using the var with moves
      for(lnptr = cblk->lines; lnptr; lnptr = lnptr->next)
      {
         // check for define lines with vars
         // that are not indirectly referenced
         if(
               lnptr->type == DEFINE_LINE
               && !isindirect(lnptr->items[0])
           )
         {
            // get the variable string
            free(lnptr->text);
            strcpy(varstr,lnptr->text);
            ind = 0;
            while( varstr[ind] != ' ')
               ind++;
            varstr[ind] = 0;

            // get a free reg
            itype = getvartype(varstr);

            // if no reg to allocate, move to next line
            if(!allocreg(itype,_usedregs,newreg))
               continue;

            incropt(REGISTER_ALLOCATION);

            // add allocated reg to unavailable regs
            insreg(newreg,_usedregs,calcregpos(newreg));

            // finish constructing the comment line
            strcpy(commstr,"! ");
            strcat(commstr,varstr);
            sprintf(regstr," = %s",newreg);
            strcat(commstr,regstr);
            lnptr->type = COMMENT_LINE;

            // update line string
            lnptr->text = allocstring(commstr);

            // replace loads and stores with moves
            replacememrefs(varstr,newreg,itype);
         }
      }
   }
}

// get the type of the variable represented by varstr
short getvartype(char * varstr)
{
   int i = 0;
   extern struct varinfo vars[];
   extern int numvars;

   // look through vars[] for a var that matches varstr
   // and return the type
   for(;i<numvars;i++)
      if(strcmp(varstr,vars[i].name) == 0)
         return vars[i].type;

   fprintf(stderr,"\n!! ERROR: varstr %s not found in vars[]\n",varstr);

   return -1;
}

// determine if variable is indirectly referenced
int isindirect(char * varstr)
{
   int i = 0;
   extern struct varinfo vars[];
   extern int numvars;

   // look through vars[] for a var that matches varstr
   // and return whether indirectly referenced or not
   for(;i<numvars;i++)
      if(strcmp(varstr,vars[i].name) == 0)
         if(vars[i].indirect)
            return 1;

   return 0;
}

// replace loads and stores with moves
void replacememrefs(char * varstr, char * newreg, short itype)
{
   extern struct bblk* top;
   struct bblk * cblk;
   struct assemline * lnptr;
   char * c;

   // for each basic block in the function
   for (cblk = top; cblk; cblk = cblk->down)
   {
      // find loads and stores
      for(lnptr = cblk->lines; lnptr; lnptr = lnptr->next)
      {
         // replace loads and stores w/ moves
         if(
               lnptr->type == LOAD_INST
               && (c = strstr(lnptr->text,varstr))
               && (c[strlen(varstr)] == ']')
            )
         {
            createmove(itype,newreg,lnptr->items[2],lnptr);
            lnptr->type = MOV_INST;
         }
         else if(
                  lnptr->type == STORE_INST
                  && (c = strstr(lnptr->text,varstr))
                  && (c[strlen(varstr)] == ']')
                )
         {
            createmove(itype,lnptr->items[1],newreg,lnptr);
            lnptr->type = MOV_INST;
         }
      }
   }
}
