import math
from fractions import Fraction

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
    print('VERNIER DESIGN\n')

    dm = float(vin("Distance spanned by one major division on main scale", 1))
    nm = int(vin("Number of subdivisions of one major divison on main scale", 8))

    dl = float(vin("Distance measured by one division on vernier scale", 1/32))
    print('Nearest Rational Fraction = ' + str(Fraction(dl).limit_denominator(512)))

    xlm=dm/nm
    xnv=xlm/dl
    nv=math.floor(xnv)

    if (nv != xnv):
        print('\nNumber of vernier subdivisions is not an integer!')
        return
    print('\nDistance spanned by one division on main scale = {0}'.format(round(xlm,4)))
    print('Nearest Rational Fraction = ' + str(Fraction(xlm).limit_denominator(512)))
    print('Number of subdivisions required on vernier scale = {0}'.format(nv))
    tdv=dm-xlm
    print('\nA typical vernier scale would span a distance of {0}'.format(round(tdv,4)))
    tdv = float(vin("Distance spanned by vernier scale", tdv))
    
    xlv=tdv/nv
    print('\nDistance spanned by one division on vernier scale = {0}'.format(round(xlv,4)))
    print('Nearest Rational Fraction = ' + str(Fraction(xlv).limit_denominator(512)))

main()