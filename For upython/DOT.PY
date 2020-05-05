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
    print('DEPTH OF THREAD CALCULATIONS\n')

    ta = float(vin("Threads angle", 60, units="deg"))
    tpi = float(vin("Threads per inch", 20))
    rang = float(vin("Compound rest angle", 29.5, units="deg"))
    
    cang = math.cos(math.radians(rang))
    pitch = 1/tpi
    h = dotscsr = 0.5*pitch/tand(0.5*ta) #dot sharp crest to sharp root
    dotfcfr=0.625*h #dot flat crest to flat root
    dotscfr=0.75*h #dot sharp crest to flat root
    dotfcsr=0.875*h #dot flat crest to sharp root
    undercut=0.25*h #undercut below nominal diameter for class 2A/2B threads

    print('\nthread angle = {0} deg'.format(ta))
    print('threads per inch = {0}'.format(tpi) + '\t(pitch = {0} in/thread)'.format(pitch))
    print('{{compound feed at compound angle = {0} deg}}\n'.format(rang))
    
    print('(A) dot sharp crest - sharp root = {0} in {{{1} in}}'.format(round(dotscsr,4),round(dotscsr/cang,4)))

    if (not(ta == 60)):
        print('\n***********************')
        print('NB... For thread forms other than American National, the following')
        print('information may not be valid unless the root and crest flats are defined')
        print('as in the American National form.')
        print('***********************\n')

    print('(B) dot flat  crest - flat  root = {0} in {{{1} in}}'.format(round(dotfcfr,4),round(dotfcfr/cang,4)))
    print('(C) dot sharp crest - flat  root = {0} in {{{1} in}}'.format(round(dotscfr,4),round(dotscfr/cang,4)))
    print('(D) dot flat  crest - sharp root = {0} in {{{1} in}}'.format(round(dotfcsr,4),round(dotfcsr/cang,4)))
    print('(E) double dot sharp crest - sharp root = {0} in'.format(round(2*dotscsr,4)))

    z=(3/16)*pitch/tand(30) #distance of pitch line below p/8 flat
    print('\nFor American National (60 deg) thread form, subtract {0} in from'.format(round(2*z,4)))
    print('major diameter (assumes p/8 flat on crest) to obtain pitch diameter\n')

    intp = None

    fracp=math.modf(tpi)
    if (fracp[0]):
        print('use any odd-numbered line on threading dial')
    elif (int(fracp[1] % 2)):
        print('use any numbered line on threading dial')
    else:
        print('use any line on threading dial')

main()