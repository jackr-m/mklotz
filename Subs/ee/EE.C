/*************************************************************************
**                                                                       **
** EE.C         Expression Evaluator                                     **
**                                                                       **
** AUTHOR:      Mark Morley                                              **
** COPYRIGHT:   (c) 1992 by Mark Morley                                  **
** DATE:        December 1991                                            **
** HISTORY:     Jan 1992 - Made it squash all command line arguments     **
**                         into one big long string.                     **
**                       - It now can set/get VMS symbols as if they     **
**                         were variables.                               **
**                       - Changed max variable name length from 5 to 15 **
**              Jun 1992 - Updated comments and docs                     **
**                                                                       **
** You are free to incorporate this code into your own works, even if it **
** is a commercial application.  However, you may not charge anyone else **
** for the use of this code!  If you intend to distribute your code,     **
** I'd appreciate it if you left this message intact.  I'd like to       **
** receive credit wherever it is appropriate.  Thanks!                   **
**                                                                       **
** I don't promise that this code does what you think it does...         **
**                                                                       **
** Please mail any bug reports/fixes/enhancments to me at:               **
**      morley@camosun.bc.ca                                             **
** or                                                                    **
**      Mark Morley                                                      **
**      3889 Mildred Street                                              **
**      Victoria, BC  Canada                                             **
**      V8Z 7G1                                                          **
**      (604) 479-7861                                                   **
**                                                                       **
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <setjmp.h>

/* type synonyms */
typedef double dbl;
typedef float real;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;

#define TYPE            dbl          /* Type of numbers to work with */

#define VARLEN          15              /* Max length of variable names */
#define MAXVARS         50              /* Max user-defined variables */
#define TOKLEN          30              /* Max token length */

#define VAR             1
#define DEL             2
#define NUM             3

typedef struct
{
   char name[VARLEN + 1];               /* Variable name */
   TYPE value;                          /* Variable value */
} VARIABLE;

typedef struct
{
   char* name;                          /* Function name */
   int   args;                          /* Number of arguments to expect */
   TYPE  (*func)();                     /* Pointer to function */
} FUNCTION;

#define iswhite(c)  (c == ' ' || c == '\t')
#define isnumer(c)  ((c >= '0' && c <= '9') || c == '.')
#define isdelim(c)  (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' \
                  || c == '^' || c == '(' || c == ')' || c == ',' || c == '=')

/* Codes returned from the evaluator */
#define E_OK           0        /* Successful evaluation */
#define E_SYNTAX       1        /* Syntax error */
#define E_UNBALAN      2        /* Unbalanced parenthesis */
#define E_DIVZERO      3        /* Attempted division by zero */
#define E_UNKNOWN      4        /* Reference to unknown variable */
#define E_MAXVARS      5        /* Maximum variables exceeded */
#define E_BADFUNC      6        /* Unrecognised function */
#define E_NUMARGS      7        /* Wrong number of arguments to funtion */
#define E_NOARG        8        /* Missing an argument to a funtion */
#define E_EMPTY        9        /* Empty expression */


#define ERR(n) {ERROR=n; ERPOS=expression-ERANC-1; strcpy (ERTOK,token); longjmp(jb,1);}

/* pmark.. */
static Level1 (TYPE* r);
static void Level2 (TYPE* r);
static void Level3 (TYPE* r);
static void Level4 (TYPE* r);
static void Level5 (TYPE* r);
static void Level6 (TYPE* r);

/*************************************************************************
**                                                                       **
** PROTOTYPES FOR CUSTOM MATH FUNCTIONS                                  **
**                                                                       **
 *************************************************************************/

dbl deg (dbl x);
dbl rad (dbl x);
dbl sind (dbl x);
dbl tand (dbl x);
dbl cosd (dbl x);
dbl asind (dbl x);
dbl acosd (dbl x);
dbl atand (dbl x);

#define PI    3.14159265358979323846
#define M_E   2.71828182845904523536
#define DPR (180./PI)
#define RPD (PI/180.)

/*************************************************************************
**                                                                       **
** VARIABLE DECLARATIONS                                                 **
**                                                                       **
 *************************************************************************/

