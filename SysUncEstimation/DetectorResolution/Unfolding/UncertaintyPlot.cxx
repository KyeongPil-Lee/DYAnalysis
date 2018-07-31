#include <Include/SimplePlotTools.h>

TH1D* QuadSum( TH1D* h1, TH1D* h2 );
void UncertaintyPlot()
{
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName1 = rootFilePath+"/ROOTFile_SysUnc_Unfolding.root";
  TH1D* h_relUnc_altMC     = PlotTool::Get_Hist(fileName1, "h_RelSysUnc_AltMC_Percent");
  TH1D* h_relUnc_MCStat    = PlotTool::Get_Hist(fileName1, "h_SysUnc_MCSize_Percent");
  TH1D* h_total            = QuadSum( h_relUnc_altMC, h_relUnc_MCStat );

  PlotTool::HistCanvaswRatio *c_unfold = new PlotTool::HistCanvaswRatio( "c_relUnc_unfold", 1, 1 );
  c_unfold->Register( h_total, "Quad. Sum (unfolding unc.)", kRed);
  c_unfold->Register( h_relUnc_altMC, "Alternative MC", kGreen+2);
  c_unfold->Register( h_relUnc_MCStat, "MC statistics", kBlue);
  c_unfold->SetTitle( "m [GeV]", "Uncertainty (%)", "rato to total");
  c_unfold->SetLegendPosition( 0.15, 0.80, 0.75, 0.95 );
  c_unfold->SetRangeY( 1e-2, 2e2 );
  c_unfold->Latex_CMSPre();
  c_unfold->Draw("LPSAME");
}

TH1D* QuadSum( TH1D* h1, TH1D* h2 )
{
  TH1D* h_result = (TH1D*)h1->Clone();

  Int_t nBin = h1->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;
    Double_t value1 = h1->GetBinContent(i_bin);
    Double_t value2 = h2->GetBinContent(i_bin);

    Double_t quadSum = sqrt( value1*value1 + value2*value2 );

    h_result->SetBinContent(i_bin, quadSum);
  }

  return h_result;
}
