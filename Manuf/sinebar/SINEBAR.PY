import math
from decimal import getcontext, Decimal, ROUND_HALF_UP
import os
import sys

def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")

"""

This is an attempted rewrite of Marv Klotz's "SINEBAR" program, which calculates the height stack required to generate a particular angle when using a sine bar.

"""

blk = [4.000,3.000,2.000,1.000,0.950,0.900,0.850,0.800,0.750,0.700,0.650,0.600,0.550,0.500,0.450,0.400,0.350,0.300,0.250,0.200,0.150,0.149,0.148,0.147,0.146,0.145,0.144,0.143,0.142,0.141,0.140,0.139,0.138,0.137,0.136,0.135,0.134,0.133,0.132,0.131,0.130,0.129,0.128,0.127,0.126,0.125,0.124,0.123,0.122,0.121,0.120,0.119,0.118,0.117,0.116,0.115,0.114,0.113,0.112,0.111,0.110,0.109,0.108,0.107,0.106,0.105,0.104,0.103,0.102,0.101,0.1009,0.1008,0.1007,0.1006,0.1005,0.1004,0.1003,0.1002,0.1001,0.1000,0.050]
nb = len(blk)

LMAX = 10
index = [LMAX]
llimit = [LMAX]
ulimit = [LMAX]
inc = [LMAX]
nlm = 5
nl = None

def main():
    cls()
    print('SINEBAR CALCULATONS\n')

    f = 0
    for i in (0, nb - 2):
        if (blk[i] < blk[i+1]):
            blk[i] = blk[i+1]
            t = blk[i+1]
            f = 1

    bl = (round(sum(blk), 7))

    roll = float(input('Distance between sine bar rolls [5]') or 5)

    op = input('Angle input mode [d]ecimal degrees, (x) deg/min/sec ? ') or "d"

    if (op == "x"):
        td = int(input('Degrees ? ') or 30)
        tm = int(input('Minutes ? ') or 7)
        ts = int(input('Seconds ? ') or 30)
        theta = td + (tm/60) + (ts/3600)
    else:
        theta = float(input('Angle in decimal degrees ? '))

    print('\nDistance between rolls = {0}\n'.format(roll))
    print('Angle = {0} deg\n'.format(theta))
    stack = round(roll*math.sin(math.radians(theta)), 7)
    print('Stack height = {0}\n'.format(stack))
    print('Stack height measured in same units as roll separation.')

    if (theta < 90):
        es=round(math.degrees(0.001)/(roll*math.cos(math.radians(theta))), 5)
        er=round(math.degrees(0.001*stack)/(roll*roll*math.cos(math.radians(theta))), 5)
    print('A .001 error in the roll distance will cause an error of {0}'.format(er))
    print('A .001 error in the roll distance will cause an error of {0}'.format(es))

    if (stack < blk[nb-1]):
        print('Required stack size smaller than smallest available block!\n')
        exit()
    elif (stack > bl):
        print('Required stack size larger than total length of all blocks!\n')
        exit()
    
    blk81(stack)

def blk81(stack):
    x = stack

    x = float(truncate(x, 4))

    print('\nBlocks from standard 81 gage block set needed to form stack = {0} in:\n\n'.format(x))

    """
    kblk = [None]
    for i in (0, nb):
        kblk.append(0)

    #skips over blocks bigger than spacer required
    k=0
    for j in (0, nb -1):
        if (blk[j] <= x):
            k=j
            break
    #set loop limits and increments
    for l in (0, nlm -1):
        llimit[l]=k
        ulimit[l]=nb
        inc[l] = 1

    #looping
    for nl in (1, nlm):
        #initialize loop indicies
        for l in (0, nl):
            index[l]=llimit[l]
            
            #can't use the same block twice
            for j in (0, nl):
                for i in (0, nl):
                    if (i != j & index[i] == index[j]):
                        next
            
            #nested loop processing
            t=0
            for l in (0, nl):
                t+=blk[index[l]]
                if ((t-x) > 1.e-8):
                    next
            d=abs(t-x)
            """
            #print('t,x,d: {0},{1},{2}'.format(t,x,d))
    
    print(minCoins(blk, nb, int(x)))
            

def truncate(f, n):
    '''Truncates/pads a float f to n decimal places without rounding'''
    s = '{}'.format(f)
    if 'e' in s or 'E' in s:
        return '{0:.{1}f}'.format(f, n)
    i, p, d = s.partition('.')
    return '.'.join([i, (d+'0'*n)[:n]])

