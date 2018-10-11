#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>

class ExpectedUncTool
{
public:
  TString leptonType_;

  Double_t arr_massBinEdge_[nMassBin+1];

  TH1D* h_relUnc_stat_;
  TH1D* h_relUnc_acc_;
  TH1D* h_relUnc_effSF_;
  TH1D* h_relUnc_detRes_;
  TH1D* h_relUnc_bkg_;
  TH1D* h_relUnc_FSR_;
  TH1D* h_relUnc_lumi_;
  TH1D* h_relUnc_syst_; // -- without lumi, acceptance uncertainty -- //

  TString fileName_2015_;
  TString histName_2015_stat_;
  TString histName_2015_effSF_;
  TString histName_2015_detRes_;
  TString histName_2015_bkg_;
  TString histName_2015_FSR_;
  TString histName_2015_syst_; // -- without lumi, acceptance uncertainty -- //

  TH1D* h_relUnc_2015_stat_;
  TH1D* h_relUnc_2015_effSF_;
  TH1D* h_relUnc_2015_detRes_;
  TH1D* h_relUnc_2015_bkg_;
  TH1D* h_relUnc_2015_FSR_;
  TH1D* h_relUnc_2015_syst_;

  Double_t lumi2015_;
  Double_t targetLumi_;
  Double_t lumiScale_;


  ExpectedUncTool(TString leptonType)
  {
    leptonType_ = leptonType;
    Init();
  }

  void SetLumi(Double_t value )
  {
    targetLumi_ = value;
    lumiScale_ = sqrt( lumi2015_ / targetLumi_ );
  }

  void SetStatUncHist( TH1D* h_relUnc_stat )
  {
    h_relUnc_stat_ = (TH1D*)h_relUnc_stat->Clone();
  }

  void DrawPlot()
  {
    SetHistAll();
    DrawSummaryPlot();
  }

  void Save(TString fileName)
  {
    TFile *f_output = TFile::Open(fileName, "RECREATE");
    f_output->cd();

    h_relUnc_2015_stat_->SetName("h_relUnc_2015_stat");
    h_relUnc_2015_stat_->Write();

    h_relUnc_2015_effSF_->SetName("h_relUnc_2015_effSF");
    h_relUnc_2015_effSF_->Write();

    h_relUnc_2015_detRes_->SetName("h_relUnc_2015_detRes");
    h_relUnc_2015_detRes_->Write();

    h_relUnc_2015_bkg_->SetName("h_relUnc_2015_bkg");
    h_relUnc_2015_bkg_->Write();

    h_relUnc_2015_FSR_->SetName("h_relUnc_2015_FSR");
    h_relUnc_2015_FSR_->Write();

    h_relUnc_2015_syst_->SetName("h_relUnc_2015_syst");
    h_relUnc_2015_syst_->Write();

    h_relUnc_stat_->SetName("h_relUnc_stat");
    h_relUnc_stat_->Write();

    h_relUnc_acc_->SetName("h_relUnc_acc");
    h_relUnc_acc_->Write();

    h_relUnc_effSF_->SetName("h_relUnc_effSF");
    h_relUnc_effSF_->Write();

    h_relUnc_detRes_->SetName("h_relUnc_detRes");
    h_relUnc_detRes_->Write();

    h_relUnc_bkg_->SetName("h_relUnc_bkg");
    h_relUnc_bkg_->Write();

    h_relUnc_FSR_->SetName("h_relUnc_FSR");
    h_relUnc_FSR_->Write();

    h_relUnc_lumi_->SetName("h_relUnc_lumi");
    h_relUnc_lumi_->Write();

    h_relUnc_syst_->SetName("h_relUnc_syst");
    h_relUnc_syst_->Write(); // -- without lumi, acceptance uncertainty -- //
  }


private:
  void Init()
  {
    Double_t arr_massBinEdge_temp[nMassBin+1] = 
    {
      15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
      64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
      110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
      200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
      830, 1000, 1500, 3000
    };

    for(Int_t i=0; i<nMassBin+1; i++)
      arr_massBinEdge_[i] = arr_massBinEdge_temp[i];

    if( leptonType_ == "Muon" )     lumi2015_ = 2759.017; // -- MuonPhys
    if( leptonType_ == "Electron" ) lumi2015_ = 2257.998; // -- Golden

    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

    TString histName_2015_stat = "";
    TString histName_2015_effSF = "";
    TString histName_2015_detRes = "";
    TString histName_2015_bkg = "";
    TString histName_2015_FSR = "";
    TString histName_2015_syst = ""; 
    if( leptonType_ == "Muon" )
    {
      fileName_2015_ = rootFilePath + "/ROOTFile_DiffXSec_FullUnc.root";

      histName_2015_stat = "h_RelStatUnc_Percent";
      histName_2015_effSF = "h_RelSysUnc_Eff.SF._Percent";
      histName_2015_detRes = "h_RelSysUnc_Det.Res_Percent";
      histName_2015_bkg = "h_RelSysUnc_Bkg.Est._Percent";
      histName_2015_FSR = "h_RelSysUnc_FSR_Percent";
      histName_2015_syst = "h_RelSysUnc_Tot_Percent"; 
    }
    else if( leptonType_ == "Electron" )
    {
      fileName_2015_ = rootFilePath + "/ROOTFile_RelUnc_All_Electron_v2.root";
      histName_2015_stat = "h_RelStatUnc_Percent";
      histName_2015_effSF = "h_RelSysUnc_Eff.SF._Percent";
      histName_2015_detRes = "h_RelSysUnc_Det.Res_Percent";
      histName_2015_bkg = "h_RelSysUnc_Bkg.Est._Percent";
      histName_2015_FSR = "h_RelSysUnc_FSR_Percent";
      histName_2015_syst = "h_RelSysUnc_Tot_Percent";
    }
    else
      printf("%s is not recognized lepton type!\n", leptonType_.Data());

    h_relUnc_2015_stat_   = PlotTool::Get_Hist( fileName_2015_, histName_2015_stat );
    h_relUnc_2015_effSF_  = PlotTool::Get_Hist( fileName_2015_, histName_2015_effSF );
    h_relUnc_2015_detRes_ = PlotTool::Get_Hist( fileName_2015_, histName_2015_detRes );
    h_relUnc_2015_bkg_    = PlotTool::Get_Hist( fileName_2015_, histName_2015_bkg );
    h_relUnc_2015_FSR_    = PlotTool::Get_Hist( fileName_2015_, histName_2015_FSR );
    h_relUnc_2015_syst_   = PlotTool::Get_Hist( fileName_2015_, histName_2015_syst );
  }

