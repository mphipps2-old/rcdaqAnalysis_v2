// Note: meant to be run in root. In order to later analysis as smooth as possible, it's best to process all raw data first and save to trees. Then run offline g++ analysis scripts.

int ProcessRawData(char fileName[256]){
  //  char fileName[256];
  //  int firstFile = 138;
  //int lastFile = 142;
  // for (int i=firstFile; i<= lastFile; i++) {
  //    if (i == 140) continue;
    gSystem->Load("libAnalysis.so");
    real_init();
    //  sprintf(fileName,"/home/sickles-lab/data/test/test_00000%d-0000.prdf" ,i);
    // else if (i >= 100) sprintf(fileName,"/data/gem/ExtPulserIcalScan/ExtPulser_0000000%d-0000.evt",i);
  //else sprintf(fileName, "/data/gem/srsCalib/srsCalib_40000000%d-0000.evt",i);
    //    ResetCount();
    cout << "file " << fileName << endl;
    poncsopen(fileName);
    cout << " file opened " << endl;
    prun();
    //    pclose();
    //}
  return 0;

}
