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

c=12/math.pi

dfile = "DIAMPY.DAT"
ofile = "DIAMPY.OUT"

fp = open(ofile, "w")


def main():
    cls()
    print('MACHINING DIAMETER UTILITY\n')

    rdata()
    print('number of data entries read = ')

def rdata():
    temp = []
    nd=0 # number of data entries
    k=0
    f=0
    ns=0

    


main()