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
    """Create an input prompt with a default answer
    
    Add a "units" tag for kwargs for units to be included"""
    units = kwargs.get('units', None)
    if (units == None):
        input_prompt = prompt + ' [' + str(defvalue) + '] ? '
    else:
        input_prompt = prompt + ' [' + str(defvalue) + '] ' + str(units) + ' ? '
    return (input(input_prompt) or defvalue)

def main():
    cls()
    print('TEMPERATURE CONVERTOR\n')

    c0=273.18 #Celius to Kelvin
    r0=1.8*c0-32 #0k = 0r

    print("Temperature scales are")
    print("  Centigrade")
    print("  Fahrenheit")
    print("  Kelvin")
    print("  Rankine")
    print("  rEaumur")

    def agin():
        print('Enter temperature as 123.45x, x=c,[f],k,r,e or q(uit)')
        
    




    agin()

main()