/*
* By Dale Holt    3945 Vondelpark Place, Colorado Springs, CO 80907
* AmigaBasic. November 1986, March 1991
* Conversion to C.  May 1993
* Fixed fmtcomma.  23 Oct 1993  (memmove ...l -> l+1)
* Fixed checks for 'e' and 'E' for scientific notation.  7 Mar 1994
* Added pi as a constant.  2 Sept 1994
* Fixed unary - and + after =.  6 Feb 1995
* Further unary - and + checks.  23 Aug 1995
* Remove spaces, better unary - check.  6 Oct 1997
* Fix handling of [,],{,} and remove terminating decimal point.  20 May 1999
* Fix handling of [,],{,} and add option for no comma in output.  7 Jul 1999
* Implement angles in degrees instead of radians.  22 Jan 2004
*
* Compile:   cc math.c -o math -lm
*            gcc math.c -o math -lm -lansi 
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#ifndef max
#   define max(A, B) ((A) > (B) ? (A) : (B))
#endif
#ifndef min
#   define min(A, B) ((A) > (B) ? (B) : (A))
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEGREES * M_PI / 180.0   /* For degrees */
/* #define DEGREES */ /* For radians */

char *strtrim(char *string);
char *strlower(char *string);
void fmtcomma(char *value, double number, int comma);
int compile(char *infunct, int *postfix_array, double *constant,
	      int *use_x, int *use_y, int *use_z, int *use_equal,
	      int *stack);
double evaluate(int *postfix, double *constant, int numops,
                double xpt, double ypt, double zpt, int stack_size);



