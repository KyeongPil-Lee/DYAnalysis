#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString fileName_nom_;
  TString fileName_alt_;

  PlotProducer(TString fileName_nom, TString fileName_alt)
  {
    fileName_nom_ = fileName_nom;
    fileName_alt_ = fileName_alt;
  }

  void Produce()
  {
    DrawComparisonPlot();
  }

private:
  void DrawComparisonPlot()
  {
    TH1D* h_nom = PlotTool::Get_Hist(fileName_nom_, "h_eff_data_pt10to20");
    TH1D* h_alt_plus  = PlotTool::Get_Hist(fileName_alt_, "h_eff_plusSigma");
    TH1D* h_alt_minus = PlotTool::Get_Hist(fileName_alt_, "h_eff_minusSigma");

    TGraphAsymmErrors* g_nom = Convert_HistToGraph_noECALGAP(h_nom);
    TGraphAsymmErrors* g_alt_plus  = Convert_HistToGraph_noECALGAP(h_alt_plus);
    TGraphAsymmErrors* g_alt_minus = Convert_HistToGraph_noECALGAP(h_alt_minus);

    TString canvasName = "c_comparison_eff";
    PlotTool::GraphCanvaswRatio *canvas = new PlotTool::GraphCanvaswRatio(canvasName, 0, 0);
    // canvas->SetTitle("#eta(e)", "(#varepsilon_{alt.} - #varepsilon_{cen.}) / #varepsilon_{cen.} (Rel.diff.)");
    canvas->SetTitle("#eta(e)", "Efficiency", "Ratio to nominal");

    canvas->Register( g_nom, "Nominal value (April)",          kBlack);
    canvas->Register( g_alt_plus,  "Alternative value with init. fit param. = nom. fit. param. + 1#sigma", kBlue);
    canvas->Register( g_alt_minus, "Alternative value with init. fit param. = nom. fit. param. - 1#sigma", kGreen+2);
    canvas->SetRangeY(0.7, 1.1);
    canvas->SetRangeRatio(0.99, 1.01);
    canvas->SetLegendPosition(0.20, 0.32, 0.95, 0.50);

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
  TString fileName_nom = "../ROOTFile_RecoSF_April2019.root";
  TString fileName_alt = "ROOTFile_Eff_Pt10to20_VariedInitFitParam.root";
  PlotProducer* producer = new PlotProducer(fileName_nom, fileName_alt);
  producer->Produce();
}