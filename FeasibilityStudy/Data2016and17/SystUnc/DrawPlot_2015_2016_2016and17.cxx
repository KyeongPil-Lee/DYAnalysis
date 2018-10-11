#include <Include/SimplePlotTools.h>

void DrawPlot(TString flavor, TString type, TH1D* h_2015, TH1D* h_2016, TH1D* h_2016and17)
{
  TString canvasName = "c_ExpectedUnc_1D_"+type+"_"+flavor;
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->Register( h_2015,      "2015 data",                  kBlack);
  canvas->Register( h_2016,      "2016 data (expectation)",    kGreen+2);
  canvas->Register( h_2016and17, "2016+17 data (expectation)", kBlue);

  TString titleY = "";
  if( type == "Stat" ) titleY = "Stat. Unc. (%)";
  if( type == "Syst" ) titleY = "Syst. Unc. (%)";
  canvas->SetTitle( "m [GeV]", titleY, "2016(+17)/2015");

  canvas->Latex_CMSPre();
  if( type == "Stat" ) canvas->SetRangeY(5e-3, 2e2);
  if( type == "Syst" ) canvas->SetRangeY(5e-1, 2e2);

  canvas->SetLegendPosition( 0.45, 0.32, 0.95, 0.45 );
  canvas->SetRangeRatio(0, 1.01);
  canvas->Draw("HISTLPSAME");
}

void DrawPlot_2015_2016_2016and17()
{
  // -- Muon
  TH1D* h_2015_stat_mu      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016_Muon.root", "h_relUnc_2015_stat");
  TH1D* h_2016_stat_mu      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016_Muon.root", "h_relUnc_stat");
  TH1D* h_2016and17_stat_mu = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016and17_Muon.root", "h_relUnc_stat");
  DrawPlot( "Muon", "Stat", h_2015_stat_mu, h_2016_stat_mu, h_2016and17_stat_mu );

  TH1D* h_2015_syst_mu      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016_Muon.root", "h_relUnc_2015_syst");
  TH1D* h_2016_syst_mu      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016_Muon.root", "h_relUnc_syst");
  TH1D* h_2016and17_syst_mu = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016and17_Muon.root", "h_relUnc_syst");
  DrawPlot( "Muon", "Syst", h_2015_syst_mu, h_2016_syst_mu, h_2016and17_syst_mu );

  // -- Electron
  TH1D* h_2015_stat_elec      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016_Electron.root", "h_relUnc_2015_stat");
  TH1D* h_2016_stat_elec      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016_Electron.root", "h_relUnc_stat");
  TH1D* h_2016and17_stat_elec = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016and17_Electron.root", "h_relUnc_stat");
  DrawPlot( "Electron", "Stat", h_2015_stat_elec, h_2016_stat_elec, h_2016and17_stat_elec );

  TH1D* h_2015_syst_elec      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016_Electron.root", "h_relUnc_2015_syst");
  TH1D* h_2016_syst_elec      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016_Electron.root", "h_relUnc_syst");
  TH1D* h_2016and17_syst_elec = PlotTool::Get_Hist("ROOTFile_ExpectedUnc_2016and17_Electron.root", "h_relUnc_syst");
  DrawPlot( "Electron", "Syst", h_2015_syst_elec, h_2016_syst_elec, h_2016and17_syst_elec );
}