int
main(int argc, char *argv[])
{
char
	*infunct,
	var,
	value[80];

int
	*postfix,
	stack_size,
	numops,
	j,
	maxlen=0,
	iter_ct,
	iter_total,
	solve=0,
	repeat=0,
	use_x=0,
	use_y=0,
	use_z=0,
	use_equal=0,
	use_from=0,
        do_comma=1;

double
	*constant,
	*var_ptr,
	x=0,
	y=0,
	z=0,
	var_inc,
	err,
	err_sign,
	prev_err,
	prev_err_sign,
	var_inc_mult,
	answer,
	search_start = 1.0,
	range_min,
	range_max;

/* format the output with commas? */

if (!strcmp(argv[0], "mathnc")) do_comma = 0;


/* determine the length of the input algebraic expression */

j = 1;
while (j < argc)
    {
    if (!strcasecmp(argv[j],"from"))
	{
	if (argc != j+2)
	    {
	    printf("math: Format error.  There should be one number after 'from'.\n");
	    goto ListInstruct;
	    }
	use_from = 1;
	search_start = atof(argv[++j]);
	argc = j-1;
	}
    else
        maxlen += strlen(argv[j++]);
    }

/* is there anything to calculate? */

if ( argc == 1 || ( argc == 2 && !strcmp(argv[1],"-help") ) )
    {
ListInstruct:
    printf("The algebraic expression evaluator by Dale Holt, copyright 1993.\n");
    printf("This program takes a standard algebraic expression and calculates\n");
    printf("an answer.  Version 3.1  2004 January 22\n\n");
    printf("The functions supported are sin, cos, tan, atan, acos, asin,\n");
    printf("sinh, cosh, tanh, ln, log, exp, sqrt, abs, int, and frac.\n");
    printf("Either ** or ^ may be used for exponentiation.\n");
    printf("PI or pi is acceptable for 3.14159265358979323846\n");
    
    err = 1.0 DEGREES;
    if (err < 0.5) 
        printf("The trigonometric functions operate with degrees, not radians.\n");
    else
        printf("The trigonometric functions operate with radians, not degrees.\n");
    
    printf("Parentheses may be used to group terms.  The symbols ^ and * may\n");
    printf("have to be quoted or escaped to get past the UNIX shell.\n");
    printf("The entire math expression may be quoted, if desired.\n");
    printf("The use of (, ), [, ], {, } are acceptable as parentheses.\n");
    printf("Commas will be ignored.\n\n");
    printf("If variables are included, then you will be prompted for values\n");
    printf("to be used and the expression will be evaluated with those values.\n");
    printf("This mode will end when a blank value is entered.\n\n");
    printf("Equations will be solved if entered with an x, y, or z and an equal sign.\n");
    printf("The starting value for a solution search is entered by appending from number.\n");
    printf("Examples:  x = 1 + sin(x)\n");
    printf("           y / 5 + exp(y^2) - 30 = 0\n");
    printf("           x+sin(x) = 4 from 5\n");
    printf("Please note that a large error means that a solution was not found!\n\n");
    printf("Commas will not be placed in the output if the executable is named mathnc\n");
    exit(-1);
    }

/* concatenate all arguments */

if ( (infunct = (char *)malloc(maxlen+4+argc)) == NULL )
    {
    printf("\aCould not allocate memory!\n");
    exit(-1);
    }

infunct[0] = '\0';
for (j=1; j<argc; j++)
	{
	strcat(infunct,strtrim(argv[j]));
	strcat(infunct," ");        /* for error checking of separate numbers */
	}

j = strlen(infunct)+5;

if ( (postfix = (int *)malloc(j*sizeof(int))) == NULL )
	{
	printf("Could not allocate %d bytes of memory!\n\a",j*sizeof(int));
	free(infunct);
	exit(-1);
	}

if ( (constant = (double *)malloc(j*sizeof(double)/2)) == NULL )
	{
	free(infunct);
	free(postfix);
	printf("Could not allocate %d bytes of memory!\n\a",j*sizeof(double)/2);
	exit(-1);
	}


/*
   Send function, get array of postfix operators and constants,
   returns number of PostFix operations, negative is error.
*/

numops = compile(infunct, postfix, constant,
	         &use_x, &use_y, &use_z, &use_equal, &stack_size);
if (numops>0)
    {
    if ( use_equal && (use_x || use_y || use_z) )
	{
	if (use_x + use_y + use_z > 1)
	    {
	    printf("%s can only solve equations with one variable.\n",argv[0]);
	    exit(-1);
	    }
	solve = 1;
	}
    else
    if ( use_equal && !(use_x || use_y || use_z) )
	{
	printf("Equal sign without variable!\n");
	exit(-1);
	}
    else
    if (use_x || use_y || use_z)
	repeat = 1;

    if (use_from && !solve)
	printf("math: ignoring from value.\n");

    if (solve)
	{
	if (use_x) { var = 'x'; var_ptr = &x; }
	if (use_y) { var = 'y'; var_ptr = &y; }
	if (use_z) { var = 'z'; var_ptr = &z; }
	printf("Solving from %c = %G ...\n",var,search_start);
	iter_ct = iter_total = 0;
	range_min = range_max = *var_ptr = search_start;
	var_inc = *var_ptr / 100.0;
	var_inc_mult = 10.0;    /* only changes sign */
	prev_err = err = evaluate(postfix,constant,numops,x,y,z,stack_size);
	prev_err_sign = (prev_err >= 0 ? 1 : -1);

	while ( err != 0 && iter_total < 1000 && fabs(var_inc) > 1.0e-30)
	    {
	    *var_ptr += var_inc;
	    range_min = min(range_min,*var_ptr);
	    range_max = max(range_max,*var_ptr);
	    iter_ct++;
	    iter_total++;
	    prev_err = err;
	    prev_err_sign = err_sign;
	    err = evaluate(postfix,constant,numops,x,y,z,stack_size);
	    if (err == 0) break;
	    err_sign = (err >= 0 ? 1 : -1);
	    if ( (err_sign == prev_err_sign) &&
		 (err_sign * err) < (prev_err_sign * prev_err) )
		{      /* keep going this direction */
		if (++iter_ct > 2 * var_inc_mult)   /* need bigger increment */
		    {
		    var_inc *= var_inc_mult;
		    iter_ct = 0;
		    }
		}
	    else     /* go the other direction */
		{
		var_inc /= (-var_inc_mult);
		iter_ct = 0;
		}
	    }
	printf("%c = %.10G, with error = %G in %d iterations.\n",
		var,*var_ptr,err,iter_total);
	printf("Search range %G to %G\n",range_min,range_max);
	if (fabs(err) > 0.0001)
		printf("NOTICE: Solution was probably NOT found!\n");
	}
    else
    if (repeat)
	{
	while (repeat)
	    {
	    if (use_x)
		{
		printf("  x val? "); fflush(stdout);
		if (!fgets(value,20,stdin)) goto quit;
		if (!strlen(strtrim(value))) goto quit;
		x = atof(value);
		}
	    if (use_y)
		{
		printf("  y val? "); fflush(stdout);
		if (!fgets(value,20,stdin)) goto quit;
		if (!strlen(strtrim(value))) goto quit;
		y = atof(value);
		}
	    if (use_z)
		{
		printf("  z val? "); fflush(stdout);
		if (!fgets(value,20,stdin)) goto quit;
		if (!strlen(strtrim(value))) goto quit;
		z = atof(value);
		}
	    answer = evaluate(postfix,constant,numops,x,y,z,stack_size);
	    fmtcomma(value,answer,do_comma);
	    printf("result = %s for", value, x, y, z);
	    if (use_x) printf("  x = %G",x);
	    if (use_y) printf("  y = %G",y);
	    if (use_z) printf("  z = %G",z);
	    printf("\n"); fflush(stdout);
            }
	}
    else
	{
	answer = evaluate(postfix,constant,numops,x,y,z,stack_size);
	fmtcomma(value,answer, do_comma);
	printf("%s\n",value);
	exit(0);
	}
    }

quit:
        /* free the memory */

   free(infunct);
   free(postfix);
   free(constant);
   exit(1);
}




