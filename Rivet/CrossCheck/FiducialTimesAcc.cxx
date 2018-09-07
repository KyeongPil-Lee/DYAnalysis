#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>

TH1D* h_dXSec_KP();
TH1D* CorrectAcceptance( TH1D* h_dXSec, TGraphAsymmErrors* g_acc );

void FiducialTimesAcc()
{
  TH1D* h_MC_dXSec_KP = h_dXSec_KP();

  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName_Ridhi = rootFilePath + "/DiffXsec_Electron_v8.root";
  TH1D* h_MC_dXSec_Ridhi = PlotTool::Get_Hist( fileName_Ridhi, "h_DiffXSec_MCNLO_Fiducial" );

  TString fileName_acc = rootFilePath+"/ROOTFile_AccEff_Electron.root";
  // TGraphAsymmErrors* g_acc_Ridhi = PlotTool::Get_Graph( fileName_acc, "g_acc" );
  TGraphAsymmErrors* g_acc_Ridhi = PlotTool::Get_Graph( fileName_acc, "g_accEff" );

  TH1D* h_MC_dXSecAccCorr_KP = CorrectAcceptance( h_MC_dXSec_KP, g_acc_Ridhi );
  TH1D* h_MC_dXSecAccCorr_Ridhi = CorrectAcceptance( h_MC_dXSec_Ridhi, g_acc_Ridhi );

  // -- FPS = Full phase space
  TString FileName_data = rootFilePath + "/DiffXsec_Electron_v8.root";
  TH1D* h_dXSec_FPS = PlotTool::Get_Hist(FileName_data, "h_DiffXSec");


  // TString canvasName = "c_DXSec_EE_KP_vs_Ridhi_AccCorr";
  TString canvasName = "c_DXSec_EE_KP_vs_Ridhi_AccEffCorr";

  PlotTool::HistCanvaswRatio *c_DXSec_EE = new PlotTool::HistCanvaswRatio( canvasName, 1, 1 );
  c_DXSec_EE->Register( h_dXSec_FPS, "Data (by Ridhi)", kBlack);
  // c_DXSec_EE->Register( h_MC_dXSecAccCorr_Ridhi, "aMC@NLO: Fiducial (Ridhi) * Acc (Ridhi)", kRed);
  // c_DXSec_EE->Register( h_MC_dXSecAccCorr_KP, "aMC@NLO: Fiducial (KP) * Acc (Ridhi)", kBlue);
  c_DXSec_EE->Register( h_MC_dXSecAccCorr_Ridhi, "aMC@NLO: Fiducial (Ridhi) * AccEff (Ridhi)", kRed);
  c_DXSec_EE->Register( h_MC_dXSecAccCorr_KP, "aMC@NLO: Fiducial (KP) * AccEff (Ridhi)", kBlue);
  c_DXSec_EE->SetTitle( "m [GeV]", "d#sigma/dm [pb/GeV]", "MC/Data");
  // c_DXSec_EE->SetLegendPosition( 0.15, 0.82, 0.75, 0.95 );
  c_DXSec_EE->SetRangeY( 1e-7, 2e3 );
  c_DXSec_EE->SetRangeRatio( 0, 2 );
  c_DXSec_EE->Latex_CMSPre();
  c_DXSec_EE->Draw("EPSAME");
}

TH1D* CorrectAcceptance( TH1D* h_dXSec, TGraphAsymmErrors* g_acc )
{
  TH1D* h_dXSec_AccCorr = (TH1D*)h_dXSec->Clone();

  Int_t nBin = h_dXSec->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    Double_t dXSec_before = h_dXSec->GetBinContent(i_bin);
    Double_t dXSecErr_before = h_dXSec->GetBinError(i_bin);

    Double_t x, acc;
    g_acc->GetPoint(i, x, acc);

    Double_t dXSec_after = dXSec_before / acc;
    Double_t dXSecErr_after = dXSecErr_before / acc;

    h_dXSec_AccCorr->SetBinContent(i_bin, dXSec_after);
    h_dXSec_AccCorr->SetBinError(i_bin, dXSecErr_after);
  }

  return h_dXSec_AccCorr;
}

TH1D* h_dXSec_KP()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_KP = analyzerPath+"/TheoryValues/PDFUnc_aMCNLO/ROOTFile_Hists_ForPDFUnc_XSec_aMCNLO_Electron.root";
  TH1D* h_MC_KP = PlotTool::Get_Hist(fileName_KP, "h_mass_withinAcc_CV_All");
  h_MC_KP->Scale( 1.0 / Lumi_Elec );
  h_MC_KP = PlotTool::DivideEachBin_ByBinWidth( h_MC_KP ); // -- make diff. x-section -- //

  return h_MC_KP;
}