REM (This program calculates gear and gearcutter dimensions.)
WIDTH 40
CLS
        COLOR 14
1       LOCATE 6, 3: PRINT "JOE'S DATA PROGRAM TO HELP YOU MAKE "
        LOCATE 7, 3: PRINT "DP AND METRIC MODULE GEAR CUTTERS"
        LOCATE 8, 3: PRINT "USING THE SPACED PIN CUTTER METHOD."
        COLOR 10
        LOCATE 12, 3: PRINT "[To be used in conjunction with the data as"
        LOCATE 13, 2: PRINT "published in MODEL ENGINEERING WORKSHOP Number 41 dated April 1997.]"
        COLOR 14
        LOCATE 23, 1: INPUT " TO CONTINUE PRESS 1 AND ENTER. TO EXIT               JUST ENTER "; joe
        IF joe = 1 GOTO 2 ELSE GOTO 1820
2       COLOR 12
10      CLS
        LET TANDE1 = 0
        LET TANDE2 = 0
        LET TANDE3 = 0
        LET TANDE4 = 0
        LET TANDE5 = 0
        LET TANDE6 = 0
        LET TANDE7 = 0
        LET TANDE8 = 0
        LET TANDE9 = 0
        LET TANDE10 = 0
        LOCATE 10, 2: PRINT " MODULE (ENTER 1) OR DP (ENTER 2) TYPE?"
        LOCATE 13, 8: INPUT " 1(Module) or 2(Dp)"; tipe
        IF tipe = 1 GOTO 1010 ELSE GOTO 7
7       IF tipe = 2 GOTO 8 ELSE GOTO 2
8       CLS
        COLOR 10
        LOCATE 8, 7: PRINT " DIAMETRAL PITCH SYSTEM"
        COLOR 12
        LOCATE 16, 2: PRINT " WHICH PRESSURE ANGLE DO YOU WANT?"
        LOCATE 19, 6: INPUT " 14.5, 20 OR 30 degrees "; presang
        CLS
9       IF presang = 14.5 GOTO 13 ELSE GOTO 11
11      IF presang = 20 GOTO 311 ELSE GOTO 12
12      IF presang = 30 GOTO 611 ELSE GOTO 8
13      COLOR 10
        LOCATE 2, 6: PRINT "14.5 DEGREES PRESSURE ANGLE"
        COLOR 12
        LOCATE 5, 4: INPUT " How many teeth do you want"; TANDE
        LOCATE 7, 2: INPUT " What diametral pitch do you want"; dp
        IF dp = 0 GOTO 1710
        module = 25.4 / dp
        IF TANDE >= 135 THEN TANDE1 = 135
        IF TANDE <= 134 AND TANDE >= 55 THEN LET TANDE2 = 55
        IF TANDE <= 54 AND TANDE >= 35 THEN LET TANDE3 = 35
        IF TANDE <= 34 AND TANDE >= 26 THEN LET TANDE4 = 26
        IF TANDE <= 25 AND TANDE >= 21 THEN LET TANDE5 = 21
        IF TANDE <= 20 AND TANDE >= 17 THEN LET TANDE6 = 17
        IF TANDE <= 16 AND TANDE >= 14 THEN LET TANDE7 = 14
        IF TANDE <= 14 AND TANDE >= 13 THEN LET TANDE8 = 13
        IF TANDE <= 12 AND TANDE >= 10 THEN LET TANDE9 = 10
        IF TANDE <= 10 AND TANDE >= 0 THEN GOTO 1710 ELSE GOTO 120
120     IF TANDE1 = 135 THEN GOTO 121 ELSE GOTO 130
121     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER ="; module * 33.8; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES ="; module * 34.2; "mm"
        LOCATE 14, 12: PRINT "IN FEED ="; module * 4.42; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH ="; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
