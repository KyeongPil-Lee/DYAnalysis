#include <Include/SimplePlotTools.h>
#include <ElecChannel/Include/DYElectronTool.h>

void RemoveAllTerm(TH2D* h2D)
{
  Int_t nBin = h2D->GetNbinsX();

  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;
    for(Int_t j=0; j<nBin; j++)
    {
      Int_t j_bin = j+1;
      h2D->SetBinContent(i_bin, j_bin, 0);
    }
  }
}

void MakeSummaryROOTFile()
{
  TString fileName_output = "ROOTFile_CovM_EfficiencySF.root";
  TFile *f_output = TFile::Open(fileName_output, "RECREATE");

  vector<TString> vec_uncType = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};

  vector<TH2D*> vec_cov_syst;
  for(const auto& uncType : vec_uncType )
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

    TString fileName = analyzerPath+"/ElecChannel/CovMatrix/EfficiencySF/ROOTFile_CovMProducer_"+uncType+".root";

    TH2D* h_cov  = PlotTool::Get_Hist2D(fileName, "h_cov_"+uncType);
    TH2D* h_corr = PlotTool::Get_Hist2D(fileName, "h_corr_"+uncType);

    // -- remove the uncertainties & correlations
    if( uncType == "bkgChange" || uncType == "tagChange" )
    {
      RemoveAllTerm(h_cov);
      RemoveAllTerm(h_corr);
    }

    f_output->cd();
    h_cov->Write();
    h_corr->Write();

    if( uncType != "stat" )
      vec_cov_syst.push_back( h_cov );
  }

  if( vec_cov_syst.size() != 4 )
  {
    cout << "vec_cov_syst.size() != 4" << endl;
    return;
  }

  TH2D* h_cov_syst_temp1 = PlotTool::Hist2DOperation("h_cov_syst_temp1", vec_cov_syst[0], vec_cov_syst[1], "+");
  TH2D* h_cov_syst_temp2 = PlotTool::Hist2DOperation("h_cov_syst_temp2", vec_cov_syst[2], vec_cov_syst[3], "+");

  TH2D* h_cov_syst  = PlotTool::Hist2DOperation("h_cov_syst", h_cov_syst_temp1, h_cov_syst_temp2, "+");
  TH2D* h_corr_syst = DYTool::Convert_CovMToCorrM(h_cov_syst, "h_corr_syst");

  f_output->cd();
  h_cov_syst->Write();
  h_corr_syst->Write();

  f_output->Close();
}