#include <Include/SimplePlotTools.h>

void DrawCanvas_Ratio_Eff(TH1D* h_ratio, TH1D *h_eff);
void DrawCanvas_Unc( TH1D* h_before, TH1D* h_after, TString type);
void DrawCanvas_Ratio2_Eff(TH1D* h_ratio, TH1D *h_eff);
void Validate_Data()
{
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName_before = rootFilePath+"/DiffXsec_Electron_v8.root";
  // TString fileName_after = rootFilePath+"/DiffXsec_Electron_v9.root";
  // TString fileName_after = +"./Temp/DiffXsec_Electron_v9_1.root"; // -- new result by Ridhi
  TString fileName_after = rootFilePath+"/DiffXsec_Electron_v10.root"; // -- final update

  TString fileName_accEff = rootFilePath+"/ROOTFile_AccEff_Electron.root";

  // -- central values
  TH1D* h_DXSec_before = PlotTool::Get_Hist(fileName_before, "h_DiffXSec_Fiducial");
  TH1D* h_DXSec_after = PlotTool::Get_Hist(fileName_after, "h_DiffXSec_Fiducial");

  TH1D* h_ratio = (TH1D*)h_DXSec_after->Clone("h_ratio");
  h_ratio->Divide( h_DXSec_before, h_DXSec_after ); // -- before/after: should be same with eff.

  TGraphAsymmErrors *g_eff = PlotTool::Get_Graph(fileName_accEff, "g_eff");
  TH1D* h_eff = PlotTool::Convert_GraphToHist(g_eff);

  PlotTool::IsRatio1( h_ratio, h_eff );
  DrawCanvas_Ratio_Eff( h_ratio, h_eff );

  // -- uncertainties
  TH1D* h_relUnc_stat_before = PlotTool::Get_Hist(fileName_before, "h_RelUnc_Stat_Fiducial"); 
  TH1D* h_relUnc_stat_after = PlotTool::Get_Hist(fileName_after, "h_RelUnc_Stat_Fiducial"); 
  PlotTool::IsRatio1( h_relUnc_stat_before, h_relUnc_stat_after );
  DrawCanvas_Unc(h_relUnc_stat_before, h_relUnc_stat_after, "Stat.");

  TH1D* h_relUnc_syst_before = PlotTool::Get_Hist(fileName_before, "h_RelUnc_Syst_Fiducial"); 
  TH1D* h_relUnc_syst_after = PlotTool::Get_Hist(fileName_after, "h_RelUnc_Syst_Fiducial"); 
  PlotTool::IsRatio1( h_relUnc_syst_before, h_relUnc_syst_after );
  DrawCanvas_Unc(h_relUnc_syst_before, h_relUnc_syst_after, "Syst.");
}

void DrawCanvas_Ratio_Eff(TH1D* h_ratio, TH1D *h_eff)
{
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_Ratio_vs_Eff", 1, 0);
  canvas->Register( h_ratio, "Ratio (before/after F-DXSec update)", kBlack);
  canvas->Register( h_eff, "Efficiency", kBlue);

  canvas->SetTitle( "m [GeV]", "Value", "Blue/Black");
  canvas->Latex_CMSPre();
  canvas->SetRangeY(0, 1);
  canvas->SetLegendPosition( 0.40, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0.94, 1.06);
  canvas->Draw("EPSAME");
}

void DrawCanvas_Unc( TH1D* h_before, TH1D* h_after, TString type)
{
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_Unc_"+type, 1, 1);
  canvas->Register( h_before, "Before", kBlack);
  canvas->Register( h_after, "After", kBlue);

  canvas->SetTitle( "m [GeV]", "Rel. Uncertainty ("+type+") (%)", "Blue/Black");
  canvas->Latex_CMSPre();
  canvas->SetRangeY(1e-1, 5e2);
  canvas->SetLegendPosition( 0.60, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0.94, 1.06);
  canvas->Draw("EPSAME");
}