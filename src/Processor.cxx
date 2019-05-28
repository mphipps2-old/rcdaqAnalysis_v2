#include "Processor.h"
#include "Configuration.h"

ClassImp(Processor);

Processor::Processor(const char* name, const char* title, Configuration *config) {

  fConfig = config;
  fNChannels = fConfig->GetNChannels();
  fNDevices = fConfig->GetNDevices();
  fNTimeBins = fConfig->GetNTimeBins();
  fPedRange = fConfig->GetOnlinePedRange();
  fNSigma = fConfig->GetNSigma();
  fCutType = fConfig->GetCutType();
  fBinaryCut = fConfig->GetBinaryCut();
  char pedName[100];
  sprintf(pedName, "pedMean");
  fPedMean = new TH1F(pedName,pedName,fNChannels*fNDevices,0,fNChannels*fNDevices);
  fPedMean->SetXTitle("Hardware Channel Number");
  fPedMean->SetYTitle("Mean Signal");
  sprintf(pedName, "pedRMS");
  fPedRMS = new TH1F(pedName,pedName,fNChannels*fNDevices,0,fNChannels*fNDevices);
  fPedRMS->SetXTitle("Hardware Channel Number");
  fPedRMS->SetYTitle("Signal RMS");
  /*
  for (int i = 0; i < 20; ++i) {
    char name[256];
    sprintf(name,"RawSamples%d",i);
    fRawData[i] = new TH1F(name,name,1024,0,1023);
    sprintf(name,"Samples%d",i);
    fData[i] = new TH1F(name,name,1024,0,1023);
  }
  */
}

//////////////////////////////////////////////////////

Processor::~Processor() {

  if( fPedMean ) delete fPedMean;
  if( fPedRMS  ) delete fPedRMS;   

}

//////////////////////////////////////////////////////

void Processor::FreeMemory() {
  cout << "Processor::FreeMemory(): freeing memory" << endl; 
  if( fPedMean )fPedMean->Reset();
  if( fPedRMS  )fPedRMS->Reset();
  delete fPedMean; fPedMean = NULL;
  delete fPedRMS;  fPedRMS  = NULL;
}

//////////////////////////////////////////////////////

void Processor::Reset() {
  for (int p = 0; p < fNDevices; ++p) {
    delete fPacket_v[p];  
  }
  fPacket_v.clear();
}

//////////////////////////////////////////////////////

void Processor::InitializeEvent(Event *e) {
  fEvent = e;  
  fRunNum = e->getRunNumber();
  fData_m.clear();
   fRawData_m.clear();
  fEventNum = e->getEvtSequence();
}

