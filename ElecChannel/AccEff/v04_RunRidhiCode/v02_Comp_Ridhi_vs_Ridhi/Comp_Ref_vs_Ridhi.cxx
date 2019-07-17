#include <Include/SimplePlotTools.h>

void Comp_Ref_vs_Ridhi(TString type)
{
  TString fileName_ref    = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/CentralValue/Dressed/DYEE_AccEff_M10to3000.root";
  TString fileName_Ridhi = "../DYEE_AccEff_M10to3000.root";

  TString histName_ref    = "";
  TString histName_Ridhi = "";
  if( type == "accTotal" )
  {
    histName_ref = "h_mass_AccTotal";
    histName_Ridhi = "h_mass_AccTotal";
  }
  else if( type == "accPass" )
  {
    histName_ref = "h_mass_AccPass";
    histName_Ridhi = "h_mass_AccPass";
  }
  else if( type == "effPass" )
  {
    histName_ref = "h_mass_EffPass";
    histName_Ridhi = "h_mass_EffPass";
  }

  TString canvasName = histName_ref;
  canvasName.ReplaceAll("h_", "c_");
  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->SetTitle("m(ee) [GeV]", "# events", "Ridhi/Ref.");

  TH1D* h_ref   = PlotTool::Get_Hist(fileName_ref,   histName_ref);
  TH1D* h_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);
  canvas->Register(h_ref,   "Reference (input for x-section)",                      kBlack);
  canvas->Register(h_Ridhi, "Obtained by running the code (under work_76x/AccEff/acceff.C)", kBlue);

  canvas->Latex_CMSSim();
  TString histInfo = "";
  if( type == "accTotal" ) histInfo = "All generated events (denominator of the acceptance)";
  if( type == "accPass" )  histInfo = "Events passing acceptance (numerator of the acceptance)";
  if( type == "effPass" )  histInfo = "Events passing event selection (numerator of the efficiency)";
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+histInfo+"}}");

  canvas->SetRangeY(1e-1, 1e7);
  canvas->SetRangeRatio(0.97, 1.03);
  // if( type == "effPass_M100to200" )
  // {
  //   canvas->SetRangeX(50, 200);
  //   canvas->SetRangeRatio(0.99, 1.01);
  // }

  canvas->SetLegendPosition(0.16, 0.32, 0.90, 0.47);

  canvas->Draw();

  PlotTool::IsRatio1(h_Ridhi, h_ref);
}

void Comp_Ref_vs_Ridhi()
{
  Comp_Ref_vs_Ridhi("accTotal");
  Comp_Ref_vs_Ridhi("accPass");
  Comp_Ref_vs_Ridhi("effPass");
}