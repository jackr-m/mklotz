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

    units = kwargs.get('units', None)
    
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '

    return (input(input_prompt) or defvalue)

dfile = "FITSPY.DAT"
fp = open(dfile, 'r')

nd = 0
nf = []
fit_names = []
constants = []
allowances = []

def main():
    cls()
    print('SHAFT/HOLE FIT COMPUTATIONS')
    rdata()
    global nd
    global nf
    global fit_names
    global allowances
    global constants
    print('Number of data items read = {0}'.format(nd))

    for i in range (0, nd):
        print(str(i+1) + "  " + fit_names[i])
    
    def agin():
        global nf
        nf = int(vin("Fit desired", 4))
        if ((nf < 1) or (nf > nd)):
            print('OPTION SELECTION ERROR')
            agin()
    agin()
    nf-=1
    diam = float(vin("Nominal diameter of shaft", 1, units="in"))
    dc=diam+0.001*(float(allowances[nf])*diam+float(constants[nf]))
    print('\nDiameter of shaft for {0} Fit = {1} in'.format(fit_names[nf],round(dc,4)))

def rdata():
    global nd
    global nf
    global fit_names
    global constants
    global allowances
    nd = 0
    f = 0

    for i in fp:
        fit_name = re.search(r"[a-zA-z| ]+,", i)
        if (fit_name is not None):
            fit_names.append(fit_name.group())

        constant = re.search(r",(\d|.){1,5},", i)
        if (constant is not None):
            constants.append(constant.group())

        allowance = re.search(r",(\d|.){1,5}$", i)
        if (allowance is not None):
            allowances.append(allowance.group())
    
    fit_names = [sub[ : -1] for sub in fit_names]
    constants = [sub[1 : -1] for sub in constants]
    allowances = [sub[1 : ] for sub in allowances]

    for i in constants:
        i = float(i)
    for i in allowances:
        i = float(i)
    nd = len(fit_names)


main()