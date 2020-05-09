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
- \MANUF\FEED
- \MANUF\FITS
- \MANUF\GAGE
- \MANUF\KNURL
- \MANUF\LOFT
- \MANUF\ROTARY
- \MANUF\SINEBAR (EVERYTHING DONE EXCEPT FOR FINDING THE INDIVIDUAL GAGE BLOCKS)
- \MANUF\SPEED

### Programs to be converted soon:
- \MANUF\BELT\BELT.EXE
- \MANUF\CHANGE
- \MANUF\DIVHEAD
- \MANUF\EXPAND
- \MANUF\GEAR
- \MANUF\GEARPA
    - Add more pressure angles
- \MANUF\HORSE
    - Will add an option to convert between Watts & HP
- \MANUF\LVERN
- \MANUF\MANDREL
- \MANUF\MRADIUS
- \MANUF\MIXTURE
- \MANUF\REVOLVER
- \MANUF\ROUNDER
- \MANUF\RATTLE
- \MANUF\SPROCKET
- \MANUF\STICK
- \MAUF\SLUG
    - Includes SLUG.C & PLATE.C
- \MANUF\TUBEWALL
- \MANUF\UGROOVE
- \MANUF\VREV
- \MANUF\WIRE
- \MISC\TEMP
- \SUBS\CSK
- \SUBS\KEYWAY
- \SUBS\OHM


### Challenge programs (will take a while to convert but are extremely useful):
- \MANUF\DRILL
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

## FAQ:
----
- Q: Why haven't you converted program X?
    - A: Check the "Programs to be converted soon" list. If it's on there, please be patient.  If it's not, file a GitHub issue and I'll take a look at it!

- Q: What's the upython.exe for?
    - A: That's micropython that's been compiled for FreeDOS (but works with any DOS from experience). Taken from https://github.com/pohmelie/micropython-freedos

- Q: Why are there no trailing zeros in the outputs like the original programs?
    - A: I haven't fully figured out how to get the python formatting to do that reliably.  When/if I do, I will most likely go back and change all of the programs to match the originals, as I think seeing a zero at the end is easier to match what the handwheel or DRO says.  It's not totally unusable without it though, and I'd rather be able to use the programs.

- Q: When will this be done?
    - A: Whenever I get around to it. This really is just for fun.  I probably won't be doing any more conversions past what's on the "Programs to be converted soon" list.

- Q: Too many decimal places!
    - A: In most cases I tried to round the answers to the tenths of and inch, as that is the usual practical limit of measuring normal things in a shop.  If you need more change it!  If this is a common request I might go back and add a "rounding" variable at the top of each file for people to change.

- Q: Why is are the data (.DAT) files different for the python programs?
    - A: Python handles files differently than C (obviously). In many cases I found it easier to change up the data structure so that the files could be handled with regular expressions, making it easier to modify the format, and allow as many entries as your RAM space can handle.

# Change-Log:
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