SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(GENIE)/src/make/Make.include
#include Make.include

GENIE_LIBS  = $(shell $(GENIE)/src/scripts/setup/genie-config --libs)
#RAT_LIBS = -L$(RATROOT)/lib -lRATEvent

#LIBRARIES  := $(GENIE_LIBS) $(LIBRARIES) $(CERN_LIBRARIES) $(RAT_LIBS)
LIBRARIES  := $(GENIE_LIBS) $(LIBRARIES) $(CERN_LIBRARIES)

INCLUDES := $(INCLUDES) -I$(GENIE)/src/Framework/ -I$(GENIE)/src/
#INCLUDES := $(INCLUDES) -I$(GENIE)/src/Framework/ -I$(GENIE)/src/

#CXXFLAGS = -std=c++14

all: genie2txt

genie2txt: FORCE
	$(CXX) -g -o genie2txt $(CXXFLAGS) $(LDFLAGS) genie2txt.cc $(INCLUDES) $(LIBRARIES)

#################### CLEANING

purge: FORCE
	$(RM) *.o *~ core 

clean: FORCE
	$(RM) *.o *~ core 
	$(RM) genie2txt


FORCE:

# DO NOT DELETE
