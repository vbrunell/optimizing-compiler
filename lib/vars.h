#define MAXREGCHAR 5   /* maximum number of characters in a register       */
#define MAXREGS 64     /* maximum number of registers                      */
#define MAXVARS 32     /* maximum number of variables in a function        */
#define MAXVARLINE 500 /* maximum number of characters shown in a varstate */

/* variable types */
#define INT_TYPE    1
#define FLOAT_TYPE  2
#define DOUBLE_TYPE 3

void varinit(varstate);
int varcmp(varstate, varstate);
int varempty(varstate);
void unionvar(varstate, varstate, varstate);
void intervar(varstate, varstate, varstate);
void minusvar(varstate, varstate, varstate);
void varcopy(varstate, varstate);
int varcommon(varstate, varstate);
void delreg(char *, varstate, int);
void delvar(varstate, int);
int calcregpos(char *);
int isreg(char *);
void insreg(char *, varstate, int);
void insvar(varstate, int);
int regexists(char *, varstate);
void setsuses(char *, enum insttype, int, itemarray, int, varstate, varstate,
              int, int);
int allocreg(short, varstate, char *);
char *varname(int);
void dumpvarstate(char *, varstate);
