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
    print('DIVISON PLATE CALCULATION\n')

    ndiv = int(vin("Number of divisions",14))
    dc = float(vin("Diameter of mounting circle",112))
    rc = 0.5*dc
    theta = 360/ndiv
    s=sind(0.5*theta)
    rd=rc*s/(1-s)
    print('Disk diameter = {0}'.format(round(2*rd,4)))

main()