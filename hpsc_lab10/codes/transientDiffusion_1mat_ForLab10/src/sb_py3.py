#!/usr/bin/python3

import os

import sys
import getopt
import glob
import struct


# ==
# ||
# || Main Program
# ||
# ==

def sb(argv):

#    os.system('clear')
    
    print()
    print( '------------------------------------')
    print( '   sb.py                            ')
    print( '------------------------------------')
    print()
    
    # -
    # |
    # | Get user arguments
    # |
    # -

    readFile = 'NULL'
    nCellx   = 0
    nPEx     = 0
    
    try:
        opts, args = getopt.getopt(argv,"h f: c: n:")
      
    except:
       print( 'Error in command-line arguments.')
       exit(0)

    for opt, arg in opts:
        
        if opt == "-h":
            print( './sb_py3.py -f <binary file to be read> -c <no. cells in x/y dir *per PE*> -n <nPEx>')
            exit(0)
        
        if opt == "-c":
            nCellx = int(arg)
        
        if opt == "-n":
            nPEx  = int(arg)
        
        elif opt == "-f":
            readFile = arg

    # -
    # |
    # | Check user arguments
    # |
    # -

    if readFile == 'NULL':
        print("Fatal Error: Specify binary file with -f")
        exit(0)
        
    if nCellx == 0:
        print("Fatal Error: Specify no. cells in x/y direction with -c")
        exit(0)
        
    if nPEx == 0:
        print("Fatal Error: Specify no. PE in x direction with -n")
        exit(0)

    # -
    # |
    # | Specifications for reading the binary and writing to tty
    # |
    # -

    nx_total      = nPEx*nCellx + 1
    DecimalSize   = 8        # 4 for single precision (float) 8 for double precisioni (double)
    DecimalFormat = 'd'    # 'f' for single, 'd' for double

    # -
    # |
    # | Read binary file and print it to screen
    # |
    # -
    
    print()
    print( 'Read user specified file: ' + readFile)
    print( 'n points x-dir._total   : ' , nx_total)

    file = open(readFile, "rb")
    
    print()
    print( "Matrix Read from Binary File:")
    print( "---------------------------------------------------")
    print()

    # Read first data
    
    Num  = file.read(DecimalSize)

    count_x_total = 0

    # Continue reading data until end of file
    
    while Num:

        # Read and print data with no line feed
        
        floatValue = struct.unpack(DecimalFormat,Num)[0]   # 'f' for single, 'd' for double
        print( "{:5.1f}".format(floatValue),end='')

        # Put line feed at the end of the mesh

        count_x_total += 1
        if count_x_total == nx_total:
            print()
            count_x_total = 0
            
        Num  = file.read(DecimalSize)

    
    file.close()


    
    
    print()
    print()

    

if __name__ == "__main__":
    sb(sys.argv[1:])



