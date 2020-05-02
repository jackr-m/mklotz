import math

def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")

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
    

main()