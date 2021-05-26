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

dh = None
dcsk = None

def main():
    cls()
    print('COUNTERSINK COMPUTATIONS\n')

    theta = float(vin("Included angle of countersink", 60, units="deg"))
    tz=tand(0.5*theta)

    global dh
    global dcsk

    def agin():
        global dh
        global dcsk
        dh = float(vin("Drilled hole diameter", 0.5, units="in"))
        dcsk = float(vin("Desired countersink diameter", 0.6, units="in"))
        
        if (dcsk <= dh):
            print('Whoops, csk diameter smaller than drilled hole diameter')
            agin()

    agin()

    a=0.5*(dcsk-dh)/tz
    print('\nDepth from initial contact with drilled hole = {0} in'.format(round(a,4)))
    b=0.5*dcsk/tz
    print('\nDepth from surface of work = {0} in'.format(round(b,4)))


main()