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
    """Create an input prompt with a defualt answer
    
    Add a "units" tag for kwargs for units to be included"""
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

def main():
    cls()
    print('KEYWAY CALCULATIONS\n')

    print("Shaft Diameter   Recommended Key Size\n")
    print("0.3125-0.4375    0.0625-0.1250")
    print("0.4375-0.5000    0.0625-0.1562")
    print("0.5000-0.8750    0.0938-0.3125")
    print("0.8750-1.1250    0.1250-0.4375")
    print("1.1250-1.1500    0.1562-0.5000")
    print("1.5000-2.0000    0.1875-0.5000\n")

    d = float(vin("Shaft diameter", 1, units="in"))
    k = float(vin("Key width", 0.25, units="in"))
    h = float(vin("Key height", 0.3, units="in"))

    x=math.sqrt(d*d-k*k)
    y=0.5*(d-x)
    m=d-(y+0.5*h)
    n=m+h
    q=d-m
    r=h-q

    print("\nShaft diameter (D)         = {0} in".format(d))
    print("Key width (K)              = {0} in".format(k))
    print("Key height (H)             = {0} in".format(h))
    print("Keyseat depth (M)          = {0} in".format(round(m,4)))
    print("Keyway depth (N)           = {0} in".format(round(n,4)))
    print("Depth of cut in shaft (Q)  = {0} in".format(round(q,4)))
    print("Depth of cut in hub (R)    = {0} in".format(round(r,4)))

    print('\nKeyseat depth measured bottom of keyseat to opposite side of shaft on cl.')
    print('Keyway depth measured top of hub keyway to opposite side of hole on cl.')

main()