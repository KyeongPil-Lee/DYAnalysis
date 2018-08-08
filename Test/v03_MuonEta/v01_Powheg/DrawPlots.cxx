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

  DrawOnePlot( "h_eta_subLead", kFALSE );
  DrawOnePlot( "h_eta_subLead", kTRUE );
}

void DrawOnePlot(TString histNameBase, Bool_t applyEffSF = kTRUE )
{
  TString applyEffSFStr = "";
  if( applyEffSF ) applyEffSFStr = "withSF";
  else             applyEffSFStr = "withoutSF";

  TString fileName_data = "../Local/ROOTFile_MuonPlot_MuonPhys.root";
  TH1D* h_data = PlotTool::Get_Hist( fileName_data, histNameBase+"_Data");

  DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );
  vector<TString> ntuplePath; 
  vector<TString> tag;
  vector<Double_t> xSec;
  vector<Double_t> nEvent;
  analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_Powheg", &ntuplePath, &tag, &xSec, &nEvent);

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

    if( tag[i_sample].Contains("ZMuMu") )
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
  canvas->Register( h_DYMuMu, "Z/#gamma*#rightarrow#mu#mu (Powheg)", kOrange-2, isMC);

  Double_t minX, maxX;
  GetRangeX( histNameBase, minX, maxX);

  canvas->SetRangeX(minX, maxX);
  canvas->SetRangeY(1e-1, 1e8);
  canvas->SetRangeRatio(0.9, 1.05);
  canvas->Latex_CMSPre( 2.8, 13 );
  if( histNameBase != "h_mass" )
    canvas->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{60 < M(#mu#mu) < 120 GeV}}");

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

  if( histNameBase == "h_eta_lead" )
  {
    titleX = "#eta(#mu, leading)";
    titleY = "Muon / 0.1";
  }

  if( histNameBase == "h_eta_subLead" )
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