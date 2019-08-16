class Chi2Producer
{
public:
  TString fileName_;

  Chi2Producer(TString fileName)
  {
    fileName_ = fileName;
    Init();
  }

  void Produce()
  {
    MakeChi2Hist_vsEta();
    vector<TString> vec_canvasName = {
      "bin00_probe_sc_abseta_0p00To0p80_probe_sc_et_10p00To20p00_Canv",
      "bin01_probe_sc_abseta_0p80To1p44_probe_sc_et_10p00To20p00_Canv",
      "bin03_probe_sc_abseta_1p57To2p00_probe_sc_et_10p00To20p00_Canv",
      "bin04_probe_sc_abseta_2p00To2p50_probe_sc_et_10p00To20p00_Canv"
    };

    // vector<TString> vec_canvasName = {
    //   "bin10_probe_sc_eta_m2p50Tom2p00_probe_sc_et_30p00To40p00_Canv",
    //   "bin11_probe_sc_eta_m2p00Tom1p57_probe_sc_et_30p00To40p00_Canv",
    //   "bin13_probe_sc_eta_m1p44Tom0p80_probe_sc_et_30p00To40p00_Canv",
    //   "bin14_probe_sc_eta_m0p80To0p00_probe_sc_et_30p00To40p00_Canv",
    //   "bin15_probe_sc_eta_0p00To0p80_probe_sc_et_30p00To40p00_Canv",
    //   "bin16_probe_sc_eta_0p80To1p44_probe_sc_et_30p00To40p00_Canv",
    //   "bin18_probe_sc_eta_1p57To2p00_probe_sc_et_30p00To40p00_Canv",
    //   "bin19_probe_sc_eta_2p00To2p50_probe_sc_et_30p00To40p00_Canv"
    // };

    for(const auto& canvasName : vec_canvasName )
    {
      Chi2(canvasName, "pass");
      Chi2(canvasName, "fail");
    }
  }

private:
  void Init()
  {

  }

  TH1D* MakeChi2Hist_vsEta()
  {
    // const Int_t nEtaBin = "";
    // Double_t arr_etaBinEdge = {""}
    // TH1D* h_chi2 = 
    TString canvasName = "bin00_probe_sc_abseta_0p00To0p80_probe_sc_et_10p00To20p00_Canv";
    Double_t chi2_pass = Chi2(canvasName, "pass");
    Double_t chi2_fail = Chi2(canvasName, "fail");

    return nullptr;
  }

  Double_t Chi2(TString canvasName, TString type)
  {


    TFile* f_input = TFile::Open(fileName_);

    TString padName = "";
    if( type == "pass" ) padName = "c_2";
    if( type == "fail" ) padName = "c_3";
    if( padName == "" ) return -1;

    TCanvas *c = (TCanvas*)f_input->Get(canvasName)->Clone();
    TPad *pad = (TPad*)c->GetPrimitive(padName)->Clone();
    pad->cd();

    TString curveName = "";
    for(const auto&& obj: *pad->GetListOfPrimitives())
    {
      TString objName = obj->GetName();
      TString className = obj->ClassName();
      // cout << "(className, objName) = (" << className << ", " << objName << ")" << endl;

      if( className == "RooCurve" && !objName.Contains("Comp[bkg") )
      {
        curveName = objName;
        // cout << "   -> choose" << endl;
        break;
      }
    }

    TString histName = "";
    if( type == "pass" ) histName = "h_hPass";
    if( type == "fail" ) histName = "h_hFail";
    if( histName == "" ) return -1;

    RooHist* h = (RooHist*)pad->GetPrimitive(histName);
    RooCurve* curve = (RooCurve*)pad->GetPrimitive(curveName);

    Double_t chi2 = curve->chiSquare(*h, 1); // -- chi2/ndof
    cout << "(canvas, type) = (" << canvasName << "," << type << ") -> chi2/ndof = " << chi2 << endl;
    return chi2;
  }

};

void GetChi2()
{
  // TString fileName = "data_Run2015D_passingRECO.nominalFit_pt10to20.root";
  TString fileName = "data_Run2015D_passingRECO.nominalFit_pt10to20_closeToOld.root";
  // TString fileName = "data_Run2015D_passingRECO.nominalFit_otherBins.root";
  Chi2Producer* producer = new Chi2Producer(fileName);
  producer->Produce();
}