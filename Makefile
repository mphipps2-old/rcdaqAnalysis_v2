PACKAGE = Analysis
ROOTFLAGS = $(shell root-config --cflags)

ROOTLIBS = $(shell root-config --glibs --libs)

##CXXFLAGS = -I. -g -O2  $(ROOTFLAGS) -I$(ONLINE_MAIN)/include -I$(OFFLINE_MAIN)/include
##CXXFLAGS = -g -O2  -I./include $(ROOTFLAGS) -I$(ONLINE_MAIN)/include -I$(OFFLINE_MAIN)/include
CXXFLAGS = -g -O2  -I./include $(ROOTFLAGS) -I$(ONLINE_MAIN)/include -I$(OFFLINE_MAIN)/include

##LDFLAGS = -L$(ONLINE_MAIN)/lib -L$(OFFLINE_MAIN)/lib -lpmonitor -fPIC
LDFLAGS = -Wl,--no-as-needed  -lSpectrum  -L$(ONLINE_MAIN)/lib -L$(OFFLINE_MAIN)/lib -lpmonitor -lEvent -lNoRootEvent -lmessage  $(ROOTLIBS) -fPIC

HDRFILES =  include/$(PACKAGE).h include/Configuration.h include/Processor.h include/TreeManager.h
LINKFILE = include/$(PACKAGE)LinkDef.h

ADDITIONAL_SOURCES = src/Configuration.cxx src/Processor.cxx src/TreeManager.cxx

SO = lib$(PACKAGE).so


#$(SO) : $(PACKAGE).cc $(ADDITIONAL_SOURCES) $(LINKFILE)
#	$(CXX) $(CXXFLAGS) -o $@ -shared  $<  $(ADDITIONAL_SOURCES) $(LDFLAGS) 
$(SO) : $(PACKAGE).cc $(PACKAGE)_dict.C $(ADDITIONAL_SOURCES) $(LINKFILE)
	$(CXX) $(CXXFLAGS) -o $@ -shared  $<  $(ADDITIONAL_SOURCES) $(PACKAGE)_dict.C $(LDFLAGS) 


$(PACKAGE)_dict.C : $(HDRFILES) $(LINKFILE)
	rootcint -f $@ -c $(CXXFLAGS) -p $(HDRFILES) $^


.PHONY: clean

clean: 
	rm -f $(SO) $(PACKAGE)_dict.C $(PACKAGE)_dict.h

