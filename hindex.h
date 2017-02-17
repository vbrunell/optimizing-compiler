
=======================================
flow.h
=======================================

void numberblks();
struct bblk *findblk(char *);
void setupcontrolflow();
void clearstatus();
void check_cf();

=======================================
io.h
=======================================

void classifyinst(short, itemarray, enum insttype *, int *, int *, int *);
void reclassifyinsts();
void makeinstitems(char *, short *, itemarray *, int);
void setupinstinfo(struct assemline *);
int readinfunc();
void dumpblk(FILE *, struct bblk *);
void dumpoutblks(FILE *, unsigned int, unsigned int);
void dumpblks(int, int);
void dumpfunc();
void dumpruleusage();
void dumpfunccounts();
void dumptotalcounts();
void dumpoptcounts();

=======================================
misc.h
=======================================

void *alloc(unsigned int);
char *allocstring(char *);
void replacestring(char **, char *, char *);
void createmove(int, char *, char *, struct assemline *);
void assignlabel(struct bblk *, char *);
struct bblk *newblk(char *);
void freeblk(struct bblk *);
int inblist(struct blist *, struct bblk *);
void freeblist(struct blist *);
struct assemline *newline(char *);
void hookupline(struct bblk *, struct assemline *, struct assemline *);
void unhookline(struct assemline *);
struct assemline *insline(struct bblk *, struct assemline *, char *);
void delline(struct assemline *);
void freeline(struct assemline *);
void addtoblist(struct blist **, struct bblk *);
void sortblist(struct blist *);
void orderpreds();
void deleteblk(struct bblk *);
void unlinkblk(struct bblk *);
void delfrompreds_succs(struct bblk *);
void delfromsuccs_preds(struct bblk *);
struct bblk *delfromblist(struct blist **, struct bblk *);
struct loopnode *newloop();
void freeloops();
void dumploops(FILE *);
void dumploop(FILE *, struct loopnode *);
void incropt(enum opttype);
void quit(int);

=======================================
opt.h
=======================================

// main include file for the opt program

#define NUMVARWORDS   3      /* number of words required to represent the
                                registers and variables on the SPARC     */
#define MAXFIELD     30      /* maximum characters in a field            */
#define MAXLINE      81      /* maximum characters in an assembly line   */
#define LOG2_INT      5      /* sizeof(int) = 32 = 2**5                  */
#define INT_REM      31      /* if (v & INT_REM) v is not an integer
                                multiple of sizeof(int)*4                */
#define ARGSIZE      20      /* maximum argument size for peephole rule  */
#define MAXRULES    100      /* maximum number of peephole rules         */

#define FALSE         0
#define TRUE          1

/* block status bits */
#define DONE          (1 << 0)

/* used to initialize bvect's */
#define binit() ((bvect) NULL)

/* used to allocate space for a basic block vector */
#define BALLOC  ((unsigned int *) malloc(sizeof(unsigned int)*bvectlen))

/*
 * block membership bit vector
 */
typedef unsigned int *bvect;

/*
 * variable state structure
 */
typedef unsigned int varstate[NUMVARWORDS];

/*
 * item array type
 */
typedef char **itemarray;

/* instruction types */
enum insttype {
   ARITH_INST,
   BRANCH_INST,
   CALL_INST,
   CMP_INST,
   CONV_INST,
   JUMP_INST,
   LOAD_INST,
   MOV_INST,
   RESTORE_INST,
   RETURN_INST,
   SAVE_INST,
   STORE_INST,
   COMMENT_LINE,
   DEFINE_LINE
};

/* optimization types */
enum opttype {
   REVERSE_BRANCHES,
   BRANCH_CHAINING,
   DEAD_ASG_ELIM,
   LOCAL_CSE,
   FILL_DELAY_SLOTS,
   CODE_MOTION,
   COPY_PROPAGATION,
   PEEPHOLE_OPT,
   REGISTER_ALLOCATION,
   UNREACHABLE_CODE_ELIM
};

