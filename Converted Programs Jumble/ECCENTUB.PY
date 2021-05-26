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
    print('TUBE SIZE FOR TURNING ECCENTRICS\n')
    D = float(vin("Diameter of parent stock", 1, units="in"))
    e = float(vin("Required eccentric offset", 0.1, units="in"))
    R = 0.5*D
    r2=R-e
    dtube =2.*math.sqrt(7*R*R-9*R*r2+3*r2*r2)
    print('\nDiameter of required tube = {0} in'.format(round(dtube,4)))

main()