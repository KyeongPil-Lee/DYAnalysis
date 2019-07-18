#include <Include/SimplePlotTools.h>

void Comp_KP_vs_Ridhi()
{
  TString fileName_KP    = "../ROOTFile_EfficiencySF_perMassBin.root";
  TString fileName_Ridhi = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/EfficiencySF/v01_TnPEff_Old_vs_New/RidhiResult/ROOTFile_StatUnc_EffCorr.root";

  TString histName_KP    = "h_effSF_perMassBin";
  TString histName_Ridhi = "h_mass_EffPass_Corr_CentralValue";

  TString canvasName = histName_KP;
  canvasName.ReplaceAll("h_", "c_");
  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 0);
  canvas->SetTitle("m(ee) [GeV]", "Efficiency SF", "KP/Ridhi");

  TH1D* h_KP    = PlotTool::Get_Hist(fileName_KP,    histName_KP);
  TH1D* h_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);
  canvas->Register(h_Ridhi, "Ridhi", kBlack);
  canvas->Register(h_KP,    "KP",    kBlue);

  canvas->Latex_CMSPre();
  TString histInfo = "Efficiency SF per mass bin";
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+histInfo+"}}");

  // canvas->SetRangeY(1e-1, 1e7);
  canvas->SetRangeY(0.95, 1.01);
  canvas->SetRangeRatio(0.97, 1.03);
  canvas->SetLegendPosition(0.70, 0.75, 0.95, 0.90);
  canvas->Draw("HISTLPSAME");

  PlotTool::IsRatio1(h_Ridhi, h_KP);
}