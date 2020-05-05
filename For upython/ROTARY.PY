import math
import tabulate.py as tabulate
#from tabulate import tabulate

def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")

ofile = 'ROTARYPY.OUT'
fp = open(ofile, "w")

nd = None

def main():

    cls()
    print('ROTARY TABLE CALCULATIONS\n')

    global nd
    nd = int((input('Number of divisions [13] ? ') or 13))

    psol()

    print("\nYour data is on: {0}\n".format(ofile))

def psol():
    fp.write("Number of divisions = {0} \n".format(nd))
    fp.write("\n")

    header = ["DIVISON","degdec","deg","min","sec"]
    
    angles = []

    dtheta = 360/nd
    for k in range(0,nd+1):
        theta = k*dtheta
        deg=math.floor(theta)
        minutes=math.floor((theta-deg)*60)
        sec=math.floor(((theta-deg-minutes/60)*3600)+0.5)
        if (sec == 60):
            sec = 0
            minutes+=1
        if (minutes == 60):
            minutes =0
            deg+=1
        angles.append([k,round(theta,4),deg,minutes,sec])

    fp.write(tabulate(angles, headers=header))

main()