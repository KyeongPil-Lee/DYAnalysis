#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>

#define nMassRange 6

class Expected2DUncTool
{
public:
  TString leptonType_;

  TString massRangeInfo_[nMassRange];
  TH1D* h_relUnc_stat_[nMassRange];
  TH1D* h_relUnc_effSF_[nMassRange];
  TH1D* h_relUnc_detRes_[nMassRange];
  TH1D* h_relUnc_bkg_[nMassRange];
  TH1D* h_relUnc_FSR_[nMassRange];
  TH1D* h_relUnc_lumi_[nMassRange];
  TH1D* h_relUnc_syst_[nMassRange];
  TH1D* h_relUnc_collCS_[nMassRange]; // -- for electron channel only

  TH1D* h_relUnc_2012_stat_[nMassRange];
  TH1D* h_relUnc_2012_syst_[nMassRange];

  Double_t lumi2012_;
  Double_t targetLumi_;
  Double_t lumiScale_;

  TString fileName_stat_;

  Expected2DUncTool(TString leptonType)
  {
    leptonType_ = leptonType;

    massRangeInfo_[0] = "M20to30";
    massRangeInfo_[1] = "M30to45";
    massRangeInfo_[2] = "M45to60";
    massRangeInfo_[3] = "M60to120";
    massRangeInfo_[4] = "M120to200";
    massRangeInfo_[5] = "M200to1500";

    lumi2012_ = 19700.000;
    targetLumi_ = 0.0;
    lumiScale_ = 0.0;
  }

  void SetLumi(Double_t value )
  {
    targetLumi_ = value;
    lumiScale_ = sqrt( lumi2012_ / targetLumi_ );
  }

  void SetFileName_StatUnc( TString value )
  {
    fileName_stat_ = value;
  }

  void DrawPlot()
  {
    SetAll();

    for(Int_t i_mr=0; i_mr<nMassRange; i_mr++)
      DrawSummaryPlot(i_mr);
  }

  void Save(TString fileName)
  {
    TFile *f_output = TFile::Open(fileName, "RECREATE"); f_output->cd();

    for(Int_t i_mr=0; i_mr<nMassRange; i_mr++)
    {
      h_relUnc_stat_[i_mr]->SetName("h_relUnc_stat_"+massRangeInfo_[i_mr]);
      h_relUnc_stat_[i_mr]->Write();

      h_relUnc_effSF_[i_mr]->SetName("h_relUnc_effSF_"+massRangeInfo_[i_mr]);
      h_relUnc_effSF_[i_mr]->Write();

      h_relUnc_detRes_[i_mr]->SetName("h_relUnc_detRes_"+massRangeInfo_[i_mr]);
      h_relUnc_detRes_[i_mr]->Write();

      h_relUnc_bkg_[i_mr]->SetName("h_relUnc_bkg_"+massRangeInfo_[i_mr]);
      h_relUnc_bkg_[i_mr]->Write();

      h_relUnc_FSR_[i_mr]->SetName("h_relUnc_FSR_"+massRangeInfo_[i_mr]);
      h_relUnc_FSR_[i_mr]->Write();

      h_relUnc_lumi_[i_mr]->SetName("h_relUnc_lumi_"+massRangeInfo_[i_mr]);
      h_relUnc_lumi_[i_mr]->Write();

      h_relUnc_syst_[i_mr]->SetName("h_relUnc_syst_"+massRangeInfo_[i_mr]);
      h_relUnc_syst_[i_mr]->Write();

      if( leptonType_ == "Electron" )
      {
        h_relUnc_collCS_[i_mr]->SetName("h_relUnc_collCS_"+massRangeInfo_[i_mr]);
        h_relUnc_collCS_[i_mr]->Write();
      }

      h_relUnc_2012_stat_[i_mr]->SetName("h_relUnc_2012_stat_"+massRangeInfo_[i_mr]);
      h_relUnc_2012_stat_[i_mr]->Write();

      h_relUnc_2012_syst_[i_mr]->SetName("h_relUnc_2012_syst_"+massRangeInfo_[i_mr]);
      h_relUnc_2012_syst_[i_mr]->Write();
    }

    printf("[Expected2DUncTool::Save] All histograms are saved in %s\n", fileName.Data());

    f_output->Close();
  }
private:
  void SetAll()
  {
    SetUncHist_Stat();
    SetUncHist_Lumi(); // -- need to be called after stat. unc. histograms are set

    SetUncHist_SystSource_M20to30();
    SetUncHist_SystSource_M30to45();
    SetUncHist_SystSource_M45to60();
    SetUncHist_SystSource_M60to120();
    SetUncHist_SystSource_M120to200();
    SetUncHist_SystSource_M200to1500();
  }

