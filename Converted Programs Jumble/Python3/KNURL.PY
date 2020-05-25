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
    print('WORKPIECE DIAMETER FOR PERFECT KNURLING\n')

    dk = float(vin("Diameter of knurl wheel",0.625, units="in"))
    nk = int(vin("Number of teeth on knurl wheel",40))
    d0 = float(vin("Nominal diameter of workpiece",round(0.87,4), units="in"))
    deltak=math.pi*dk/nk
    print('Spaing between knurl teeth = {0} in\n'.format(round(deltak,4)))
    m=int(math.floor(math.pi*d0/deltak))
    print('Integer number of crests to make on workpiece = {0}\n'.format(m))
    print('Required workpiece circumference = {0} in'.format(round(m*deltak,4)))
    print('\nWORKPIECE DIAMETER FOR PERFECT KNURLING = {0} in'.format(round(m*deltak/math.pi,4)))


main()