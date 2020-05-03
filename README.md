# mklotz
A python3 rewrite of Marv Klotz's classic programs.  Designed to work in any python 3 environment, including micropython.

These are designed to be as close to a 1-1 copy of Marv Klotz's programs from http://www.myvirtualnetwork.com/mklotz/


Currently converted programs:
- 3WIRE
- ROTARY
- SINEBAR (EVERYTHING DONE EXCEPT FOR FINDING THE INDIVIDUAL GAGE BLOCKS)
- BOLTCIRC
- COMPOUND
- DALLOW
- KNURL
- DOT
- DPLATE

Programs to be converted soon:
- DIAM
- SPEED
- DRILL
- ECCENT
- FEED
- FITS
- GAGE

To-do List:
1. Create two directiories for short file names (DOS compatability) and long file names (more descriptive)
2. Compile python programs into a single .exe file to mimick mklotz's original programs
3. Create a list to help you find/figure out what each program is for.

Change-Log:

v0.00:
- Added readme & license

v0.10:
- Added unzipped files from Marv Klotz's programs
- Added current python3 conversions

v0.11:
- Fixed README.md formatting

v0.20:
- Finished DOT.PY
- Added template.py to root
- Updated README.md

v0.21:
- Fixed README.md formatting

v0.22:
- Updated to-do list

v0.30:
- Started work on DIAM.PY
- Updated template.py

FAQ:
- Q: What's the upython.exe for?
    - A: That's micropython that's been compiled for FreeDOS (but works with any DOS from experience). Taken from https://github.com/pohmelie/micropython-freedos

- Q: I don't like the default values for some of the inputs.
    - A: Well change them! If you see a 'vin' command, the 2nd argument is the default value.

- Q: When will this be done?
    - A: Whenever I get around to it. This really is just for fun.  I (hopefully) plan to finish all of the \manuf programs, and possibly some of the submissions (if source code is available)

- Q: Too many decimal places!
    - A: In most cases I tried to round the answers to the tenths of and inch, as that is the usual practical limit of measuring normal things in a shop.  If you need more change it!  If this is a common request I might go back and add a "rounding" variable at the top of each file for people to change.