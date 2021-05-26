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
    print('SPROCKET CENTER DISTANCE GIVEN CHAIN LEGTH\n')

    p = float(vin("Chain pitch", 1, units="in"))
    l = float(vin("Chain length", 48, units="pitches"))
    nl = int(vin("Number of teeth in large sprocket", 18))
    ns = int(vin("Number of teeth in small sprocket", 9))

    a=2*l-nl-ns
    b=nl-ns
    c=(p/8.)*(2.*l-nl-ns+math.sqrt(a*a-0.810*b*b))

    print('\nSprocket center-to-center distance = {0} in'.format(round(c,4)))

main()