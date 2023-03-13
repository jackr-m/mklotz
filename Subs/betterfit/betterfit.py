import math
import json

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
    fits = json.load(open('betterfits.json', 'r'))

    print(fits)


main()