  void DrawSummaryPlot(Int_t i_mr)
  {
    TString canvasName = TString::Format("c_ExpectedUnc_2D_%s_%.1lf_%s", leptonType_.Data(), targetLumi_ /1000.0, massRangeInfo_[i_mr].Data() );
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 1);
    canvas->SetCanvasSize(800, 700);
    canvas->ForceSameMarkerStyle(kFALSE);

    h_relUnc_stat_[i_mr]->SetMarkerStyle(20);
    canvas->Register(h_relUnc_stat_[i_mr], "Statistical", kBlack);

    h_relUnc_syst_[i_mr]->SetMarkerStyle(kFullSquare);
    canvas->Register(h_relUnc_syst_[i_mr], "Total systematic", kRed);

    h_relUnc_effSF_[i_mr]->SetMarkerStyle(25);
    canvas->Register(h_relUnc_effSF_[i_mr], "Efficiency SF", TColor::GetColor("#0000ff"));

    h_relUnc_detRes_[i_mr]->SetMarkerStyle(26);
    canvas->Register(h_relUnc_detRes_[i_mr], "Detector resolution", TColor::GetColor("#cc00ff"));

    h_relUnc_bkg_[i_mr]->SetMarkerStyle(27);
    h_relUnc_bkg_[i_mr]->SetMinimum(0.001);
    canvas->Register(h_relUnc_bkg_[i_mr], "Background", TColor::GetColor("#00cc00"));

    h_relUnc_FSR_[i_mr]->SetMarkerStyle(28);
    canvas->Register(h_relUnc_FSR_[i_mr], "FSR", TColor::GetColor("#9999ff"));

    if( leptonType_ == "Electron" )
    {
      h_relUnc_collCS_[i_mr]->SetMarkerStyle(34);
      canvas->Register(h_relUnc_collCS_[i_mr], "Coll.CS", 7);
    }

    h_relUnc_lumi_[i_mr]->SetMarkerStyle(22);
    canvas->Register(h_relUnc_lumi_[i_mr], "Luminosity", TColor::GetColor("#ff9933"));

    canvas->SetTitle( "|y(ll)|", "Rel. Unc. (%)");

    canvas->SetRangeY(2e-2, 30);
    if( i_mr == 3 ) canvas->SetRangeY(1e-4, 10);

    canvas->SetLegendPosition( 0.15, 0.15, 0.965, 0.35 );
    canvas->SetLegendColumn(4);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.78, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", targetLumi_ /1000.0));