/**************** infix to postfix compiler ***************/

int
compile(char *infunct, int *postfix_array, double *constant,
	int *use_x, int *use_y, int *use_z, int *use_equal,
        int *stack)
{

int
	j,
	k,
	l,
	c,
	no    = 0,
	yes   = 1,
	EqErr = 0,
	infix = 0,
	funct_offset = 30,
	this_op,
	next_op,
	digit,
	decimal,
	exponent,
	operator,
	in_const,
	num_const,
	Lparen,
	Rparen,
	infixlen,
	err_pos,
	*infptr,
	*pendop,
	*level,
	*infix_array,
        plevel,         /* parenthesis level */
        pif,            /* infix string position */
        ppf,            /* postfix array position */
        pop,            /* pending operation stack position */
	num_functs=26,
	opriority[30]={0,3,3,2,2,1,1,4,4,4,4,4,4,4,
		       4,4,4,4,4,4,4,4,4,4,4,4,4,4,1},
	opfunctlen[30]={1,2,1,1,1,1,1,4,4,4,4,4,4,3,
			3,3,3,2,3,4,3,3,4,1,1,1,1,1,1};

char
	*ptr1,
	*ptr2,
	pchar,
	nchar,
	tchar,
	*funct,
	tempstr[64],
	opfunct[30][5]={" ", "**", "^", "*", "/",
		        "+", "-", "sinh", "cosh", "tanh",
			"asin", "acos", "atan", "sin", "cos",
			"tan", "log", "ln", "exp", "sqrt",
			"abs", "int", "frac", "x", "y",
			"z", "(", ")", "="},
		    /* 1st must be blank */
                       
	ErrMsg2[80] = "";


j = strlen(infunct);
if (!j)
	{
	printf("Zero length input string!\n");
	return 0;
	}
/* printf("Expression: %s\n",infunct); */

/* allocate memory */

j++;
if (j<4) j = 4;
if ( (funct = (char *)malloc(j)) == NULL )
	{
	printf("Could not allocate %d bytes of memory!\n\a",j);
	exit(-1);
	}

if ( (infix_array = (int *)malloc(j*sizeof(int))) == NULL )
	{
	printf("Could not allocate %d bytes of memory!\n\a",j*sizeof(int));
	exit(-1);
	}

if ( (pendop = (int *)malloc(j*sizeof(int)/2)) == NULL )
	{
	printf("Could not allocate %d bytes of memory!\n\a",j*sizeof(int)/2);
	exit(-1);
	}

if ( (level = (int *)malloc(j*sizeof(int)/2)) == NULL )
	{
	printf("Could not allocate %d bytes of memory!\n\a",j*sizeof(int)/2);
	exit(-1);
	}

/* remove spaces, tabs, commas, line feeds, returns */
/* convert [,],{,} to (,)                           */

ptr1 = ptr2 = infunct;
while (*ptr2)
{
    if (*ptr2 == '[') *ptr2 = '(';
    else
    if (*ptr2 == '{') *ptr2 = '(';
    else
    if (*ptr2 == ']') *ptr2 = ')';
    else
    if (*ptr2 == '}') *ptr2 = ')';
    else
    if (*ptr2 == '\t') *ptr2 = ' ';

    if (*ptr2 != ' ' && *ptr2 != '\n' && *ptr2 != '\r' && *ptr2 != ',')
        *ptr1++ = *ptr2;

    ptr2++;
}
*ptr1 = '\0';
if (ptr1 == infunct) return 0;      /* all blanks */

/* check that parentheses match */

Lparen = Rparen = 0;
k = strlen(infunct);
for (j=0; j<k; j++)
	{
	if (infunct[j] == '(') Lparen++;
	else
	if (infunct[j] == ')')
	    {
	    if (++Rparen > Lparen)
		{
		strcpy(ErrMsg2,"Unmatched right parenthesis.");
		strcpy(funct,infunct);
		ptr1 = funct + j;
		goto CompileErr;
		}
	    }
	else
	if (infunct[j] == '=')
	    {
	    if ((*use_equal)++)
		{
		strcpy(ErrMsg2,"Second equal sign.");
		strcpy(funct,infunct);
		ptr1 = funct + j;
		goto CompileErr;
		}
	    if (Lparen != Rparen)
		{
		strcpy(ErrMsg2,"Unmatched left parenthesis at equal sign.");
		strcpy(funct,infunct);
		ptr1 = funct + j;
		goto CompileErr;
		}
	    }
	}
if (Lparen != Rparen)
	{
	strcpy(ErrMsg2,"Unmatched left parenthesis.");
	strcpy(funct,infunct);
	ptr1 = funct + j;
	goto CompileErr;
	}

/* check for unary + and - signs */

    ptr1 = infunct;
    ptr2 = funct;
    pchar = '(';
    operator = 0;
    digit = 0;                    
    while (tchar = *ptr1)
	{
	if ( (nchar = *(ptr1+1)) == '.' || isdigit(nchar) )
		 digit = yes; else digit = no;
	switch (pchar)
	    {
	    case '=':
	    case '>':
	    case '<':
	    case '^':
	    case '*':
	    case '/':
		operator = 1;
		break;
	    default:
		operator = 0;
	    }
	switch (tchar)
	    {
	    case '~':     /* my special character */
		strcpy(ErrMsg2,"Invalid character: ~");
		goto CompileErr;

	    case '+':
		if ( (operator && digit) ||
		     ( (pchar == '(') || ( (pchar == 'e' || pchar == 'E') && digit) ) )
		    tchar = '\0';
		break;

	    case '-':
		if ( (operator && digit) ||
		     ( (pchar == '(') || ( (pchar == 'e' || pchar == 'E') && digit) ) )
		    tchar = '~';     /* signal negative constant */
		if ( (pchar == '(' || pchar == '=') && (!digit) )
		    {
		    *ptr2++ = '0';
		    tchar = '-';
		    }
		break;

	    default:
		break;
	    }
	if (tchar) *ptr2++ = tchar;
	pchar = tchar;
	ptr1++;
	}
*ptr2 = '\0';

/* convert to tokens */

ptr1 = funct;
infptr = infix_array;
num_const = 0;
digit = 0;
exponent = 0;
operator = 0;

while (*ptr1)
	{
	/* skip spaces */
	if (*ptr1 == ' ') goto next_char;

	/* is this a defined operation or function? */
	for (j=1; j<num_functs+3; j++)
		{
		if (!strncasecmp(ptr1,opfunct[j],opfunctlen[j]))
			{
			*infptr++ = j;
			ptr1 += (opfunctlen[j] - 1);
			if (j == 23) *use_x = yes;
			if (j == 24) *use_y = yes;
			if (j == 25) *use_z = yes;
			goto next_char;
			}
		}

	if (!strncasecmp(ptr1, "pi", 2))
	    {
	    constant[++num_const] = M_PI;
	    *infptr++ = funct_offset + num_const;
	    ptr1++;
	    goto next_char;
	    }

	/* then it should be a constant */
	for (j=0; j<64; j++) tempstr[j] = '\0';
	in_const = yes;
	decimal = exponent = no;
	while (in_const)
		{
		tchar = *ptr1;
		if (tchar == '.')
			{
			if (decimal)
				{
				strcpy(ErrMsg2,"Second decimal point in constant.");
				goto CompileErr;
				}
			if (exponent)
				{
				strcpy(ErrMsg2,"Decimal point in exponent.");
				goto CompileErr;
				}
			decimal = yes;
			}
		else
		if (tchar == 'e' || tchar == 'E')
			{
			if (exponent)
				{
				strcpy(ErrMsg2,"Exponent specified twice.");
				goto CompileErr;
				}
			if (strlen(tempstr) == 0
			    || (strlen(tempstr) == 1 && tempstr[0] == '.'))
				{
				strcpy(ErrMsg2,"Invalid constant.");
				goto CompileErr;
				}
			exponent = yes;
			}
		else
		if (isdigit(tchar))
			{
			;
			}
		else
		if (tchar == '~')
			{
			if (strlen(tempstr) == 0
			    || tempstr[strlen(tempstr)-1] == 'e'
			    || tempstr[strlen(tempstr)-1] == 'E')
				tchar = '-';
			else
				{
				strcpy(ErrMsg2,"Misplaced unary minus sign.");
				goto CompileErr;
				}
			}
		else
			{
			in_const = no;
			if (strlen(tempstr) == 0)
				{
				strcpy(ErrMsg2,"Invalid character or function.");
				goto CompileErr;
				}
			if (strlen(tempstr) == 1 && !isdigit(*tempstr))
				{
				strcpy(ErrMsg2,"Unconnected decimal point.");
				ptr1--;
				goto CompileErr;
				}
			ptr1--;   /* save this character */
			}

		if (in_const)
			{
			tempstr[strlen(tempstr)] = tchar;
			ptr1++;
			}
		}
	tchar = tempstr[strlen(tempstr)-1];
	if (!isdigit(tchar) && tchar != '.')
		{
		strcpy(ErrMsg2,"Constant did not end with a digit.");
		goto CompileErr;
		}
	constant[++num_const] = strtod(tempstr,NULL);
	*infptr++ = funct_offset + num_const;
	decimal = exponent = no;

  next_char:
	++ptr1;
	*infptr = 0;
	}
infixlen = (int)(infptr - infix_array);
infix = yes;

/* check that the math rules are obeyed */

/************************************************************

RuleChecking:
1. Constants and variables must be followed by ) or = or operator.
2. Operator cannot be followed by operator or ) or =.
3. Function must be followed by (.
4. ( cannot be followed by operator or ) or =.
5. ) must be followed by operator or ) or =.
6. Expression must end with an constant, variable, or ).
Variables: X, Y, Z      --- not used in calc ---
Operators: + - * / ^ **

************************************************************/

if (infix_array[infixlen-1] < 23)
	{
	strcpy(ErrMsg2,"Expression must end with constant, variable, or ')'.");
	j = infixlen-1;   /* error position */
	goto CompileErr;
	}

if (infix_array[0] < 7)
	{
	strcpy(ErrMsg2,"Expression may not start with an operator.");
	j = 0;   /* error position */
	goto CompileErr;
	}

for (j=0; j<infixlen-1; j++)
	{
	this_op = infix_array[j];
	next_op = infix_array[j+1];
	if (this_op > funct_offset || this_op == 23 || this_op ==24 || this_op == 25)
		{     /* constants and variables */
		if (next_op > 6 && next_op != 27 && next_op != 28)    /*  27 is ), 28 is = */
			{
			strcpy(ErrMsg2,"Constant or variable must be followed by operator or ')'.");
			goto CompileErr;
			}
		}
	else
	if (this_op < 7)     /* operators */
		{
		if (next_op < 7 || next_op == 27 || next_op == 28)    /*  27 is ), 28 is = */
			{
			strcpy(ErrMsg2,"Operator must be followed by constant, variable, or function.");
			goto CompileErr;
			}
		}
	else
	if (this_op > 6 && this_op < 23)     /* functions, 23 = X, 24 = Y, 25 = Z, 26 = ( */
		{
		if (next_op != 26)
			{
			strcpy(ErrMsg2,"Function must be followed by '('.");
			goto CompileErr;
			}
		}
	else
	if (this_op == 26)     /* 26 = ( */
		{
		if (next_op == 27)
			{
			strcpy(ErrMsg2,"Empty set of parentheses.");
			goto CompileErr;
			}
		else
		if (next_op < 7)
			{
			strcpy(ErrMsg2,"'(' cannot be followed by an operator.");
			goto CompileErr;
			}
		else
		if (next_op == 28)
			{
			strcpy(ErrMsg2,"'(' cannot be followed by an equal sign.");
			goto CompileErr;
			}
		}
	else
	if (this_op == 27 && next_op > 6 && next_op != 27 && next_op != 28)    /* 27 = ) */
		{
		strcpy(ErrMsg2,"An operator, ')', or '=' must follow ')'.");
		goto CompileErr;
		}
   }

/*******************************************************
printf("infix: ");
for (j=0; j<infixlen; j++)
    {
    if (infix_array[j]<funct_offset)
	printf("%s ",opfunct[infix_array[j]]);
    else
	printf("%G ",constant[infix_array[j]-funct_offset]);
    }
printf("\n");
********************************************************/

/* convert to postfix */

 plevel = 1;          /* parenthesis level */
 pif = -1;            /* infix string position */
 ppf = -1;            /* postfix array position */
 pop = 0;             /* pending operation stack position */
 pendop[0] = 0;       /* signal bottom of stack */
 level[0] = 0;        /* pending operator parenthesis level */

while (++pif < infixlen)
 {
 c = infix_array[pif];

 if (c>funct_offset || c==23 || c==24)     /* constant or variable */
     postfix_array[++ppf] = c;

 else
 if (c>6 && c<23)               /* function */
    {
    pendop[++pop] = c;
    level[pop] = plevel;
    }  

 else
 if (c==26)               /* left parenthesis */
    plevel++;   

 else
 if (c==27 || c==28)              /* right parenthesis or equal */
    {
    while (level[pop] == plevel)
	postfix_array[++ppf] = pendop[pop--];
    plevel--;        /* reduce parenthesis level */
    }

 else          /* must be operator */
    {
    while (level[pop] == plevel && opriority[pendop[pop]] >= opriority[c])
	postfix_array[++ppf] = pendop[pop--];
    pendop[++pop] = c;
    level[pop] = plevel;
    }
 }
 
 while (pop>-1)
    postfix_array[++ppf] = pendop[pop--];

 if (*use_equal)
    postfix_array[ppf++] = 6;  /* append a subtraction operator */

 /* determine the required stack size for evaluating the expression */

 *stack = 0;
 for (k=j=0; j<=ppf; j++)
    {
    if (postfix_array[j] > 22)   /* variable or constant */
	{
	k++;
	}
    else
    if (postfix_array[j] < 7)  /* operator */
	{
	*stack = max(*stack,k);
	k--;
	}
    }

/*******************************************************
printf("postfix: ");
for (j=0; j<ppf; j++)
    {
    if (postfix_array[j]<funct_offset)
	printf("%s ",opfunct[postfix_array[j]]);
    else
	printf("%G ",constant[postfix_array[j]-funct_offset]);
    }
printf("\nrequired stack %d\n",*stack);
********************************************************/

free(funct);
free(infix_array);
free(pendop);
free(level);

return ppf;


CompileErr:
if (!infix)
	{
	err_pos = (int)(ptr1 - funct);
	ptr1 = funct;
	while (*ptr1)
		{
		if (*ptr1 == '~') *ptr1 = '-';   /* unary sign conversion */
		ptr1++;
		}
	printf("%s\n",funct);
	printf("%.*s^\n",err_pos,"--------------------------------------------------------------------");
	}
else
	{
	k=0;
	for (l=0; l<infixlen; l++)
		{
		if (infix_array[l] < funct_offset)
			{
			sprintf(tempstr,"%s",opfunct[infix_array[l]]);
			printf("%s",tempstr);
			k += strlen(tempstr);
			}
		else
		if (infix_array[l] > funct_offset)
			{
			sprintf(tempstr,"%G",constant[infix_array[l]-funct_offset]);
			printf("%s",tempstr);
			k += strlen(tempstr);
			}
		if (l == j) err_pos = k;
		}
	printf("\n%.*s^\n",err_pos,"--------------------------------------------------------------------");
	}

printf("Error: %s\n",ErrMsg2);

ppf = -1;

free(funct);
free(infix_array);
free(pendop);
free(level);

return ppf;
}

