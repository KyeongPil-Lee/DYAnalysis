#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>
void DrawCanvas_DataMCRatio_old_new(TH1D* h_old, TH1D *h_new);

void Comparison_DataMCRatio()
{
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName_old = rootFilePath + "/DiffXsec_Electron_v8.root";
  TString fileName_new = rootFilePath + "/DiffXsec_Electron_v10.root";

  TH1D* h_data_old = PlotTool::Get_Hist(fileName_old, "h_DiffXSec_Fiducial");
  TH1D* h_MC_old = PlotTool::Get_Hist(fileName_old, "h_DiffXSec_MCNLO_Fiducial");
  TH1D* h_ratio_old = (TH1D*)h_data_old->Clone();
  h_ratio_old->Divide( h_MC_old, h_data_old );

  TH1D* h_data_new = PlotTool::Get_Hist(fileName_new, "h_DiffXSec_Fiducial");
  TH1D* h_MC_new = PlotTool::Get_Hist(fileName_new, "h_DiffXSec_MCNLO_Fiducial");
  TH1D* h_ratio_new = (TH1D*)h_data_new->Clone();
  h_ratio_new->Divide( h_MC_new, h_data_new );

  DrawCanvas_DataMCRatio_old_new(h_ratio_old, h_ratio_new);
  PlotTool::IsRatio1( h_ratio_old, h_ratio_new );
}

void DrawCanvas_DataMCRatio_old_new(TH1D* h_old, TH1D *h_new)
{
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_DataMCRatio_old_vs_new", 1, 0);
  canvas->Register( h_old, "aMC@NLO/Data (old)", kBlack);
  canvas->Register( h_new, "aMC@NLO/Data (new)", kBlue);

  canvas->SetTitle( "m [GeV]", "aMC@NLO/Data", "Blue/Black");
  canvas->Latex_CMSPre();
  canvas->SetRangeY(0.2, 1.8);
  canvas->SetLegendPosition( 0.40, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0.94, 1.06);
  // canvas->SetRangeRatio(0.88, 1.12);
  // canvas->SetRangeRatio(0.99, 1.01);
  canvas->Draw("EPSAME");
}