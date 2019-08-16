#include <Include/SimplePlotTools.h>

#define nMassRange 6

void DrawPlot(TString flavor, TString type, TString massRangeInfo, TH1D* h_2012, TH1D* h_2016, TH1D* h_2016and17)
{
  TString canvasName = "c_ExpectedUnc2D_"+type+"_"+flavor+"_"+massRangeInfo;
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 1);
  canvas->Register( h_2012,      "2012 data (scaled from 2016 expectation)", kBlack);
  canvas->Register( h_2016,      "2016 data (expectation)",    kGreen+2);
  canvas->Register( h_2016and17, "2016+17 data (expectation)", kBlue);

  TString titleY = "";
  if( type == "Stat" ) titleY = "Stat. Unc. (%)";
  if( type == "Syst" ) titleY = "Syst. Unc. (%)";
  canvas->SetTitle( "|y(ll)|", titleY, "2016(+17)/2012");

  canvas->Latex_CMSPre();
  TString info = "";
  if( flavor == "Muon" )     info = "Expected uncertainties (#mu channel)";
  if( flavor == "Electron" ) info = "Expected uncertainties (e channel)";
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+info+"}}");

  TString massRangeLatex = "";
  if( massRangeInfo == "M20to30" )    massRangeLatex = "20 < M < 30 GeV";
  if( massRangeInfo == "M30to45" )    massRangeLatex = "30 < M < 45 GeV";
  if( massRangeInfo == "M45to60" )    massRangeLatex = "45 < M < 60 GeV";
  if( massRangeInfo == "M60to120" )   massRangeLatex = "60 < M < 120 GeV";
  if( massRangeInfo == "M120to200" )  massRangeLatex = "120 < M < 200 GeV";
  if( massRangeInfo == "M200to1500" ) massRangeLatex = "200 < M < 1500 GeV";
  canvas->RegisterLatex( 0.16, 0.87, "#scale[0.6]{#font[42]{"+massRangeLatex+"}}");

  canvas->SetRangeY(2e-2, 30);
  if( massRangeInfo == "M60to120" ) canvas->SetRangeY(1e-4, 10);

  canvas->SetLegendPosition( 0.30, 0.32, 0.95, 0.50 );
  canvas->SetRangeRatio(0, 1.01);
  canvas->Draw("HISTLPSAME");
}

void DrawPlot_2012_2016_2016and17_EachMassRange(TString massRangeInfo)
{
  // -- Muon
  TH1D* h_2012_stat_mu      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016_Muon.root", "h_relUnc_2012_stat_"+massRangeInfo);
  TH1D* h_2016_stat_mu      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016_Muon.root", "h_relUnc_stat_"+massRangeInfo);
  TH1D* h_2016and17_stat_mu = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016and17_Muon.root", "h_relUnc_stat_"+massRangeInfo);
  DrawPlot( "Muon", "Stat", massRangeInfo, h_2012_stat_mu, h_2016_stat_mu, h_2016and17_stat_mu );

  TH1D* h_2012_syst_mu      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016_Muon.root", "h_relUnc_2012_syst_"+massRangeInfo);
  TH1D* h_2016_syst_mu      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016_Muon.root", "h_relUnc_syst_"+massRangeInfo);
  TH1D* h_2016and17_syst_mu = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016and17_Muon.root", "h_relUnc_syst_"+massRangeInfo);
  DrawPlot( "Muon", "Syst", massRangeInfo, h_2012_syst_mu, h_2016_syst_mu, h_2016and17_syst_mu );

  // -- Electron
  TH1D* h_2012_stat_elec      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016_Electron.root", "h_relUnc_2012_stat_"+massRangeInfo);
  TH1D* h_2016_stat_elec      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016_Electron.root", "h_relUnc_stat_"+massRangeInfo);
  TH1D* h_2016and17_stat_elec = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016and17_Electron.root", "h_relUnc_stat_"+massRangeInfo);
  DrawPlot( "Electron", "Stat", massRangeInfo, h_2012_stat_elec, h_2016_stat_elec, h_2016and17_stat_elec );

  TH1D* h_2012_syst_elec      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016_Electron.root", "h_relUnc_2012_syst_"+massRangeInfo);
  TH1D* h_2016_syst_elec      = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016_Electron.root", "h_relUnc_syst_"+massRangeInfo);
  TH1D* h_2016and17_syst_elec = PlotTool::Get_Hist("ROOTFile_ExpectedUnc2D_2016and17_Electron.root", "h_relUnc_syst_"+massRangeInfo);
  DrawPlot( "Electron", "Syst", massRangeInfo, h_2012_syst_elec, h_2016_syst_elec, h_2016and17_syst_elec );
}

void DrawPlot_2012_2016_2016and17_2D()
{
  vector< TString > vec_massRangeInfo(6);
  vec_massRangeInfo[0] = "M20to30";
  vec_massRangeInfo[1] = "M30to45";
  vec_massRangeInfo[2] = "M45to60";
  vec_massRangeInfo[3] = "M60to120";
  vec_massRangeInfo[4] = "M120to200";
  vec_massRangeInfo[5] = "M200to1500";

  for( const auto& massRangeInfo : vec_massRangeInfo )
    DrawPlot_2012_2016_2016and17_EachMassRange(massRangeInfo);
}