/*
' optimize the postfix array

' perform calculations which just use constants
 PRINT "Attempting to optimize...";
 ON ERROR GOTO OptError
 someOpt=no
 opt=yes
 WHILE opt=yes
   opt=no
   FOR j=1 TO ppf%-2     'combine sequential constants
     IF PostFix%(j)>funct_offset AND PostFix%(j+1)>funct_offset AND PostFix%(j+2)<7 THEN
       c=PostFix%(j)-funct_offset
       t1=Const!(c)
       t2=Const!(PostFix%(j+1)-funct_offset)
       ON PostFix%(j+2) GOTO b1,b2,b3,b4,b5,b6
       b1:  
       b2: Const!(c)=t1^t2 : GOTO OptB
       b3: Const!(c)=t1*t2 : GOTO OptB
       b4: Const!(c)=t1/t2 : GOTO OptB
       b5: Const!(c)=t1+t2 : GOTO OptB
       b6: Const!(c)=t1-t2
OptB:  FOR k=j+1 TO ppf%-2
         PostFix%(k)=PostFix%(k+2)
       NEXT k
       ppf%=ppf%-2
       opt=yes
       someOpt=yes
     END IF
   NEXT j

   FOR j=1 TO ppf%-1     'evaluate functions of constants
     IF PostFix%(j)>funct_offset AND PostFix%(j+1)>6 AND PostFix%(j+1)<23 THEN
       c=PostFix%(j)-funct_offset
       t1=Const!(c)
       ON PostFix%(j+1)-6 GOTO c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16
       c1: Const!(c)=FNsinh(t1)   : GOTO OptC
       c2: Const!(c)=FNcosh!(t1)  : GOTO OptC  
       c3: Const!(c)=FNtanh(t1)   : GOTO OptC  
       c4: Const!(c)=FNasin!(t1)  : GOTO OptC  
       c5: Const!(c)=FNacos!(t1)  : GOTO OptC  
       c6: Const!(c)=ATN(t1)      : GOTO OptC  
       c7: Const!(c)=SIN(t1)      : GOTO OptC  
       c8: Const!(c)=COS(t1)      : GOTO OptC  
       c9: Const!(c)=TAN(t1)      : GOTO OptC  
       c10: Const!(c)=FNlog10!(t1) : GOTO OptC 'log base 10 
       c11: Const!(c)=LOG(t1)      : GOTO OptC 'nat log
       c12: Const!(c)=EXP(t1)      : GOTO OptC  
       c13: Const!(c)=SQR(t1)      : GOTO OptC  
       c14: Const!(c)=ABS(t1)      : GOTO OptC  
       c15: Const!(c)=INT(t1)      : GOTO OptC  
       c16: Const!(c)=t1-INT(t1) 
OptC:  FOR k=j+1 TO ppf%-1
         PostFix%(k)=PostFix%(k+1)
       NEXT k
       ppf%=ppf%-1
       opt=yes
       someOpt=yes
     END IF
   NEXT j

optD:     'reduce addition and subtraction of constants
   j=1
   WHILE j<ppf%-2
     IF PostFix%(j)>funct_offset AND (PostFix%(j+1)=5 OR PostFix%(j+1)=6) AND PostFix%(j+2)>funct_offset AND (PostFix%(j+3)=5 OR PostFix%(j+3)=6) THEN
       t1=0
       t2=Const!(PostFix%(j)-funct_offset)
       op=PostFix%(j+1)
       IF op=5 THEN t1=t1+t2 ELSE t1=t1-t2
       t2=Const!(PostFix%(j+2)-funct_offset)
       op=PostFix%(j+3)
       IF op=5 THEN t1=t1+t2 ELSE t1=t1-t2
       Const!(PostFix%(j)-funct_offset)=t1
       PostFix%(j+1)=5  'add
       FOR k=j+2 TO ppf%-2
         PostFix%(k)=PostFix%(k+2)
       NEXT k
       ppf%=ppf%-2
       someOpt=yes
       opt=yes
     END IF
     j=j+1
   WEND  
optE:      'reduce multiplication and division of constants
   j=1
   WHILE j<ppf%-2
     IF PostFix%(j)>funct_offset AND (PostFix%(j+1)=3 OR PostFix%(j+1)=4) AND PostFix%(j+2)>funct_offset AND (PostFix%(j+3)=3 OR PostFix%(j+3)=4) THEN
       t1=1
       t2=Const!(PostFix%(j)-funct_offset)
       op=PostFix%(j+1)
       IF op=3 THEN t1=t1*t2 ELSE t1=t1/t2
       t2=Const!(PostFix%(j+2)-funct_offset)
       op=PostFix%(j+3)
       IF op=3 THEN t1=t1*t2 ELSE t1=t1/t2
       Const!(PostFix%(j)-funct_offset)=t1
       PostFix%(j+1)=3  'multiply
       FOR k=j+2 TO ppf%-2
         PostFix%(k)=PostFix%(k+2)
       NEXT k
       ppf%=ppf%-2
       someOpt=yes
       opt=yes
     END IF
     j=j+1
   WEND  
 WEND
 IF someOpt=yes THEN
   PRINT "reduced to ";ppf%;" operations."
 ELSE
   PRINT "could not."
 END IF
CompileEnd:
 RETURN
 
CompileErr:
 PRINT "Expression Error."
 IF infix=no THEN
   PRINT " ";
   FOR kk=1 TO LEN(funct$)
     t$=MID$(funct$,kk,1)
     IF t$="~" THEN PRINT "-"; ELSE PRINT t$;
   NEXT kk
   PRINT
   IF j<>0 THEN PRINT SPC(j);"^"
 ELSE
   FOR k=1 TO LEN(infix$)
     c=ASC(MID$(infix$,k,1))
     IF c<funct_offset THEN
       PRINT funct$(c);
     ELSEIF c>funct_offset THEN
       PRINT Const!(c-funct_offset);
     END IF
     IF k=j THEN kk=POS(0)-1
   NEXT k
   PRINT
   PRINT SPC(kk);"^"
 END IF
 PRINT ErrMsg2$
 EqErr=yes
 RETURN
 
ResNext:
 ErrStatus=yes
 RESUME NEXT

OptError:
 PRINT
 PRINT "A math error occurred while operating on the constants."
 EqErr=yes
 RESUME CompileEnd
  
END SUB
*/



