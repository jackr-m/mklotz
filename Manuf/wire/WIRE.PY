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
    print('ELECTRICAL WIRE COMPUTATIONS\n')

    current = float(vin("Current wire must carry", 12, units="amps"))
    density = float(vin("Desired current density", 0.0025, units="amps/cmil recommended"))

    R = 1.12294049

    def diam(g):
        return (324.87/math.pow(R,(g)))

    def gage(d):
        return ((math.log10(0.32487)-math.log10(1.e-3*(d)))/math.log10(R))

    g = gage(math.sqrt(current/density))
    g = round(g, 0)
    g = int(g)

    d = diam(g)

    print('\nRecommended AWG gage = {0}\n'.format(g))
    print('Diameter = {0} mils'.format(round(d,1)))
    print('Area = {0} circular mils'.format(round(d*d,1)))
    print('Resistance = {0} ohms/1000 ft'.format(round(10370/(d*d),3)))
    print('Weight = {0} lbs/1000 ft'.format(round(3.02675e-3*d*d,1)))

main()