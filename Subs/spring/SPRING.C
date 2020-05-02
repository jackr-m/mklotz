/* SPRING.C   M. W. Klotz  11/98

Helical and leaf spring design
(adapted from E. V. Rutkowski's original BASIC program)
Data and formulae adapted from Machinery's Handbook 13th Edition
DECEMBER 30, 1991                                  by  E.V.RUTKOWSKI

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
void init (void);
void helix (void);
void leaf (void);

/* gmark..-------------- global graphics variables ------------------------ */
/* some are contained in graphics.h */

/* vmark..------------ global variable declarations ----------------------- */
#define NS 19
char name [NS][80];	/* names of spring parameters */
dbl sp [NS];		/* values of spring parameters */
char stype;		/* spring type 1=helical, 2=leaf */
char hopt;		/* helical spring option */

/* smark..----------- global structure declarations ----------------------- */

/* main..================================================================== */

main (int argc, char *argv[], char *envp[] )

/* argc=count of command line arguments */
/* argv points to parameters, envp points to environment parameters */

{
int i,j,k;

/* -------------------------- initializations ------------------------------ */
textscreen ();
init ();
/* ------------------------------------------------------------------------- */

puts ("COIL AND LEAF SPRING DESIGN");
puts ("Adapted from Machinery's Handbook (13th Edition) by:");
puts ("  Eugene (Gene) V. Rutkowski 12/30/91\n\n");
puts ("1  Cylindrical Coil (Helical) Compression Springs");
puts ("2  Full and Half Elliptical Leaf Springs");
stype=getkey("Spring Type [1] ?"); if (stype != '2') stype='1';
if (stype == '1') helix ();
if (stype == '2') leaf ();
/* post run processing */
mexit:
exit(0);			/* normal exit */
}
/* ======================================================================== */
/* init..------------------------------------------------------------------ */

/* various initializations */

void init (void)

{
/* coil spring parameters */
strcpy (name[0],"Load (lbf)");
strcpy (name[1],"Outside Diameter (in)");
strcpy (name[2],"Length (in)");
strcpy (name[3],"Total Deflection (in)");
strcpy (name[4],"Allowable Working Stress (psi)");
strcpy (name[5],"Minimum Wire Diameter Based on Allowed Stress (in)");
strcpy (name[6],"Wire Diameter Based on Desired Deflection (in)");
strcpy (name[7],"Selected Wire Diameter (in)");
strcpy (name[8],"Number of Full Coils");
strcpy (name[9],"Pitch (in/coil)");
/* leaf spring parameters */
strcpy (name[10],"Leaf Spring Material (B)ronze or (S)teel");
strcpy (name[11],"Modulus");
strcpy (name[12],"Allowable Stress (psi)");
strcpy (name[13],"Leaf Thickness (in)");
strcpy (name[14],"Leaf Width (in)");
strcpy (name[15],"Spring Length (in)");
strcpy (name[16],"Number of Leaves in Half Spring");
strcpy (name[17],"Maximum Allowable Load (lbf)");
strcpy (name[18],"Spring Deflection Under Load (in)");
}
/* helix..----------------------------------------------------------------- */

/* helical spring design */

void helix (void)

