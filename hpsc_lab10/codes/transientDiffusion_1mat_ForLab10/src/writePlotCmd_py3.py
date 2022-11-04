#!/usr/bin/python3

import os

import sys
import getopt
import glob



def WriteOnePCfile(PElist,timeList,varName):

    g = open('pc_' + varName,'w')
    
    for time in sorted(timeList):
        
        print( 'splot \\',file=g)
        for PE in sorted(PElist):
            print( '"'+varName+'_' + str(PE) + '_' + str(time) + '.plt" w l , \\',file=g)
        print( '',file=g)
        print( 'pause .01',file=g)
        
    g.close()


# ==
# ||
# || Main Program
# ||
# ==

def writePlotCmd(argv):



    # List of all ptcl files
    
    ptclList = glob.glob("phi*.plt")

    # Parse the filenames counting the number of PEs for this run

    PElist = []
    timeList = []
    
    for i in range(0,len(ptclList)):
        name = ptclList[i];
        tmp = name.split('_')

        PE  = tmp[1]
        if PE not in PElist:
            PElist.append(PE)

        tmp2 = tmp[2].split(".")
        time = int(tmp2[0])
        if time not in timeList:
            timeList.append(int(time))

            
    WriteOnePCfile(PElist,timeList,'phi')


if __name__ == "__main__":
    writePlotCmd(sys.argv[1:])
