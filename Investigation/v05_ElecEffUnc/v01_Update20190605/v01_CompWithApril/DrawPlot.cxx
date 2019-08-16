#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString fileName_old_;
  TString fileName_new_;

  PlotProducer(TString fileName_old, TString fileName_new)
  {
    fileName_old_ = fileName_old;
    fileName_new_ = fileName_new;
  }

  void Produce()
  {
    DrawComparisonPlot();
  }

private:
  void DrawComparisonPlot()
  {
    TString histName = "h_eff_data_pt10to20";

    TH1D* h_old = PlotTool::Get_Hist(fileName_old_, histName);
    TH1D* h_new = PlotTool::Get_Hist(fileName_new_, histName);

    TGraphAsymmErrors* g_old = Convert_HistToGraph_noECALGAP(h_old);
    TGraphAsymmErrors* g_new = Convert_HistToGraph_noECALGAP(h_new);

    TString canvasName = histName;
    canvasName.ReplaceAll("h_", "c_");
    PlotTool::GraphCanvaswRatio *canvas = new PlotTool::GraphCanvaswRatio(canvasName, 0, 0);
    // canvas->SetTitle("#eta(e)", "(#varepsilon_{alt.} - #varepsilon_{cen.}) / #varepsilon_{cen.} (Rel.diff.)");
    canvas->SetTitle("#eta(e)", "Efficiency", "Red/Black");

    canvas->Register( g_old, "Nominal value (April)",          kBlack);
    canvas->Register( g_new, "Alternative value with a new fit", kRed);
    canvas->SetRangeY(0.8, 1.1);
    canvas->SetRangeRatio(0.93, 1.01);
    canvas->SetLegendPosition(0.50, 0.82, 0.95, 0.95);

    canvas->Latex_CMSPre();

    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{Reco. efficiency}}");

    TString ptInfoLatex = TString::Format("%.0lf < p_{T}(e) < %0.lf GeV", 10.0, 20.0);
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+ptInfoLatex+"}}");

    canvas->Draw("LP");
  }


  TGraphAsymmErrors* Convert_HistToGraph_noECALGAP(TH1D* h)
  {
    TGraphAsymmErrors* g = new TGraphAsymmErrors();

    Int_t nBin = h->GetNbinsX();

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t binLowEdge = h->GetBinLowEdge(i_bin);
      if( binLowEdge == -1.566 || binLowEdge == 1.444 ) continue; // -- do not include ECAL gap: meaningless values

      Double_t x     = h->GetBinCenter(i_bin);
      Double_t xErr  = h->GetBinWidth(i_bin) / 2.0;

      Double_t y     = h->GetBinContent(i_bin);
      Double_t yErr  = h->GetBinError(i_bin);

      // printf("(i, x, y) = (%d, %lf, %lf)\n", i, x, y);

      g->SetPoint(g->GetN(), x, y);
      g->SetPointError(g->GetN()-1, xErr, xErr, yErr, yErr);
    }

    return g;
  }

};


void DrawPlot()
{
  TString fileName_old = "../../ROOTFile_RecoSF_April2019.root";
  TString fileName_new = "../ROOTFile_RecoSF_05June2019.root";
  PlotProducer* producer = new PlotProducer(fileName_old, fileName_new);
  producer->Produce();
}