130     IF TANDE2 = 55 THEN GOTO 131 ELSE GOTO 140
131     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 13.77; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 14.81; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 3.61; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
140     IF TANDE3 = 35 THEN GOTO 141 ELSE GOTO 150
141     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 8.76; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 9.96; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 3.18; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
150     IF TANDE4 = 26 THEN GOTO 151 ELSE GOTO 160
151     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 6.51; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 7.78; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.9; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
160     IF TANDE5 = 21 THEN GOTO 161 ELSE GOTO 170
161     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 5.26; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 6.57; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.7; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
170     IF TANDE6 = 17 THEN GOTO 171 ELSE GOTO 180
171     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 4.26; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 5.61; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.51; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
180     IF TANDE7 = 14 THEN GOTO 181 ELSE GOTO 190
181     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 3.51; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 4.88; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.33; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
190     IF TANDE8 = 12 THEN GOTO 191 ELSE GOTO 200
191     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 3!; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 4.4; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.19; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
200     IF TANDE9 = 10 THEN GOTO 201 ELSE GOTO 1810
201     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 2.5; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 3.92; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.01; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
311     COLOR 10
        LOCATE 2, 6: PRINT "20 DEGREE PRESSURE ANGLE"
        COLOR 12
        LOCATE 5, 4: INPUT " How many teeth do you want"; TANDE
        LOCATE 7, 2: INPUT " What diametral pitch do you want"; dp
        IF dp = 0 GOTO 1710
        module = 25.4 / dp
        IF TANDE >= 135 THEN TANDE1 = 135
        IF TANDE <= 134 AND TANDE >= 55 THEN LET TANDE2 = 55
        IF TANDE <= 54 AND TANDE >= 35 THEN LET TANDE3 = 35
        IF TANDE <= 34 AND TANDE >= 26 THEN LET TANDE4 = 26
        IF TANDE <= 25 AND TANDE >= 21 THEN LET TANDE5 = 21
        IF TANDE <= 20 AND TANDE >= 17 THEN LET TANDE6 = 17
        IF TANDE <= 16 AND TANDE >= 14 THEN LET TANDE7 = 14
        IF TANDE <= 14 AND TANDE >= 13 THEN LET TANDE8 = 13
        IF TANDE <= 12 AND TANDE >= 10 THEN LET TANDE9 = 10
        IF TANDE <= 10 AND TANDE >= 0 THEN GOTO 1710 ELSE GOTO 420
420     IF TANDE1 = 135 THEN GOTO 421 ELSE GOTO 430
421     COLOR 14
        LOCATE 12, 9: PRINT "PIN DIAMETER ="; module * 46.17; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES ="; module * 44.78; "mm"
        LOCATE 14, 12: PRINT "IN FEED ="; module * 3.93; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH ="; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
430     IF TANDE2 = 55 THEN GOTO 431 ELSE GOTO 440
431     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 18.81; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 19.07; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 3.42; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
440     IF TANDE3 = 35 THEN GOTO 441 ELSE GOTO 450
441     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 11.97; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 12.64; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 3.1; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
450     IF TANDE4 = 26 THEN GOTO 451 ELSE GOTO 460
451     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 8.89; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 9.75; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.87; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
460     IF TANDE5 = 21 THEN GOTO 461 ELSE GOTO 470
461     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 7.18; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 8.15; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.71; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
470     IF TANDE6 = 17 THEN GOTO 471 ELSE GOTO 480
471     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 5.81; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 6.86; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.54; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
480     IF TANDE7 = 14 THEN GOTO 481 ELSE GOTO 490
481     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 4.79; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 5.9; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.39; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
490     IF TANDE8 = 12 THEN GOTO 491 ELSE GOTO 500
491     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 4.1; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 5.27; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.26; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
500     IF TANDE9 = 10 THEN GOTO 501 ELSE GOTO 1810
501     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 3.42; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 4.63; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.11; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
611     COLOR 10
        LOCATE 2, 6: PRINT "30 DEGREE PRESSURE ANGLE"
        LOCATE 5, 4: INPUT " How many teeth do you want"; TANDE
        LOCATE 7, 2: INPUT " What diametral pitch do you want"; dp
        IF dp = 0 GOTO 1710
        module = 25.4 / dp
        IF TANDE >= 135 THEN TANDE1 = 135
        IF TANDE <= 134 AND TANDE >= 55 THEN LET TANDE2 = 55
        IF TANDE <= 54 AND TANDE >= 35 THEN LET TANDE3 = 35
        IF TANDE <= 34 AND TANDE >= 26 THEN LET TANDE4 = 26
        IF TANDE <= 25 AND TANDE >= 21 THEN LET TANDE5 = 21
        IF TANDE <= 20 AND TANDE >= 17 THEN LET TANDE6 = 17
        IF TANDE <= 16 AND TANDE >= 14 THEN LET TANDE7 = 14
        IF TANDE <= 14 AND TANDE >= 13 THEN LET TANDE8 = 13
        IF TANDE <= 12 AND TANDE >= 10 THEN LET TANDE9 = 10
        IF TANDE <= 10 AND TANDE >= 0 THEN GOTO 1710

