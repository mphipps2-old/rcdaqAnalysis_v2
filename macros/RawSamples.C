//#include "AtlasStyle.C"

int RawSamples() {
  

  const int nChannels = 20;

  // draw them
  TCanvas *c1 = new TCanvas("c1","Monitor",1900,900);
  c1->Divide(4,10);

  gStyle->SetStatFontSize(0.06);

  // get histograms
  TH1* vRawSamples[nChannels];
  TH1* vSamples[nChannels];


  // pr = processed  
  // br = board
  // ch = channel
  for( int pr = 0; pr < 2; pr++){
      for( int ch = 0; ch < nChannels; ch++ ){


	c1->cd( ch+1 + (pr*20) );

	std::string hName;
	
	if (pr == 0) {
	  hName = Form( "RawSamples%d", ch );
	  cout << "name " << hName << endl;
	  vRawSamples[ch] = 
	    static_cast< TH1F* >( gDirectory->Get( hName.c_str() ) );

	  vRawSamples[ch]->Draw();

	  vRawSamples[ch]->SetTitle
	    ( Form( "%s; time[ns]; Amp [mV]", hName.c_str() ) );

	  vRawSamples[ch]->SetTitleSize( 0.08, "xy" );
	  vRawSamples[ch]->SetLabelSize( 0.08, "xy" );
	  vRawSamples[ch]->GetXaxis()->SetTitleOffset( 1.10 );
	  vRawSamples[ch]->GetYaxis()->SetTitleOffset( 0.85 );
	}
	else {
	  hName = Form( "Samples%d", ch );
	  vSamples[ch] = 
	    static_cast< TH1F* >( gDirectory->Get( hName.c_str() ) );

	  vSamples[ch]->Draw();
	  vSamples[ch]->SetTitle
	    ( Form( "%s; time[ns]; Amp [mV]", hName.c_str() ) );
	
	  vSamples[ch]->SetTitleSize( 0.08, "xy" );
	  vSamples[ch]->SetLabelSize( 0.08, "xy" );
	  vSamples[ch]->GetXaxis()->SetTitleOffset( 1.10 );
	  vSamples[ch]->GetYaxis()->SetTitleOffset( 0.85 );	  
	}


      }
  }
  //  c1->SaveAs("RawSamples.root");
  return 0;
  // we do not want the empty channels draw, so lets draw something useful.
  // if something gets connected to them, put the return before this.
  /*
  TH2* hSamplesXT    = static_cast< TH2F* >
    ( gDirectory->Get( "SamplesXT" ) );
  TH2* hChargeMapAvg = static_cast< TH2F* >
    ( gDirectory->Get( "ChargeMapAvg" ) );
   
  hSamplesXT->SetTitleSize( 0.08, "xy" );
  hSamplesXT->SetLabelSize( 0.08, "xy" );
  hSamplesXT->GetXaxis()->SetTitleOffset( 1.10 );
  hSamplesXT->GetYaxis()->SetTitleOffset( 0.85 );

  hChargeMapAvg->SetTitleSize( 0.08, "xy" );
  hChargeMapAvg->SetLabelSize( 0.08, "xy" );
  hChargeMapAvg->GetXaxis()->SetTitleOffset( 1.10 );
  hChargeMapAvg->GetYaxis()->SetTitleOffset( 0.85 );
  
  c1->cd(19);
  gPad->SetLogz();
  hSamplesXT->Draw("colz");
  c1->cd(20);
  gPad->SetLogz();
  hChargeMapAvg->Draw("colz");
  */
}
