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
    """Create an input prompt with a defualt answer
    
    Add a "units" tag for kwargs for units to be included"""
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

torque = 0
rpm = 0
hp = 0
watts = 0

def main():
    cls()
    print('TORQUE-SPEED-HORSEPOWER OR WATTS-HORSEPOWER CALCULATIONS\n')
    print('Input whatever data you know; press return if not known')
    print('You must enter two data items to obtain a solution.\n')

    print('Skip torque/rpm inputs for watts/hp conversion\n')

    global torque
    global rpm
    global hp
    global watts

    def disp():
        print('\nTorque     = {0} ft-lb = {1} in-lb'.format(round(torque,4), round(torque*12,4)))
        print('Speed      = {0} rpm'.format(round(rpm,4)))
        print('Horsepower = {0} hp'.format(round(hp,4)))
        print('Watts = {0} w'.format(round(watts,4)))

    def doit():
        global torque
        global rpm
        global hp
        global watts
        if ((hp != 0) & (torque == 0) & (rpm == 0) & (watts == 0)):
            watts = hp*746
            disp()
            return
        if ((watts != 0) & (hp == 0) & (torque == 0) & (rpm == 0)):
            hp = watts/746
            disp()
            return

        if ((hp != 0) & (rpm != 0)):
            torque = hp*5252/rpm
            disp()
            return
        if ((hp != 0) & (torque != 0)):
            rpm = hp*5252/torque
            disp()
            return
        if ((torque != 0) & (rpm != 0)):
            hp = torque*rpm/5252
            disp()
            return
        print("\nINsUFFICIENT DATA FOR SOLUTION")


    k=0
    print('ft-lb = in-lb/12')
    torque = float(vin("Torque", 0, units="ft-lb"))
    if (torque != 0):
        k+=1
    rpm = float(vin("Rotational speed", 0, units="rpm"))
    if (rpm != 0):
        k+=1
    if (k == 2):
        doit()
        return
    hp = float(vin("Horsepower", 0, units="hp"))
    if (hp != 0):
        k+=1
    if ((k == 2) | (hp != 0)):
        doit()
        return
    watts = float(vin("Watts", 0, units="w"))
    if (watts != 0):
        doit()
        return

    print("\nINsUFFICIENT DATA FOR SOLUTION")

main()