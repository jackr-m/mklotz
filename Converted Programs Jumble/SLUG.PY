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
    print('CHAIN DRILLING LARGE HOLES\n')

    d1 = float(vin("Diameter of final hole", 3, units="in"))
    h = float(vin("Radial allowance for finish machining", 0.050, units="in"))
    dh = float(vin("Drill diameter", 0.25, units="in"))
    w = float(vin("Approximate web thickness", 0.050, units="in"))

    rh = 0.5*dh
    d2=d1-2*h
    d3=d2-dh
    r3=0.5*d3
    theta=2*math.asin(rh/r3)
    phi=w/r3
    omega=theta+phi
    n=math.floor((math.pi*2)/omega)
    omega=(math.pi*2)/n
    w=r3*(omega-theta)
    chord=d3*math.sin(0.5*omega)

    print('\nNumber of holes = {0}'.format(n))
    print('Diameter of drilling circle = {0} in'.format(round(d3,4)))
    print('Final web thickness = {0} in'.format(round(w,4)))
    print('Angle between adjacent holes = {0} deg'.format(round(math.degrees(omega),3)))
    print('Chordal distance between adjacent holes = {0} in'.format(round(chord,4)))


main()