/************* evaluate the postfix expression *******************/

double
evaluate(int *postfix, double *constant, int numops, double xpt,
	 double ypt, double zpt, int stack_size)
{
int
	l,
	ls,
	funct_offset = 30;

double
	result,
	*stack;

/* allocate memory */

if ( (stack = (double *)malloc(stack_size*sizeof(double))) == NULL )
	{
	printf("Could not allocate %d bytes of memory!\n\a",numops*sizeof(double));
	exit(-1);
	}

for (l=0, ls=-1; l<numops; l++)
    {
    switch (postfix[l])
	{
	case 1:
	case 2:
	    stack[ls-1] = pow(stack[ls-1],stack[ls]);
	    ls--;
	    break;

	case 3:
	    stack[ls-1] *= stack[ls];
	    ls--;
	    break;

	case 4:
	    if (stack[ls] == 0) goto DivZero;
	    stack[ls-1] /= stack[ls];
	    ls--;
	    break;

	case 5:
	    stack[ls-1] += stack[ls];
	    ls--;
	    break;

	case 6:
	    stack[ls-1] -= stack[ls];
	    ls--;
	    break;

	case 7:
	    stack[ls] = sinh(stack[ls] DEGREES);
	    break;

	case 8:
	    stack[ls] = cosh(stack[ls] DEGREES);
	    break;

	case 9:
	    stack[ls] = tanh(stack[ls] DEGREES);
	    break;

	case 10:
	    if (fabs(stack[ls]) > 1.0) goto TrigRange;
	    stack[ls] = asin(stack[ls]);
            stack[ls] = stack[ls] / (1.0 DEGREES);
	    break;

	case 11:
	    if (fabs(stack[ls]) > 1.0) goto TrigRange;
	    stack[ls] = acos(stack[ls]);
            stack[ls] = stack[ls] / (1.0 DEGREES);
	    break;

	case 12:
	    stack[ls] = atan(stack[ls]);
            stack[ls] = stack[ls] / (1.0 DEGREES);
	    break;

	case 13:
	    stack[ls] = sin(stack[ls] DEGREES);
	    break;

	case 14:
	    stack[ls] = cos(stack[ls] DEGREES);
	    break;

	case 15:
	    stack[ls] = tan(stack[ls] DEGREES);
	    break;

	case 16:
	    if (stack[ls] <= 0) goto LogNeg;
	    stack[ls] = log10(stack[ls]);
	    break;

	case 17:
	    if (stack[ls] <= 0) goto LogNeg;
	    stack[ls] = log(stack[ls]);
	    break;

	case 18:
	    stack[ls] = exp(stack[ls]);
	    break;

	case 19:
	    if (stack[ls] < 0) goto SqrtNeg;
	    stack[ls] = sqrt(stack[ls]);
	    break;

	case 20:
	    stack[ls] = fabs(stack[ls]);
	    break;

	case 21:
	    stack[ls] = floor(stack[ls]);
	    break;

	case 22:
	    stack[ls] = stack[ls] - floor(stack[ls]);
	    break;

	case 23:
	    stack[++ls] = xpt;
	    break;

	case 24:
	    stack[++ls] = ypt;
	    break;

	case 25:
	    stack[++ls] = zpt;
	    break;

	default:
	    stack[++ls] = constant[postfix[l]-funct_offset];
	}

    }
if (ls) printf("Non-zero stack ptr: %d\a\n",ls);
result = stack[0];
free(stack);
return result;

DivZero:
printf("math: Divide by zero.\n");
free(stack);
exit(-1);

TrigRange:
printf("math: Trig function input out of range.\n");
free(stack);
exit(-1);

SqrtNeg:
printf("math: Square root of negative number.\n");
free(stack);
exit(-1);

LogNeg:
printf("math: Log of negative number.\n");
free(stack);
exit(-1);
}

