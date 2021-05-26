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

dfile = "DIAMPY.DAT"
ofile = "DIAMPY.OUT"

fp = open(dfile, "r")
fpw = open(ofile, "w")

materials = []
speeds = []
sfms = []
sfmlow = []
sfmhi = []
nm = 0
diameters = []

def main():
    cls()
    print('MACHINING DIAMETER UTILITY\n')

    rdata()
    print('number of data entries read = {0}'.format(len(materials)))

    for i in materials:
        print(i)
    
    def matnum():
        global nm
        nm = int(vin("Material Number",1))-1
        if (not (-1<nm<=len(materials))):
            print('INVALID NUMBER')
            matnum()
    matnum()
    diamcalc()

def diamcalc():
    material = materials[nm]
    fpw.write('Material = {0}\n'.format(material))

    header = ["SPEED (rpm)", "MIN DIAMETER (in)", "MAX DIAMETER (in)"]

    ns = len(sfms)
    for i in range(0,ns-1):
        dlow=round(c*float(sfmlow[nm])/float(speeds[i]),4)
        dhigh=round(c*float(sfmhi[nm])/float(speeds[i]),4)
        diameters.append([speeds[i],dlow,dhigh])

    fpw.write(tabulate(diameters, headers=header))

def rdata():
    temp = []
    nd=0 # number of data entries
    k=0
    f=0
    ns=0

    global materials
    global speeds
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
        
        y = re.search(r"^\d{1,4}$", i)
        try:
            result = y.group()
        except AttributeError:
            result = None
        if result:
            speeds.append(result)

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