/////////////////////////////////////////////////////
void Processor::LoadMapping() {

  // Channel Mapping
  ifstream file(gSystem->ExpandPathName(fConfig->GetMappingFile()));
  if(!file.is_open()) cerr << "Processor::LoadMapping(): mapping file not open" << endl;
  TString line;
  while (line.ReadLine(file)) {
  // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    if(line.BeginsWith("#")) continue;
    // Array of tokens separated by ","
    TObjArray *tokens = line.Tokenize(",");
    TIter myIter(tokens);
    while(TObjString *st = (TObjString*) myIter.Next()) {
      fChannelMapping_v.push_back( (((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')).Atoi());        
      fInvertSignal_v.push_back( (((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')));
      fMinHitSize_v.push_back( (((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')).Atoi());
    }
    delete tokens;
  }

  // Device Mapping (ie packets corresponding to digitizer devices
  ifstream deviceFile(gSystem->ExpandPathName(fConfig->GetDeviceMappingFile()));
  if(!deviceFile.is_open()) cerr << "Processor::LoadMapping(): device mapping file not open" << endl;
  while (line.ReadLine(deviceFile)) {
  // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    if(line.BeginsWith("#")) continue;
    // Array of tokens separated by ","
    TObjArray *tokens = line.Tokenize(",");
    TIter myIter(tokens);
    while(TObjString *st = (TObjString*) myIter.Next()) {
      fDeviceMapping_v.push_back( (((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')).Atoi());   
    }
    delete tokens;
  }
}

/////////////////////////////////////////////////////
// Note: this function must be updated whenever new readout system added
void Processor::UnpackData() {
  // Device numbering in RCDAQ
  for (int device=0; device < fNDevices; ++device) {
    int packetNb = fDeviceMapping_v[device];
    fPacket_v.push_back(fEvent->getPacket(packetNb));
  }

  // Fill fRawData with fully mapped data from all time bins (no cuts)
  for (int device=0; device < fNDevices; ++device) {
    for (int chNo=0; chNo < fNChannels; ++chNo) {
      vector<Float_t> timeBinVec; 
      Int_t hwChannelNum; 
      hwChannelNum = fChannelMapping_v[chNo+(device*fNChannels)]; 
      //      fRawData[hwChannelNum]->Reset();
      for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
	  timeBinVec.push_back(fPacket_v[device]->rValue(timeBin,chNo));
	  //	  fRawData[hwChannelNum]->Fill(timeBin,fPacket_v[device]->rValue(timeBin,chNo));
      }
      fRawData_m[hwChannelNum] = timeBinVec;  // each entry in fRawData is the data from one event. This includes all time bins across all channels

    }
  }
}


//////////////////////////////////////////////////////

void Processor::MakeCuts() {
  for (int device = 0; device < fNDevices; ++device) {
  // Pedestal subtraction (correction in time for each strip -- ie each strip has its own correction value)
    for (int chNo=(device*fNChannels); chNo < fNChannels+(device*fNChannels); ++chNo) {
      vector<Float_t> timeBinVec;    
      Int_t hwChannelNum = fChannelMapping_v[chNo];
      for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
	float signal = fRawData_m[hwChannelNum][timeBin] - fPedMean->GetBinContent(hwChannelNum+1);
	   if (fInvertSignal_v[chNo].EqualTo("YES",TString::kIgnoreCase)) {
             signal *= -1;
	   }
           timeBinVec.push_back(signal); 
      }
      fData_m[hwChannelNum] = timeBinVec;
    }
  }
  // set hit limits (zero suppression)
  
  for (int ch=0; ch < fNChannels * fNDevices; ++ch) {
    vector<Float_t> timeBinSignal; 
    vector<Float_t> rawTimeBinSignal; 
    Float_t charge; 
    Float_t cut;
    Int_t hwChannelNum = fChannelMapping_v[ch];
    //    fData[hwChannelNum]->Reset();
    if (fCutType.EqualTo("Analog",TString::kIgnoreCase)) cut = fNSigma * fPedRMS->GetBinContent(hwChannelNum+1);
    else cut = fBinaryCut;
    float pedMean = fPedMean->GetBinContent(hwChannelNum+1);
    for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
      rawTimeBinSignal.push_back(fRawData_m[hwChannelNum][timeBin]);
    }
    // flexible sliding window: signal should be longer than some length and the more consecutive time bins above baseline the more significant the signal is. Assume noise is discrete random distribution so sigma = Sqrt( (1/N) Sum(xi - x)^2)
    for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
      float totalSignal2 = fData_m[hwChannelNum][timeBin] * fData_m[hwChannelNum][timeBin];
      int nBins = 0;
      if (fData_m[hwChannelNum][timeBin] > 0 && timeBin+1 != fNTimeBins) { 
	// > 0 since pedmean already subtracted
	// set sliding window here and check neighboring time bins
	++nBins; 
        for (int N=1; timeBin+N < fNTimeBins; ++N) {  
          float next = fData_m[hwChannelNum][timeBin+N]; 
          if (next <= 0) {  break;}	 
          else {
	    totalSignal2 += (next*next); 
	    nBins++; 
	  } 
        } 
        if ((totalSignal2 > (nBins* cut* cut)) && nBins > fMinHitSize_v[ch]) {
	  for (int N=0; N < nBins; ++N) {
	    timeBinSignal.push_back(fData_m[hwChannelNum][timeBin+N]);
	    //    fData[hwChannelNum]->Fill(timeBin+N,fData_m[hwChannelNum][timeBin+N]);
	  }
	}
	else {
	  for (int N=0; N < nBins; ++N) {
	    timeBinSignal.push_back(0);
	    //	    fData[hwChannelNum]->Fill(timeBin+N,0);
	  }
	}
        timeBin+=nBins;
      }
      else {
        timeBinSignal.push_back(0);
	//	fData[hwChannelNum]->Fill(timeBin,0);
      }
    }
  }
}


//////////////////////////////////////////////////////

void Processor::SetOnlinePedestal(pair<Int_t,Int_t> pedRange) {

  fPedMean->Reset();
  fPedRMS->Reset();

  for (int ch=0; ch < fNChannels*fNDevices; ++ch) {
    Int_t hwChannelNum = fChannelMapping_v[ch]; 
    TH1F *pedestals = new TH1F("Ped","Ped",2000,-1000,1000);
    for (int timeBin=pedRange.first; timeBin <= pedRange.second; ++timeBin) {
      pedestals->Fill(fRawData_m[hwChannelNum][timeBin]);
    }
    fPedMean->Fill(hwChannelNum,pedestals->GetMean());
    fPedRMS->Fill(hwChannelNum,pedestals->GetRMS());
    delete pedestals;
  }
}
