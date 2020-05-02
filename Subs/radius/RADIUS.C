/* RADIUS.C   M. W. Klotz   3/01, 10/03, 1/04

Cutting a non-standard radius with an endmill.  Based on formulae supplied by:

Horace K. B. "Steve" Steven, Jr.
Engineer, Surface Ship Test Section
Code 223, Bldg 19, NNSY
SUPSHIP Portsmouth, VA 23709
email: StevenHK@supship.navy.mil, Phone: 757 396 4001 x 2113
FAX 757 396 4055, efax 208 379 9699
Home email hsteven@cavtel.net

*/

/* head..-------------------- header files -------------------------------- */

/* Unneeded headers can be left in place - the header files consist mainly of
'defines' for the compiler and do not add to the size of the executable code.
*/

/* turns off assert macros when not debugging */
#define NDEBUG

#include <assert.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <float.h>
#include <graph.h>
#include <malloc.h> 
#include <math.h>
#include <search.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* def..----------------------- defines -------------------------------------*/

/* type synonyms */
typedef double dbl;
typedef float real;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

/*
typ	format		pointer		size	range
0	c		char		1	
1	s		char		?
2	h(d,i)		char		1	-128 : 127
3	hu		uchar		1	0 : 255
4	d,i		int		2	-32768 : 32767
5	u		uint		2	0 : 65535
6	ld,li		long		4	-2,147,483,647 : 2,147,483,648
7	lu		ulong		4	0 : 4,294,967,295
8	e,E,f,F,g,G	float (real)	4	7 digit 10^+/-38
9	l(e,E,f,F,g,G)	double (dbl)	8	15 digit 10^+/-308
*/

/* macro declarations */
#define _in stdin			/* makes redirection easier */
#define _out stdout
#define pause getch()
#define pausem getkey("press a key\n")
#define beep tone(440,100L)
#define bpause beep; pause
#define bpausem beep; pausem
#define quit beep; fcloseall(); exit(1)
#define print printf("\n")
#define FALSE 0
#define TRUE !FALSE
#define cls _clearscreen(0)

/* global utility functions (can return only one value)
   note: no space between function name and parameter list
*/
#define SQR(a) ((a)*(a))
#define CUBE(a) ((a)*(a)*(a))
#define FRAC(a) ((a)-floor(a))		/* or use modf function */
	/* see also split() in C:\CCC\GOODIES.TXT */
#define MAX(a,b) ((a)>=(b) ? (a) : (b))
#define MIN(a,b) ((a)<=(b) ? (a) : (b))
#define ABS(a) ((a)>=0 ? (a) : -(a))
#define SGN(a) ((a)!=0 ? ((a)>0 ? 1 : -1) : 0)
#define ATOB(a,b) (pow((a),(b)))
#define LOG(a) (log(a))
#define LOG10(a) (log10(a))
#define SIND(a) sin((a)*RPD)
#define COSD(a) cos((a)*RPD)
#define TAND(a) tan((a)*RPD)
#define ASN(a) asin((ABS(a)<1) ? (a) : SGN(a))
#define ACS(a) acos((ABS(a)<1) ? (a) : SGN(a))
#define ASND(a) DPR*asin((ABS(a)<1) ? (a) : SGN(a))
#define ACSD(a) DPR*acos((ABS(a)<1) ? (a) : SGN(a))
#define ATAND(a) DPR*atan(a)
#define ATAN2(s,c) ((s!=0||c!=0) ? atan2((s),(c)) : 0)
#define ATAN2D(s,c) DPR*((s!=0||c!=0) ? atan2((s),(c)) : 0)
#define URAND(a,b) (((b)-(a))*(float)rand()/32767.+(a))
#define RSS2(a,b) sqrt((a)*(a)+(b)*(b))
#define RSS3(a,b,c) sqrt((a)*(a)+(b)*(b)+(c)*(c))
#define RND(a) floor ((a)+.5)

/* scan codes for keys */
/* #include <keycode.h> */

/* constant declarations - all 'defines' - no executable code */
#include <constant.h>

/* -------------------------- function prototypes ------------------------- */

/*
Minimize the number of these included since they contribute to exectuable
size.  Many of the more oft-used functions are now in MWK.LIB so they can
be called from source code without the need to include the include files.
*/

/*
global utility functions (now in MWK.LIB)
void delay (clock_t wait);
char getkey (char *prompt);
void ptext (char *text, int row, int col, int fgd, int bkg);
void textscreen (void);
void tone (int frequency, long duration);
void kbdset (uint kbtog);
*/
/* #include <util.h> */

/*
generalized io functions (now in MWK.LIB)
void vin (char *prompt, void *x, char *format, dbl def, char *udef);
void vpr (char *label, void *x, char *format);
char cf (char *format);
int places (void *x, char *format);
void apr (char *label, void *x, char *format, int n1, int n2);
*/
/* #include <io.h> */

/*
simplified string tools (now in MWK.LIB)
char *sex (char *d, char *s, int p, int n);
char *sdel (char *s, char c);
char *ssi (char *s, char *p, char *r, int n);
void sdp (char *s);
*/
/* #include <str.h> */

