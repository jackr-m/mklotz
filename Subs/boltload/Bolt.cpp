
#include <condefs.h>
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#pragma hdrstop

/*
Program Copyright Richard Williams (R.W.W) e-mail richardandtracy@yahoo.com

Permission is hereby granted to anyone who wants to, to use and distribute the
program free of charge for non-commercial use. Commercial use must be discussed
with R.W.W first.

The source code is bundled with the program so that you can follow the code if
you wish to. HOWEVER I WOULD VERY, VERY, VERY STRONGLY RECOMMEND THAT YOU DO NOT
MODIFY THE CODE. The unit load calculation routine is exceedingly complex, and
will give wildly inaccurate results if so much as a + or - is changed. [To give
an example, while debugging I had a + where there should have been a -, and the
analysis converged on a figure of 8.63E305, when it should have been -0.500.]


There - those conditions aren't too onerous are they?



Please forgive me as this is only my third DOS console program
and it isn't as efficient as it could be! This program started as a 3.6Mb
Windows exe file with about 1.5Mb of ancilliary bits - so it is a bit smaller
than it was....  And a lot of user friendliness has been sacrificed. If you
want the full Windows program, contact me at richardandtracy@yahoo.com, but I
warn you, the installation package is around 3 Mb fully zipped with all the
compression I can get.

I have retained a lot of the original Borland VCL of the original file to make
the "DOS-ING" of the file quicker - but at the sacrifice of a certain amount of
file size. If you have any arguements about the program - all I can say is "Show
me a better way, then". I'm entirely self taught, so I am certain to have done
some things the hard way (and I don't mind being shown a better way...).

*/





//Structures
typedef struct  {
    double x, y, z;
    } nodalposition;

typedef struct  {
    double aa, ba, ca;
    double ab, bb, cb;
    double ac, bc, cc;
    } matrix;

enum TBoltOrientation {tboNotDefined, tboXTensionYZShear,
                       tboYTensionXZShear, tboZTensionXYShear};

typedef struct  {
    double x, y, z;
    double fx, fy, fz;
    int nodenumber;
    TBoltOrientation orientation;
    } boltproperties;

//next one is for unit interface loads at a bolt, Axis for direction of applied load
//component direction for a load caused in that global axis direction by the applied load.
//So an X axis applied load may cause XAxis_XComponent, XAxis_YComponent and
//XAxis_ZComponent to be non zero.
typedef struct {
    double XAxis_XComponent, XAxis_YComponent, XAxis_ZComponent;
    double YAxis_XComponent, YAxis_YComponent, YAxis_ZComponent;
    double ZAxis_XComponent, ZAxis_YComponent, ZAxis_ZComponent;
    } interfaceload;

typedef struct {
    double XAxis_YComponent, XAxis_ZComponent;
    double YAxis_XComponent, YAxis_ZComponent;
    double ZAxis_XComponent, ZAxis_YComponent;
    } axiscomponent;

typedef struct {
    double x, y, z;
    double XYZResultant, XYPlane, XZPlane, YZPlane;
    } actualloads;


typedef struct {
    int LineNumber;
    AnsiString Text;
    } LineSearch;





//Global Variables
    AnsiString FileName;
    AnsiString EndOfFileName;

    TStringList *inputfile;
    TStringList *outputfile;
    AnsiString FileExtension;
    bool fBusy;
    int counter;
    DynamicArray<boltproperties> BoltPosition; //actual 3D bolt positions
    DynamicArray<interfaceload> UnitInterfaceLoads;
    DynamicArray<actualloads> ActualInterfaceLoads;
    int numberofbolts;
    //with AppliedLoad x, y and z are the position of the load
    //                 fx, fy and fz are the applied load components
    boltproperties AppliedLoad;
    bool loadpresent;
    boltproperties UnitLoad;
    AnsiString AnalysisTitle;
    AnsiString LoadCaseTitle;
    double ResidualMoment;
    bool UnitLoadsCalculated;
    bool ActualLoadsCalculated;


//Prototyping functions
    void Open(const AnsiString AFileName);
    AnsiString GetData(AnsiString Data, AnsiString Default);
    boltproperties ExtractBoltData(AnsiString boltdata);

    void CalculateUnitLoads(void);
    void CalculateActualLoads(void);


    //from "Stringhelp"
    double ToDoubleDef(AnsiString Value, double DefaultVal);
    AnsiString ExtractWord(AnsiString string, AnsiString Delimiter);
    AnsiString DeleteWord(AnsiString string, AnsiString word);
    AnsiString FixedLen(int Length, bool value);
    AnsiString FixedLen(int Length, int value);
    AnsiString FixedLen(int Length, long double value, TFloatFormat format, int precision, int digits);
    AnsiString FixedLen(int Length, AnsiString string);

    //methods from TBaseForm - converted to functions
    void addblank (void);
    void ClearLists(void);
    void OutputAdd(AnsiString temp);
    void InputAdd(AnsiString temp);
    int InputSearch(int i, AnsiString Search);
    AnsiString InputString(int i);
    void InputDelete(int i);
    void OutputInsert(int i, AnsiString text);
    int OutputSearch(int i, AnsiString Search);
    int OutputLength(void);
    void OutputDelete(int i);
    int InputLength(void);
    void InputClear(void);
    void OutputClear(void);
    AnsiString OutputString(int i);
    void OutputSave(AnsiString filename);
    LineSearch InputFind(AnsiString text);
    TStringList * OutputLines(void);





/*


To those who wish to understand this code, I offer my wholehearted sympathy

                  ha! Ha! HA!


*/





//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{
    //setup data
    ResidualMoment = 0.00001;
    AnalysisTitle = "Untitled Model";
    LoadCaseTitle = "Untitled Load Case";
    loadpresent = false;

    UnitLoadsCalculated = false;
    ActualLoadsCalculated = false;
    inputfile = new TStringList();
    outputfile = new TStringList();


    //checks to see whether it's being used correctly
	if (argc < 2) {
		fprintf(stderr,"Usage: %s Filename\n",argv[0]);
		exit(-1);
	    }

    FileName = AnsiString(argv[1]);

    //does the opening of the files.
    try {
        Open(FileName);
        }
    __finally {
        if (!fBusy) {

            OutputSave(ExtractFileDir(FileName) + "BoltLoad.txt");
            for (int i = 0; i < OutputLength(); i++) {
                printf((OutputString(i) + "\n").c_str());
                }
            /*

            Now will the last one out, please turn off the lights.

            */

            }
        delete inputfile;
        delete outputfile;
        }



        printf("Finished\n");


        return 0;
}


//---------------------------------------------------------------------------

void Open(const AnsiString AFileName)
{
    //does the opening of the files.
    try {
        //just to prevent premature closure
        fBusy = true;

        numberofbolts = 0;
        BoltPosition.Length = 0;
        AppliedLoad.x = 0.0;
        AppliedLoad.y = 0.0;
        AppliedLoad.z = 0.0;
        AppliedLoad.fx = 0.0;
        AppliedLoad.fy = 0.0;
        AppliedLoad.fz = 0.0;
        UnitLoad.x = 0.0;
        UnitLoad.y = 0.0;
        UnitLoad.z = 0.0;
        UnitLoad.fx = 1.0;
        UnitLoad.fy = 1.0;
        UnitLoad.fz = 1.0;
        AnalysisTitle = "";
        LoadCaseTitle = "";

        if (FileExists(AFileName)) {
                //new unlimited input
                //open file
                inputfile->LoadFromFile(AFileName);
                AnsiString AnalysisType = GetData("Analysis Type", "None");
                if (AnalysisType.Pos("Bolt Group")) {
                    //open file data here
                    AnalysisTitle = GetData("Analysis Title", "Untitled Model");
                    LoadCaseTitle = GetData("Load Case Title", "Untitled Load Case");
                    numberofbolts = GetData("Number of Bolts", "0").ToIntDef(0);
                    BoltPosition.Length = numberofbolts;
                    for (int i = 0; i < numberofbolts; i++) {
                        BoltPosition[i] = ExtractBoltData(GetData("Bolt " + AnsiString(i + 1),"0,0,0,1,1,1,0,0"));
                        }
                    AppliedLoad = ExtractBoltData(GetData("Applied Load","0,0,0,1,1,1,0,0"));
                    UnitLoad.x = AppliedLoad.x;
                    UnitLoad.y = AppliedLoad.y;
                    UnitLoad.z = AppliedLoad.z;
                    if (AppliedLoad.fx != 0.0 || AppliedLoad.fy != 0.0 || AppliedLoad.fz != 0.0) loadpresent = true;
                    CalculateUnitLoads();
                    CalculateActualLoads();
                    } else {
                    OutputAdd("Not a Bolt Group Analysis Data File");
                    }
            } else {
            OutputAdd("File \"" + AFileName +"\" Not Found");
            }
        }

    __finally {
        fBusy = false;
        }



}
//---------------------------------------------------------------------------


