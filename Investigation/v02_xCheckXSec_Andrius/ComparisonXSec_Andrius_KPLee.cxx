#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>

class ComparisonTool
{
public:
  // -- KP: result used for the paper
  TH1D* h_dXSec_mm_Andrius_old_;
  TH1D* h_dXSec_mm_Andrius_new_;
  TH1D* h_dXSec_mm_KP_;

  TH1D* h_dXSec_ee_Andrius_old_;
  TH1D* h_dXSec_ee_Andrius_new_;
  TH1D* h_dXSec_ee_KP_;

  ComparisonTool()
  {
    Init();
  }

  void DrawPlot_All()
  {
    ComparisonPlot("muon", h_dXSec_mm_KP_, h_dXSec_mm_Andrius_old_, h_dXSec_mm_Andrius_new_);
    ComparisonPlot("electron", h_dXSec_ee_KP_, h_dXSec_ee_Andrius_old_, h_dXSec_ee_Andrius_new_);
  }

private:
  void Init()
  {
    // -- KP
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

    TString fileName_mm_KP = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root";
    h_dXSec_mm_KP_ = PlotTool::Get_Hist(fileName_mm_KP, "h_dXSec");

    TString fileName_ee_KP = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Electron.root";
    h_dXSec_ee_KP_ = PlotTool::Get_Hist(fileName_ee_KP, "h_dXSec");

    // -- Andrius
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName_Andrius_old = rootFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
    h_dXSec_mm_Andrius_old_ = PlotTool::Get_Hist(fileName_Andrius_old, "BLUE_inp/h1csMM");
    h_dXSec_ee_Andrius_old_ = PlotTool::Get_Hist(fileName_Andrius_old, "BLUE_inp/h1csEE");

    TString fileName_Andrius_new = rootFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20190208hepdata.root";
    h_dXSec_mm_Andrius_new_ = PlotTool::Get_Hist(fileName_Andrius_new, "BLUE_inp/h1csMM");
    h_dXSec_ee_Andrius_new_ = PlotTool::Get_Hist(fileName_Andrius_new, "BLUE_inp/h1csEE");
  }

  void ComparisonPlot(TString type, TH1D* h_dXSec_KP, TH1D* h_dXSec_Andrius_old, TH1D* h_dXSec_Andrius_new)
  {
    cout << "[ComparisonPlot] type = " << type << endl;
    TString canvasName = "c_"+type;
    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, 1, 1 );
    canvas->Register( h_dXSec_KP,          "KP Lee's root file (paper)", kRed);
    canvas->Register( h_dXSec_Andrius_old, "Andrius' root file (old: 20171204)",        kBlue);
    canvas->Register( h_dXSec_Andrius_new, "Andrius' root file (new: 20190208hepdata)", kGreen+2);
    canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "Andrius/KP");
    canvas->Latex_CMSPre();
    canvas->SetLegendPosition( 0.45, 0.75, 0.95, 0.95 );
    canvas->SetRangeY(1e-8, 1e+4);
    canvas->SetRangeRatio(0.96, 1.04);

    TString channelInfo = "";
    if( type == "muon" )     channelInfo = "#mu channel";
    if( type == "electron" ) channelInfo = "e channel";
    canvas->RegisterLatex(0.16, 0.91, "#font[62]{#scale[0.6]{"+channelInfo+"}}");
    canvas->Draw();

    PlotTool::IsRatio1(h_dXSec_KP, h_dXSec_Andrius_new);
  }
};

void ComparisonXSec_Andrius_KPLee()
{
  ComparisonTool* tool = new ComparisonTool();
  tool->DrawPlot_All();
}