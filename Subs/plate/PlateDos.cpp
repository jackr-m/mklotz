
#include <condefs.h>
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#pragma hdrstop

/*
Program Copyright R.W.Williams (R.W.W) e-mail richardandtracy@yahoo.com

Permission is hereby granted to anyone who wants to, to use, distribute and
modify the program in any way they want free of charge for non-commercial use.
HOWEVER.... if the program is modified, the modifier MUST:-
1)  Acknowledge the R.W.W copyright material somewhere.
2)  EITHER send the modified program back to R.W.W for a cursory accuracy check
    OR state somewhere visible to the user that the program has been modified
    and the accuracy of the changes has not been verified.
3)  Not sell the modified program without discussions with R.W.W.
Commerial use must be discussed with R.W.W first.

There - those conditions aren't too onerous are they?







This program is to enable the user to develop a flat plate from a 3d boundary
of the plate.

It takes a DXF filename, then reads the file. After this, it identifies a boundary,
then allocates lines in the boundary to a "Top" and a "Bottom" Line. Also all
the explicit cubic equations are calculated. Finally it steps through the
boundary to flatten the plate, and writes out the output file.

One final note - please forgive me as this is only my second DOS console program
and it isn't as efficient as it could be! This program started as a 1.6Mb
Windows exe file with about 1.5Mb of ancilliary bits - so it is a bit smaller
than it was....  And a lot of user friendliness has been sacrificed. If you
want the full Windows program, contact me at richardandtracy@yahoo.com, but I
warn you, the installation package is 1.99 Mb fully zipped with all the
compression I can get.

I have retained a lot of the original Borland VCL of the original file to make
the "DOS-ING" of the file quicker - but at the sacrifice of a certain amount of
file size. If you have any arguements about the program - all I can say is "Show
me a better way, then". I'm entirely self taught, so I am certain to have done
some things the hard way (and I don't mind being shown a better way...).


12 May 2003
This program has been modified in the light of experiences I have had developing
the plates of the pilot house of my yacht. The real useage of the program showed
that there were a number of areas that needed improvement - so they have been
incorporated.

*/

//Structures used
typedef struct  {
    double x, y, z;
    int linenumber;
    } linestruct;

//structure to hold the extents of the model - used to get default step size
typedef struct {
    double xmax, xmin, ymax, ymin, zmax, zmin;
    } extents;

//structure to define line equations
typedef struct {
    //co-efficients for x
    //x co-efficients not used for the explicit cubic line calc.
    double A0,A1,A2,A3;
    //co-efficients for y
    double B0,B1,B2,B3;
    //co-efficients for z
    double C0,C1,C2,C3;
    //maxx and minx are included below to indicate the range of x values over
    //which the equations are applicable
    double minx, maxx;
    } line_equation;

//structure to define the lines in a boundary
typedef struct {
    int linenumber;
    bool startatbeginning;
    } boundarylines;

//structure to hold the co-ordinates of the developed 2D lines
//similar to TPoint, only for doubles not ints
typedef struct {
    double x, y;
    } point;

//2 x 2 matrix definition using row and column identifiers a and b
//first digit = column, second digit = row
typedef struct  {
    double aa, ba;
    double ab, bb;
    } TwoDMatrix;

typedef struct {
    double x,y,z;
    bool ErrorPresent;
    AnsiString ErrorMessage;
    } ErrorProneResult;

typedef struct  {
    double x, y, z;
    } nodalposition;

typedef nodalposition *NodalPosition;

//3 x 3 matrix definition using row and column identifiers a, b and c
//first digit = column, second digit = row
typedef struct  {
    double aa, ba, ca;
    double ab, bb, cb;
    double ac, bc, cc;
    } matrix;

typedef struct {
    int LineNumber;
    AnsiString Text;
    } LineSearch;



//Global variables
    TStringList * ErrorListing;
    double Accuracy;
    double LargeNumber;
    AnsiString FileExtension;
    DynamicArray< DynamicArray < nodalposition > > Vertices;
    DynamicArray< DynamicArray < nodalposition > > DisplayVertices;
    DynamicArray< nodalposition> LineSegment;
    DynamicArray<int> Lines;
    DynamicArray<nodalposition> Nodes;
    nodalposition extrusiondirection;
    double extrusiondepth;
    int numberofvertices;
    extents ModelExtents;
    extents LineExtents;
    DynamicArray< DynamicArray < line_equation > > LineEquations;
    DynamicArray< DynamicArray < boundarylines > > BoundaryLines;
    DynamicArray < boundarylines > TopLine;
    DynamicArray < boundarylines > BottomLine;
    DynamicArray <nodalposition> StartLine;
    double StartX;
    bool Stop;
    bool fBusy;
    int counter;

    DynamicArray <point> TopLineAft;
    DynamicArray <point> TopLineFwd;
    DynamicArray <point> BottomLineAft;
    DynamicArray <point> BottomLineFwd;

    AnsiString EndOfFileName;
    AnsiString FileName;

    TStringList *inputfile;
    TStringList *outputfile;



//Prototyping the functions
    matrix MatrixMultiply(matrix Matrix1, matrix Matrix2);
    nodalposition MatrixMultiply (matrix Matrix1, nodalposition Vector1);
    nodalposition UnitVector(nodalposition vector1);
    nodalposition VectorProduct(nodalposition vector1, nodalposition vector2);
    nodalposition ScaleVector(nodalposition vector1, double scalar);
    nodalposition VectorTranslate(nodalposition vector1, nodalposition vector2);

    bool ReadDXFFile(AnsiString AFileName);
    bool InterpretDXFFile(void);
    void ProcessEntity(void);
    void ReadLine(void);
    void ReadArcs(void);
    void ReadPolyline(void);

    nodalposition GetPoint(AnsiString PointNo);
    bool Extruded(void);
    double GetRadius(void);
    double GetGroup(AnsiString group);

    void GetModelExtents(void);
    void GetLineEquations(void);
    line_equation GetLineEquation(void);
    line_equation GetQuadraticLineEquation(void);
    double DistanceBetweenPoints(nodalposition node1, nodalposition node2);
    void MergeLines(void);
    void SortLineOrientation(void);
    void DeleteDuplicateLines(void);
    void DeleteZeroLengthLines(void);
    void DeleteLine(int LineNumber);
    void SplitLongLines(void);
    bool CheckContinuousBoundary(void);

    ErrorProneResult GetCoordinate(double x, int linenumber);
    ErrorProneResult GetCoord(double x, line_equation Equation);
    void CreateTopandBottomLines(void);

    ErrorProneResult GetTopLinePosition(double x);
    ErrorProneResult GetBottomLinePosition(double x);




    //Developing Plate
    void DevelopPlate(void);
    void CreateandDisplayDXFFile(void);
    point SolveTriangle(double FirstSide, double SecondSide, double ThirdSide);
    point MatrixMultiply(TwoDMatrix Matrix1, point Vector1);
    point VectorTranslate(point vector1, point vector2);
    double DistanceBetweenPoints(point node1, point node2);
    void Develop(void);

    //writing DXF file
    void WriteInitialLines(void);
    void WriteLine(point Point1, point Point2);
    void WriteFinalLines(void);


    nodalposition GetCoordinate(ErrorProneResult Result);
    void DisplayErrors(void);


    //the main function superceeds the Open method
    //void Open(const AnsiString AFileName);


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


    //from "Stringhelp"
    double ToDoubleDef(AnsiString Value, double DefaultVal);






/*


To those who wish to understand this code, I offer my wholehearted sympathy

                  ha! Ha! HA!


*/






















//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{

    //check arguements are OK.

    //set up some global variables
    ModelExtents.xmax = 0.0;
    ModelExtents.xmin = 0.0;
    ModelExtents.ymax = 0.0;
    ModelExtents.ymin = 0.0;
    ModelExtents.zmax = 0.0;
    ModelExtents.zmin = 0.0;
    LineExtents = ModelExtents;

    Accuracy = 1e-6;
    LargeNumber = 1e10;
    counter = 0;

    for (int i = 0; i < BoundaryLines.Length; i++) {
        BoundaryLines[i].Length = 0;
        }
    BoundaryLines.Length = 0;

    ErrorListing = new TStringList();
    inputfile = new TStringList();
    outputfile = new TStringList();

    FileExtension = ".dxf";
    EndOfFileName = " Developed Plate.dxf";

    fBusy = false;


    //checks to see whether it's being used correctly
	if (argc < 2) {
		fprintf(stderr,"Usage: %s DXFfilename\n",argv[0]);
		exit(-1);
	    }


    FileName = AnsiString(argv[1]);

    //does the opening of the files.
  try {
        if (ExtractFileExt(FileName).LowerCase() != ".dxf") {
		    fprintf(stderr,"Usage: %s DXFfilename\n",argv[0]);
		    exit(-1);
            }


        for (int i = 0; i < Vertices.Length; i++) {
            Vertices[i].Length = 0;
            }
        Vertices.Length = 0;

        TopLine.Length = 0;
        BottomLine.Length = 0;

        for (int i = 0; i < BoundaryLines.Length; i++) {
            BoundaryLines[i].Length = 0;
            }
        BoundaryLines.Length = 0;

        StartLine.Length = 0;

        if (FileExists(FileName)) {
            if (ExtractFileExt(FileName).LowerCase() == ".dxf") {
                printf("Loading DXF File\n");

                //put dxf file read here
                if (ReadDXFFile(FileName)) {
                    //now to check the lines for continuity etc
                    if (InterpretDXFFile()) {
                        //lines with zero length can cause problems
                        DeleteZeroLengthLines();

                        //merge ends of lines
                        MergeLines();

                        //sort all lines into order so that vertices with minimum
                        //x value are to the lower end of the array, ie
                        //vertices[i][0].x < vertices[i][vertices[i].Length - 1].x
                        SortLineOrientation();

                        //delete duplicate lines
                        DeleteDuplicateLines();

                        //get the extents of the model
                        GetModelExtents();

                        //make sure that there are no lines reaching from xmin to xmax
                        SplitLongLines();

                        //check to see that boundary is closed.
                        //and delete all lines not in the boundary
                        if (CheckContinuousBoundary()) {
                            //get the extents of the model
                            GetModelExtents();

                            //get the equations of all the lines in cubic parametric form
                            //  x
                            //  y = B0 + B1*x + B2*x^2 + B3*x^3
                            //  z = C0 + C1*x + C2*x^2 + C3*x^3
                            //
                            GetLineEquations();

                            //decide on top line and bottom line of plate
                            CreateTopandBottomLines();

                            //decide on x co-ordinate of start line
                            StartLine.Length = 2;
                            StartX = (LineExtents.xmax + LineExtents.xmin)/2;
                            ErrorProneResult Result;
                            Result = GetTopLinePosition(StartX);
                            if (Result.ErrorPresent) {
                                ErrorListing->Add("Error finding Top End of Start Line");
                                ErrorListing->Add("Error Message = " + Result.ErrorMessage);
                                }
                            StartLine[0] = GetCoordinate(Result);
                            Result = GetBottomLinePosition(StartX);
                            if (Result.ErrorPresent) {
                                ErrorListing->Add("Error finding Bottom End of Start Line");
                                ErrorListing->Add("Error Message = " + Result.ErrorMessage);
                                }
                            StartLine[1] = GetCoordinate(Result);

                            Develop();

                            } else {
                            printf("No continuous boundary\n");
                            }
                        }
                    } else {
                    printf("DXF File Load failed\n");
                    }
                }
            }
        }

    __finally {
        DisplayErrors();
        if (!fBusy) {
            //tidy up before exiting
            printf("Clearing Memory\n");
            for (int i = 0; i < Vertices.Length; i++) {
                Vertices[i].Length = 0;
                }
            Vertices.Length = 0;
            Lines.Length = 0;
            Nodes.Length = 0;
            for (int i = 0; i < LineEquations.Length; i++) {
                LineEquations[i].Length = 0;
                }
            LineEquations.Length = 0;
            for (int i = 0; i < BoundaryLines.Length; i++) {
                BoundaryLines[i].Length = 0;
                }
            BoundaryLines.Length = 0;
            TopLine.Length = 0;
            BottomLine.Length = 0;
            StartLine.Length = 0;
            TopLineAft.Length = 0;
            TopLineFwd.Length = 0;
            BottomLineAft.Length = 0;
            BottomLineFwd.Length = 0;
            ErrorListing->Clear();

            /*

            Now will the last one out, please turn off the lights.
        
            */
        
            }
        delete ErrorListing;
        delete inputfile;
        delete outputfile;
        }



        printf("Finished\n");


        return 0;
}

//---------------------------------------------------------------------------























//now all the functions
//---------------------------------------------------------------------------

matrix MatrixMultiply(matrix Matrix1, matrix Matrix2)
{
    matrix combined;

    //first column
    combined.aa = Matrix1.aa * Matrix2.aa + Matrix1.ba * Matrix2.ab + Matrix1.ca * Matrix2.ac;
    combined.ab = Matrix1.ab * Matrix2.aa + Matrix1.bb * Matrix2.ab + Matrix1.cb * Matrix2.ac;
    combined.ac = Matrix1.ac * Matrix2.aa + Matrix1.bc * Matrix2.ab + Matrix1.cc * Matrix2.ac;
    //second column
    combined.ba = Matrix1.aa * Matrix2.ba + Matrix1.ba * Matrix2.bb + Matrix1.ca * Matrix2.bc;
    combined.bb = Matrix1.ab * Matrix2.ba + Matrix1.bb * Matrix2.bb + Matrix1.cb * Matrix2.bc;
    combined.bc = Matrix1.ac * Matrix2.ba + Matrix1.bc * Matrix2.bb + Matrix1.cc * Matrix2.bc;
    //third column
    combined.ca = Matrix1.aa * Matrix2.ca + Matrix1.ba * Matrix2.cb + Matrix1.ca * Matrix2.cc;
    combined.cb = Matrix1.ab * Matrix2.ca + Matrix1.bb * Matrix2.cb + Matrix1.cb * Matrix2.cc;
    combined.cc = Matrix1.ac * Matrix2.ca + Matrix1.bc * Matrix2.cb + Matrix1.cc * Matrix2.cc;

    return combined;
}
//---------------------------------------------------------------------------