#from https://stackoverflow.com/questions/44213144/coin-change-with-limited-coins-complexity
def getchange(coins, amount):
    minCount = None

    def recurse(amount, coinIndex, coinCount):
        nonlocal minCount
        if amount == 0:
            if minCount == None or coinCount < minCount:
                minCount = coinCount
                return [] # success
            return None # not optimal
        if coinIndex >= len(coins):
            return None # failure
        bestChange = None
        coin = coins[coinIndex]
        # Start by taking as many as possible from this coin
        cantake = min(amount // coin["value"], coin["count"])
        # Reduce the number taken from this coin until 0
        for count in range(int(cantake), -1, -1):
            # Recurse, taking out this coin as a possible choice
            change = recurse(amount - coin["value"] * count, coinIndex + 1, 
                                                             coinCount + count)
            # Do we have a solution that is better than the best so far?
            if change != None: 
                if count: # Does it involve this coin?
                    change.append({ "value": coin["value"], "count": count })
                bestChange = change # register this as the best so far
        return bestChange

    return recurse(amount, 0, 0)

def minCoins(coins, m, V): 
      
    # table[i] will be storing the minimum  
    # number of coins required for i value.  
    # So table[V] will have result 
    table = [0 for i in range(V + 1)] 
  
    # Base case (If given value V is 0) 
    table[0] = 0
  
    # Initialize all table values as Infinite 
    for i in range(1, V + 1): 
        table[i] = sys.maxsize 
  
    # Compute minimum coins required  
    # for all values from 1 to V 
    for i in range(1, V + 1): 
          
        # Go through all coins smaller than i 
        for j in range(m): 
            if (coins[j] <= i): 
                sub_res = table[int(i - coins[int(j)])] 
                if (sub_res != sys.maxsize and 
                    sub_res + 1 < table[i]): 
                    table[i] = sub_res + 1
    return table[V]


blk_for_coins = [{ "value": 4.000, "count": 1},
                 { "value": 3.000, "count": 1},
                 { "value": 2.000, "count": 1},
                 { "value": 1.000, "count": 1},
                 { "value": 0.950, "count": 1},
                 { "value": 0.900, "count": 1},
                 { "value": 0.850, "count": 1},
                 { "value": 0.800, "count": 1},
                 { "value": 0.750, "count": 1},
                 { "value": 0.700, "count": 1},
                 { "value": 0.650, "count": 1},
                 { "value": 0.600, "count": 1},
                 { "value": 0.550, "count": 1},
                 { "value": 0.500, "count": 1},
                 { "value": 0.450, "count": 1},
                 { "value": 0.400, "count": 1},
                 { "value": 0.350, "count": 1},
                 { "value": 0.300, "count": 1},
                 { "value": 0.250, "count": 1},
                 { "value": 0.200, "count": 1},
                 { "value": 0.150, "count": 1},
                 { "value": 0.149, "count": 1},
                 { "value": 0.148, "count": 1},
                 { "value": 0.147, "count": 1},
                 { "value": 0.146, "count": 1},
                 { "value": 0.145, "count": 1},
                 { "value": 0.144, "count": 1},
                 { "value": 0.143, "count": 1},
                 { "value": 0.142, "count": 1},
                 { "value": 0.141, "count": 1},
                 { "value": 0.140, "count": 1},
                 { "value": 0.139, "count": 1},
                 { "value": 0.138, "count": 1},
                 { "value": 0.137, "count": 1},
                 { "value": 0.136, "count": 1},
                 { "value": 0.135, "count": 1},
                 { "value": 0.134, "count": 1},
                 { "value": 0.133, "count": 1},
                 { "value": 0.132, "count": 1},
                 { "value": 0.131, "count": 1},
                 { "value": 0.130, "count": 1},
                 { "value": 0.129, "count": 1},
                 { "value": 0.128, "count": 1},
                 { "value": 0.127, "count": 1},
                 { "value": 0.126, "count": 1},
                 { "value": 0.125, "count": 1},
                 { "value": 0.124, "count": 1},
                 { "value": 0.123, "count": 1},
                 { "value": 0.122, "count": 1},
                 { "value": 0.121, "count": 1},
                 { "value": 0.120, "count": 1},
                 { "value": 0.119, "count": 1},
                 { "value": 0.118, "count": 1},
                 { "value": 0.117, "count": 1},
                 { "value": 0.116, "count": 1},
                 { "value": 0.115, "count": 1},
                 { "value": 0.114, "count": 1},
                 { "value": 0.113, "count": 1},
                 { "value": 0.112, "count": 1},
                 { "value": 0.111, "count": 1},
                 { "value": 0.110, "count": 1},
                 { "value": 0.109, "count": 1},
                 { "value": 0.108, "count": 1},
                 { "value": 0.107, "count": 1},
                 { "value": 0.106, "count": 1},
                 { "value": 0.105, "count": 1},
                 { "value": 0.104, "count": 1},
                 { "value": 0.103, "count": 1},
                 { "value": 0.102, "count": 1},
                 { "value": 0.101, "count": 1},
                 { "value": 0.1009, "count": 1},
                 { "value": 0.1008, "count": 1},
                 { "value": 0.1007, "count": 1},
                 { "value": 0.1006, "count": 1},
                 { "value": 0.1005, "count": 1},
                 { "value": 0.1004, "count": 1},
                 { "value": 0.1003, "count": 1},
                 { "value": 0.1002, "count": 1},
                 { "value": 0.1001, "count": 1},
                 { "value": 0.1000, "count": 1},
                 { "value": 0.050, "count": 1},
]
"""coins = [{ "value": 10, "count":  1 },
         { "value":  2, "count":  1 },
         { "value":  0.5, "count": 1 }]"""

main()