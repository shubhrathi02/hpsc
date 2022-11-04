#!/usr/bin/python

import os

import sys
import getopt
import glob
import struct


def printLine(nx_total):
    
    print
    for i in range(0,nx_total*4):
        print '-',


# ==
# ||
# || Main Program
# ||
# ==

def sb(argv):

#    os.system('clear')
    
    print
    print '------------------------------------'
    print '   sb.py                            '
    print '------------------------------------'
    print
    # -
    # |
    # | User arguments
    # |
    # -

    readFile = 'NULL'
    nCellx   = 0
    nPEx     = 0
    
    try:
        opts, args = getopt.getopt(argv,"h f: c: n:")
      
    except:
       print 'Error in command-line arguments.'
       exit(0)

    for opt, arg in opts:
        
        if opt == "-h":
            print './sb.py -f <binary file to be read>'
        
        if opt == "-c":
            nCellx = int(arg)
        
        if opt == "-n":
            nPEx  = int(arg)
        
        elif opt == "-f":
            readFile = arg

    # Form list for PE divisions

    nx = {}
    ny = {}
    for i in range(0,nPEx):
        nx[i] = nCellx
        ny[i] = nCellx

    nx[0] += 1
    ny[0] += 1

    nx_total = 0
    for i in range(0,nPEx):
        nx_total += nx[i]

    print
    print 'Read user specified file: ' 
    print '-------------------------------------'
    print
    print 'File name = ' + readFile
    print

    file = open(readFile, "rb")
    
    # Read matrix:
    
    DecimalSize = 8        # 4 for single precision (float) 8 for double precisioni (double)
    DecimalFormat = 'd'    # 'f' for single, 'd' for double
    
    print
    print
    print "Matrix:"
    print "---------------------------------------------------"

    Num  = file.read(DecimalSize)

    count_x       = 0
    count_x_total = 0
    count_y = 0
    iPE     = 0
    jPE     = 0
    print "nx_total = " , nx_total
    
    printLine(nx_total)
    print
                
    while Num:
        
        floatValue = struct.unpack(DecimalFormat,Num)[0]   # 'f' for single, 'd' for double
        print "{:5.1f}".format(floatValue),

        # Print horizontal divider
        
        count_x       += 1
        count_x_total += 1

        if count_x == nx[iPE]:
            print ' | ',
            count_x = 0
            iPE += 1
            
        # Print vertical divider
            
        if count_x_total == nx_total:
            count_y      += 1
            count_x_total = 0
            iPE           = 0

            if count_y == ny[jPE]:
                printLine(nx_total)

                count_y = 0
                jPE    += 1
                if jPE >= nPEx: jPE = nPEx-1
            print

            
        Num  = file.read(DecimalSize)

    
    file.close()


    
    
    print
    print

    

if __name__ == "__main__":
    sb(sys.argv[1:])



