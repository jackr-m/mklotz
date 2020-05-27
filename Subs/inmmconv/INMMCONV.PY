import math
from msvcrt import getch
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
    print('INCH-MILLIMETER OR MILLIMETER-INCH CONVERSIONS\n')

    print('Choose your conversion, then input your measurement and press enter.')
    print("Input 'q' to quit, or 'c' to change conversions.\n")

    def prompt():
        print('(M)illimeter Conversion or [I]nch Conversion ? ')
        convinput = getch()
        convinput = ord(convinput)
        if (convinput == 13):
            convinput = 105
        if (convinput == 109):
            measureMM()
        elif (convinput == 105):
            measureINCH()
        else:
            print('INVALID CONVERSION')
            prompt()

    def measureMM():
        mm = vin("MM measurement", 5, units="mm")
        if (mm == 'q'):
            return
        elif (mm == 'c'):
            measureINCH()
        else:
            d = Decimal(mm)
            mm = float(mm)
            inch = mm/25.4
            if (d.as_tuple().exponent <= -3):
                digits = 5
            else:
                digits = 4
            print('{0} mm = {1} in = {2} in'.format(mm, tofrac(inch), round(inch,digits)))
            measureMM()
    
    def measureINCH():
        inch = vin("Inch measurement", 0.1969, units="in")
        if (inch == 'q'):
            return
        if (inch == 'c'):
            measureMM()
        else:
            d = Decimal(inch)
            inch = float(inch)
            mm = inch*25.4
            if (d.as_tuple().exponent <= -4):
                digits = 4
            else:
                digits = 3
            print('{0} in = {1} in = {2} mm'.format(inch, tofrac(inch), round(mm,digits)))
            measureINCH()

    prompt()

main()