nodalposition MatrixMultiply (matrix Matrix1, nodalposition Vector1)
{
    nodalposition combined;

    combined.x = Matrix1.aa * Vector1.x + Matrix1.ba * Vector1.y + Matrix1.ca * Vector1.z;
    combined.y = Matrix1.ab * Vector1.x + Matrix1.bb * Vector1.y + Matrix1.cb * Vector1.z;
    combined.z = Matrix1.ac * Vector1.x + Matrix1.bc * Vector1.y + Matrix1.cc * Vector1.z;


    return combined;

}
//---------------------------------------------------------------------------

nodalposition UnitVector(nodalposition vector1)
{
    //unitises the vector
    double length = pow(vector1.x,2.0) + pow(vector1.y,2.0) + pow(vector1.z,2.0);
    length = sqrt(length);

    if (length == 0.0) {
        vector1.x = 1.0;
        vector1.y = 1.0;
        vector1.z = 1.0;
        } else {
        vector1.x = vector1.x / length;
        vector1.y = vector1.y / length;
        vector1.z = vector1.z / length;
        }

    return vector1;
}
//---------------------------------------------------------------------------


nodalposition VectorProduct(nodalposition vector1, nodalposition vector2)
{
    //returns the vector product of two 3D vectors
    nodalposition value;

    value.x = vector1.y * vector2.z - vector1.z * vector2.y;
    value.y = vector1.z * vector2.x - vector1.x * vector2.z;
    value.z = vector1.x * vector2.y - vector1.y * vector2.x;

    return value;

}
//---------------------------------------------------------------------------

nodalposition ScaleVector(nodalposition vector1,  double scalar)
{
    //returns the scaled vector

    vector1.x = vector1.x * scalar;
    vector1.y = vector1.y * scalar;
    vector1.z = vector1.z * scalar;

    return vector1;

}
//---------------------------------------------------------------------------

bool ReadDXFFile(AnsiString AFileName)
{
    //this method reads DXF files. It is somewhat unstable with long files,
    //hence the try catch business. Don't expect long files.
    bool suceeded = true;

    try {
        //read the file
        inputfile->LoadFromFile(AFileName);
        }

    catch (...) {
        suceeded = false;
        }

    return suceeded;

}
//---------------------------------------------------------------------------

bool InterpretDXFFile(void)
{
    //This method interprets the DXF file loaded. A large part of this has been
    //cribbed from the EPS LTL NISATools program (DXFtoNEU translator).
    bool suceeded = true;

    try {
        //get rid of all comments
        counter = 0;
        while (counter < InputLength() - 1) {
            if (InputString(counter) == "999") {
                InputDelete(counter);
                InputDelete(counter);
                counter -= 2;
                }
            counter += 2;
            }

        //now look to see if there is anything in the body of the DXF file
        //NOTE:- All blocks will be discarded SO THERE!
        LineSearch Search = InputFind("ENTITIES");

        if (Search.LineNumber) {
            //delete everything up to & including "Entities"
            for (int i = 0; i < Search.LineNumber + 1; i++) {
                InputDelete(0);
                }

            //now to go through the input file copying entities to the output
            //file for processing.
            int firstline = 0;
            //the last line is intended to point to the line after the entity
            //finishes
            int lastline;
            while (firstline < InputLength()){
                OutputClear();
                numberofvertices = 0;
                lastline = firstline + 1;
                if (firstline + 1 < InputLength() && InputSearch(firstline + 1,"POLYLINE")) {
                    //special case for polylines which also have vertices
                    while (lastline < InputLength() && !InputSearch(lastline,"SEQEND")) {
                        if (InputSearch(lastline,"VERTEX")) numberofvertices++;
                        lastline++ ;
                        }
                    lastline++;
                    } else {
                    lastline++;
                    while (lastline < InputLength() && InputString(lastline).Trim() != "0") {
                        lastline += 2;
                        }
                    }
                //copy all the relavent entity lines to the output file
                for (int i = firstline; i < lastline; i++) {
                    OutputAdd(InputString(i));
                    }
                //add a zero to terminate the entity - internal use only
                OutputAdd(" 0");

                //now to extract all the relavent data from the entity.
                ProcessEntity();
                firstline = lastline;
                }
            } else {
            printf("No Entities in DXF File\n");
            suceeded = false;
            }
        }

    catch (...) {
        printf("No Entities Read from DXF File\n");
        suceeded = false;
        }

    return suceeded;

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

void ProcessEntity(void)
{
    //this method acts as a redirection house for determining what to do when
    //extracting  data from an entity.
    counter = 0;
    AnsiString temp;

    while (counter < OutputLength()) {
        if (!(counter % 2)) {
            temp = OutputString(counter).Trim();
            if (temp == "0") {
                temp = OutputString(counter + 1).Trim();
                if (temp == "LINE") ReadLine();
                if (temp == "ARC") ReadArcs();
                if (temp == "POLYLINE") ReadPolyline();
                }
            }

        counter++;
        }

}
//---------------------------------------------------------------------------

void ReadLine(void)
{
    //reads a line
    //getting the points at the ends of the line
    nodalposition Node;
    Vertices.Length++;
    Vertices[Vertices.Length - 1].Length = 2;

    for (int i = 0; i < 2; i++) {
        Node = GetPoint(AnsiString(i));
        Vertices[Vertices.Length - 1][i].x = Node.x;
        Vertices[Vertices.Length - 1][i].y = Node.y;
        Vertices[Vertices.Length - 1][i].z = Node.z;
        }


}
//---------------------------------------------------------------------------

void ReadArcs(void)
{
    //there are lots of comments with this one as I'm trying to work it out as I
    //go along, and it ain't easy!

    //reads an arc
    //all arcs can be extruded, as it defines the 3D orientation of the arc
    Extruded();
    //the extrusiondepth value defines the extrusion depth - which isn't used.

    //finds the centre of the circle of which the arc is a segment
    nodalposition centre = GetPoint("0");

    //now the radius
    double radius = GetRadius();
    double startangle = GetGroup("50");
    double endangle = GetGroup("51");

    while (endangle < startangle) endangle += 360;

    //now are in a position to set up arrays etc;
    //firsly get the local X and local Y axes, assuming the extrusion direction is the
    //local z axis

    //it's important that all the vectors are unit size, and the only one that may
    //not be is the extrusion direction - better be safe
    extrusiondirection = UnitVector(extrusiondirection);

    //the way to do this is by using the vector product twice
    //first set up the initial values
    nodalposition localx, localy;

    //Create the axes based around the normal taking extrusion direction as
    //the local z axis, then making up the columns of the transformation matrix
    //as being localx, localy and local z.
    localy.x = 0.0;
    localy.y = 1.0;
    localy.z = 0.0;
    //vector products this way round to get the normals right.
    localx = VectorProduct(localy, extrusiondirection);
    localy = VectorProduct(extrusiondirection, localx);
    matrix transformation;
    transformation.aa = localx.x;
    transformation.ab = localx.y;
    transformation.ac = localx.z;
    transformation.ba = localy.x;
    transformation.bb = localy.y;
    transformation.bc = localy.z;
    transformation.ca = extrusiondirection.x;
    transformation.cb = extrusiondirection.y;
    transformation.cc = extrusiondirection.z;

    //right, got all the ancilliary bits set up, now to begin to operate on the arc
    //first set up all the node positions for an arc on the unit circle

    //to begin with, sort out the number of arcs necessary, want to limit each
    //arc to less than 45 degrees.
    double arclength = endangle - startangle;
    int numberofarcs = ((int)ceil(fabs(arclength)/45.0))*3;
    Nodes.Length = numberofarcs + 1;
    double arcangle = arclength/numberofarcs * M_PI / 180;
    //convert startangle and endangle to radians
    startangle = startangle * M_PI / 180;
    //endangle = endangle * M_PI / 180;

    for (int i = 0; i < Nodes.Length; i++) {
        Nodes[i].x = radius * cos(startangle + i * arcangle) ;
        Nodes[i].y = radius * sin(startangle + i * arcangle) ;
        Nodes[i].z = 0.0;
        }

    //now we've got all the control points set up, it's time to apply the
    //transformation and the translation to all the points

    for (int i = 0; i < Nodes.Length; i++) {
        Nodes[i] = MatrixMultiply(transformation,Nodes[i]);
        Nodes[i] = VectorTranslate(Nodes[i],centre);
        }

    //right, now got the whole lot in the correct place in the right orientation
    //now to start creating the relavent lines.
    for (int i = 0; i < Nodes.Length - 1; i += 3) {
        Vertices.Length++;
        Vertices[Vertices.Length - 1].Length = 4;
        Vertices[Vertices.Length - 1][0] = Nodes[i];
        Vertices[Vertices.Length - 1][1] = Nodes[i + 1];
        Vertices[Vertices.Length - 1][2] = Nodes[i + 2];
        Vertices[Vertices.Length - 1][3] = Nodes[i + 3];
        }

    //nodes not needed any more
    Nodes.Length = 0;


}
//---------------------------------------------------------------------------

void ReadPolyline(void)
{
    //Read in a Polyline
    //AND THROW A WOBBLY IF IT'S NOT A CUBIC LINE.
    //BE WARNED!

    //bool polylineerror = false;
    //static bool erroralready = false;
    //if (!Layer()) return;

    //check to se if there are enough vertices to do anything with
    if (numberofvertices < 2) return;

    //polylines are probably the hardest one to get sorted

    //check if extruded. Stop converting if it is.
    if (Extruded()) return;


    //now check for all the possible polyline data
    //elevation data
    nodalposition elevation = GetPoint("0");
    //Polyline flag
    //this is bit coded, so only needs a short int
    unsigned short polylineflag = (unsigned short)GetGroup("70");
    //starting and ending widths (40 & 41) are ignored
    //M vertex count
    int mvertexcount = (int)GetGroup("71");
    //n vertex count
    int nvertexcount = (int)GetGroup("72");
    //There should be NO n direction vertices, so return if there are.
    if (nvertexcount > 1) return;

    //smooth surface density (73 & 74) are ignored
    //curves & smooth surface type (75) is ignored as cubic is assumed

    //now to check array size parameters are OK
    if (nvertexcount == 0) nvertexcount = 1;
    if (mvertexcount == 0) mvertexcount = numberofvertices/nvertexcount;

    //check if there is an error in the number of vertices
    if (mvertexcount * nvertexcount != numberofvertices) return;


    //this to make sure that the line is in the correct orientation
    if (mvertexcount == 1 && nvertexcount > mvertexcount) {
        mvertexcount = nvertexcount;
        nvertexcount = 1;
        }
    //set up vertex array dimensions n x m
    //using DynamicArrays because they're easier, safer and more flexible
    //than calloc or malloc (more difficult to trace when debugging though...)
    DynamicArray< DynamicArray <nodalposition> > nodesarray;
    nodesarray.Length = nvertexcount;
    //this line is because dynamicarrays may have different numbers of elements
    //in each row (just like lines of text can have different numbers of
    //characters on each line)
    for (int i = 0; i < nvertexcount; i++) nodesarray[i].Length = mvertexcount;

    //having got all the relavent data about the polyline,
    //now to search out the vertex data.
    for (int i = 0; i < nvertexcount; i++) {
        for (int j = 0; j < mvertexcount; j++) {
            //consider changing this next bit for a TStringList->Find operation
            //will probably be quicker
            while (OutputString(1).Trim() != "VERTEX" && OutputLength()) {
                OutputDelete(0);
                }
            nodesarray[i][j] = GetPoint("0");

            //now to add the elevation
            nodesarray[i][j].z += elevation.z;

            //continue
            OutputDelete(0);
            if (!(j%20) && j && numberofvertices > 100) {
                //this is put in as some large lines take ages.
                printf(("Point " + AnsiString(j) + " of " + AnsiString(mvertexcount) + "\n").c_str());
                }
            }
        }

    if (numberofvertices > 100) {
        printf("Converting Polyline\n");
        }

    //got all the relavent data from the dxf file
    //now to start dealing with it.

    if (polylineflag & 1) {
        //the line is closed in the m direction. However the program can't cope
        //with loops. So copy first vertex to last point in line.
        for (int i = 0; i < nodesarray.Length; i++) {
            nodesarray[i].Length++;
            nodesarray[i][nodesarray[i].Length - 1] = nodesarray[i][0];
            }
        }

    //we should now have one extra line in nodesarray - it's all structured for
    //more than 1 line just in case my traps don't work.
    //Now copy it across into the Vertices array
    if (polylineflag & 8) {
        //cubic splined curves retain their curvature
        for (int i = 0; i < nodesarray.Length; i++) {
            Vertices.Length++;
            Vertices[Vertices.Length - 1].Length = nodesarray[i].Length;
            for (int j = 0; j < nodesarray[i].Length; j++) {
                Vertices[Vertices.Length - 1][j] = nodesarray[i][j];
                }
            }
        } else {
        //all others are converted to linear between vertices
        for (int i = 0; i < nodesarray.Length; i++) {
            for (int j = 0; j < nodesarray[i].Length - 1; j++) {
                Vertices.Length++;
                Vertices[Vertices.Length - 1].Length = 2;
                Vertices[Vertices.Length - 1][0] = nodesarray[i][j];
                Vertices[Vertices.Length - 1][1] = nodesarray[i][j + 1];
                }
            }
        }

    //finishing, so free memory associated with nodesarray
    for (int i = 0; i < nodesarray.Length; i++) nodesarray[i].Length = 0;
    nodesarray.Length = 0;
    Nodes.Length = 0;
}
//---------------------------------------------------------------------------

nodalposition GetPoint(AnsiString PointNo)
{
    nodalposition node;
    node.x = 0.0;
    node.y = 0.0;
    node.z = 0.0;

    //now must find x point
    int i = 2;
    AnsiString temp = "1" + PointNo ;

    while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != temp) {
        i += 2;
        }
    if (OutputString(counter + i).Trim() == temp) node.x = (OutputString(counter + i + 1).Trim()).ToDouble();

    //now must find y point
    i = 2;
    temp = "2" + PointNo ;

    while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != temp) {
        i += 2;
        }
    if (OutputString(counter + i).Trim() == temp) node.y = (OutputString(counter + i + 1).Trim()).ToDouble();

    //now must find z point
    i = 2;
    temp = "3" + PointNo ;

    while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != temp) {
        i += 2;
        }
    if (OutputString(counter + i).Trim() == temp) node.z = (OutputString(counter + i + 1).Trim()).ToDouble();


    return node;
}
//---------------------------------------------------------------------------