int   ERROR;                 /* The error number */
char  ERTOK[TOKLEN + 1];     /* The token that generated the error */
int   ERPOS;                 /* The offset from the start of the expression */
char* ERANC;                 /* Used to calculate ERPOS */

/*
   Add any "constants" here...  These are "read-only" values that are
   provided as a convienence to the user.  Their values can not be
   permanently changed.  The first field is the variable name, the second
   is its value.
*/
VARIABLE Consts[] =
{
   /* name, value */
   { "pi",      PI },
   { "e",       M_E },
   { "dpr",     DPR },
   { "rpd",     RPD },
   { 0 }
};

/*
   Add any math functions that you wish to recognise here...  The first
   field is the name of the function as it would appear in an expression.
   The second field tells how many arguments to expect.  The third is
   a pointer to the actual function to use.
*/
FUNCTION Funcs[] =
{
   /* name, funtion to call */
   { "sin",     1,    sind },
   { "cos",     1,    cosd },
   { "tan",     1,    tand },
   { "asin",    1,    asind },
   { "acos",    1,    acosd },
   { "atan",    1,    atand },
   { "sinr",    1,    sin },
   { "cosr",    1,    cos },
   { "tanr",    1,    tan },
   { "asinr",   1,    asin },
   { "acosr",   1,    acos },
   { "atanr",   1,    atan },
   { "sinh",    1,    sinh },
   { "cosh",    1,    cosh },
   { "tanh",    1,    tanh },
   { "exp",     1,    exp },
   { "ln",      1,    log },
   { "log",     1,    log10 },
   { "sqrt",    1,    sqrt },
   { "sqr",     1,    sqrt },
   { "floor",   1,    floor },
   { "ceil",    1,    ceil },
   { "abs",     1,    fabs },
   { "hypot",   2,    hypot },
   { "rss",     2,    hypot },
   { "deg",     1,    deg },
   { "rad",     1,    rad },

   { 0 }
};

VARIABLE        Vars[MAXVARS];       /* Array for user-defined variables */
unsigned char*  expression;          /* Pointer to the user's expression */
unsigned char   token[TOKLEN + 1];   /* Holds the current token */
int             type;                /* Type of the current token */
jmp_buf         jb;                  /* jmp_buf for errors */


/*************************************************************************
**                                                                       **
** Some custom math functions...   Note that they must be prototyped     **
** above (if your compiler requires it)                                  **
**                                                                       **
 *************************************************************************/

dbl deg(dbl x)
{return (x*DPR);}

dbl rad(dbl x)
{return (x*RPD);}

dbl sind (dbl x)
{return sin(x*RPD);}

dbl cosd (dbl x)
{return cos(x*RPD);}

dbl tand (dbl x)
{return tan(x*RPD);}

dbl asind (dbl x)
{return DPR*asin(x);}

dbl acosd (dbl x)
{return DPR*acos(x);}

dbl atand (dbl x)
{return DPR*atan(x);}

/* ======================================================================== */
/* getsymbol..------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** GetSymbol (char* s)                                                   **
**                                                                       **
** This routine obtains a value from the program's environment.          **
** This works for DOS and VMS (and other OS's???)
**                                                                       **
 ************************************************************************/

GetSymbol (char* s, TYPE* v)
{
   char* e;

   e=getenv(s);
   if (!e) return (0);
   *v=atof(e);
   return (1);
}
/* clearallvars..---------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** ClearAllVars()                                                        **
**                                                                       **
** Erases all user-defined variables from memory. Note that constants    **
** can not be erased or modified in any way by the user.                 **
**                                                                       **
** Returns nothing.                                                      **
**                                                                       **
 *************************************************************************/

ClearAllVars ()
{
   int i;

   for (i=0; i < MAXVARS; i++) {*Vars[i].name=0; Vars[i].value=0;}
}
/* clearvar..-------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** ClearVar (char* name)                                                 **
**                                                                       **
** Erases the user-defined variable that is called NAME from memory.     **
** Note that constants are not affected.                                 **
**                                                                       **
** Returns 1 if the variable was found and erased, or 0 if it didn't     **
** exist.                                                                **
**                                                                       **
 *************************************************************************/

