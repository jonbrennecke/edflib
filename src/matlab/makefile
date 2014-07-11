# ======================================
# MATLAB/OCTAVE integration for EDFLIB
#
# this script will automatically compile the matlab (.mex*) or octave (.oct*) files
# for EDFLIB
#
# by @jonbrennecke github.com/jonbrennecke
# ======================================

CXX := g++
CXXFLAGS := -std=c++11 -Wall -O2
CFLAGS := -Wall -O2 -std=c++11 -fpermissive

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
	LIBS := -L$(MATLABROOT)/bin/win$(LBITS) -L$(MATLABROOT)/extern/lib/win$(LBITS)/microsoft
	MEXFLAGS := $(CFLAGS) -DMATLAB_MEX_FILE -shared
	# -I$(MATLABROOT)/extern/include -shared $(LIBS)


else

	# it's a unix system!

	# determine wether the system is 32 or 64 bits

	UNAME_S := $(shell uname -s)

	# Linux
	ifeq ($(UNAME_S),Linux)
		CFLAGS += -D LINUX
		LBITS := $(shell getconf LONG_BIT)

	endif

	# OSX
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += -D OSX


		# looks like we're using Octave
		# PATH = 'c:\Program\ Files\MATLAB\R2012b'
		MEXC := g++
		MEXEXT := .mex
		OCTAVEVER := 3.8.0
		OCTAVEDIR := /usr/local/Cellar/octave/$(OCTAVEVER)
		LIBS := -L$(OCTAVEDIR)/lib/octave/$(OCTAVEVER) -L$(OCTAVEDIR)/lib/octave/$(OCTAVEVER)/oct/x86_64-apple-darwin13.1.0/
		LIBS += -loctave -loctinterp
		MEXFLAGS := $(CFLAGS) -DMATLAB_MEX_FILE -I$(OCTAVEDIR)/include/octave-$(OCTAVEVER)/octave/ -shared $(LIBS)

	endif

endif



TARGET := edf
SRC := ../bin/edf.hpp
MEXSRC := edfmat.cpp
MINGW := C:\MinGW

all: mex

# lib: $(SRC)

# 	@echo ------- building C++ source -------

# 	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(subst ../bin,.,$(subst .hpp,.o, $(SRC)))

mex: $(MEXSRC)
	
	# @echo ------- building MEX file -------

	@rm -f *.mexw$(LBITS)
	$(MEXC) $(MEXFLAGS) $(MEXSRC) -o $(TARGET).mex$(WIN)$(LBITS)
	