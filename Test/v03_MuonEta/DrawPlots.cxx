#include <Include/DYAnalyzer.h> // -- for the luminosity values
#include <Include/SimplePlotTools.h>

void DrawOnePlot(TString histNameBase, Bool_t applyEffSF = kTRUE );

void AddHistogram(TH1D*& h_sum, TH1D* h_temp );
void GetTitle(  TString histNameBase, TString& titleX, TString& titleY );
void GetRangeX( TString histNameBase, Double_t &minX, Double_t &maxX );

void DrawPlots()
{
  DrawOnePlot( "h_mass", kFALSE );
  DrawOnePlot( "h_mass", kTRUE );

  DrawOnePlot( "h_rapidity", kFALSE );
  DrawOnePlot( "h_rapidity", kTRUE );

  DrawOnePlot( "h_eta", kFALSE );
  DrawOnePlot( "h_eta", kTRUE );

  DrawOnePlot( "h_eta_lead", kFALSE );
  DrawOnePlot( "h_eta_lead", kTRUE );

  DrawOnePlot( "h_eta_lead_Pt10to22", kFALSE );
  DrawOnePlot( "h_eta_lead_Pt10to22", kTRUE );

  DrawOnePlot( "h_eta_lead_Pt22to40", kFALSE );
  DrawOnePlot( "h_eta_lead_Pt22to40", kTRUE );

  DrawOnePlot( "h_eta_lead_Pt40to70", kFALSE );
  DrawOnePlot( "h_eta_lead_Pt40to70", kTRUE );

  DrawOnePlot( "h_eta_lead_Pt70to250", kFALSE );
  DrawOnePlot( "h_eta_lead_Pt70to250", kTRUE );

  DrawOnePlot( "h_eta_lead_Pt250toInf", kFALSE );
  DrawOnePlot( "h_eta_lead_Pt250toInf", kTRUE );


  DrawOnePlot( "h_eta_subLead", kFALSE );
  DrawOnePlot( "h_eta_subLead", kTRUE );

  DrawOnePlot( "h_eta_subLead_Pt10to22", kFALSE );
  DrawOnePlot( "h_eta_subLead_Pt10to22", kTRUE );

  DrawOnePlot( "h_eta_subLead_Pt22to40", kFALSE );
  DrawOnePlot( "h_eta_subLead_Pt22to40", kTRUE );

  DrawOnePlot( "h_eta_subLead_Pt40to70", kFALSE );
  DrawOnePlot( "h_eta_subLead_Pt40to70", kTRUE );

  DrawOnePlot( "h_eta_subLead_Pt70to250", kFALSE );
  DrawOnePlot( "h_eta_subLead_Pt70to250", kTRUE );

  DrawOnePlot( "h_eta_subLead_Pt250toInf", kFALSE );
  DrawOnePlot( "h_eta_subLead_Pt250toInf", kTRUE );
}