ClearVar (char* name)
{
   int i;

   for (i=0; i < MAXVARS; i++)
      if (*Vars[i].name && ! strcmp(name, Vars[i].name))
      {
         *Vars[i].name=0;
         Vars[i].value=0;
         return (1);
      }
   return (0);
}
/* getvalue..-------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** GetValue (char* name, TYPE* value)                                    **
**                                                                       **
** Looks up the specified variable (or constant) known as NAME and       **
** returns its contents in VALUE.                                        **
**                                                                       **
** First the user-defined variables are searched, then the constants are **
** searched.                                                             **
**                                                                       **
** Returns 1 if the value was found, or 0 if it wasn't.                  **
**                                                                       **
 *************************************************************************/

GetValue (char* name, TYPE* value)
{
   int i;

   /* First check for an environment variable reference... */
   if (*name == '_') return (GetSymbol(name + 1, value));

   /* Now check the user-defined variables. */
   for (i=0; i < MAXVARS; i++)
      if (*Vars[i].name && ! strcmp(name, Vars[i].name))
      {
         *value=Vars[i].value; return (1);
      }

   /* Now check the programmer-defined constants. */
   for (i=0; *Consts[i].name; i++)
      if (*Consts[i].name && ! strcmp(name, Consts[i].name))
      {
         *value=Consts[i].value; return (1);
      }
   return (0);
}
/* setvalue..-------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** SetValue (char* name, TYPE* value)                                    **
**                                                                       **
** First, it erases any user-defined variable that is called NAME.  Then **
** it creates a new variable called NAME and gives it the value VALUE.   **
**                                                                       **
** Returns 1 if the value was added, or 0 if there was no more room.     **
**                                                                       **
 *************************************************************************/

SetValue (char* name, TYPE* value)
{
   char b[30];
   int  i;

   ClearVar (name);
   for (i=0; i < MAXVARS; i++)
      if (! *Vars[i].name)
      {
         strcpy (Vars[i].name, name);
         Vars[i].name[VARLEN]=0;
         Vars[i].value=*value;
         return (1);
      }
   return (0);
}
/* parse..----------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** Parse ()  Internal use only                                           **
**                                                                       **
** This function is used to grab the next token from the expression that **
** is being evaluated.                                                   **
**                                                                       **
 *************************************************************************/

static Parse ()
{
   char* t;

   type=0;
   t=token;
   while (iswhite (*expression))
      expression++;
   if (isdelim (*expression))
   {
      type=DEL;
      *t++=*expression++;
   }
   else if (isnumer (*expression))
   {
      type=NUM;
      while (isnumer (*expression))
         *t++=*expression++;
   }
   else if (isalpha (*expression))
   {
      type=VAR;
      while (isalpha (*expression))
        *t++=*expression++;
      token[VARLEN]=0;
   }
   else if (*expression)
   {
      *t++=*expression++;
      *t=0;
      ERR(E_SYNTAX);
   }
   *t=0;
   while (iswhite (*expression))
      expression++;
}
/* level1..---------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** Level1 (TYPE* r)   Internal use only                                  **
**                                                                       **
** This function handles any variable assignment operations.             **
** It returns a value of 1 if it is a top-level assignment operation,    **
** otherwise it returns 0                                                **
**                                                                       **
 *************************************************************************/

static Level1 (TYPE* r)
{
   char t[VARLEN + 1];

   if (type == VAR)
      if (*expression == '=')
      {
         strcpy (t, token);
         Parse();
         Parse();
         if (!*token)
         {
            ClearVar(t);
            return (1);
         }
         Level2(r);
         if (! SetValue(t, r))
            ERR(E_MAXVARS);
         return (1);
      }
   Level2(r);
   return (0);
}
/* level2..---------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** Level2 (TYPE* r)   Internal use only                                  **
**                                                                       **
** This function handles any addition and subtraction operations.        **
**                                                                       **
 *************************************************************************/

