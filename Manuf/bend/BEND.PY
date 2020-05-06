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
    print('BEND ALLOWANCE COMPUTATION\n')
    t = float(vin('Thickness of material', 0.125, units="in"))
    r = float(vin("Radius of bend", 3, units="in"))
    ang = float(vin("Angle of bend", 90, units="deg"))
    ang=math.radians(ang)
    x=0.4*t
    if (r < 2*t):
        x=(1/3)*t
    if (r > 4*t):
        x=0.5*t
    ba=ang*(r+x)
    print('\nLength of bend exterior = {0} in'.format(round(ang*(r+t),4)))
    print('Length of bend interior = {0} in'.format(round(ang*r,4)))
    print('\nLength of material required to form bend = {0}'.format(round(ba,4)))

main()