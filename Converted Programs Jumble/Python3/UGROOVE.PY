import math
from tabulate import tabulate

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
    print('INCREMENTAL CUTTING OF A U-SHAPED GROOVE\n')

    rw = float(vin("Groove radius", 1, units="in"))
    dm = float(vin("Lathe tool diameter", 0.25, units="in"))

    op = (input("[A]ngular or (L)inear increment ? ") or "a")
    aorl=0
    if (op == "l"):
        aorl=1

    if (aorl == 1):
        dx = float(vin("Linear increment", 0.0625, units="in"))
    elif (aorl == 0):
        dtheta = float(vin("Angular increment", 5, units="deg"))
    
    rm=0.5*dm
    r=rw-rm

    ofile = "UGROOVE.OUT"
    fp = open(ofile, 'w')

    fp.write('Groove radius = {0} in\n'.format(rw))
    fp.write('Lathe tool diameter = {0} in\n'.format(dm))

    header = ["x", "DOC"]
    movements = []

    if (aorl == 1):
        x=r
        while (x>0):
            y=math.sqrt(r*r-x*x)+rm
            movements.append([round(x,4),round(y,4)])
            x-=dx
    elif (aorl == 0):
        theta=0
        while (theta <= 90):
            x=r*cosd(theta)
            y=math.sqrt(r*r-x*x)+rm
            movements.append([round(x,4),round(y,4)])
            theta+=dtheta
    
    fp.write(tabulate(movements, headers=header))


main()