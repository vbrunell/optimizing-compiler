/*
 * VJB
 * FSU Department of Computer Science
 * Peephole Optimizer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opt.h"
#include "misc.h"
#include "vars.h"
#include "opts.h"
#include "io.h"

extern int numpeeprules;
extern int numrulesapplied[MAXRULES];

#define NUMRLINES 500
#define NUMWILD 10
#define NUMREMLINES 200

char rules[NUMRLINES][MAXLINE];
char wilds[NUMWILD][MAXLINE];
struct assemline * remlines[NUMREMLINES];
int numlines;

// ----------------------------------------------------
int match(char *c, char *d, struct assemline * lnptr);
void initwilds();
void initremlines();
char * getrepline(char * c);
void printwilds();
// ----------------------------------------------------

/*
 * readinrules - read in the peephole rules from the rules file
 */
void readinrules()
{
   FILE * fp = fopen("rules.txt","r");
   char rln[MAXLINE];
   int i = 0;
   numpeeprules = 0;
   numlines = 0;

   // read the rules into rules[]
   while(fgets(rln,MAXLINE,fp))
   {
      strcpy(rules[i],rln);
      //fprintf(stderr,">> %s",rules[i]);
      i++;
      numlines++;

      // count number of rules read in
      if(strcmp(rln,"=\n") == 0)
         numpeeprules++;
   }
}

/*
 * applypeeprules - apply peephole rules to the function
 */
void applypeeprules(int *changes)
{
   //extern int numrulesapplied[MAXRULES];
   extern struct bblk* top;
   struct bblk* cblk = (struct cblk*)NULL;
   struct assemline* lnptr = (struct assemline*)NULL;
   struct assemline* tmptr = (struct assemline*)NULL;
   int i = 0, j = 0, k = 0, rulecnt = 0, ismatch = 0;

   // for each basic block in the function
   // look at each line
   for (cblk = top; cblk; cblk = cblk->down)
      for (lnptr = cblk->lines; lnptr; )
      {
         if(INST(lnptr->type))
         {
            rulecnt = -1;
            // compare the line to match lines in rules[]
            for(i = 0, j = 0; i < numlines; i++)
            {
               ++rulecnt;

               // check the match line against the current assemline
               initwilds();
               initremlines();
               j = 0;
               if(match(rules[i],lnptr->text,lnptr))
               {
                  ismatch = 1;

                  // save a line to be replaced
                  remlines[j] = lnptr;
                  j++;

                  // if first match line matches assemline
                  // check the remaining match lines against subsequent assemlines
                  // until we see a "=" line
                  tmptr = lnptr;
                  for(++i; strcmp(rules[i],"=\n"); i++)
                  {
                     tmptr = tmptr->next;

                     // check if there's a next line to match
                     if(!tmptr && !strcmp(rules[i],"=\n"))
                     {
                        ismatch = 0;
                        break;
                     }

                     // if the line is an instruction
                     if(tmptr && INST(tmptr->type))
                     {
                        if(match(rules[i],tmptr->text,tmptr))
                        {
                           // save a line to be replaced
                           remlines[j++] = tmptr;
                        }
                        else
                        {
                           // no match, so move to next set of match lines
                           for(; strcmp(rules[i],"=\n"); i++);
                           i += 2;
                           ismatch = 0;
                           break;
                        }
                     }
                     // if we see a define or comment, skip it
                     else if(tmptr && (tmptr->type == DEFINE_LINE
                                       || tmptr->type == COMMENT_LINE) )
                     {
                        // stay on current rule
                        i--;

                        // check next assemline
                        continue;
                     }
                     else
                     {
                        // no match, so move to next set of match lines
                        for(; strcmp(rules[i],"=\n"); i++);
                        i += 2;
                        ismatch = 0;
                        break;
                     }
                  }

                  // if we matched all matchlines
                  // insert replacement line and
                  // remove the lines matched
                  if(ismatch)
                  {
                     // ----------------- replace line -------------------

                     // update changes and rules applied count
                     *changes = *changes + 1;
                     numrulesapplied[rulecnt] = numrulesapplied[rulecnt] + 1;
                     rulecnt = -1;

                     // construct replacement line
                     char * repline;
                     repline = getrepline(rules[++i]);

                     // insert replacement line before first line matched
                     lnptr = insline(lnptr->blk,remlines[0],repline);
                     setupinstinfo(remlines[0]->prev);

                     // delete all lines matched
                     for(k = 0; k < j; k++)
                        delline(remlines[k]);

                     // retry all the rules
                     i = 0;
                     continue;

                     // ---------------------------------------------------
                  }
               }
               else
               {
                  // move to next set of match lines
                  for(; strcmp(rules[i],"=\n"); i++);
                  i += 2;
               }
            }
         }
         lnptr = lnptr->next;
      }
}

