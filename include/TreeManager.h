#ifndef __TREEMANAGER__
#define __TREEMANAGER__

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include <fstream>
#include <vector>


#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
#include "TString.h"
#include "TList.h"

#include "Processor.h"
#include "Configuration.h"
#include "TROOT.h"
#endif

using namespace std ;

class TreeManager : public TObject {
	
  public:
    TreeManager(Configuration *config);
    TreeManager(int i=0){;} 
    ~TreeManager();
    void InitRootFile() ;
    void WriteRootFile() ;
    void FillObjectsInRoot(Processor *processor) ;
    void Reset();

  private:
    int fRunNum;
    int fEventNum;
    int fNChannels;
    int fNDevices;
    int fNTimeBins;
    TFile *fFile ;
    TTree * fChannelTree;
    TString fRunName;
    TString fDirectoryName;
    vector<int> fChannelMapping_v;
    Processor *fProcessor;

    // Used for traditional TTree
    vector<Float_t> fPedMean_v;
    vector<Float_t> fPedRMS_v;
    vector<vector<Float_t> > fTimeBinSignal_vv;
    vector<vector<Float_t> > fRawTimeBinSignal_vv;
    ClassDef(TreeManager,1)
};

#endif