    TString info = "";
    if( leptonType_ == "Muon" )     info = "Expected uncertainties (#mu channel)";
    if( leptonType_ == "Electron" ) info = "Expected uncertainties (e channel)";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+info+"}}");

    TString massRangeLatex = "";
    if( massRangeInfo_[i_mr] == "M20to30" )    massRangeLatex = "20 < M < 30 GeV";
    if( massRangeInfo_[i_mr] == "M30to45" )    massRangeLatex = "30 < M < 45 GeV";
    if( massRangeInfo_[i_mr] == "M45to60" )    massRangeLatex = "45 < M < 60 GeV";
    if( massRangeInfo_[i_mr] == "M60to120" )   massRangeLatex = "60 < M < 120 GeV";
    if( massRangeInfo_[i_mr] == "M120to200" )  massRangeLatex = "120 < M < 200 GeV";
    if( massRangeInfo_[i_mr] == "M200to1500" ) massRangeLatex = "200 < M < 1500 GeV";
    canvas->RegisterLatex( 0.16, 0.87, "#scale[0.6]{#font[42]{"+massRangeLatex+"}}");

    canvas->Draw("HISTLPSAME");
  }

  void SetUncHist_Stat()
  {
    for(Int_t i=0; i<nMassRange; i++)
    {
      TString histName = TString::Format("h_RelStatUnc_absdiRap_%s_DY", massRangeInfo_[i].Data());
      TH1D* h = PlotTool::Get_Hist( fileName_stat_, histName );
      h_relUnc_stat_[i] = (TH1D*)h->Clone();
      h_relUnc_stat_[i]->Scale(100); // -- convert to % -- //

      // -- 2012 uncertainty: no reference.
      // -- just scale up using the luminosity raito
      h_relUnc_2012_stat_[i] = (TH1D*)h_relUnc_stat_[i]->Clone();
      h_relUnc_2012_stat_[i]->Scale( 1.0 / lumiScale_ );
    }
  }

  void SetUncHist_Lumi()
  {
    Double_t relUnc_lumi = 2.5;

    for(Int_t i_mr=0; i_mr<nMassRange; i_mr++)
    {
      h_relUnc_lumi_[i_mr] = (TH1D*)h_relUnc_stat_[i_mr]->Clone();
      Int_t nBin = h_relUnc_lumi_[i_mr]->GetNbinsX();
      for(Int_t i=0; i<nBin; i++)
      {
        Int_t i_bin = i+1;
        h_relUnc_lumi_[i_mr]->SetBinContent(i_bin, relUnc_lumi);
        h_relUnc_lumi_[i_mr]->SetBinError(i_bin, 0);
      }
    }
  }

  void SetUncHist_SystSource_M20to30()
  {
    if( leptonType_ == "Muon" )     SetUncHist_SystSource_M20to30_Muon();
    if( leptonType_ == "Electron" ) SetUncHist_SystSource_M20to30_Electron();
  }

  void SetUncHist_SystSource_M30to45()
  {
    if( leptonType_ == "Muon" )     SetUncHist_SystSource_M30to45_Muon();
    if( leptonType_ == "Electron" ) SetUncHist_SystSource_M30to45_Electron();
  }

  void SetUncHist_SystSource_M45to60()
  {
    if( leptonType_ == "Muon" )     SetUncHist_SystSource_M45to60_Muon();
    if( leptonType_ == "Electron" ) SetUncHist_SystSource_M45to60_Electron();
  }

  void SetUncHist_SystSource_M60to120()
  {
    if( leptonType_ == "Muon" )     SetUncHist_SystSource_M60to120_Muon();
    if( leptonType_ == "Electron" ) SetUncHist_SystSource_M60to120_Electron();
  }

  void SetUncHist_SystSource_M120to200()
  {
    if( leptonType_ == "Muon" )     SetUncHist_SystSource_M120to200_Muon();
    if( leptonType_ == "Electron" ) SetUncHist_SystSource_M120to200_Electron();
  }

  void SetUncHist_SystSource_M200to1500()
  {
    if( leptonType_ == "Muon" )     SetUncHist_SystSource_M200to1500_Muon();
    if( leptonType_ == "Electron" ) SetUncHist_SystSource_M200to1500_Electron();
  }

  void SetUncHist_SystSource_M20to30_Muon()
  {
    Int_t i_mr = 0; // -- first bin -- //

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      1.62, 1.65, 1.67, 1.68, 1.69, 1.67, 1.64, 1.60, 1.53, 1.46, 
      1.39, 1.33, 1.30, 1.29, 1.30, 1.33, 1.35, 1.37, 1.40, 1.44, 
      1.50, 1.60, 1.72, 1.87
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      1.45, 1.85, 1.87, 1.81, 1.78, 1.76, 2.22, 2.33, 1.57, 2.16, 
      2.09, 2.10, 1.83, 2.24, 2.20, 3.13, 2.76, 2.56, 2.35, 2.67, 
      5.61, 5.01, 6.25, 3.94 
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      2.26, 2.48, 2.39, 2.36, 2.36, 2.44, 2.34, 2.37, 2.50, 2.48, 
      2.40, 2.40, 2.35, 2.24, 2.28, 2.27, 2.16, 2.12, 2.11, 2.27, 
      2.21, 2.29, 2.61, 3.87
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.46, 0.45, 0.43, 0.41, 0.39, 0.37, 0.35, 0.33, 0.31, 0.30,
      0.29, 0.27, 0.25, 0.23, 0.22, 0.22, 0.24, 0.29, 0.37, 0.48,
      0.59, 0.71, 0.83, 0.95
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR );
  }

  void SetUncHist_SystSource_M30to45_Muon()
  {
    Int_t i_mr = 1;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      1.11, 1.12, 1.17, 1.22, 1.26, 1.28, 1.29, 1.27, 1.24, 1.18, 
      1.11, 1.06, 1.02, 1.01, 1.02, 1.05, 1.07, 1.08, 1.08, 1.08, 
      1.09, 1.13, 1.18, 1.25
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.43, 0.51, 0.50, 0.52, 0.52, 0.53, 0.51, 0.54, 0.52, 0.58, 
      0.57, 0.62, 0.64, 0.69, 0.68, 0.70, 0.73, 0.73, 0.70, 0.74, 
      0.79, 0.86, 1.05, 1.91

    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      1.44, 1.43, 1.44, 1.43, 1.43, 1.45, 1.45, 1.46, 1.49, 1.50, 
      1.51, 1.52, 1.53, 1.53, 1.58, 1.62, 1.63, 1.66, 1.68, 1.71,
      1.70, 1.74, 1.85, 0.93
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.49, 0.52, 0.53, 0.53, 0.51, 0.47, 0.43, 0.38, 0.34, 0.31, 
      0.28, 0.26, 0.25, 0.26, 0.29, 0.34, 0.42, 0.53, 0.66, 0.80,
      0.93, 1.05, 1.16, 1.26
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR );
  }

  void SetUncHist_SystSource_M45to60_Muon()
  {
    Int_t i_mr = 2;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      1.13, 1.00, 0.89, 0.79, 0.71, 0.65, 0.62, 0.61, 0.62, 0.64, 
      0.67, 0.69, 0.72, 0.74, 0.75, 0.76, 0.76, 0.75, 0.74, 0.72, 
      0.70, 0.68, 0.67, 0.67
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.81, 0.93, 0.91, 1.00, 0.96, 0.97, 0.96, 1.01, 1.04, 1.05, 
      1.03, 1.04, 1.13, 1.14, 1.13, 1.19, 1.19, 1.29, 1.37, 1.34, 
      1.55, 1.88, 2.32, 3.74
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      1.34, 1.31, 1.29, 1.31, 1.30, 1.32, 1.32, 1.33, 1.32, 1.32, 
      1.28, 1.24, 1.19, 1.09, 1.01, 0.93, 0.86, 0.80, 0.75, 0.79, 
      0.80, 0.87, 1.01, 1.40

    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.48, 0.73, 0.93, 1.04, 1.04, 0.96, 0.83, 0.70, 0.60, 0.54, 
      0.52, 0.53, 0.55, 0.60, 0.71, 0.91, 1.21, 1.63, 2.12, 2.63, 
      3.12, 3.55, 3.90, 4.21
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR );
  }

  void SetUncHist_SystSource_M60to120_Muon()
  {
    Int_t i_mr = 3;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      0.72, 0.68, 0.63, 0.58, 0.53, 0.49, 0.44, 0.40, 0.38, 0.36, 
      0.36, 0.38, 0.40, 0.44, 0.48, 0.53, 0.58, 0.64, 0.68, 0.73, 
      0.77, 0.80, 0.84, 0.87
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.06, 0.09, 0.09, 0.09, 0.10, 0.10, 0.09, 0.10, 0.10, 0.10, 
      0.10, 0.10, 0.10, 0.11, 0.11, 0.12, 0.12, 0.11, 0.11, 0.12, 
      0.12, 0.12, 0.14, 0.22
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 
      0.04, 0.04, 0.04, 0.04, 0.04, 0.05, 0.05, 0.06, 0.06, 0.07, 
      0.08, 0.09, 0.11, 0.06
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.00, 0.01, 0.02, 0.02, 0.02, 0.02, 0.01, 0.02, 0.02, 0.02,
      0.03, 0.05, 0.06, 0.08, 0.10, 0.13, 0.16, 0.20, 0.23, 0.27,
      0.31, 0.35, 0.39, 0.43
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR );
  }

  void SetUncHist_SystSource_M120to200_Muon()
  {
    Int_t i_mr = 4;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      0.43, 0.41, 0.38, 0.38, 0.40, 0.42, 0.44, 0.47, 0.50, 0.51, 
      0.52, 0.52, 0.52, 0.54, 0.58, 0.65, 0.75, 0.85, 0.92, 0.98, 
      1.04, 1.32, 1.72, 1.85
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.86, 1.06, 1.06, 1.02, 1.05, 1.05, 1.07, 1.12, 1.13, 1.11, 
      1.20, 1.20, 1.17, 1.22, 1.35, 1.27, 1.44, 1.55, 1.54, 1.61, 
      1.77, 2.25, 2.57, 4.69
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      1.25, 1.40, 1.47, 1.52, 1.49, 1.47, 1.39, 1.33, 1.29, 1.23, 
      1.17, 1.13, 1.08, 1.06, 0.98, 0.96, 0.92, 0.88, 0.86, 0.84, 
      0.84, 0.94, 1.14, 1.03
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.30, 0.36, 0.40, 0.41, 0.38, 0.33, 0.27, 0.21, 0.17, 0.15, 
      0.14, 0.13, 0.13, 0.14, 0.15, 0.17, 0.21, 0.25, 0.29, 0.33, 
      0.36, 0.39, 0.40, 0.40
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR );
  }

  void SetUncHist_SystSource_M200to1500_Muon()
  {
    Int_t i_mr = 5;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      1.70, 1.66, 1.65, 1.64, 1.67, 1.79, 1.93, 2.19, 2.69, 3.38, 
      4.38, 7.56
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.66, 0.80, 0.72, 0.78, 0.82, 0.85, 0.96, 1.10, 1.19, 1.62,
      2.19, 5.25
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      4.82, 4.38, 3.88, 3.25, 2.72, 2.39, 2.06, 1.73, 1.48, 1.54,
      1.77, 1.22
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.21, 0.18, 0.17, 0.17, 0.19, 0.23, 0.29, 0.36, 0.43, 0.50,
      0.57, 0.64
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR );
  }


  // -- electron
  void SetUncHist_SystSource_M20to30_Electron()
  {
    Int_t i_mr = 0; // -- first bin -- //

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      6.23, 6.02, 5.72, 5.65, 5.08, 4.99, 4.64, 4.60, 4.41, 4.26,
      4.05, 4.32, 4.52, 4.70, 4.85, 4.97, 4.68, 4.85, 4.83, 5.24,
      5.76, 6.36, 7.87, 14.32
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      1.21, 1.57, 1.55, 1.52, 1.49, 1.55, 1.68, 1.71, 1.47, 1.83, 
      1.75, 1.94, 1.94, 2.19, 2.50, 2.61, 2.57, 2.66, 2.81, 2.79,
      3.83, 4.03, 4.99, 9.41
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      2.74, 3.20, 3.38, 3.35, 2.91, 2.89, 3.45, 3.22, 2.98, 3.12,
      3.24, 2.64, 2.75, 2.38, 2.25, 2.10, 2.31, 2.75, 2.03, 1.61,
      1.54, 1.41, 3.76, 3.07
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.72, 0.75, 0.78, 0.75, 0.74, 0.74, 0.75, 0.79, 0.83, 0.76,
      0.75, 0.70, 0.80, 0.71, 0.75, 0.77, 0.77, 0.72, 0.84, 0.77,
      0.80, 0.92, 1.07, 1.68
    };

    // -- pileup effect -- //
    vector<Double_t> vec_relUnc8TeV_collCS =
    {
      2.03, 1.72, 2.11, 1.52, 2.22, 1.06, 2.06, 2.37, 1.55, 1.41,
      1.63, 1.64, 1.19, 1.63, 2.01, 1.77, 1.84, 2.21, 2.30, 2.13,
      2.76, 2.42, 3.25, 1.17
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR, vec_relUnc8TeV_collCS );
  }

  void SetUncHist_SystSource_M30to45_Electron()
  {
    Int_t i_mr = 1;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      5.97, 5.91, 5.71, 5.34, 5.03, 4.63, 4.19, 3.90, 3.69, 3.75, 
      3.86, 4.01, 4.04, 4.09, 4.18, 4.16, 4.19, 4.17, 4.03, 3.97,
      4.14, 4.66, 5.52, 8.98
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.57, 0.64, 0.62, 0.63, 0.64, 0.65, 0.63, 0.70, 0.68, 0.76,
      0.76, 0.86, 0.91, 1.02, 1.09, 1.08, 1.13, 1.19, 1.19, 1.23,
      1.32, 1.46, 1.77, 2.83
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      1.20, 1.11, 1.04, 1.08, 1.18, 1.11, 1.07, 1.16, 1.22, 1.21, 
      1.12, 1.26, 1.23, 1.21, 1.24, 1.19, 1.25, 1.15, 1.23, 1.21,
      1.16, 1.30, 1.59, 2.69
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.55, 0.53, 0.55, 0.54, 0.55, 0.54, 0.51, 0.52, 0.52, 0.52,
      0.52, 0.54, 0.51, 0.50, 0.57, 0.55, 0.53, 0.54, 0.49, 0.57,
      0.53, 0.51, 0.74, 1.02
    };

    // -- pileup effect -- //
    vector<Double_t> vec_relUnc8TeV_collCS =
    {
      2.22, 2.34, 2.14, 2.12, 2.00, 2.18, 2.21, 1.84, 1.95, 1.96,
      1.94, 1.64, 1.72, 1.92, 1.75, 2.31, 2.04, 2.04, 2.20, 2.61,
      3.06, 2.69, 4.30, 5.22
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR, vec_relUnc8TeV_collCS );
  }

  void SetUncHist_SystSource_M45to60_Electron()
  {
    Int_t i_mr = 2;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      3.54, 3.42, 3.30, 3.16, 2.94, 2.87, 2.76, 2.74, 2.80, 2.79,
      2.85, 2.93, 2.93, 2.93, 2.97, 3.09, 3.30, 3.41, 3.47, 3.57,
      4.25, 4.87, 6.11, 8.67

    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.89, 0.95, 0.90, 1.01, 0.98, 1.04, 1.00, 1.04, 1.13, 1.17,
      1.12, 1.21, 1.24, 1.30, 1.32, 1.30, 1.43, 1.58, 1.67, 1.73,
      1.91, 2.39, 3.09, 4.58
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      1.54, 1.54, 1.48, 1.59, 1.57, 1.62, 1.66, 1.64, 1.76, 1.63,
      1.62, 1.57, 1.65, 1.61, 1.34, 1.31, 1.39, 1.38, 1.38, 1.76,
      1.59, 1.94, 2.00, 3.01
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.64, 0.64, 0.65, 0.70, 0.68, 0.69, 0.66, 0.69, 0.70, 0.68,
      0.68, 0.68, 0.65, 0.65, 0.69, 0.73, 0.72, 0.76, 0.81, 0.88,
      1.01, 1.25, 1.53, 2.01
    };

    // -- pileup effect -- //
    vector<Double_t> vec_relUnc8TeV_collCS =
    {
      1.47, 1.51, 1.55, 1.41, 1.66, 1.54, 1.28, 1.51, 1.16, 1.67,
      1.28, 1.45, 1.43, 1.40, 1.84, 1.29, 1.66, 2.06, 1.72, 1.84,
      2.42, 2.04, 2.17, 3.04
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR, vec_relUnc8TeV_collCS );
  }

  void SetUncHist_SystSource_M60to120_Electron()
  {
    Int_t i_mr = 3;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      0.82, 0.78, 0.72, 0.67, 0.66, 0.66, 0.66, 0.66, 0.66, 0.67, 
      0.65, 0.60, 0.56, 0.52, 0.51, 0.52, 0.54, 0.62, 0.63, 0.72,
      1.07, 1.23, 1.37, 1.63
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.06, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.10, 0.10, 0.10, 
      0.10, 0.10, 0.11, 0.11, 0.11, 0.12, 0.12, 0.13, 0.13, 0.14,
      0.14, 0.15, 0.17, 0.26
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      0.09, 0.08, 0.09, 0.09, 0.09, 0.09, 0.08, 0.08, 0.08, 0.08,
      0.08, 0.08, 0.07, 0.07, 0.08, 0.09, 0.09, 0.10, 0.10, 0.12,
      0.12, 0.15, 0.20, 0.29

    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.29, 0.30, 0.30, 0.30, 0.30, 0.30, 0.31, 0.31, 0.32, 0.31,
      0.31, 0.31, 0.31, 0.31, 0.32, 0.33, 0.33, 0.32, 0.32, 0.32,
      0.35, 0.32, 0.36, 0.39 
    };

    // -- pileup effect -- //
    vector<Double_t> vec_relUnc8TeV_collCS =
    {
      0.44, 0.50, 0.50, 0.50, 0.51, 0.54, 0.55, 0.57, 0.53, 0.59,
      0.58, 0.60, 0.59, 0.55, 0.54, 0.54, 0.60, 0.55, 0.56, 0.56,
      0.70, 0.79, 0.84, 0.99
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR, vec_relUnc8TeV_collCS );
  }

  void SetUncHist_SystSource_M120to200_Electron()
  {
    Int_t i_mr = 4;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      1.26, 1.26, 1.20, 1.20, 1.20, 1.21, 1.19, 1.24, 1.21, 1.20, 
      1.25, 1.26, 1.32, 1.49, 1.56, 1.67, 1.83, 2.03, 2.31, 2.61,
      3.12, 3.74, 5.02, 10.39
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.78, 0.92, 0.92, 0.93, 0.89, 0.93, 0.97, 0.96, 0.95, 0.97,
      1.05, 1.01, 1.08, 1.11, 1.23, 1.34, 1.47, 1.56, 1.62, 1.73,
      2.20, 2.75, 4.95
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      1.66, 1.62, 1.58, 1.57, 1.49, 1.49, 1.49, 1.51, 1.46, 1.28,
      1.25, 1.27, 1.11, 1.08, 0.95, 1.10, 1.23, 1.11, 1.34, 1.44,
      1.76, 1.91, 4.02
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.64, 0.76, 0.70, 0.78, 0.77, 0.79, 0.79, 0.78, 0.75, 0.78,
      0.80, 0.81, 0.79, 0.80, 0.82, 0.87, 0.90, 0.95, 1.00, 1.08,
      1.28, 1.40, 1.73, 3.12
    };

    // -- pileup effect -- //
    vector<Double_t> vec_relUnc8TeV_collCS =
    {
      0.14, 0.20, 0.37, 0.33, 0.11, 0.07, 0.24, 0.23, 0.23, 0.07,
      0.33, 0.27, 0.07, 0.44, 0.37, 0.56, 0.17, 0.33, 0.18, 0.76,
      0.47, 0.77, 0.73, 1.38
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR, vec_relUnc8TeV_collCS );
  }

  void SetUncHist_SystSource_M200to1500_Electron()
  {
    Int_t i_mr = 5;

    vector<Double_t> vec_relUnc8TeV_effSF = 
    {
      1.11, 1.10, 1.12, 1.08, 1.12, 1.15, 1.27, 1.42, 1.76, 2.36,
      3.42, 5.95
    };

    vector<Double_t> vec_relUnc8TeV_detRes = 
    {
      0.62, 0.73, 0.65, 0.73, 0.74, 0.79, 0.89, 1.02, 1.13, 1.51,
      2.11, 4.15
    };

    vector<Double_t> vec_relUnc8TeV_bkg =
    {
      4.49, 4.57, 4.31, 3.42, 3.34, 3.14, 2.67, 2.31, 2.47, 2.92,
      3.74, 7.64
    };

    vector<Double_t> vec_relUnc8TeV_FSR =
    {
      0.57, 0.62, 0.60, 0.63, 0.64, 0.70, 0.67, 0.72, 0.83, 1.00,
      1.26, 2.37
    };

    // -- pileup effect -- //
    vector<Double_t> vec_relUnc8TeV_collCS =
    {
      0.22, 0.04, 0.16, 0.09, 0.06, 0.20, 0.17, 0.16, 0.36, 0.20,
      0.67, 1.76
    };

    SetUncHist_MassRange( i_mr, vec_relUnc8TeV_effSF, vec_relUnc8TeV_detRes, vec_relUnc8TeV_bkg, vec_relUnc8TeV_FSR, vec_relUnc8TeV_collCS );
  }

  void SetUncHist_MassRange( Int_t i_mr, vector<Double_t> vec_relUnc8TeV_effSF, vector<Double_t> vec_relUnc8TeV_detRes, vector<Double_t> vec_relUnc8TeV_bkg, vector<Double_t> vec_relUnc8TeV_FSR )
  {
    h_relUnc_effSF_[i_mr]  = MakeHist_FromVector( vec_relUnc8TeV_effSF );
    h_relUnc_detRes_[i_mr] = MakeHist_FromVector( vec_relUnc8TeV_detRes );
    h_relUnc_bkg_[i_mr]    = MakeHist_FromVector( vec_relUnc8TeV_bkg );
    h_relUnc_FSR_[i_mr]    = MakeHist_FromVector( vec_relUnc8TeV_FSR );

    // -- total systematic: for 2012 uncertainty
    vector< TH1D* > vec_histsForQuadSum_2012;
    vec_histsForQuadSum_2012.push_back( h_relUnc_effSF_[i_mr] );
    vec_histsForQuadSum_2012.push_back( h_relUnc_detRes_[i_mr] );
    vec_histsForQuadSum_2012.push_back( h_relUnc_bkg_[i_mr] );
    vec_histsForQuadSum_2012.push_back( h_relUnc_FSR_[i_mr] );
    h_relUnc_2012_syst_[i_mr] = MakeHist_byQuadSum(vec_histsForQuadSum_2012);

    // -- scale down the uncertainties corrleated with the statistics
    h_relUnc_detRes_[i_mr]->Scale( lumiScale_ );
    h_relUnc_bkg_[i_mr]->Scale( lumiScale_ );

    // -- expected total systematic uncertainty w.r.t. target lumi.
    vector< TH1D* > vec_histsForQuadSum;
    vec_histsForQuadSum.push_back( h_relUnc_effSF_[i_mr] );
    vec_histsForQuadSum.push_back( h_relUnc_detRes_[i_mr] );
    vec_histsForQuadSum.push_back( h_relUnc_bkg_[i_mr] );
    vec_histsForQuadSum.push_back( h_relUnc_FSR_[i_mr] );
    h_relUnc_syst_[i_mr] = MakeHist_byQuadSum(vec_histsForQuadSum);
  }

  // -- one more histogram: for electron channel
  void SetUncHist_MassRange( Int_t i_mr, vector<Double_t> vec_relUnc8TeV_effSF, vector<Double_t> vec_relUnc8TeV_detRes, vector<Double_t> vec_relUnc8TeV_bkg, vector<Double_t> vec_relUnc8TeV_FSR, vector<Double_t> vec_relUnc8TeV_collCS )
  {
    h_relUnc_effSF_[i_mr]  = MakeHist_FromVector( vec_relUnc8TeV_effSF );
    h_relUnc_detRes_[i_mr] = MakeHist_FromVector( vec_relUnc8TeV_detRes );
    h_relUnc_bkg_[i_mr]    = MakeHist_FromVector( vec_relUnc8TeV_bkg );
    h_relUnc_FSR_[i_mr]    = MakeHist_FromVector( vec_relUnc8TeV_FSR );
    h_relUnc_collCS_[i_mr] = MakeHist_FromVector( vec_relUnc8TeV_collCS );

    // -- for 2012 uncertainty
    vector< TH1D* > vec_histsForQuadSum_2012;
    vec_histsForQuadSum_2012.push_back( h_relUnc_effSF_[i_mr] );
    vec_histsForQuadSum_2012.push_back( h_relUnc_detRes_[i_mr] );
    vec_histsForQuadSum_2012.push_back( h_relUnc_bkg_[i_mr] );
    vec_histsForQuadSum_2012.push_back( h_relUnc_FSR_[i_mr] );
    vec_histsForQuadSum_2012.push_back( h_relUnc_collCS_[i_mr] );
    h_relUnc_2012_syst_[i_mr] = MakeHist_byQuadSum(vec_histsForQuadSum_2012);

    // -- scale down the uncertainties corrleated with the statistics
    h_relUnc_detRes_[i_mr]->Scale( lumiScale_ );
    h_relUnc_bkg_[i_mr]->Scale( lumiScale_ );

    // -- expected systematic uncertainty w.r.t. target lumi.
    vector< TH1D* > vec_histsForQuadSum;
    vec_histsForQuadSum.push_back( h_relUnc_effSF_[i_mr] );
    vec_histsForQuadSum.push_back( h_relUnc_detRes_[i_mr] );
    vec_histsForQuadSum.push_back( h_relUnc_bkg_[i_mr] );
    vec_histsForQuadSum.push_back( h_relUnc_FSR_[i_mr] );
    vec_histsForQuadSum.push_back( h_relUnc_collCS_[i_mr] );
    h_relUnc_syst_[i_mr] = MakeHist_byQuadSum(vec_histsForQuadSum);
  }

  TH1D* MakeHist_byQuadSum(vector<TH1D*> vec_hist)
  {
    TH1D* h_return = (TH1D*)vec_hist[0]->Clone();

    Int_t nBin = h_return->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t quadSum = 0;
      for(const auto& h : vec_hist )
      {
        Double_t binContent = h->GetBinContent(i_bin);
        quadSum = quadSum + binContent*binContent;
      }

      quadSum = sqrt(quadSum);
      h_return->SetBinContent(i_bin, quadSum);
      h_return->SetBinError(i_bin, 0);
    }

    return h_return;
  }

  TH1D* MakeHist_FromVector( vector<Double_t> vec_RelUnc )
  {
    Int_t nRapBin = vec_RelUnc.size();
    TH1D* h = new TH1D("h", "", nRapBin, 0, 2.4 );
    for(Int_t i=0; i<nRapBin; i++)
    {
      Int_t i_bin = i+1;
      h->SetBinContent(i_bin, vec_RelUnc[i]);
      h->SetBinError(i_bin, 0);
    }

    return h;
  }

};

