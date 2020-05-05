import math

def cls():
    print("\x1B\x5B2J", end="")
    print("\x1B\x5BH", end="")


mpi=25.4

def main():
    cls()
    print('3 WIRE THREAD MEASUREMENTS (60 degree threads)\n')

    op = input('(M)etric or [I]mperial threads?') or "I"
    if (op == "m"):
        met = 1
    else:
        met = 0

    ta=float(60)
    a=0.5*ta

    if (met):
        tpi = float(input('Thread pitch [1 mm] ? ') or 1)
        p=tpi/mpi
    else:
        tpi = float(input('Thread pitch [20 tpi] ? ') or 20)
        p=1/tpi

    print('\nMost common threads are single start.')
    s = int(input('Number of starts [1] ? ') or 1)
    t=0.5*p
    w=round(t/(math.cos(math.radians(a))),4)
    wmet=round(w*mpi,4)

    print('\nBest wire diameter to use = {0} mm = {1} in\n'.format(wmet,w))

    if (met):
        w = float((input('Wire diameter used [' + str(wmet) + ' mm]') or wmet))
        w/=mpi
    else:
        w = float((input('Wire diameter used [' + str(w) + ' in] ? ') or w))

    z=((3/16)*p)/math.tan(math.radians(a))
    print('\n')

    if (not met):
        print('For American National Thread form, subtract {} in from'.format(round(2*z,4)))
        print('major diameter (assumes p/8 flat on crest) to obtain pitch diameter.\n')

    if (met):
        md = float((input('Major diameter of thread [6 mm] ? ') or 6))
        md/=mpi
    else:
        md = float((input('Major diameter of thread [0.25 in] ? ') or 0.25))
    
    print('pd = pitch diameter, mow = measurement over wires')
    op = (input('Calculate pd from mow (p) or mow from pd [m]?') or 'm')
    if (op == 'p'):
        pd = 1
    else:
        pd = 0

    ed = md-2*z

    e = ed
    b=(p*s)/(math.pi*e)
    q=math.tan(math.radians(a))
    #x=-t/(q+w*(1+(1/(math.sin(math.radians(a)+0.5*b*b*math.cos(math.radians(a)))/q))))
    x=-t/q+w*(1.+1./math.sin(math.radians(a))+0.5*b*b*math.cos(math.radians(a))/q)

    if (not pd):
        if (met):
            e = float((input('Pitch diameter of thread [' + str(round(ed*mpi,4)) + ' mm] ? ') or e))
            e/=mpi
        else:
            e = float((input('Pitch diameter of thread [' + str(round(ed,4)) + ' in] ? ') or e))
    else:
        if (met):
            m = float((input('Measurement over wires [' + str(round((e+x)*mpi,4)) + ' mm] ? ') or ((e+x)*mpi)))
            m/=mpi
        else:
            m = float((input('Measurement over wires [' + str(round(e+x,4)) + ' in] ? ') or (e+x)))

    if (not pd):
        print('\nMeasurement over wires = {0} mm = {1} in\n'.format(round((e+x)*mpi,4),round(e+x,4)))
    else:
        print('\nPitch diameter = {0} mm = {1} in\n'.format(round((m-x)*mpi,4),round(m-x,4)))

main()