#include <Include/SimplePlotTools.h>

class ComparisonTool
{
public:
  TString effType_;
  TString type_;

  vector<Double_t> vec_ptBinEdge_;
  vector<Double_t> vec_etaBinEdge_;
  Int_t nPtBin_;
  Int_t nEtaBin_;

  vector<TH1D*> vec_hists_official_;
  vector<TH1D*> vec_hists_DM_;

  ComparisonTool(TString effType, TString type)
  {
    effType_ = effType;
    type_ = type;

  }

  void Comparison()
  {
    Init();

    for(Int_t i_pt=0; i_pt<nPtBin_; i_pt++)
    {
      if( type_.Contains("eff_") || type_.Contains("absRelDiff_") )  ComparisonwRatio_GivenPtBin(i_pt);
      if( type_.Contains("relDiff_") || type_.Contains("statUnc_") ) ComparisonNoRatio_GivenPtBin(i_pt);
      if( type_ == "SF" ) ComparisonwRatio_GivenPtBin(i_pt);
    }
  }

private:
  void ComparisonNoRatio_GivenPtBin(Int_t i_pt)
  {
    TGraphAsymmErrors* g_official = Convert_HistToGraph_noECALGAP( vec_hists_official_[i_pt] );
    TGraphAsymmErrors* g_DM       = Convert_HistToGraph_noECALGAP( vec_hists_DM_[i_pt] );

    TString canvasName = TString::Format("c_off_vs_DM_%s_%s_pt%.0lfto%.0lf", effType_.Data(), type_.Data(), vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
    PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);

    TString titleX = "undefined";
    if( effType_ == "reco" ) titleX = "#eta(e)";
    if( effType_ == "ID" )   titleX = "|#eta(e)|";

    canvas->SetTitle(titleX, "Value");

    canvas->Register( g_official, "Official value",   kBlack);
    canvas->Register( g_DM,       "Reproduced value", kRed);
    if( type_.Contains("statUnc_") ) canvas->SetRangeY(0, 0.01);
    if( type_.Contains("relDiff_") ) canvas->SetRangeY(-0.06, 0.06);
    if( type_ == "SF" ) canvas->SetRangeY(0.9, 1.1);
    
    canvas->SetLegendPosition(0.50, 0.78, 0.95, 0.91);

    canvas->Latex_CMSPre();

    TString typeLatex = TypeLatex();
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+typeLatex+" ("+effType_+")}}");

    TString ptInfoLatex = TString::Format("%.0lf < p_{T}(e) < %0.lf GeV", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
    if( vec_ptBinEdge_[i_pt] == 50 )
      ptInfoLatex = TString::Format("p_{T}(e) > %0.lf GeV", vec_ptBinEdge_[i_pt]);
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+ptInfoLatex+"}}");

