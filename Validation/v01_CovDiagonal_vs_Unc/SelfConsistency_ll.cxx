#include <Include/SimplePlotTools.h>

TH1D* MakeHist_sqrtDiagonal( TH2D* h2D, TH1D* h_template )
{
  TH1D* h_sqrtDiag = (TH1D*)h_template->Clone();
  h_sqrtDiag->Reset("ICES");

  Int_t nBin = h2D->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    Double_t diag     = h2D->GetBinContent(i_bin, i_bin);
    Double_t unc_diag = h2D->GetBinError(i_bin, i_bin);

    h_sqrtDiag->SetBinContent(i_bin, sqrt(diag));
    h_sqrtDiag->SetBinError(i_bin, sqrt(unc_diag));
  }

  return h_sqrtDiag;
}

TH1D* Extract_AbsUnc(TH1D* h)
{
  TH1D* h_return = (TH1D*)h->Clone();

  Int_t nBin = h->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;
    Double_t absUnc = h->GetBinError(i_bin);

    h_return->SetBinContent(i_bin, absUnc);
    h_return->SetBinError(i_bin, 0);
  }

  return h_return;
}

// -- check whether i-th bin error of the x-sec histogram == sqrt( cov(i, i) )?
void SelfConsistency_ll()
{
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";

  TH1D* h_dXSec = PlotTool::Get_Hist(fileName, "ll/h_dXSec");
  TH1D* h_absUnc_tot_extracted = Extract_AbsUnc(h_dXSec);
  
  TH1D* h_absUnc_tot = PlotTool::Get_Hist(fileName, "ll/h_absUnc_tot");

  TH2D* h_cov_tot = PlotTool::Get_Hist2D(fileName, "ll/h_cov_tot");
  TH1D* h_sqrtDiag = MakeHist_sqrtDiagonal( h_cov_tot, h_absUnc_tot );

  TString canvasName = TString::Format("c_SelfConsistency_ll");
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, 1, 1 );
  canvas->Register( h_absUnc_tot_extracted, "Total uncertainty (extracted from x-sec hist.)", kRed);
  canvas->Register( h_absUnc_tot,           "Total uncertainty (from Andrius' root file)", kBlack);
  canvas->Register( h_sqrtDiag,             "sqrt( cov(i,i) )",  kBlue);
  canvas->SetTitle( "m [GeV]",    "Unc. [pb/GeV]",     "black/red");

  canvas->Latex_CMSPre();
  canvas->SetLegendPosition( 0.45, 0.82, 0.95, 0.95 );
  canvas->SetRangeY(1e-8, 1e+4);
  canvas->SetRangeRatio(0.96, 1.04);

  canvas->RegisterLatex(0.16, 0.91, "#font[62]{#scale[0.6]{Combined result}}");
  canvas->Draw();

  PlotTool::IsRatio1(h_absUnc_tot, h_sqrtDiag);
  PlotTool::IsRatio1(h_absUnc_tot_extracted, h_sqrtDiag);
}