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
    print('OUTER RADIUS CALCULATOR\n')

    d = float(vin("Diameter of rollers (d)", 0.25))
    m = float(vin("Measurement across rollers (m)", 2))
    g = float(vin("Gap (g)", 0.1))

    r=0.5*d
    w=0.5*m-r
    R=((r-g)*(r-g)+w*w-r*r)/(2*(d-g))

    print('\nRadius of curvature = {0}'.format(round(R,4)))

main()