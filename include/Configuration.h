#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <TSystem.h>
#include <TString.h>
#include <TObjArray.h>
#include <TIterator.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <utility>
#include <TObjString.h>
#include <TObject.h>

using namespace std;

class Configuration : public TObject {

  public:
    Configuration(TString configFile);
    Configuration(int i=0) {;}
    ~Configuration();
    inline TString GetRunName() {return fRunName;}
    inline Int_t GetNSigma() {return fNSigma;}
    inline Int_t GetNChannels() {return fNChannels;}
    inline Int_t GetNTimeBins() {return fNTimeBins;}
    inline TString GetCutType() {return fCutType;}
    inline Int_t GetBinaryCut() {return fBinaryCut;}
    inline Int_t GetNDevices() {return fNDevices;}
    inline Int_t GetMinHitSize() {return fMinHitSize;}
    inline TString GetMappingFile() {return fMappingFile;}
    inline TString GetDeviceMappingFile() {return fDeviceMappingFile;}
    inline TString GetTreeDirectory() {return fTreeDirectory;}
    inline pair<Int_t,Int_t> GetOnlinePedRange() {pair<Int_t,Int_t> pedRange; pedRange = make_pair(fFirstOnlinePedTB,fLastOnlinePedTB); return pedRange;}
    void Load(char* configFile) ;


  private:
    TString fRunName;
    TString fTreeDirectory;
    Int_t fNSigma;
    Int_t fNTimeBins;
    Int_t fNChannels;
    Int_t fNDevices;
    Int_t fMinHitSize;
    TString fCutType;
    Int_t fBinaryCut;
    TString fMappingFile;
    TString fDeviceMappingFile;
    Int_t fFirstOnlinePedTB;
    Int_t fLastOnlinePedTB;

  ClassDef(Configuration,1)
};

#endif

