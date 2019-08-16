
TH1D* MakeHist(TString histName, vector<Double_t> vec_eff, vector<Double_t> vec_absUnc)
{
  const Int_t nEtaBin = 10;
  Double_t arr_etaBinEdge[nEtaBin+1] = {-2.5, -2.0, -1.566, -1.444, -0.8, 0, 0.8, 1.444, 1.566, 2.0, 2.5};

  TH1D* h_eff = new TH1D(histName, "", nEtaBin, arr_etaBinEdge);

  Int_t nAbsEtaBin = 5;
  for(Int_t i_absEta=0; i_absEta<nAbsEtaBin; i_absEta++)
  {
    Double_t eff    = vec_eff[i_absEta];
    Double_t absUnc = vec_absUnc[i_absEta];

    Int_t i_etaBin1 = i_absEta+6;
    Int_t i_etaBin2 = 5-i_absEta;

    h_eff->SetBinContent(i_etaBin1, eff);
    h_eff->SetBinError(i_etaBin1,   absUnc);

    h_eff->SetBinContent(i_etaBin2, eff);
    h_eff->SetBinError(i_etaBin2,   absUnc);
  }

  return h_eff;
}


void ConvertToROOTHist()
{
  vector<Double_t> vec_eff_plusSigma    = {0.8865, 0.9999, 0.500, 0.9739, 0.9167};
  vector<Double_t> vec_absUnc_plusSigma = {0.013, 0.008, 0.001, 0.007, 0.010};

  vector<Double_t> vec_eff_minusSigma    = {0.8867, 0.9999, 0.500, 0.9739, 0.9167};
  vector<Double_t> vec_absUnc_minusSigma = {0.013, 0.008, 0.001, 0.007, 0.010};

  TH1D* h_eff_plusSigma  = MakeHist("h_eff_plusSigma", vec_eff_plusSigma, vec_absUnc_plusSigma);
  TH1D* h_eff_minusSigma = MakeHist("h_eff_minusSigma", vec_eff_minusSigma, vec_absUnc_minusSigma);

  TFile *f_output = TFile::Open("ROOTFile_Eff_Pt10to20_VariedInitFitParam.root", "RECREATE");
  f_output->cd();

  h_eff_plusSigma->Write();
  h_eff_minusSigma->Write();
}