void EstimateExpectedUnc_2D()
{
  // -- Muon channel
  Expected2DUncTool *tool_2016_Muon = new Expected2DUncTool("Muon");
  tool_2016_Muon->SetLumi( 35863.818 );
  tool_2016_Muon->SetFileName_StatUnc( "../StatUnc/ROOTFile_RelStatUnc_2016_Muon.root" );
  tool_2016_Muon->DrawPlot();
  tool_2016_Muon->Save("ROOTFile_ExpectedUnc2D_2016_Muon.root");

  Expected2DUncTool *tool_2016and17_Muon = new Expected2DUncTool("Muon");
  tool_2016and17_Muon->SetLumi( 35863.818+41526.280 );
  tool_2016and17_Muon->SetFileName_StatUnc( "../StatUnc/ROOTFile_RelStatUnc_2016and17_Muon.root" );
  tool_2016and17_Muon->DrawPlot();
  tool_2016and17_Muon->Save("ROOTFile_ExpectedUnc2D_2016and17_Muon.root");


  // -- Electron channel
  Expected2DUncTool *tool_2016_Electron = new Expected2DUncTool("Electron");
  tool_2016_Electron->SetLumi( 35863.818 );
  tool_2016_Electron->SetFileName_StatUnc( "../StatUnc/ROOTFile_RelStatUnc_2016_Electron.root" );
  tool_2016_Electron->DrawPlot();
  tool_2016_Electron->Save("ROOTFile_ExpectedUnc2D_2016_Electron.root");

  Expected2DUncTool *tool_2016and17_Electron = new Expected2DUncTool("Electron");
  tool_2016and17_Electron->SetLumi( 35863.818+41526.280 );
  tool_2016and17_Electron->SetFileName_StatUnc( "../StatUnc/ROOTFile_RelStatUnc_2016and17_Electron.root" );
  tool_2016and17_Electron->DrawPlot();
  tool_2016and17_Electron->Save("ROOTFile_ExpectedUnc2D_2016and17_Electron.root");
}