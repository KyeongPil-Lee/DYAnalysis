#include <Include/SimplePlotTools.h>

void Comparison_EffSF()
{
  // -- new efficiency SF with the same ntuple (DY_forEff_M10to3000.root)
  TString fileName_An = "./chk_eeEffSF_2019Aug07.root";
  TString histName_An = "h1effSF";

  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_KP = analyzerPath+"/ElecChannel/EfficiencySF/ROOTFile_EfficiencySF_perMassBin.root";
  TString histName_KP = "h_effSF_perMassBin";

  TH1D* h_An = PlotTool::Get_Hist(fileName_An, histName_An);
  TH1D* h_KP = PlotTool::Get_Hist(fileName_KP, histName_KP);

  // -- canvas
  TString canvasName = "c_effSF_Andrius_vs_KP_fromSameNtuple";
  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 0);
  canvas->SetTitle("m(ee) [GeV]", "Efficiency SF", "Andrius/KP");

  canvas->Register(h_KP, "KP",                         kBlue);
  canvas->Register(h_An, "Andrius (from same ntuple)", kBlack);


  canvas->Latex_CMSPre();
  TString histInfo = "Efficiency SF per mass bin w/ new TnP efficiency measured by Dalmin";
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.5]{"+histInfo+"}}");

  // canvas->SetRangeY(1e-1, 1e7);
  canvas->SetRangeY(0.95, 1.01);
  canvas->SetRangeRatio(0.99, 1.01);
  canvas->SetLegendPosition(0.50, 0.75, 0.95, 0.90);
  canvas->Draw("HISTLPSAME");

  PlotTool::IsRatio1(h_An, h_KP);
}