720     IF TANDE1 = 135 THEN GOTO 721 ELSE GOTO 730
721     COLOR 14
        LOCATE 12, 9: PRINT "PIN DIAMETER ="; module * 67.5; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES ="; module * 59.64; "mm"
        LOCATE 14, 12: PRINT "IN FEED ="; module * 3.15; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH ="; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
730     IF TANDE2 = 55 THEN GOTO 731 ELSE GOTO 740
731     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 27.5; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 25!; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.92; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
740     IF TANDE3 = 35 THEN GOTO 741 ELSE GOTO 750
741     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 17.5; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 16.34; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.74; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
750     IF TANDE4 = 26 THEN GOTO 751 ELSE GOTO 760
751     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 13!; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 12.45; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.61; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
760     IF TANDE5 = 21 THEN GOTO 761 ELSE GOTO 770
761     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 10.5; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 10.28; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.5; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
770     IF TANDE6 = 17 THEN GOTO 771 ELSE GOTO 780
771     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 8.5; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 8.55; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.38; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
780     IF TANDE7 = 14 THEN GOTO 781 ELSE GOTO 790
781     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 7!; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 7.26; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.27; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
790     IF TANDE8 = 12 THEN GOTO 791 ELSE GOTO 800
791     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 6!; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 6.4; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.17; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
800     IF TANDE9 = 10 THEN GOTO 801 ELSE GOTO 1810
801     COLOR 14
        LOCATE 10, 10: PRINT "PIN DIAMETER"; module * 5!; "mm"
        LOCATE 12, 10: PRINT "PIN CENTRES"; module * 5.54; "mm"
        LOCATE 14, 12: PRINT "IN FEED"; module * 2.05; "mm"
        LOCATE 16, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 18, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 20, 11: PRINT "WHOLE TOOTH DEPTH ="; module * 2.2; "mm"
        GOTO 1810
REM (Start of program for module cutters.)
        WIDTH 40
1010    CLS
        LET TANDE1 = 0
        LET TANDE2 = 0
        LET TANDE3 = 0
        LET TANDE4 = 0
        LET TANDE5 = 0
        LET TANDE6 = 0
        LET TANDE7 = 0
        LET TANDE8 = 0
        LET TANDE9 = 0
        LET TANDE10 = 0
        COLOR 10
        LOCATE 8, 11: PRINT " METRIC MODULE SYSTEM"
        COLOR 12
        LOCATE 10, 4: PRINT " WHICH PRESSURE ANGLE DO YOU WANT?"
        LOCATE 13, 6: INPUT " 14.5, 20 OR 30 degrees "; presang
        CLS
        IF presang = 14.5 GOTO 1013 ELSE GOTO 1011
1011    IF presang = 20 GOTO 1311 ELSE GOTO 1012
1012    IF presang = 30 GOTO 1611
1013    COLOR 10
        LOCATE 2, 6: PRINT "14.5 DEGREES PRESSURE ANGLE"
        COLOR 12
        LOCATE 5, 4: INPUT " How many teeth do you want"; TANDE
        LOCATE 7, 2: INPUT " What metric module do you want"; module
        IF module = 0 GOTO 1710
        IF TANDE >= 135 THEN TANDE1 = 135
        IF TANDE <= 134 AND TANDE >= 55 THEN LET TANDE2 = 55
        IF TANDE <= 54 AND TANDE >= 35 THEN LET TANDE3 = 35
        IF TANDE <= 34 AND TANDE >= 26 THEN LET TANDE4 = 26
        IF TANDE <= 25 AND TANDE >= 21 THEN LET TANDE5 = 21
        IF TANDE <= 20 AND TANDE >= 17 THEN LET TANDE6 = 17
        IF TANDE <= 16 AND TANDE >= 14 THEN LET TANDE7 = 14
        IF TANDE <= 14 AND TANDE >= 13 THEN LET TANDE8 = 13
        IF TANDE <= 12 AND TANDE >= 10 THEN LET TANDE9 = 10
        IF TANDE <= 10 AND TANDE >= 0 THEN GOTO 1710 ELSE GOTO 1120
