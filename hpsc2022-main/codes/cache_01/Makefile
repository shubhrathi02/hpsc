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
#
# In  ubuntu, to install OpenMPI:
#
# sudo  apt-get update               <-- In Windows Ubuntu, needed this first
# sudo  apt-get install  openmpi-bin
# 
# Be sure you have g++ installed, too!
#
# Once  installed, the build command below will work.  Note  that the
# std=c++11 is requied to use the timers.
#


main: main.cpp Makefile main.h
	mpicxx -g main.cpp -o main -std=c++11
#	g++ -fopenmp main.cpp -o main



