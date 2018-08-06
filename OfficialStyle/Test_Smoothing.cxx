#include <Include/SimplePlotTools.h>

void Smoothing( TH1D* h, Double_t lowerEdge, Double_t upperEdge );

void Test_Smoothing()
{
  // -- background
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName_bkgUnc = rootFilePath+"/ROOTFile_SysUnc_BkgEst.root";
  TH1D* h_relUncBkg_tot = PlotTool::Get_Hist( fileName_bkgUnc, "h_RelUnc_Tot_Total" );

  TH1D* h_relUncBkg_tot_smth = (TH1D*)h_relUncBkg_tot->Clone();
  Smoothing( h_relUncBkg_tot_smth, 101, 3000);

  PlotTool::HistCanvaswRatio *c_relUncBkg_tot = new PlotTool::HistCanvaswRatio( "c_relUnc_relUncBkg_tot_smoothing", 1, 1 );
  c_relUncBkg_tot->Register( h_relUncBkg_tot, "Before smoothing", kBlack);
  c_relUncBkg_tot->Register( h_relUncBkg_tot_smth, "After smoothing (101 < M < 3000 GeV)", kRed);
  c_relUncBkg_tot->SetTitle( "m [GeV]", "Unc. from background (%)", "after/before");
  c_relUncBkg_tot->SetLegendPosition( 0.15, 0.82, 0.75, 0.95 );
  c_relUncBkg_tot->SetRangeY( 1e-2, 2e2 );
  c_relUncBkg_tot->Latex_CMSPre();
  c_relUncBkg_tot->Draw("LPSAME");

  //////////////////////////////////////////////////
  // -- need to check individual uncertainties -- //
  //////////////////////////////////////////////////

  // -- FSR
  TString fileName_FSR = rootFilePath+"/ROOTFile_SysUnc_FSR.root";
  TH1D* h_relUncFSR_tot = PlotTool::Get_Hist( fileName_FSR, "h_SysUnc_Tot_Percent" );

  TH1D* h_relUncFSR_tot_smth = (TH1D*)h_relUncFSR_tot->Clone();
  Smoothing( h_relUncFSR_tot_smth, 15, 3000 );

  PlotTool::HistCanvaswRatio *c_relUncFSR_tot = new PlotTool::HistCanvaswRatio( "c_relUnc_relUncFSR_tot_smoothing", 1, 1 );
  c_relUncFSR_tot->Register( h_relUncFSR_tot, "Before smoothing", kBlack);
  c_relUncFSR_tot->Register( h_relUncFSR_tot_smth, "After smoothing (15 < M < 3000 GeV)", kRed);
  c_relUncFSR_tot->SetTitle( "m [GeV]", "Unc. from FSR (%)", "after/before");
  c_relUncFSR_tot->SetLegendPosition( 0.15, 0.82, 0.75, 0.95 );
  c_relUncFSR_tot->SetRangeY( 1e-2, 2e2 );
  c_relUncFSR_tot->Latex_CMSPre();
  c_relUncFSR_tot->Draw("LPSAME");

  //////////////////////////////////////////////////
  // -- need to check individual uncertainties -- //
  //////////////////////////////////////////////////
}






void Smoothing( TH1D* h, Double_t lowerEdge, Double_t upperEdge )
{
  h->GetXaxis()->SetRangeUser(lowerEdge, upperEdge);
  h->Smooth(10, "R");

  h->GetXaxis()->SetRangeUser(15, 3000);
}
