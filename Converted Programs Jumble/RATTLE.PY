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
def asind(angle):
    return math.degrees(math.asin(angle))

def vin(prompt, defvalue, **kwargs):
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

def main():
    cls()
    print('DIAMETER MEASUREMENT WITHOUT LARGE ENOUGH CALIPERS\n')

    print('Use a stick of known length less than the dimension or')
    print('use the max distance measured by a pair of calipers\n')
    stick = float(vin("Distance measured by stick/calipers", 4))
    
    print('\nMeasure the gap between the end of the stick and the diameter to measure')
    rattle = float(vin("Rattle distance", 0.2))

    theta = asind(0.5*rattle/stick)
    beta = asind(rattle/stick)
    diam = stick*cosd(theta)/(1.-0.5*(1.-cosd(beta)))
    diff = diam-stick

    print('\nDiameter = {0}'.format(round(diam,6)))
    print('Diameter - stick = {0}'.format(round(diff,6)))

main()