    canvas->Draw("LP");
  }

  void ComparisonwRatio_GivenPtBin(Int_t i_pt)
  {
    TGraphAsymmErrors* g_official = Convert_HistToGraph_noECALGAP( vec_hists_official_[i_pt] );
    TGraphAsymmErrors* g_DM       = Convert_HistToGraph_noECALGAP( vec_hists_DM_[i_pt] );

    TString canvasName = TString::Format("c_off_vs_DM_%s_%s_pt%.0lfto%.0lf", effType_.Data(), type_.Data(), vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
    PlotTool::GraphCanvaswRatio *canvas = new PlotTool::GraphCanvaswRatio(canvasName, 0, 0);

    TString titleX = "undefined";
    if( effType_ == "reco" ) titleX = "#eta(e)";
    if( effType_ == "ID" )   titleX = "|#eta(e)|";
    canvas->SetTitle(titleX, "Value", "new/old");

    canvas->Register( g_official, "Official value", kBlack);
    canvas->Register( g_DM,       "Reproduced value", kRed);
    if( effType_ == "reco" && type_.Contains("eff_") ) canvas->SetRangeY(0.6, 1.2);
    if( effType_ == "ID"   && type_.Contains("eff_") ) canvas->SetRangeY(0.3, 1.15);
    if( effType_ == "reco" && type_.Contains("absRelDiff_") ) canvas->SetRangeY(0, 0.15);
    if( effType_ == "ID"   && type_.Contains("absRelDiff_") ) canvas->SetRangeY(0, 0.15);

    if( effType_ == "reco" && type_.Contains("eff_") ) canvas->SetRangeRatio(0.97, 1.03);
    if( effType_ == "ID"   && type_.Contains("eff_") ) canvas->SetRangeRatio(0.97, 1.03);
    if( type_.Contains("absRelDiff_") ) canvas->SetRangeRatio(0, 2.5);
    if( type_ == "SF" ) canvas->SetRangeY(0.9, 1.15);
    if( type_ == "SF" ) canvas->SetRangeRatio(0.99, 1.01);
    
    canvas->SetLegendPosition(0.50, 0.78, 0.95, 0.91);

    canvas->Latex_CMSPre();

    TString typeLatex = TypeLatex();
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+typeLatex+" ("+effType_+")}}");

    TString ptInfoLatex = TString::Format("%.0lf < p_{T}(e) < %0.lf GeV", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
    if( vec_ptBinEdge_[i_pt] == 50 )
      ptInfoLatex = TString::Format("p_{T}(e) > %0.lf GeV", vec_ptBinEdge_[i_pt]);
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+ptInfoLatex+"}}");

    canvas->Draw("LP");
  }

  TString TypeLatex()
  {
    TString latex = "undefined";
    if( type_ == "eff_data" ) latex = "Data efficiency";
    if( type_ == "eff_mc" )   latex = "MC efficiency";

    if( type_ == "statUnc_data" )   latex = "Data Stat. unc.";
    if( type_ == "statUnc_mc" )     latex = "MC Stat. unc.";

    if( type_ == "eff_bkgChange_data" ) latex = "Data efficiency with alternative bkg. shape";
    if( type_ == "eff_sgnChange_data" ) latex = "Data efficiency with alternative signal shape";
    if( type_ == "eff_tagChange_mc" )   latex = "MC efficiency with alternative tag selection";
    if( type_ == "eff_nlo_mc" )         latex = "MC efficiency with alternative MC sample";

    if( type_ == "relDiff_bkgChange_data" ) latex = "(#varepsilon_{alt.} - #varepsilon_{cen.}) / #varepsilon_{cen.} (bkg. change)";
    if( type_ == "relDiff_sgnChange_data" ) latex = "(#varepsilon_{alt.} - #varepsilon_{cen.}) / #varepsilon_{cen.} (signal change)";
    if( type_ == "relDiff_tagChange_mc" ) latex = "(#varepsilon_{alt.} - #varepsilon_{cen.}) / #varepsilon_{cen.} (tag change)";
    if( type_ == "relDiff_nlo_mc" )         latex = "(#varepsilon_{alt.} - #varepsilon_{cen.}) / #varepsilon_{cen.} (MC sample change)";

    if( type_ == "absRelDiff_bkgChange_data" ) latex = "Relative uncertainty (bkg. change)";
    if( type_ == "absRelDiff_sgnChange_data" ) latex = "Relative uncertainty (signal change)";
    if( type_ == "absRelDiff_tagChange_mc" )   latex = "Relative uncertainty (tag change)";
    if( type_ == "absRelDiff_nlo_mc" )         latex = "Relative uncertainty (MC sample change)";
    if( type_ == "absRelDiff_tot" )            latex = "Relative uncertainty (total)";

    if( type_ == "SF" ) latex = "Scale factor (data/MC)";
 
    return latex;
  }

  void Init()
  {
    vector<Double_t> vec_ptBinEdge_temp = {10, 20, 30, 40, 50, 200};
    vec_ptBinEdge_ = vec_ptBinEdge_temp;
    nPtBin_ = (Int_t)vec_ptBinEdge_.size() - 1;

    if( effType_ == "reco" )
    {
      vector<Double_t> vec_etaBinEdge_temp = {-2.5, -2.0, -1.566, -1.444, -0.8, 0, 0.8, 1.444, 1.566, 2.0, 2.5};
      vec_etaBinEdge_ = vec_etaBinEdge_temp;
      nEtaBin_ = (Int_t)vec_etaBinEdge_temp.size() - 1;
    }

    if( effType_ == "ID" )
    {
      vector<Double_t> vec_etaBinEdge_temp = {0, 0.8, 1.444, 1.566, 2.0, 2.5};
      vec_etaBinEdge_ = vec_etaBinEdge_temp;
      nEtaBin_ = (Int_t)vec_etaBinEdge_temp.size() - 1;
    }

    Init_Official();
    Init_DM();
  }

  void Init_Official()
  {
    TString fileName = "";
    if( effType_ == "reco" ) fileName = "ROOTFile_RecoSF_SMP17001.root";
    if( effType_ == "ID" )   fileName = "ROOTFile_IDSF_SMP17001.root";

    TString histName_base = "h_"+type_;

    for(Int_t i=0; i<nPtBin_; i++)
    {
      Double_t ptMax = 0;
      if( vec_ptBinEdge_[i+1] == 200 )  ptMax = 2000;
      else                              ptMax = vec_ptBinEdge_[i+1];

      TH1D* h_temp = nullptr;
      if( type_ == "SF" )
      {
        TString histName_data = TString::Format("h_eff_data_pt%.0lfto%.0lf", vec_ptBinEdge_[i], ptMax);
        TString histName_mc   = TString::Format("h_eff_mc_pt%.0lfto%.0lf", vec_ptBinEdge_[i], ptMax);

        TH1D* h_eff_data = PlotTool::Get_Hist(fileName, histName_data);
        TH1D* h_eff_mc = PlotTool::Get_Hist(fileName, histName_mc);
        h_temp = (TH1D*)h_eff_data->Clone();
        h_temp->Divide(h_eff_data, h_eff_mc); // -- data/MC
        h_temp->SetName("h_SF");
      }
      else
      {
        TString histName = TString::Format("%s_pt%.0lfto%.0lf", histName_base.Data(), vec_ptBinEdge_[i], ptMax);
        h_temp = PlotTool::Get_Hist(fileName, histName);
      }
      cout << h_temp->GetName() << endl;


      // -- x-axis: eta -> abs(eta) to be compared with DM's result
      if( effType_ == "ID" )
      {
        TH1D* h_temp_absEta = Convert_absEtaHist(h_temp);
        vec_hists_official_.push_back( h_temp_absEta );
      }

      // -- bin edge for ECAL gap is slightly different -> need to be sync. with DM's histograms
      if( effType_ == "reco" )
      {
        TH1D* h_temp_syncBinEdge = Convert_SyncBinEdgeWithDM(h_temp);
        vec_hists_official_.push_back( h_temp_syncBinEdge );
      }
    }
  }

  void Init_DM()
  {
    TString fileName = "undefined";
    if( effType_ == "reco" ) fileName = "ROOTFile_RecoSF_April2019.root";
    if( effType_ == "ID" )   fileName = "ROOTFile_IDSF_April2019.root";

    TString histName_base = "h_"+type_;

    for(Int_t i=0; i<nPtBin_; i++)
    {
      TH1D* h_temp = nullptr;
      if( type_ == "SF" )
      {
        TString histName_data = TString::Format("h_eff_data_pt%.0lfto%.0lf", vec_ptBinEdge_[i], vec_ptBinEdge_[i+1]);
        TString histName_mc   = TString::Format("h_eff_mc_pt%.0lfto%.0lf", vec_ptBinEdge_[i], vec_ptBinEdge_[i+1]);

        TH1D* h_eff_data = PlotTool::Get_Hist(fileName, histName_data);
        TH1D* h_eff_mc = PlotTool::Get_Hist(fileName, histName_mc);
        h_temp = (TH1D*)h_eff_data->Clone();
        h_temp->Divide(h_eff_data, h_eff_mc); // -- data/MC
        h_temp->SetName("h_SF");
      }
      else
      {
        TString histName = TString::Format("%s_pt%.0lfto%.0lf", histName_base.Data(), vec_ptBinEdge_[i], vec_ptBinEdge_[i+1]);
        h_temp = PlotTool::Get_Hist(fileName, histName);
      }
      cout << h_temp->GetName() << endl;

      vec_hists_DM_.push_back( h_temp );
    }
  }

  TH1D* Convert_SyncBinEdgeWithDM(TH1D* h)
  {
    if( h->GetNbinsX() != nEtaBin_ )
    {
      cout << "[Convert_SyncBinEdgeWithDM] h->GetNbinsX() != nEtaBin_ " << endl;
      return nullptr;      
    }

    Double_t* arr_absEtaBinEdge = new Double_t[nEtaBin_+1];
    for(Int_t i=0; i<nEtaBin_+1; i++)
      arr_absEtaBinEdge[i] = vec_etaBinEdge_[i];

    TString histName = h->GetName();
    histName = histName + "_sync";
    TH1D* h_sync = new TH1D(histName, "", nEtaBin_, arr_absEtaBinEdge);

    for(Int_t i=0; i<nEtaBin_; i++)
    {
      Int_t i_bin = i+1;
      Double_t value = h->GetBinContent(i_bin);
      Double_t error = h->GetBinError(i_bin);

      h_sync->SetBinContent(i_bin, value);
      h_sync->SetBinError(i_bin, error);
    }

    delete[] arr_absEtaBinEdge;

    return h_sync;
  }

  TH1D* Convert_absEtaHist(TH1D* h )
  {
    Double_t* arr_absEtaBinEdge = new Double_t[nEtaBin_+1];
    for(Int_t i=0; i<nEtaBin_+1; i++)
      arr_absEtaBinEdge[i] = vec_etaBinEdge_[i];

    TString histName = h->GetName();
    histName = histName + "_absEta";
    TH1D* h_absEta = new TH1D(histName, "", nEtaBin_, arr_absEtaBinEdge);

    for(Int_t i=0; i<nEtaBin_; i++)
    {
      Int_t i_absEtaBin = i+1;
      Int_t i_etaBin = i_absEtaBin+5;

      Double_t value = h->GetBinContent(i_etaBin);
      Double_t error = h->GetBinError(i_etaBin);

      h_absEta->SetBinContent(i_absEtaBin, value);
      h_absEta->SetBinError(i_absEtaBin, error);
    }

    delete[] arr_absEtaBinEdge;

    return h_absEta;
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

void Comparison_Official_vs_DM()
{
  vector<TString> vec_type = {"eff_data", "statUnc_data", "eff_mc", "statUnc_mc", "eff_bkgChange_data", "eff_sgnChange_data", "eff_nlo_mc", "eff_tagChange_mc",
                              "relDiff_bkgChange_data", "relDiff_sgnChange_data", "relDiff_nlo_mc", "relDiff_tagChange_mc",
                              "absRelDiff_bkgChange_data", "absRelDiff_sgnChange_data", "absRelDiff_nlo_mc", "absRelDiff_tagChange_mc", "absRelDiff_tot", "SF"};

  // vector<TString> vec_type = {"eff_data", "statUnc_data", "eff_mc", "statUnc_mc", "eff_bkgChange_data", "eff_sgnChange_data", "eff_nlo_mc", "eff_tagChange_mc",
  //                             "relDiff_bkgChange_data", "relDiff_sgnChange_data", "relDiff_nlo_mc", "relDiff_tagChange_mc",
  //                             "absRelDiff_bkgChange_data", "absRelDiff_sgnChange_data", "absRelDiff_nlo_mc", "absRelDiff_tagChange_mc", "absRelDiff_tot"};

  for(const auto& type: vec_type )
  {
    ComparisonTool* tool = new ComparisonTool("reco", type);
    tool->Comparison();
  }

  for(const auto& type: vec_type )
  {
    ComparisonTool* tool = new ComparisonTool("ID", type);
    tool->Comparison();
  }
}