#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString fileName_;
  TString effType_;

  vector<TString> vec_uncType_ = {"bkgChange", "sgnChange", "tagChange", "nlo"};
  vector<TString> vec_uncType_data_ = {"bkgChange", "sgnChange"};
  vector<TString> vec_uncType_mc_   = {"tagChange", "nlo"};


  vector<Double_t> vec_ptBinEdge_ = {10, 20, 30, 40, 50, 200};

  PlotProducer(TString fileName, TString effType)
  {
    fileName_ = fileName;
    effType_ = effType;

    if( effType_ == "trig" )
    {
      Int_t nPtBinEdge = (Int_t)vec_ptBinEdge_.size();
      vec_ptBinEdge_[nPtBinEdge-1] = 2000;
    }
  }

  void Produce()
  {
    if( effType_ == "reco" || effType_ == "ID" )
    {
      DrawEffPlot_CV_vs_Alt( "data" );
      DrawEffPlot_CV_vs_Alt( "mc" );
    }
    else if( effType_ == "trig" )
      DrawEffPlot_CV_vs_Alt( "mc" );

    DrawDiffPlot();
  }

private:
  void DrawEffPlot_CV_vs_Alt(TString dataType)
  {
    Int_t nPtBin = (Int_t)vec_ptBinEdge_.size()-1;
    for(Int_t i=0; i<nPtBin; i++)
      DrawEffPlot_CV_vs_Alt_PtRange( dataType, vec_ptBinEdge_[i], vec_ptBinEdge_[i+1]);
  }

  void DrawDiffPlot()
  {
    Int_t nPtBin = (Int_t)vec_ptBinEdge_.size()-1;
    for(Int_t i=0; i<nPtBin; i++)
      DrawDiffPlot_PtRange(vec_ptBinEdge_[i], vec_ptBinEdge_[i+1]);
  }

  void DrawEffPlot_CV_vs_Alt_PtRange(TString dataType, Double_t minPt, Double_t maxPt )
  {
    TString ptBinInfo = TString::Format("pt%.0lfto%.0lf", minPt, maxPt);

    TString canvasName = TString::Format("c_eff_%s_%s_%s", effType_.Data(), dataType.Data(), ptBinInfo.Data());
    PlotTool::GraphCanvaswRatio *canvas = new PlotTool::GraphCanvaswRatio(canvasName, 0, 0);
    canvas->SetTitle("#eta(e)", "Efficiency", "alt./central");

    TString histName_CV = TString::Format("h_eff_%s_%s", dataType.Data(), ptBinInfo.Data());
    TH1D* h_eff_cv = PlotTool::Get_Hist(fileName_, histName_CV);
    TGraphAsymmErrors* g_eff_cv = Convert_HistToGraph_noECALGAP(h_eff_cv);
    canvas->Register( g_eff_cv, "Central value", kBlack);

    vector<TString> vec_uncType;
    if( dataType == "data" ) vec_uncType = vec_uncType_data_;
    if( dataType == "mc" )   vec_uncType = vec_uncType_mc_;
    for(const auto& uncType : vec_uncType )
    {
      TString histName_alt = TString::Format("h_eff_%s_%s_%s", uncType.Data(), dataType.Data(), ptBinInfo.Data());
      TH1D* h_eff_alt = PlotTool::Get_Hist(fileName_, histName_alt);
      TGraphAsymmErrors* g_eff_alt = Convert_HistToGraph_noECALGAP(h_eff_alt);

      TString legend = "";
      if( uncType == "bkgChange" ) legend = "Alt. value by Background shape change";
      if( uncType == "sgnChange" ) legend = "Alt. value by Signal shape change";
      if( uncType == "tagChange" ) legend = "Alt. value by Tag condition change";
      if( uncType == "nlo" )       legend = "Alt. value by MC sample change: NLO #rightarrow LO";

      canvas->Register( g_eff_alt, legend, ColorForUnc(uncType) );
    }

    canvas->SetRangeY(0.8, 1.1);
    canvas->SetRangeRatio(0.97, 1.03);
    if( effType_ == "reco" && dataType == "data" && minPt == 10 ) canvas->SetRangeRatio(0.94, 1.06);
    canvas->SetLegendPosition(0.40, 0.75, 0.95, 0.95);

    canvas->Latex_CMSPre();

    TString typeLatex = "";
    if( effType_ == "reco")    typeLatex = "Reco. efficiency";
    if( effType_ == "ID")      typeLatex = "ID efficiency";
    if( effType_ == "trig")    typeLatex = "Trigger efficiency";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+typeLatex+"}}");

    TString ptInfoLatex = TString::Format("%.0lf < p_{T}(e) < %0.lf GeV", minPt, maxPt);
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+ptInfoLatex+"}}");

    canvas->Draw("LP");
  }

  void DrawDiffPlot_PtRange(Double_t minPt, Double_t maxPt )
  {
    TString ptBinInfo = TString::Format("pt%.0lfto%.0lf", minPt, maxPt);

    TString canvasName = TString::Format("c_diff_%s_%s", effType_.Data(), ptBinInfo.Data());
    PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);
    canvas->SetTitle("#eta(e)", "#varepsilon_{alt.} - #varepsilon_{cen.}");

    for(const auto& uncType : vec_uncType_ )
    {
      TString dataType = FindDataType(uncType);

      if( effType_ == "trig" && 
         (uncType == "bkgChange" || uncType == "sgnChange" ) ) continue;

      TString histName_alt = TString::Format("h_diff_%s_%s_%s", uncType.Data(), dataType.Data(), ptBinInfo.Data());
      TH1D* h_diff_alt = PlotTool::Get_Hist(fileName_, histName_alt);
      TGraphAsymmErrors* g_diff_alt = Convert_HistToGraph_noECALGAP(h_diff_alt);

      TString legend = "";
      if( uncType == "bkgChange" ) legend = "Background shape change (data)";
      if( uncType == "sgnChange" ) legend = "Signal shape change (data)";
      if( uncType == "tagChange" ) legend = "Tag condition change (MC)";
      if( uncType == "nlo" )       legend = "MC sample change: NLO #rightarrow LO (MC)";

      canvas->Register( g_diff_alt, legend, ColorForUnc(uncType) );
    }

    canvas->SetRangeY(-0.05, 0.08);
    if( effType_ == "trig" ) canvas->SetRangeY(-0.2, 0.1);
    canvas->SetLegendPosition(0.40, 0.75, 0.95, 0.95);

    canvas->Latex_CMSPre();

    TString typeLatex = "";
    if( effType_ == "reco")    typeLatex = "Reco. efficiency";
    if( effType_ == "ID")      typeLatex = "ID efficiency";
    if( effType_ == "trig")    typeLatex = "Trigger efficiency";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+typeLatex+"}}");

    TString ptInfoLatex = TString::Format("%.0lf < p_{T}(e) < %0.lf GeV", minPt, maxPt);
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+ptInfoLatex+"}}");

    canvas->Draw("LP");
  }

  TString FindDataType(TString uncType)
  {
    TString dataType = "";
    if( uncType == "bkgChange" || uncType == "sgnChange" ) dataType = "data";
    if( uncType == "tagChange" || uncType == "nlo" )       dataType = "mc";

    return dataType;
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

  Int_t ColorForUnc(TString uncType )
  {
    Int_t color = -1;
    if( uncType == "bkgChange" ) color =  kOrange+2;
    if( uncType == "sgnChange" ) color =  kGreen+2;
    if( uncType == "tagChange" ) color =  kViolet;
    // if( uncType == "nlo" ) color =  kCyan;
    if( uncType == "nlo" ) color =  kBlue;

    return color;
  }
};

void DrawPlot_Reco()
{
  TString fileName = "../ROOTFile_RecoSF_April2019.root";
  TString effType = "reco";
  PlotProducer *producer = new PlotProducer(fileName, effType);
  producer->Produce();
}

void DrawPlot_ID()
{
  TString fileName = "../ROOTFile_IDSF_April2019.root";
  TString effType = "ID";
  PlotProducer *producer = new PlotProducer(fileName, effType);
  producer->Produce();
}

void DrawPlot_Trig()
{
  TString fileName = "../ROOTFile_trigSF_SMP17001.root";
  TString effType = "trig";
  PlotProducer *producer = new PlotProducer(fileName, effType);
  producer->Produce();
}

void DrawPlot()
{
  DrawPlot_Reco();
  DrawPlot_ID();
  DrawPlot_Trig();
}