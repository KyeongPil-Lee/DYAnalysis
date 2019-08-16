#include <Include/SimplePlotTools.h>

class HistContainer
{
public:
  TString fileName_;
  TString histName_base_;
  vector<Double_t> vec_ptBinEdge_;
  vector<TH1D*> vec_hist_;

  HistContainer(TString fileName, TString histName_base)
  {
    vec_ptBinEdge_.clear();

    fileName_ = fileName;
    histName_base_ = histName_base;

    Init();
  }

  HistContainer(TString fileName, TString histName_base, vector<Double_t> vec_ptBinEdge)
  {
    vec_ptBinEdge_.clear();

    fileName_ = fileName;
    histName_base_ = histName_base;
    vec_ptBinEdge_ = vec_ptBinEdge;
    Init();
  }

private:
  void Init()
  {
    if( vec_ptBinEdge_.size() == 0 )
    {
      vector<Double_t> vec_temp = {10, 20, 30, 40, 50, 200};
      vec_ptBinEdge_ = vec_temp;
    }

    Int_t nBin = (Int_t)vec_ptBinEdge_.size() - 1;
    for(Int_t i=0; i<nBin; i++)
    {
      TString histName = TString::Format("%s_pt%.0lfto%.0lf", histName_base_.Data(), vec_ptBinEdge_[i], vec_ptBinEdge_[i+1]);
      // cout << "histName: " << histName << endl;

      TH1D* h_temp = PlotTool::Get_Hist(fileName_, histName);
      vec_hist_.push_back( h_temp );
    }
  }


};

class PlotProducer
{
public:
  TString type_;

  vector<Double_t> vec_ptBinEdge_;

  HistContainer* hists_statUnc_data_;
  HistContainer* hists_statUnc_mc_;
  HistContainer* hists_relDiff_sgnChange_;
  HistContainer* hists_relDiff_bkgChange_;
  HistContainer* hists_relDiff_tagChange_;
  HistContainer* hists_relDiff_nlo_;
  HistContainer* hists_relDiff_oldEffData_;
  HistContainer* hists_relDiff_oldEffMC_;

  PlotProducer(TString type)
  {
    type_ = type;
    Init();
  }

  void Produce()
  {
    ComparisonPlot_Unc_All();
  }

private:
  void ComparisonPlot_Unc_All()
  {
    Int_t nBin = (Int_t)vec_ptBinEdge_.size() - 1;
    for(Int_t i=0; i<nBin; i++)
      ComparisonPlot_Unc(i);
  }