{
int i;
dbl s,ts,d,mdia,whal,newd,d2,d3,maxcoil,ncoil;

ts=11500000.;			/* allowable torsional stress */
puts ("\nHELICAL COMPRESSION COIL SPRING DESIGN");
puts ("(Round Steel Wire <= 0.299 in Diameter)");
puts ("NB: Ends are assumed ground square\n");
puts ("1  Load, deflection and spring dimensions known. Calculate wire size.");
puts ("2  Spring dimensions, wire size known. Calculate deflection and stress.");
hopt=getkey ("Design Option [1] ?");
if (hopt != '2') hopt='1';
if (hopt == '1')
{
print;
for (i=0 ; i<4 ; i++) vin (name[i],&sp[i],"lf",0.,"");
d=pow((8.*sp[0]*.8*sp[1]*1.5/(PI*69000.)),0.333333);	/* first guess */
iter:
mdia=sp[1]-d;
whal=2.120672*pow((mdia/d),-0.268606);	/* whal's constant */
s=floor(43094-(11037*log(d)));		/* allowable stess, average service */
newd=pow((8.*sp[0]*mdia*whal/(PI*s)),0.333333);
if ((ABS(d-newd)/newd) > 0.0005) {d=newd; goto iter;}
sp[4]=s; sp[5]=newd;
d2=sp[5];
maxcoil=0.5*((sp[2]/sp[5])-1);
ncoil=ceil(0.5*(maxcoil+4));
iter2:
mdia=sp[1]-d2;
d3=pow(8.*sp[0]*ncoil*pow(mdia,3.)/(sp[3]*ts),0.25);
if (ABS(d3-d2) > 0.0005) {d2=0.5*(d3+d2); goto iter2;}
if (d3*(2.*ncoil-1.) > sp[2]) {ncoil--; goto iter2;}
sp[6]=d3;
printf ("%s = %.4lf\n",name[6],sp[6]);
if (sp[6] < sp[5])
  { beep; puts ("To meet required deflection, the spring would be overstressed");}
sp[8]=ncoil;
sp[9]=sp[2]/(ncoil-1.);
}
if (hopt == '2')
{
print;
for (i=0 ; i<3 ; i++) vin (name[i],&sp[i],"lf",0.,"");
vin (name[7],&sp[7],"lf",0.,"");
g7: if (sp[7] > 0.299)
 {beep; puts ("Wire Diameter outside program range (<= 0.299)"); goto g7;}
vin (name[8],&sp[8],"lf",0.,"");
sp[3]=8.*sp[0]*sp[8]*pow((sp[1]-sp[7]),3.)/(ts*pow(sp[7],4));
if (sp[3] > ((sp[8]+1)*sp[7]))
  {beep; puts ("Impossible design!  Spring collapses!"); return;}
sp[4]=floor(43094-(11037*log(sp[7])));	/* allowable stess, average service */
sp[9]=sp[2]/(sp[8]-1.);
}
puts ("\nSummary:\n");
for (i=0 ; i<10 ; i++)
{
if (hopt == '1' && i == 7) continue;
if (hopt == '2' && (i == 5 || i == 6)) continue;
printf ("%s = %.4lf\n",name[i],sp[i]);
}
}
/* leaf..------------------------------------------------------------------ */

/* leaf spring design */

void leaf (void)

{
int i;
char mat;

puts ("\nHALF/FULL ELLIPTICAL LEAF SPRING DESIGN\n");
mat=getkey("Leaf Spring Material (B)ronze or [S]teel ?");
if (mat != 'b') mat='s';
if (mat == 'b')
{
sp[11]=31000; sp[12]=7000000;
strcpy (name[10],"Spring Material = Phosphor Bronze");
}
else
{
sp[11]=80000; sp[12]=25400000;
strcpy (name[10],"Spring Material = Carbon Steel");
}
puts ("gage             thickness (in)");
puts ("  43             0.0022");
puts ("  35             0.0056");
puts ("  29             0.0113");
puts ("  26             0.0159");
puts ("  24             0.0201");
puts ("  21             0.0285");
puts ("  19             0.0359");
puts ("  17             0.0453");
puts ("  16             0.0508");
puts ("  14             0.0641");
puts ("  13             0.0720");
puts ("  10             0.1019");
puts ("   8             0.1285");
puts ("   4             0.2043");
puts ("   2             0.2576");
puts ("   0             0.3249");
g13: vin (name[13],&sp[13],"lf",0.,"");
if (sp[13] > 0.9)
 {beep; puts ("Thickness must be <= 0.9"); goto g13;}
for (i=14 ; i<17 ; i++)
{
if (i == 15)
puts ("Spring length is measured from center of spring to support (1/2 oal)");
vin (name[i],&sp[i],"lf",0.,"");
}
sp[17]=2.*sp[11]*sp[14]*sp[13]*sp[13]*sp[16]/(3.*sp[15]);
sp[18]=sp[11]*sp[15]*sp[15]/(4.*sp[12]*sp[13]);

puts ("\nSummary:\n");
printf ("%s\n",name[10]);
for (i=11 ; i<19 ; i++) printf ("%s = %.4lf\n",name[i],sp[i]);
puts ("\nThe maximum load is the same for both types of leaf spring.");
puts ("The deflection of the full elliptical spring is TWICE that of the");
puts ("half elliptical.");
}
/* ------------------------------------------------------------------------ */
