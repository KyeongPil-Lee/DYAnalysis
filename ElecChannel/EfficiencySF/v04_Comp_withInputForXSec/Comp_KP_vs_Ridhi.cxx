#include <Include/SimplePlotTools.h>

void Comp_KP_vs_Ridhi()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_KP    = "../ROOTFile_EfficiencySF_perMassBin.root";
  TString fileName_Ridhi = analyzerPath+"/ElecChannel/Code_Ridhi/CentralValue/Dressed/FinalCorr_v11.root";

  TString histName_KP    = "h_effSF_perMassBin";
  TString histName_Ridhi = "h_EfficiencySF";

  TString fileName_old = analyzerPath+"/ElecChannel/EfficiencySF/v01_TnPEff_Old_vs_New/ROOTFile_EffSFPerMassBin_toKeep.root";
  TString histName_old = "h_oldSF_perMassBin";

  TString canvasName = histName_KP;
  canvasName.ReplaceAll("h_", "c_");
  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 0);
  canvas->SetTitle("m(ee) [GeV]", "Efficiency SF", "KP/Ridhi");

  TH1D* h_KP    = PlotTool::Get_Hist(fileName_KP,    histName_KP);
  TH1D* h_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);
  TH1D* h_old   = PlotTool::Get_Hist(fileName_old, histName_old);
  canvas->Register(h_Ridhi, "Ridhi",         kBlack);
  canvas->Register(h_KP,    "KP",            kBlue);
  canvas->Register(h_old,   "KP (Old eff.)", kGreen+2);

  canvas->Latex_CMSPre();
  TString histInfo = "Efficiency SF per mass bin";
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+histInfo+"}}");

  // canvas->SetRangeY(1e-1, 1e7);
  canvas->SetRangeY(0.91, 1.01);
  canvas->SetRangeRatio(0.97, 1.03);
  canvas->SetLegendPosition(0.70, 0.75, 0.95, 0.90);
  canvas->Draw("HISTLPSAME");

  PlotTool::IsRatio1(h_Ridhi, h_KP);
  PlotTool::IsRatio1(h_Ridhi, h_old);
}