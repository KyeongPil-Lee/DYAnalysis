class ComparisonTool
{
public:
  TH2D* h_tnpEff_Ridhi_;
  TH2D* h_tnpEff_KP_;

  ComparisonTool(TH2D* h_KP, TH2D* h_Ridhi)
  {


  }

private:

};

TH2D* MakeHist_Unc()

void Comparison_sgnChange()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_KP = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult/ROOTFile_RecoSF_April2019.root";
  TString histName_KP = "h_diff_sgnChange_data";

  TH2D* h_diff_KP = PlotTool::Get_Hist2D(fileName_KP, histName_KP);

  // -- Ridhi
  TString fileName_Ridhi = analyzerPath+"/Investigation/v13_TnP_Ridhi_vs_KP/Result_Ridhi/sgnChange/RECO_Syst2.root";
  TString histName_Ridhi = "hdeff";

  TH2D* h_eff_Ridhi = PlotTool::Get_Hist2D(fileName_Ridhi, histName_Ridhi);




}

void Comparison_TnPEff()
{
  Compare_tagChange();
}