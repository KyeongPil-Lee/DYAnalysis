#include <Include/SimplePlotTools.h>

class ComparisonTool
{
public:
  TString uncType_;

  TH1D* h_dXSec_Ridhi_;
  TH1D* h_dXSec_KP_;

  ComparisonTool() {}
  ComparisonTool(TString uncType, TH1D* h_dXSec_Ridhi, TH1D* h_dXSec_KP)
  {
    uncType_ = uncType;

    TH1::AddDirectory(kFALSE);
    h_dXSec_Ridhi_ = (TH1D*)h_dXSec_Ridhi->Clone();
    h_dXSec_KP_    = (TH1D*)h_dXSec_KP->Clone();
  }

  void Comparison()
  {
    DrawPlot();
    PlotTool::IsRatio1(h_dXSec_Ridhi_, h_dXSec_KP_);
  }

private:
  void DrawPlot()
  {
    TString canvasName = "c_comparison_dXSec_Ridhi_vs_KP_"+uncType_;
    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
    canvas->SetTitle("m(ee) [GeV]", "d#sigma/dm [pb/GeV]", "KP/Ridhi");

    canvas->Register(h_dXSec_Ridhi_, "Ridhi", kBlack);
    canvas->Register(h_dXSec_KP_,    "KP",    kBlue);

    canvas->SetRangeY(1e-7, 1e4);
    canvas->SetRangeRatio(0.94, 1.06);
    canvas->SetLegendPosition(0.82, 0.75, 0.95, 0.95);

    canvas->Latex_CMSPre();

    canvas->Draw("HISTLPSAME");
  }

};

void Comparison_GivenUncType(TString uncType)
{
  TString analyerPath = gSystem->Getenv("KP_ANALYZER_PATH");

  // -- use old electron efficiency
  TString baseDir_Ridhi = analyerPath+"/ElecChannel/Code_Ridhi/CentralValue/Dressed";
  TString fileName_Ridhi = baseDir_Ridhi+"/FinalCorr_v11.root";
  TString histName_Ridhi = "h_xSec_dM_FSRCorr";
  TH1D* h_dXSec_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);

  // -- use new electron efficiency
  TString baseDir_KP  = analyerPath+"/ElecChannel/CovMatrix/EfficiencySF";
  TString fileName_KP = baseDir_KP+"/ROOTFile_SmearedDXSecProducer_"+uncType+".root";
  TString histName_KP = "h_dXSec_cv";

  TH1D* h_dXSec_KP = PlotTool::Get_Hist(fileName_KP, histName_KP);

  ComparisonTool* tool = new ComparisonTool(uncType, h_dXSec_Ridhi, h_dXSec_KP);
  tool->Comparison();
}

void ComparisonDXSec_Ridhi_vs_KP()
{
  Comparison_GivenUncType("bkgChange");
  Comparison_GivenUncType("sgnChange");
  Comparison_GivenUncType("tagChange");
  Comparison_GivenUncType("nlo");
  Comparison_GivenUncType("stat");
}