bool Extruded(void)
{
    //checks to see if the entity is extruded

    //firstly setup default figures
    extrusiondirection.x = 0.0;
    extrusiondirection.y = 0.0;
    extrusiondirection.z = 1.0;
    extrusiondepth = 0.0;

    //check to see if extruded at all
    int i = 2;
    bool found = false;
    while (OutputString(counter + i).Trim() != "0" && !found) {
        if (OutputString(counter + i).Trim() == "39" || OutputString(counter + i).Trim() == "210") found = true;
        i += 2;
        }

    if (!found) {
        return false;
        }

    //now must find whether a thickness is specified
    i = 2;
    AnsiString temp = "39";

    while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != temp) {
        i += 2;
        }

    //a thickness has been found
    if (OutputString(counter + i).Trim() == temp) {
        temp = InputString(counter + i + 1).Trim();
        extrusiondepth = ToDoubleDef(temp,1.0);
        }
    i = 2;
    temp = "210";
    while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != temp) {
        i += 2;
        }
    if (OutputString(counter + i).Trim() == temp) {
        //an extrusion direction other than the world Z has been found
        //x axis
        extrusiondirection.x = ToDoubleDef(OutputString(counter + i + 1).Trim(),0.0);
        //y axis
        i = 2;
        temp = "220";
        while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != temp) {
            i += 2;
            }
        if (OutputString(counter + i).Trim() == temp) {
            extrusiondirection.y = ToDoubleDef(OutputString(counter + i + 1).Trim(),0.0);
            }
        //z axis
        i = 2;
        temp = "230";
        while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != temp) {
            i += 2;
            }
        if (OutputString(counter + i).Trim() == temp) {
            extrusiondirection.z = ToDoubleDef(OutputString(counter + i + 1).Trim(),1.0);
            }
        }


    temp.~AnsiString();

    if (extrusiondepth == 0.0) {
        return false;
        } else {
        return true;
        }

}
//---------------------------------------------------------------------------

double GetRadius(void)
{
    //gets the radius of an arc or circle
    int i = 2;
    AnsiString temp = "40";
    double radius = 0;

    while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != temp) {
        i += 2;
        }
    if (OutputString(counter + i).Trim() == temp) radius = ToDoubleDef(OutputString(counter + i + 1).Trim(),0);

    return radius;
}
//---------------------------------------------------------------------------

double GetGroup(AnsiString group)
{
    //gets the value associated with group (it's general extension of GetRadius())
    int i = 2;
    double value = 0;

    while (OutputString(counter + i).Trim() != "0" && OutputString(counter + i).Trim() != group) {
        i += 2;
        }
    if (OutputString(counter + i).Trim() == group) value = ToDoubleDef(OutputString(counter + i + 1).Trim(),0);

    return value;
}
//---------------------------------------------------------------------------

nodalposition VectorTranslate(nodalposition vector1, nodalposition vector2)
{
    //carries out vector addition
    vector1.x = vector1.x + vector2.x;
    vector1.y = vector1.y + vector2.y;
    vector1.z = vector1.z + vector2.z;

    return vector1;
}
//---------------------------------------------------------------------------

void GetModelExtents(void)
{
    //this function finds the model extents
    if (Vertices.Length) {
        ModelExtents.xmax = Vertices[0][0].x;
        ModelExtents.xmin = Vertices[0][0].x;
        ModelExtents.ymax = Vertices[0][0].y;
        ModelExtents.ymin = Vertices[0][0].y;
        ModelExtents.zmax = Vertices[0][0].z;
        ModelExtents.zmin = Vertices[0][0].z;
        LineExtents = ModelExtents;
        for (int i = 0; i < Vertices.Length; i++) {
            for (int j = 0; j < Vertices[i].Length; j++) {
                if (Vertices[i][j].x > ModelExtents.xmax) ModelExtents.xmax = Vertices[i][j].x;
                if (Vertices[i][j].x < ModelExtents.xmin) ModelExtents.xmin = Vertices[i][j].x;
                if (Vertices[i][j].y > ModelExtents.ymax) ModelExtents.ymax = Vertices[i][j].y;
                if (Vertices[i][j].y < ModelExtents.ymin) ModelExtents.ymin = Vertices[i][j].y;
                if (Vertices[i][j].z > ModelExtents.zmax) ModelExtents.zmax = Vertices[i][j].z;
                if (Vertices[i][j].z < ModelExtents.zmin) ModelExtents.zmin = Vertices[i][j].z;
                if (j == 0 || j == Vertices[i].Length - 1) {
                    if (Vertices[i][j].x > LineExtents.xmax) LineExtents.xmax = Vertices[i][j].x;
                    if (Vertices[i][j].x < LineExtents.xmin) LineExtents.xmin = Vertices[i][j].x;
                    if (Vertices[i][j].y > LineExtents.ymax) LineExtents.ymax = Vertices[i][j].y;
                    if (Vertices[i][j].y < LineExtents.ymin) LineExtents.ymin = Vertices[i][j].y;
                    if (Vertices[i][j].z > LineExtents.zmax) LineExtents.zmax = Vertices[i][j].z;
                    if (Vertices[i][j].z < LineExtents.zmin) LineExtents.zmin = Vertices[i][j].z;
                    }
                }
            }
        } else {
        ModelExtents.xmax = 0.0;
        ModelExtents.xmin = 0.0;
        ModelExtents.ymax = 0.0;
        ModelExtents.ymin = 0.0;
        ModelExtents.zmax = 0.0;
        ModelExtents.zmin = 0.0;
        LineExtents = ModelExtents;
        }

}
//---------------------------------------------------------------------------

void GetLineEquations(void)
{
    if (Vertices.Length) {
        //get the equations of all the lines in cubic parametric form
        //  x = A0 + A1*t + A2*t^2 + A3*t^3
        //  y = B0 + B1*t + B2*t^2 + B3*t^3
        //  z = C0 + C1*t + C2*t^2 + C3*t^3
        //where 0.00 <= t <= 1.00

        LineSegment.Length = 4;

        //there is one equation for each segment of line between vertices, so there
        //is one fewer equation than vertices in each line.
        LineEquations.Length = Vertices.Length;
        for (int i = 0; i < Vertices.Length; i++) {
            if (Vertices[i].Length > 1) LineEquations[i].Length = Vertices[i].Length - 1;
            }

        //now to create the equations, going through line by line
        for (int i = 0; i < Vertices.Length; i++) {
            switch (Vertices[i].Length) {
                //decide what to do depending on the number of vertices in the line
                case 0:
                case 1:     break;
                case 2:     {
                            //new explicit calculation
                            LineSegment[0] = Vertices[i][0];
                            LineSegment[1].x = Vertices[i][0].x + 0.3 * (Vertices[i][1].x - Vertices[i][0].x);
                            LineSegment[1].y = Vertices[i][0].y + 0.3 * (Vertices[i][1].y - Vertices[i][0].y);
                            LineSegment[1].z = Vertices[i][0].z + 0.3 * (Vertices[i][1].z - Vertices[i][0].z);
                            LineSegment[2].x = Vertices[i][0].x + 0.7 * (Vertices[i][1].x - Vertices[i][0].x);
                            LineSegment[2].y = Vertices[i][0].y + 0.7 * (Vertices[i][1].y - Vertices[i][0].y);
                            LineSegment[2].z = Vertices[i][0].z + 0.7 * (Vertices[i][1].z - Vertices[i][0].z);
                            LineSegment[3] = Vertices[i][1];
                            LineEquations[i][0] = GetLineEquation();
                            LineEquations[i][0].minx = Vertices[i][0].x;
                            LineEquations[i][0].maxx = Vertices[i][1].x;
                            }
                            break;
                case 3:     {
                            //explicit calculation
                            for (int j = 0; j < 3; j++) {
                                LineSegment[j] = Vertices[i][j];
                                }
                            LineEquations[i][0] = GetQuadraticLineEquation();
                            LineEquations[i][0].minx = Vertices[i][0].x;
                            LineEquations[i][0].maxx = Vertices[i][1].x;
                            LineEquations[i][1] = LineEquations[i][0];
                            LineEquations[i][1].minx = Vertices[i][1].x;
                            LineEquations[i][1].maxx = Vertices[i][2].x;

                            /*
                            If I get uncertain about everything, here's the
                            point at which code should be put in to verify that
                            the equations give point X1, Y1, Z1.

                            This will be necessary eventually prior to building
                            my yacht. I can't afford it to be wrong.
                            */

                            }
                            break;
                default:    {
                            //explicit calculation method
                            for (int j = 0; j < Vertices[i].Length - 3; j++) {
                                LineSegment[0] = Vertices[i][j];
                                LineSegment[1] = Vertices[i][j+1];
                                LineSegment[2] = Vertices[i][j+2];
                                LineSegment[3] = Vertices[i][j+3];
                                LineEquations[i][j] = GetLineEquation();
                                }
                            for (int j = Vertices[i].Length - 3; j > -1; j--) {
                                LineEquations[i][j + 1] = LineEquations[i][j];
                                }
                            LineEquations[i][LineEquations[i].Length - 1] = LineEquations[i][LineEquations[i].Length - 2];
                            for (int j = 0; j < LineEquations[i].Length; j++) {
                                LineEquations[i][j].minx = Vertices[i][j].x;
                                LineEquations[i][j].maxx = Vertices[i][j+1].x;
                                }
                            }
                            break;
                }
            }
        }

}
//---------------------------------------------------------------------------

line_equation GetLineEquation(void)
{
    line_equation Line;

    if (LineSegment.Length) {

        //6 march 2003
        //now for the new try of explicitly solving the equations
        //because it's explicit we only need 8 values, and the X equations are
        //redundant. Thus:-
        Line.A0 = 0.0;
        Line.A1 = 0.0;
        Line.A2 = 0.0;
        Line.A3 = 0.0;

        //now temporary variables
        double b = LineSegment[0].y;
        double c = LineSegment[0].x;
        double d = c * c;
        double e = c * d;
        double f = LineSegment[1].y;
        double g = LineSegment[1].x;
        double h = g * g;
        double j = g * h;
        double k = LineSegment[2].y;
        double l = LineSegment[2].x;
        double m = l * l;
        double n = l * m;
        double p = LineSegment[3].y;
        double q = LineSegment[3].x;
        double r = q * q;
        double s = q * r;
        //division by zero error trap variable
        double temp;

        //now to work out the y equations
        temp = ((c-l)*(d-h)+(m-d)*(c-g))*((s-e)*(c-g)-(q-c)*(e-j))-((c-l)*(e-j)+(n-e)*(c-g))*((r-d)*(c-g)-(q-c)*(d-h));
        if (fabs(temp) > Accuracy) {
            Line.B3 = ((c-l)*(d-h)+(m-d)*(c-g))*((p-b)*(c-g)-(b-f)*(q-c))-((k-b)*(c-g)-(l-c)*(b-f))*((r-d)*(c-g)-(q-c)*(d-h));
            Line.B3 = Line.B3 /(temp);
            } else {
            //here's where we would have got to if trying to divide by zero.
            if (temp < 0.0) {
                Line.B3 = -LargeNumber;
                } else {
                Line.B3 = LargeNumber;
                }
            }

        temp = (c-l)*(d-h)+(m-d)*(c-g);
        if (fabs(temp) > Accuracy) {
            Line.B2 = ((k-b)*(c-g)-(l-c)*(b-f)-Line.B3*((c-l)*(e-j)+(n-e)*(c-g)))/(temp);
            } else {
            if (temp < 0.0) {
                Line.B2 = -LargeNumber;
                } else {
                Line.B2 = LargeNumber;
                }
            }

        temp = c-g;
        if (fabs(temp) > Accuracy) {
            Line.B1 = (b-f-Line.B2*(d-h)-Line.B3*(e-j))/(temp);
            } else {
            if (temp < 0.0) {
                Line.B1 = -LargeNumber;
                } else {
                Line.B1 = LargeNumber;
                }
            }
        Line.B0 = b-Line.B1*c-Line.B2*d-Line.B3*e;

        //z equations
        b = LineSegment[0].z;
        f = LineSegment[1].z;
        k = LineSegment[2].z;
        p = LineSegment[3].z;

        temp = ((c-l)*(d-h)+(m-d)*(c-g))*((s-e)*(c-g)-(q-c)*(e-j))-((c-l)*(e-j)+(n-e)*(c-g))*((r-d)*(c-g)-(q-c)*(d-h));
        if (fabs(temp) > Accuracy) {
            Line.C3 = ((c-l)*(d-h)+(m-d)*(c-g))*((p-b)*(c-g)-(b-f)*(q-c))-((k-b)*(c-g)-(l-c)*(b-f))*((r-d)*(c-g)-(q-c)*(d-h));
            Line.C3 = Line.C3 /(temp);
            } else {
            //here's where we would have got to if trying to divide by zero.
            if (temp < 0.0) {
                Line.C3 = -LargeNumber;
                } else {
                Line.C3 = LargeNumber;
                }
            }

        temp = (c-l)*(d-h)+(m-d)*(c-g);
        if (fabs(temp) > Accuracy) {
            Line.C2 = ((k-b)*(c-g)-(l-c)*(b-f)-Line.C3*((c-l)*(e-j)+(n-e)*(c-g)))/(temp);
            } else {
            if (temp < 0.0) {
                Line.C2 = -LargeNumber;
                } else {
                Line.C2 = LargeNumber;
                }
            }

        temp = c-g;
        if (fabs(temp) > Accuracy) {
            Line.C1 = (b-f-Line.C2*(d-h)-Line.C3*(e-j))/(temp);
            } else {
            if (temp < 0.0) {
                Line.C1 = -LargeNumber;
                } else {
                Line.C1 = LargeNumber;
                }
            }
        Line.C0 = b-Line.C1*c-Line.C2*d-Line.C3*e;

        }

    return Line;

}
//---------------------------------------------------------------------------

