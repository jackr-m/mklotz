#This is the first program I created to add onto Marv Klotz's originals.
#I made this because I only have inch venier calipers but wanted to measure something
#I knew was metric to put into CAD.

import math

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
    print('INCH TO MILLIMETER CONVERSIONS\n')

    print('Input your measurement and press enter for a conversion.')
    print("Input 'q' to quit.\n")

    def measure():
        inch = vin("Inch measurement", 0.1969, units="in")
        if (inch == 'q'):
            return
        else:
            inch = float(inch)
            mm = inch*25.4
            print('{0} in = {1} mm'.format(inch, round(mm,3)))
            measure()

    measure()

main()