AnsiString GetData(AnsiString Data, AnsiString Default)
{
    LineSearch Search = InputFind(Data);

    if (Search.Text.Pos("=")) {
        Search.Text.Delete(1,Search.Text.Pos("="));

        } else {
        Search.Text = Default;
        }
    if (Search.LineNumber > 0) InputDelete(Search.LineNumber);

    return Search.Text;
}
//---------------------------------------------------------------------------

boltproperties ExtractBoltData(AnsiString boltdata)
{
    boltproperties bolt;
    AnsiString temp;
    boltdata += ",";
    temp = ExtractWord(boltdata,",");
    bolt.x = ToDoubleDef(temp,0.0);
    boltdata = DeleteWord(boltdata,temp);
    temp = ExtractWord(boltdata,",");
    bolt.y = ToDoubleDef(temp,0.0);
    boltdata = DeleteWord(boltdata,temp);
    temp = ExtractWord(boltdata,",");
    bolt.z = ToDoubleDef(temp,0.0);
    boltdata = DeleteWord(boltdata,temp);
    temp = ExtractWord(boltdata,",");
    bolt.fx = ToDoubleDef(temp,1.0);
    boltdata = DeleteWord(boltdata,temp);
    temp = ExtractWord(boltdata,",");
    bolt.fy = ToDoubleDef(temp,1.0);
    boltdata = DeleteWord(boltdata,temp);
    temp = ExtractWord(boltdata,",");
    bolt.fz = ToDoubleDef(temp,1.0);
    boltdata = DeleteWord(boltdata,temp);
    temp = ExtractWord(boltdata,",");
    bolt.orientation = (TBoltOrientation) temp.ToIntDef(0);
    boltdata = DeleteWord(boltdata,temp);
    temp = ExtractWord(boltdata,",");
    bolt.nodenumber = temp.ToIntDef(0);

    temp.~AnsiString();

    return bolt;
}
//---------------------------------------------------------------------------

void addblank (void)
{
	//adds blank line to outputfile
    outputfile->Add(" ");
}
//---------------------------------------------------------------------------


void ClearLists(void)
{
    outputfile->Clear();
    inputfile->Clear();

}
//---------------------------------------------------------------------------

void OutputSave(AnsiString filename)
{
    outputfile->SaveToFile(filename);
}
//---------------------------------------------------------------------------

void OutputAdd(AnsiString temp)
{
	outputfile->Add(temp);

}
//---------------------------------------------------------------------------

void InputAdd(AnsiString temp)
{
	inputfile->Add(temp);

}
//---------------------------------------------------------------------------

int InputSearch(int i, AnsiString Search)
{
    //returns the position of Search in input file line i
    if (i > -1 && i < inputfile->Count) {
        return inputfile->Strings[i].Pos(Search);
        } else {
        return 0;
        }
}
//---------------------------------------------------------------------------

AnsiString InputString(int i)
{
    //returns inputfile->Strings[i]
    //if i is within range, returns the string, else returns
    //a null string.
    if (i < inputfile->Count && i > -1) {
        return inputfile->Strings[i];
        } else {
        return "";
        }

}
//---------------------------------------------------------------------------

void InputDelete(int i)
{
    if (i > -1 && i < inputfile->Count) {
        inputfile->Delete(i);
        }
}
//---------------------------------------------------------------------------

void OutputDelete(int i)
{
    if (i > -1 && i < outputfile->Count) {
        outputfile->Delete(i);
        }
}
//---------------------------------------------------------------------------

void OutputInsert(int i, AnsiString text)
{
    if (i > -1 && i < outputfile->Count) {
        outputfile->Insert(i, text);
        }
}
//---------------------------------------------------------------------------

int OutputSearch(int i, AnsiString Search)
{
    //returns the position of Search in input file line i
    if (i > -1 && i < outputfile->Count) {
        return outputfile->Strings[i].Pos(Search);
        } else {
        return 0;
        }
}
//---------------------------------------------------------------------------

AnsiString OutputString(int i)
{
    //add in a catch to make this bomb proof.
    if (i < outputfile->Count && i > -1) {
        return outputfile->Strings[i];
        } else {
        return "";
        }

}
//---------------------------------------------------------------------------

int OutputLength(void)
{
    return outputfile->Count;
}
//---------------------------------------------------------------------------

int InputLength(void)
{
    return inputfile->Count;
}
//---------------------------------------------------------------------------

void InputClear(void)
{
    //allows the input file to be cleared without getting rid of data in the
    //output file
    if (inputfile->Count) inputfile->Clear();
}
//---------------------------------------------------------------------------

void OutputClear(void)
{
    //allows the output file to be cleared without getting rid of data in the
    //input file
    if (outputfile->Count) outputfile->Clear();
}
//---------------------------------------------------------------------------

LineSearch InputFind(AnsiString text)
{
    LineSearch Search;
    Search.LineNumber = 0;

    Search.LineNumber = inputfile->IndexOfName(text);
    if (Search.LineNumber > -1) Search.Text = InputString(Search.LineNumber);

    return Search;

}
//---------------------------------------------------------------------------

TStringList * OutputLines(void)
{
    return outputfile;
}
//---------------------------------------------------------------------------


