/*
 * register and local variable analysis functions
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "opt.h"
#include "misc.h"
#include "vars.h"


/*
 * regstring provides a mapping from a register position to the assembly
 * string representing the register
 */
char *regstring[] = {
  "%g0", "%g1", "%g2", "%g3", "%g4", "%g5", "%g6", "%g7",
  "%o0", "%o1", "%o2", "%o3", "%o4", "%o5", "%sp", "%o7",
  "%l0", "%l1", "%l2", "%l3", "%l4", "%l5", "%l6", "%l7",
  "%i0", "%i1", "%i2", "%i3", "%i4", "%i5", "%fp", "%i7"
};

struct varinfo vars[MAXVARS];   /* variable information */
int numvars;                    /* number of variables  */

/*
 * varinit - initialize a variable state
 */
void varinit(varstate v)
{
   int i;

   for (i = 0; i < NUMVARWORDS; i++)
      v[i] = 0;
}

/*
 * varcmp - returns TRUE if two variable states are identical
 */
int varcmp(varstate v1, varstate v2)
{
   int i;

   for (i = 0; i < NUMVARWORDS; i++)
      if (v1[i] != v2[i])
         return FALSE;
   return TRUE;
}

/*
 * varempty - returns TRUE if a variable state is empty
 */
int varempty(varstate v)
{
   int i;

   for (i = 0; i < NUMVARWORDS; i++)
      if (v[i] != 0)
         return FALSE;
   return TRUE;
}

/*
 * unionvar - union two varstates together and store in a third
 */
void unionvar(varstate vd, varstate v1, varstate v2)
{
   int i;

   for (i = 0; i < NUMVARWORDS; i++)
      vd[i] = v1[i] | v2[i];
}

/*
 * intervar - intersect two varstates together and store in a third
 */
void intervar(varstate vd, varstate v1, varstate v2)
{
   int i;

   for (i = 0; i < NUMVARWORDS; i++)
      vd[i] = v1[i] & v2[i];
}

/*
 * minusvar - subtract one varstate from another and store in a third
 */
void minusvar(varstate vd, varstate v1, varstate v2)
{
   int i;

   for (i = 0; i < NUMVARWORDS; i++)
      vd[i] = v1[i] & ~v2[i];
}

/*
 * varcopy - copy one varstate to another
 */
void varcopy(varstate vd, varstate vs)
{
   int i;

   for (i = 0; i < NUMVARWORDS; i++)
      vd[i] = vs[i];
}

/*
 * varcommon - compare if two varstates have any variables in common
 */
int varcommon(varstate v1, varstate v2)
{
   int i;

   for (i = 0; i < NUMVARWORDS; i++)
      if (v1[i] & v2[i])
         return TRUE;
   return FALSE;
}

/*
 * delreg - delete a register from a variable state
 */
void delreg(char *regstr, varstate vars, int numreg)
{
   int regnum, i;

   /* determine the starting index associated with the register */
   regnum = calcregpos(regstr);
   if (regnum == -1)
      return;

   /* remove the bit(s) associated with the register(s) */ 
   for (i = 0; i < numreg; i++)
      if (regnum+i < 32)
         vars[0] &= ~(1 << (regnum+i));
      else
         vars[1] &= ~(1 << ((regnum+i) - 32));
}

/*
 * delvar - delete a bit associated with a memory variable
 */
void delvar(varstate v, int pos)
{
   v[2] &= ~(1 << pos);
}

/*
 * calcregpos - calculates the register position of a register
 */
int calcregpos(char *regtext)
{
   int num;
   char type;

   if (strcmp(regtext, "%fp") == 0)
      return 30;
   else if (strcmp(regtext, "%sp") == 0)
      return 14;
   else if (sscanf(regtext, "%%%c%d", &type, &num) == 2) {
      if (type == 'g')
         return num;
      else if (type == 'o')
         return num+8;
      else if (type == 'l')
         return num+16;
      else if (type == 'i')
         return num+24;
      else if (type == 'f')
         return num+32;
   }
   return -1;
}

