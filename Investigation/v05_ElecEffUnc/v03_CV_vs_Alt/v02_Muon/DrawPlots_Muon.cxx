#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString effType_; // -- RecoID, Iso, HLTv4p2, HLTv4p3
  TString sampleType_; // -- Data, MC
  TString uncType_; // -- sgnChange, bkgChange ...

  TString fileName_cen_;
  TString fileName_alt_;

  TH2D* h2D_eff_cen_;
  TH2D* h2D_eff_alt_;

  vector<Double_t> vec_etaBinEdge_ = {-2.4, -1.2, -0.3, -0.3, 1.2, 2.4};
  vector<Double_t> vec_ptBinEdge_ = {10, 22, 40, 70, 250};

  vector<TH1D*> vec_effHist_cen_;
  vector<TH1D*> vec_effHist_alt_;

  PlotProducer(TString effType, TString sampleType, TString uncType, TString fileName_cen, TString fileName_alt)
  {
    effType_    = effType;
    sampleType_ = sampleType;
    uncType_    = uncType;

    fileName_cen_ = fileName_cen;
    fileName_alt_ = fileName_alt;

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

    TString canvasName = TString::Format("c_absDiff_%s_%s_%s", effType_.Data(), uncType_.Data(), sampleType_.Data() );
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);
    canvas->SetTitle("#eta(e)", "#varepsilon_{alt.} - #varepsilon_{cen.}");

    Int_t entry_plus  = 0;
    Int_t entry_minus = 0;

    vector<Int_t> vec_color = {kBlack, kRed, kGreen+2, kBlue, kViolet, kOrange+2};
    Int_t nPtBin = (Int_t)vec_ptBinEdge_.size() - 1;
    for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
    {
      TH1D* h_cen = vec_effHist_cen_[i_pt];
      TH1D* h_alt = vec_effHist_alt_[i_pt];
      TString histName = TString::Format("h_absDiff_pt%.0lfto%.0lf", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
      TH1D* h_absDiff = PlotTool::HistOperation(histName, h_alt, h_cen, "-");

      TString legend = TString::Format("%.0lf < p_{T} < %.0lf GeV", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1]);
      canvas->Register( h_absDiff, legend, vec_color[i_pt]);

      // -- print values
      for(Int_t i=0; i<h_absDiff->GetNbinsX(); i++)
      {
        // Double_t eta, absDiff;
        // g_absDiff->GetPoint(i, eta, absDiff);

        Int_t i_bin = i+1;
        Double_t absDiff = h_absDiff->GetBinContent(i_bin);
        printf("%lf\t", absDiff);

        if( absDiff >= 0 ) entry_plus++;
        if( absDiff < 0  ) entry_minus++;
      }
      cout << endl;
    }
    printf("(entry_plus, entry_minus) = (%d, %d)\n", entry_plus, entry_minus);

    // canvas->SetRangeY(-0.1, 0.1);
    canvas->SetRangeY(-0.03, 0.03);
    canvas->SetLegendPosition(0.60, 0.18, 0.95, 0.33);
    canvas->Latex_CMSPre();

    TString typeLatex = "";
    if( effType_ == "RecoID")   typeLatex = "Reco.+ID efficiency";
    if( effType_ == "Iso")      typeLatex = "Iso. efficiency";
    if( effType_ == "HLTv4p2")  typeLatex = "Trigger efficiency (HLT v4.2)";
    if( effType_ == "HLTv4p3")  typeLatex = "Trigger efficiency (HLT v4.3)";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+typeLatex+"}}");

    TString uncInfo = "";
    if( uncType_ == "sgnChange" ) uncInfo = TString::Format("#varepsilon_{alt.} = %s efficiency from alternative signal shape", sampleType_.Data());
    if( uncType_ == "bkgChange" ) uncInfo = TString::Format("#varepsilon_{alt.} = %s efficiency from alternative background shape", sampleType_.Data());
    if( uncType_ == "M60to130"  ) uncInfo = TString::Format("#varepsilon_{alt.} = %s efficiency from alternative mass rantge (60-130)", sampleType_.Data());
    if( uncType_ == "M70to120"  ) uncInfo = TString::Format("#varepsilon_{alt.} = %s efficiency from alternative mass rantge (70-120)", sampleType_.Data());
    if( uncType_ == "nBin30"  )   uncInfo = TString::Format("#varepsilon_{alt.} = %s efficiency from alternative # mass bin (30)", sampleType_.Data());
    if( uncType_ == "nBin50"  )   uncInfo = TString::Format("#varepsilon_{alt.} = %s efficiency from alternative # mass bin (50)", sampleType_.Data());
    if( uncType_ == "TagPt20"  )  uncInfo = TString::Format("#varepsilon_{alt.} = %s efficiency from alternative tag p_{T} criteria (20 GeV)", sampleType_.Data());
    if( uncType_ == "TagPt24"  )  uncInfo = TString::Format("#varepsilon_{alt.} = %s efficiency from alternative tag p_{T} criteria (24 GeV)", sampleType_.Data());

    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+uncInfo+"}}");

    canvas->Draw("LP");

    cout << "[Draw_AbsDiff] Done" << endl;
    cout << endl;
  }
  void Init()
  {
    TString histName = TString::Format("h_2D_Eff_%s_%s", effType_.Data(), sampleType_.Data());
    h2D_eff_cen_ = PlotTool::Get_Hist2D(fileName_cen_, histName);
    h2D_eff_alt_ = PlotTool::Get_Hist2D(fileName_alt_, histName);

    FillVector_EffHist("cen", vec_effHist_cen_, h2D_eff_cen_);
    FillVector_EffHist("alt", vec_effHist_alt_, h2D_eff_alt_);
  }

  void FillVector_EffHist( TString type, vector<TH1D*>& vec_effHist, TH2D* h2D_eff )
  {
    Int_t nPtBin = (Int_t)vec_ptBinEdge_.size() - 1;
    Int_t nEtaBin = (Int_t)vec_etaBinEdge_.size() - 1;
    for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
    {
      Int_t i_ptBin = i_pt+1;

      TString histName = TString::Format("h_eff_pt%.0lfto%.0lf_%s", vec_ptBinEdge_[i_pt], vec_ptBinEdge_[i_pt+1], type.Data());
      TH1D* h_eff = MakeHist_EtaBinned(histName);

      for(Int_t i_eta=0; i_eta<nEtaBin; i_eta++)
      {
        Int_t i_etaBin = i_eta+1;

        Double_t eff     = h2D_eff->GetBinContent(i_etaBin, i_ptBin);
        Double_t eff_err = h2D_eff->GetBinError(i_etaBin, i_ptBin);
        h_eff->SetBinContent(i_etaBin, eff);
        h_eff->SetBinError(i_etaBin, eff_err);
      }

      vec_effHist.push_back( h_eff );
    }
  }

  TH1D* MakeHist_EtaBinned(TString histName)
  {
    Int_t nBin = (Int_t)vec_etaBinEdge_.size() - 1;
    Double_t* arr_binEdge = new Double_t[nBin+1];
    for(Int_t i=0; i<nBin+1; i++)
      arr_binEdge[i] = vec_etaBinEdge_[i];

    TH1D* h_return = new TH1D(histName, "", nBin, arr_binEdge);
    delete[] arr_binEdge;

    return h_return;
  }

  // void FillVector_EffGraph(vector<TGraphAsymmErrors*> &vec_effGraph, TString fileName)
  // {
  //   TString graphName_base = TString::Format("g_Eff_%s_%s", effType_.Data(), sampleType_.Data() );

  //   for(Int_t i_absEta=0; i_absEta<nAbsEtaBin_; i_absEta++)
  //   {
  //     TString graphName = graphName_base+TString::Format("_EtaBin%d", i_absEta);
  //     // cout << "graphName: " << graphName << endl;

  //     TGraphAsymmErrors* g_temp = PlotTool::Get_Graph(fileName, graphName);
  //     vec_effGraph.push_back( g_temp );
  //   }
  // }
};


void DrawPlots_Muon()
{
  TString effType = "RecoID";
  vector<TString> vec_sampleType = {"Data", "MC"};
  vector<TString> vec_uncType = {"sgnChange", "bkgChange", "M60to130", "M70to120", "nBin30", "nBin50", "TagPt20", "TagPt24"};
  TString fileName_cen = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/Include/ROOTFile_TagProbeEfficiency_76X_v20160502.root";
  TString fileName_alt_base = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly/TnPEffs/Inputs";
  TString fileName_alt = "";

  for(const auto& uncType : vec_uncType )
  {
    fileName_alt = fileName_alt_base+"/ROOTFile_ExtractEfficiency_CalcSysUnc_2D_"+uncType+".root";

    for(const auto& sampleType : vec_sampleType )
    {
      PlotProducer* producer = new PlotProducer(effType, sampleType, uncType, fileName_cen, fileName_alt);
      producer->Produce();
    }
  }
}