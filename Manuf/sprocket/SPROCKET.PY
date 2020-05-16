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
    print('ANSI ROLLER CHAIN SPROCKET CALCULATION\n')
    
    n = int(vin("Number of teeth in sprocket", 9))
    p = float(vin("Chain pitch", 1, units="in"))
    rd = float(vin("Roller diameter", 0.25, units="in"))

    pd=p/sind(180/n)
    od=p*(0.6+1/tand(180/n))
    
    if (n % 2 == 0):
        cfact=pd*cosd(90/n)
        cd=cfact-rd
    else:
        cd=pd-rd
    mhd=p*(1/tand(180/n)-1)-0.030

    print('\nSprocket pitch diameter = {0} in'.format(round(pd,4)))
    print('Sprocket outside diameter = {0} in'.format(round(od,4)))
    if (n % 2 == 0):
        print('Caliper factor = {0} in'.format(round(cfact,4)))
    print('Sprocket caliper diameter = {0} in'.format(round(cd,4)))
    print('Maximum hub diameter = {0} in'.format(round(mhd,4)))

main()