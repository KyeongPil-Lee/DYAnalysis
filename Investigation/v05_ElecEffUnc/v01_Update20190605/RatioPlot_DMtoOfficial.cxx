#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString effType_;

  vector<Double_t> vec_ptBinEdge_;
  vector<Double_t> vec_etaBinEdge_;
  Int_t nPtBin_;
  Int_t nEtaBin_;

  vector<TH1D*> vec_hists_ratio_reco_data_;
  vector<TH1D*> vec_hists_ratio_reco_MC_;
  vector<TH1D*> vec_hists_ratio_ID_data_;
  vector<TH1D*> vec_hists_ratio_ID_MC_;

  PlotProducer()
  {
    Init();
  }

  void Produce()
  {
    for(Int_t i_pt=0; i_pt<nPtBin_; i_pt++)
      Comparison_GivenPtBin(i_pt);
  }

private:
  void Init()
  {
    vector<Double_t> vec_ptBinEdge_temp = {10, 20, 30, 40, 50, 200};
    vec_ptBinEdge_ = vec_ptBinEdge_temp;
    nPtBin_ = (Int_t)vec_ptBinEdge_.size() - 1;

    vector<Double_t> vec_etaBinEdge_temp = {-2.5, -2.0, -1.566, -1.444, -0.8, 0, 0.8, 1.444, 1.566, 2.0, 2.5};
    vec_etaBinEdge_ = vec_etaBinEdge_temp;
    nEtaBin_ = (Int_t)vec_etaBinEdge_temp.size() - 1;

    Init_HistVector( vec_hists_ratio_reco_data_, "reco", "data" );
    Init_HistVector( vec_hists_ratio_reco_MC_,   "reco", "MC" );
    Init_HistVector( vec_hists_ratio_ID_data_,   "ID",   "data" );
    Init_HistVector( vec_hists_ratio_ID_MC_,     "ID",   "MC" );
  }

  void Comparison_GivenPtBin(Int_t i_pt)
  {
    TGraphAsymmErrors* g_reco_data = Convert_HistToGraph_noECALGAP( vec_hists_ratio_reco_data_[i_pt] );
    TGraphAsymmErrors* g_reco_MC   = Convert_HistToGraph_noECALGAP( vec_hists_ratio_reco_MC_[i_pt] );
    TGraphAsymmErrors* g_ID_data   = Convert_HistToGraph_noECALGAP( vec_hists_ratio_ID_data_[i_pt] );
    TGraphAsymmErrors* g_ID_MC     = Convert_HistToGraph_noECALGAP( vec_hists_ratio_ID_MC_[i_pt] );

    TString canvasName = TString::Format("c_newToOldRatio_pt%.0lfto%.0lf", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
    PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);

    TString titleX = "#eta(e)";
    TString titleY = "Ratio of efficiency (new/old)";
    canvas->SetTitle(titleX, titleY);

    canvas->Register( g_reco_data, "new/old (Data, reco.eff.)", kBlack);
    canvas->Register( g_reco_MC,   "new/old (MC, reco.eff.)",   kRed);
    canvas->Register( g_ID_data,   "new/old (Data, ID eff.)", kGreen+2);
    canvas->Register( g_ID_MC,     "new/old (MC, ID eff.)",   kBlue);

    if( i_pt == 0 ) canvas->SetRangeY(0.97, 1.1);
    else            canvas->SetRangeY(0.98, 1.04);

    canvas->SetLegendPosition(0.50, 0.82, 0.95, 0.95);
    canvas->Latex_CMSPre();

    TString ptInfoLatex = TString::Format("%.0lf < p_{T}(e) < %0.lf GeV", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
    if( vec_ptBinEdge_[i_pt] == 50 )
      ptInfoLatex = TString::Format("p_{T}(e) > %0.lf GeV", vec_ptBinEdge_[i_pt]);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+ptInfoLatex+"}}");

    canvas->Draw("LP");
  }


  void Init_HistVector(vector<TH1D*>& vec_hists, TString effType, TString dataType)
  {
    TString fileName_old = "";
    TString fileName_new = "";

    if( effType == "reco" )
    {
      fileName_old = "../ROOTFile_RecoSF_SMP17001.root";
      fileName_new = "ROOTFile_RecoSF_05June2019.root";
    }
    else if( effType == "ID" )
    {
      fileName_old = "../ROOTFile_IDSF_SMP17001.root";
      fileName_new = "ROOTFile_IDSF_05June2019.root";
    }

    for(Int_t i=0; i<nPtBin_; i++)
    {
      Double_t ptMaxOld = 0;
      if( vec_ptBinEdge_[i+1] == 200 )  ptMaxOld = 2000;
      else                              ptMaxOld = vec_ptBinEdge_[i+1];

      TString histName_base = "";
      if( dataType == "data" ) histName_base = "h_eff_data";
      if( dataType == "MC" )   histName_base = "h_eff_mc";

      TString histName_old = TString::Format("%s_pt%.0lfto%.0lf", histName_base.Data(), vec_ptBinEdge_[i], ptMaxOld);
      TString histName_new = TString::Format("%s_pt%.0lfto%.0lf", histName_base.Data(), vec_ptBinEdge_[i], vec_ptBinEdge_[i+1]);

      TH1D* h_old = PlotTool::Get_Hist( fileName_old, histName_old );
      TH1D* h_new = PlotTool::Get_Hist( fileName_new, histName_new );

      h_old = Convert_SyncBinEdgeWithDM(h_old);
      if( effType == "ID" ) h_new = Convert_absEtaHistToEtaHist(h_new);



      TH1D* h_ratio = (TH1D*)h_old->Clone();
      h_ratio->Divide(h_new, h_old);

      vec_hists.push_back( h_ratio );
    }
  }

  TH1D* MakeHistFormat_vsEta(TString histName)
  {
    Double_t* arr_absEtaBinEdge = new Double_t[nEtaBin_+1];
    for(Int_t i=0; i<nEtaBin_+1; i++)
      arr_absEtaBinEdge[i] = vec_etaBinEdge_[i];

    TH1D* h_return = new TH1D(histName, "", nEtaBin_, arr_absEtaBinEdge);

    delete[] arr_absEtaBinEdge;

    return h_return;
  }

  TH1D* Convert_absEtaHistToEtaHist(TH1D* h)
  {
    TString histName = h->GetName();
    histName = histName + "_converted";

    TH1D* h_converted = MakeHistFormat_vsEta(histName);

    for(Int_t i=0; i<5; i++)
    {
      Int_t i_bin = i+1;
      Double_t value = h->GetBinContent(i_bin);
      Double_t error = h->GetBinError(i_bin);

      h_converted->SetBinContent(i_bin+5, value);
      h_converted->SetBinError(i_bin+5, error);

      h_converted->SetBinContent(6-i_bin, value);
      h_converted->SetBinError(6-i_bin, error);
    }

    return h_converted;
  }

  TH1D* Convert_SyncBinEdgeWithDM(TH1D* h)
  {
    if( h->GetNbinsX() != nEtaBin_ )
    {
      cout << "[Convert_SyncBinEdgeWithDM] h->GetNbinsX() != nEtaBin_ " << endl;
      return nullptr;      
    }

    TString histName = h->GetName();
    histName = histName + "_sync";
    TH1D* h_sync = MakeHistFormat_vsEta(histName);

    for(Int_t i=0; i<nEtaBin_; i++)
    {
      Int_t i_bin = i+1;
      Double_t value = h->GetBinContent(i_bin);
      Double_t error = h->GetBinError(i_bin);

      h_sync->SetBinContent(i_bin, value);
      h_sync->SetBinError(i_bin, error);
    }

    return h_sync;
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

void RatioPlot_DMtoOfficial()
{
  PlotProducer* producer = new PlotProducer();
  producer->Produce();
}