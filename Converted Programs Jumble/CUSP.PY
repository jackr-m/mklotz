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
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

def main():
    cls()
    b = float(vin("Ball mill diameter", 0.25, units="in"))
    c = float(vin("Desired cusp height", 0.001, units="in"))
    r=0.5*b

    d=2*math.sqrt(2*r*c-c*c)

    print('\nSpacing between successive cuts = {0} in'.format(round(d,4)))

main()