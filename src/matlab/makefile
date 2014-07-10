MINGWPATH=c:/MinGW

# determine wether the system is 32 or 64 bits
# LBITS := $(shell getconf LONG_BIT)
# ifeq ($(LBITS),64)
#    # do 64 bit stuff here

#    MATLABROOT='c:\Program Files\MATLAB\R2012b'


# else
#    # do 32 bit stuff here
#    MATLABROOT='c:\Program Files (x86)\MATLAB\R2010a Student'
# endif

PATH = 'c:\Program\ Files\MATLAB\R2012b'
ifneq ("$(wildcard $(PATH))","")
MATLABROOT='c:\Program Files\MATLAB\R2012b'
LBITS := 64
else
MATLABROOT='c:\Program Files (x86)\MATLAB\R2010a Student'
LBITS := 32
endif

TARGET=edf
SRC=../bin/edf.hpp
MEXSRC=edfmat.cpp
MINGW = C:\MinGW
CCX = g++
MEX = g++
CXXFLAGS= -std=c++11 -Wall -O2
CFLAGS = -Wall -O2 -std=c++11 -fpermissive
# -Wall -O2 -I$(MATLABROOT)/extern/include $(SRC) $(LIBS)
# MEXFLAGS= -shared -DMATLAB_MEX_FILE -I$(MATLABROOT)/extern/include -Wl,--export-all-symbols $(LIBS) $(MEXSRC) -o $(MEXTGT).mexw32
MEXFLAGS= $(CFLAGS) -DMATLAB_MEX_FILE -I$(MATLABROOT)/extern/include -shared $(LIBS)
# LIBS= -L$(MATLABROOT)/bin/win32 -L$(MATLABROOT)/extern/lib/win32/microsoft -lmex -lmx -lmwlapack -lmwblas -leng
LIBS= -L$(MATLABROOT)/bin/win$(LBITS) -L$(MATLABROOT)/extern/lib/win$(LBITS)/microsoft -lmex -lmx -lmwlapack -lmwblas -leng


all: mex

# lib: $(SRC)

# 	@echo ------- building C++ source -------

# 	$(CCX) $(CXXFLAGS) -c $(SRC) -o $(subst ../bin,.,$(subst .hpp,.o, $(SRC)))

mex: $(MEXSRC)
	
	# @echo ------- building MEX file -------

	# @rm -f *.mexw$(LBITS)
	$(MEX) $(MEXFLAGS) $(MEXSRC) -o $(TARGET).mexw$(LBITS)
	