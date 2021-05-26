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

def main():
    cls()
    print('GEAR PRESSURE ANGLE BY MEASUREMENT OF CHORDAL SPAN\n')

    n = int(vin("Number of teeth on gear", 30))
    dp = float(vin("Diametral pitch of gear", 6))

    pamax = 2
    pa = [14.5,20,25,30]

    r=0.5*n/dp
    t=0.5*math.pi/dp

    if ((n<12) | (n>110)):
        print('Not able to calculate number of teeth to measure across')
        return
    
    for i in range(0, pamax):
        p=pa[i]
        if (i == 0):
            if ((n >= 12) & (n<=18)):
                teeth=2
            if ((n >= 19) & (n<=37)):
                teeth=3
            if ((n >= 38) & (n<=50)):
                teeth=4
            if ((n >= 51) & (n<=62)):
                teeth=5
            if ((n >= 53) & (n<=75)):
                teeth=6
            if ((n >= 76) & (n<=87)):
                teeth=7
            if ((n >= 88) & (n<=100)):
                teeth=8
            if ((n >= 101) & (n<=110)):
                teeth=9
        if (i == 1):
            if ((n >= 12) & (n<=18)):
                teeth=2
            if ((n >= 19) & (n<=27)):
                teeth=3
            if ((n >= 28) & (n<=36)):
                teeth=4
            if ((n >= 37) & (n<=45)):
                teeth=5
            if ((n >= 46) & (n<=54)):
                teeth=6
            if ((n >= 55) & (n<=63)):
                teeth=7
            if ((n >= 64) & (n<=72)):
                teeth=8
            if ((n >= 73) & (n<=81)):
                teeth=9
        s=teeth-1

        f=2*(tand(p)-math.radians(p)) #twice the involute of pa
        print('Pressure angle = {0} deg...'.format(pa[i]))
        m=r*cosd(p)*(t/r+2.*math.pi*s/n+f) #chordal measurement
        if ((i == 0) & ((n<12) | (n>110))):
            print('Number of teeth out of range for 14.5 deg pa')
            continue
        if ((i == 1) & ((n<12) | (n>81))):
            print('Number of teeth out of range for 20 deg pa')
            continue
        """if ((i == 2) & ((n<12) | (n>60))):
            print('Number of teeth out of range for 25 deg pa')
            continue
        if ((i == 3) & ((n<12) | (n>50))):
            continue"""
        print("Chordal span over {0} teeth = {1} in".format(teeth,round(m,4)))

main()