void DrawOnePlot(TString histNameBase, Bool_t applyEffSF = kTRUE )
{
  TString applyEffSFStr = "";
  if( applyEffSF ) applyEffSFStr = "withSF";
  else             applyEffSFStr = "withoutSF";

  TString fileName_data = "Local/ROOTFile_MuonPlot_MuonPhys.root";
  TH1D* h_data = PlotTool::Get_Hist( fileName_data, histNameBase+"_Data");

  DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );
  vector<TString> ntuplePath; 
  vector<TString> tag;
  vector<Double_t> xSec;
  vector<Double_t> nEvent;
  analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntuplePath, &tag, &xSec, &nEvent);

  // vector<TString> MCProcessStr = 
  // {
  //   "ZZ", "WZ", "WZ",
  //   "WJets",
  //   "DYTauTau_M10to50", "DYTauTau",
  //   "ttbar",
  //   "DYMuMu_M10to50",
  //   "DYMuMu_M50to100",
  //   "DYMuMu_M100to200",
  //   "DYMuMu_M200to400",
  //   "DYMuMu_M400to500",
  //   "DYMuMu_M500to700",
  //   "DYMuMu_M700to800",
  //   "DYMuMu_M800to1000",
  //   "DYMuMu_M1000to1500",
  //   "DYMuMu_M1500to2000",
  //   "DYMuMu_M2000to3000"
  // };

  TString fileName = "Local/ROOTFile_MuonPlot_MC_"+applyEffSFStr+".root";

  TH1D* h_diboson = NULL;
  TH1D* h_wjets = NULL;
  TH1D* h_DYTauTau = NULL;
  TH1D* h_ttbar = NULL;
  TH1D* h_DYMuMu = NULL;

  Int_t nSample = (Int_t)tag.size();
  for(Int_t i_sample=0; i_sample<nSample; i_sample++)
  {
    TString histName = histNameBase + "_" + tag[i_sample];
    TH1D* h_temp = PlotTool::Get_Hist(fileName, histName);
    h_temp->Sumw2();

    // -- normalization
    Double_t normFactor = (xSec[i_sample] * Lumi) / nEvent[i_sample];
    h_temp->Scale( normFactor );

    if( tag[i_sample] == "ZZ" || tag[i_sample] == "WZ" || tag[i_sample] == "WW" )
      AddHistogram( h_diboson, h_temp);

    if( tag[i_sample] == "WJets" )
      AddHistogram( h_wjets, h_temp);

    if( tag[i_sample].Contains("DYTauTau") )
      AddHistogram( h_DYTauTau, h_temp);

    if( tag[i_sample] == "ttbar" )
      AddHistogram( h_ttbar, h_temp);

    if( tag[i_sample].Contains("DYMuMu") )
      AddHistogram( h_DYMuMu, h_temp);
  }

  TString canvasName = histNameBase.ReplaceAll("h_" ,"c_" )+"_"+applyEffSFStr;
  histNameBase.ReplaceAll("c_", "h_" ); // -- back to the original
  PlotTool::DataMCStackCanvaswRatio *canvas = new PlotTool::DataMCStackCanvaswRatio(canvasName, 0, 1);
  TString titleX = "";
  TString titleY = "";
  GetTitle( histNameBase, titleX, titleY );
  canvas->SetTitle( titleX, titleY, "Data/MC" );
  canvas->SetLegendPosition(0.75, 0.75, 0.95, 0.95);

  Bool_t isMC = 0;
  canvas->Register( h_data, "Data", kBlack, isMC);

  isMC = 1;
  canvas->Register( h_diboson, "VV", kGreen+2, isMC);
  canvas->Register( h_wjets, "W+jets", kBlue, isMC);
  canvas->Register( h_DYTauTau, "Z/#gamma*#rightarrow#tau#tau", kBlue-9, isMC);
  canvas->Register( h_ttbar, "t#bar{t}", kRed, isMC);
  canvas->Register( h_DYMuMu, "Z/#gamma*#rightarrow#mu#mu", kOrange-2, isMC);

  // if( histNameBase.Contains("Pt10to22") || 
  //     histNameBase.Contains("Pt22to40") || 
  //     histNameBase.Contains("Pt40to70") || 
  //     histNameBase.Contains("Pt70to250") || 
  //     histNameBase.Contains("Pt250toInf") )
  //   canvas->SetRebin( 2 );

  Double_t minX, maxX;
  GetRangeX( histNameBase, minX, maxX);

  canvas->SetRangeX(minX, maxX);
  canvas->SetRangeY(1e-1, 1e8);
  canvas->SetRangeRatio(0.9, 1.05);
  canvas->Latex_CMSPre( 2.8, 13 );
  if( histNameBase != "h_mass" )
    canvas->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{60 < M(#mu#mu) < 120 GeV}}");

  if( histNameBase.Contains("Pt10to22") )
    canvas->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{10 < P_{T}(#mu) < 22 GeV}}");
  if( histNameBase.Contains("Pt22to40") )
    canvas->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{22 < P_{T}(#mu) < 40 GeV}}");
  if( histNameBase.Contains("Pt40to70") )
    canvas->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{40 < P_{T}(#mu) < 70 GeV}}");
  if( histNameBase.Contains("Pt70to250") )
    canvas->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{70 < P_{T}(#mu) < 250 GeV}}");
  if( histNameBase.Contains("Pt250toInf") )
    canvas->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{P_{T}(#mu) > 250 GeV}}");

  canvas->Draw();
}

void AddHistogram(TH1D*& h_sum, TH1D* h_temp )
{
  if( h_sum == NULL )
    h_sum = (TH1D*)h_temp->Clone();
  else
    h_sum->Add( h_temp );
}

void GetTitle( TString histNameBase, TString& titleX, TString& titleY )
{
  if( histNameBase == "h_mass" )
  {
    titleX = "m [GeV]";
    titleY = "Event / 1 GeV";
  }

  if( histNameBase == "h_eta" )
  {
    titleX = "#eta(#mu)";
    titleY = "Muon / 0.1";
  }

  if( histNameBase.Contains("h_eta_lead") )
  {
    titleX = "#eta(#mu, leading)";
    titleY = "Muon / 0.1";
  }

  if( histNameBase.Contains("h_eta_subLead") )
  {
    titleX = "#eta(#mu, sub-leading)";
    titleY = "Muon / 0.1";
  }

  if( histNameBase == "h_rapidity" )
  {
    titleX = "y(#mu#mu)";
    titleY = "Event / 0.1";
  }
}

void GetRangeX( TString histNameBase, Double_t &minX, Double_t &maxX )
{
  if( histNameBase == "h_mass" )
  {
    minX = 60;
    maxX = 120;
  }

  if( histNameBase.Contains("h_eta") )
  {
    minX = -2.4;
    maxX = 2.4;
  }

  if( histNameBase == "h_rapidity" )
  {
    minX = -2.4;
    maxX = 2.4;
  }
}