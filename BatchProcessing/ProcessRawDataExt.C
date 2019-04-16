// Note: meant to be run in root. In order to later analysis as smooth as possible, it's best to process all raw data first and save to trees. Then run offline g++ analysis scripts.

int main() {

  char fileName[256];
  int firstFile = 100;
  int lastFile = 950;
  gSystem->Load("libAnalysis.so");
  real_init();
  for (int i=firstFile; i<= lastFile; i+=25) {
    cout << "File: "<< i << endl;
    if (i < 100) sprintf(fileName, "/data/gem/srsCalib/srsCalibExtPulseMux2_00000000%d-0000.evt",i);
    else sprintf(fileName, "/data/gem/srsCalib/srsCalibExtPulseMux2_0000000%d-0000.evt",i);
    ResetCount();
    poncsopen(fileName);
    prun(2000);
    pclose();
  }
  return 0;

}
