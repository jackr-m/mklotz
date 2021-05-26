# mklotz Rewrite
A python3 rewrite of Marv Klotz's classic programs.  Designed to work in any python 3 environment, including micropython.

These are designed to be close to a 1-1 copy of Marv Klotz's programs from http://www.myvirtualnetwork.com/mklotz/


## Currently converted programs:
- \MANUF\3WIRE
- \MANUF\BALLCUT
- \MANUF\BELT\PCD
- \MANUF\BELT\QBELT
- \MANUF\BEND
- \MANUF\BOLTCIRC
- \MANUF\CMITER
- \MANUF\COLLET
- \MANUF\COMPOUND
- \MANUF\CUSP
- \MANUF\DALLOW
- \MANUF\DIAM
- \MANUF\DIFFTHRD
- \MANUF\DOT
- \MANUF\DOVETAIL
- \MANUF\DPLATE
- \MANUF\ECCENT
- \MANUF\EXPAND
- \MANUF\FEED
- \MANUF\FITS
- \MANUF\GAGE
- \MANUF\GEAR
- \MANUF\GEARPA - PLEASE file a GitHub issue is you know the range of teeth for 25 or 30 deg pressure angle
- \MANUF\HORSE
- \MANUF\KNURL
- \MANUF\LOFT
- \MANUF\LVERN
- \MANUF\MANDREL
- \MANUF\MRADIUS
- \MANUF\RATTLE
- \MANUF\REVOLVER
- \MANUF\ROTARY
- \MANUF\ROUNDER
- \MANUF\SINEBAR (EVERYTHING DONE EXCEPT FOR FINDING THE INDIVIDUAL GAGE BLOCKS)
- \MANUF\SPEED
- \MANUF\SPROCKET\CHAIN
- \MANUF\SPROCKET\SPROCKET
- \MANUF\STICK
- \MANUF\TUBEWALL
- \MANUF\UGROOVE
- \MANUF\WIRE
- \MAUF\SLUG\PLATE
- \MAUF\SLUG\SLUG
- \MISC\TEMP
- \SUBS\CSK
- \SUBS\INCHTOMM
- \SUBS\INMMCONV
- \SUBS\KEYWAY
- \SUBS\OHM

### Programs to be converted soon:
- \MANUF\BELT\BELT.EXE
- \MANUF\CHANGE
- \MANUF\DIVHEAD
- \MANUF\MIXTURE
- \MANUF\VREV


### Challenge programs (will take a while to convert but are extremely useful):
- \MANUF\DRILL
- \MISC\UNIT
- \SUBS\SPRING
- \MANUF\FINDTHRD
- \MANUF\GEARFIND
- \MANUF\GEARATIO
- \MANUF\PROFILE
- \MANUF\WEIGHT
- \SUBS\CUTTER
    - Will do if I'm feeling like brushing up on my Visual Basic

## To-do List (for things other than converting programs):
-----
1. Create two directiories for short file names (DOS compatability) and long file names (more descriptive)
2. Compile python programs into a single .exe file to mimick mklotz's original programs
3. Create a list to help you find/figure out what each program is for.
4. Figure out how releases work
5. Add option to print result to the console or a file for the programs that normally only print to a file
6. Create branch for Beeware version of code

## FAQ:
----
- Q: Why?  What are you doing this for?
    - A: Fun.  These programs are of great utility to me (plenty for now, and lots of nice-to-haves for the future).  My ultimate goal is to have these programs in three main formats for myself and others.
        1. CLI.  As they exist right now, the programs are perfectly usable from a computer and command line.
        2. Calculator.  My calculator, TI-Nspire, has a port of micropython on it.  I'd like to be able to bring my calculator into the shop and use it for all of my machining calculations as it is a lot more robust than a laptop.
        3. Universal app.  For CNC programmers that are already at the computer anyway, having a real dedicated UI for the most important programs is going to be pretty nice.  For machinists out by the machines, having a phone app version of these programs will be incredibly handy especially if you don't have a calculator like I do.

- Q: Why haven't you converted program X?
    - A: Check the "Programs to be converted soon" list. If it's on there, please be patient.  If it's not, file a GitHub issue and I'll take a look at it!

- Q: What's the upython.exe for?
    - A: That's micropython that's been compiled for FreeDOS (but works with any DOS from experience). Taken from https://github.com/pohmelie/micropython-freedos

- Q: When will this be done?
    - A: Whenever I get around to it. This really is just for fun.  I probably won't be doing any more conversions past what's on the "Programs to be converted soon" list.