void CalculateUnitLoads(void)
{
try {
    fBusy = true;

    //calculates the unit loads
    UnitLoadsCalculated = false;

    if (numberofbolts > 2 && loadpresent) {
        printf("Calculating Unit Loads\n");
        //this bit is for the calculation of unit loads
        //set up local variables
        //variable for sum of bolt properties
        DynamicArray<axiscomponent> BoltStiffnesses;
        axiscomponent BoltStiffnessesSum;
        axiscomponent MeanPositions;
        DynamicArray<axiscomponent> BoltCentroidRadii;
        DynamicArray<axiscomponent> BoltCentroidRSquared;
        nodalposition MomentRSquared;
        interfaceload UnitInterfaceLoadsSum;
        DynamicArray<interfaceload> UnitInterfaceMoment;
        interfaceload UnitInterfaceMomentSum;

        double l = AppliedLoad.x;
        double M = AppliedLoad.y;
        double o = AppliedLoad.z;
        double w = 1.0;
        int collector20 = 0;
        DynamicArray <double> x;
        x.Length = numberofbolts + 1;
        DynamicArray <double> y;
        y.Length = numberofbolts + 1;
        DynamicArray <double> z;
        z.Length = numberofbolts + 1;
        x[collector20] = 0.0;
        y[collector20] = 0.0;
        z[collector20] = 0.0;
        DynamicArray < DynamicArray <double> > f;
        f.Length = 4;
        f[0].Length = 1;
        f[1].Length = numberofbolts + 1;
        f[2].Length = numberofbolts + 1;
        f[3].Length = numberofbolts + 1;
        f[1][collector20] = 0.0;
        f[2][collector20] = 0.0;
        f[3][collector20] = 0.0;
        //copy across the bolt positions into the BASIC derived code
        for (int n = 1; n < numberofbolts + 1; n++) {
            x[n] = BoltPosition[n - 1].x;
            y[n] = BoltPosition[n - 1].y;
            z[n] = BoltPosition[n - 1].z;
            f[1][n] = BoltPosition[n - 1].fx;
            f[2][n] = BoltPosition[n - 1].fy;
            f[3][n] = BoltPosition[n - 1].fz;
            }
        //continue with the BASIC derived initialisation
        int q = numberofbolts;
        int k;
        int numberofloops = 1;

        DynamicArray <DynamicArray <double> > A;
        DynamicArray <double> B;
        DynamicArray <DynamicArray <double> > C;
        DynamicArray <DynamicArray <double> > d;
        A.Length = 7;
        B.Length = 7;
        C.Length = 7;
        d.Length = 7;
        for (int n = 1; n < 7; n++) {
            A[n].Length = numberofbolts + 1;
            C[n].Length = numberofbolts + 1;
            d[n].Length = numberofbolts + 1;
            }
        double i = ResidualMoment;
        for (int countr = 1; countr < 7; countr++) {
            for (int pointer = 0; pointer < numberofbolts + 1; pointer++) {
                A[countr][pointer] = 0.0;
                C[countr][pointer] = 0.0;
                d[countr][pointer] = 0.0;
                }
            B[countr] = 0.0;
            }

        DynamicArray < DynamicArray < double> > R;
        R.Length = 10;

        DynamicArray < DynamicArray < double> > v;
        v.Length = 10;

        DynamicArray < DynamicArray < double> > m;
        m.Length = 10;
        DynamicArray < DynamicArray < double> > S;
        S.Length = 10;
        for (int countr = 1; countr < 10; countr++) {
            m[countr].Length = numberofbolts + 1;
            S[countr].Length = numberofbolts + 1;
            for (int pointer = 1; pointer < numberofbolts + 1; pointer++) {
                m[countr][pointer] = 0.0;
                S[countr][pointer] = 0.0;
                }
            }
        for (int countr = 1; countr < 10; countr++) {
            m[countr][collector20] = 0.0;
            }

        DynamicArray <double> e;
        e.Length = 4;
        for (int countr = 1; countr < 4; countr++) {
            e[countr] = 0.0;
            }

        DynamicArray <double> g;
        g.Length = 7;
        for (int countr = 1; countr < 7; countr++) {
            g[countr] = 0.0;
            }

        DynamicArray <double> h;
        h.Length = 10;
        for (int countr = 1; countr < 10; countr++) {
            h[countr] = 0.0;
            }


        //end of initialisation
        //start of unit load calc
        for (int n = 1; n < q + 1; n++) {
            x[collector20] += x[n];             //sig.x
            y[collector20] += y[n];             //sig.y
            z[collector20] += z[n];             //sig.z
            f[1][collector20] += f[1][n];       //sig.fx
            f[2][collector20] += f[2][n];       //sig.fy
            f[3][collector20] += f[3][n];       //sig.fz
            A[1][n] = f[1][n] * y[n];           //fx(y)
            A[2][n] = f[1][n] * z[n];           //fx(z)
            A[3][n] = f[2][n] * x[n];           //fy(x)
            A[4][n] = f[2][n] * z[n];           //fy(z)
            A[5][n] = f[3][n] * x[n];           //fz(x)
            A[6][n] = f[3][n] * y[n];           //fz(y)
            A[1][collector20] += A[1][n];       //sig. fx(y)
            A[2][collector20] += A[2][n];       //sig. fx(z)
            A[3][collector20] += A[3][n];       //sig. fy(x)
            A[4][collector20] += A[4][n];       //sig. fy(z)
            A[5][collector20] += A[5][n];       //sig. fz(x)
            A[6][collector20] += A[6][n];       //sig. fz(y)
            }

        B[1] = A[6][collector20]/ f[3][collector20];     //bar y for Mx
        B[2] = A[4][collector20]/ f[2][collector20];     //bar z for Mx
        B[3] = A[5][collector20]/ f[3][collector20];     //bar x for My
        B[4] = A[2][collector20]/ f[1][collector20];     //bar z for My
        B[5] = A[3][collector20]/ f[2][collector20];     //bar x for Mz
        B[6] = A[1][collector20]/ f[1][collector20];     //bar y for Mz

        for (int n = 1; n < q + 1; n++) {
            C[1][n] = y[n] - B[1];                      //y for Mx
            C[2][n] = z[n] - B[2];                      //z for Mx
            C[3][n] = x[n] - B[3];                      //x for My
            C[4][n] = z[n] - B[4];                      //z for My
            C[5][n] = x[n] - B[5];                      //x for Mz
            C[6][n] = y[n] - B[6];                      //y for Mz
            }

        for (int n = 1; n < q + 1; n++) {
            d[1][n] = f[2][n] * C[2][n] * C[2][n];      //z comp of r^2 for Mx
            d[2][n] = f[3][n] * C[1][n] * C[1][n];      //y comp of r^2 for Mx
            d[3][n] = f[1][n] * C[4][n] * C[4][n];      //z comp of r^2 for My
            d[4][n] = f[3][n] * C[3][n] * C[3][n];      //x comp of r^2 for My
            d[5][n] = f[1][n] * C[6][n] * C[6][n];      //y comp of r^2 for Mz
            d[6][n] = f[2][n] * C[5][n] * C[5][n];      //x comp of r^2 for Mz
            d[1][collector20] += d[1][n];               //sig.z-comp of r^2 for Mx
            d[2][collector20] += d[2][n];               //sig.y-comp of r^2 for Mx
            d[3][collector20] += d[3][n];               //sig.z-comp of r^2 for My
            d[4][collector20] += d[4][n];               //sig.x-comp of r^2 for My
            d[5][collector20] += d[5][n];               //sig.y-comp of r^2 for Mz
            d[6][collector20] += d[6][n];               //sig.x-comp of r^2 for Mz
            }

        e[1] = d[1][collector20] + d[2][collector20];   //r^2 for Mx
        e[2] = d[3][collector20] + d[4][collector20];   //r^2 for My
        e[3] = d[5][collector20] + d[6][collector20];   //r^2 for Mz

        g[1] = w * (o - B[4]);                          //My for load in x dir
        g[2] = w * (M - B[6]);                          //Mz for load in x dir
        g[3] = w * (o - B[2]);                          //Mx for load in y dir
        g[4] = 0.0 - w * (l - B[5]);                      //Mz for load in y dir
        g[5] = 0.0 - w * (M - B[1]);                      //Mx for load in z dir
        g[6] = 0.0 - w * (l - B[3]);                      //My for load in z dir

        h[1] = g[1]/e[2];                       //My/sig.r^2 for load in x-dir
        h[2] = g[2]/e[3];                       //Mz/sig.r^2 for load in x-dir
        h[3] = g[3]/e[1];                       //Mx/sig.r^2 for load in y-dir
        h[4] = g[4]/e[3];                       //Mz/sig.r^2 for load in y-dir
        h[5] = g[5]/e[1];                       //Mx/sig.r^2 for load in z-dir
        h[6] = g[6]/e[2];                       //My/sig.r^2 for load in z-dir

        h[7] = 0.0 + w/f[1][collector20];         //factor for direct x load
        h[8] = 0.0 - w/f[2][collector20];         //factor for direct x load
        h[9] = 0.0 - w/f[3][collector20];         //factor for direct x load

        for (int n = 1; n < q + 1; n++) {
            S[1][n] = f[1][n] * (h[7] + h[1] * C[4][n] + h[2] * C[6][n]);   //x load for x load applied
            S[2][n] = f[2][n] * h[2] * C[5][n]; //y load for x load applied
            S[3][n] = f[3][n] * h[1] * C[3][n]; //z load for x load applied
            S[4][n] = f[1][n] * h[4] * C[6][n]; //x load for y load applied
            S[5][n] = f[2][n] * (h[8] + h[4] * C[5][n] - h[3] * C[2][n]);   //y load for y load applied
            S[6][n] = f[3][n] * h[3] * C[1][n]; //z load for y load applied
            S[7][n] = f[1][n] * h[6] * C[4][n]; //x load for z load applied
            S[8][n] = 0.0 - f[2][n] * h[5] * C[2][n]; //y load for z load applied
            S[9][n] = f[3][n] * (h[9] + h[6] * C[3][n] + h[5] * C[1][n]);   //z load for z load applied
            S[1][collector20] += S[1][n];
            S[2][collector20] += S[2][n];
            S[3][collector20] += S[3][n];
            S[4][collector20] += S[4][n];
            S[5][collector20] += S[5][n];
            S[6][collector20] += S[6][n];
            S[7][collector20] += S[7][n];
            S[8][collector20] += S[8][n];
            S[9][collector20] += S[9][n];
            }

        for (int n = 1; n < q + 1; n++) {
            m[1][n] = 0.0 - S[2][n] * C[2][n] + S[3][n] * C[1][n]; //Mx for X load
            m[2][n] = 0.0 + S[1][n] * C[4][n] + S[3][n] * C[3][n]; //My for X load
            m[3][n] = 0.0 + S[1][n] * C[6][n] + S[2][n] * C[5][n]; //Mz for X load

            m[4][n] = 0.0 - S[5][n] * C[2][n] + S[6][n] * C[1][n]; //Mx for Y load
            m[5][n] = 0.0 + S[4][n] * C[4][n] + S[6][n] * C[3][n]; //My for Y load
            m[6][n] = 0.0 + S[4][n] * C[6][n] + S[5][n] * C[5][n]; //Mz for y load

            m[7][n] = 0.0 - S[8][n] * C[2][n] + S[9][n] * C[1][n]; //Mx for z load
            m[8][n] = 0.0 + S[7][n] * C[4][n] + S[9][n] * C[3][n]; //My for z load
            m[9][n] = 0.0 + S[7][n] * C[6][n] + S[8][n] * C[5][n]; //Mz for z load

            m[1][collector20] += m[1][n];       //sig.Mx for x load
            m[2][collector20] += m[2][n];       //sig.My for x load
            m[3][collector20] += m[3][n];       //sig.Mz for x load
            m[4][collector20] += m[4][n];       //sig.Mx for y load
            m[5][collector20] += m[5][n];       //sig.My for y load
            m[6][collector20] += m[6][n];       //sig.Mz for y load
            m[7][collector20] += m[7][n];       //sig.Mx for z load
            m[8][collector20] += m[8][n];       //sig.My for z load
            m[9][collector20] += m[9][n];       //sig.Mz for z load

            }

        if (fabs(m[1][collector20]) > i) goto calcjump1;
        if (fabs(m[2][collector20] - g[1]) > i) goto calcjump1;
        if (fabs(m[3][collector20] - g[2]) > i) goto calcjump1;
        if (fabs(m[4][collector20] - g[3]) > i) goto calcjump1;
        if (fabs(m[5][collector20]) > i) goto calcjump1;
        if (fabs(m[6][collector20] - g[4]) > i) goto calcjump1;
        if (fabs(m[7][collector20] - g[5]) > i) goto calcjump1;
        if (fabs(m[8][collector20] - g[6]) > i) goto calcjump1;
        if (fabs(m[9][collector20]) > i) goto calcjump1;

        goto calcjump2;


        calcjump1:
        k = 1;
        for (int n = 1; n < 10; n++) {
            R[n].Length = k + 1;
            }
        R[1][k] = 0.0  - m[1][collector20]; //-Mx out of balance x load applied
        R[2][k] = g[1] - m[2][collector20]; //-My out of balance x load applied
        R[3][k] = g[2] - m[3][collector20]; //-Mz out of balance x load applied

        R[4][k] = g[3] - m[4][collector20]; //-Mx out of balance y load applied
        R[5][k] = 0.0  - m[5][collector20]; //-My out of balance y load applied
        R[6][k] = g[4] - m[6][collector20]; //-Mz out of balance z load applied

        R[7][k] = g[5] - m[7][collector20]; //-Mx out of balance z load applied
        R[8][k] = g[6] - m[8][collector20]; //-My out of balance z load applied
        R[9][k] = 0.0  - m[9][collector20]; //-Mz out of balance z load applied



        calcjump3:
        for (int n = 1; n < q + 1; n++) {
            int factor = n + k * q;
            v[1].Length = factor + 1;
            v[2].Length = factor + 1;
            v[3].Length = factor + 1;
            v[4].Length = factor + 1;
            v[5].Length = factor + 1;
            v[6].Length = factor + 1;
            v[7].Length = factor + 1;
            v[8].Length = factor + 1;
            v[9].Length = factor + 1;
            
            v[1][factor] = f[1][n] * (0.0 + R[2][k] * C[4][n]/e[2] + R[3][k] * C[6][n]/e[3]);     //x load for x load applied
            v[2][factor] = f[2][n] * (0.0 - R[1][k] * C[2][n]/e[1] + R[3][k] * C[5][n]/e[3]);     //y load for x load applied
            v[3][factor] = f[3][n] * (0.0 + R[1][k] * C[1][n]/e[1] + R[2][k] * C[3][n]/e[2]);     //z load for x load applied

            v[4][factor] = f[1][n] * (0.0 + R[5][k] * C[4][n]/e[2] + R[6][k] * C[6][n]/e[3]);     //x load for y load applied
            v[5][factor] = f[2][n] * (0.0 - R[4][k] * C[2][n]/e[1] + R[6][k] * C[5][n]/e[3]);     //y load for y load applied
            v[6][factor] = f[3][n] * (0.0 + R[4][k] * C[1][n]/e[1] + R[5][k] * C[3][n]/e[2]);     //z load for y load applied

            v[7][factor] = f[1][n] * (0.0 + R[8][k] * C[4][n]/e[2] + R[9][k] * C[6][n]/e[3]);     //x load for z load applied
            v[8][factor] = f[2][n] * (0.0 - R[7][k] * C[2][n]/e[1] + R[9][k] * C[5][n]/e[3]);     //y load for z load applied
            v[9][factor] = f[3][n] * (0.0 + R[7][k] * C[1][n]/e[1] + R[8][k] * C[3][n]/e[2]);     //z load for z load applied

            }

        for (int U = 1; U < 10; U++) {
            m[U][collector20] = 0.0;
            }

        for (int n = 1; n < q + 1; n++) {
            int factor = n + k * q;
            m[1][n] = 0.0 - (0.0 - v[2][factor] * C[2][n] + v[3][factor] * C[1][n]);
            m[2][n] = 0.0 - (0.0 + v[1][factor] * C[4][n] + v[3][factor] * C[3][n]);
            m[3][n] = 0.0 - (0.0 + v[1][factor] * C[6][n] + v[2][factor] * C[5][n]);

            m[4][n] = 0.0 - (0.0 - v[5][factor] * C[2][n] + v[6][factor] * C[1][n]);
            m[5][n] = 0.0 - (0.0 + v[4][factor] * C[4][n] + v[6][factor] * C[3][n]);
            m[6][n] = 0.0 - (0.0 + v[4][factor] * C[6][n] + v[5][factor] * C[5][n]);

            m[7][n] = 0.0 - (0.0 - v[8][factor] * C[2][n] + v[9][factor] * C[1][n]);
            m[8][n] = 0.0 - (0.0 + v[7][factor] * C[4][n] + v[9][factor] * C[3][n]);
            m[9][n] = 0.0 - (0.0 + v[7][factor] * C[6][n] + v[8][factor] * C[5][n]);
            }

        for (int n = 1; n < q + 1; n++) {
            m[1][collector20] += m[1][n];       //sig.Mx for x load
            m[2][collector20] += m[2][n];       //sig.My for x load
            m[3][collector20] += m[3][n];       //sig.Mz for x load
            m[4][collector20] += m[4][n];       //sig.Mx for y load
            m[5][collector20] += m[5][n];       //sig.My for y load
            m[6][collector20] += m[6][n];       //sig.Mz for y load
            m[7][collector20] += m[7][n];       //sig.Mx for z load
            m[8][collector20] += m[8][n];       //sig.My for z load
            m[9][collector20] += m[9][n];       //sig.Mz for z load
            }

        k++;
        if (k > 200) goto calcjump2;

        for (int n = 1; n < 10; n++) {
            R[n].Length = k + 1;
            R[n][k] = m[n][collector20] + R[n][k-1];
            }

        if (fabs(R[1][k]) > i) goto calcjump3;
        if (fabs(R[2][k]) > i) goto calcjump3;
        if (fabs(R[3][k]) > i) goto calcjump3;
        if (fabs(R[4][k]) > i) goto calcjump3;
        if (fabs(R[5][k]) > i) goto calcjump3;
        if (fabs(R[6][k]) > i) goto calcjump3;
        if (fabs(R[7][k]) > i) goto calcjump3;
        if (fabs(R[8][k]) > i) goto calcjump3;
        if (fabs(R[9][k]) > i) goto calcjump3;

        numberofloops = k;


        calcjump2:

        for (int n = 1; n < q + 1; n++) {
            for (int countr = 1; countr < numberofloops + 1; countr++) {
                k = countr;
                int factor = n + k * q;
                if (factor < v[1].Length) {
                    S[1][n] += v[1][factor];
                    S[2][n] += v[2][factor];
                    S[3][n] += v[3][factor];
                    S[4][n] += v[4][factor];
                    S[5][n] += v[5][factor];
                    S[6][n] += v[6][factor];
                    S[7][n] += v[7][factor];
                    S[8][n] += v[8][factor];
                    S[9][n] += v[9][factor];
                    }
                }
            }

        //now tweak to aft = +X, Stbd = + y, Up = +Z
        for (int n = 1; n < q + 1; n++) {
            S[2][n] = -S[2][n];
            S[3][n] = -S[3][n];
            S[5][n] = -S[5][n];
            S[6][n] = -S[6][n];
            S[8][n] = -S[8][n];
            S[9][n] = -S[9][n];
            }


        //end of converted BASIC method

        //now copy across to C++ arrays & clear out un-required arrays
        //clear v array
        for (int n = 0; n < 10; n++) {
            v[n].Length = 0;
            }
        v.Length = 0;

        //pass over Unit Interface Loads
        UnitInterfaceLoads.Length = numberofbolts;

        for (int n = 1; n < q + 1; n++) {
            UnitInterfaceLoads[n-1].XAxis_XComponent = S[1][n];
            UnitInterfaceLoads[n-1].XAxis_YComponent = S[2][n];
            UnitInterfaceLoads[n-1].XAxis_ZComponent = S[3][n];
            UnitInterfaceLoads[n-1].YAxis_XComponent = S[4][n];
            UnitInterfaceLoads[n-1].YAxis_YComponent = S[5][n];
            UnitInterfaceLoads[n-1].YAxis_ZComponent = S[6][n];
            UnitInterfaceLoads[n-1].ZAxis_XComponent = S[7][n];
            UnitInterfaceLoads[n-1].ZAxis_YComponent = S[8][n];
            UnitInterfaceLoads[n-1].ZAxis_ZComponent = S[9][n];
            }
        for (int n = 0; n < 10; n++) {
            S[n].Length = 0;
            }
        S.Length = 0;

        //unit interface loads sum
        UnitInterfaceLoadsSum.XAxis_XComponent = 0.0;
        UnitInterfaceLoadsSum.XAxis_YComponent = 0.0;
        UnitInterfaceLoadsSum.XAxis_ZComponent = 0.0;
        UnitInterfaceLoadsSum.YAxis_XComponent = 0.0;
        UnitInterfaceLoadsSum.YAxis_YComponent = 0.0;
        UnitInterfaceLoadsSum.YAxis_ZComponent = 0.0;
        UnitInterfaceLoadsSum.ZAxis_XComponent = 0.0;
        UnitInterfaceLoadsSum.ZAxis_YComponent = 0.0;
        UnitInterfaceLoadsSum.ZAxis_ZComponent = 0.0;
        for (int i = 0; i < numberofbolts; i++) {
            UnitInterfaceLoadsSum.XAxis_XComponent += UnitInterfaceLoads[i].XAxis_XComponent;
            UnitInterfaceLoadsSum.XAxis_YComponent += UnitInterfaceLoads[i].XAxis_YComponent;
            UnitInterfaceLoadsSum.XAxis_ZComponent += UnitInterfaceLoads[i].XAxis_ZComponent;
            UnitInterfaceLoadsSum.YAxis_XComponent += UnitInterfaceLoads[i].YAxis_XComponent;
            UnitInterfaceLoadsSum.YAxis_YComponent += UnitInterfaceLoads[i].YAxis_YComponent;
            UnitInterfaceLoadsSum.YAxis_ZComponent += UnitInterfaceLoads[i].YAxis_ZComponent;
            UnitInterfaceLoadsSum.ZAxis_XComponent += UnitInterfaceLoads[i].ZAxis_XComponent;
            UnitInterfaceLoadsSum.ZAxis_YComponent += UnitInterfaceLoads[i].ZAxis_YComponent;
            UnitInterfaceLoadsSum.ZAxis_ZComponent += UnitInterfaceLoads[i].ZAxis_ZComponent;
            if ( !(i % 100)) Application->ProcessMessages();
            }

        //residual moment
        if (R[1].Length) {
            UnitInterfaceMomentSum.XAxis_XComponent = R[1][R[1].Length - 1];
            UnitInterfaceMomentSum.XAxis_YComponent = R[2][R[2].Length - 1];
            UnitInterfaceMomentSum.XAxis_ZComponent = R[3][R[3].Length - 1];
            UnitInterfaceMomentSum.YAxis_XComponent = R[4][R[4].Length - 1];
            UnitInterfaceMomentSum.YAxis_YComponent = R[5][R[5].Length - 1];
            UnitInterfaceMomentSum.YAxis_ZComponent = R[6][R[6].Length - 1];
            UnitInterfaceMomentSum.ZAxis_XComponent = R[7][R[7].Length - 1];
            UnitInterfaceMomentSum.ZAxis_YComponent = R[8][R[8].Length - 1];
            UnitInterfaceMomentSum.ZAxis_ZComponent = R[9][R[9].Length - 1];
            } else {
            UnitInterfaceMomentSum.XAxis_XComponent = 0.0;
            UnitInterfaceMomentSum.XAxis_YComponent = 0.0;
            UnitInterfaceMomentSum.XAxis_ZComponent = 0.0;
            UnitInterfaceMomentSum.YAxis_XComponent = 0.0;
            UnitInterfaceMomentSum.YAxis_YComponent = 0.0;
            UnitInterfaceMomentSum.YAxis_ZComponent = 0.0;
            UnitInterfaceMomentSum.ZAxis_XComponent = 0.0;
            UnitInterfaceMomentSum.ZAxis_YComponent = 0.0;
            UnitInterfaceMomentSum.ZAxis_ZComponent = 0.0;
            }


        for (int n = 0; n < 10; n++) {
            R[n].Length = 0;
            }
        R.Length = 0;

        BoltStiffnesses.Length = numberofbolts;
        for (int n = 1; n < q + 1; n++) {
            BoltStiffnesses[n-1].XAxis_YComponent = A[1][n];
            BoltStiffnesses[n-1].XAxis_ZComponent = A[2][n];
            BoltStiffnesses[n-1].YAxis_XComponent = A[3][n];
            BoltStiffnesses[n-1].YAxis_ZComponent = A[4][n];
            BoltStiffnesses[n-1].ZAxis_XComponent = A[5][n];
            BoltStiffnesses[n-1].ZAxis_YComponent = A[6][n];
            }
        BoltStiffnessesSum.XAxis_YComponent = A[1][collector20];
        BoltStiffnessesSum.XAxis_ZComponent = A[2][collector20];
        BoltStiffnessesSum.YAxis_XComponent = A[3][collector20];
        BoltStiffnessesSum.YAxis_ZComponent = A[4][collector20];
        BoltStiffnessesSum.ZAxis_XComponent = A[5][collector20];
        BoltStiffnessesSum.ZAxis_YComponent = A[6][collector20];
        for (int n = 0; n < 7; n++) {
            A[n].Length = 0;
            }
        A.Length = 0;

        MeanPositions.XAxis_YComponent = B[1];
        MeanPositions.XAxis_ZComponent = B[2];
        MeanPositions.YAxis_XComponent = B[3];
        MeanPositions.YAxis_ZComponent = B[4];
        MeanPositions.ZAxis_XComponent = B[5];
        MeanPositions.ZAxis_YComponent = B[6];
        B.Length = 0;

        BoltCentroidRadii.Length = numberofbolts;
        for (int n = 1; n < q + 1; n++) {
            BoltCentroidRadii[n-1].XAxis_YComponent = C[1][n];
            BoltCentroidRadii[n-1].XAxis_ZComponent = C[2][n];
            BoltCentroidRadii[n-1].YAxis_XComponent = C[3][n];
            BoltCentroidRadii[n-1].YAxis_ZComponent = C[4][n];
            BoltCentroidRadii[n-1].ZAxis_XComponent = C[5][n];
            BoltCentroidRadii[n-1].ZAxis_YComponent = C[6][n];
            }
        for (int n = 0; n < 7; n++) {
            C[n].Length = 0;
            }
        C.Length = 0;

        BoltCentroidRSquared.Length = numberofbolts;
        for (int n = 1; n < q + 1; n++) {
            BoltCentroidRSquared[n-1].XAxis_YComponent = d[2][n];
            BoltCentroidRSquared[n-1].XAxis_ZComponent = d[1][n];
            BoltCentroidRSquared[n-1].YAxis_XComponent = d[4][n];
            BoltCentroidRSquared[n-1].YAxis_ZComponent = d[3][n];
            BoltCentroidRSquared[n-1].ZAxis_XComponent = d[6][n];
            BoltCentroidRSquared[n-1].ZAxis_YComponent = d[5][n];
            }
        for (int n = 0; n < 7; n++) {
            d[n].Length = 0;
            }
        d.Length = 0;

        MomentRSquared.x = e[1];
        MomentRSquared.y = e[2];
        MomentRSquared.z = e[3];
        e.Length = 0;

        g.Length = 0;
        h.Length = 0;
        for (int n = 0; n < f.Length; n++) {
            f[n].Length = 0;
            }
        f.Length = 0;
        for (int n = 0; n < m.Length; n++) {
            m[n].Length = 0;
            }
        m.Length = 0;
        B.Length = 0;
        x.Length = 0;
        y.Length = 0;
        z.Length = 0;


        //now to start writing the output
        AnsiString temp;

        OutputAdd("");
        temp = "Analysis Results";
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("");
        OutputAdd("Analysis Title = " + AnalysisTitle);
        OutputAdd("");
        OutputAdd("");
        temp = "Positive Directions are:-";
        OutputAdd(temp);
        OutputAdd("\t+X Aft");
        OutputAdd("\t+Y Starboard");
        OutputAdd("\t+Z Up");
        OutputAdd("");
        OutputAdd("");
        temp = "Position of Applied Load";
        OutputAdd(temp);
        OutputAdd("\tX = " + FixedLen(12,AppliedLoad.x,ffFixed,10,3));
        OutputAdd("\tY = " + FixedLen(12,AppliedLoad.y,ffFixed,10,3));
        OutputAdd("\tZ = " + FixedLen(12,AppliedLoad.z,ffFixed,10,3));
        OutputAdd("");
        OutputAdd("");
        temp = "Loads Due to Unit Applied Load in each Direction at X" + FixedLen(10,AppliedLoad.x,ffFixed,10,3) + ", Y" + FixedLen(10,AppliedLoad.y,ffFixed,10,3) + ", Z" + FixedLen(10,AppliedLoad.z,ffFixed,10,3);
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("Attachment     Aft (+X)      Starboard (+Y)             Up (+Z)");
        OutputAdd("");
        for (int i = 0; i < numberofbolts; i++) {
            temp = FixedLen(5,AnsiString(i+1) + "x") + FixedLen(18,UnitInterfaceLoads[i].XAxis_XComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoads[i].YAxis_XComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoads[i].ZAxis_XComponent,ffFixed,10,4);
            OutputAdd(temp);
            temp = FixedLen(5,"y") + FixedLen(18,UnitInterfaceLoads[i].XAxis_YComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoads[i].YAxis_YComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoads[i].ZAxis_YComponent,ffFixed,10,4);
            OutputAdd(temp);
            temp = FixedLen(5,"z") + FixedLen(18,UnitInterfaceLoads[i].XAxis_ZComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoads[i].YAxis_ZComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoads[i].ZAxis_ZComponent,ffFixed,10,4);
            OutputAdd(temp);
            if ( !(i % 100)) Application->ProcessMessages();
            }
        OutputAdd("");
        OutputAdd("Sum");
        temp = FixedLen(5,"x") + FixedLen(18,UnitInterfaceLoadsSum.XAxis_XComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoadsSum.YAxis_XComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoadsSum.ZAxis_XComponent,ffFixed,10,4);
        OutputAdd(temp);
        temp = FixedLen(5,"y") + FixedLen(18,UnitInterfaceLoadsSum.XAxis_YComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoadsSum.YAxis_YComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoadsSum.ZAxis_YComponent,ffFixed,10,4);
        OutputAdd(temp);
        temp = FixedLen(5,"z") + FixedLen(18,UnitInterfaceLoadsSum.XAxis_ZComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoadsSum.YAxis_ZComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceLoadsSum.ZAxis_ZComponent,ffFixed,10,4);
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("");
        temp = "Residual Rotational Moments";
        OutputAdd(temp);
        if (numberofloops > 99) {
            temp = "Moment Balance Convergence was slow, there were " + AnsiString(numberofloops) + " balance iterations.";
            OutputAdd(temp);
            }
        OutputAdd("               Residual Moment due to load in each Direction");
        OutputAdd("About          Aft (+X)      Starboard (+Y)             Up (+Z)");
        OutputAdd("X Axis" + FixedLen(17,UnitInterfaceMomentSum.XAxis_XComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceMomentSum.YAxis_XComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceMomentSum.ZAxis_XComponent,ffFixed,10,4));
        OutputAdd("Y Axis" + FixedLen(17,UnitInterfaceMomentSum.XAxis_YComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceMomentSum.YAxis_YComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceMomentSum.ZAxis_YComponent,ffFixed,10,4));
        OutputAdd("Z Axis" + FixedLen(17,UnitInterfaceMomentSum.XAxis_ZComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceMomentSum.YAxis_ZComponent,ffFixed,10,4) + "  " + FixedLen(18,UnitInterfaceMomentSum.ZAxis_ZComponent,ffFixed,10,4));
        OutputAdd("");
        OutputAdd("");
        temp = "Attachment Positions (X, Y & Z) and Stiffnesses in each Axis (fx, fy & fz)";
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("Attachment       X           Y           Z           fx          fy          fz");
        OutputAdd("");
        for (int i = 0; i < numberofbolts; i++) {
            temp = FixedLen(4,AnsiString(i+1)) + "      " + FixedLen(12,BoltPosition[i].x,ffFixed,10,3) + FixedLen(12,BoltPosition[i].y,ffFixed,10,3) + FixedLen(12,BoltPosition[i].z,ffFixed,10,3);
            temp += FixedLen(12,BoltPosition[i].fx,ffFixed,10,4) + FixedLen(12,BoltPosition[i].fy,ffFixed,10,4) + FixedLen(12,BoltPosition[i].fz,ffFixed,10,4);
            OutputAdd(temp);
            }
        OutputAdd("");
        OutputAdd("");
        bool allboltsundefined = true;
        for (int i = 0; i < numberofbolts; i++) {
            if (BoltPosition[i].orientation == tboXTensionYZShear || BoltPosition[i].orientation == tboYTensionXZShear || BoltPosition[i].orientation == tboZTensionXYShear) allboltsundefined = false;
            }
        if (!allboltsundefined) {
            temp = "Definition of Bolt Axial Direction and Shear Plane";
            OutputAdd(temp);
            OutputAdd("");
            OutputAdd("Attachment      Axial Direction   Shear Plane");
            OutputAdd("");
            for (int i = 0; i < numberofbolts; i++) {
                temp = FixedLen(4,AnsiString(i+1)) + "            ";
                switch (BoltPosition[i].orientation) {
                    case tboXTensionYZShear:    {
                                                temp += "  X Axis          YZ Plane";
                                                }
                                                break;
                    case tboYTensionXZShear:    {
                                                temp += "  Y Axis          XZ Plane";
                                                }
                                                break;
                    case tboZTensionXYShear:    {
                                                temp += "  Z Axis          XY Plane";
                                                }
                                                break;
                    default:                    {
                                                temp += "  Not Defined     Not Defined";
                                                }
                                                break;
                    }
                OutputAdd(temp);
                }
            OutputAdd("");
            OutputAdd("");
            }
        temp = "Calculation of Attachment CG's in each plane";
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("Attachment      fx(y)       fx(z)       fy(x)       fy(z)       fz(x)       fz(y)");
        OutputAdd("");
        for (int i = 0; i < numberofbolts; i++) {
            temp = FixedLen(4,AnsiString(i+1)) + "      " + FixedLen(12,BoltStiffnesses[i].XAxis_YComponent,ffFixed,10,3) + FixedLen(12,BoltStiffnesses[i].XAxis_ZComponent,ffFixed,10,3) + FixedLen(12,BoltStiffnesses[i].YAxis_XComponent,ffFixed,10,3);
            temp += FixedLen(12,BoltStiffnesses[i].YAxis_ZComponent,ffFixed,10,3) + FixedLen(12,BoltStiffnesses[i].ZAxis_XComponent,ffFixed,10,3) + FixedLen(12,BoltStiffnesses[i].ZAxis_YComponent,ffFixed,10,3);
            OutputAdd(temp);
            }
        temp = " Sum      " + FixedLen(12,BoltStiffnessesSum.XAxis_YComponent,ffFixed,10,3) + FixedLen(12,BoltStiffnessesSum.XAxis_ZComponent,ffFixed,10,3) + FixedLen(12,BoltStiffnessesSum.YAxis_XComponent,ffFixed,10,3);
        temp += FixedLen(12,BoltStiffnessesSum.YAxis_ZComponent,ffFixed,10,3) + FixedLen(12,BoltStiffnessesSum.ZAxis_XComponent,ffFixed,10,3) + FixedLen(12,BoltStiffnessesSum.ZAxis_YComponent,ffFixed,10,3);
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("For Mx in yz-plane:       For Mz in xy-plane:       For My in xz-plane:");
        OutputAdd("");
        OutputAdd("zbar = " + FixedLen(16,MeanPositions.XAxis_ZComponent,ffFixed,10,3) + "   xbar = " + FixedLen(16,MeanPositions.ZAxis_XComponent,ffFixed,10,3) + "   xbar = " + FixedLen(16,MeanPositions.YAxis_XComponent,ffFixed,10,3));
        OutputAdd("ybar = " + FixedLen(16,MeanPositions.XAxis_YComponent,ffFixed,10,3) + "   ybar = " + FixedLen(16,MeanPositions.ZAxis_YComponent,ffFixed,10,3) + "   zbar = " + FixedLen(16,MeanPositions.YAxis_ZComponent,ffFixed,10,3));
        OutputAdd("");
        OutputAdd("");
        temp = "Calculation of Distances from Attachment CG's in each plane";
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("                      Mx                      My                      Mz");
        OutputAdd("Attachment    (y-ybar)    (z-zbar)    (x-xbar)    (z-zbar)    (x-xbar)    (y-ybar)");
        OutputAdd("");
        for (int i = 0; i < numberofbolts; i++) {
            temp = FixedLen(4,AnsiString(i+1)) + "      " + FixedLen(12,BoltCentroidRadii[i].XAxis_YComponent,ffFixed,10,3) + FixedLen(12,BoltCentroidRadii[i].XAxis_ZComponent,ffFixed,10,3) + FixedLen(12,BoltCentroidRadii[i].YAxis_XComponent,ffFixed,10,3);
            temp += FixedLen(12,BoltCentroidRadii[i].YAxis_ZComponent,ffFixed,10,3) + FixedLen(12,BoltCentroidRadii[i].ZAxis_XComponent,ffFixed,10,3) + FixedLen(12,BoltCentroidRadii[i].ZAxis_YComponent,ffFixed,10,3);
            OutputAdd(temp);
            if ( !(i % 100)) Application->ProcessMessages();
            }
        OutputAdd("");
        OutputAdd("");
        temp = "Calculation of Components of r^2 for each plane";
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("                      Mx                      My                      Mz");
        OutputAdd("Attachment fy(z-zbr)^2 fz(y-ybr)^2 fx(z-zbr)^2 fz(x-xbr)^2 fx(y-ybr)^2 fy(x-xbr)^2");
        OutputAdd("");
        for (int i = 0; i < numberofbolts; i++) {
            temp = FixedLen(4,AnsiString(i+1)) + "      " + FixedLen(12,BoltCentroidRSquared[i].XAxis_YComponent,ffFixed,10,3) + FixedLen(12,BoltCentroidRSquared[i].XAxis_ZComponent,ffFixed,10,3) + FixedLen(12,BoltCentroidRSquared[i].YAxis_XComponent,ffFixed,10,3);
            temp += FixedLen(12,BoltCentroidRSquared[i].YAxis_ZComponent,ffFixed,10,3) + FixedLen(12,BoltCentroidRSquared[i].ZAxis_XComponent,ffFixed,10,3) + FixedLen(12,BoltCentroidRSquared[i].ZAxis_YComponent,ffFixed,10,3);
            OutputAdd(temp);
            }
        temp = " Sum      " + FixedLen(18,MomentRSquared.x,ffFixed,10,3) + FixedLen(24,MomentRSquared.y,ffFixed,10,3) + FixedLen(24,MomentRSquared.z,ffFixed,10,3);
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("");
        OutputAdd("");
        OutputAdd("");


        printf("Unit Loads Calculated\n");

        //destroy local arrays
        BoltStiffnesses.Length = 0;
        BoltCentroidRadii.Length = 0;
        BoltCentroidRSquared.Length = 0;
        UnitInterfaceMoment.Length = 0;

        UnitLoadsCalculated = true;
        } else {
        if (loadpresent) {
            printf("Not Enough Bolts\n");
            } else {
            printf("No Applied Load\n");
            }
        }
    }

__finally {
    fBusy = false;
    }
}
//---------------------------------------------------------------------------

