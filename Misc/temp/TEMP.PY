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
        temp_input = input()
    
        if (temp_input == "q"):
            return
        else:
            scale = temp_input[-1:]
            #temp = temp_input[:-1]
            temp = temp_input

            try:
                int(scale)
                scale = "f"
            except ValueError:
                pass

            try:
                temp = float(temp)
            except ValueError:
                try: 
                    temp = temp_input[:-1]
                    temp = float(temp)
                except ValueError:
                    print("Error with temperature format!")
                    agin()
                    return

            if ((temp < 0) & ((scale == 'k') | (scale == 'r'))):
                print("Absolute temperatures are always positive!")
            
            if (scale == "k"):
                tk=temp
                tc=tk-c0
                tf=1.8*tc+32.
                tr=tf+r0
                te=tc*0.8
            elif (scale == "c"):
                tc=temp
                tf=1.8*tc+32.
                tr=tf+r0
                tk=tc+c0
                te=tc*0.8
            elif (scale == "r"):
                tr=temp
                tf=tr-r0
                tc=(tf-32.)/1.8
                tk=tc+c0
                te=tc*0.8
            elif (scale == "f"):
                tf=temp
                tr=tf+r0
                tc=(tf-32.)/1.8
                tk=tc+c0
                te=tc*0.8
            elif (scale == "e"):
                te=temp
                tc=te/0.8
                tf=1.8*tc+32.
                tr=tf+r0
                tk=tc+c0
            else:
                print("INVALID FORMAT")
                agin()
                return

            print("\nCentigrade = {0}".format(round(tc,2)))
            print("Fahrenheit = {0}".format(round(tf,2)))
            print("Kelvin     = {0}".format(round(tk,2)))
            print("Rankine    = {0}".format(round(tr,2)))
            print("Reaumur    = {0}\n".format(round(te,2)))

            agin()


    agin()

main()