  void SetHistAll()
  {
    // -- uncorrelated with statistics
    h_relUnc_effSF_  = (TH1D*)h_relUnc_2015_effSF_->Clone();
    h_relUnc_FSR_    = (TH1D*)h_relUnc_2015_FSR_->Clone();

    // -- correlated with statistics
    h_relUnc_detRes_ = (TH1D*)h_relUnc_2015_detRes_->Clone(); h_relUnc_detRes_->Scale( lumiScale_ );
    h_relUnc_bkg_    = (TH1D*)h_relUnc_2015_bkg_->Clone();    h_relUnc_bkg_->Scale( lumiScale_ );

    SetUncHist_Lumi();
    SetUncHist_Acc_Using8TeV();

    SetUncHist_Syst();
  }

  void DrawSummaryPlot()
  {
    TString canvasName = TString::Format("c_ExpectedUnc_1D_%s_%.1lf", leptonType_.Data(), targetLumi_ /1000.0);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 1, 1);
    canvas->SetCanvasSize(800, 700);
    canvas->ForceSameMarkerStyle(kFALSE);

    h_relUnc_stat_->SetMarkerStyle(20);
    canvas->Register(h_relUnc_stat_, "Statistical", kBlack);

    h_relUnc_syst_->SetMarkerStyle(kFullSquare);
    canvas->Register(h_relUnc_syst_, "Total systematic", kRed);

    h_relUnc_effSF_->SetMarkerStyle(25);
    canvas->Register(h_relUnc_effSF_, "Efficiency SF", TColor::GetColor("#0000ff"));

    h_relUnc_detRes_->SetMarkerStyle(26);
    canvas->Register(h_relUnc_detRes_, "Detector resolution", TColor::GetColor("#cc00ff"));

    h_relUnc_bkg_->SetMarkerStyle(27);
    h_relUnc_bkg_->SetMinimum(0.001);
    canvas->Register(h_relUnc_bkg_, "Background", TColor::GetColor("#00cc00"));

    h_relUnc_FSR_->SetMarkerStyle(28);
    canvas->Register(h_relUnc_FSR_, "FSR", TColor::GetColor("#9999ff"));

    h_relUnc_acc_->SetMarkerStyle(24);
    canvas->Register(h_relUnc_acc_, "Acceptance + PDF", TColor::GetColor("#99ff99"));

