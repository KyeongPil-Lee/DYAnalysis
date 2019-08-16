#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TH1D* h_absContToChi2_;
  TH1D* h_relContToChi2_;

  PlotProducer()
  {
    Init();
  }

  void Produce()
  {
    ComparisonPlot_OldNew("absCont", h_absContToChi2_);
    ComparisonPlot_OldNew("relCont", h_relContToChi2_);
  }

private:
  void ComparisonPlot_OldNew(TString type, TH1D* h)
  {
    TString canvasName = "c_contToChi2_"+type;
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas( canvasName, 1, 0 );

    canvas->Register( h, "Comparison between e & #mu channels", kBlack);

    TString titleY = "";
    if( type == "absCont" ) titleY = "Abs. contribution to #chi^{2}";
    if( type == "relCont" ) titleY = "Rel. contribution to #chi^{2} (%)";
    canvas->SetTitle("m [GeV]", titleY);

    if( type == "absCont" ) canvas->SetRangeY(-1, 15);
    if( type == "relCont" ) canvas->SetRangeY(-1, 35);

    canvas->SetLegendPosition(0.16, 0.82, 0.80, 0.95);

    canvas->Draw("HISTLPSAME");
  }

  void Init()
  {
    TString fileName = "ROOTFile_Compatibility_eemm.root";

    h_absContToChi2_ = PlotTool::Get_Hist( fileName, "h_absContToChi2" );
    h_relContToChi2_ = PlotTool::Get_Hist( fileName, "h_relContToChi2" );
  }
};

void DrawPlot()
{
  PlotProducer* producer = new PlotProducer();
  producer->Produce();
}