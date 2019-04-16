#include "TreeManager.h"
ClassImp(TreeManager);

TreeManager::TreeManager(Configuration *config) {
  fRunName = config->GetRunName();
  fDirectoryName = config->GetTreeDirectory();
  fNDevices = config->GetNDevices();
  fNChannels = config->GetNChannels();
  fNTimeBins = config->GetNTimeBins();
  InitRootFile();
}
//====================================================================================================================
TreeManager::~TreeManager() {
    delete fChannelTree;
}

//===================================================================================================================

void TreeManager::Reset() {
  fChannelTree->Reset();
}

//====================================================================================================================
// Called in TreeManager()
void TreeManager::InitRootFile() { 
  fChannelTree = new TTree("tree","tree") ;
  fChannelTree->Branch("RunNumber", &fRunNum);
  fChannelTree->Branch("EventNumber", &fEventNum);
  fChannelTree->Branch("PedMean","std::vector<float>", &fPedMean_v);
  fChannelTree->Branch("PedRMS","std::vector<float>", &fPedRMS_v);
  fPedMean_v.resize(fNChannels*fNDevices);
  fPedRMS_v.resize(fNChannels*fNDevices);
  fRawTimeBinSignal_vv.resize(fNChannels*fNDevices);
  fTimeBinSignal_vv.resize(fNChannels*fNDevices);
  for( int c = 0; c < fNDevices*fNChannels; c++ ){
    fRawTimeBinSignal_vv[c].resize(fNTimeBins);
    fTimeBinSignal_vv[c].resize(fNTimeBins);
    fChannelTree->Branch
      ( Form( "RawSignal%d", c), "std::vector<float>", &fRawTimeBinSignal_vv[c] );
    fChannelTree->Branch
      ( Form( "Signal%d", c), "std::vector<float>", &fTimeBinSignal_vv[c] );
    }
  //  cout << "done w/ init root file " << endl;
}

//====================================================================================================================
// Called in FillRootFile()
void TreeManager::FillObjectsInRoot(Processor *processor) {
  fProcessor = processor;
  fChannelMapping_v = fProcessor->GetChannelMapping();
  fEventNum = fProcessor->GetEventNum();
  fRunNum = fProcessor->GetRunNum();
  for(Int_t ch = 0; ch < fNChannels*fNDevices; ch++) {      
    Int_t hwChannelNum = fChannelMapping_v[ch]; 
    fPedMean_v[hwChannelNum] = fProcessor->GetPedMean(hwChannelNum);
    fPedRMS_v[hwChannelNum] = fProcessor->GetPedRMS(hwChannelNum);
    fRawTimeBinSignal_vv[hwChannelNum] = fProcessor->GetRawTimeBinSignal(hwChannelNum);
    fTimeBinSignal_vv[hwChannelNum] = fProcessor->GetTimeBinSignal(hwChannelNum);
  }
  fChannelTree->Fill();
}


//====================================================================================================================
void TreeManager::WriteRootFile() {
 
  char fileName[256];
  sprintf(fileName,"%s/%sRun%d.root",fDirectoryName.Data(),fRunName.Data(), fProcessor->GetRunNum());
  fFile = TFile::Open(fileName,"RECREATE");
  fChannelTree->Write();

  fFile->Close();
}
