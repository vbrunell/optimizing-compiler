/*
 * main driver for the program
 */
#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>
#include "opt.h"
#include "analysis.h"
#include "flow.h"
#include "io.h"
#include "misc.h"
#include "opts.h"

int swa = FALSE;      /* assembly with no extra comments   */
int swb = FALSE;      /* reverse branches flag             */
int swc = FALSE;      /* branch chaining flag              */
int swd = FALSE;      /* dead assignment elimination flag  */
int swe = FALSE;      /* local cse flag                    */
int swf = FALSE;      /* fill delay slots flag             */
int swm = FALSE;      /* loop-invariant code motion flag   */
int swo = FALSE;      /* copy propagation flag             */
int swp = FALSE;      /* peephole optimization flag        */
int swr = FALSE;      /* register allocation flag          */
int swu = FALSE;      /* unreachable code elimination flag */

int moreopts = TRUE;  /* should more optimizations be performed */
jmp_buf my_env;

/*
 * checkflags - check the optimization flags
 */
void checkflags(char *flags)
{
   int i, j;
   extern struct optinfo totopts[];

   /* check that compilation flags begin with a '-' */
   if (flags[0] != '-') {
      fprintf(stderr,
              "checkflags - optimization flags should begin with '-'\n");
      quit(1);
   }

   /* set all the appropriate flag variables */
   for (i = 1; flags[i]; i++) {
      switch (flags[i]) {
         case 'A':
            swa = TRUE;
            break;

         case 'B':
            swb = TRUE;
            break;

         case 'C':
            swc = TRUE;
            break;

         case 'D':
            swd = TRUE;
            break;

         case 'E':
            swe = TRUE;
            break;

         case 'F':
            swf = TRUE;
            break;

         case 'M':
            swm = TRUE;
            break;

         case 'O':
            swo = TRUE;
            break;

         case 'P':
            swp = TRUE;
            break;

         case 'R':
            swr = TRUE;
            break;

         case 'U':
            swu = TRUE;
            break;

         default:
            fprintf(stderr, "%c is an invalid optimization flag\n", flags[i]);
            quit(1);
      }

      /* set maximum number of transformations for the optimization if it
         is specfied */
      if (isdigit((int) flags[i+1])) {
         for (j = 0; totopts[j].optchar; j++)
            if (totopts[j].optchar == flags[i])
               break;
         if (!totopts[j].optchar) {
            fprintf(stderr, "flag %c not in totopts\n", flags[i]);
            quit(1);
         }
         sscanf(&flags[i+1], "%d", &totopts[j].max);
         for ( ; isdigit((int) flags[i+1]); i++)
            ;
      }
   }
}

/*
 * main - main function for the assembly optimizer
 */
int main(int argc, char *argv[])
{
   int changes, anychanges;
   char *flags = "-BCDEFMOPRU";

   /* read in peephole optimization rules */
   readinrules();

   /* check for flags */
   if (argc == 1)
      checkflags(flags);
   else if (argc == 2)
      checkflags(argv[1]);
   else {
      fprintf(stderr, "main - wrong number of arguments\n");
      quit(1);
   }

   /* process each function in the file */
   while (readinfunc()) {

      /* setup the control flow between the basic blocks */
      setupcontrolflow();

      /* setup to stop applying transformations */
      setjmp(my_env);
      if (moreopts) {

         /* perform branch optimizations */
         if (swc)
            remvbranchchains();
         if (swb)
            reversebranches();

         /* remove unreachable code */
         if (swu)
            unreachablecodeelim();
      }

      /* find the loops in the function */
      numberblks();
      findloops();

      if (moreopts) {

         /* allocate variables to registers */
         if (swr)
            regalloc(&changes);

         /* apply peephole optimization rules */
         calclivevars();
         calcdeadvars();
         if (swp) {
            applypeeprules(&changes);
            if (changes) {
               calclivevars();
               calcdeadvars();
            }
         }

         /* perform local common subexpression elimination */
         if (swe)
            localcse(&changes);

         /* perform copy propagation */
         if (swo) {
            calclivevars();
            calcdeadvars();
            localcopyprop(&changes);
         }

         /* calculate live variable information */
         calclivevars();

         /* remove dead assignments */
         if (swd)
            deadasgelim();

         /* apply peephole optimization rules */
         calclivevars();
         calcdeadvars();
         if (swp) {
            applypeeprules(&changes);
            if (changes) {
               calclivevars();
               calcdeadvars();
            }
         }

         /* repeatedly apply loop-invariant code motion, common subexpression
            elimination, copy propagation, and dead assignment elimination */
         do {
            anychanges = FALSE;
            if (swm) {
               codemotion(&anychanges);
               if (swe) {
                  changes = FALSE;
                  localcse(&changes);
                  if (swo) {
                     calclivevars();
                     calcdeadvars();
                     localcopyprop(&changes);
                     if (swd) {
                        calclivevars();
                        deadasgelim();
                     }
                  }
                  if (changes)
                     anychanges = TRUE;
               }
               if (anychanges) {
                  calclivevars();
                  calcdeadvars();
                  changes = FALSE;
                  if (swp) {
                     applypeeprules(&changes);
                     if (changes) {
                        calclivevars();
                        calcdeadvars();
                     }
                  }
               }
            }
         }
         while (anychanges);

         /* fill delay slots */
         if (swf)
            filldelayslots();
      }

      /* dump out the assembly code */
      dumpfunc();

      /* dump out counts */
      dumpfunccounts();
   }

   /* dump out total counts */
   dumptotalcounts();

   /* dump out number of transformations for each phase */
   dumpoptcounts();

   /* dump out rule usage */
   dumpruleusage();

   /* successfully terminate execution */
   return 0;
}
