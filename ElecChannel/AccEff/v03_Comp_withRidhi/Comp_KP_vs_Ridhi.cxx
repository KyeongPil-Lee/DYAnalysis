#include <Include/SimplePlotTools.h>

void Comp_KP_vs_Ridhi(TString type)
{
  TString fileName_KP    = "../ROOTFile_AccEff_Electron.root";
  TString fileName_Ridhi = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/CentralValue/Dressed/DYEE_AccEff_M10to3000.root";

  TString histName_KP    = "";
  TString histName_Ridhi = "";
  if( type == "accTotal" )
  {
    histName_KP = "h_mass_accTotal_all";
    histName_Ridhi = "h_mass_AccTotal";
  }
  else if( type == "accPass" )
  {
    histName_KP = "h_mass_accPass_all";
    histName_Ridhi = "h_mass_AccPass";
  }
  else if( type == "effPass" )
  {
    histName_KP = "h_mass_effPass_all";
    histName_Ridhi = "h_mass_EffPass";
  }

  TString canvasName = histName_KP;
  canvasName.ReplaceAll("h_", "c_");
  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->SetTitle("m(ee) [GeV]", "# events", "KP/Ridhi");

  TH1D* h_KP    = PlotTool::Get_Hist(fileName_KP,    histName_KP);
  TH1D* h_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);
  canvas->Register(h_Ridhi, "Ridhi", kBlack);
  canvas->Register(h_KP,    "KP",    kBlue);

  canvas->Latex_CMSSim();
  TString histInfo = "";
  if( type == "accTotal" ) histInfo = "All generated events (denominator of the acceptance)";
  if( type == "accPass" )  histInfo = "Events passing acceptance (numerator of the acceptance)";
  if( type == "effPass" )  histInfo = "Events passing event selection (numerator of the efficiency)";
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+histInfo+"}}");

  canvas->SetRangeY(1e-1, 1e7);
  canvas->SetRangeRatio(0.97, 1.03);

  canvas->SetLegendPosition(0.70, 0.75, 0.95, 0.90);

  canvas->Draw();
}

void Comp_KP_vs_Ridhi()
{
  Comp_KP_vs_Ridhi("accTotal");
  Comp_KP_vs_Ridhi("accPass");
  Comp_KP_vs_Ridhi("effPass");
}