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
    print('CYLINDRICAL COLLETS FOR POLYGONAL STOCK\n')

    print('Collets can be made for square, hex, or otagonal stock.')
    n = int(vin("Number of stock polygon sides", 6))
    af = float(vin("Stock across flats dimension", 3/16, units="in"))
    sw = float(vin("Collet slot width", 0.045, units="in"))

    theta = 180/n
    ct = cosd(theta)
    st = sind(theta)

    d=2*math.sqrt(0.25*sw*sw+math.pow((0.5*af/ct-0.5*sw*st/ct),2))

    print('\nRequired collet bore diameter = {0} in'.format(round(d,4)))


main()