line_equation GetQuadraticLineEquation(void)
{
    line_equation Line;

    if (LineSegment.Length) {

        //6 march 2003
        //now for the new try of explicitly solving the equations
        //because it's explicit we only need 8 values, and the X equations are
        //redundant. Thus:-
        Line.A0 = 0.0;
        Line.A1 = 0.0;
        Line.A2 = 0.0;
        Line.A3 = 0.0;

        //now temporary variables
        double b = LineSegment[0].y;
        double c = LineSegment[0].x;
        double d = c * c;
        //double e = c * d;
        double f = LineSegment[1].y;
        double g = LineSegment[1].x;
        double h = g * g;
        //double j = g * h;
        double k = LineSegment[2].y;
        double l = LineSegment[2].x;
        double m = l * l;
        //double n = l * m;
        //double p = LineSegment[3].y;
        //double q = LineSegment[3].x;
        //double r = q * q;
        //double s = q * r;
        //division by zero error trap variable
        double temp;

        //now to work out the y equations
        /*temp = ((c-l)*(d-h)+(m-d)*(c-g))*((s-e)*(c-g)-(q-c)*(e-j))-((c-l)*(e-j)+(n-e)*(c-g))*((r-d)*(c-g)-(q-c)*(d-h));
        if (fabs(temp) > Accuracy) {
            Line.B3 = ((c-l)*(d-h)+(m-d)*(c-g))*((p-b)*(c-g)-(b-f)*(q-c))-((k-b)*(c-g)-(l-c)*(b-f))*((r-d)*(c-g)-(q-c)*(d-h));
            Line.B3 = Line.B3 /(temp);
            } else {
            //here's where we would have got to if trying to divide by zero.
            if (temp < 0.0) {
                Line.B3 = -LargeNumber;
                } else {
                Line.B3 = LargeNumber;
                }
            }*/
        Line.B3 = 0.0;

        temp = (c-l)*(d-h)+(m-d)*(c-g);
        if (fabs(temp) > Accuracy) {
            Line.B2 = ((k-b)*(c-g)-(l-c)*(b-f)/*-Line.B3*((c-l)*(e-j)+(n-e)*(c-g))*/)/(temp);
            } else {
            if (temp < 0.0) {
                Line.B2 = -LargeNumber;
                } else {
                Line.B2 = LargeNumber;
                }
            }

        temp = c-g;
        if (fabs(temp) > Accuracy) {
            Line.B1 = (b-f-Line.B2*(d-h)/*-Line.B3*(e-j)*/)/(temp);
            } else {
            if (temp < 0.0) {
                Line.B1 = -LargeNumber;
                } else {
                Line.B1 = LargeNumber;
                }
            }
        Line.B0 = b-Line.B1*c-Line.B2*d/*-Line.B3*e*/;

        //z equations
        b = LineSegment[0].z;
        f = LineSegment[1].z;
        k = LineSegment[2].z;
        //p = LineSegment[3].z;

        /*temp = ((c-l)*(d-h)+(m-d)*(c-g))*((s-e)*(c-g)-(q-c)*(e-j))-((c-l)*(e-j)+(n-e)*(c-g))*((r-d)*(c-g)-(q-c)*(d-h));
        if (fabs(temp) > Accuracy) {
            Line.C3 = ((c-l)*(d-h)+(m-d)*(c-g))*((p-b)*(c-g)-(b-f)*(q-c))-((k-b)*(c-g)-(l-c)*(b-f))*((r-d)*(c-g)-(q-c)*(d-h));
            Line.C3 = Line.C3 /(temp);
            } else {
            //here's where we would have got to if trying to divide by zero.
            if (temp < 0.0) {
                Line.C3 = -LargeNumber;
                } else {
                Line.C3 = LargeNumber;
                }
            }*/
        Line.C3 = 0.0;

        temp = (c-l)*(d-h)+(m-d)*(c-g);
        if (fabs(temp) > Accuracy) {
            Line.C2 = ((k-b)*(c-g)-(l-c)*(b-f)/*-Line.C3*((c-l)*(e-j)+(n-e)*(c-g))*/)/(temp);
            } else {
            if (temp < 0.0) {
                Line.C2 = -LargeNumber;
                } else {
                Line.C2 = LargeNumber;
                }
            }

        temp = c-g;
        if (fabs(temp) > Accuracy) {
            Line.C1 = (b-f-Line.C2*(d-h)/*-Line.C3*(e-j)*/)/(temp);
            } else {
            if (temp < 0.0) {
                Line.C1 = -LargeNumber;
                } else {
                Line.C1 = LargeNumber;
                }
            }
        Line.C0 = b-Line.C1*c-Line.C2*d/*-Line.C3*e*/;

        }

    return Line;

}
//---------------------------------------------------------------------------

double DistanceBetweenPoints(nodalposition node1, nodalposition node2)
{
    double length = (node2.x - node1.x) * (node2.x - node1.x);
    length += (node2.y - node1.y) * (node2.y - node1.y);
    length += (node2.z - node1.z) * (node2.z - node1.z);
    length = sqrt(length);

    return length;
}
//---------------------------------------------------------------------------

void MergeLines(void)
{
    //this method iterates through all the lines in the model checking whether
    //the vertices are within the distance of "Accuracy" of one another
    if (Vertices.Length) {
        for (int i = 0; i < Vertices.Length - 1; i++) {
            for (int j = i + 1; j < Vertices.Length; j++) {
                if (DistanceBetweenPoints(Vertices[i][0],Vertices[j][0]) < Accuracy) Vertices[j][0] = Vertices[i][0];
                if (DistanceBetweenPoints(Vertices[i][Vertices[i].Length - 1],Vertices[j][0]) < Accuracy) Vertices[j][0] = Vertices[i][Vertices[i].Length - 1];
                if (DistanceBetweenPoints(Vertices[i][0],Vertices[j][Vertices[j].Length - 1]) < Accuracy) Vertices[j][Vertices[j].Length - 1] = Vertices[i][0];
                if (DistanceBetweenPoints(Vertices[i][Vertices[i].Length - 1],Vertices[j][Vertices[j].Length - 1]) < Accuracy) Vertices[j][Vertices[j].Length - 1] = Vertices[i][Vertices[i].Length - 1];
                }
            }
        }
}
//---------------------------------------------------------------------------

void SortLineOrientation(void)
{
    //this method checks to see whether the line has it's x value greater
    //at the end or the beginning. And makes sure it's greater at the end.
    if (Vertices.Length) {
        for (int i = 0; i < Vertices.Length; i++) {
            if (Vertices[i][0].x > Vertices[i][Vertices[i].Length - 1].x) {
                //swaps all vertices in line if end greater than beginning
                nodalposition swap;
                int halfway = (int)floor( Vertices[i].Length / 2);
                for (int j = 0; j < halfway; j++) {
                    swap = Vertices[i][j];
                    Vertices[i][j] = Vertices[i][Vertices[i].Length - j - 1];
                    Vertices[i][Vertices[i].Length - j - 1] = swap;
                    }
                }
            }
        }

}
//---------------------------------------------------------------------------