/*
 * isreg - determines if the string is a register
 */
int isreg(char *regtext)
{
   return calcregpos(regtext) != -1;
}

/*
 * insreg - set a bit associated with a register
 */
void insreg(char *reg, varstate v, int numreg)
{
   int num, i;

   /* calculate the starting bit position associated with the register */
   num = calcregpos(reg);
   if (num == -1)
      return;
   if (numreg > 1 && !(*reg == '%' && *(reg+1) == 'f' && 
       isdigit((int) *(reg+2))))
      numreg = 1;

   /* set the bit(s) associated with the register(s) */
   for (i = 0; i < numreg; i++)
      if (num+i < 32)
         v[0] |= (1 << (num+i));
      else
         v[1] |= (1 << ((num+i)-32));
}

/*
 * insvar - set a bit associated with a memory variable
 */
void insvar(varstate v, int pos)
{
   v[2] |= (1 << pos);
}

/*
 * regexists - checks if a register is in a register state
 */
int regexists(char *reg, varstate v)
{
   int num;

   num = calcregpos(reg);
   if (num < 32)
      return v[0] & (1 << num);
   else
      return v[1] & (1 << (num-32));
}

/*
 * setsuses - set the bits associated with the registers set and used in an
 *            instruction
 */
void setsuses(char *text, enum insttype type, int instinfonum, itemarray items,
              int numitems, varstate sets, varstate uses, int numdstregs,
              int numsrcregs)
{
   int i, j;
   char c, *p;
   char fields[4][MAXFIELD], tmp[MAXLINE];
   extern int numvars;
   extern short functype;
   extern struct instinfo insttypes[];

   /* initialize the sets and uses fields */
   varinit(sets);
   varinit(uses);

   /* insert the register sets and uses depending on the type of the
      instruction */
   if (!INST(type))
      return;
   switch (type) {
      case ARITH_INST:
      case CONV_INST:
      case MOV_INST:
         switch (numitems) {
            case 4:
               insreg(items[1], uses, numsrcregs);
               insreg(items[2], uses, numsrcregs);
               insreg(items[3], sets, numdstregs);
               break;

            case 3:
               insreg(items[1], uses, numsrcregs);
               insreg(items[2], sets, numdstregs);
               break;
               
            default:
               fprintf(stderr,
                       "setsuses - should not have %d args for ARITH_INST\n",
                       numitems - 1);
               quit(1);
         }
         break;

      case BRANCH_INST:
         break;

      case CALL_INST:

         /* should set all of the scratch registers since the called
            function could overwrite all of them */
         sets[0] = 0x0000FFFE;
         sets[1] = 0xFFFFFFFF;

         /* set the uses to be all of the arguments registers that are
            passed to the called function */
         j = atoi(items[2]);
         for (i = 0; i < j; i++)
            uses[0] |= (1 << (i+8));
         break;

      case CMP_INST:
         insreg(items[1], uses, numsrcregs);
         insreg(items[2], uses, numsrcregs);
         break;

      case JUMP_INST:
         break;

      case LOAD_INST:
      case STORE_INST:
         if (type == LOAD_INST)
            strcpy(tmp, items[1]);
         else
            strcpy(tmp, items[2]);
         j = 0;
         for (i = 1; tmp[i]; i++)
            if (tmp[i] == '+' || tmp[i] == ']')
               break;
            else
               fields[0][j++] = tmp[i];
         fields[0][j] = '\0';
         j = 0;
         if (tmp[i] == '+')
            for (i++; tmp[i]; i++) {
               if (tmp[i] == ']')
                  break;
               else
                  fields[1][j++] = tmp[i];
            }
         fields[1][j] = '\0';
         if (!tmp[i]) {
            fprintf(stderr, "setsuses - could not find ] in load %s\n", text);
            quit(1);
         }
         insreg(fields[0], uses, numsrcregs);
         insreg(fields[1], uses, numsrcregs);
         if (type == LOAD_INST)
            insreg(items[2], sets, numdstregs);
         else
            insreg(items[1], uses, numsrcregs);
         break;

      case RESTORE_INST:
      case SAVE_INST:
         if (numitems == 4) {
            insreg(items[1], uses, numsrcregs);
            insreg(items[2], uses, numsrcregs);
            insreg(items[3], sets, numsrcregs);
         }
         break;

      case RETURN_INST:
         if (functype == INT_TYPE)
            insreg("%i0", uses, 1);
         else if (functype == FLOAT_TYPE)
            insreg("%f0", uses, 1);
         else if (functype == DOUBLE_TYPE)
            insreg("%f0", uses, 2);
         break;

      case COMMENT_LINE:
      case DEFINE_LINE:
         break;
   }

   /* check if a variable has had its address taken, determine
      the type of the variable, and update the sets and uses
      of the load or store */
   for (i = 0; i < numvars; i++)
      for (j = 1; j < numitems; j++)
         if ((p = strstr(items[j], vars[i].name))) {
            if ((c = *(p+strlen(vars[i].name))) == ']') {
               vars[i].type = insttypes[instinfonum].datatype;
               if (type == LOAD_INST)
                  insvar(uses, i);
               else if (type == STORE_INST)
                  insvar(sets, i);
               else {
                  fprintf(stderr, "setuses - memref not in load or store\n");
                  quit(1);
               }
            }
            else if (!isalnum((int) c) && c != '_')
               vars[i].indirect = TRUE;
         }

}

