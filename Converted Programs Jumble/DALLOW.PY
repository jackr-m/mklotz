import math

def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")

def vin(prompt, defvalue, **kwargs):

    units = kwargs.get('units', None)
    
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '

    return (input(input_prompt) or defvalue)

def main():
    cls()
    print('DRILL TIP ALLOWANCE COMPUTATION\n')

    theta = float(vin("Included angle of drill tip",118))
    tz = math.tan(math.radians(0.5*theta))
    dh = float(vin("Drill diameter", 0.5, units="in"))
    a = round(0.5*dh/tz,4)
    print('\nAllowance for drill tip = {0} in\n'.format(a))

main()