/*
 * input and output support functions
 */
#include <stdio.h>
#include <string.h>
#include "opt.h"
#include "analysis.h"
#include "misc.h"
#include "io.h"
#include "vars.h"
#include "vect.h"

#define MAXLOOPLEVELS 10

int totnuminsts;               /* total static instructions */
int totnummems;                /* total static memory references */
int totinsts[MAXLOOPLEVELS];   /* total static insts at each loop nest level */
int totmems[MAXLOOPLEVELS];    /* total static mems at each loop nest level */

/* insttypes are used to identify the type of each instruction */
struct instinfo insttypes[] = {
   {"add", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"addcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"and", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"andcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"andn", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"andncc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"ba", JUMP_INST, 1, 0, 0, FALSE, 0},
   {"ba,a", JUMP_INST, 1, 0, 0, FALSE, 0},
   {"be", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"bg", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"bge", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"bgeu", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"bgu", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"bl", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"ble", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"bleu", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"blu", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"bne", BRANCH_INST, 1, 0, 0, FALSE, INT_TYPE},
   {"call", CALL_INST, 2, 0, 1, FALSE, 0},
   {"cmp", CMP_INST, 2, 0, 1, TRUE, INT_TYPE},
   {"fadds", ARITH_INST, 3, 1, 1, FALSE, FLOAT_TYPE},
   {"faddd", ARITH_INST, 3, 2, 2, FALSE, DOUBLE_TYPE},
   {"fbe", BRANCH_INST, 1, 0, 0, FALSE, DOUBLE_TYPE},
   {"fbg", BRANCH_INST, 1, 0, 0, FALSE, DOUBLE_TYPE},
   {"fbge", BRANCH_INST, 1, 0, 0, FALSE, DOUBLE_TYPE},
   {"fbl", BRANCH_INST, 1, 0, 0, FALSE, DOUBLE_TYPE},
   {"fble", BRANCH_INST, 1, 0, 0, FALSE, DOUBLE_TYPE},
   {"fbne", BRANCH_INST, 1, 0, 0, FALSE, DOUBLE_TYPE},
   {"fdivs", ARITH_INST, 3, 1, 1, FALSE, FLOAT_TYPE},
   {"fdivd", ARITH_INST, 3, 2, 2, FALSE, DOUBLE_TYPE},
   {"fdtoi", CONV_INST, 2, 1, 2, FALSE, 0},
   {"fdtos", CONV_INST, 2, 1, 2, FALSE, 0},
   {"fitos", CONV_INST, 2, 1, 1, FALSE, 0},
   {"fitod", CONV_INST, 2, 2, 1, FALSE, 0},
   {"fmovs", MOV_INST, 2, 1, 1, FALSE, FLOAT_TYPE},
   {"fmovd", MOV_INST, 2, 2, 2, FALSE, DOUBLE_TYPE},
   {"fmuls", ARITH_INST, 3, 1, 1, FALSE, FLOAT_TYPE},
   {"fmuld", ARITH_INST, 3, 2, 2, FALSE, DOUBLE_TYPE},
   {"fnegs", ARITH_INST, 2, 1, 1, FALSE, FLOAT_TYPE},
   {"fstod", CONV_INST, 2, 2, 1, FALSE, 0},
   {"fstoi", CONV_INST, 2, 1, 1, FALSE, 0},
   {"fsubs", ARITH_INST, 3, 1, 1, FALSE, FLOAT_TYPE},
   {"fsubd", ARITH_INST, 3, 2, 2, FALSE, DOUBLE_TYPE},
   {"ld", LOAD_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"ldd", LOAD_INST, 2, 2, 1, FALSE, DOUBLE_TYPE},
   {"ldf", LOAD_INST, 2, 1, 1, FALSE, FLOAT_TYPE},
   {"ldsb", LOAD_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"ldsh", LOAD_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"ldub", LOAD_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"lduh", LOAD_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"mov", MOV_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"or", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"orcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"orn", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"orncc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"restore", RESTORE_INST, 0, 0, 0, FALSE, 0},
   {"restore", RESTORE_INST, 3, 1, 1, FALSE, 0},
   {"ret", RETURN_INST, 0, 0, 0, FALSE, 0},
   {"retl", RETURN_INST, 0, 0, 0, FALSE, 0},
   {"save", SAVE_INST, 3, 1, 1, FALSE, 0},
   {"sdiv", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"sdivcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"sethi", ARITH_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"sll", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"smul", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"smulcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"sra", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"srl", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"st", STORE_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"stb", STORE_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"std", STORE_INST, 2, 1, 2, FALSE, DOUBLE_TYPE},
   {"stf", STORE_INST, 2, 1, 1, FALSE, FLOAT_TYPE},
   {"sth", STORE_INST, 2, 1, 1, FALSE, INT_TYPE},
   {"sub", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"subcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"umul", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"umulcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"udiv", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"udivcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"xor", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"xorcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"xnor", ARITH_INST, 3, 1, 1, FALSE, INT_TYPE},
   {"xnorcc", ARITH_INST, 3, 1, 1, TRUE, INT_TYPE},
   {"", 0, 0, 0, 0, 0, 0}
};

/* totopts are used to track the number of transformations for each phase */
struct optinfo totopts[] = {
   { 0, -1, 'B', "reverse branches" },
   { 0, -1, 'C', "branch chaining" },
   { 0, -1, 'D', "dead assignment elimination" },
   { 0, -1, 'E', "local cse" },
   { 0, -1, 'F', "fill delay slots" },
   { 0, -1, 'M', "loop-invariant code motion" },
   { 0, -1, 'O', "copy propagation" },
   { 0, -1, 'P', "peephole optimization" },
   { 0, -1, 'R', "register allocation" },
   { 0, -1, 'U', "unreachable code elimination" },
   { 0, 0, '\0', "" }
};

   

struct bblk *top = (struct bblk *) NULL;  /* top block in the function    */
struct bblk *bot = (struct bblk *) NULL;  /* end block in the function    */
char funcname[MAXFIELD];                  /* name of the current function */
short functype;                           /* type of the current function */
int numpeeprules;                         /* number of peephole rules     */
int numrulesapplied[MAXRULES];            /* count each rule is applied   */

/*
 * classifyinst - classify an instruction
 */
void classifyinst(short numitems, itemarray items, enum insttype *type,
                  int *instinfonum, int *numdstregs, int *numsrcregs)
{
   int i;

   /* search through the list of known instructions */
   for (i = 0; insttypes[i].mneumonic[0]; i++)
      if (numitems-1 == insttypes[i].numargs &&
          strcmp(insttypes[i].mneumonic, items[0]) == 0) {
         *type = insttypes[i].type;
         *instinfonum = i;
         *numdstregs = insttypes[i].numdstregs;
         *numsrcregs = insttypes[i].numsrcregs;
         return;
      }
   fprintf(stderr, "classifyinst - %s is unknown instruction\n", items[0]);
   quit(1);
}

/*
 * reclassifyinsts - reclassify the instructions
 */
void reclassifyinsts()
{
   struct bblk *cblk;
   struct assemline *ptr;
   extern struct bblk *top;

   for (cblk = top; cblk; cblk = cblk->down) {
      for (ptr = cblk->lines; ptr; ptr = ptr->next)
         if (INST(ptr->type))
            setupinstinfo(ptr);
   }
}

/*
 * makeinstitems - make the items associated with an instruction
 */
void makeinstitems(char *text, short *numitems, itemarray *items, int strip)
{
   int i, j, commas, tabs;
   char args[4][MAXFIELD], inst[MAXLINE];

   /* strip out the blanks */
   j = 0;
   for (i = 0; text[i]; i++)
      if (text[i] != ' ' || !strip)
         inst[j++] = text[i];
   inst[j] = '\0';

   /* replace the commas with tabs */
   commas = 0;
   tabs = 0;
   for (i = 0; inst[i]; i++)
      if (inst[i] == '\t')
         tabs++;
      else if (tabs == 2 && inst[i] == ',') {
         commas++;
         inst[i] = '\t';
      }
      
   /* setup the arguments in the instruction */
   if (commas == 2 &&
       sscanf(inst, "\t%s\t%s\t%s\t%s", args[0], args[1], args[2], args[3])
       == 4)
      *numitems = 4;
   else if (commas == 1 &&
            sscanf(inst, "\t%s\t%s\t%s", args[0], args[1], args[2]) == 3)
      *numitems = 3;
   else if (commas == 0 &&
            sscanf(inst, "\t%s\t%s", args[0], args[1]) == 2)
      *numitems = 2;
   else if (commas == 0 && sscanf(inst, "\t%s", args[0]) == 1)
      *numitems = 1;
   else if (commas == 0 && sscanf(text, "%s = %s", args[0], args[1]) == 2)
      *numitems = 2;
   *items = (itemarray) alloc(*numitems * sizeof(char *));
   for (i = 0; i < *numitems; i++)
      (*items)[i] = allocstring(args[i]);
}

/*
 * setupinstinfo - setup instruction information
 */
void setupinstinfo(struct assemline *ptr)
{
   int numdstregs, numsrcregs;

   makeinstitems(ptr->text, &ptr->numitems, &ptr->items, TRUE);
   classifyinst(ptr->numitems, ptr->items, &ptr->type, &ptr->instinfonum,
                &numdstregs, &numsrcregs);
   setsuses(ptr->text, ptr->type, ptr->instinfonum, ptr->items, ptr->numitems,
            ptr->sets, ptr->uses, numdstregs, numsrcregs);
}

/*
 * readinfunc - read in the function and store the instructions into basic
 *              blocks
 */
int readinfunc()
{
   int n;
   struct assemline *ptr;
   struct bblk *tblk;
   char line[MAXLINE], items[2][MAXFIELD];
   char *status;
   extern int numvars;
   extern struct varinfo vars[];

   /* print out a pending switch to the data segment */
   printf("\t.seg\t\"data\"\n");

   /* read in and dump out directives until read the text segment */
   while ((status = fgets(line, MAXLINE, stdin))) {
      printf("%s", line);
      if (strcmp(line, "\t.seg\t\"text\"\n") == 0)
         break;
   }
   if (!status)
      return FALSE;

   /* check if we are at the beginning of a function */
   fgets(line, MAXLINE, stdin);
   if (strcmp(line, "\t.align\t8\n") != 0) {
      fprintf(stderr, "expecting\n\t.align\t8\n and got\n%s", line);
      quit(1);
   }
   printf("%s", line);
   fgets(line, MAXLINE, stdin);
   if (strncmp(line, "\t.global", 8) != 0) {
      fprintf(stderr, "expecting\n\t.global\t%%s\n and got\n%s", line);
      quit(1);
   }
   printf("%s", line);
   fgets(line, MAXLINE, stdin);
   if (sscanf(line, "\t.proc\t%d", &n) != 1) {
      fprintf(stderr, "expecting\n\t.proc\n and got\n%s", line);
      quit(1);
   }
   printf("%s", line);
   fgets(line, MAXLINE, stdin);
   if (line[strlen(line)-2] != ':') {
      fprintf(stderr, "readinfunc - expecting label and got\n%s", line);
      quit(1);
   }

   /* setup function name and type */
   strcpy(funcname, line);
   funcname[strlen(funcname)-2] = '\0';
   switch (n) {
      case 0:
         functype = 0;
         break;

      case 6:
         functype = FLOAT_TYPE;
         break;

      case 7:
         functype = DOUBLE_TYPE;
         break;

      default:
         functype = INT_TYPE;
         break;
   }

   /* read in the instructions for the function */
   top = bot = newblk(funcname);
   numvars = 0;
   while ((status = fgets(line, MAXLINE, stdin))) {
      line[strlen(line)-1] = '\0';

      /* check if we are at the end of the function */
      if (strcmp(line, "\t.seg\t\"data\"") == 0) {

         /* clean up last empty block */
         if (!bot->lines) {
            tblk = bot;
            bot = bot->up;
            deleteblk(tblk);
         }
         return TRUE;
      }

      /* ignore nop instructions */
      if (strcmp(line, "\tnop") == 0)
         continue;

      /* store a comment line */
      else if (line[0] == '!') {
         ptr = insline(bot, (struct assemline *) NULL, line);
         ptr->type = COMMENT_LINE;
      }

      /* store a define line */
      else if (sscanf(line, "%s = %s", items[0], items[1]) == 2) {
         ptr = insline(bot, (struct assemline *) NULL, line);
         ptr->type = DEFINE_LINE;
         makeinstitems(ptr->text, &ptr->numitems, &ptr->items, 0);
         if (numvars == MAXVARS) {
            fprintf(stderr, "readinfunc - too many variables in %s\n",
                    funcname);
            quit(1);
         }
         vars[numvars].name = allocstring(ptr->items[0]);
         vars[numvars].type = 0;
         vars[numvars].indirect = FALSE;
         numvars++;
      }

      /* if a label, then start a new block */
      else if (line[0] == '.' && line[1] == 'L') {
         line[strlen(line)-1] = '\0';
         if (bot->lines || bot->label) {
            tblk = newblk(line);
            tblk->up = bot;
            bot->down = tblk;
            bot = tblk;
         }
         else
            assignlabel(bot, line);
      }

      /* else an instruction */
      else {
         ptr = insline(bot, (struct assemline *) NULL, line);
         setupinstinfo(ptr);
         if ((TOC(ptr->type) && ptr->type != RETURN_INST) ||
             ptr->type == RESTORE_INST) {
            tblk = newblk((char *) NULL);
            tblk->up = bot;
            bot->down = tblk;
            bot = tblk;
         }
      }
   }
   if (!status) {
      fprintf(stderr, "unexpected end of file after function\n");
      quit(1);
   }

   /* clean up last empty block */
   if (!bot->lines) {
      tblk = bot;
      bot = bot->up;
      deleteblk(tblk);
   }

   /* indicate that a function was read in */
   return TRUE;
}

/*
 * dumpblk - dumps the information for a block
 */
void dumpblk(FILE *fout, struct bblk *cblk)
{
   int i;
   struct blist *bptr;
   struct assemline *ptr;
   char vartext[MAXVARLINE], new[MAXLINE];
   extern int swa;
#define COMMENT_START 20
#define USES_START    (COMMENT_START+13)
#define DEADS_START   (USES_START+14)

   /* print out predecessor and successor information */
   if (!swa) {
      fprintf(fout, "!\n");
      fprintf(fout, "! block %d\n", cblk->num);
      fprintf(fout, "! preds:");
      for (bptr = cblk->preds; bptr; bptr = bptr->next)
         fprintf(fout, " %d", bptr->ptr->num);
      fprintf(fout, "\n");
      fprintf(fout, "! succs:");
      for (bptr = cblk->succs; bptr; bptr = bptr->next)
         fprintf(fout, " %d", bptr->ptr->num);
      fprintf(fout, "\n");
      fprintf(fout, "!  doms:");
      bdump(fout, cblk->dom);
      fprintf(fout, "\n");
      fprintf(fout, "!   ins=");
      dumpvarstate(vartext, cblk->ins);
      fprintf(fout, "%s\n", vartext);
      fprintf(fout, "!  outs=");
      dumpvarstate(vartext, cblk->outs);
      fprintf(fout, "%s\n", vartext);
      fprintf(fout, "!\n");
   }

   /* print out label */
   if (cblk->label)
      fprintf(fout, "%s:\n", cblk->label);

   /* for each assembly line in the block */
   for (ptr = cblk->lines; ptr; ptr = ptr->next) {

      /* print out the text of the assembly line */
      fprintf(fout, "%s", ptr->text);

      /* if the line contains an instruction */
      if (INST(ptr->type) && !swa) {

         /* line up sets after the instruction */
         new[0] = '\0';
         for (i = 1; ptr->text[i]; i++)
            if (ptr->text[i] == '\t') {
               i++;
               break;
            }
         if (!ptr->text[i])
            fprintf(fout, "\t");
         i = strlen(&ptr->text[i]);
         for (; i < COMMENT_START; i++)
            strcat(new, " ");
         fprintf(fout, new);

         /* print out the sets */
         if (ptr->type == CALL_INST) {
            strcpy(vartext, "scratch");
            fprintf(fout, "! sets=%s", vartext);
         }
         else if (ptr->type == SAVE_INST || ptr->type == RESTORE_INST) {
            strcpy(vartext, "window");
            fprintf(fout, "! sets=%s", vartext);
         }
         else {
            dumpvarstate(vartext, ptr->sets);
            fprintf(fout, "! sets=%s", vartext);
         }

         /* line up uses after the sets */
         new[0] = '\0';
         for (i = COMMENT_START+strlen("sets=")+strlen(vartext);
              i < USES_START; i++)
            strcat(new, " ");
         fprintf(fout, new);

         /* print out the uses */
         dumpvarstate(vartext, ptr->uses);
         fprintf(fout, "uses=%s", vartext);

         /* line up deads after the uses */
         new[0] = '\0';
         for (i = USES_START+strlen("uses=")+strlen(vartext);
              i < DEADS_START; i++)
            strcat(new, " ");
         fprintf(fout, new);

         /* print out the deads */
         dumpvarstate(vartext, ptr->deads);
         fprintf(fout, "deads=%s", vartext);
      }

      /* print out the carriage return */
      fprintf(fout, "\n");
      if ((ptr->type == CALL_INST || ptr->type == BRANCH_INST ||
           ptr->type == RETURN_INST) && !ptr->next)
         fprintf(fout, "\tnop\n");
   }
}

/*
 * dumpoutblks - dumps a range of blocks to a file pointer
 */
void dumpoutblks(FILE *fout, unsigned int num1, unsigned int num2)
{
   struct bblk *cblk;
   void dumpblk(FILE *, struct bblk *);
   extern struct bblk *top;
 
   /* find the start block */
   for (cblk = top; cblk; cblk = cblk->down)
      if (cblk->num == num1)
         break;
 
   /* check if no more blocks */
   if (!cblk) {
      fprintf(fout, "no blocks in the range %d to %d\n", num1, num2);
      return;
   }
 
   /* print each block in the range */
   for (; cblk; cblk = cblk->down)
      if (cblk->num <= num2)
         dumpblk(fout, cblk);
      else
         break;
}

/*
 * dumpblks - diagnostic function to dump out a range of blocks
 */
void dumpblks(int num1, int num2)
{
   dumpoutblks(stderr, num1, num2);
}

/*
 * dumpfunc - write out the function to stdout
 */
void dumpfunc()
{
   struct bblk *cblk;
   extern int swa;

   /* dump out loop information */
   if (!swa)
      dumploops(stdout);

   /* order the preds in each block */
   orderpreds();

   /* print out each block in the program */
   for (cblk = top; cblk; cblk = cblk->down)
      dumpblk(stdout, cblk);
}

/*
 * dumpruleusage - dump out how many times each rule was applied
 */
void dumpruleusage()
{
   int i, n;

   /* print out which peephole optimization rules were applied */
   n = 0;
   for (i = 0; i < numpeeprules; i++)
      n += numrulesapplied[i];
   fprintf(stderr, "\nNumber of peeprules is %d, applied %d times.\n", 
           numpeeprules, n);
   for (i = 0; i < numpeeprules; i++)
      if (numrulesapplied[i] > 0) {
         fprintf(stderr, "Peephole rule %d applied %d times.\n",
                 i+1, numrulesapplied[i]);
      }
}

/*
 * dumpfunccounts - dump out the number of instructions and memory references
 *                  for the function
 */
void dumpfunccounts()
{
   int i, numinsts, nummems;
   int insts[MAXLOOPLEVELS];
   int mems[MAXLOOPLEVELS];
   struct bblk *cblk;
   struct assemline *ptr;
   static int first = TRUE;

   numinsts = nummems = 0;
   for (i = 0; i < MAXLOOPLEVELS; i++)
      insts[i] = mems[i] = 0;
   for (cblk = top; cblk; cblk = cblk->down)
      for (ptr = cblk->lines; ptr; ptr = ptr->next)
         if (INST(ptr->type)) {
            numinsts++;
            insts[cblk->loopnest]++;
            if (ptr->type == LOAD_INST || ptr->type == STORE_INST) {
               nummems++;
               mems[cblk->loopnest]++;
            }
            if ((ptr->type == CALL_INST || ptr->type == BRANCH_INST ||
                ptr->type == RETURN_INST) && !ptr->next) {
               numinsts++;
               insts[cblk->loopnest]++;
            }
         }
   if (first) {
      fprintf(stderr, "function     level instructions memory refs\n");
      fprintf(stderr, "------------ ----- ------------ -----------\n");
      first = FALSE;
   }
   fprintf(stderr, "%-12s total %12d %11d\n", funcname, numinsts, nummems);
   totnuminsts += numinsts;
   totnummems += nummems;
   if (insts[1] > 0)
      for (i = 0; i < MAXLOOPLEVELS && insts[i] > 0; i++) {
         totinsts[i] += insts[i];
         totmems[i] += mems[i];
         fprintf(stderr, "             %5d %12d %11d\n", i, insts[i], mems[i]);
      }
   else {
      totinsts[0] += insts[0];
      totmems[0] += mems[0];
   }
}

/*
 * dumptotalcounts - dump out the number of instructions and memory references
 *                   for the entire file
 */
void dumptotalcounts()
{
   int i;

   fprintf(stderr, "------------ ----- ------------ -----------\n");
   fprintf(stderr, "%-12s total %12d %11d\n", "program", 
           totnuminsts, totnummems);
   if (totinsts[1] > 0)
      for (i = 0; i < MAXLOOPLEVELS && totinsts[i] > 0; i++) {
         fprintf(stderr, "             %5d %12d %11d\n", 
                 i, totinsts[i], totmems[i]);
      }
}

/*
 * dumpoptcounts - dump out the number of transformations applied for 
 *                 each optimization
 */
void dumpoptcounts()
{
   int i, n;

   fprintf(stderr, "\n");
   n = 0;
   for (i = 0; totopts[i].optchar; i++)
      if (totopts[i].count) {
         n += totopts[i].count;  
         fprintf(stderr, "%3d transformations applied by %s phase.\n",
                 totopts[i].count, totopts[i].name);
      }
   fprintf(stderr, "---\n");
   fprintf(stderr, "%3d transformations applied by all optimization phases.\n",
           n);
}