/*
 * allocreg - allocate a register of a given type
 */
int allocreg(short type, varstate r, char *reg)
{
   int i;
   int noallocate = 0xc000c000;  /* don't allocate %sp, %o7, %fp, %i7 */

   /* find the first available allocable register of a given type,
      copy the string associated with the register, and indicate
      that an available register was found */
   if (type == INT_TYPE) {
      for (i = 1; i < 32; i++)
         if (!(r[0] & (1 << i)) && !(noallocate & (1 << i))) {
            strcpy(reg, regstring[i]);
            return TRUE;
         }
      return FALSE;
   }
   else if (type == FLOAT_TYPE) {
      for (i = 0; i < 32; i++)
         if (!(r[1] & (1 << i))) {
            sprintf(reg, "%%f%d", i);
            return TRUE;
         }
      return FALSE;
   }
   else if (type == DOUBLE_TYPE) {
      for (i = 0; i < 32; i += 2)
         if (!(r[1] & (1 << i)) && !(r[1] & (1 << (i+1)))) {
            sprintf(reg, "%%f%d", i);
            return TRUE;
         }
      return FALSE;
   }
   else {
      fprintf(stderr, "allocreg - invalid register type %d\n", type);
      quit(1);
   }

   /* should never reach here */
   return FALSE;
}

/*
 * varname - return the variable name associated with the position of
 *           of the variable
 */
char *varname(int pos)
{
   return vars[pos].name;
}

/*
 * dumpvarstate - dump the variables that are in the variable state variable
 */
void dumpvarstate(char *out, varstate v)
{
   int i, j;
   char new[MAXVARLINE];

   /* dump the lower word first */
   out[0] = '\0';
   new[0] = '\0';
   for (i = 0; i < 32; i++)
      if (v[0] & (1 << i)) {
         sprintf(new, "%s:", regstring[i]);
         strcat(out, new);
      }

   /* dump the middle word next */
   new[0] = '\0';
   for (i = 0; i < MAXREGS-32; i++)
      if (v[1] & (1 << i)) {
         sprintf(new, "%%f%d:", i);
         strcat(out, new);
      }

   /* dump the upper word next */
   new[0] = '\0';
   for (i = 0; i < numvars; i++)
      if (v[2] & (1 << i)) {
         sprintf(new, "%s:", varname(i));
         j = 0;
         if (new[j] == '.') {
            j++;
            while (isdigit((int) new[j++]))
               ;
         }
         else {
            fprintf(stderr, "dumpvarstate - illegal variable name %s\n", new);
            quit(1);
         }
         strcat(out, &new[j]);
      }
}