void CalculateActualLoads(void)
{
try {
    // Prints out the actual bolt loads.
    ActualLoadsCalculated = false;
    fBusy = true;
    if (UnitLoadsCalculated) {
        printf("Calculating Actual Loads\n");
        ActualInterfaceLoads.Length = numberofbolts;

        for (int i = 0; i < numberofbolts; i++) {
            ActualInterfaceLoads[i].x = UnitInterfaceLoads[i].XAxis_XComponent * AppliedLoad.fx + UnitInterfaceLoads[i].YAxis_XComponent * AppliedLoad.fy + UnitInterfaceLoads[i].ZAxis_XComponent * AppliedLoad.fz;
            ActualInterfaceLoads[i].y = UnitInterfaceLoads[i].XAxis_YComponent * AppliedLoad.fx + UnitInterfaceLoads[i].YAxis_YComponent * AppliedLoad.fy + UnitInterfaceLoads[i].ZAxis_YComponent * AppliedLoad.fz;
            ActualInterfaceLoads[i].z = UnitInterfaceLoads[i].XAxis_ZComponent * AppliedLoad.fx + UnitInterfaceLoads[i].YAxis_ZComponent * AppliedLoad.fy + UnitInterfaceLoads[i].ZAxis_ZComponent * AppliedLoad.fz;
            ActualInterfaceLoads[i].XYZResultant = ActualInterfaceLoads[i].x * ActualInterfaceLoads[i].x + ActualInterfaceLoads[i].y * ActualInterfaceLoads[i].y + ActualInterfaceLoads[i].z * ActualInterfaceLoads[i].z;
            if (ActualInterfaceLoads[i].XYZResultant < 0.0) {
                ActualInterfaceLoads[i].XYZResultant = 0.0;
                } else {
                ActualInterfaceLoads[i].XYZResultant = sqrt(ActualInterfaceLoads[i].XYZResultant);
                }
            ActualInterfaceLoads[i].XYPlane = ActualInterfaceLoads[i].x * ActualInterfaceLoads[i].x + ActualInterfaceLoads[i].y * ActualInterfaceLoads[i].y;
            if (ActualInterfaceLoads[i].XYPlane < 0.0) {
                ActualInterfaceLoads[i].XYPlane = 0.0;
                } else {
                ActualInterfaceLoads[i].XYPlane = sqrt(ActualInterfaceLoads[i].XYPlane);
                }
            ActualInterfaceLoads[i].XZPlane = ActualInterfaceLoads[i].x * ActualInterfaceLoads[i].x + ActualInterfaceLoads[i].z * ActualInterfaceLoads[i].z;
            if (ActualInterfaceLoads[i].XZPlane < 0.0) {
                ActualInterfaceLoads[i].XZPlane = 0.0;
                } else {
                ActualInterfaceLoads[i].XZPlane = sqrt(ActualInterfaceLoads[i].XZPlane);
                }
            ActualInterfaceLoads[i].YZPlane = ActualInterfaceLoads[i].y * ActualInterfaceLoads[i].y + ActualInterfaceLoads[i].z * ActualInterfaceLoads[i].z;
            if (ActualInterfaceLoads[i].YZPlane < 0.0) {
                ActualInterfaceLoads[i].YZPlane = 0.0;
                } else {
                ActualInterfaceLoads[i].YZPlane = sqrt(ActualInterfaceLoads[i].YZPlane);
                }
            }

        AnsiString temp;
        temp = "Load Case = " + LoadCaseTitle;
        OutputAdd(temp);
        OutputAdd("");
        temp = "Actual Applied Load at X" + FixedLen(10,AppliedLoad.x,ffFixed,10,3) + ", Y" + FixedLen(10,AppliedLoad.y,ffFixed,10,3) + ", Z" + FixedLen(10,AppliedLoad.z,ffFixed,10,3);
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("\tFX = " + FixedLen(12,AppliedLoad.fx,ffFixed,10,3));
        OutputAdd("\tFY = " + FixedLen(12,AppliedLoad.fy,ffFixed,10,3));
        OutputAdd("\tFZ = " + FixedLen(12,AppliedLoad.fz,ffFixed,10,3));
        OutputAdd("");
        OutputAdd("");
        temp = "Actual Attachment Loads";
        OutputAdd(temp);
        OutputAdd("");
        OutputAdd("Attachment          X                   Y                   Z");
        OutputAdd("");
        for (int i = 0; i < numberofbolts; i++) {
            temp = FixedLen(5,AnsiString(i+1) + " ") + FixedLen(18,ActualInterfaceLoads[i].x,ffFixed,10,4) + "  " + FixedLen(18,ActualInterfaceLoads[i].y,ffFixed,10,4) + "  " + FixedLen(18,ActualInterfaceLoads[i].z,ffFixed,10,4);
            OutputAdd(temp);
            if ( !(i % 100)) Application->ProcessMessages();
            }
        OutputAdd("");
        OutputAdd("");
        DynamicArray<int> UnresolvedBolts;
        DynamicArray<int> ResolvedBolts;
        for (int i = 0; i < numberofbolts; i++) {
            switch (BoltPosition[i].orientation) {
                case tboXTensionYZShear:
                case tboYTensionXZShear:
                case tboZTensionXYShear:    {
                                            ResolvedBolts.Length++;
                                            ResolvedBolts[ResolvedBolts.Length - 1] = i;
                                            }
                                            break;
                default:                    {
                                            UnresolvedBolts.Length++;
                                            UnresolvedBolts[UnresolvedBolts.Length - 1] = i;
                                            }
                                            break;
                }
            }

        if (UnresolvedBolts.Length) {
            //no shear axes defined for these bolts, so need to print out all
            //the results
            temp = "Resolved Loads for Attachments with Undefined Orientations";
            OutputAdd(temp);
            OutputAdd("");
            OutputAdd("Attachment   XYZ Resultant        In XY Plane         In XZ Plane         In YZ Plane");
            OutputAdd("");
            for (int i = 0; i < UnresolvedBolts.Length; i++) {
                temp = FixedLen(5,AnsiString(UnresolvedBolts[i]+1) + " ") + FixedLen(18,ActualInterfaceLoads[UnresolvedBolts[i]].XYZResultant,ffFixed,10,4) + "  "
                       + FixedLen(18,ActualInterfaceLoads[UnresolvedBolts[i]].XYPlane,ffFixed,10,4) + "  "
                       + FixedLen(18,ActualInterfaceLoads[UnresolvedBolts[i]].XZPlane,ffFixed,10,4) + "  "
                       + FixedLen(18,ActualInterfaceLoads[UnresolvedBolts[i]].YZPlane,ffFixed,10,4);
                OutputAdd(temp);
                }
            OutputAdd("");
            OutputAdd("");
            }

        if (ResolvedBolts.Length) {
            //all the axes are defined, so only need to consider printing out
            //tension and shear components for these bolts
            temp = "Resolved Loads for Attachments with Defined Orientations";
            OutputAdd(temp);
            OutputAdd("");
            OutputAdd("Attachment    Axial Load          Shear Load");
            OutputAdd("");
            for (int i = 0; i < ResolvedBolts.Length; i++) {
                temp = FixedLen(5,AnsiString(ResolvedBolts[i]+1) + " ");
                switch (BoltPosition[ResolvedBolts[i]].orientation) {
                    case tboXTensionYZShear:    {
                                                temp += FixedLen(18,ActualInterfaceLoads[ResolvedBolts[i]].x,ffFixed,10,4) + "  " + FixedLen(18,ActualInterfaceLoads[ResolvedBolts[i]].YZPlane,ffFixed,10,4);
                                                }
                                                break;
                    case tboYTensionXZShear:    {
                                                temp += FixedLen(18,ActualInterfaceLoads[ResolvedBolts[i]].y,ffFixed,10,4) + "  " + FixedLen(18,ActualInterfaceLoads[ResolvedBolts[i]].XZPlane,ffFixed,10,4);
                                                }
                                                break;
                    case tboZTensionXYShear:    {
                                                temp += FixedLen(18,ActualInterfaceLoads[ResolvedBolts[i]].z,ffFixed,10,4) + "  " + FixedLen(18,ActualInterfaceLoads[ResolvedBolts[i]].XYPlane,ffFixed,10,4);
                                                }
                                                break;
                    default:                    {
                                                temp += "Attachment Orientation Not defined";
                                                }
                                                break;
                    }
                OutputAdd(temp);
                }
            OutputAdd("");
            OutputAdd("");
            }


        UnresolvedBolts.Length = 0;
        ResolvedBolts.Length = 0;


        ActualLoadsCalculated = true;
        printf("Analysis Complete\n");
        }
    }

__finally {
    fBusy = false;
    }

}
//---------------------------------------------------------------------------

