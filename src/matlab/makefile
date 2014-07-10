# ======================================
# MATLAB/OCTAVE integration for EDFLIB
#
# this script will automatically compile the matlab (.mex*) or octave (.oct*) files
# for EDFLIB
#
# by @jonbrennecke github.com/jonbrennecke
# ======================================

ifeq ($(OS),Windows_NT)

	# do windows things

	MINGWPATH=c:/MinGW
	PATH = 'c:\Program\ Files\MATLAB\R2012b'
	ifneq ("$(wildcard $(PATH))","")
		MATLABROOT='c:\Program Files\MATLAB\R2012b'
		LBITS := 64
	else
		MATLABROOT='c:\Program Files (x86)\MATLAB\R2010a Student'
		LBITS := 32
	endif

	# looks like we're using MATLAB with Mex
	MEXC := g++
	MEXEXT := .mex

else

	# it's a unix system!

	# determine wether the system is 32 or 64 bits
	LBITS := $(shell getconf LONG_BIT)

	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		CCFLAGS += -D OSX

		# looks like we're using Octave
		# PATH = 'c:\Program\ Files\MATLAB\R2012b'
		MEXC := g++
		MEXEXT := .oct

	endif

endif



TARGET := edf
SRC := ../bin/edf.hpp
MEXSRC := edfmat.cpp
MINGW := C:\MinGW
CXX := g++
CXXFLAGS := -std=c++11 -Wall -O2
CFLAGS := -Wall -O2 -std=c++11 -fpermissive
MEXFLAGS := $(CFLAGS) -DMATLAB_MEX_FILE -I$(MATLABROOT)/extern/include -shared $(LIBS)
LIBS := -L$(MATLABROOT)/bin/win$(LBITS) -L$(MATLABROOT)/extern/lib/win$(LBITS)/microsoft -lmex -lmx -lmwlapack -lmwblas -leng


all: mex

# lib: $(SRC)

# 	@echo ------- building C++ source -------

# 	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(subst ../bin,.,$(subst .hpp,.o, $(SRC)))

mex: $(MEXSRC)
	
	# @echo ------- building MEX file -------

	@rm -f *.mexw$(LBITS)
	$(MEXC) $(MEXFLAGS) $(MEXSRC) -o $(TARGET).mexw$(LBITS)
	