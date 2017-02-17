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
