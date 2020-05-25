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

mf = None

def main():
    cls()
    print('DOVETAIL MEASUREMENTS\n')
    
    global mf
    h = None

    def again():
        global mf
        mf = input('(M)ale or (F)emale dovetail ? ')
        if ((mf is not "m") & (mf is not "f")):
            print("error, try again")
            again()
    again()
    
    angle = float(vin("Dovetail angle", 60, units="deg"))
    pin = float(vin("Pin diameter", 0.375))

    if (mf == "m"):
        h = float(vin("Height of dovetail", 0.5))
        x = float(vin("Measurement across pins", 2.5))
    elif (mf == "f"):
        h = float(vin("Depth of dovetail", 0.5))
        x = float(vin("Measurement between pins", 2.5))

    c=h/tand(angle)
    ta=tand(0.5*angle)
    q=1+1/ta

    if (mf == "m"):
        b=x-pin*q
        t=b+2*c
    elif (mf == "f"):
        b=x+pin*q
        t=b-2*c
    
    print('\nDovetail angle = {0} deg'.format(angle))
    print('Pin diameter = {0}'.format(pin))
    print('Measurement across pins = {0}'.format(x))
    print('Height of dovetail = {0}'.format(h))
    print('Measurement across top of dovetail = {0}'.format(round(t,4)))
    print('Measurement across bottom of dovetail = {0}'.format(round(b,4)))

main()