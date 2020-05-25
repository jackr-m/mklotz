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
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

ofile = "GEAR.OUT"

def main():
    cls()
    print('GEAR CALCULATIONS\n')
    fp = open(ofile, 'w')

    N = []
    OD = []
    A = []
    D = []
    W = []
    CP = []
    T = []
    PD = []
    RB = []
    RP = []

    NG = 2

    N.append(int(vin("Number of teeth on gear 1", 45)))
    N.append(int(vin("Number of teeth on gear 2", 20)))
    P = float(vin("Diametral Pitch (25.4/mod)", 20))
    phi = float(vin("Pressure Angle", 20, units="deg"))

    gratio = max(N)/min(N)
    
    for i in range(0, NG):
        OD.append((N[i]+2)/P)
        A.append(1/P)
        D.append(1.2/P)
        W.append(2.2/P)
        CP.append(math.pi/P)
        T.append(0.48*CP[i])
        PD.append(N[i]/P)
        RB.append(0.5*PD[i]*cosd(phi))
        RP.append(0.5*PD[i]*sind(phi))
    CD = 0.5*(PD[0]+PD[1])

    fp.write("\nFOR BOTH GEARS:\n")
    fp.write("Gear ratio = {0}:1\n".format(gratio))
    fp.write("Diametral Pitch = {0}\n".format(P))
    fp.write("Pressure Angle = {0} deg\n".format(phi))
    fp.write("Center Distance = {0} in = {1} mm\n".format(round(CD,4),round(CD*25.4,4)))

    for i in range(0,NG):
        fp.write("\nFOR GEAR {0}:\n".format(i+1))
        fp.write("Number of teeth = {0}\n".format(N[i]))
        fp.write("Outside Diameter = {0} in = {1} mm\n".format(round(OD[i],4),round(OD[i]*25.4,4)))
        fp.write("Addendum = {0} in = {1} mm\n".format(round(A[i],4),round(A[i]*25.4,4)))
        fp.write("Dedendum = {0} in = {1} mm\n".format(round(D[i],4),round(D[i]*25.4,4)))
        fp.write("Whole Depth = {0} in = {1} mm\n".format(round(W[i],4),round(W[i]*25.4,4)))
        fp.write("Circular Pitch = {0} in = {1} mm\n".format(round(CP[i],4),round(CP[i]*25.4,4)))
        fp.write("Tooth Thickness = {0} in = {1} mm\n".format(round(T[i],4),round(T[i]*25.4,4)))
        fp.write("Pitch Diameter = {0} in = {1} mm\n".format(round(PD[i],4),round(PD[i]*25.4,4)))
        fp.write("Base Circle Radius = {0} in = {1} mm\n".format(round(RB[i],4),round(RB[i]*25.4,4)))
        fp.write("Tooth Profile Radius = {0} in = {1} mm\n".format(round(RP[i],4),round(RP[i]*25.4,4)))

    print('\nYour data is on: {0}'.format(ofile))
    fp.close()

main()