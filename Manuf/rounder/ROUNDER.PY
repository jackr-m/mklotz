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
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

ofile = "ROUNDER.OUT"

def main():
    cls()
    print('ROUNDING OVER CALCULATIONS\n')

    rw = float(vin("Workpiece radius", 1, units="in"))
    dm = float(vin("Ball-end mill diameter", 0.25, units="in"))
    dtheta = float(vin("Angular increment", 5, units="deg"))

    rm=0.5*dm
    r=rw+rm



    def psol():
        fp = open(ofile, 'w')

        fp.write('Workpiece radius = {0} in\n'.format(rw))
        fp.write('Ball mill diameter = {0} in\n'.format(dm))
        fp.write('A = (R+r)*sin(theta)\n')
        fp.write('B = (R+r)*cos(theta)\n')
        fp.write('C = (R+r)*(1 - sin(theta)\n')
        fp.write('D = (R+r)*(1 - cos(theta)\n\n')

        header = ["ANGLE", "A", "B", "C", "D"]

        data = []

        theta = 0
        while (theta <= 90):
            a=r*sind(theta)
            b=r*cosd(theta)
            c=r*(1-sind(theta))
            d=r*(1-cosd(theta))

            data.append([theta,round(a,4),round(b,4),round(c,4),round(d,4)])
            theta+=dtheta


        fp.write(tabulate(data, headers=header))
        
        fp.close()


    psol()
    print('\nYour data is on {0}'.format(ofile))

main()