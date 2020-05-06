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
    root3 = math.sqrt(3)
    print('THREE JAW ECCENTRIC PACKING CALCULATION\n')
    w = float(vin("Witdth of chuck jaws", 0.125, units="in"))
    d = float(vin("Diameter of workpiece", 1.5625, units="in"))
    r=0.5*d
    e = float(vin("Required eccentric offset", 0.28125, units="in"))

    if (e>(r+w)/root3):
        print('Work will fall through unpacked jaws.')
        return
    elif (w>root3*e):
        p=1.5*e
    else:
        p=1.5*e-r+0.5*math.sqrt(4*r*r-3*e*e+2*e*w*root3-w*w)
    print('\nRequired Packing Size = {0} in'.format(round(p,4)))


main()