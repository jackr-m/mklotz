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

def vin(prompt:str, defvalue:float, **kwargs):
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
    print('MEASURING TUBE WALL THICKNESS WITH OUTSIDE MICROMETER\n')

    a2 = float(vin("Micrometer anvil diameter", 0.249))
    a=0.5*a2
    d = float(vin("Tube outside diameter", 0.879))
    R=0.5*d
    m = float(vin("Micrometer measurement", 0.0625))

    B=-d
    C=-a*a+2*R*m-m*m

    t=-0.5*(B+math.sqrt(B*B-4*C))

    print('\nTube wall thickness = {0}'.format(round(t,4)))
    print('Tube inside diameter = {0}'.format(round(d-2*t,4)))

main()