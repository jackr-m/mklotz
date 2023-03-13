import math
from msvcrt import getch

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
    print('ELECTRICAL WIRE COMPUTATIONS\n')

    current = float(vin("Current wire must carry", 12, units="amps"))
    density = float(vin("Desired current density", 0.0025, units="amps/cmil recommended"))

    R = 1.12294049

    def diam(g):
        return (324.87/math.pow(R,(g)))

    def gage(d):
        return ((math.log10(0.32487)-math.log10(1.e-3*(d)))/math.log10(R))

    g = gage(math.sqrt(current/density))
    g = round(g, 0)
    g = int(g)

    d = diam(g)

    print('\nRecommended AWG gage = {0}\n'.format(g))
    print('Diameter = {0} mils'.format(round(d,1)))
    print('Area = {0} circular mils'.format(round(d*d,1)))
    print('Resistance = {0} ohms/1000 ft'.format(round(10370/(d*d),3)))
    print('Weight = {0} lbs/1000 ft'.format(round(3.02675e-3*d*d,1)))


    def calcost():
        if (g == 6):
            perFoot = 0.81
        elif (g == 8):
            perFoot = 0.55
        elif (g == 10):
            perFoot = 0.36
        elif (g == 12):
            perFoot = 0.27
        elif (g == 14):
            perFoot = 0.18
        elif (g == 16):
            perFoot = 0.11
        elif (g == 18):
            perFoot = 0.06
        else:
            perFoot = 1

        price = float((input("Price per foot [${0}] ? ".format(perFoot)) or perFoot))
        feet = float(vin("Desired length", 100, units="feet"))
        strands = float(vin("Number of wires", 3))
        tax = float(vin("Local sales tax rate", 8, units="%"))

        totalCost = price*feet*strands*(1+(tax/100))

        print('Total cost = ${0}'.format(round(totalCost,2)))

    print('\nWould you like to calculate cost? (y)/[N]')
    costinput = getch()
    costinput = ord(costinput)

    if (costinput == 13):
        costinput = 110

    if (costinput == 110):
        return
    elif (costinput == 121):
        calcost()

main()