  void ComparisonPlot_Unc(Int_t i_pt)
  {
    TH1D* h_statUnc_data      = hists_statUnc_data_->vec_hist_[i_pt];
    TH1D* h_statUnc_mc        = hists_statUnc_mc_->vec_hist_[i_pt];
    TH1D* h_relDiff_sgnChange = hists_relDiff_sgnChange_->vec_hist_[i_pt];
    TH1D* h_relDiff_bkgChange = hists_relDiff_bkgChange_->vec_hist_[i_pt];
    TH1D* h_relDiff_tagChange = hists_relDiff_tagChange_->vec_hist_[i_pt];
    TH1D* h_relDiff_nlo       = hists_relDiff_nlo_->vec_hist_[i_pt];
    TH1D* h_relDiff_oldEffData = hists_relDiff_oldEffData_->vec_hist_[i_pt];
    TH1D* h_relDiff_oldEffMC   = hists_relDiff_oldEffMC_->vec_hist_[i_pt];

    TGraphAsymmErrors* g_statUnc_data = Convert_HistToGraph_noECALGAP(h_statUnc_data);
    TGraphAsymmErrors* g_statUnc_mc   = Convert_HistToGraph_noECALGAP(h_statUnc_mc);

    TGraphAsymmErrors* g_relDiff_sgnChange = Convert_HistToGraph_noECALGAP(h_relDiff_sgnChange);
    TGraphAsymmErrors* g_relDiff_bkgChange = Convert_HistToGraph_noECALGAP(h_relDiff_bkgChange);
    TGraphAsymmErrors* g_relDiff_tagChange = Convert_HistToGraph_noECALGAP(h_relDiff_tagChange);
    TGraphAsymmErrors* g_relDiff_nlo       = Convert_HistToGraph_noECALGAP(h_relDiff_nlo);
    TGraphAsymmErrors* g_relDiff_oldEffData = Convert_HistToGraph_noECALGAP(h_relDiff_oldEffData);
    TGraphAsymmErrors* g_relDiff_oldEffMC   = Convert_HistToGraph_noECALGAP(h_relDiff_oldEffMC);


    TString canvasName = TString::Format("c_unc_%s_pt%.0lfto%.0lf", type_.Data(), vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
    PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);
    // canvas->SetTitle("#eta(e)", "(#varepsilon_{alt.} - #varepsilon_{cen.}) / #varepsilon_{cen.} (Rel.diff.)");
    canvas->SetTitle("#eta(e)", "|#varepsilon_{alt.} - #varepsilon_{cen.}| / #varepsilon_{cen.} (Abs. rel. diff.)");

    canvas->Register( g_statUnc_data,      "Stat.unc. (data)", kBlack);
    canvas->Register( g_statUnc_mc,        "Stat.unc. (mc)",   kRed);
    canvas->Register( g_relDiff_sgnChange, "Signal shape", kGreen+2);
    canvas->Register( g_relDiff_bkgChange, "Bkg. shape", kOrange+2);
    canvas->Register( g_relDiff_tagChange, "Tag. change", kViolet);
    canvas->Register( g_relDiff_nlo,       "LO", kCyan);
    // canvas->Register( g_relDiff_oldEffData, "Old eff. (data)", kBlack);
    // canvas->Register( g_relDiff_oldEffMC,   "Old eff. (MC)",   kRed);

    canvas->SetRangeY(0, 0.07);
    canvas->SetLegendPosition(0.50, 0.75, 0.95, 0.95);

    canvas->Latex_CMSPre();

    TString typeLatex = "";
    if( type_ == "reco")    typeLatex = "Reco. efficiency";
    if( type_ == "ID")      typeLatex = "ID efficiency";
    if( type_ == "trigger") typeLatex = "Trigger efficiency";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+typeLatex+"}}");

    TString ptInfoLatex = TString::Format("%.0lf < p_{T}(e) < %0.lf GeV", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+ptInfoLatex+"}}");

    canvas->Draw("LP");
  }

  void Init()
  {
    vector<Double_t> vec_temp = {10, 20, 30, 40, 50, 200};
    vec_ptBinEdge_ = vec_temp;

    TString inputFileName = "";
    if( type_ == "reco" ) inputFileName = "ROOTFile_RecoSF_April2019.root";
    if( type_ == "ID" )   inputFileName = "ROOTFile_IDSF_April2019.root";

    // -- absolute values: need to be divided by eff.
    hists_statUnc_data_ = new HistContainer( inputFileName, "h_statUnc_data", vec_ptBinEdge_);
    hists_statUnc_mc_ = new HistContainer( inputFileName, "h_statUnc_mc", vec_ptBinEdge_);

    hists_relDiff_sgnChange_ = new HistContainer( inputFileName, "h_absRelDiff_sgnChange_data", vec_ptBinEdge_);
    hists_relDiff_bkgChange_ = new HistContainer( inputFileName, "h_absRelDiff_bkgChange_data", vec_ptBinEdge_);
    hists_relDiff_tagChange_ = new HistContainer( inputFileName, "h_absRelDiff_tagChange_mc", vec_ptBinEdge_);

    hists_relDiff_nlo_ = new HistContainer( inputFileName, "h_absRelDiff_nlo_mc", vec_ptBinEdge_);

    hists_relDiff_oldEffData_ = new HistContainer( inputFileName, "h_absRelDiff_oldEffData", vec_ptBinEdge_);
    hists_relDiff_oldEffMC_   = new HistContainer( inputFileName, "h_absRelDiff_oldEffMC", vec_ptBinEdge_);
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
  PlotProducer* producer_reco = new PlotProducer("reco");
  producer_reco->Produce();

  PlotProducer* producer_ID = new PlotProducer("ID");
  producer_ID->Produce();
}