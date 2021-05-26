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
def atand(angle):
    return math.degrees(math.atan(angle))

def vin(prompt, defvalue, **kwargs):
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

def main():
    cls()
    print('TABLE SAW ANGLES FOR POLYGONAL FORMS\n')

    n = int(vin("Number of sides", 4))
    s = float(vin("Slope", 30, units="deg"))

    def done():
        print('Miter gauge angle = {0} deg'.format(round(mg,4)))
        print('Blade tilt (mitered) = {0}'.format(round(btm,4)))
        print('Blate tilt (butted) = {0} deg'.format(round(btb,4)))


    print('\nNumber of sides = {0}'.format(n))
    print('Slope = {0} deg'.format(s))
    if (s == 90):
        mg = 90
        btm = 180/n
        btb = 0
        done()
    elif (s == 0):
        mg=atand(1./(cosd(s)*tand(180./n)))
        btm = 0
        btb = 90
        done()
    else:
        mg=atand(1./(cosd(s)*tand(180./n)))
        cmg=cosd(mg)
        ts=math.tan(math.radians(s))
        btm=atand(cmg*ts)
        btb=atand(cmg/ts)
        done()

main()