int run( std::string fName = "" ){
  
  if( fName.empty() ){
    std::cerr << "no filename provided" << std::endl;
    return 1;
  }

  gSystem->Load("libAnalysis.so");
  real_init();
  pfileopen( fName.c_str() );
  prun();

  return 0;
}