    h_relUnc_lumi_->SetMarkerStyle(22);
    canvas->Register(h_relUnc_lumi_, "Luminosity", TColor::GetColor("#ff9933"));

    canvas->SetTitle( "m [GeV]", "Rel. Unc. (%)");

    canvas->SetRangeY(5e-3, 1e2);

    canvas->SetLegendPosition( 0.45, 0.15, 0.95, 0.35 );
    canvas->SetLegendColumn(2);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.78, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", targetLumi_ /1000.0));

    TString info = "";
    if( leptonType_ == "Muon" )     info = "Expected uncertainties (#mu channel)";
    if( leptonType_ == "Electron" ) info = "Expected uncertainties (e channel)";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+info+"}}");

    canvas->Draw("HISTLPSAME");
  }

  void SetUncHist_Lumi()
  {
    h_relUnc_lumi_ = new TH1D("h_relUnc_lumi", "", nMassBin, arr_massBinEdge_);

    Double_t relUnc_lumi = 2.5; // -- %
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      h_relUnc_lumi_->SetBinContent(i_bin, relUnc_lumi);
      h_relUnc_lumi_->SetBinError(i_bin, 0);
    }
  }

  void SetUncHist_Acc_Using8TeV()
  {
    TH1D* h_relUnc_accPDF = nullptr;
    TH1D* h_relUnc_modeling = nullptr;

    if( leptonType_ == "Muon" )
    {
      // -- % -- //
      Double_t RelUnc_AccPDF[nMassBin] =
      {
        4.00, 3.78, 3.57, 3.35, 3.12, 2.90, 2.68, 2.46, 2.26, 2.08, 
        1.92, 1.80, 1.70, 1.64, 1.61, 1.60, 1.62, 1.66, 1.72, 1.80,
        1.89, 1.99, 2.10, 2.23, 2.37, 2.53, 2.69, 2.86, 3.03, 3.19,
        3.44, 3.62, 3.80, 3.97, 4.23, 4.19, 4.40, 4.65, 4.92, 4.92,
        4.92, 5.33, 5.62
      };

      Double_t RelUnc_Modeling[nMassBin] = 
      {
        2.01, 1.65, 1.31, 1.00, 0.72, 0.49, 0.30, 0.17, 0.08, 0.02,
        0.00, 0.00, 0.01, 0.03, 0.06, 0.10, 0.14, 0.18, 0.23, 0.27, 
        0.32, 0.37, 0.41, 0.46, 0.49, 0.53, 0.56, 0.59, 0.61, 0.65,
        0.70, 0.76, 0.85, 0.96, 1.10, 1.28, 1.48, 1.70, 1.95, 1.95,
        1.95, 2.20, 2.42
      };

      h_relUnc_accPDF = MakeHist_FromArray( RelUnc_AccPDF );
      h_relUnc_modeling = MakeHist_FromArray( RelUnc_Modeling );
    }
    else if( leptonType_ == "Electron" )
    {
      Double_t RelUnc_AccPDF[nMassBin] =
      {
        4.24, 3.82, 3.60, 3.36, 3.13, 2.91, 2.69, 2.48, 2.29, 2.13, 
        1.99, 1.87, 1.77, 1.67, 1.62, 1.60, 1.62, 1.67, 1.74, 1.83, 
        1.93, 2.04, 2.16, 2.31, 2.45, 2.62, 2.80, 2.98, 3.14, 3.34, 
        3.41, 3.57, 3.74, 3.89, 4.07, 4.24, 4.41, 4.65, 4.91, 4.91, 
        4.91, 5.20, 5.50
      };

      Double_t RelUnc_Modeling[nMassBin] = 
      {
        2.01, 1.65, 1.31, 1.00, 0.72, 0.49, 0.30, 0.17, 0.08, 0.02, 
        0.00, 0.00, 0.01, 0.03, 0.06, 0.10, 0.14, 0.18, 0.23, 0.27, 
        0.32, 0.37, 0.41, 0.46, 0.49, 0.53, 0.56, 0.59, 0.61, 0.65, 
        0.70, 0.76, 0.85, 0.96, 1.10, 1.28, 1.48, 1.70, 1.95, 1.95, 
        1.95, 2.20, 2.42
      };

      h_relUnc_accPDF = MakeHist_FromArray( RelUnc_AccPDF );
      h_relUnc_modeling = MakeHist_FromArray( RelUnc_Modeling );
    }

    h_relUnc_acc_ = PlotTool::QuadSum_NoError( h_relUnc_accPDF, h_relUnc_modeling );
  }

  void SetUncHist_Syst()
  {
    h_relUnc_syst_ = new TH1D("h_relUnc_syst", "", nMassBin, arr_massBinEdge_);

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t relUnc_effSF = h_relUnc_effSF_->GetBinContent(i_bin);
      Double_t relUnc_detRes = h_relUnc_detRes_->GetBinContent(i_bin);
      Double_t relUnc_bkg = h_relUnc_bkg_->GetBinContent(i_bin);
      Double_t relUnc_FSR = h_relUnc_FSR_->GetBinContent(i_bin);

      Double_t relUnc_syst = sqrt(relUnc_effSF  * relUnc_effSF + 
                                  relUnc_detRes * relUnc_detRes + 
                                  relUnc_bkg    * relUnc_bkg + 
                                  relUnc_FSR    * relUnc_FSR );

      h_relUnc_syst_->SetBinContent(i_bin, relUnc_syst);
      h_relUnc_syst_->SetBinError(i_bin, 0);
    }
  }

  TH1D* MakeHist_FromArray( Double_t Array[nMassBin] )
  {
    TH1D* h = new TH1D("h", "", nMassBin, arr_massBinEdge_ );
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      h->SetBinContent(i_bin, Array[i]);
      h->SetBinError(i_bin, 0);
    }

    return h;
  }
};