1120    IF TANDE1 = 135 THEN GOTO 1121 ELSE GOTO 1130
1121    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER ="; module * 33.8; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES ="; module * 34.2; "mm"
        LOCATE 16, 12: PRINT "IN FEED ="; module * 4.42; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH ="; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1130    IF TANDE2 = 55 THEN GOTO 1131 ELSE GOTO 1140
1131    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 13.77; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 14.81; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 3.61; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1140    IF TANDE3 = 35 THEN GOTO 1141 ELSE GOTO 1150
1141    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 8.76; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 9.96; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 3.18; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1150    IF TANDE4 = 26 THEN GOTO 1151 ELSE GOTO 1160
1151    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 6.51; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 7.78; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.9; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1160    IF TANDE5 = 21 THEN GOTO 1161 ELSE GOTO 1170
1161    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 5.26; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 6.57; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.7; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1170    IF TANDE6 = 17 THEN GOTO 1171 ELSE GOTO 1180
1171    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 4.26; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 5.61; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.51; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1180    IF TANDE7 = 14 THEN GOTO 1181 ELSE GOTO 1190
1181    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 3.51; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 4.88; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.33; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1190    IF TANDE8 = 12 THEN GOTO 1191 ELSE GOTO 1200
1191    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 3!; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 4.4; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.19; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1200 IF TANDE9 = 10 THEN GOTO 1201 ELSE GOTO 1810
1201    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 2.5; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 3.92; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.01; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1311    COLOR 10
        LOCATE 2, 6: PRINT "20 DEGREE PRESSURE ANGLE"
        COLOR 12
        LOCATE 5, 4: INPUT " How many teeth do you want"; TANDE
        LOCATE 7, 2: INPUT "What metric module do you want"; module
        IF module = 0 GOTO 1710
        IF TANDE >= 135 THEN TANDE1 = 135
        IF TANDE <= 134 AND TANDE >= 55 THEN LET TANDE2 = 55
        IF TANDE <= 54 AND TANDE >= 35 THEN LET TANDE3 = 35
        IF TANDE <= 34 AND TANDE >= 26 THEN LET TANDE4 = 26
        IF TANDE <= 25 AND TANDE >= 21 THEN LET TANDE5 = 21
        IF TANDE <= 20 AND TANDE >= 17 THEN LET TANDE6 = 17
        IF TANDE <= 16 AND TANDE >= 14 THEN LET TANDE7 = 14
        IF TANDE <= 14 AND TANDE >= 13 THEN LET TANDE8 = 13
        IF TANDE <= 12 AND TANDE >= 10 THEN LET TANDE9 = 10
        IF TANDE <= 10 AND TANDE >= 0 THEN GOTO 1710 ELSE GOTO 1420
1420    IF TANDE1 = 135 THEN GOTO 1421 ELSE GOTO 1430
1421    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER ="; module * 46.17; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES ="; module * 44.78; "mm"
        LOCATE 16, 12: PRINT "IN FEED ="; module * 3.93; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH ="; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1430    IF TANDE2 = 55 THEN GOTO 1431 ELSE GOTO 1440
