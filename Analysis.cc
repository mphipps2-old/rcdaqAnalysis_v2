/*
********************************
To compile, use makefile found in this directory
To run, start ROOT and type:
".L libAnalysis.so"
real_init();
"poncsopen(filename);"
"prun();"
********************************
*/

#include "Analysis.h"
#include <pmonitor.h>
#include <iostream>
#include <fstream>
#include <time.h> 
#include <stdlib.h>
#include "Processor.h"
#include "TreeManager.h"
#include "TROOT.h"

Configuration *fConfig;
Processor *fProcessor;
TreeManager *fTrees;
Int_t fCount = 0;
clock_t fT;

int pinit() {
  return 0;
}

int real_init() {
  fCount = 0;
  if (fCount == 0) {
    fT = clock();
    fConfig = new Configuration("RunParameters.txt");
    fProcessor = new Processor("processor","processor",fConfig);
    fTrees = new TreeManager(fConfig); 
  }
  fProcessor->LoadMapping(); 
  ++fCount;

  return 0;
}

void Write() {
  fTrees->WriteRootFile();
  cout << " Analysis.cc: Writing TTrees " << endl;
}

void ResetCount() {
  fCount = 0;
  fT = clock();
}

int nFreed = 0;

int process_event(Event *e) {

  if (e->getEvtType() == 9 || e == 0 || e->getEvtType() == 0 || e->getEvtType() == 601) return 0;
  ++fCount;
  if ( fCount < 3) return 0; 

  if (fCount % 500 == 0) {
      MemInfo_t memInfo;
      CpuInfo_t cpuInfo;
      
      // Get CPU information
      gSystem->GetCpuInfo(&cpuInfo, 100);
      // Get Memory information
      gSystem->GetMemInfo(&memInfo);
    
      // Print the number of events processed, CPU and RAM utilization
      cout << "\r" << left;
      cout << Form( "Processed %d events, ", fCount);
      cout << Form( "CPU: %d", (int)cpuInfo.fTotal) << "%, ";
      cout << Form( "RAM:%3.1f/%3.1fGB", (double)memInfo.fMemUsed/1024, (double)memInfo.fMemTotal/1024) << flush;
  }
  // End of run event
  if (e->getEvtType() == 12 ) {
    fT = clock() - fT;
    cout << endl;
    printf ("Run Time: %f seconds.\n",((float)fT)/CLOCKS_PER_SEC);
    fT = clock();   
    WriteTrees();  
    fProcessor->FreeMemory();
    if( fConfig    ) delete fConfig;     fConfig    = NULL;
    if( fProcessor ) delete fProcessor;  fProcessor = NULL;
    if( fTrees     ) delete fTrees;      fTrees     = NULL;
    nFreed++;
    exit( EXIT_SUCCESS ); // I give up....
    return 0;
  }
  // if beginning of run events
  if (fCount < 2 || e->getEvtType() == 12) return 0;
  fProcessor->InitializeEvent(e);
  // Note: UnpackData() must be updated whenever new readout system added
  fProcessor->UnpackData(); 
  fProcessor->SetOnlinePedestal(fConfig->GetOnlinePedRange());

  fProcessor->MakeCuts(); 
  fTrees->FillObjectsInRoot(fProcessor);
  fProcessor->Reset();

  return 0;
}

int WriteTrees() {

  fTrees->WriteRootFile();
  return 0;
}

int FreeMemory() {
  fProcessor->Reset();
  return 0;
}
