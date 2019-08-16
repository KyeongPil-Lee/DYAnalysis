#include <Include/SimplePlotTools.h>

void Comparison_DXSec()
{
  TString fileName_KP = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/CovMatrix/EfficiencySF/ROOTFile_SmearedDXSecProducer_bkgChange.root";
  TString histName_KP = "h_dXSec_cv";
  TH1D* h_dXSec_KP = PlotTool::Get_Hist(fileName_KP, histName_KP);

  // TString fileName_An = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/Combination/Converter_BLUEOutput/ROOTFile_hepdata__corr02Aug_Full_Converted.root";
  // TString histName_An = "ee/h_dXSec";
  TString fileName_An = "/Users/KyeongPil_Lee/Physics/DYCodeForComb/DYee2015_newEffSF/Covariance/ROOTFile_Input_Covs_v7_20190523.root";
  TString histName_An = "h_DiffXSec_ee";

  TH1D* h_dXSec_An = PlotTool::Get_Hist(fileName_An, histName_An);

  TString canvasName = "c_dXSec";
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->SetTitle("m(ee) [GeV]", "d#sigma/dm [pb/GeV]", "KP/Andrius");

  canvas->Register(h_dXSec_An, "Andrius", kBlack);
  canvas->Register(h_dXSec_KP, "KP",      kBlue);

  canvas->SetLegendPosition(0.70, 0.80, 0.95, 0.95);

  canvas->SetRangeRatio(0.98, 1.01);

  canvas->Draw("HISTLPSAME");
}