#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>

TH1D* MakeHist_RelLumiUnc( TH1D* h_template, Double_t RelLumiUnc );
void AssignTotalError( TH1D* h_CV, TH1D* h_RelUnc_Stat, TH1D* h_RelUnc_Syst, TH1D* h_RelUnc_Lumi );
TH1D* MultiplyEfficiency( TH1D* h_dXSec, TGraphAsymmErrors* g_eff );

void CompareFiducialXSec_KP_Ridhi()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_KP = analyzerPath+"/TheoryValues/PDFUnc_aMCNLO/ROOTFile_Hists_ForPDFUnc_XSec_aMCNLO_Electron.root";
  TH1D* h_MC_KP = PlotTool::Get_Hist(fileName_KP, "h_mass_withinAcc_CV_All");
  h_MC_KP->Scale( 1.0 / Lumi_Elec );
  h_MC_KP = PlotTool::DivideEachBin_ByBinWidth( h_MC_KP ); // -- make diff. x-section -- //

  // -- multiply the efficiency
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName_accEff = rootFilePath+"/ROOTFile_AccEff_Electron.root";
  TGraphAsymmErrors* g_eff_Ridhi = PlotTool::Get_Graph( fileName_accEff, "g_eff" );
  TH1D* h_MC_KPTimesEff = MultiplyEfficiency(h_MC_KP, g_eff_Ridhi);

  // -- Ridhi's cross sections
  TString fileName_Ridhi = rootFilePath + "/DiffXsec_Electron_v8.root";
  TH1D* h_Data_Ridhi = PlotTool::Get_Hist( fileName_Ridhi, "h_DiffXSec_Fiducial" );
  TH1D* h_MC_Ridhi = PlotTool::Get_Hist( fileName_Ridhi, "h_DiffXSec_MCNLO_Fiducial" );

  TH1D* h_Data_RelUnc_Stat = PlotTool::Get_Hist( fileName_Ridhi, "h_RelUnc_Stat_Fiducial" ); h_Data_RelUnc_Stat->Scale( 0.01 );
  TH1D* h_Data_RelUnc_Syst = PlotTool::Get_Hist( fileName_Ridhi, "h_RelUnc_Syst_Fiducial" ); h_Data_RelUnc_Syst->Scale( 0.01 );
  TH1D* h_RelUnc_Lumi = MakeHist_RelLumiUnc( h_Data_Ridhi, 0.023 );
  AssignTotalError( h_Data_Ridhi, h_Data_RelUnc_Stat, h_Data_RelUnc_Syst, h_RelUnc_Lumi );

  PlotTool::HistCanvaswRatio *c_DXSec_EE = new PlotTool::HistCanvaswRatio( "c_DXSec_EE_KP_vs_Ridhi", 1, 1 );
  c_DXSec_EE->Register( h_Data_Ridhi, "Data (by Ridhi)", kBlack);
  c_DXSec_EE->Register( h_MC_Ridhi, "aMC@NLO (by Ridhi)", kRed);
  c_DXSec_EE->Register( h_MC_KP, "aMC@NLO (by KP)", kBlue);
  c_DXSec_EE->Register( h_MC_KPTimesEff, "aMC@NLO (by KP) * Eff (by Ridhi)", kViolet);
  c_DXSec_EE->SetTitle( "m [GeV]", "d#sigma/dm [pb/GeV]", "MC/Data");
  // c_DXSec_EE->SetLegendPosition( 0.15, 0.82, 0.75, 0.95 );
  c_DXSec_EE->SetRangeY( 1e-7, 2e2 );
  c_DXSec_EE->Latex_CMSPre();
  c_DXSec_EE->Draw("EPSAME");


  PlotTool::HistCanvaswRatio *c_DXSec_EE2 = new PlotTool::HistCanvaswRatio( "c_DXSec_EE_KP_vs_Ridhi2", 1, 1 );
  c_DXSec_EE2->Register( h_MC_Ridhi, "aMC@NLO (by Ridhi)", kRed);
  c_DXSec_EE2->Register( h_MC_KPTimesEff, "aMC@NLO (by KP) * Eff (by Ridhi)", kViolet);
  c_DXSec_EE2->SetTitle( "m [GeV]", "d#sigma/dm [pb/GeV]", "MC/Data");
  // c_DXSec_EE2->SetLegendPosition( 0.15, 0.82, 0.75, 0.95 );
  c_DXSec_EE2->SetRangeY( 1e-7, 2e2 );
  c_DXSec_EE2->SetRangeRatio( 0.95, 1.05 );
  c_DXSec_EE2->Latex_CMSPre();
  c_DXSec_EE2->Draw("EPSAME");
}

TH1D* MakeHist_RelLumiUnc( TH1D* h_template, Double_t RelLumiUnc )
{
  TH1D* h_RelLumiUnc = (TH1D*)h_template->Clone();
  Int_t nBin = h_RelLumiUnc->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;
    h_RelLumiUnc->SetBinContent(i_bin, RelLumiUnc );
    h_RelLumiUnc->SetBinError(i_bin, 0);
  }

  return h_RelLumiUnc;
}

void AssignTotalError( TH1D* h_CV, TH1D* h_RelUnc_Stat, TH1D* h_RelUnc_Syst, TH1D* h_RelUnc_Lumi )
{
  Int_t nBin = h_CV->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    Double_t RelUnc_Stat = h_RelUnc_Stat->GetBinContent(i_bin);
    Double_t RelUnc_Syst = h_RelUnc_Syst->GetBinContent(i_bin);
    Double_t RelUnc_Lumi = h_RelUnc_Lumi->GetBinContent(i_bin);
    Double_t RelUnc_Tot = sqrt( RelUnc_Stat*RelUnc_Stat + RelUnc_Syst*RelUnc_Syst + RelUnc_Lumi*RelUnc_Lumi );

    Double_t DXSec = h_CV->GetBinContent(i_bin);
    h_CV->SetBinError(i_bin, DXSec*RelUnc_Tot);
  }
}

TH1D* MultiplyEfficiency( TH1D* h_dXSec, TGraphAsymmErrors* g_eff )
{
  TH1D* h_dXSec_Corr = (TH1D*)h_dXSec->Clone();

  Int_t nBin = h_dXSec->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    Double_t dXSec_before = h_dXSec->GetBinContent(i_bin);
    Double_t dXSecErr_before = h_dXSec->GetBinError(i_bin);

    Double_t x, eff;
    g_eff->GetPoint(i, x, eff);

    Double_t dXSec_after = dXSec_before * eff;
    Double_t dXSecErr_after = dXSecErr_before * eff;

    h_dXSec_Corr->SetBinContent(i_bin, dXSec_after);
    h_dXSec_Corr->SetBinError(i_bin, dXSecErr_after);
  }

  return h_dXSec_Corr;
}