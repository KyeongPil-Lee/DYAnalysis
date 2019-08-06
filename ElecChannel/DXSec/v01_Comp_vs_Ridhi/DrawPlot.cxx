#include <Include/SimplePlotTools.h>

void DrawPlot_DXSec()
{
  TString fileName_KP = "../ROOTFile_dXSec.root";
  TString histName_KP = "h_dXSec";
  TH1D* h_dXSec_KP = PlotTool::Get_Hist(fileName_KP, histName_KP);

  TString fileName_Ridhi = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/Code_Ridhi/CentralValue/Dressed/FinalCorr_v11.root";
  TString histName_Ridhi = "h_xSec_dM_FSRCorr";
  TH1D* h_dXSec_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);

  TString canvasName = "c_dXSec_KP_vs_Ridhi";
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->SetTitle("m(ee) [GeV]", "d#sigma/dm [pb/GeV]", "KP/Ridhi");

  canvas->Register(h_dXSec_Ridhi, "Ridhi", kBlack);
  canvas->Register(h_dXSec_KP,    "KP",    kBlue);

  canvas->SetRangeRatio(0.94, 1.01);
  // canvas->SetRangeRatio(0.99, 1.01);
  canvas->SetRangeY(1e-7, 1e3);
  canvas->SetLegendPosition(0.75, 0.75, 0.95, 0.95);

  canvas->Latex_CMSPre();

  canvas->Draw();

  PlotTool::IsRatio1( h_dXSec_KP, h_dXSec_Ridhi );
}

void DrawPlot_FpoF_DXSec()
{
  TString fileName_KP = "../ROOTFile_dXSec.root";
  TString histName_KP = "h_FpoF_dXSec";
  TH1D* h_dXSec_KP = PlotTool::Get_Hist(fileName_KP, histName_KP);

  TString fileName_Ridhi = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/Code_Ridhi/CentralValue/Fiducial/FinalCorr_postFSR_v11.root";
  TString histName_Ridhi = "h_xSec_dM_FSRCorr";
  TH1D* h_dXSec_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);

  TString canvasName = "c_FpoF_dXSec_KP_vs_Ridhi";
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->SetTitle("m(ee) [GeV]", "d#sigma/dm [pb/GeV]", "KP/Ridhi");

  canvas->Register(h_dXSec_Ridhi, "Ridhi", kBlack);
  canvas->Register(h_dXSec_KP,    "KP",    kBlue);

  canvas->SetRangeRatio(0.94, 1.01);
  // canvas->SetRangeRatio(0.99, 1.01);
  canvas->SetRangeY(1e-7, 1e3);
  canvas->SetLegendPosition(0.75, 0.75, 0.95, 0.95);

  canvas->Latex_CMSPre();

  canvas->Draw();

  PlotTool::IsRatio1( h_dXSec_KP, h_dXSec_Ridhi );
}

void DrawPlot_DXSec_old_vs_newSF()
{
  TString fileName_KP = "../ROOTFile_dXSec.root";
  TString histName_KP = "h_dXSec";
  TH1D* h_dXSec_KP = PlotTool::Get_Hist(fileName_KP, histName_KP);

  TString fileName_Ridhi = "ROOTFile_dXSec_oldEffSF.root";
  TString histName_Ridhi = "h_dXSec";
  TH1D* h_dXSec_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);

  TString canvasName = "c_dXSec_old_vs_newEffSF";
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->SetTitle("m(ee) [GeV]", "d#sigma/dm [pb/GeV]", "KP/Ridhi");

  canvas->Register(h_dXSec_Ridhi, "Old eff. SF", kBlack);
  canvas->Register(h_dXSec_KP,    "New eff. SF",    kBlue);

  canvas->SetRangeRatio(0.94, 1.01);
  // canvas->SetRangeRatio(0.99, 1.01);
  canvas->SetRangeY(1e-7, 1e3);
  canvas->SetLegendPosition(0.75, 0.75, 0.95, 0.95);

  canvas->Latex_CMSPre();

  canvas->Draw();

  PlotTool::IsRatio1( h_dXSec_KP, h_dXSec_Ridhi );
}

void DrawPlot_FpoF_DXSec_old_vs_newSF()
{
  TString fileName_KP = "../ROOTFile_dXSec.root";
  TString histName_KP = "h_FpoF_dXSec";
  TH1D* h_dXSec_KP = PlotTool::Get_Hist(fileName_KP, histName_KP);

  TString fileName_Ridhi = "ROOTFile_dXSec_oldEffSF.root";
  TString histName_Ridhi = "h_FpoF_dXSec";
  TH1D* h_dXSec_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);

  TString canvasName = "c_FpoF_dXSec_old_vs_newEffSF";
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->SetTitle("m(ee) [GeV]", "d#sigma/dm [pb/GeV]", "KP/Ridhi");

  canvas->Register(h_dXSec_Ridhi, "Old eff. SF", kBlack);
  canvas->Register(h_dXSec_KP,    "New eff. SF",    kBlue);

  canvas->SetRangeRatio(0.94, 1.01);
  // canvas->SetRangeRatio(0.99, 1.01);
  canvas->SetRangeY(1e-7, 1e3);
  canvas->SetLegendPosition(0.75, 0.75, 0.95, 0.95);

  canvas->Latex_CMSPre();

  canvas->Draw();

  PlotTool::IsRatio1( h_dXSec_KP, h_dXSec_Ridhi );
}

void DrawPlot()
{
  DrawPlot_DXSec();
  DrawPlot_FpoF_DXSec();
  DrawPlot_DXSec_old_vs_newSF();
  DrawPlot_FpoF_DXSec_old_vs_newSF();
}