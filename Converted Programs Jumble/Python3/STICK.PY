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

stick = None

def main():
    cls()
    print('THREAD CHASING OFFSETS\n')

    opl = (input('Type of leadscrew thread (M)etric or [I]mperial ? ') or "i")
    if (opl == 'm'):
        lsp = float(vin("Leadscrew pitch", 4, units="mm"))
        a=25.4/lsp
    elif (opl == 'i'):
        lsp = float(vin("Leadscrew pitch", 10, units="in"))
        a=lsp
    else:
        print('INVALID INPUT')
        return
    
    opt = (input('Type of thread [M]etric or (I)mperial ? ') or 'm')
    if (opt == 'i'):
        tp = float(vin("Pitch of thread being cut", 15, units="tpi"))
        b=tp
    elif (opt == 'm'):
        tp = float(vin("Pitch of thread being cut", 4, units="mm"))
        b=25.4/tp
    else:
        print('INVALID INPUT')
        return
    
    global stick

    # gonna move one leadscrew thread at a time - thus a*stick always an integer
    c=1/a
    stick=c
    low=0.000001
    high=1-low

    def done():
        global stick
        print('\nMove carriage {0} in = {1} mm to right to recapture thread'.format(round(stick,4),round(25.4*stick,4)))

    def agin():
        global stick
        f=math.modf(b*stick)
        if ((f[0] < low) | (f[0] > high)):
            done()
            return
        stick+=c
        agin()
    agin()

main()