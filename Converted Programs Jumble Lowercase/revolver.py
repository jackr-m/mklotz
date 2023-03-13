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
    print('REVOLVER CYLINDER TOOL HOLDER CALCULATIONS\n')

    nh = int(vin("Number of holes", 6))

    print('\nHole size to fit tool body')
    dh = float(vin("Diameter of holes", 0.25, units="in"))

    print('\nRemember to allow for diameter of tool top if larger than tool body')
    space = float(vin("Spacing between hole edges", 0.5, units="in"))

    print('\nHoles cannot be tangent to cylinder circumference')
    t = float(vin("Thickness required at outer edge of holes", 0.25, units="in"))

    sp=360/nh
    rh=0.5*dh
    rc=(space+dh)/(2*sind(0.5*sp))
    cr=rc+rh+t
    cd=2*cr

    print('\nRadius for hole placement = {0} in'.format(round(rc,4)))
    print('Required stock diameter for cylinder = {0} in'.format(round(cd,4)))



main()