- Q: Why is are the data (.DAT) files different for the python programs?
    - Original A: Python handles files differently than C (obviously). In many cases I found it easier to change up the data structure so that the files could be handled with regular expressions, making it easier to modify the format, and allow as many entries as your RAM space can handle.
    - New A: Some of the older programs I converted used different data files.  For the new programs, I changed up my method of regular expressions to better follow the old format.  These new programs (I'll check when it changed) use the same data files as the originals.

# Change-Log:
v.0EC5:
- Capitalized "Shaft" on input for LMC/MMC

v.0EC4:
- Removed last testing print from basicfit.py

v.0EC3:
- Moved converted programs jumble from /Converted Programs Jumble/Python3 to just /Converted Programs Jumble
    - Only have Python3 programs at the moment
- Removed /Converted Programs Jumble/Python3 directory

v.0EC2:
- Fixed some issues with basicfit.py
    - Want to add the ability to have any of the 4 variables filled in, not necessarily 2 per type
- Added basicfit.py to Converted Programs Jumble
-Removed 3to2 folder from /Converted Progams Jumble

v.0EC0:
- Added my own basic fit calculator, basicfit.py
    - Enter hole or shaft dimensions, and clearance/allowance (any combination) and the rest is calculated

v.0DC0:
- Added my own time conversion program, time.py

v0.CC6:
- Fixed MM conversion for nspire version of inmmconv.py
    - Also fixed rounding of input

v0.CC5:
- Added inmmconv.py.tns file to For upython/ndless directory
    - Has been modified to work with micropython for TI-Nspire
    - Fully functional

v0.CC1:
- Transferred the new inch-conversion system to INMMCONV.PY

v0.CC0:
- Updated INCHTOMM.PY to handle fractional input

v0.BC7:
- Updated Converted Programs List

v0.BC6:
- Added WIRE.PY to Converted Programs Jumble

v0.BC5:
- Added cost calculator to WIRE.PY

v0.BC0:
- Finished WIRE.PY

v0.BB6:
- Added TEMP.PY to Converted Programs Jumble

v0.BB5:
- Finished TEMP.PY

v0.BB3:
- Added decimal-placement aware rounding
    - e.g., if you input something in the microns, output in ten-milionths rather than tenths
- Updated Q&A

v0.BB2:
- Added inch fractional conversions (to 128ths) for the inch-mm conversion programs

v0.BB1:
- Added "Converted Programs Jumble"

v0.BB0:
- Started TEMP.PY
- Updated to-do list
- Fixed spelling of "millimeter"

v0.AB0:
- Finished \SUBS\CSK.PY
- Updated Q&A
- Fixed program list on README.md

v0.AA0:
- Finished UGROOVE.PY

v0.Z0:
- Finished TUBEWALL.PY

v0.Y0:
- Finished my next step in inch-to-mm conversions by adding a program that can do both and switch quickly.

v0.X0:
- Added my own programs, INCHTOMM.PY & MMTOINCH.PY for those who want to do quick inch to milimeter conversions
    - Especially useful if you only have inch calipers/micrometers or mm calipers/micrometers

v0.W0:
- Finished SUBS\KEYWAY.PY

v0.V0:
- Finished STICK.PY

v0.U0:
- Corrected the lack of sprocket caliper factor for SPROCKET.PY
- Finished EXPAND.PY

v0.T0:
- Finished SPROCKET.PY
- Finished CHAIN.PY

v0.S0:
- Finished HORSE.PY
    - Added ability to convert between watts and horsepower

v0.R0:
- Finished SLUG.PY
- Finished PLATE.PY
- Updated template.py

v0.Q0:
- Finished RATTLE.PY

v0.P0:
- Finished LVERN.PY

v0.O0:
- Finished ROUNDER.PY
- Removed accidental BALLCUTPY.OUT from root

v0.N0:
- Finished REVOLVER.PY

v0.M0:
- Finished MRADIUS.PY

v0.L0:
- Finished GEARPA.PY
    - Except for adding more pressure angles

v0.K0:
- Finished \SUBS\OHM.PY

v0.J0:
- Finished GEAR.PY
- Updated template.py

v0.I0:
- Finished MANDREL.PY
    - Original MANDREL.EXE seems to do the same calculation for music wire and phosphorus bronze?
        - My program gives the correct? 2 different measurements

v0.H0:
- Finished LOFT.PY
    - Added option to change calculation if materials are different
- Sorted Completed Programs list alphabetically

v0.G0:
- Finished DOVETAIL.PY

v0.F0:
- Finished DIFFTHRD.PY

v0.E0:
- Finished COLLET.PY
- Moved FAQ above Change-Log

v0.D0:
- Finished CMITER.PY

v0.C5:
- Finished PCD.PY

v0.C0:
- Finished BALLCUT.PY
- Updated Q&A

v0.B0:
- Finished QBELT.PY
- Started PCD.PY
- Started BALLCUT.PY
- Updated README.md formatting

v0.A0:
- Finished CUSP.PY

v0.91:
- Updated soon-to-convert list
    - Will likely be the last of the programs I convert aside from a few, as these are the ones I would most likely need
    - Please file a GitHub issue if you want a specific program converted

v0.90:
- Finished BEND.PY

v0.80:
- Finished SPEED.PY
- Added .gitignore for VSCode's settings.json
- Changed changelog to reverse chronological order

v0.70:
- Finished ECCENT.PY
- Finished ECCENTUB.PY

v0.60:
- Finished FITS.PY

v0.50:
- Finished FEED.PY
- Updated to-do list

v0.49:
- Added \For upython\ directory for all of the currently converted programs
- Updated README completed list
- Fixed formatting on BOLTCIRC.PY
- Fixed formatting on DOT.PY
- Fixed formatting on KNURL.PY
- Tested pyinstaller & py2exe on BOLTCIRC.PY in the \Compiled Executables directory
- Fixed a promt on GAGE.PY

v0.45:
- Finished DIAM.PY

v0.41:
- Continued work on DIAM.PY
    - Switched to using regular expressions instead of mklotz's read algorithm for more versatility
- Changed DIAM.DAT to DIAMPY.DAT

v0.40:
- Started work on GAGE.PY
- Changed GAGE.PY to use 2 separate data files

v0.35:
- Finished GAGE.PY
- Added metric conversion for gages

v0.30:
- Started work on DIAM.PY
- Updated template.py

v0.22:
- Updated to-do list

v0.21:
- Fixed README.md formatting

v0.20:
- Finished DOT.PY
- Added template.py to root
- Updated README.md

v0.11:
- Fixed README.md formatting

v0.10:
- Added unzipped files from Marv Klotz's programs
- Added current python3 conversions

v0.00:
- Added readme & license