static void Level2 (TYPE* r)
{
   TYPE t=0;
   char o;

   Level3(r);
   while ((o=*token) == '+' || o == '-')
   {
      Parse();
      Level3(&t);
      if (o == '+')
         *r=*r + t;
      else if (o == '-')
         *r=*r - t;
   }
}
/* level3..---------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** Level3 (TYPE* r)   Internal use only                                  **
**                                                                       **
** This function handles any multiplication, division, or modulo.        **
**                                                                       **
 *************************************************************************/

static void Level3 (TYPE* r)
{
   TYPE t;
   char o;

   Level4(r);
   while ((o=*token) == '*' || o == '/' || o == '%')
   {
      Parse();
      Level4(&t);
      if (o == '*')
         *r=*r * t;
      else if (o == '/')
      {
         if (t == 0)
            ERR(E_DIVZERO);
         *r=*r / t;
      }
      else if (o == '%')
      {
         if (t == 0)
            ERR(E_DIVZERO);
         *r=fmod(*r, t);
      }
   }
}
/* level4..---------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** Level4 (TYPE* r)   Internal use only                                  **
**                                                                       **
** This function handles any "to the power of" operations.               **
**                                                                       **
 *************************************************************************/

static void Level4(TYPE* r)
{
   TYPE t;

   Level5(r);
   if (*token == '^')
   {
      Parse();
      Level5(&t);
      *r=pow(*r, t);
   }
}
/* level5..---------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** Level5 (TYPE* r)   Internal use only                                  **
**                                                                       **
** This function handles any unary + or - signs.                         **
**                                                                       **
 *************************************************************************/

static void Level5 (TYPE* r)
{
   char o=0;

   if (*token == '+' || *token == '-')
   {
      o=*token;
      Parse();
   }
   Level6(r);
   if (o == '-')
      *r=-*r;
}
/* level6..---------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** Level6 (TYPE* r)   Internal use only                                  **
**                                                                       **
** This function handles any literal numbers, variables, or functions.   **
**                                                                       **
 *************************************************************************/

static void Level6 (TYPE* r)
{
   int  i,n;
   TYPE a[3];

   if (*token == '(')
   {
      Parse();
      if (*token == ')')
         ERR(E_NOARG);
      Level1(r);
      if (*token != ')')
         ERR(E_UNBALAN);
      Parse();
   }
   else
   {
      if (type == NUM)
      {
         *r=(TYPE) atof(token);
         Parse();
      }
      else if (type == VAR)
      {
         if (*expression == '(')
         {
            for (i=0; *Funcs[i].name; i++)
               if (! strcmp(token, Funcs[i].name))
               {
                  Parse();
                  n=0;
                  do
                  {
                     Parse();
                     if (*token == ')' || *token == ',')
                        ERR(E_NOARG);
                     a[n]=0;
                     Level1(&a[n]);
                     n++;
                  } while (n < 4 && *token == ',');
                  Parse();
                  if (n != Funcs[i].args)
                  {
                     strcpy (token, Funcs[i].name);
                     ERR(E_NUMARGS);
                  }
                  *r=Funcs[i].func(a[0], a[1], a[2]);
                  return;
               }
               if (! *Funcs[i].name)
                  ERR(E_BADFUNC);
            }
            else if (! GetValue(token, r))
               ERR(E_UNKNOWN);
         Parse();
      }
      else
         ERR(E_SYNTAX);
   }
}
/* evaluate..-------------------------------------------------------------- */
/*************************************************************************
**                                                                       **
** Evaluate (char* e, TYPE* result, int* a)                              **
**                                                                       **
** This function is called to evaluate the expression E and return the   **
** answer in RESULT.  If the expression was a top-level assignment, a    **
** value of 1 will be returned in A, otherwise it will contain 0.        **
**                                                                       **
** Returns E_OK if the expression is valid, or an error code.            **
**                                                                       **
 *************************************************************************/

