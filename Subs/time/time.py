import math
import time
from time import gmtime, localtime

def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")

def sind(angle):
    return math.sin(math.radians(angle))
def cosd(angle):
    return math.cos(math.radians(angle))
def tand(angle):
    return math.tan(math.radians(angle))

def vin(prompt, defvalue, **kwargs):
    """Create an input prompt with a default answer
    
    Add a "units" tag for kwargs for units to be included"""
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

def main():
    cls() 
    print('TIME CONVERTER\n')

    print('Current time is: {0} or {1}'.format(time.asctime(gmtime()) + "z", time.asctime(localtime()) + " " + time.tzname[0]) + "/" + str(int(time.timezone/60/60)) + "hrs")

    def convert():
        direction = str(vin("Convert [c]urrent timezone to other timezone, or (o)ther timezone to current, or (q)uit", "c"))

        if direction == "c":
            newzone = str(vin("Target timezone [UTC]", "UTC"))
        elif direction == "o":
            pass
        elif direction == "q":
            return
        else:
            convert()

    convert()

main()