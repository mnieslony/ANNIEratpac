SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

RAT_LIBS = -L$(RATROOT)/lib -lRATEvent

ROOT_FLAG = `root-config --cflags --libs`

LIBRARIES  := $(LIBRARIES) $(CERN_LIBRARIES) $(RAT_LIBS)

INCLUDES := $(INCLUDES) -I$(RATROOT)/include -I$(ROOTSYS)/include


all: txt2rat

txt2rat: FORCE
	$(CXX) -g $(ROOT_FLAG) -o txt2rat $(CXXFLAGS) $(LDFLAGS) txt2rat.cc $(INCLUDES) $(LIBRARIES)

#################### CLEANING

purge: FORCE
	$(RM) *.o *~ core 

clean: FORCE
	$(RM) *.o *~ core 
	$(RM) genie2rat


FORCE:

# DO NOT DELETE
