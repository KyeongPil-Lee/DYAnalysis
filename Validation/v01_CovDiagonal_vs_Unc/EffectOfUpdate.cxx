#include <Include/SimplePlotTools.h>

class ComparisonTool
{
public:
  TH1D* h_dXSec_before_;
  TH1D* h_absUnc_tot_before_;
  TH1D* h_relUnc_tot_before_;

  TH1D* h_dXSec_after_;
  TH1D* h_absUnc_tot_after_;
  TH1D* h_relUnc_tot_after_;

  ComparisonTool()
  {
    Init();
  }

  void Comparison_All()
  {
    Comparison_before_vs_after("dXSec",      h_dXSec_before_,      h_dXSec_after_ );
    Comparison_before_vs_after("absUnc_tot", h_absUnc_tot_before_, h_absUnc_tot_after_ );
    Comparison_before_vs_after("relUnc_tot", h_relUnc_tot_before_, h_relUnc_tot_after_ );
  }

private:
  void Comparison_before_vs_after(TString type, TH1D* h_before, TH1D* h_after)
  {
    TString canvasName = TString::Format("c_beforeAfter_%s", type.Data());

    Bool_t isLogY = kTRUE;
    // if( type == "relUnc" ) isLogY = kFALSE;

    TString titleY = "";
    if( type == "dXSec"  ) titleY = "d#sigma/dm [pb/GeV]";
    if( type == "absUnc_tot" ) titleY = "Total unc. [pb/GeV]";
    if( type == "relUnc_tot" )
    {
      h_before->Scale(100);
      h_after->Scale(100);
      titleY = "Total unc. (%)";
    }

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, 1, isLogY );
    canvas->Register( h_before, "Before update (current paper)", kRed);
    canvas->Register( h_after, "After update", kBlack);
    canvas->SetTitle("m [GeV]", titleY, "after/before");

    canvas->Latex_CMSPre();
    canvas->SetLegendPosition( 0.45, 0.82, 0.95, 0.95 );
    canvas->SetRangeY(1e-8, 1e+4);
    if( type == "relUnc_tot" ) canvas->SetRangeY(1, 2e+2);

    // canvas->SetRangeRatio(0.84, 1.16);
    canvas->SetRangeRatio(0.3, 1.7);
    if( type == "relUnc_tot" ) canvas->SetRangeRatio(0.7, 1.3);

    // canvas->RegisterLatex(0.16, 0.91, "#font[62]{#scale[0.6]{"+channelInfo+"}}");
    if( type.Contains("Unc") ) canvas->Draw("HISTLPSAME");
    else                       canvas->Draw();

  }

  void Init()
  {
    Init_Before();
    Init_After();
  }

  void Init_Before()
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Combined.root";

    h_dXSec_before_ = PlotTool::Get_Hist(fileName, "h_dXSec");
    h_absUnc_tot_before_ = PlotTool::Get_Hist(fileName, "h_absUnc_tot");
    h_relUnc_tot_before_ = PlotTool::Get_Hist(fileName, "h_relUnc_tot");      
  }

  void Init_After()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";

    h_dXSec_after_ = PlotTool::Get_Hist(fileName, "ll/h_dXSec");
    h_absUnc_tot_after_ = PlotTool::Get_Hist(fileName, "ll/h_absUnc_tot");
    h_relUnc_tot_after_ = PlotTool::Extract_RelUnc(h_dXSec_after_);
  }
};

void EffectOfUpdate()
{
  ComparisonTool *tool = new ComparisonTool();
  tool->Comparison_All();
}