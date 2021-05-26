import math
import re

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

def closest(lst, K):
    return lst[min(range(len(lst)), key = lambda i: abs(lst[i]-K))] 

wirefile = "WIREGAGE.DAT"
sheetfile = "SHEETGAGE.DAT"

gages = []
sizesstr = []
sizes = []

def main():
    cls()
    print('WIRE AND SHEET GAGE UTILITY\n')

    ws = (input("[W]ire or (S)heet ? ") or "w")
    gs = (input("Find (G)age or [S]ize ? ") or "s")

    print(rdata(ws, gs))

def rdata(wire_or_sheet, gage_or_size):
    if (wire_or_sheet == "w"):
        fp = open(wirefile)
    elif (wire_or_sheet == "s"):
        fp = open(sheetfile)
    
    global gages
    global sizesstr
    global sizes

    for i in fp:
        gages.append(re.search(r"\d{1,6},",i).group())
        sizesstr.append(re.search(r",(\d|.){1,7}",i).group())
    
    gages = [sub[ : -1] for sub in gages]
    sizesstr = [sub[1 : ] for sub in sizesstr]
    for i in sizesstr:
        sizes.append(float(i))
    #print(gages)
    #print(sizes)

    if (wire_or_sheet == "w"):
        if (gage_or_size == "s"):
            gage = input("Gage designation ? ")
            index = gages.index(gage)
            return('SIZE OF WIRE GAGE ' + gage + ' = {0} in = {1} mm'.format(sizes[index],round(sizes[index]*25.4,4)))
        
        if (gage_or_size == "g"):
            size = float(vin("Thickness",0.1, units="in"))
            closestSize = closest(sizes, size)
            index = sizes.index(closestSize)
            return('CLOSEST WIRE GAGE = {0} WITH SIZE {1} in = {2} mm'.format(gages[index],sizes[index],round(sizes[index]*25.4,4)))
    elif (wire_or_sheet == "s"):
        if (gage_or_size == "s"):
            gage = input("Gage designation ? ")
            index = gages.index(gage)
            return('SIZE OF SHEET GAGE ' + gage + ' = {0} in'.format(sizes[index],round(sizes[index]*25.4,4)))
        
        if (gage_or_size == "g"):
            size = float(vin("Thickness",0.1, units="in"))
            closestSize = closest(sizes, size)
            index = sizes.index(closestSize)
            return('CLOSEST SHEET GAGE = {0} WITH SIZE {1} in = {2} mm'.format(gages[index],sizes[index],round(sizes[index]*25.4,4)))


main()