void EstimateExpectedUnc()
{
  ////////////////////////
  // -- muon channel -- //
  ////////////////////////
  // -- 2016
  TH1D* h_relStatUnc2016_muon = PlotTool::Get_Hist("../StatUnc/ROOTFile_RelStatUnc_2016_Muon.root", "h_RelStatUnc_mass_DY");
  h_relStatUnc2016_muon->Scale(100); // -- convert to %

  ExpectedUncTool* tool_2016_muon = new ExpectedUncTool("Muon");
  tool_2016_muon->SetLumi(35863.818);
  tool_2016_muon->SetStatUncHist(h_relStatUnc2016_muon);
  tool_2016_muon->DrawPlot();
  tool_2016_muon->Save("ROOTFile_ExpectedUnc_2016_Muon.root");

  // -- 2016+17
  TH1D* h_relStatUnc2016and17_muon = PlotTool::Get_Hist("../StatUnc/ROOTFile_RelStatUnc_2016and17_Muon.root", "h_RelStatUnc_mass_DY");
  h_relStatUnc2016and17_muon->Scale(100); // -- convert to %

  ExpectedUncTool* tool_2016and17_muon = new ExpectedUncTool("Muon");
  tool_2016and17_muon->SetLumi(35863.818+41526.280);
  tool_2016and17_muon->SetStatUncHist(h_relStatUnc2016and17_muon);
  tool_2016and17_muon->DrawPlot();
  tool_2016and17_muon->Save("ROOTFile_ExpectedUnc_2016and17_Muon.root");


  ////////////////////////////
  // -- electron channel -- //
  ////////////////////////////
  // -- 2016
  TH1D* h_relStatUnc2016_electron = PlotTool::Get_Hist("../StatUnc/ROOTFile_RelStatUnc_2016_Electron.root", "h_RelStatUnc_mass_DY");
  h_relStatUnc2016_electron->Scale(100); // -- convert to %

  ExpectedUncTool* tool_2016_electron = new ExpectedUncTool("Electron");
  tool_2016_electron->SetLumi(35863.818);
  tool_2016_electron->SetStatUncHist(h_relStatUnc2016_electron);
  tool_2016_electron->DrawPlot();
  tool_2016_electron->Save("ROOTFile_ExpectedUnc_2016_Electron.root");

  // -- 2016+17
  TH1D* h_relStatUnc2016and17_electron = PlotTool::Get_Hist("../StatUnc/ROOTFile_RelStatUnc_2016and17_Electron.root", "h_RelStatUnc_mass_DY");
  h_relStatUnc2016and17_electron->Scale(100); // -- convert to %

  ExpectedUncTool* tool_2016and17_electron = new ExpectedUncTool("Electron");
  tool_2016and17_electron->SetLumi(35863.818+41526.280);
  tool_2016and17_electron->SetStatUncHist(h_relStatUnc2016and17_electron);
  tool_2016and17_electron->DrawPlot();
  tool_2016and17_electron->Save("ROOTFile_ExpectedUnc_2016and17_Electron.root");

  // ExpectedUncTool* tool_electron = new ExpectedUncTool("Electron");
}