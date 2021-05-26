
def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")

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
    #cls()
    print('BASIC FIT CALCULATOR WITH MMC/LMC AND CLEARANCE/ALLOWANCE')
    print('MMC = MAX MATERIAL CONDITION, LMC = LEAST MATERIAL CONDITION')
    print('ENTER ANY TWO KNOWNS (HOLE/SHAFT/FIT)')
    print('LEAVE UNKNOWNS AS ?')
    print('\nNOTE: Hole LMC will be the LARGER value')
    print('NOTE: Clearance = Loosest fit & Allowance = tighest fit')

    global hole_lmc
    global hole_mmc
    global shaft_lmc
    global shaft_mmc
    global clearance
    global allowance

    def disp():
        print("\n\nHole LMC = {0} in".format(str(round(hole_lmc,4))))
        print("Hole MMC = {0} in".format(str(round(hole_mmc,4))))
        print("Shaft LMC = {0} in".format(str(round(shaft_lmc,4))))
        print("Shaft MMC = {0} in".format(str(round(shaft_mmc,4))))
        print("Clearance = {0} in".format(str(round(clearance,4))))
        print("Allowance = {0} in\n".format(str(round(allowance,4))))

    #Hole size
    hole_lmc = vin("Hole LMC", "?", units="in")
    hole_mmc = vin("Hole MMC", "?", units="in")
    if hole_lmc == "?" and hole_mmc != "?":
        print("Both hole values must either a number or ?")
        return
    elif hole_lmc != "?" and hole_mmc == "?":
        print("Both hole values must either a number or ?")
        return
    
    if hole_lmc != "?":
        hole_lmc = float(hole_lmc)
    if hole_mmc != "?":
        hole_mmc = float(hole_mmc)

    #Shaft size
    shaft_lmc = vin("shaft LMC", "?", units="in")
    shaft_mmc = vin("shaft MMC", "?", units="in")
    if shaft_lmc == "?" and shaft_mmc != "?":
        print("Both shaft values must either a number or ?")
    elif shaft_lmc != "?" and shaft_mmc == "?":
        print("Both shaft values must either a number or ?")
    
    if shaft_lmc != "?":
        shaft_lmc = float(shaft_lmc)
    if shaft_mmc != "?":
        shaft_mmc = float(shaft_mmc)

    #Clearance/allowance
    clearance = vin("Clearance", "?", units="in")
    allowance = vin("Allowance", "?", units="in")
    if clearance == "?" and allowance != "?":
        print("Both values must either a number or ?")
    elif clearance != "?" and allowance == "?":
        print("Both values must either a number or ?")
    
    print("\n\n" + clearance + "\n\n")
    if clearance != "?":
        clearance = float(clearance)
    if allowance != "?":
        allowance = float(allowance)



    #Calculations
    if hole_lmc == "?" and shaft_lmc != "?" and clearance !="?":
        hole_lmc = clearance + shaft_lmc
        hole_mmc = allowance + shaft_mmc
        disp()
        return
    elif shaft_lmc == "?" and hole_lmc != "?" and clearance !="?":
        shaft_lmc = hole_lmc - clearance
        shaft_mmc = hole_mmc - allowance        
        disp()
        return
    elif clearance == "?" and hole_lmc != "?" and shaft_lmc != "?":
        clearance = hole_lmc - shaft_lmc
        allowance = hole_mmc - shaft_mmc
        disp()
        return
    else:
        print("You must enter at least 2 items! (4 values total)")
        return


main()