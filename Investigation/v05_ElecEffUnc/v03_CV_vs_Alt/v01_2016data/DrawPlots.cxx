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
  TString uncType_;
  TString fileName_;

  vector<Double_t> vec_ptBinEdge_;
  HistContainer* hists_cen_;
  HistContainer* hists_alt_;


  PlotProducer(TString type, TString uncType, TString fileName)
  {
    type_ = type;
    uncType_ = uncType;
    fileName_ = fileName;
    Init();
  }

  void Produce()
  {
    Draw_AbsDiff();
  }

private:
  void Draw_AbsDiff()
  {
    TH1::AddDirectory(kFALSE);

    TString canvasName = TString::Format("c_absDiff_%s_%s", type_.Data(), uncType_.Data() );
    PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);
    canvas->SetTitle("#eta(e)", "#varepsilon_{alt.} - #varepsilon_{cen.}");

    Int_t entry_plus  = 0;
    Int_t entry_minus = 0;

    vector<Int_t> vec_color = {kBlack, kRed, kGreen+2, kBlue, kViolet, kOrange+2};
    Int_t nPtBin = (Int_t)vec_ptBinEdge_.size() - 1;
    for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
    {
      TH1D* h_cen = hists_cen_->vec_hist_[i_pt];
      TH1D* h_alt = hists_alt_->vec_hist_[i_pt];
      TString histName = TString::Format("h_absDiff_pt%.0lfto%.0lf", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
      TH1D* h_absDiff = PlotTool::HistOperation(histName, h_alt, h_cen, "-");
      TGraphAsymmErrors* g_absDiff = Convert_HistToGraph_noECALGAP(h_absDiff);

      TString legend = TString::Format("%.0lf < p_{T} < %.0lf GeV", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
      canvas->Register( g_absDiff, legend, vec_color[i_pt]);

      // -- print values
      for(Int_t i=0; i<g_absDiff->GetN(); i++)
      {
        Double_t eta, absDiff;
        g_absDiff->GetPoint(i, eta, absDiff);
        printf("%lf\t", absDiff);

        if( absDiff >= 0 ) entry_plus++;
        if( absDiff < 0 ) entry_minus++;
      }
      cout << endl;
    }
    printf("(entry_plus, entry_minus) = (%d, %d)\n", entry_plus, entry_minus);

    canvas->SetRangeY(-0.05, 0.04);
    if( uncType_ == "sgnChange" ) canvas->SetLegendPosition(0.50, 0.20, 0.95, 0.40);
    else                          canvas->SetLegendPosition(0.50, 0.65, 0.95, 0.85);
    canvas->Latex_CMSPre();

    TString typeLatex = "";
    if( type_ == "reco")    typeLatex = "Reco. efficiency";
    if( type_ == "ID")      typeLatex = "ID efficiency";
    if( type_ == "trigger") typeLatex = "Trigger efficiency";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+typeLatex+"}}");

    TString uncInfo = "";
    if( uncType_ == "sgnChange" ) uncInfo = "#varepsilon_{alt.} = Data efficiency from alternative signal shape";
    if( uncType_ == "bkgChange" ) uncInfo = "#varepsilon_{alt.} = Data efficiency from alternative background shape";
    if( uncType_ == "tagChange" ) uncInfo = "#varepsilon_{alt.} = MC efficiency from alternative tag selection";
    if( uncType_ == "nlo" )    uncInfo = "#varepsilon_{alt.} = MC efficiency from alternative MC sample (LO)";
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+uncInfo+"}}");

    canvas->Draw("LP");

    cout << "[Draw_AbsDiff] Done" << endl;
    cout << endl;
  }

  void Init()
  {
    if( type_ == "reco" )
    {
      vector<Double_t> vec_temp = {25, 2000};
      vec_ptBinEdge_ = vec_temp;
    }
    else if( type_ == "ID" )
    {
      vector<Double_t> vec_temp = {10, 20, 35, 50, 90, 150, 500};
      vec_ptBinEdge_ = vec_temp;
    }

    TString sampleType = "";
    if( uncType_ == "sgnChange" || uncType_ == "bkgChange" ) sampleType = "data";
    if( uncType_ == "tagChange" || uncType_ == "nlo" )       sampleType = "mc";

    hists_cen_  = new HistContainer( fileName_, "h_eff_"+sampleType,       vec_ptBinEdge_);
    hists_alt_  = new HistContainer( fileName_, "h_eff_"+uncType_+"_"+sampleType, vec_ptBinEdge_);
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


void DrawPlots_Reco()
{
  TString fileName = "ROOTFile_RecoSF_2016_Moriond17.root";
  vector<TString> vec_uncType = {"sgnChange", "bkgChange", "tagChange", "nlo"};
  for(const auto& uncType : vec_uncType )
  {
    PlotProducer* producer = new PlotProducer("reco", uncType, fileName);
    producer->Produce();
  }
}

void DrawPlots_ID()
{
  TString fileName = "ROOTFile_IDSF_2016_Moriond17.root";
  vector<TString> vec_uncType = {"sgnChange", "bkgChange", "tagChange", "nlo"};
  for(const auto& uncType : vec_uncType )
  {
    PlotProducer* producer = new PlotProducer("ID", uncType, fileName);
    producer->Produce();
  }
}

void DrawPlots()
{
  DrawPlots_Reco();
  DrawPlots_ID();
}