1431    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 18.81; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 19.07; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 3.42; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1440    IF TANDE3 = 35 THEN GOTO 1441 ELSE GOTO 1450
1441    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 11.97; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 12.64; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 3.1; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1450    IF TANDE4 = 26 THEN GOTO 1451 ELSE GOTO 1460
1451    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 8.89; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 9.75; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.87; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1460    IF TANDE5 = 21 THEN GOTO 1461 ELSE GOTO 1470
1461    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 7.18; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 8.15; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.71; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1470    IF TANDE6 = 17 THEN GOTO 1471 ELSE GOTO 1480
1471    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 5.81; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 6.86; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.54; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1480    IF TANDE7 = 14 THEN GOTO 1481 ELSE GOTO 1490
1481    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 4.79; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 5.9; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.39; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1490    IF TANDE8 = 12 THEN GOTO 1491 ELSE GOTO 1500
1491    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 4.1; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 5.27; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.26; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1500    IF TANDE9 = 10 THEN GOTO 1501 ELSE GOTO 1810
1501    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 3.42; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 4.63; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.11; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1611    COLOR 10
        LOCATE 2, 6: PRINT "30 DEGREE PRESSURE ANGLE"
        LOCATE 5, 4: INPUT " How many teeth do you want"; TANDE
        LOCATE 7, 2: INPUT "What metric module do you want"; module
        IF module = 0 GOTO 1710
        IF TANDE >= 135 THEN TANDE1 = 135
        IF TANDE <= 134 AND TANDE >= 55 THEN LET TANDE2 = 55
        IF TANDE <= 54 AND TANDE >= 35 THEN LET TANDE3 = 35
        IF TANDE <= 34 AND TANDE >= 26 THEN LET TANDE4 = 26
        IF TANDE <= 25 AND TANDE >= 21 THEN LET TANDE5 = 21
        IF TANDE <= 20 AND TANDE >= 17 THEN LET TANDE6 = 17
        IF TANDE <= 16 AND TANDE >= 14 THEN LET TANDE7 = 14
        IF TANDE <= 14 AND TANDE >= 13 THEN LET TANDE8 = 13
        IF TANDE <= 12 AND TANDE >= 10 THEN LET TANDE9 = 10
        IF TANDE <= 10 AND TANDE >= 0 THEN GOTO 1710 ELSE GOTO 1720
1710    CLS
        COLOR 10
        LOCATE 14, 5: PRINT "NOT POSSIBLE - SPECIAL CUTTER"
        GOTO 1810
1720    IF TANDE1 = 135 THEN GOTO 1721 ELSE GOTO 1730
1721    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER ="; module * 67.5; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES ="; module * 59.64; "mm"
        LOCATE 16, 12: PRINT "IN FEED ="; module * 3.15; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH ="; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1730    IF TANDE2 = 55 THEN GOTO 1731 ELSE GOTO 1740
1731    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 27.5; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 25!; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.92; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1740    IF TANDE3 = 35 THEN GOTO 1741 ELSE GOTO 1750
1741    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 17.5; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 16.34; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.74; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        GOTO 1810
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
1750    IF TANDE4 = 26 THEN GOTO 1751 ELSE GOTO 1760
1751    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 13!; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 12.45; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.61; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1760    IF TANDE5 = 21 THEN GOTO 1761 ELSE GOTO 1770
1761    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 10.5; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 10.28; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.5; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1770    IF TANDE6 = 17 THEN GOTO 1771 ELSE GOTO 1780
1771    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 8.5; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 8.55; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.38; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1780    IF TANDE7 = 14 THEN GOTO 1781 ELSE GOTO 1790
1781    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 7!; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 7.26; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.27; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1790    IF TANDE8 = 12 THEN GOTO 791 ELSE GOTO 800
1791    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 6!; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 6.4; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.17; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1800    IF TANDE9 = 10 THEN GOTO 1801 ELSE GOTO 1810
1801    COLOR 14
        LOCATE 12, 10: PRINT "PIN DIAMETER"; module * 5!; "mm"
        LOCATE 14, 10: PRINT "PIN CENTRES"; module * 5.54; "mm"
        LOCATE 16, 12: PRINT "IN FEED"; module * 2.05; "mm"
        LOCATE 18, 11: PRINT "CUTTER WIDTH"; module * 4!; "mm"
        LOCATE 20, 7: PRINT "GEAR BLANK DIAMETER ="; (TANDE + 2) * module; "mm"
        LOCATE 22, 7: PRINT "WHOLE TOOTH DEPTH ="; 2.25 * module; "mm"
        GOTO 1810
1810    COLOR 10
        LOCATE 23, 5: INPUT "ARE YOU FINISHED  1=NO  2=YES"; KLAAR
        IF KLAAR = 1 GOTO 2 ELSE GOTO 1820
1820    END