void DeleteDuplicateLines(void)
{
    //this method ensures that there are no duplicate lines in the model.
    //A line is considered to be duplicate if 2 or more vertices are duplicated.
    if (Vertices.Length) {
        int commonvertices;
        int i = 0;
        int j;
        while (i < Vertices.Length) {
            //using while loops here as the total number of lines may change
            //during a loop
            j = i + 1;
            while (j < Vertices.Length) {
                commonvertices = 0;
                for (int k = 0; k < Vertices[i].Length; k++) {
                    for (int l = 0; l < Vertices[j].Length; l++) {
                        if (DistanceBetweenPoints(Vertices[i][k],Vertices[j][l]) < Accuracy) commonvertices++;
                        }
                    }
                if (commonvertices > 1) {
                    /*
                    There's more than 1 duplicated vertex between these two lines
                    So, what to do?
                    There are several options, as listed below:-
                    0)    Line j is equal to line i
                    1)    Line j is a subset of line i
                    2)    Line i is a subset of line j
                    3)    Lines i & j overlap at one or other end
                    4)    Lines i & j are coincident over some part of their length
                    5)    Intersections at line ends creating loops
                    6)    Multiple intersections not all at line ends

                    Depending on which is found, the following will be done
                    0)    Delete line j
                    1)    Delete line j
                    2)    Delete line i
                    3)    Merge lines i & j into a single line
                    4)    Break the lines into up to 5 pieces
                          - no, can't be bothered, it's all too difficult
                    5)    Delete line with fewest vertices
                    6)    Detonate explosive under user's seat

                    The following easy indicators may be used
                    0)    commonvertices = Vertices[j].Length
                    1)    commonvertices = Vertices[j].Length
                    2)    commonvertices = Vertices[i].Length
                          Vertices[i].Length < Vertices[j].Length
                    3&4)  No easy indicators
                    5)    Vertices[i][0] = Vertices[j][0]
                          Vertices[i][Vertices[i].Length - 1] = Vertices[j][Vertices[j].Length - 1]
                          commonvertices = 2
                    6)    No easy indicators
                    */
                    if (commonvertices == Vertices[j].Length) {
                        //intersection situation 0 & 1 found
                        DeleteLine(j);
                        j--;
                        } else {
                        if (commonvertices == Vertices[i].Length) {
                            //intersection situation 2 found
                            DeleteLine(i);
                            i--;
                            j = Vertices.Length;
                            } else {
                            if (DistanceBetweenPoints(Vertices[i][0],Vertices[j][0]) < Accuracy &&
                                DistanceBetweenPoints(Vertices[i][Vertices[i].Length - 1],Vertices[j][Vertices[j].Length - 1]) < Accuracy
                                && commonvertices == 2) {
                                //intersection situation 5
                                if (Vertices[i].Length > Vertices[j].Length) {
                                    DeleteLine(j);
                                    j--;
                                    } else {
                                    DeleteLine(i);
                                    i--;
                                    j = Vertices.Length;
                                    }
                                } else {
                                /*
                                Narrowed condition down to intersection
                                conditions 3, 4, or 6. Not easy this, is it?

                                I Can't be bothered to sort out 4 or 6, so I'll
                                get the computer to throw up a raspberry message
                                and delete the line with the least vertices.

                                Maybe I'll come back to 4 later.
                                */
                                // Check for Condition 3a.
                                // End of line i overlaps beginning of line j
                                bool overlapping3a = true;
                                for (int k = 0; k < commonvertices; k++) {
                                    if (DistanceBetweenPoints(Vertices[i][Vertices[i].Length - 1 - k],Vertices[j][k]) > Accuracy) overlapping3a = false;
                                    }
                                if (overlapping3a) {
                                    for (int k = commonvertices - 1; k < Vertices[j].Length; k++) {
                                        Vertices[i].Length++;
                                        Vertices[i][Vertices[i].Length - 1] = Vertices[j][k];
                                        }
                                    DeleteLine(j);
                                    i--;
                                    j = Vertices.Length;
                                    } else {
                                    //case 3a false. try case 3b, end of line j
                                    //overlaps beginning of line i
                                    bool overlapping3b = true;
                                    for (int k = 0; k < commonvertices; k++) {
                                        if (DistanceBetweenPoints(Vertices[j][Vertices[j].Length - 1 - k],Vertices[i][k]) > Accuracy) overlapping3b = false;
                                        }
                                    if (overlapping3b) {
                                        for (int k = commonvertices - 1; k < Vertices[i].Length; k++) {
                                            Vertices[j].Length++;
                                            Vertices[j][Vertices[j].Length - 1] = Vertices[i][k];
                                            }
                                        DeleteLine(i);
                                        i--;
                                        j = Vertices.Length;
                                        } else {
                                        //case 3b false.
                                        //It's case 4 or 6. Blow Raspberry.
                                        //delete line with fewest vertices.
                                        printf("Sorry, but two lines intersect at more than one point.\n\nPlease check the input file and delete at least one.\n");
                                        if (Vertices[i].Length > Vertices[j].Length) {
                                            DeleteLine(j);
                                            j--;
                                            } else {
                                            DeleteLine(i);
                                            i--;
                                            j = Vertices.Length;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                j++;
                }
            i++;
            }
        }
}
//---------------------------------------------------------------------------

void DeleteZeroLengthLines(void)
{
    //this method ensures that there are no very small length lines in the model
    if (Vertices.Length) {
        double length;
        int i = 0;
        while (i < Vertices.Length) {
            length = 0.0;
            for (int j = 0; j < Vertices[i].Length - 1; j++) {
                length += DistanceBetweenPoints(Vertices[i][j], Vertices[i][j + 1]);
                }
            if (length < Accuracy) {
                //delete the current line and move all remaining ones up
                DeleteLine(i);
                i--;
                }
            i++;
            }
        }

}
//---------------------------------------------------------------------------

void DeleteLine(int LineNumber)
{
    //This method deletes a line of vertices

    for (int j = LineNumber; j < Vertices.Length - 1; j++) {
        Vertices[j].Length = Vertices[j + 1].Length;
        for (int k = 0; k < Vertices[j].Length; k++) {
            Vertices[j][k] = Vertices[j + 1][k];
            }
        }
    Vertices[Vertices.Length - 1].Length = 0;
    Vertices.Length--;

}
//---------------------------------------------------------------------------

bool CheckContinuousBoundary(void)
{
    /*
    This method checks whether there is a continuous boundary.
    If there is, well that's great. Method deletes all lines not on the boundary
    and returns true.
    If there is no boundary loop, then that's it - returns false and stops.

    Because it's possible for a boundary loop to exist, yet not to include the
    first line, the method works by starting with the first line, then
    attempting to find a loop. While going through the line list, other lines
    are checked and kept as alternatives. If the first alternative fails to
    produce a loop, the program backs up to the last vertex with an alternative
    and tries that until there are no more alternatives to try.

    If this fails, the next line is tried until all
    lines have been tried.

    Eventually, if no boundary has been found, the accuracy of merging the
    ends of the lines is repeatedly reduced by a factor of 10 until the accuracy
    is 1/100 the model size. This gets over a problem I had where AutoCAD LT97
    didn't snap exactly to the ends of two lines.
    */


    //variable to indicate whether a boundary has been found
    bool continuousboundary = false;

    BoundaryLines.Length = 0;
    boundarylines CurrentBoundaryLine;
    nodalposition CurrentVertex;

    int startlineno;
    bool continuesearching;
    double localAccuracy = Accuracy;

    while (!continuousboundary && localAccuracy < (LineExtents.xmax - LineExtents.xmin)/100.0) {
        startlineno = 0;
        while (!continuousboundary && startlineno < Vertices.Length) {
            if (BoundaryLines.Length > 1) {
                for (int i = 1; i < BoundaryLines.Length; i++) {
                    BoundaryLines[i].Length = 0;
                    }
                }
            BoundaryLines.Length = 1;
            BoundaryLines[0].Length = 1;
            CurrentBoundaryLine.linenumber = startlineno;
            CurrentBoundaryLine.startatbeginning = true;

            BoundaryLines[0][0] = CurrentBoundaryLine;

            continuesearching = true;

            //look for next line in this loop
            /*
            I had to think about this for more than a week before I could figure
            out a way of doing this without "goto" statements!

            I think that this is one of the more difficult routines I've ever
            done. The code isn't difficult, it was just thinking a way around
            the problem that was almost impossible. As usual, I finally worked
            it out while doing the washing up one evening.
            */
            while (continuesearching) {
                //get vertex at other end of current boundary line
                CurrentVertex = Vertices[CurrentBoundaryLine.linenumber][(CurrentBoundaryLine.startatbeginning ? Vertices[CurrentBoundaryLine.linenumber].Length - 1 : 0)];
                //look for all vertices joining CurrentVertex
                BoundaryLines.Length++;
                for (int i = 0; i < Vertices.Length; i++) {
                    if (i != CurrentBoundaryLine.linenumber) {
                        if (DistanceBetweenPoints(CurrentVertex, Vertices[i][0]) < localAccuracy) {
                            BoundaryLines[BoundaryLines.Length - 1].Length++;
                            BoundaryLines[BoundaryLines.Length - 1][BoundaryLines[BoundaryLines.Length - 1].Length - 1].linenumber = i;
                            BoundaryLines[BoundaryLines.Length - 1][BoundaryLines[BoundaryLines.Length - 1].Length - 1].startatbeginning = true;
                            }
                        if (DistanceBetweenPoints(CurrentVertex, Vertices[i][Vertices[i].Length - 1]) < localAccuracy) {
                            BoundaryLines[BoundaryLines.Length - 1].Length++;
                            BoundaryLines[BoundaryLines.Length - 1][BoundaryLines[BoundaryLines.Length - 1].Length - 1].linenumber = i;
                            BoundaryLines[BoundaryLines.Length - 1][BoundaryLines[BoundaryLines.Length - 1].Length - 1].startatbeginning = false;
                            }
                        }
                    }

                //check if any lines attaching to the current one have been found
                if (BoundaryLines[BoundaryLines.Length - 1].Length) {
                    //lines found then see if the found line is on the list
                    bool foundinlist = false;
                    for (int i = 0; i < BoundaryLines.Length - 1; i++) {
                        if (BoundaryLines[i][0].linenumber == BoundaryLines[BoundaryLines.Length - 1][0].linenumber) foundinlist = true;
                        }
                    if (foundinlist) {
                        //if yes, set continuesearching = false,
                        // and continuousboundary = true
                        continuesearching = false;
                        continuousboundary = true;
                        } else {
                        //if not, set CurrentBoundaryLine as beginning of most
                        //recently found line
                        CurrentBoundaryLine = BoundaryLines[BoundaryLines.Length - 1][0];
                        }
                    } else {
                    //no joining lines, check to see if there are alternatives
                    while (BoundaryLines.Length && BoundaryLines[BoundaryLines.Length - 1].Length < 2) {
                        BoundaryLines[BoundaryLines.Length - 1].Length = 0;
                        BoundaryLines.Length--;
                        }
                    if (BoundaryLines.Length) {
                        //if yes, set most recent alternative as
                        //CurrentBoundaryLine
                        for (int i = 0; i < BoundaryLines[BoundaryLines.Length - 1].Length - 1; i++) {
                            BoundaryLines[BoundaryLines.Length - 1][i] = BoundaryLines[BoundaryLines.Length - 1][i+1];
                            }
                        CurrentBoundaryLine = BoundaryLines[BoundaryLines.Length - 1][0];
                        } else {
                        //if no, set continuesearching as false and need a new
                        //start line to continue
                        continuesearching = false;
                        }
                    }
                }

            startlineno++;
            }
        localAccuracy = localAccuracy * 10.0;

        }

    /*
    Having got here, the boundary line list (if there is one) may contain lines
    that are not actually on the boundary. The un-necessary lines in the
    boundary list are deleted
    */

    if (continuousboundary) {
        //first delete all un-necessary alternative lines
        for (int i = 0; i < BoundaryLines.Length; i++) {
            BoundaryLines[i].Length = 1;
            }
        //now delete all boundary lines found before the continuous boundary
        while (BoundaryLines.Length && BoundaryLines[0][0].linenumber != BoundaryLines[BoundaryLines.Length - 1][0].linenumber ) {
            for (int i = 0; i < BoundaryLines.Length - 1; i++) {
                BoundaryLines[i][0] = BoundaryLines[i+1][0];
                }
            BoundaryLines[BoundaryLines.Length - 1].Length = 0;
            BoundaryLines.Length--;
            }
        //if the continuous boundary includes the first line found, the
        //beginning and end entries will be identical
        if (BoundaryLines.Length && BoundaryLines[0][0].linenumber == BoundaryLines[BoundaryLines.Length - 1][0].linenumber && BoundaryLines[0][0].startatbeginning == BoundaryLines[BoundaryLines.Length - 1][0].startatbeginning) {
            //no need to duplicate first line in boundary
            BoundaryLines[BoundaryLines.Length - 1].Length = 0;
            BoundaryLines.Length--;
            } else {
            //first and last lines not in boundary - so delete
            BoundaryLines[BoundaryLines.Length - 1].Length = 0;
            BoundaryLines.Length--;
            for (int i = 0; i < BoundaryLines.Length - 1; i++) {
                BoundaryLines[i][0] = BoundaryLines[i+1][0];
                }
            BoundaryLines[BoundaryLines.Length - 1].Length = 0;
            BoundaryLines.Length--;
            }

        //final safety check
        continuousboundary = (bool) BoundaryLines.Length;

        /*
        Now to loop through the boundary merging the ends of the lines in an
        effort to be certain that there will be no break in the boundary. This
        is done by making the vertices identical at the ends of adjoining lines.
        */
        if (BoundaryLines.Length > 1) {
            //boundarylines previousline, thisline;
            for (int i = 1; i < BoundaryLines.Length; i++) {
                //loop through finding vertices
                //previousline = BoundaryLines[i-1][0];
                //thisline = BoundaryLines[i][0];
                Vertices[BoundaryLines[i][0].linenumber][(int)(1-(int)BoundaryLines[i][0].startatbeginning) * (Vertices[BoundaryLines[i][0].linenumber].Length - 1)] =
                    Vertices[BoundaryLines[i-1][0].linenumber][(int)((int)BoundaryLines[i-1][0].startatbeginning) * (Vertices[BoundaryLines[i-1][0].linenumber].Length - 1)];
                }
            }

        }

    //delete lines not in the boundary
    int checkingline = 0;
    bool foundinboundary;
    while (Vertices.Length && checkingline < Vertices.Length) {
        foundinboundary = false;
        for (int i = 0; i < BoundaryLines.Length; i++) {
            if (checkingline == BoundaryLines[i][0].linenumber) foundinboundary = true;
            }
        if (!foundinboundary) {
            DeleteLine(checkingline);
            for (int i = 0; i < BoundaryLines.Length; i++) {
                if (checkingline < BoundaryLines[i][0].linenumber) BoundaryLines[i][0].linenumber--;
                }
            checkingline--;
            }
        checkingline++;
        }

    return continuousboundary;

}
//---------------------------------------------------------------------------

ErrorProneResult GetCoordinate(double x, int linenumber)
{
    //this is a very important routine. It calculates the position of
    //a co-ordinate based solely on the x co-ordinate and the line number

    //OK, let's get on with it.



    //find the line equation number applicable.
    int equationnumber = 0;
    ErrorProneResult Coordinate;
    Coordinate.x = x;
    Coordinate.ErrorPresent = false;

    while (equationnumber < LineEquations[linenumber].Length - 1 && LineEquations[linenumber][equationnumber].maxx < x) {
        equationnumber++;
        }


    /*
    Now we've identified the line number equations it's time to solve the cubic
    equations
        y = b0 + b1*x + b2*x^2 + b3*x^3
        z = c0 + c1*x + c2*x^2 + c3*x^3
    */

    //solving for x
    line_equation Equation = LineEquations[linenumber][equationnumber];



    //explicit calculation now

    //OK, let's get on with it.
    //explicit calculation
    Coordinate = GetCoord(x,Equation);

    return Coordinate;
}
//---------------------------------------------------------------------------

void CreateTopandBottomLines(void)
{
    //creates the top and bottom lines of the boundary
    /*
    The top line is the boundary line where "Startatbeginning" is true, and
    extends backwards to xmin and forwards to xmax. The bottom line is
    all the other lines in the boundary.

    Note, the top line is not necessarily the "Top" line in a physical sense in
    the 3D boundary. It actually relates to the display position in the
    developed plate diagram.
    */
    TopLine.Length = 0;
    BottomLine.Length = 0;

    //run through the boundary line list until a line ends at the minx point
    int linenumber = 0;
    while (linenumber < Vertices.Length && Vertices[linenumber][0].x != LineExtents.xmin){
        linenumber++;
        }
    //now linenumber holds the number of a line that touches the xmin end of the
    //model.
    int boundarypointer = 0;
    while (boundarypointer < BoundaryLines.Length && BoundaryLines[boundarypointer][0].linenumber != linenumber) {
        boundarypointer++;
        }
    //now we've found a line that has an end at the minimum point of the model.
    //however still are not sure that it's on the top line
    while (boundarypointer < BoundaryLines.Length && !BoundaryLines[boundarypointer][0].startatbeginning) {
        boundarypointer++;
        }


    //double check to see that the line identified as the start of the top line
    //actually starts at xmin.
    if (boundarypointer >= BoundaryLines.Length) {
        //it appears there may be a special case where things go wrong if the
        //start of the last boundary line is also the start of the top line
        //This also means that the boundary is reversed for the purposes of this
        //analysis
        if (!BoundaryLines[BoundaryLines.Length - 1][0].startatbeginning && Vertices[BoundaryLines[BoundaryLines.Length - 1][0].linenumber][0].x == ModelExtents.xmin) {
            boundarylines temp /*= BoundaryLines[0][0]*/;
            for (int j = 0; j < (int) ceil((double)BoundaryLines.Length / 2.0); j++) {
                temp = BoundaryLines[j][0];
                temp.startatbeginning = !temp.startatbeginning;
                BoundaryLines[j][0] = BoundaryLines[BoundaryLines.Length - 1 - j][0];
                BoundaryLines[j][0].startatbeginning = !BoundaryLines[j][0].startatbeginning;
                BoundaryLines[BoundaryLines.Length - 1 - j][0] = temp;
                }
            boundarypointer = 0;
            /*while (boundarypointer < BoundaryLines.Length - 2 && BoundaryLines[boundarypointer][0].startatbeginning) {
                boundarypointer++;
                }*/
            /*
            for (int j = 1; j < BoundaryLines.Length; j++) {
                BoundaryLines[j - 1][0] = BoundaryLines[j][0];
                }
            BoundaryLines[BoundaryLines.Length - 1][0] = temp;
            boundarypointer = BoundaryLines.Length - 2;
            */
            } else {
            //debug code
            /*
            RichEdit1->Lines->Add("Number of boundary Lines = " + AnsiString(BoundaryLines.Length));
            for (int j = 0; j < BoundaryLines.Length; j++) {
                if (BoundaryLines[j].Length) {
                    RichEdit1->Lines->Add("BoundaryLine = " + AnsiString(j) + "\tLine Number = " + AnsiString(BoundaryLines[j][0].linenumber) + "\tStart at Beginning = " + FixedLen(5,BoundaryLines[j][0].startatbeginning));
                    } else {
                    RichEdit1->Lines->Add("BoundaryLine = " + AnsiString(j) + "\tZero Length entry");
                    }
                }
                //end debug
                */
            printf("CreateTopandBottomLines\nError: Cannot Find Top Line.\n'boundarypointer' bigger than 'BoundaryLines.Length'\n\nContact Technical Support.");
            return;
            }
        }

    if (Vertices[BoundaryLines[boundarypointer][0].linenumber][0].x != LineExtents.xmin) {
        printf("CreateTopandBottomLines:\nError: Cannot Find Top Line.\nLine does not start at xmin\n\nCheck to see that a line starts at the xmin point of the model. If it does,\nContact Technical Support.");
        return;
        }
    //now it's pointing to the top line, so can copy the boundary lines to the
    //top and bottom lines
    bool addingtotopline = true;
    for (int i = boundarypointer; i < BoundaryLines.Length; i++) {
        if (BoundaryLines[i][0].startatbeginning && addingtotopline) {
            TopLine.Length++;
            TopLine[TopLine.Length - 1] = BoundaryLines[i][0];
            } else {
            addingtotopline = false;
            BottomLine.Length++;
            for (int j = BottomLine.Length - 2; j > -1; j--) {
                BottomLine[j+1] = BottomLine[j];
                }
            BottomLine[0].linenumber = BoundaryLines[i][0].linenumber;
            BottomLine[0].startatbeginning = false;
            }
        }
    for (int i = 0; i < boundarypointer; i++) {
        if (BoundaryLines[i][0].startatbeginning && addingtotopline) {
            TopLine.Length++;
            TopLine[TopLine.Length - 1] = BoundaryLines[i][0];
            } else {
            addingtotopline = false;
            BottomLine.Length++;
            for (int j = BottomLine.Length - 2; j > -1; j--) {
                BottomLine[j+1] = BottomLine[j];
                }
            BottomLine[0].linenumber = BoundaryLines[i][0].linenumber;
            BottomLine[0].startatbeginning = false;
            }
        }

    //that's it, done. The bottom line and top line are completed, listed in
    //order of -x to +x

    /*
    Now, to avoid problems with lines that are perpendicular to the X axis
    which join the TopLine and BottomLine, these lines should be deleted from
    the top or bottom lines.
    The only lines where this may occur is at either the beginning or end of the
    Top and Bottom Lines.
    */

    if (TopLine.Length) {
        if (fabs(Vertices[TopLine[0].linenumber][0].x - LineExtents.xmin) < Accuracy
            && fabs(Vertices[TopLine[0].linenumber][1].x - LineExtents.xmin) < Accuracy
            && Vertices[TopLine[0].linenumber].Length == 2) {
            //The line is straight joining the top & bottom lines
            for (int i = 0; i < TopLine.Length - 1; i++) {
                TopLine[i] = TopLine[i+1];
                }
            TopLine.Length--;
            }
        if (fabs(Vertices[TopLine[TopLine.Length - 1].linenumber][0].x - LineExtents.xmax) < Accuracy
            && fabs(Vertices[TopLine[TopLine.Length - 1].linenumber][1].x - LineExtents.xmax) < Accuracy
            && Vertices[TopLine[TopLine.Length - 1].linenumber].Length == 2) {
            //The line is straight joining the top & bottom lines
            TopLine.Length--;
            }
        }

    if (BottomLine.Length) {
        if (fabs(Vertices[BottomLine[0].linenumber][0].x - LineExtents.xmin) < Accuracy
            && fabs(Vertices[BottomLine[0].linenumber][1].x - LineExtents.xmin) < Accuracy
            && Vertices[BottomLine[0].linenumber].Length == 2) {
            //The line is straight joining the top & bottom lines
            for (int i = 0; i < BottomLine.Length - 1; i++) {
                BottomLine[i] = BottomLine[i+1];
                }
            BottomLine.Length--;
            }
        if (fabs(Vertices[BottomLine[BottomLine.Length - 1].linenumber][0].x - LineExtents.xmax) < Accuracy
            && fabs(Vertices[BottomLine[BottomLine.Length - 1].linenumber][1].x - LineExtents.xmax) < Accuracy
            && Vertices[BottomLine[BottomLine.Length - 1].linenumber].Length == 2) {
            //The line is straight joining the top & bottom lines
            BottomLine.Length--;
            }
        }

    /*
    //debug code
    RichEdit1->Lines->Add("Top Line");
    for (int i = 0; i < TopLine.Length; i++) {
        RichEdit1->Lines->Add("TopLine = " + AnsiString(i) + "\tLine Number = " + AnsiString(TopLine[i].linenumber) + "\tStart at Beginning = " + FixedLen(5,TopLine[i].startatbeginning));
        }
    RichEdit1->Lines->Add("Bottom Line");
    for (int i = 0; i < BottomLine.Length; i++) {
        RichEdit1->Lines->Add("BottomLine = " + AnsiString(i) + "\tLine Number = " + AnsiString(BottomLine[i].linenumber) + "\tStart at Beginning = " + FixedLen(5,BottomLine[i].startatbeginning));
        }

    //end debug code
    //seems to work now
    */

}
//---------------------------------------------------------------------------

ErrorProneResult GetTopLinePosition(double x)
{
    //This method returns a position on the top line given by x
    ErrorProneResult coord;
    coord.ErrorPresent = false;

    if (TopLine.Length) {
        //first find out which line we're dealing with
        int linenumber = 0;
        while (linenumber < TopLine.Length && Vertices[TopLine[linenumber].linenumber][Vertices[TopLine[linenumber].linenumber].Length - 1].x < x) {
            linenumber++;
            }

        //get the actual linenumber
        linenumber = TopLine[linenumber].linenumber;

        coord = GetCoordinate(x, linenumber);
        } else {
        coord.x = 0.0;
        coord.y = 0.0;
        coord.z = 0.0;
        coord.ErrorPresent = true;
        coord.ErrorMessage = "GetTopLinePosition - No Top Line Defined";
        //Application->MessageBox("No Top Line Defined","GetTopLinePosition",MB_OK + MB_ICONERROR);
        }

    return coord;

}
//---------------------------------------------------------------------------

ErrorProneResult GetBottomLinePosition(double x)
{
    //This method returns a position on the Bottom line given by x

    ErrorProneResult coord;
    coord.ErrorPresent = false;

    if (BottomLine.Length) {
        //first find out which line we're dealing with
        int linenumber = 0;
        while (linenumber < BottomLine.Length && Vertices[BottomLine[linenumber].linenumber][Vertices[BottomLine[linenumber].linenumber].Length - 1].x < x) {
            linenumber++;
            }

        //get the actual linenumber
        linenumber = BottomLine[linenumber].linenumber;

        coord = GetCoordinate(x, linenumber);
        } else {
        coord.x = 0.0;
        coord.y = 0.0;
        coord.z = 0.0;
        coord.ErrorPresent = true;
        coord.ErrorMessage = "GetBottomLinePosition - No Bottom Line Defined";
        //Application->MessageBox("No Bottom Line Defined","GetBottomLinePosition",MB_OK + MB_ICONERROR);
        }

    return coord;

}
//---------------------------------------------------------------------------

void Develop(void)
{
    //Develop the Plate NOW....
    if (TopLine.Length && BottomLine.Length) {

        //right then, start the bit that the whole program's here for
        try {
            fBusy = true;


            OutputClear();

            //this is an error trap which should be un-necessary
            //better safe than sorry though.
            if (!StartLine.Length) {
                StartLine.Length = 2;
                ErrorProneResult Result;
                Result = GetTopLinePosition((LineExtents.xmax + LineExtents.xmin) * 0.5);
                if (Result.ErrorPresent) {
                    ErrorListing->Add("Preparing to Develop Plate");
                    ErrorListing->Add("Error finding Top End of Start Line");
                    ErrorListing->Add("Error Message = " + Result.ErrorMessage);
                    }
                StartLine[0] = GetCoordinate(Result);
                Result = GetBottomLinePosition((LineExtents.xmax + LineExtents.xmin) * 0.5);
                if (Result.ErrorPresent) {
                    ErrorListing->Add("Preparing to Develop Plate");
                    ErrorListing->Add("Error finding Bottom End of Start Line");
                    ErrorListing->Add("Error Message = " + Result.ErrorMessage);
                    }
                StartLine[1] = GetCoordinate(Result);
                }

            try {
                DevelopPlate();
                }

            catch (...) {
                //RichEdit1->Lines->Clear();
                printf("Error Developing Plate\n");
                }

            DisplayErrors();


            try {
                CreateandDisplayDXFFile();
                printf("Plate Development Complete\n");
                }

            catch (...) {
                printf("Error saving DXF File\n");
                }

            }

        __finally {
            fBusy = false;
            }

        }


}
//---------------------------------------------------------------------------

void DevelopPlate(void)
{
    //Do the actual development
    /*
    The development is done by starting at the start line, then working towards
    one end of the model until the end is reached, then working the other way.
    The XY coordinates are kept in four arrays, TopLineAft, TopLineFwd,
    BottomLineAft, BottomLineFwd.

    The development starts at the start line, then increments both ways.
    The size of the x direction step is variable, but defaults to 1/1000
    the maximum length of the model. There is a maximum limit on the size
    of 1e6 * Accuracy - just to ensure that even larger numbers of steps are
    taken with large models.

    If the length of the segment from the previous point exceeds 1/500 the model
    x direction length, the step size is halved in order to maintain accuracy.

    The reason for having the start line somewhere in the middle is to reduce
    the size of the error at each end of the developed plate (maybe!!!).
    */
    printf("Developing Plate\n");

    //get rid of all previous attempts
    TopLineAft.Length = 1;
    TopLineFwd.Length = 1;
    BottomLineAft.Length = 1;
    BottomLineFwd.Length = 1;

    //set the first points for the top and bottom lines
    //The start line is set as being at the X = 0 line
    TopLineAft[0].x = 0;
    TopLineAft[0].y = DistanceBetweenPoints(StartLine[0],StartLine[1]);
    TopLineFwd[0] = TopLineAft[0];
    BottomLineAft[0].x = 0;
    BottomLineAft[0].y = 0;
    BottomLineFwd[0] = BottomLineAft[0];

    //going aft from start line
    nodalposition TopOldPoint = StartLine[0];
    nodalposition BottomOldPoint = StartLine[1];
    nodalposition TopNewPoint;
    nodalposition BottomNewPoint;
    double OldX = StartX;
    double stepsize = max((LineExtents.xmax - LineExtents.xmin)/1000,Accuracy);
    if (stepsize > Accuracy * 1e6) stepsize = Accuracy * 1e6;
    double NewX;
    double tempstepsize;
    double maximumsegmentlength = stepsize * 2;
    //the next variables are line length storage variables
    double OldToptoOldBottom, OldToptoNewTop, OldBottomtoNewTop, OldToptoNewBottom, OldBottomtoNewBottom;
    point NewTop2D, NewBottom2D;
    double theta;
    TwoDMatrix MatrixZ;
    ErrorProneResult Result;
    bool carryingerroraround;
    //double size = LineExtents.xmax - LineExtents.xmin;

    //escape clause!
    int loopcount = 1;

    try {
    try {
    while (OldX < LineExtents.xmax && !Stop ) {
        NewX = OldX + stepsize;
        if (NewX > LineExtents.xmax) NewX = LineExtents.xmax;
        Result = GetTopLinePosition(NewX);
        if (Result.ErrorPresent) {
            ErrorListing->Add("Developing Plate, Segment " + AnsiString(loopcount));
            ErrorListing->Add("Can't Find the Top Line Position");
            ErrorListing->Add("Error Message = " + Result.ErrorMessage);
            } else {
            TopNewPoint = GetCoordinate(Result);
            Result = GetBottomLinePosition(NewX);
            if (Result.ErrorPresent) {
                ErrorListing->Add("Developing Plate, Segment " + AnsiString(loopcount));
                ErrorListing->Add("Can't Find the Bottom Line Position");
                ErrorListing->Add("Error Message = " + Result.ErrorMessage);
                } else {
                BottomNewPoint = GetCoordinate(Result);
                tempstepsize = stepsize;
                carryingerroraround = true;
                while ((DistanceBetweenPoints(TopOldPoint,TopNewPoint) > maximumsegmentlength ||  DistanceBetweenPoints(BottomOldPoint,BottomNewPoint) > maximumsegmentlength || carryingerroraround) && tempstepsize > 100 * Accuracy) {
                    tempstepsize = tempstepsize/2;
                    NewX = OldX + tempstepsize;
                    Result = GetTopLinePosition(NewX);
                    carryingerroraround = Result.ErrorPresent;
                    TopNewPoint = GetCoordinate(Result);
                    Result = GetBottomLinePosition(NewX);
                    carryingerroraround = carryingerroraround || Result.ErrorPresent;
                    BottomNewPoint = GetCoordinate(Result);

                    }
                //now have got two points within range. It's time to find the lengths of
                //the lines between them
                OldToptoOldBottom = DistanceBetweenPoints(TopOldPoint, BottomOldPoint);
                OldToptoNewTop = DistanceBetweenPoints(TopOldPoint,TopNewPoint);
                OldBottomtoNewTop = DistanceBetweenPoints(BottomOldPoint, TopNewPoint);
                OldToptoNewBottom = DistanceBetweenPoints(TopOldPoint, BottomNewPoint);
                OldBottomtoNewBottom = DistanceBetweenPoints(BottomOldPoint,BottomNewPoint);
                //now to solve the coordinate positions of the new points relative to
                //the old points with the line between the old points forming the local
                //y axis
                NewTop2D = SolveTriangle(OldBottomtoNewTop, OldToptoNewTop, OldToptoOldBottom);
                NewBottom2D = SolveTriangle(OldBottomtoNewBottom, OldToptoNewBottom, OldToptoOldBottom);
                //these new co-ordinates must be rotated about the Z axis so that the
                //local y axis co-incides with the OldTop to OldBottom line
                if (fabs((TopLineAft[TopLineAft.Length - 1].x - BottomLineAft[BottomLineAft.Length - 1].x)/OldBottomtoNewBottom) > 1) {
                    AnsiString Message = "TopLineAft[TopLineAft.Length - 1].x = " + AnsiString(TopLineAft[TopLineAft.Length - 1].x) +
                                        "\nBottomLineAft[BottomLineAft.Length - 1].x = " + AnsiString(BottomLineAft[BottomLineAft.Length - 1].x) +
                                        "\nOldBottomtoNewBottom = " + AnsiString(OldBottomtoNewBottom) +
                                        "\nloopcount = " + AnsiString(loopcount);
                    Message.~AnsiString();
                    theta = 0;
                    } else {
                    theta = (TopLineAft[TopLineAft.Length - 1].x - BottomLineAft[BottomLineAft.Length - 1].x)/DistanceBetweenPoints(TopLineAft[TopLineAft.Length - 1],BottomLineAft[BottomLineAft.Length - 1]);
                    theta = -asin(theta);
                    }
                if (fabs(theta) > Accuracy) {
                    //rotation about Z;
                    MatrixZ.aa = cos(theta);
                    MatrixZ.ba = sin(theta);
                    MatrixZ.ab = -sin(theta);
                    MatrixZ.bb = cos(theta);

                    //apply matrix rotation
                    NewTop2D = MatrixMultiply(MatrixZ,NewTop2D);
                    NewBottom2D = MatrixMultiply(MatrixZ,NewBottom2D);
                    }

                //now to translate the point
                NewTop2D = VectorTranslate(BottomLineAft[BottomLineAft.Length - 1], NewTop2D);
                NewBottom2D = VectorTranslate(BottomLineAft[BottomLineAft.Length - 1], NewBottom2D);
                //Add the results to the developed lines
                TopLineAft.Length++;
                TopLineAft[TopLineAft.Length - 1] = NewTop2D;
                BottomLineAft.Length++;
                BottomLineAft[BottomLineAft.Length - 1] = NewBottom2D;

                //finish the while statement to ensure that the loop will eventually
                //end
                } //end of the bit missed if the bottom line can't be found
            } //end of the bit missed if the top line cant be found
        if (!(loopcount % 100)) {
            printf(("Developing Segment " + AnsiString(loopcount) + "\n").c_str());
            }
        loopcount++;
        OldX = NewX;
        TopOldPoint = TopNewPoint;
        BottomOldPoint = BottomNewPoint;
        }
    }
    catch (...) {
        ErrorListing->Add("An array error occurred in the development of the Aft part of the Plate at X = " + AnsiString(NewX));
        }
    }

    __finally {
    //now to close out the profile at the aft end
    if (TopLineAft[TopLineAft.Length - 1].x != BottomLineAft[BottomLineAft.Length - 1].x || TopLineAft[TopLineAft.Length - 1].y != BottomLineAft[BottomLineAft.Length - 1].y) {
        TopLineAft.Length++;
        TopLineAft[TopLineAft.Length - 1] = BottomLineAft[BottomLineAft.Length - 1];
        }
    //that's the aft end profile complete, so now to go on to the fwd end
    if (Stop) {
        ErrorListing->Add("Development of the Aft part of the plate stopped at Segment " + AnsiString(loopcount) + "\t\tX = " + AnsiString(NewX));
        }
    }


    TopOldPoint = StartLine[0];
    BottomOldPoint = StartLine[1];
    OldX = StartX;

    try {
    try {
    while (OldX > LineExtents.xmin && !Stop ) {
        NewX = OldX - stepsize;
        if (NewX < LineExtents.xmin) NewX = LineExtents.xmin;
        Result = GetTopLinePosition(NewX);
        if (Result.ErrorPresent) {
            ErrorListing->Add("Developing Plate, Segment " + AnsiString(loopcount));
            ErrorListing->Add("Can't Find the Top Line Position");
            ErrorListing->Add("Error Message = " + Result.ErrorMessage);
            } else {
            TopNewPoint = GetCoordinate(Result);
            Result = GetBottomLinePosition(NewX);
            if (Result.ErrorPresent) {
                ErrorListing->Add("Developing Plate, Segment " + AnsiString(loopcount));
                ErrorListing->Add("Can't Find the Bottom Line Position");
                ErrorListing->Add("Error Message = " + Result.ErrorMessage);
                } else {
                BottomNewPoint = GetCoordinate(Result);
                tempstepsize = stepsize;
                while (DistanceBetweenPoints(TopOldPoint,TopNewPoint) > maximumsegmentlength ||  DistanceBetweenPoints(BottomOldPoint,BottomNewPoint) > maximumsegmentlength) {
                    tempstepsize = tempstepsize/2;
                    NewX = OldX - tempstepsize;
                    Result = GetTopLinePosition(NewX);
                    TopNewPoint = GetCoordinate(Result);
                    Result = GetBottomLinePosition(NewX);
                    BottomNewPoint = GetCoordinate(Result);
                    }
                //now have got two points within range. It's time to find the lengths of
                //the lines between them
                OldToptoOldBottom = DistanceBetweenPoints(TopOldPoint, BottomOldPoint);
                OldToptoNewTop = DistanceBetweenPoints(TopOldPoint,TopNewPoint);
                OldBottomtoNewTop = DistanceBetweenPoints(BottomOldPoint, TopNewPoint);
                OldToptoNewBottom = DistanceBetweenPoints(TopOldPoint, BottomNewPoint);
                OldBottomtoNewBottom = DistanceBetweenPoints(BottomOldPoint,BottomNewPoint);
                //now to solve the coordinate positions of the new points relative to
                //the old points with the line between the old points forming the local
                //y axis
                NewTop2D = SolveTriangle(OldBottomtoNewTop, OldToptoNewTop, OldToptoOldBottom);
                NewTop2D.x = -NewTop2D.x;
                NewBottom2D = SolveTriangle(OldBottomtoNewBottom, OldToptoNewBottom, OldToptoOldBottom);
                NewBottom2D.x = -NewBottom2D.x;
                //these new co-ordinates must be rotated about the Z axis so that the
                //local y axis co-incides with the OldTop to OldBottom line
                if (fabs((TopLineAft[TopLineAft.Length - 1].x - BottomLineAft[BottomLineAft.Length - 1].x)/DistanceBetweenPoints(TopLineFwd[TopLineFwd.Length - 1],BottomLineFwd[BottomLineFwd.Length - 1])) > 1) {
                    AnsiString Message = "TopLineAft[TopLineAft.Length - 1].x = " + AnsiString(TopLineAft[TopLineAft.Length - 1].x) +
                                        "\nBottomLineAft[BottomLineAft.Length - 1].x = " + AnsiString(BottomLineAft[BottomLineAft.Length - 1].x) +
                                        "\nOldBottomtoNewBottom = " + AnsiString(OldBottomtoNewBottom) +
                                        "\nloopcount = " + AnsiString(loopcount);
                    //Application->MessageBox(Message.c_str(),"Angle Error", MB_OK + MB_ICONERROR);
                    Message.~AnsiString();
                    theta = 0;
                    ErrorListing->Add("Angle Error in Segment " + AnsiString(loopcount));
                    } else {
                    theta = (TopLineFwd[TopLineFwd.Length - 1].x - BottomLineFwd[BottomLineFwd.Length - 1].x)/DistanceBetweenPoints(TopLineFwd[TopLineFwd.Length - 1],BottomLineFwd[BottomLineFwd.Length - 1]);
                    theta = -asin(theta);
                    }
                if (fabs(theta) > Accuracy) {
                    //rotation about Z;
                    MatrixZ.aa = cos(theta);
                    MatrixZ.ba = sin(theta);
                    MatrixZ.ab = -sin(theta);
                    MatrixZ.bb = cos(theta);

                    //apply matrix rotation
                    NewTop2D = MatrixMultiply(MatrixZ,NewTop2D);
                    NewBottom2D = MatrixMultiply(MatrixZ,NewBottom2D);
                    }

                //now to translate the point
                NewTop2D = VectorTranslate(BottomLineFwd[BottomLineFwd.Length - 1], NewTop2D);
                NewBottom2D = VectorTranslate(BottomLineFwd[BottomLineFwd.Length - 1], NewBottom2D);
                //Add the results to the developed lines
                TopLineFwd.Length++;
                TopLineFwd[TopLineFwd.Length - 1] = NewTop2D;
                BottomLineFwd.Length++;
                BottomLineFwd[BottomLineFwd.Length - 1] = NewBottom2D;
                } //end of the bit missed if the bottom line can't be found
            } //end of the bit missed if the top line can't be found
        //finish the while statement to ensure that the loop will eventually
        //end
        if (!(loopcount % 100)) {
            printf(("Developing Segment " + AnsiString(loopcount) + "\n").c_str());
            }
        loopcount++;
        OldX = NewX;
        TopOldPoint = TopNewPoint;
        BottomOldPoint = BottomNewPoint;
        }
    }
    catch (...) {
        ErrorListing->Add("An array error occurred in the development of the Fwd part of the Plate at X = " + AnsiString(NewX));
        }
    }

    __finally {
    //now to close out the profile at the fwd end
    if (TopLineFwd[TopLineFwd.Length - 1].x != BottomLineFwd[BottomLineFwd.Length - 1].x || TopLineFwd[TopLineFwd.Length - 1].y != BottomLineFwd[BottomLineFwd.Length - 1].y) {
        TopLineFwd.Length++;
        TopLineFwd[TopLineFwd.Length - 1] = BottomLineFwd[BottomLineFwd.Length - 1];
        }
    //that's the fwd end profile complete, so now it's developed !!!!!!!!!
    if (Stop) {
        if (NewX < StartLine[0].x) {
            ErrorListing->Add("Development of the Fwd part of the plate stopped at Segment " + AnsiString(loopcount) + "\t\tX = " + AnsiString(NewX));
            }
        }
    Stop = false;
    }

}
//---------------------------------------------------------------------------

void CreateandDisplayDXFFile(void)
{


    //create the DXF file and display it
    WriteInitialLines();

    for (int i = 0; i < TopLineAft.Length - 1; i++) WriteLine(TopLineAft[i],TopLineAft[i+1]);
    for (int i = 0; i < TopLineFwd.Length - 1; i++) WriteLine(TopLineFwd[i],TopLineFwd[i+1]);
    for (int i = 0; i < BottomLineAft.Length - 1; i++) WriteLine(BottomLineAft[i],BottomLineAft[i+1]);
    for (int i = 0; i < BottomLineFwd.Length - 1; i++) WriteLine(BottomLineFwd[i],BottomLineFwd[i+1]);

    WriteFinalLines();

        printf("Writing DXF file\n");

        if (!FileName.Pos(EndOfFileName)) FileName = ChangeFileExt(FileName, EndOfFileName);
        OutputSave(FileName);
        printf(("DXF file saved as " + FileName+ "\n").c_str());

        //finishing
        printf("Clearing Memory\n");
        ClearLists();



}
//---------------------------------------------------------------------------

point SolveTriangle(double FirstSide, double SecondSide, double ThirdSide)
{
    //this routine takes the lengths of a triangle and returns the co-ordinate
    //of the end of line FirstSide in terms of X and Y relative to the start of
    //FirstSide, assuming that ThirdSide is in the local Y direction

    point Coordinate;
    double s = (FirstSide + SecondSide + ThirdSide) / 2;
    double S = sqrt(s * (s - FirstSide) * (s - SecondSide) * (s - ThirdSide));
    double f;

    Coordinate.x = 2 * S / ThirdSide;
    if (FirstSide > Coordinate.x) {
        Coordinate.y = sqrt(FirstSide * FirstSide - Coordinate.x * Coordinate.x);
        } else {
        Coordinate.y = -sqrt(Coordinate.x * Coordinate.x - FirstSide * FirstSide);
        }
    if (SecondSide > Coordinate.x) {
        f =  sqrt(SecondSide * SecondSide - Coordinate.x * Coordinate.x);
        } else {
        f =  sqrt(Coordinate.x * Coordinate.x - SecondSide * SecondSide);
        }

    if (fabs(f + Coordinate.y - ThirdSide) > Accuracy && SecondSide > FirstSide) Coordinate.y = -Coordinate.y;

    return Coordinate;

}
//---------------------------------------------------------------------------

point MatrixMultiply(TwoDMatrix Matrix1, point Vector1)
{
    point combined;

    combined.x = Matrix1.aa * Vector1.x + Matrix1.ba * Vector1.y;
    combined.y = Matrix1.ab * Vector1.x + Matrix1.bb * Vector1.y;


    return combined;

}
//---------------------------------------------------------------------------

void WriteInitialLines(void)
{
        OutputAdd("999");
        OutputAdd("DXF File Produced by the R.W.W. Plate Development Program");
        OutputAdd("999");
        OutputAdd("Program Copyright R.W.Williams");
        OutputAdd("999");
        OutputAdd("\tE-Mail richardandtracy@yahoo.com");
        OutputAdd(" 0");
        OutputAdd("SECTION");
        OutputAdd(" 2");
        OutputAdd("ENTITIES");
}
//---------------------------------------------------------------------------

void WriteLine(point Point1, point Point2)
{
    static int entitynumber = 1;
    OutputAdd(" 0");
    OutputAdd("LINE");
    OutputAdd(" 8");
    OutputAdd("Developed_Plate");
    OutputAdd(" 5");
    OutputAdd(" " + IntToHex(entitynumber++,4));
    OutputAdd(" 10");
    OutputAdd(FloatToStrF(Point1.x, ffGeneral, 12, 2));
    OutputAdd(" 20");
    OutputAdd(FloatToStrF(Point1.y, ffGeneral, 12, 2));
    OutputAdd(" 30");
    OutputAdd("0.00000");
    OutputAdd(" 11");
    OutputAdd(FloatToStrF(Point2.x, ffGeneral, 12, 2));
    OutputAdd(" 21");
    OutputAdd(FloatToStrF(Point2.y, ffGeneral, 12, 2));
    OutputAdd(" 31");
    OutputAdd("0.00000");

}
//---------------------------------------------------------------------------

void WriteFinalLines(void)
{
        OutputAdd(" 0");
        OutputAdd("ENDSEC");
        OutputAdd(" 0");
        OutputAdd("EOF");
}
//---------------------------------------------------------------------------

point VectorTranslate(point vector1, point vector2)
{
    //carries out vector addition
    vector1.x = vector1.x + vector2.x;
    vector1.y = vector1.y + vector2.y;

    return vector1;
}
//---------------------------------------------------------------------------

double DistanceBetweenPoints(point node1, point node2)
{
    double length = (node2.x - node1.x) * (node2.x - node1.x);
    length += (node2.y - node1.y) * (node2.y - node1.y);
    length = sqrt(length);

    return length;
}
//---------------------------------------------------------------------------

nodalposition GetCoordinate(ErrorProneResult Result)
{
    nodalposition Coordinate;

    Coordinate.x = Result.x;
    Coordinate.y = Result.y;
    Coordinate.z = Result.z;

    return Coordinate;

}
//---------------------------------------------------------------------------

void DisplayErrors(void)
{
    //this method shows the errors generated during the development
    if (!ErrorListing->Count) ErrorListing->Add("No Mathematical Errors were encountered");

    for (int i = 0; i < ErrorListing->Count; i++) {
        printf((ErrorListing->Strings[i] + "\n").c_str());
        }

    ErrorListing->Clear();

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

void SplitLongLines(void)
{
    //this method makes sure that no lines extend from one end to the other
    //of the model.
    int i = 0;

    while (i < Vertices.Length) {
        if (Vertices[i].Length && fabs((Vertices[i][Vertices[i].Length - 1].x - Vertices[i][0].x)/(ModelExtents.xmax - ModelExtents.xmin)) > 0.7) {
            //identified a long line, now split it in two
            //deal differently depending on number of points in line
            switch (Vertices[i].Length) {
                case 0:
                case 1:     break;
                case 2:     {
                            //it's a straight line. Break the line in the middle
                            nodalposition splitpoint;
                            splitpoint.x = (Vertices[i][1].x + Vertices[i][0].x) / 2.0;
                            splitpoint.y = (Vertices[i][1].y + Vertices[i][0].y) / 2.0;
                            splitpoint.z = (Vertices[i][1].z + Vertices[i][0].z) / 2.0;
                            Vertices.Length++;
                            Vertices[Vertices.Length - 1].Length = 2;
                            Vertices[Vertices.Length - 1][0] = splitpoint;
                            Vertices[Vertices.Length - 1][1] = Vertices[i][1];
                            Vertices[i][1] = splitpoint;
                            }
                            break;
                case 3:     {
                            //its a quadratic line. Break the line in the middle
                            nodalposition splitpoint;
                            splitpoint.x = (Vertices[i][2].x + Vertices[i][0].x) / 2.0;
                            LineSegment.Length = 3;
                            for (int j = 0; j < 3; j++) {
                                LineSegment[j] = Vertices[i][j];
                                }
                            line_equation templine = GetQuadraticLineEquation();
                            ErrorProneResult tempcoord = GetCoord(splitpoint.x,templine);
                            splitpoint.y = tempcoord.y;
                            splitpoint.z = tempcoord.z;
                            //now to split the line. It will go 2:1 or 1:2 to
                            //the new line
                            Vertices.Length++;
                            Vertices[Vertices.Length - 1].Length = 3;
                            if (Vertices[i][1].x < splitpoint.x) {
                                //2 points remain in old line
                                Vertices[Vertices.Length - 1][0] = splitpoint;
                                Vertices[Vertices.Length - 1][2] = Vertices[i][2];
                                Vertices[i][2] = splitpoint;
                                splitpoint.x = (Vertices[i][2].x + splitpoint.x) / 2.0;
                                tempcoord = GetCoord(splitpoint.x,templine);
                                Vertices[Vertices.Length - 1][1].x = tempcoord.x;
                                Vertices[Vertices.Length - 1][1].y = tempcoord.y;
                                Vertices[Vertices.Length - 1][1].z = tempcoord.z;
                                } else {
                                //1 point remains in old line
                                Vertices[Vertices.Length - 1][0] = splitpoint;
                                Vertices[Vertices.Length - 1][1] = Vertices[i][1];
                                Vertices[Vertices.Length - 1][2] = Vertices[i][2];
                                Vertices[i][2] = splitpoint;
                                splitpoint.x = (Vertices[i][0].x + splitpoint.x) / 2.0;
                                tempcoord = GetCoord(splitpoint.x,templine);
                                Vertices[i][1].x = tempcoord.x;
                                Vertices[i][1].y = tempcoord.y;
                                Vertices[i][1].z = tempcoord.z;
                                }
                            LineSegment.Length = 0;
                            }
                            break;
                default:    {
                            // it's a line with more points, still break it in
                            //the middle
                            nodalposition splitpoint;
                            splitpoint.x = (Vertices[i][Vertices[i].Length - 1].x + Vertices[i][0].x)/2.0;
                            Vertices.Length++;
                            bool firstbigger = false;
                            int pointertofirstbigger;
                            for (int j = 0; j < Vertices[i].Length; j++) {
                                if (Vertices[i][j].x > splitpoint.x) {
                                    if (!firstbigger) {
                                        firstbigger = true;
                                        pointertofirstbigger = j;
                                        }
                                    }
                                }
                            if (firstbigger) {
                                //now check where the pointer is, beginning,
                                //middle & ends of lines have to be treated
                                //differently
                                LineSegment.Length = 4;
                                line_equation templine;
                                ErrorProneResult tempcoord;
                                //Vertices.Length++;
                                //really want to use a switch here, but the case
                                //only takes a constant expression.
                                if (pointertofirstbigger == 1) {
                                    //this segment's at the beginning of the line
                                    for (int j = 0; j < 4; j++) {
                                        LineSegment[j] = Vertices[i][j];
                                        }
                                    templine = GetLineEquation();
                                    tempcoord = GetCoord(splitpoint.x,templine);
                                    splitpoint.y = tempcoord.y;
                                    splitpoint.z = tempcoord.z;
                                    Vertices[Vertices.Length - 1].Length = 4;
                                    Vertices[Vertices.Length - 1][0] = Vertices[i][0];
                                    Vertices[Vertices.Length - 1][3] = splitpoint;
                                    Vertices[i][0] = splitpoint;
                                    for (int j = 1; j < 3; j++) {
                                        tempcoord = GetCoord((Vertices[Vertices.Length - 1][0].x + (double)j * (splitpoint.x - Vertices[Vertices.Length - 1][0].x)/3),templine);
                                        Vertices[Vertices.Length - 1][j].x = tempcoord.x;
                                        Vertices[Vertices.Length - 1][j].y = tempcoord.y;
                                        Vertices[Vertices.Length - 1][j].z = tempcoord.z;
                                        }
                                    } else {
                                    if (pointertofirstbigger == 2) {
                                        //this segment's at the beginning of the line too, but we're going to discard point Vertices[i][1]
                                        for (int j = 0; j < 4; j++) {
                                            LineSegment[j] = Vertices[i][j];
                                            }
                                        templine = GetLineEquation();
                                        tempcoord = GetCoord(splitpoint.x,templine);
                                        splitpoint.y = tempcoord.y;
                                        splitpoint.z = tempcoord.z;
                                        Vertices[Vertices.Length - 1].Length = 4;
                                        Vertices[Vertices.Length - 1][0] = Vertices[i][0];
                                        Vertices[Vertices.Length - 1][3] = splitpoint;
                                        Vertices[i][1] = splitpoint;
                                        for (int j = 1; j < 3; j++) {
                                            tempcoord = GetCoord((Vertices[Vertices.Length - 1][0].x + (double)j * (splitpoint.x - Vertices[Vertices.Length - 1][0].x)/3),templine);
                                            Vertices[Vertices.Length - 1][j].x = tempcoord.x;
                                            Vertices[Vertices.Length - 1][j].y = tempcoord.y;
                                            Vertices[Vertices.Length - 1][j].z = tempcoord.z;
                                            }
                                        for (int j = 1; j < Vertices[i].Length - 1; j++) {
                                            Vertices[i][j - 1] = Vertices[i][j];
                                            }
                                        Vertices[i].Length--;
                                        } else {
                                        if (pointertofirstbigger == Vertices[i].Length - 1) {
                                            //it's the end of the line, treat similarly to when pointertofirstbigger == 1
                                            for (int j = Vertices[i].Length - 4; j < Vertices[i].Length; j++) {
                                                LineSegment[j] = Vertices[i][j];
                                                }
                                            templine = GetLineEquation();
                                            tempcoord = GetCoord(splitpoint.x,templine);
                                            splitpoint.y = tempcoord.y;
                                            splitpoint.z = tempcoord.z;
                                            Vertices[Vertices.Length - 1].Length = 4;
                                            Vertices[Vertices.Length - 1][3] = Vertices[i][Vertices[i].Length - 1];
                                            Vertices[Vertices.Length - 1][0] = splitpoint;
                                            Vertices[i][Vertices[i].Length - 1] = splitpoint;
                                            for (int j = 1; j < 3; j++) {
                                                tempcoord = GetCoord((Vertices[Vertices.Length - 1][0].x + (double)j * (Vertices[Vertices.Length - 1][3].x - Vertices[Vertices.Length - 1][0].x)/3),templine);
                                                Vertices[Vertices.Length - 1][j].x = tempcoord.x;
                                                Vertices[Vertices.Length - 1][j].y = tempcoord.y;
                                                Vertices[Vertices.Length - 1][j].z = tempcoord.z;
                                                }
                                            } else {
                                            if (pointertofirstbigger == Vertices[i].Length - 2) {
                                                //end of line. Treat similarly
                                                //to pointoffirstbigger == 2 and
                                                //discard point Vertices[i][Vertices[i].Length - 2]
                                                for (int j = Vertices[i].Length - 4; j < Vertices[i].Length; j++) {
                                                    LineSegment[j] = Vertices[i][j];
                                                    }
                                                templine = GetLineEquation();
                                                tempcoord = GetCoord(splitpoint.x,templine);
                                                splitpoint.y = tempcoord.y;
                                                splitpoint.z = tempcoord.z;
                                                Vertices[Vertices.Length - 1].Length = 4;
                                                Vertices[Vertices.Length - 1][3] = Vertices[i][Vertices[i].Length - 1];
                                                Vertices[Vertices.Length - 1][0] = splitpoint;
                                                Vertices[i][Vertices[i].Length - 2] = splitpoint;
                                                for (int j = 1; j < 3; j++) {
                                                    tempcoord = GetCoord((Vertices[Vertices.Length - 1][0].x + (double)j * (Vertices[Vertices.Length - 1][3].x - Vertices[Vertices.Length - 1][0].x)/3),templine);
                                                    Vertices[Vertices.Length - 1][j].x = tempcoord.x;
                                                    Vertices[Vertices.Length - 1][j].y = tempcoord.y;
                                                    Vertices[Vertices.Length - 1][j].z = tempcoord.z;
                                                    }
                                                Vertices[i].Length--;
                                                } else {
                                                //at last, final option. Now to
                                                //simply split line at
                                                //splitpoint.x
                                                for (int j = pointertofirstbigger - 2; j < pointertofirstbigger + 2; j++) {
                                                    LineSegment[j - pointertofirstbigger + 2] = Vertices[i][j];
                                                    }
                                                templine = GetLineEquation();
                                                tempcoord = GetCoord(splitpoint.x,templine);
                                                splitpoint.y = tempcoord.y;
                                                splitpoint.z = tempcoord.z;
                                                Vertices[Vertices.Length - 1].Length = Vertices[i].Length - pointertofirstbigger + 1;
                                                int pointer = 1;
                                                for (int j = pointertofirstbigger; j < Vertices[i].Length; j++) {
                                                    Vertices[Vertices.Length - 1][pointer] = Vertices[i][j];
                                                    pointer++;
                                                    }
                                                Vertices[Vertices.Length - 1][0] = splitpoint;
                                                Vertices[i][pointertofirstbigger] = splitpoint;
                                                Vertices[i].Length = pointertofirstbigger + 1;
                                                }
                                            }
                                        }
                                    }
                                LineSegment.Length = 0;
                                }
                            }
                            break;
                }
            }
        i++;
        }
}
//---------------------------------------------------------------------------

ErrorProneResult GetCoord(double x, line_equation Equation)
{

    ErrorProneResult Coordinate;
    Coordinate.x = x;
    Coordinate.y = Equation.B0 + Equation.B1 * x + Equation.B2 * x * x + Equation.B3 * x * x * x;
    Coordinate.z = Equation.C0 + Equation.C1 * x + Equation.C2 * x * x + Equation.C3 * x * x * x;
    Coordinate.ErrorPresent = false;
    Coordinate.ErrorMessage = "";

    return Coordinate;

}
//---------------------------------------------------------------------------


