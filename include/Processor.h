#ifndef PROCESSOR_H
#define PROCESSOR_H

//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TSystem.h>
#include <TIterator.h>
#include <TF1.h>
#include <TMath.h>
#include "Event/Event.h"
#include "TObject.h"
#include <vector>
#include <map>
#include <cmath>
#include <pmonitor.h>
#include <fstream>
#include <utility>
//#endif

using namespace std;

class Configuration;
class Processor : public TNamed {

  public:
    Processor(const char* name, const char* title, Configuration *config);
    ~Processor();
    void InitializeEvent(Event *e);
    void FreeMemory();
    void MakeCuts();
    void LoadMapping();
    void UnpackData();
    void SetOnlinePedestal(pair<Int_t,Int_t> pedRange);
    void Reset();
    inline vector<Float_t> GetRawTimeBinSignal(int hwChannel) {return fRawData_m[hwChannel];}
    inline vector<Float_t> GetTimeBinSignal(int hwChannel) {return fData_m[hwChannel];}
    inline Int_t GetNTimeBins() {return fNTimeBins;}
    inline Int_t GetNChannels() {return fNChannels;}
    inline vector<Int_t> GetChannelMapping() {return fChannelMapping_v;}
    inline Float_t GetPedMean(int hwChannel) {return fPedMean->GetBinContent(hwChannel+1);}
    inline Float_t GetPedRMS(int hwChannel) {return fPedRMS->GetBinContent(hwChannel+1);}
    inline int GetPacketNb(int i) {return fDeviceMapping_v[i];}
    inline Int_t GetEventNum() {return fEventNum;}
    inline Int_t GetRunNum() {return fRunNum;}
    inline Int_t GetDevNum() {return fDevNum;}

  private:
    Int_t fEventNum;
    Int_t fNChannels;
    Int_t fDevNum;
    Int_t fBaseline;
    Int_t fNPackets;
    pair<Int_t,Int_t> fPedRange;
    Int_t fNDevices;
    Int_t fNTimeBins;
    Int_t fNSigma;
    Int_t fBinaryCut;
    Int_t fRunNum;
    //    Int_t fMinHitSize;
    TString fCutType;
    map<Int_t,vector<Float_t> > fRawData_m;
    map<Int_t,vector<Float_t> > fData_m;
    vector<Packet*> fPacket_v;
    TH1F *fPedMean;
    TH1F *fPedRMS;
    TH1F *fRawData[20];
    TH1F *fData[20];
    Configuration *fConfig; 
    Event *fEvent;
    vector<TString> fInvertSignal_v;
    vector<Int_t> fMinHitSize_v;
    vector<int> fChannelMapping_v;
    vector<int> fDeviceMapping_v;
    ClassDef(Processor,1);
};

#endif