Evaluate (char* e, TYPE* result, int* a)
{
   if (setjmp(jb))
      return (ERROR);
   expression=e;
   ERANC=e;
   strlwr(expression);
   *result=0;
   Parse();
   if (! *token)
      ERR(E_EMPTY);
   *a=Level1(result);
   return (E_OK);
}
/* main..------------------------------------------------------------------ */
/*************************************************************************
**                                                                       **
** What follows is a main() routine that evaluates the command line      **
** arguments one at a time, and displays the results of each expression. **
** Without arguments, it becomes an interactive calculator.              **
**                                                                       **
 *************************************************************************/

char* ErrMsgs[] =
{
   "Syntax error",
   "Unbalanced parenthesis",
   "Division by zero",
   "Unknown variable",
   "Maximum variables exceeded",
   "Unrecognised funtion",
   "Wrong number of arguments to funtion",
   "Missing an argument",
   "Empty expression"
};

main (int argc, char* argv[])
{
   TYPE  result;
   int   i,ec,a;
   char  line[1024];
   char* v;

   ClearAllVars();
   /* If we have command line arguments then we evaluate them and exit. */
   if (argc > 1)
   {
      /* Concatenate all arguments into one string. */
      strcpy (line,argv[1]);
      for (i=2; i < argc; i++)
         strcat (line,argv[i]);

      /* Call the evaluator. */
      if ((ec=Evaluate (line, &result, &a)) == E_OK)
      {
         /* If we didn't assign a variable, then print the result. */
         if (!a)
            printf(" = %.9g\n", result);
      }
      else if (ec != E_EMPTY)
      {
         /* Display error info.  In this example, an E_EMPTY error is ignored. */
         printf("ERROR: %s - %s", ErrMsgs[ERROR - 1], ERTOK);
         printf("\n%s", ERANC);
         printf("\n%*s^\n", ERPOS, "");
      }
      exit (1);
   }

   /* There were no command line arguments, so lets go interactive. */
   printf("\nEE - Equation Evaluator\n");
   puts ("(a)sin(r,h),(a)cos(r,h),(a)tan(r,h)");
   puts ("exp,ln,log,sqr[t],floor,ceil");
   puts ("abs,rss,deg,rad");
   puts ("+,-,*,/,^,%");
   puts ("16 character variable names");
   puts ("see EE.TXT for further information");
   printf("cons - show constants\n");
   printf("vars - show variables\n");
   printf("clr  - clear variables\n");
   printf("EXIT or Q/q to quit\n");
   printf("\n ? ");

   /* Input one line at a time from the user.  Note that it uses stdin, so on
      DOS or UNIX you could pipe a list of expressions into it... */
   for (gets(line); !feof(stdin); gets(line))
   {
      strlwr(line);

      /* Did the user ask to exit? */
      if (!strcmp(line,"exit") || !strcmp(line,"q") || !strcmp(line,"quit")) exit (0);

      /* Did the user ask to see the variables in memory? */
      else if (! strcmp(line,"vars"))
      {
         for (i=0; i < MAXVARS; i++)
            if (*Vars[i].name)
               printf("%s = %.9g\n", Vars[i].name,Vars[i].value);
      }

      /* Did the user ask to see the constants in memory? */
      else if (!strcmp(line,"cons"))
      {
         for (i=0; *Consts[i].name; i++)
            printf("%s = %.9g\n", Consts[i].name,Consts[i].value);
      }

      /* Did the user ask to clear all variables? */
      else if (! strcmp(line,"clr")) ClearAllVars();

      /* If none of the above, then we attempt to evaluate the user's input. */
      else
      {
         /* Call the evaluator. */
         if ((ec=Evaluate(line, &result, &a)) == E_OK)
         {
            /* Only display the result if it was not an assignment. */
            if (! a) printf(" = %.9g\n",result);
         }
         else if (ec != E_EMPTY)
         {
            /* Display error information.  E_EMPTY is ignored. */
            printf("ERROR: %s - %s", ErrMsgs[ERROR - 1], ERTOK);
            printf("\n%s", ERANC);
            printf("\n%*s^\n", ERPOS, "");
         }
      }
      printf(" ? ");
   }
}
/* ------------------------------------------------------------------------ */
