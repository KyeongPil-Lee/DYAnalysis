#include <ElecChannel/Include/DYElectronTool.h>

// -- obtain the uncertainty on the efficiency SF from the diagonal term of the covariance matrices
// -- to be consistent between the cross section results and the covariance matrix results
void TotalUnc_fromCovM()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

  TString fileName_dXSec = analyzerPath+"/ElecChannel/DXSec/ROOTFile_dXSec.root";
  TH1D* h_dXSec = PlotTool::Get_Hist(fileName_dXSec, "h_dXSec");

  TString fileName_cov = analyzerPath+"/ElecChannel/CovMatrix/EfficiencySF/ROOTFile_CovM_EfficiencySF.root";
  TH2D* h_cov_stat = PlotTool::Get_Hist2D(fileName_cov, "h_cov_stat");
  TH2D* h_cov_syst = PlotTool::Get_Hist2D(fileName_cov, "h_cov_syst");


  TH1D* h_relUnc_effSF = DYTool::MakeHist_DXSecBin("h_relUnc_tot");
  Int_t nBin = h_relUnc_effSF->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    Double_t var_stat = h_cov_stat->GetBinContent(i_bin, i_bin);
    Double_t var_syst = h_cov_syst->GetBinContent(i_bin, i_bin);

    Double_t var_tot    = var_stat + var_syst;
    Double_t absUnc_tot = sqrt(var_tot);

    Double_t dXSec = h_dXSec->GetBinContent(i_bin);
    Double_t relUnc_tot = absUnc_tot / dXSec;

    h_relUnc_effSF->SetBinContent(i_bin, relUnc_tot);
    h_relUnc_effSF->SetBinError(i_bin, 0);
  }

  TFile *f_output = TFile::Open("ROOTFile_Unc_EffSF_fromCovM.root", "RECREATE");
  h_relUnc_effSF->Write();
  
  // -- comparison plot between the uncertainty from the gaussian fit vs. the uncertainty from the diagonal terms
  TString fileName_fit = analyzerPath+"/ElecChannel/Uncertainties/EfficiencySF/ROOTFile_Summary_Unc_EffSF.root";
  TH1D* h_relUnc_tot_fit = PlotTool::Get_Hist(fileName_fit, "h_relUnc_tot");

  h_relUnc_tot_fit->Scale(100.0);
  h_relUnc_effSF->Scale(100.0);

  TString canvasName = "c_relUnc_tot_gaussian_vs_covM";
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 0);
  canvas->SetTitle("m(ee) [GeV]", "Rel. unc [%]", "Blue/Black");

  canvas->Register( h_relUnc_tot_fit, "From the gaussian fit",      kBlack);
  canvas->Register( h_relUnc_effSF,   "From the covariance matrix", kBlue);

  canvas->Latex_CMSPre();

  canvas->Draw("HISTLPSAME");
}