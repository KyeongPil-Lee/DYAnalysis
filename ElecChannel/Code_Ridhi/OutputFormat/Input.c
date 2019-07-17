{

  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

  TFile *f1 = TFile::Open(analyzerPath+"/ElecChannel/CentralValue/Dressed/FinalCorr_v11.root");
  TFile *f2 = TFile::Open(analyzerPath+"/ElecChannel/FinalResult/Dressed/FinalCorr_v12_May2019.root");

  TFile *f3 = TFile::Open(analyzerPath+"/ElecChannel/CentralValue/Fiducial/FinalCorr_postFSR_v11.root");
  TFile *f4 = TFile::Open(analyzerPath+"/ElecChannel/FinalResult/Fiducial/FinalCorr_postFSR_v12_May2019.root");
  TFile *f5 = TFile::Open(analyzerPath+"/ElecChannel/FinalResult/Fiducial/Output.root");

  TFile *file = new TFile("DiffXsec_Electron_v11.root","recreate");

  // *********************************************************************************************************************************** //
  TH1D *h_Xsec1    = (TH1D*)f1->Get("h_xSec_dM_FSRCorr");
  TH1D *h_StatUnc1 = (TH1D*)f2->Get("h_RelUnc_Stat");
  TH1D *h_SystUnc1 = (TH1D*)f2->Get("h_RelUnc_Syst");	// without lumi uncertainty

  TH1D *h_FEWZ     = (TH1D*)f2->Get("h_diffXsec_FEWZ");
  TH1D *h_MCNLO1   = (TH1D*)f2->Get("h_diffXsec_MCNLO");

  TH1D *h_Xsec2    = (TH1D*)f3->Get("h_xSec_dM_FSRCorr");
  TH1D *h_StatUnc2 = (TH1D*)f4->Get("h_RelUnc_Stat");
  TH1D *h_SystUnc2 = (TH1D*)f4->Get("h_RelUnc_Syst");

  TH1D *h_MCNLO2   = (TH1D*)f5->Get("h_diffXsec_MCNLO");

  h_Xsec1->SetStats(0);
  h_StatUnc1->SetStats(0);
  h_SystUnc1->SetStats(0);

  h_FEWZ->SetStats(0);
  h_MCNLO1->SetStats(0);

  h_Xsec2->SetStats(0);
  h_StatUnc2->SetStats(0);
  h_SystUnc2->SetStats(0);

  h_MCNLO2->SetStats(0);

  h_Xsec1->Write("h_DiffXSec");
  h_StatUnc1->Write("h_RelUnc_Stat");
  h_SystUnc1->Write("h_RelUnc_Syst");	// without lumi unc.

  h_FEWZ->Write("h_DiffXSec_FEWZ");
  h_MCNLO1->Write("h_DiffXSec_MCNLO");

  h_Xsec2->Write("h_DiffXSec_Fiducial");
  h_StatUnc2->Write("h_RelUnc_Stat_Fiducial");
  h_SystUnc2->Write("h_RelUnc_Syst_Fiducial");	// without lumi unc.

  h_MCNLO2->Write("h_DiffXSec_MCNLO_Fiducial");

  file->Close();
}
