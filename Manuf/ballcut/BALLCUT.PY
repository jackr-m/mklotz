import math
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

ofile = "BALLCUTPY.OUT"
fp = open(ofile, 'w')

d = None
ds = None
fdx = None
r = None
rs = None
dtheta = None
xinc = None

def main():
    cls()
    print('INCREMENTAL SPHERE SHAPING ON LATHE\n')

    global d
    global ds
    global fdx
    global r
    global rs
    global dtheta
    global xinc

    d = float(vin("Sphere diameter", 1, units="in"))
    ds = float(vin("Stock diameter", d, units="in"))
    op = (input('Constant [A]ngle step or constant (X) step ? ') or 'a')
    if (op == 'x'):
        fdx=1
    else:
        fdx=0
    if(fdx==0):
        dtheta = float(vin("Angulat increment", 5, units="deg"))
    else:
        xinc = float(vin("X increment", 0.02, units="in"))
    r=0.5*d
    rs=0.5*ds

    wdata()

    print('\nYour data is on {0}'.format(ofile))

def wdata():
    global d
    global ds
    global fdx
    global r
    global rs
    global dtheta
    global xinc
    
    fp.write("Incremental Sphere Turning Data")
    fp.write("Sphere diameter = {0} in".format(d))
    fp.write("Stock diameter = {0} in".format(ds))
    if (fdx == 0):
        fp.write("Angular increment = {0} deg".format(dtheta))
    else:
        fp.write("X increment = {0} in".format(xinc))
    
    fp.write("N = cut number")
    fp.write("XF = axial (along lathe bed) position of tool")
    fp.write("DX = increment inx from last cut")
    fp.write("YF = depth of cut")
    fp.write("DY = increment in y from last cut")
    fp.write("WD = work diameter resulting from depth of cut YF\n")

    header = ["N", "XF", "DX", "YF", "DY", "WD"]

    theta = 0
    nc = 0
    dx = 0
    dy = 0
    while (theta <= 90):
        theta+=dtheta
        nc+=1
        x=r*math.cos(math.radians(theta))
        y=r*math.sin(math.radians(theta))
        dc=rs-y
        dw=2*y
        if (dc < 0):
            break
        if (nc):
            dx = f-fl
            dy = dc-dcl


main()