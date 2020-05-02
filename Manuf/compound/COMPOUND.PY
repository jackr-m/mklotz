import math

def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")

def vin(prompt, defvalue):
    return (input(prompt + ' [' + str(defvalue) + '] ? ') or defvalue)

ratio = None

def main():
    cls()
    print('SETTING COMPOUND REST ANGLE\n')

    print('Ratio is infeed movement/slide movement')

    def agin():
        global ratio
        #ratio = float(vin("Required movement ratio", 0.1))
        ratio = float((input('Required movement ratio [0.1] ? ') or 0.1))
        if (ratio > 1):
            print("Ratio must be <= 1")
            agin()

    agin()
    
    angle=round(math.asin(math.radians(ratio)), 4)

    print('Set compound angle to {0} deg = '.format(angle) + dms(angle))
    print('Complement of this angle = {0} deg = '.format(90-angle) + dms(90-angle))

def dms(theta):

    sgn=''
    if (theta < 0):
        sgn = '-'

    deg=math.floor(theta)
    minutes=math.floor((theta-deg)*60)
    sec=math.floor(((theta-deg-minutes/60)*3600)+0.5)
    if (sec == 60):
        sec = 0
        minutes+=1
    if (minutes == 60):
        minutes =0
        deg+=1
    
    return "{0}{1} deg, {2} min, {3} sec".format(sgn,deg,minutes,sec)


main()