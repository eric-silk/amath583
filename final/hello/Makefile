#
# This file is part of the course materials for AMATH 483/583 at the University of Washington,
# Spring 2019
#
# Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
# https://creativecommons.org/licenses/by-nc-sa/4.0/
#
# Author: Andrew Lumsdaine
# 


# Choose features here with yes or no
OMP	:= no
THREADS	:= no
PYTHON  := no
CIMG    := no


# These must be set to some value or make will pick defaults (which will be wrong)
CXX	:= mpic++
LANG	:= -std=c++11

TARGETS := hello.exe

hello.exe: hello.o

.PHONY : all

include ../include/Makefile.inc

all    : $(TARGETS)

OTHERCLEAN = $(TARGETS:.exe=.txt)


# The following 4 (yes 4) lines must be in all the subdirectory makefiles
#-----------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
hello.o: hello.cpp /usr/lib/openmpi/include/mpi.h \
 /usr/lib/openmpi/include/mpi_portable_platform.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/mpicxx.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/constants.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/functions.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/datatype.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/exception.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/op.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/status.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/request.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/group.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/comm.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/win.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/file.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/errhandler.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/intracomm.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/topology.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/intercomm.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/info.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/datatype_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/functions_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/request_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/comm_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/intracomm_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/topology_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/intercomm_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/group_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/op_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/errhandler_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/status_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/info_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/win_inln.h \
 /usr/lib/openmpi/include/openmpi/ompi/mpi/cxx/file_inln.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
