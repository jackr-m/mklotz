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

def closest(lst, K):
    return lst[min(range(len(lst)), key = lambda i: abs(lst[i]-K))] 

wirefile = "WIREGAGE.DAT"
sheetfile = "SHEETGAGE.DAT"

def main():
    cls()
    print('WIRE AND SHEET GAGE UTILITY\n')

    ws = (input("[W]ire or (S)heet ? ") or "w")
    gs = (input("Find (G)age or [S]ize ? ") or "s")

    rdata(ws, gs)

def rdata(wire_or_sheet, gage_or_size):
    if (wire_or_sheet == "w"):
        fp = open(wirefile)
    elif (wire_or_sheet == "s"):
        fp = open(sheetfile)
    


main()