double ToDoubleDef(AnsiString Value, double DefaultVal)
{
    //equivalent to the .ToIntDef(value) function for doubles
    try {
        return Value.ToDouble();
        }
    catch (...)
        {
        return DefaultVal;
        }
}
//---------------------------------------------------------------------------

AnsiString DeleteWord(AnsiString string, AnsiString word)
{
    if (string.Length()) {
        //removes up to and including the first occurrence of word in string
        int a = string.Pos(word);
        if (a) {
            string.Delete(1,a + word.Length() - 1);
            }
        }
    return string;
}
//---------------------------------------------------------------------------

AnsiString ExtractWord(AnsiString string, AnsiString Delimiter)
{
    if (string.Length() != 0) {
        //this function extracts the next word based on the string and the delimiter
        while (string.SubString(1,1) == Delimiter) string.Delete(1,1);
        int a = string.Pos(Delimiter);
        if (a) {
            string = string.SubString(1,a - 1);
            } else {
            string = "";
            }
        }
    return string;
}
//---------------------------------------------------------------------------

AnsiString FixedLen(int Length, bool value)
{
    AnsiString string;
    if (value) {
        string = "true";
        } else {
        string = "false";
        }
    while (string.Length() < Length) string = " " + string;

    return string;
}
//---------------------------------------------------------------------------

AnsiString FixedLen(int Length, int value)
{
    //returns a fixed length string with length "Length" and value "value"
    AnsiString string = AnsiString(value);
    while (string.Length() < Length) string = " " + string;

    return string;
}
//---------------------------------------------------------------------------

AnsiString FixedLen(int Length, long double value, TFloatFormat format, int precision, int digits)
{
    //does the same as FloatToStrF, only using extra figure "Length"
    //to indicate how long the string should be. If the FloatToStrF figure returns
    //a string longer than Length, no error is flagged.
    AnsiString string = FloatToStrF(value, format, precision, digits);
    while (string.Length() < Length) string = " " + string;

    return string;
}
//---------------------------------------------------------------------------

AnsiString FixedLen(int Length, AnsiString string)
{
    //Returns the string padded out with spaces
    while (string.Length() < Length) string = " " + string;

    return string;

}
//---------------------------------------------------------------------------





