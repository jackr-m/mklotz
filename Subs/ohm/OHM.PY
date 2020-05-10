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

phase = None
voltage = None
current = None
impedance = None
power = None
cp = None
rz = None

def main():
    cls()
    print("OHM'S LAW CALCULATIONS\n")

    global rz
    rz = "Resistance"
    global phase
    global voltage
    global current
    global impedance
    global power
    global cp

    phase = float(vin("Phase angle (0 for DC calculations", 0, units="deg"))
    cp = cosd(phase)
    if (phase != 0):
        rz = "Impedance"
    print('\nInput whatever data you know; press retun if not known.')
    print('You must enter two data items to obtain a solution.\n')
    k=0
    voltage = float(vin("Voltage", 0, units="volts"))
    if (voltage != 0):
        k+=1
    current = float(vin("Current", 0, units="amps"))
    if (current != 0):
        k+=1
    if (k == 2):
        done()
        return
    impedance = float(vin(rz, 0, units="ohms"))
    if (impedance != 0):
        k+=1
    if (k == 2):
        done()
        return
    power = float(vin("Power", 0, units="watts"))
    if (power != 0):
        k+=1
    if (k == 2):
        done()
        return
    print('\nINSUFFICIENT DATA FOR SOLUTION')

def done():
    global phase
    global voltage
    global current
    global impedance
    global power
    global cp
    global rz

    def disp():
        print('\nVoltage = {0} volts'.format(voltage))
        print('\nCurrent = {0} amps'.format(current))
        print(rz + ' = {0} ohms'.format(impedance))
        print('Power = {0} watts'.format(power))

    if (voltage != 0 and current != 0):
        impedance=voltage/current
        power=voltage*current*cp
        disp()
        return
    elif (voltage != 0 and impedance != 0):
        current=voltage/impedance
        power=voltage*current*cp
        disp()
        return
    elif (voltage != 0 and power != 0):
        current=power/(voltage*cp)
        impedance=voltage/current
        disp()
        return
    elif (current != 0 and impedance != 0):
        voltage=current*impedance
        power=voltage*current*cp
    elif (current != 0 and power != 0):
        voltage=power/(current*cp)
        impedance=voltage/current
    elif (impedance != 0 and power != 0):
        current=math.sqrt(math.fabs(power/(impedance*cp)))
        voltage=current*impedance
    disp()

main()