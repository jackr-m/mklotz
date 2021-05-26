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

ss = None
sf = None
sl = None
sh = None
diff = None
beta1 = None
wrap1 = None
beta2 = None
wrap2 = None
span = None
blen = None
sep = None

def main():
    cls()
    print('TWO PULLEY CENTER DISTANCE CALCULATION\n')

    d1 = float(vin("Diameter of driven pulley", 1.4))
    r1 = 0.5*d1
    d2 = float(vin("Diameter of driven pulley", 0.603))
    r2 = 0.5*d2
    dlen = float(vin("Belt length", 8.21))
    acc = float(vin("Calculation accuracy", 0.0001))

    global sf
    global ss
    global diff
    ss=0.1*dlen
    sf=10*dlen
    diff=-1

    def done():
            global beta1
            global wrap1
            global beta2
            global wrap2
            global span
            global blen
            global sep

            print('\nFor known pulley:')
            print('Diameter = {0}'.format(round(d1,4)))
            print('Wrap angle = {0} deg'.format(round(math.degrees(beta1),2)))
            print('Wrap length = {0}'.format(round(wrap1,4)))
            print('\nFor calculated pulley:')
            print('Diameter = {0}'.format(round(d2,4)))
            print('Wrap angle = {0} deg'.format(round(math.degrees(beta2),2)))
            print('Wrap length = {0}'.format(round(wrap2,4)))
            print('\nBelt span between pulleys = {0}'.format(round(span,4)))
            print('Belt length = {0}'.format(round(blen,4)))
            print('Pulley center distance = {0}'.format(round(sep,4)))

    def again():
        global ss
        global sf
        global diff
        global beta1
        global wrap1
        global beta2
        global wrap2
        global span
        global blen
        global sep

        #sh = None

        sep = ss
        while (sep <= sf):
            difflast = diff
            eps=r2-r1
            phi=math.asin(eps/sep)
            theta=2*phi
            beta1=math.pi-theta
            beta2=math.pi+theta
            wrap1=r1*beta1
            wrap2=r2*beta2
            span=math.sqrt(sep*sep-eps*eps)
            blen=2*span+wrap1+wrap2
            diff=blen-dlen
            if (abs(diff) < acc):
                done()
                return
            if ((difflast < 0) & (diff > 0)):
                sh=sep
                break
            sl=sep
            sep+=0.1*(sf-ss)
        ss=sl
        sf=sh
        again()

    again()


main()