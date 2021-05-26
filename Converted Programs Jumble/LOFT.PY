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
    print('MINIMUM LENGTH OF THREAD ENGAGEMENT')
    print('TO ENSURE SCREW BREAKS BEFORE THREADS STRIP\n')

    u = (input("(m)etric or [i]mperial ? ") or "i")
    if (u is not 'i'):
        u = 'm'
    
    if (u == 'i'):
        ul = "in"
        ua = "in^2"
    elif (u == "m"):
        ul = "mm"
        ua = "mm^2"
    
    if (u == "i"):
        x=0.25
    elif (u == "m"):
        x=4
    D = float(vin("Basic diameter of screw",x, units=ul))
    if (u == "i"):
        tpi = float(vin("Pitch of screw", 20, units="tpi"))
        p=1/tpi
    elif (u == "m"):
        p = float(vin("Pitch of screw", 0.7, units="mm"))
    
    material_strength = (input("Is the screw and hole the same material? [y]/(n)") or "y")
    if (material_strength is "y"):
        j = 1
    elif (material_strength is "n"):
        strength_screw = vin("Tensile strength of screw material", 440, units="MPa")
        strength_hole = vin("Tensile strength of hole material", 310, units="MPa")
        j = strength_screw/strength_hole

    dp=D-0.64952*p
    at=0.25*math.pi*math.pow(D-0.938194*p,2)
    ass=0.5*math.pi*dp
    le=(2*at/ass)*j

    print('\nPitch circle diameter of thread = {0} {1}'.format(round(dp,4), ua))
    print('Screw thread tensile area = {0} {1}'.format(round(at,4), ua))
    print('Thread shear area = {0} {1}\n'.format(round(ass,4), ua))

    print('Length of thread engagement = {0} {1}'.format(round(le,4),ul))
    print('  or {0} threads'.format(round(le/p,4)))


main()