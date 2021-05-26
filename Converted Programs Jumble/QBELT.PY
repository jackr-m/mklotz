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
    print('TWO PULLEY BELT LENGTH CALCULATION\n')

    d1 = float(vin("Diameter of smaller pulley", 2))
    d2 = float(vin("Diameter of smaller pulley", 6))
    sep = float(vin("Separation between pulley centers", 6))

    if (d1 > d2):
        span = d1
        d1 = d2
        d2 = span
    
    r1=0.5*d1
    r2=0.5*d2
    eps=r2-r1
    phi=math.asin(eps/sep)
    theta=2*phi
    beta1=math.pi-theta
    beta2=math.pi+theta
    wrap1=r1*beta1
    wrap2=r2*beta2
    span=math.sqrt(sep*sep-eps*eps)
    blen=2*span+wrap1+wrap2

    print('\nFor smaller pulley')
    print('Diameter = {0}'.format(d1))
    print('Wrap angle = {0} deg'.format(round(math.degrees(beta1),4)))
    print('Wrap length = {0}'.format(round(wrap1,4)))

    print('\nFor large pulley')
    print('Diameter = {0}'.format(d2))
    print('Wrap angle = {0} deg'.format(round(math.degrees(beta2),4)))
    print('Wrap length = {0}'.format(round(wrap2,4)))
    


main()