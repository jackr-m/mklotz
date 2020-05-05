import math
import tabulate.py

def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")

def vin(prompt, defvalue):
    return (input(prompt + ' [' + str(defvalue) + '] ? ') or defvalue)


ofile = 'BOLTCIRCPY.DAT'
fp = open(ofile, "w")

def main():
    print('BOLTCIRCLE COMPUTATIONS\n')
    nh = int(vin("Number of holes",6))
    rc = float(vin("Radius of bolt circle",1))
    dh = float(vin("Diameter of bolt holes",0.5))
    a0 = float(vin("Angular offset of first hole",0))
    x0 = float(vin("X offset of bolt circle center",0))
    y0 = float(vin("Y offset of bolt circle center",0))

    sp = float(360/nh)
    space = 2*rc*math.sin(math.radians(0.5*sp))-dh
    if (space < 0):
        print('\nWARNING: HOLES WILL OVERLAP !')
    
    fp.write("Boltcircle specification:\n")
    fp.write("Radius of bolt circle = {0}\n".format(rc))
    fp.write("Bolt hole diameter = {0}\n".format(dh))
    fp.write("Spacing between hole edges = {0}\n".format(format(space, '.4f')))
    fp.write("Angular offset of first hole = {0}\n".format(a0))
    fp.write("X offset of bolt circle center = {0}\n".format(x0))
    fp.write("Y offset of bolt circle center = {0}\n".format(y0))

    header = ["HOLE", "ANGLE", "X-COORD", "Y-COORD"]

    angles = []

    for i in range(0,nh):
        ang=a0+i*sp
        angles.append([i+1, ang, format(rc*math.cos(math.radians(ang))+x0, '.4f'), format(rc*math.sin(math.radians(ang))+y0, '.4f')])

    fp.write(tabulate(angles, headers=header))

    print('\nYour data is on {0}'.format(ofile))


main()