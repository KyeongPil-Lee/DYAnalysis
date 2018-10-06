#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>

void DrawCanvas_Ratio_Eff(TH1D* h_ratio, TH1D *h_eff);
void DrawCanvas_MainSample_PDFUncSample(TH1D* h_PDFUncSample, TH1D *h_mainSample);
void DrawCanvas_Ratio2_Eff(TH1D* h_ratio2, TH1D *h_eff);
void DrawCanvas_Ratio3_Eff(TH1D* h_ratio3, TH1D *h_eff);
void DrawCanvas_GENOnly_KP_Ridhi(TH1D* h_KP, TH1D *h_Ridhi);
// void DrawCanvas_GENOnly_KP_Ridhi_v2(TH1D* h_KP, TH1D *h_Ridhi);

void Validate_MC()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_KP = analyzerPath+"/TheoryValues/PDFUnc_aMCNLO/ROOTFile_Hists_ForPDFUnc_XSec_aMCNLO_Electron.root";

  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  // TString fileName_Ridhi = rootFilePath + "/DiffXsec_Electron_v9.root";
  // TString fileName_accEff = rootFilePath+"/ROOTFile_AccEff_Electron.root";

  // TH1D* h_DXSec_Ridhi = PlotTool::Get_Hist( fileName_Ridhi, "h_DiffXSec_MCNLO_Fiducial" );
  TH1D* h_DXSec_KP = PlotTool::Get_Hist(fileName_KP, "h_mass_withinAcc_CV_All");
  h_DXSec_KP->Scale( 1.0 / Lumi_Elec );
  h_DXSec_KP = PlotTool::DivideEachBin_ByBinWidth( h_DXSec_KP ); // -- make diff. x-section -- //

  // TH1D* h_ratio = (TH1D*)h_DXSec_Ridhi->Clone("h_ratio");
  // h_ratio->Divide( h_DXSec_Ridhi, h_DXSec_KP );

  // TGraphAsymmErrors *g_eff = PlotTool::Get_Graph(fileName_accEff, "g_eff");
  // TH1D* h_eff = PlotTool::Convert_GraphToHist(g_eff);

  // PlotTool::IsRatio1( h_ratio, h_eff );
  // DrawCanvas_Ratio_Eff( h_ratio, h_eff );

  // -- MC fiducial cross section by the sample used by main analysis
  // TString fileName_DXSecMC = "./DXSec_MC/ROOTFile_DXSec_MC_aMCNLO_ee_AdditionalSF.root";
  // TString fileName_DXSecMC = "./DXSec_MC/ROOTFile_DXSec_MC_aMCNLO_ee_AdditionalSF_LargeN_M10to50.root";
  // TString fileName_DXSecMC = "./DXSec_MC/ROOTFile_DXSec_MC_aMCNLO_ee_NoSF.root";
  TString fileName_DXSecMC = "./DXSec_MC/ROOTFile_DXSec_MC_aMCNLO_ee_NoSF_LargeN_M10to50.root";
  TH1D* h_DXSec_mainSample = PlotTool::Get_Hist(fileName_DXSecMC, "h_DXSec_merged");
  DrawCanvas_MainSample_PDFUncSample(h_DXSec_KP, h_DXSec_mainSample);

  // -- ratio is calculated again with the MC fiducial cross section from the sample for main analysis
  // TH1D* h_ratio2 = (TH1D*)h_DXSec_Ridhi->Clone("h_ratio2");
  // h_ratio2->Divide( h_DXSec_Ridhi, h_DXSec_mainSample );

  // DrawCanvas_Ratio2_Eff( h_ratio2, h_eff );


  // -- MC fiducial cross section provided by Ridhi: using only gen-level info.
  TString fileName_RidhiGEN = "Temp/DiffXsec_Electron_v10.root";
  TH1D* h_DXSec_RidhiGEN = PlotTool::Get_Hist(fileName_RidhiGEN, "h_DiffXSec_MCNLO_Fiducial");

  DrawCanvas_GENOnly_KP_Ridhi( h_DXSec_mainSample, h_DXSec_RidhiGEN );
  PlotTool::IsRatio1( h_DXSec_RidhiGEN, h_DXSec_mainSample );

  // TH1D* h_DXSec_mainSample_lumiTest = (TH1D*)h_DXSec_mainSample->Clone();
  // h_DXSec_mainSample_lumiTest->Scale( 2318.348 ); // -- old luminosity
  // h_DXSec_mainSample_lumiTest->Scale( 1.0 / 2257.998 ); // -- new luminosity
  // // h_DXSec_mainSample_lumiTest->Scale( 1.0 / 2258.066 ); // -- what Ridhi used

  // DrawCanvas_GENOnly_KP_Ridhi_v2( h_DXSec_mainSample_lumiTest, h_DXSec_RidhiGEN );
  // PlotTool::IsRatio1( h_DXSec_RidhiGEN, h_DXSec_mainSample_lumiTest );
}

