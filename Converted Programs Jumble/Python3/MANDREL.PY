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
    print("Kozo Hiraoka's SPRING WINDING MANDREL DIAMETER CALCULATION\n")
    wt = (input("Wire type: [m]usic wire or phosphorus (b)ronze ? ") or "w")
    if (wt is not "b"):
        wt = "m"
        wtnum = 0
    else:
        wtnum = 1
    dw = float(vin("Wire diameter", 0.040, units="in"))
    india = float(vin("Spring inside diameter", 0.203, units="in"))
    
    c0 = [0.980364,0.012436]
    c1 = [-0.012436,-0.11018]
    
    ds = india+dw #spring average diameter
    fact = c0[wtnum]+c1[wtnum]*ds/dw #empirical factor
    dm = fact*ds-dw #mandrel diameter

    print('\nRecommended mandel diameter = {0} in'.format(round(abs(dm),4)))

main()