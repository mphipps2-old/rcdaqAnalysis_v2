#include "Configuration.h"

ClassImp(Configuration)

Configuration::Configuration(TString configFile) {
  Load((char*) configFile.Data());
}

//////////////////////////////////////////////////////

Configuration::~Configuration() {
;
}

//////////////////////////////////////////////////////

void Configuration::Load(char* configFile) {
  ifstream file(gSystem->ExpandPathName(configFile), ifstream::in);
  if(!file.is_open()) cerr << "Configuration::Load(): file not open" << endl;
  TString line;
  while (line.ReadLine(file)) {
  // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    if(line.BeginsWith("#")) continue;        	
    if(line.BeginsWith("RUNNAME")) {
      char runName[1000];
      sscanf(line.Data(), "RUNNAME %s", runName);
      fRunName = runName;
    }
    if(line.BeginsWith("NSIGMA")) {
      char nSigma[1000];
      sscanf(line.Data(), "NSIGMA %s", nSigma);
      fNSigma = atoi(nSigma);
    }
   if(line.BeginsWith("NTIMEBINS")) {
      char nTimeBins[1000]; 
      sscanf(line.Data(), "NTIMEBINS %s", nTimeBins);
      fNTimeBins = atoi(nTimeBins);
    }
    if(line.BeginsWith("NCHANNELS")) {
      char nCh[1000]; 
      sscanf(line.Data(), "NCHANNELS %s", nCh);
      fNChannels = atoi(nCh);
    }
   if(line.BeginsWith("MINHITSIZE")) {
      char minHitSize[1000]; 
      sscanf(line.Data(), "MINHITSIZE %s", minHitSize);
      fMinHitSize = atoi(minHitSize);
    }
   if(line.BeginsWith("NREADOUTDEVICES")) {
      char nDevices[1000]; 
      sscanf(line.Data(), "NREADOUTDEVICES %s", nDevices);
      fNDevices = atoi(nDevices);
    }
    if(line.BeginsWith("CUTTYPE")) {
      char cutType[1000]; 
      sscanf(line.Data(), "CUTTYPE %s", cutType);
      fCutType = cutType;
    }
    if(line.BeginsWith("BINARYCUT")) {  
      char bCut[1000];  
      sscanf(line.Data(), "BINARYCUT %s", bCut);  
      fBinaryCut = atoi(bCut); 
    }
    if(line.BeginsWith("MAPPINGFILE")) {
      char mappingFile[1000];
      sscanf(line.Data(), "MAPPINGFILE %s", mappingFile);
      fMappingFile = mappingFile;
    }
    if(line.BeginsWith("DEVICEMAPPINGFILE")) {
      char deviceMappingFile[1000];
      sscanf(line.Data(), "DEVICEMAPPINGFILE %s", deviceMappingFile);
      fDeviceMappingFile = deviceMappingFile;
    }
    if(line.BeginsWith("TREEDIRECTORY")) {
      char treeDirectory[1000];
      sscanf(line.Data(), "TREEDIRECTORY %s", treeDirectory);
      fTreeDirectory = treeDirectory;
    }
    if(line.BeginsWith("FIRSTONLINEPEDTIMEBIN")) {
      char firstBin[1000]; 
      sscanf(line.Data(), "FIRSTONLINEPEDTIMEBIN %s", firstBin);
      fFirstOnlinePedTB = atoi(firstBin);
    }
    if(line.BeginsWith("LASTONLINEPEDTIMEBIN")) {
      char lastBin[1000];
      sscanf(line.Data(), "LASTONLINEPEDTIMEBIN %s", lastBin);
      fLastOnlinePedTB = atoi(lastBin);
    }
  } 
}

