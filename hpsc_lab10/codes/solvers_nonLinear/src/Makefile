
# -----------------------------------------------------
# On local Ubuntu
# -----------------------------------------------------
#
# Be sure to use MPI compiler wrappers.
# Whenever compiling an MPI program, you should use the MPI wrappers:
#
#    C - mpicc
#    C++ - mpiCC, mpicxx, mpic++
#    FORTRAN - mpifort, mpif77, mpif90
#
# These wrappers do all of the dirty work for you of making sure that
# all of the appropriate compiler flags, libraries, include directories,
# library directories, etc. are included when you compile your program.


# -----------------------------------------------------
# On Summit and Alpine:
# -----------------------------------------------------
#
# module load intel
# module load impi

# -----------------------------------------------------
# Set compiler based on platform
# -----------------------------------------------------

# Default compiler is Intel MPI compiler

COMPILER := mpicc

# Default ADVINCLUDE path is for Alpine
ADVINCLUDE := /curc/sw/install/intel/2022.1.2/advisor/2022.0.0/include

# Change compiler and ADVINCLUDE if on my local system

ifeq ($(strip $(LOGNAME)),scott)
COMPILER := mpic++ 
ADVINCLUDE := /home/scott/intel/oneapi/advisor/2022.3.0/include 
endif

# Redundant:  Option for when on Alpine

ifeq ($(strip $(MODULEPATH_ROOT)),/curc/sw/alpine-modules)
ADVINCLUDE := /curc/sw/install/intel/2022.1.2/advisor/2022.0.0/include
endif

# Option for when on Summit

ifeq ($(strip $(MODULEPATH_ROOT)),/curc/sw/modules)
ADVINCLUDE := /projects/scru5660/public/software/install/advisor_2020.2.0.606470/include 
endif


# -----------------------------------------------------
# Make solvers
#
# Notes:
#
# (1) the -I and -ldl are for Intel Advisor annotations
#
# -----------------------------------------------------


solvers: solvers.cpp solvers.h linearSolver.h plotter.h mpiInfo.h LaplacianOnGrid.h
	$(COMPILER) solvers.cpp -g -o solvers -std=c++11 -fopenmp -I $(ADVINCLUDE) -ldl


