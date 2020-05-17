import math
from msvcrt import getch

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

def main():
    cls()
    print('INCH-MILIMETER OR MILIMETER-INCH CONVERSIONS\n')

    print('Choose your conversion, then input your measruement and press enter.')
    print("Input 'q' to quit, or 'c' to change conversions.\n")

    def prompt():
        print('(M)ilimeter Conversion or [I]nch Conversion ? ')
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
            mm = float(mm)
            inch = mm/25.4
            print('{0} mm = {1} in'.format(mm, round(inch,4)))
            measureMM()
    
    def measureINCH():
        inch = vin("Inch measurement", 0.1969, units="in")
        if (inch == 'q'):
            return
        if (inch == 'c'):
            measureMM()
        else:
            inch = float(inch)
            mm = inch*25.4
            print('{0} in = {1} mm'.format(inch, round(mm,3)))
            measureINCH()

    prompt()

main()