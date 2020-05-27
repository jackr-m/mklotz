#This is the first program I created to add onto Marv Klotz's originals.
#I made this because I only have inch venier calipers but wanted to measure something
#I knew was metric to put into CAD.

import math
from decimal import Decimal

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
    """Create an input prompt with a defualt answer
    
    Add a "units" tag for kwargs for units to be included"""
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

def tofrac(x, largest_denominator=128):
    if not x >= 0:
        raise ValueError("x must be >= 0")
    scaled = int(round(x * largest_denominator))
    whole, leftover = divmod(scaled, largest_denominator)
    if leftover:
        while leftover % 2 == 0:
            leftover >>= 1
            largest_denominator >>= 1
    #return whole, leftover, largest_denominator
    if (whole == 0):
        stringVal = str(leftover) + "/" + str(largest_denominator)
    else:
        stringVal = str(whole) + " " + str(leftover) + "/" + str(largest_denominator)
    return stringVal

def main():
    cls()
    print('MILLIMETER TO INCH CONVERSIONS\n')

    print('Input your measurement and press enter for a conversion.')
    print("Input 'q' to quit.\n")

    def measure():
        mm = vin("MM measurement", 5, units="mm")
        if (mm == 'q'):
            return
        else:
            d = Decimal(mm)
            mm = float(mm)
            inch = mm/25.4
            if (d.as_tuple().exponent <= -3):
                digits = 5
            else:
                digits = 4
            print('{0} mm = {1} in = {2} in'.format(mm, tofrac(inch), round(inch,digits)))
            measure()

    measure()

main()