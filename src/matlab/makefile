MINGWPATH=c:/MinGW

TARGET=edf
SRC=../bin/edf.hpp
MEXSRC=edfmat.cpp
MATLABROOT='c:\Program Files (x86)\MATLAB\R2010a Student'
CCX=g++.exe
MEX=g++.exe 
# $(MATLABROOT)\bin\mex.pl
CXXFLAGS= -std=c++11 -Wall -O2
CFLAGS = -Wall -O2 -std=c++11 -fpermissive
# -Wall -O2 -I$(MATLABROOT)/extern/include $(SRC) $(LIBS)
# MEXFLAGS= -shared -DMATLAB_MEX_FILE -I$(MATLABROOT)/extern/include -Wl,--export-all-symbols $(LIBS) $(MEXSRC) -o $(MEXTGT).mexw32
MEXFLAGS= $(CFLAGS) -DMATLAB_MEX_FILE -I$(MATLABROOT)/extern/include -shared $(LIBS)
LIBS= -L$(MATLABROOT)/bin/win32 -L$(MATLABROOT)/extern/lib/win32/microsoft -lmex -lmx -lmwlapack -lmwblas -leng

all: mex

# lib: $(SRC)

# 	@echo ------- building C++ source -------

# 	$(CCX) $(CXXFLAGS) -c $(SRC) -o $(subst ../bin,.,$(subst .hpp,.o, $(SRC)))

mex: $(MEXSRC)
	
	@echo ------- building MEX file -------

	@rm -f *.mexw32
	$(MEX) $(MEXFLAGS) $(MEXSRC) -o $(TARGET).mexw32
	