/*
string manipulation
char *trim (char *s);
char *left (char *d, char *s, int num);
char *right (char *d, char *s, int num);
char *mid (char *d, char *s, int start, int num);
void strdelc (char *s, char c);
void strreplace (char *s, char new, char old);
char *strinsert (char *s, char insert);
char *strsub (char *s, char *pat, char *rep);
int stoint (char *s);
*/
/* #include <strings.h> */

/* data input routines */
/* #include <input.h> */

/* printing data */
/* #include <output.h> */

/* vector utilities */
/* #include <vector.h> */

/* graphics routines */
/* #include <graphics.h> */

/* file management routines */
/* #include <file.h> */

/* pmark..  functions peculiar to this program */

/* gmark..-------------- global graphics variables ------------------------ */
/* some are contained in graphics.h */

/* vmark..------------ global variable declarations ----------------------- */

/* smark..----------- global structure declarations ----------------------- */

/* main..================================================================== */

main (int argc, char *argv[], char *envp[] )

/* argc=count of command line arguments */
/* argv points to parameters, envp points to environment parameters */

{
int i,j,k;
dbl R,D,E,r,ru,du,alpha,sa,w,sb,theta;

/* -------------------------- initializations ------------------------------ */
textscreen ();
/* ------------------------------------------------------------------------- */

puts ("NONSTANDARD RADIUS CALCULATIONS - Steven Horace\n");
vin ("Desired radius",&R,"lf",0.5625,"in");
vin ("Allowable error in desired radius",&E,"lf",0.01,"in");
puts ("\nYou must specify either the depth or width of the cut.");
puts ("If you don't know the depth, enter zero and the width will be requested.");
print;

vin ("Depth of cut",&D,"lf",0.,"in");
if (D == 0.)
{
vin ("Width of cut",&w,"lf",0.,"in");
if (w == 0.) {puts ("LEARN TO READ INSTRUCTIONS!"); beep; quit;}
theta=asin(0.5*w/R); D=R*(1-cos(theta));
}

w=R*SIND(ACSD(1.-D/R));		//half width of cut
printf ("Depth of cut = %.4lf in \n",D);
printf ("Width of cut = %.4lf in \n",2.*w);
print;

/*
                          1
sin alpha = ------------------------------  = sin of minimum angle of
           {1 + 8*(E/R)*(R[2]/D[2])}[1/2]     inclination at specified error
*/

// sine of minimum angle of inclination at specified error
sa=1./sqrt(1.+8.*(E/R)*(R*R/(D*D)));

printf ("Minimum inclination angle = %.4lf deg\n",ASND(sa));

/*
               2 + (D/R)*{(1/sin[2]alpha) -1} 
      r = R* {--------------------------------} =     exact radius of cutter
                         (2/sin alpha)                at minimum angle alpha
*/

// exact radius of cutter at minimum angle alpha
r=R*((2.+(D/R)*(1./(sa*sa)-1.))/(2./sa));

printf ("Cutter diameter at minimum angle = %.4lf in\n",2.*r);
agin:
print;
printf ("Cutter diameter to use must be ");
printf ("greater than %.4lf in and less than %.4lf in\n",2.*r,2.*R);
vin ("Cutter diameter to use",&du,"lf",r+R,"in");
ru=0.5*du;
if ((ru <= r) || (ru >= R))
{
beep; puts("Error - try again\n"); goto agin;
}

/*
                            D/r 
sin alpha = ----------------------------------   = sine of angle from vertical
            1-{1-2*(R*D/r[2]) + (D/r)[2]}[1/2]        to  cutter axis 
*/

//sine of angle from vertical to cutter axis
sa=(D/ru)/(1.-sqrt(1.-2.*(R*D/(ru*ru))+(D/ru)*(D/ru)));

alpha=ASND(sa);
print;
printf ("Angle from vertical to cutter axis = %.4lf deg\n",alpha);
sb=(R-sqrt(R*R-w*w))/(ru-sqrt(ru*ru-w*w));

/*
         R-(R[2]-W[2])[1/2]
Formula: ------------------- = sine of angle from vertical to cutter axis
         r-(r[2]-W[2])[1/2]
*/
print;
puts ("Using Steve's second method, we get, as a check:");
printf ("Angle from vertical to cutter axis = %.4lf deg\n",ASND(sb));
/*
Method 1:R=radius desired, r=radius of cutter, R must be greater than r 
Formula: r/R = sine of angle from vertical to cutter axis
*/
printf ("(Steve's first method yields %.4lf deg)\n",ASND(ru/R));
print;

/* E = (R/8)*(D[2]/R[2])*{(1/sin[2] alpha) -1}  =  error in radius  */

//error in cutter radius
E=(R/8.)*(D*D/(R*R))*(1./(sa*sa)-1.);

printf ("Resultant error in radius when using %.4lf in diameter cutter = %.4lf in\n",du,E);

/* post run processing */
mexit:
fcloseall();
exit(0);			/* normal exit */
}
/* ======================================================================== */
/* ------------------------------------------------------------------------ */
