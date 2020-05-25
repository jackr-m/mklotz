import math
import re

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

names = []
rhos = []
nd = 0

def main():
    cls()
    print('MATERIAL EXPANSION CALCULATIONS\n')

    print('NOTE: The default values in the data file are averages.')
    print('You are encouraged to measure your material and add it to the data file.')

    global names
    global rhos
    global nd

    rdata()
    print('Number of data items read = {0}\n'.format(nd))

    for i in range(len(names)):
        print(names[i] + "=" + str(i+1), end=" ")


    mid = int(vin("\nMaterial number", nd+1))
    if (mid > len(names)):
        cte = float(vin("Material coefficient of thermal expansion", 10, units="ppm/degF"))
        rho = cte*1.e-6
        name = "??"
    else:
        cte = rhos[mid-1]
        rho = cte*1.e-6
        name = names[mid-1]

    length = float(vin("\nNominal length of object", 1, units="in"))
    

    print('\n A. Find length change given temperature change')
    print(' B.Find temperature change given length change')
    opt = (input("([A],B) ? ") or "a")

    if (opt == "a"):
        deltat = float(vin("Temperature change", 100, units="degF"))
        dlength=rho*length*deltat
    elif (opt == "b"):
        dlength = float(vin("Length change", 0.0001, units="in"))
        deltat=dlength/(rho*length)
    else:
        print('NOT A VALID OPTION')
        return

    print('\nCo of Therm Expan of {0} = {1} ppm/degF'.format(name, round(cte,4)))
    print('Nominal length = {0} in'.format(length))
    print('Length change = {0} in'.format(round(dlength,6)))
    print('Temperature change = {0} degF'.format(round(deltat,4)))

def rdata():
    ofile = "EXPAND.DAT"
    fp = open(ofile, 'r')

    global names
    global rhos
    global nd

    start = False
    for i in fp:
        if (start == False):
            startsrch = re.search(r"STARTOFDATA", i)
            if (startsrch != None):
                start = True
            continue
        elif (start == True):
            end = re.search(r"ENDOFDATA", i)
            if (end != None):
                if ((len(names)) != (len(rhos))):
                    print('ERROR READING DATA FILE')
                    return
                nd = len(names)
                return
            x = re.search(r"([a-zA-Z]|-)*,", i)
            if (x != None):
                x = x.group()
                x = x[:-1]
                names.append(x)
            y = re.search(r",(\d|\.)*$", i)
            if (y != None):
                y = y.group()
                y = y[1:]
                y = float(y)
                rhos.append(y)

main()