// match two strings, one with wildcards
// checks register and deads wildcards also
int match(char *c, char *d, struct assemline * lnptr)
{
   int wnum = 0, i = 0;
   char delim, arg[50], tmpstr[] = "X";

   // compare match string to line->text
   while(*d)
   {
      // look for a wildcard
      if(*c == '$')
      {
         //get the wildcard number
         tmpstr[0] = *(c+1);
         wnum = atoi(tmpstr);

         // get the delimiter
         c += 2;
         delim = *c;

         // if there's nothing in the wildcard slot
         // read the argument from the assemline into the wildcard slot
         // else, compare the arg to what's in the wildcard slot
         if(!(wilds[wnum][0]))
         {
            i = 0;
            while(*d && (*d != delim))
               wilds[wnum][i++] = *d++;
            wilds[wnum][i] = '\0';
         }
         else if(wilds[wnum])
         {
            // get the arg from the assemline
            arg[0] = '\0';
            i = 0;
            while(*d && (*d != delim))
               arg[i++] = *d++;
            arg[i] = '\0';

            // compare to the string stored in wildcards array
            // if not the same, return fail
            if(!strcmp(wilds[wnum],arg) == 0)
               return 0;
         }
      }
      // else continue to match chars in the strings
      else if(*c++ != *d++)
         return 0;
   }

   // if we reach the end of the match line
   // and there's more left in the assemline
   // or vice versa, return fail
   if( (*c == '\0' || *c == '\t') && *d )
      return 0;

   if(*c)
   {
      // check deads
      c++;
      while(*c != '\t')
      {
         if(*c == '\0')
            break;

         // look for a wildcard
         if(*c == '$')
         {
            //get the wildcard number
            tmpstr[0] = *(c+1);
            wnum = atoi(tmpstr);

            // if arg is not in deads, no match
            if(!regexists(wilds[wnum],lnptr->deads))
               return 0;
         }
         c++;
      }
   }

   if(*c)
   {
      // check regs
      if(*c == '\t')
      {
         c++;
         while(*c != '\0')
         {
            // look for a wildcard
            if(*c == '$')
            {
               //get the wildcard number
               tmpstr[0] = *(c+1);
               wnum = atoi(tmpstr);

               // if arg is not a reg, no match
               if(!isreg(wilds[wnum]))
                  return 0;
            }
            c++;
         }
      }
   }

   return 1;
}

// initialize the wildcard args
void initwilds()
{
   int i = 0;
   for(; i < NUMWILD; i++)
      wilds[i][0] = '\0';
}

// initialize the lines to be removed
void initremlines()
{
   int i = 0;
   for(; i < NUMREMLINES; i++)
      remlines[i] = NULL;
}

// build the replacement line
char * getrepline(char * c)
{
   int wnum = 0, i = 0;
   char delim, tmpstr[] = "X";
   char *w, *repline = alloc(MAXLINE);

   while(*c)
   {
      // if we see a wildcard
      // replace it in the string with
      // the corresponding argument in wilds[]
      if(*c == '$')
      {
         //get the wildcard number
         tmpstr[0] = *(c+1);
         wnum = atoi(tmpstr);

         // get the delimiter
         c += 2;
         delim = *c;

         // fill in the wildcard
         w = wilds[wnum];
         while(*w)
            repline[i++] = *w++;

         repline[i++] = delim;
         c++;
         continue;
      }

      // continue copying the string
      repline[i++] = *c++;
   }
   repline[i-1] = '\0';

   return repline;
}