#define TOC(t)  (t == BRANCH_INST || t == CALL_INST || t == JUMP_INST || \
                 t == RETURN_INST)

#define INST(t) (t != COMMENT_LINE && t != DEFINE_LINE)

/*
 * assembly line structure
 */
struct assemline {
   char *text;                 /* text of the assembly line               */
   struct assemline *next;     /* next assembly line                      */
   struct assemline *prev;     /* previous assembly line                  */
   enum insttype type;         /* type of assembly line                   */
   int instinfonum;            /* index into instruction information      */
   short numitems;             /* number of strings for instruction       */
   itemarray items;            /* list of strings for the line tokens     */
   varstate sets;              /* variables updated by the instruction    */
   varstate uses;              /* variables used by the instruction       */
   varstate deads;             /* variable values that are last used by
                                  the instruction                         */
   struct bblk *blk;           /* block containing the assembly line      */
};

/*
 * basic block structure
 */
struct bblk {
   char *label;                /* label of basic block                     */
   unsigned short num;         /* basic block number                       */
   short loopnest;             /* loop nesting level                       */
   struct assemline *lines;    /* first line in a basic block              */
   struct assemline *lineend;  /* last line in a basic block               */
   struct blist *preds;        /* list of predecessors for a basic block   */
   struct blist *succs;        /* List of successors for a basic block.
                                  The first block in the list is the
                                  fall-through successor when the block
                                  ends with a conditional branch.          */
   struct bblk *up;            /* positionally previous basic block        */
   struct bblk *down;          /* positionally following basic block       */
   bvect dom;                  /* blocks that dominate this one            */
   struct loopnode *loop;      /* set if this block is a loop header       */
   varstate uses;              /* variables used before being set          */
   varstate defs;              /* variables set in this block before used  */
   varstate ins;               /* variables live entering the block        */
   varstate outs;              /* variables live leaving the block         */
   unsigned short status;      /* status field for the block               */
};

/*
 * basic block list structure
 */
struct blist {
   struct bblk *ptr;           /* pointer to block within the list         */
   struct blist *next;         /* pointer to the next blist element        */
   };

/*
 * loop information structure
 */
struct loopnode {
   struct loopnode *next;      /* pointer to next loop record              */
   struct bblk *header;        /* pointer to head block of loop            */
   struct bblk *preheader;     /* pointer to the preheader of the loop     */
   struct blist *blocks;       /* blocks in the loop                       */
   varstate invregs;           /* loop invariant variables                 */
   varstate sets;              /* variables updated in loop                */
   int anywrites;              /* any writes to memory?                    */
};

/*
 * instruction information
 */
struct instinfo {
   char *mneumonic;             /* mneumonic of instruction                */
   enum insttype type;          /* instruction class                       */
   int numargs;                 /* number of arguments                     */
   int numdstregs;              /* number of consecutive registers
                                   associated with the destination         */
   int numsrcregs;              /* number of consecutive registers
                                   associated with each source             */
   int setscc;                  /* condition codes set?                    */
   int datatype;                /* datatype of instruction                 */
};

/*
 * variable information
 */
struct varinfo {
   char *name;                  /* variable name                           */
   short type;                  /* variable type                           */
   short indirect;              /* variable indirectly referenced?         */
};

/*
 * optimization information
 */
struct optinfo {
   int count;                   /* transformation count for opt phase      */
   int max;                     /* max transformations for opt phase       */
   char optchar;                /* character representing opt phase        */
   char *name;                  /* name of optimization                    */
};

=======================================
vars.h
=======================================

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

=======================================
vect.h
=======================================

void binsert(bvect *, unsigned int);
void bdelete(bvect *, unsigned int);
void bunion(bvect *, bvect);
void binter(bvect *, bvect);
int bin(bvect, unsigned int);
void bcpy(bvect *, bvect);
int bequal(bvect, bvect);
bvect ball();
void bclear(bvect);
int bcnt(bvect);
bvect bnone();
void bfree(bvect);
void bdump(FILE *, bvect);
