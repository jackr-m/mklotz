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

speed = None
edges = None
load = None
ipm = None
ipr = None

def main():
    cls()

    global speed
    global edges
    global load
    global ipm
    global ipr

    print('FEED RATE CALCULATIONS\n')

    print('Input whatever data you know; press return if not known.')
    print('You must enter 3 out of 5 data items to obtain a solution.')
    print('If finding speed from edges, chip load and feed, feed must be entered in in/min.\n')

    k = 0
    speed = float(vin("Speed", 0, units="rpm"))
    if (speed == 0):
        speed = None
    if (speed is not None):
        k+=1
    edges = float(vin("Number of cutting edges", 0, units="edges/revolution"))
    if (edges == 0):
        edges = None
    if (edges is not None):
        k+=1
    load = float(vin("Chip load", 0, units="in/cutting edge"))
    if (load == 0):
        load = None
    if (load is not None):
        k+=1
    if (k==3):
        doit()
        return
    ipm = float(vin("Feed", 0, units="in/min"))
    if (ipm == 0):
        ipm = None
    if (ipm is not None):
        k+=1
    if (k==3):
        doit()
        return
    if ((edges is not None) & (load is not None)):
        whoops()
    ipr = float(vin("Feed", 0, units="in/revolution"))
    if (ipr == 0):
        ipr = None
    if (ipr is not None):
        k+=1
    if (k==3):
        doit()
        return
    else:
        whoops()

def whoops():
    print('\nINSUFFICIENT DATA FOR SOLUTION')

def doit():
    global speed
    global edges
    global load
    global ipm
    global ipr

    if ((speed is not None) & (edges is not None) & (load is not None)):
        ipm=load*speed*edges
        ipr=ipm/speed
        disp()
        return
    
    if ((speed is not None) & (edges is not None) & ((ipr is not None) | (ipm is not None))):
        if ((ipm is not None)):
            ipr=ipm/speed
        if ((ipr is not None)):
            ipm=ipr*speed
        load=ipm/(speed*edges)
        disp()
        return
    
    if ((speed is not None) & (load is not None) & ((ipr is not None) | (ipm is not None))):
        if ((ipm is not None)):
            ipr=ipm/speed
        if ((ipr is not None)):
            ipm=ipr*speed
        edges=ipm/(load*speed)
        disp()
        return

    if ((edges is not None) & (load is not None) & (ipm is not None)):
        speed=ipm/(load*edges)
        ipr=ipm/speed
        disp()
        return


def disp():
    global speed
    global edges
    global load
    global ipm
    global ipr

    print('\nCutting edges = {0}'.format(round(edges,4)))
    print('Speed = {0} rpm'.format(round(speed,4)))
    print('Chip load = {0} in/edge'.format(round(load,4)))
    print('Feed = {0} in/min'.format(round(ipm,4)))
    print('Feed = {0} in/rev'.format(round(ipr,4)))

main()