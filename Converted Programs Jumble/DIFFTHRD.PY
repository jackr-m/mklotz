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


dfile = "DIFFTHRD.DAT"

np = 0
pitch = []

def rdata():
    global pitch
    fp = open(dfile, 'r')

    k = None

    for i in fp:
        x = re.search(r"tpi",i)
        y = re.search(r"mm",i)
        z = re.search(r"ENDOFDATA",i)
        if (x is not None):
            k=0
            continue
        elif (y is not None):
            k=1
            continue
        elif (z is not None):
            k=2
        if (i == "\n"):
            continue
        elif (k == 0):
            i = i[ : -1]
            pitch.append(float(i))
        elif (k == 1):
            i = i[ : -1]
            pitch.append(25.4/float(i))

    fp.close()

def main():
    cls()
    rdata()
    np = len(pitch)
    print('DIFFERENTIAL THREAD CALCULATIONS\n')
    print('Number of data items read = {0}\n'.format(np))

    pe = float(vin("Desired effective pitch of differential thread", 100, units="tpi"))

    for i in range(0,np):
        if (pe == pitch[i]):
            print('You can cut this thread directly with available screcutting gear.')

    best = 1.e6
    for i in range (0, np):
        for j in range (0, np):
            if (i==j):
                continue
            pc = pitch[i]
            pf = pitch [j]
            if (pc > pf):
                t = pc
                pc = pf
                pf = t
            d = (1/pc) - (1/pf)
            e = abs(1/d-pe)
            if (e < best):
                best = e
                ib = i
                jb = j
    
    print('\nOf available threads, best match to {0} tpi is:'.format(pe))
    pc=pitch[ib]
    pf=pitch[jb]
    if (pc > pf):
        t=pc
        pc=pf
        pf=t
    print('Coarse thread = {0} tpi = {1} mm/thrd'.format(round(pc,3), round(25.4/pc,3)))
    print('Fine thread = {0} tpi = {1} mm/thrd'.format(round(pf,3), round(25.4/pf,3)))
    print('With an effective pitch of {0} tpi'.format(round((1/((1/pc)-(1/pf))),3)))

    pc = float(vin("Pitch of coarse thread", pc, units="tpi"))
    pf = float(vin("Pich of fine thread", pf, units="tpi"))
    tnc = float(vin("Thickness of coarse (fixed) nut", 0.375, units="in"))
    tnf = float(vin("Thickness of fine (moveable) nut", 0.25, units="in"))
    dm = float(vin("Desired motion of moveable nut", 0.25, units="in"))

    if (pc > pf):
        t=pc
        pc=pf
        pf=t
    pe = 1/((1/pc)-(1/pf))
    print("\nEffective pitch = {0} tpi".format(round(pe,3)))
    print('Motion for one revolution = {0} in'.format(round(1/pe,4)))
    print('Total turns to obtain desired motion = {0}'.format(dm*pe))
    print('Minimum length of coarse thread needed = {0} in'.format(round(tnc+dm*pe/pc,4)))
    print('Minimum length of fine thread needed = {0} in'.format(round(tnf+dm*pe/pf,4)))
    print('Maximum distance between nuts = {0} in'.format(round(dm*pe/pc,4)))
    print('Minimum distance between nuts = {0} in'.format(round(dm*pe/pf,4)))

main()