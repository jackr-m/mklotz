import math
import re
from tabulate import tabulate

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

c=12/math.pi

dfile = "SPEEDPY.DAT"

fp = open(dfile, "r")

materials = []
sfms = []
sfmlow = []
sfmhi = []
nm = 0
diameters = []

def main():
    cls()
    print('MACHINING SPEED UTILITY\n')

    rdata()
    print('number of data entries read = {0}\n'.format(len(materials)))

    for i in materials:
        print(i)
    
    def matnum():
        global nm
        nm = int(vin("Material Number",1))-1
        if (not (-1<nm<=len(materials))):
            print('INVALID NUMBER')
            matnum()
    matnum()

    print('\nInput relevant diameter.')
    print('Diameter of work if lathe, diameter of cutter/drill if mill/drill press.')

    d = float(vin("Diameter", 1, units="in"))
    low = c*float(sfmlow[nm])/d
    high = c*float(sfmhi[nm])/d

    print('\nFor {0} and diameter {1} in:'.format(materials[nm],d))
    print('Recommended rpm between {0} and {1}'.format(int(low), int(high)))

def rdata():
    temp = []
    nd=0 # number of data entries
    k=0
    f=0
    ns=0

    global materials
    global sfms
    global sfmlow
    global sfmhi

    for i in fp:
        x = re.search(r"\d{1,3} (\w|\s)* \d{1,4} \d{1,4}",i)
        try:
            result = x.group()
        except AttributeError:
            result = None
        if result:
            material = re.search(r"\d{1,3} ([a-zA-Z]|\s)*",result)
            sfmsearch = re.search(r"\d{1,4} \d{1,4}",result)
            try:
                mat = material.group()
                sfm = sfmsearch.group()
            except AttributeError:
                mat = None
                sfm = None
            if mat:
                materials.append(mat.rstrip())
            if sfm:
                sfms.append(sfm)

    materials = list(dict.fromkeys(materials))
    sfms = list(dict.fromkeys(sfms))
    sfmsnew = []

    for i in range(0,len(sfms)):
        temp = (re.search(r"\d{1,4} ",sfms[i]).group())
        temp = temp.rstrip()
        sfmlow.append(temp)
    for i in range(0,len(sfms)):
        temp = (re.search(r" \d{1,4}",sfms[i]).group())
        temp = temp.lstrip()
        sfmhi.append(temp)

main()