void DrawCanvas_Ratio_Eff(TH1D* h_ratio, TH1D *h_eff)
{
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_Ratio_vs_Eff_MC", 1, 0);
  canvas->Register( h_ratio, "Ratio of MC F-DXSec (Ridhi/KP)", kBlack);
  canvas->Register( h_eff, "Efficiency", kBlue);

  canvas->SetTitle( "m [GeV]", "Value", "Blue/Black");
  canvas->Latex_CMSPre();
  canvas->SetRangeY(0, 1);
  canvas->SetLegendPosition( 0.40, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0.94, 1.06);
  canvas->Draw("EPSAME");
}

void DrawCanvas_MainSample_PDFUncSample(TH1D* h_PDFUncSample, TH1D *h_mainSample)
{
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_PDFUncSample_vs_mainSample", 1, 1);
  canvas->Register( h_PDFUncSample, "Sample for PDF Unc.", kBlack);
  canvas->Register( h_mainSample, "Sample for main analysis", kBlue);

  canvas->SetTitle( "m [GeV]", "Value", "Blue/Black");
  canvas->Latex_CMSPre();
  canvas->SetRangeY(1e-7, 1e3);
  canvas->SetLegendPosition( 0.40, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0.94, 1.06);
  canvas->Draw("EPSAME");
}

void DrawCanvas_Ratio2_Eff(TH1D* h_ratio2, TH1D *h_eff)
{
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_Ratio2_vs_Eff_MC", 1, 0);
  canvas->Register( h_ratio2, "Ratio of MC F-DXSec (Ridhi/KP)", kBlack);
  canvas->Register( h_eff, "Efficiency", kBlue);

  canvas->SetTitle( "m [GeV]", "Value", "Blue/Black");
  canvas->Latex_CMSPre();
  canvas->SetRangeY(0, 1);
  canvas->SetLegendPosition( 0.40, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0.94, 1.06);
  canvas->Draw("EPSAME");
}

void DrawCanvas_Ratio3_Eff(TH1D* h_ratio3, TH1D *h_eff)
{
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_Ratio3_vs_Eff_MC", 1, 0);
  canvas->Register( h_ratio3, "Ratio of MC F-DXSec (Ridhi/KP): GEN-only", kBlack);
  canvas->Register( h_eff, "Efficiency", kBlue);

  canvas->SetTitle( "m [GeV]", "Value", "Blue/Black");
  canvas->Latex_CMSPre();
  canvas->SetRangeY(0, 2);
  canvas->SetLegendPosition( 0.40, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0.94, 1.06);
  canvas->Draw("EPSAME");
}

void DrawCanvas_GENOnly_KP_Ridhi(TH1D* h_KP, TH1D *h_Ridhi)
{
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_GenOnly_KP_vs_Ridhi_MC", 1, 1);
  canvas->Register( h_KP, "Gen-only MC F-DXSec (KP)", kBlack);
  canvas->Register( h_Ridhi, "Gen-only MC F-DXSec (Ridhi)", kBlue);

  canvas->SetTitle( "m [GeV]", "d#sigma/dm [pb/GeV]", "Blue/Black");
  canvas->Latex_CMSSim();
  canvas->SetRangeY(1e-8, 1e3);
  canvas->SetLegendPosition( 0.40, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0.94, 1.06);
  // canvas->SetRangeRatio(0.88, 1.12);
  // canvas->SetRangeRatio(0.99, 1.01);
  canvas->Draw("EPSAME");
}

// void DrawCanvas_GENOnly_KP_Ridhi_v2(TH1D* h_KP, TH1D *h_Ridhi)
// {
//   PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio("c_GenOnly_KP_vs_Ridhi_MC_v2", 1, 1);
//   canvas->Register( h_KP, "Gen-only MC F-DXSec (KP) (lumi. test)", kBlack);
//   canvas->Register( h_Ridhi, "Gen-only MC F-DXSec (Ridhi)", kBlue);

//   canvas->SetTitle( "m [GeV]", "d#sigma/dm [pb/GeV]", "Blue/Black");
//   canvas->Latex_CMSSim();
//   canvas->SetRangeY(1e-8, 1e3);
//   canvas->SetLegendPosition( 0.40, 0.32, 0.95, 0.45 );
//   // canvas->SetRangeRatio(0.88, 1.12);
//   canvas->SetRangeRatio(0.94, 1.06);
//   canvas->Draw("EPSAME");
// }