/***************************************************************/

void fmtcomma(char *value, double number, int comma)
/*
 * 1995 Jan 10  Fixed for Negative numbers.  Dale Holt
 */
{
int  i,
     j,
     k,
     m,
     n,
     sign;

    sprintf(value,"%.10G",number);
    /* only do non-exponential numbers */
    if (strchr(value,'E')) return;

    if (strchr(value,'.') == NULL) strcat(value,".");
    /* format with commas */
    if (comma)
    {
        i = strlen(value);
        if (number < 0.0) sign=1; else sign=0;
        j = 0;
        m = sign;
        while ( (m < i) && (value[m++] != '.') ) j++;  /* count the digits */
        k = (--j) / 3;
        memmove(value+k+sign,value+sign,i+1);  /* make room for the commas */
        for (n=sign,m=k+sign; j > 2; m++ )
        {
	    value[n++] = value[m];
	    if ( (j % 3 ) == 0)   value[n++] = ',';
	    j--;
        }
    }
    /* remove trailing zeros */
    i = strlen(value)-1;
    while (value[i] == '0') value[i--] = '\0';
    if (value[i] == '.') value[i] = '\0';
}

/***************************************************************/

/* function to convert an entire string to lower case */
char *strlower(char *string)
{
do
  {
  if (isupper(*string)) *string = tolower(*string);
  }
while ( *++string != '\0' );
return string;
}

/*-----------------------------------------------------------------*/

/* function to trim leading and trailing spaces from a string */
char *strtrim(char *string)
{
int j,k;

j=0;
while (string[j] == ' ') j++;
k=strlen(string)-1;
while (string[k] == ' ' || string[k] == '\n') k--;
string[++k] = '\0';
strcpy(string,&string[j]);
return string;
}

