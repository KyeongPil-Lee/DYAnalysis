#include <vector>
#include <iostream>

#include <TPad.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TString.h>
#include <THStack.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TF1.h>

#include <Include/tdrstyle.C>
#include <Include/DYAnalyzer.h>

// -- should be updated -- //
#define Lumi_HLTv4p3_Split1 618.174
#define Lumi_HLTv4p3_Split2 633.070
#define Lumi_HLTv4p3_Split3 715.510

using namespace std;

class DrawControlPlotTool
{
public:
	Double_t MassBinEdges[nMassBin+1];
	Bool_t DrawDataDriven;
	TString NormType;

	TString FileLocation;
	TFile *f_input;
	TFile *f_input_Data;
	TFile *f_input_bkg_dataDriven;
	TFile *f_output;

	vector< TString > HistNames; vector< TString> Variables; vector< TString > XTitles;

	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents; vector< Int_t > color;

	Int_t Nfactor_overall;
	
	DrawControlPlotTool(Bool_t DrawDataDriven_arg, TString NormType_arg);
	virtual void SetupHistogramNames();
	virtual void GenLevelMassSpectrum();
	virtual void LoopForHistograms(Int_t nHist);

// protected:
	virtual Double_t Entry_Zpeak(TH1D *h);
	virtual Double_t Calc_NormFactor();
	virtual void NormalizationToLumi( vector< TH1D* > h_MC, TString Variable );
	virtual void RebinHistograms( TH1D &h_data, vector< TH1D* > &h_MC, TString Variable );
	virtual void StoreYieldHistogram( TH1D* h_data, vector< TH1D* > h_bkgs, TString Type );
	virtual void DrawMassHistogram_DataDrivenBkg(TString Type, TH1D *h_data, vector< TH1D* > h_MC);
	virtual void SetLegendPosition( TString Variable, Double_t &xlow, Double_t &ylow, Double_t &xhigh, Double_t &yhigh);
	virtual void DrawCanvas( TString Variable, TH1D* h_data, TH1D* h_pred, THStack *hs, TLegend *legend, TString XTitle);
	virtual void SetAxisRange( TString Variable, TH1D* h_data);
	virtual void SetRatioRangeLabelSize( TString Variable, Double_t &ratio_min, Double_t &ratio_max, Double_t &LabelSize );
	virtual void ChangeHistError_StatOnlyError(TH1D *h_mass, TH1D *h_StatUnc);
	virtual void DrawBkgRatioPlot( TString Type, TH1D* h_data, vector<TH1D*> h_bkgs, vector<TString> Names );
	virtual TH1D* MakeMassHistogram( TString HLTType, TString Type );
};

DrawControlPlotTool::DrawControlPlotTool(Bool_t DrawDataDriven_arg, TString NormType_arg)
{
	if( !(NormType_arg == "Lumi" || NormType_arg == "Zpeak") )
	{
		cout << NormType_arg << " is not allowed type ... Possible NormType = Lumi, Zpeak" << endl;
		return;
	}

	DrawDataDriven = DrawDataDriven_arg;
	NormType = NormType_arg;

	setTDRStyle();
	gROOT->SetStyle( "tdrStyle" );
	// tdrStyle->SetErrorX(0.5);

	Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};

	for(Int_t i=0; i<nMassBin+1; i++)
		MassBinEdges[i] = MassBinEdges_temp[i];

	// -- Get root file containing the histograms -- //
	FileLocation = GetBasePath() + "/Include/Results_ROOTFiles_76X/";

	f_input = new TFile(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root");
	f_input_Data = new TFile(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root");
	
	// -- output file -- //
	f_output = new TFile("ROOTFile_YieldHistogram.root", "RECREATE");

	DYAnalyzer *analyzer = new DYAnalyzer( "None" );
	analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_NNLOxSecForDY", &ntupleDirectory, &Tag, &Xsec, &nEvents); // -- 76X -- //

	// -- Set the colors for each sample -- //
	for(Int_t i=0; i<(Int_t)Tag.size(); i++ )
	{
		if( Tag[i] == "ZZ" || Tag[i] == "WZ" || Tag[i] == "WW" )
			color.push_back( kGreen );
		else if( Tag[i] == "WJets" )
			color.push_back( kBlue );
		else if( Tag[i].Contains("DYTauTau") )
			color.push_back( kBlue-9 );
		else if( Tag[i].Contains("DYMuMu") )
			color.push_back( kOrange );
		else if( Tag[i].Contains("ttbar") )
			color.push_back( kRed );
	}

	Nfactor_overall = 1;
	if( NormType == "Zpeak" )
		Nfactor_overall = this->Calc_NormFactor();

	TH1::AddDirectory(0);
}

void DrawControlPlotTool::SetupHistogramNames()
{
	HistNames.push_back( "h_mass_OS" );			Variables.push_back( "OSMass_DYBin" );		XTitles.push_back( "Invariant Mass(Opposite Sign) [GeV]");
	HistNames.push_back( "h_mass_OS_HLTv4p2" );				Variables.push_back( "OSMass_DYBin_HLTv4p2" );		XTitles.push_back( "Invariant Mass(Opposite Sign) [GeV] (HLTv4.2)");
	HistNames.push_back( "h_mass_OS_HLTv4p3" );				Variables.push_back( "OSMass_DYBin_HLTv4p3" );		XTitles.push_back( "Invariant Mass(Opposite Sign) [GeV] (HLTv4.3)");
	
	HistNames.push_back( "h_lead_Pt" );			Variables.push_back( "LeadPt" );			XTitles.push_back( "Leading Muon P_{T} [GeV]");

	HistNames.push_back( "h_barrel_Pt" );		Variables.push_back( "BarrelPt" );			XTitles.push_back( "Muon(|#eta|<0.9) P_{T} [GeV]");
	HistNames.push_back( "h_barrel_eta" );		Variables.push_back( "BarrelEta" );			XTitles.push_back( "Muon(|#eta|<0.9) #eta");
	HistNames.push_back( "h_barrel_phi" );		Variables.push_back( "BarrelPhi" );			XTitles.push_back( "Muon(|#eta|<0.9) #phi");

	HistNames.push_back( "h_endcap_Pt" );		Variables.push_back( "EndcapPt" );			XTitles.push_back( "Muon(|#eta|>0.9) P_{T} [GeV]");
	HistNames.push_back( "h_endcap_eta" );		Variables.push_back( "EndcapEta" );			XTitles.push_back( "Muon(|#eta|>0.9) #eta");
	HistNames.push_back( "h_endcap_phi" );		Variables.push_back( "EndcapPhi" );			XTitles.push_back( "Muon(|#eta|>0.9) #phi");

	HistNames.push_back( "h_Angle" );			Variables.push_back( "Angle" );				XTitles.push_back( "Angle between two muons");
	HistNames.push_back( "h_nVertices_before" );			Variables.push_back( "nVertice_beforePUweight" );			XTitles.push_back( "Number of Vertices (AfterSel, NoPUReWeight) [GeV]");
	HistNames.push_back( "h_nVertices_after" );			Variables.push_back( "nVertice_afterPUweight" );			XTitles.push_back( "Number of Vertices (AfterSel, PUReWeight) [GeV]");
	HistNames.push_back( "h_pixelHits" );			Variables.push_back( "PixelHits" );			XTitles.push_back( "# Pixel Hits");
	HistNames.push_back( "h_trackerLayers" );		Variables.push_back( "TrackerLayers" );		XTitles.push_back( "# Tracker Layers");
	HistNames.push_back( "h_muonHits" );			Variables.push_back( "MuonHits" );			XTitles.push_back( "# Muon Hits");
	HistNames.push_back( "h_nMatches" );			Variables.push_back( "nMatches" );			XTitles.push_back( "# Matched Stations");

	HistNames.push_back( "h_eta" );				Variables.push_back( "Eta" );				XTitles.push_back( "Muon #eta");

	
	HistNames.push_back( "h_mass_OS" );			Variables.push_back( "OSMass_M60to120" );		XTitles.push_back( "Invariant Mass(Opposite Sign) [GeV]");
	HistNames.push_back( "h_mass_OS_HLTv4p2" );				Variables.push_back( "OSMass_DYBin_HLTv4p2" );		XTitles.push_back( "Invariant Mass(Opposite Sign) [GeV] (HLTv4.2)");
	HistNames.push_back( "h_mass_OS_HLTv4p3" );				Variables.push_back( "OSMass_DYBin_HLTv4p3" );		XTitles.push_back( "Invariant Mass(Opposite Sign) [GeV] (HLTv4.3)");

	HistNames.push_back( "h_mass_OS_BB" );				Variables.push_back( "OSMass_DYBin_BB" );		XTitles.push_back( "Invariant Mass(Opposite Sign, BB) [GeV]");
	HistNames.push_back( "h_mass_OS_BE" );				Variables.push_back( "OSMass_DYBin_BE" );		XTitles.push_back( "Invariant Mass(Opposite Sign, BE) [GeV]");
	HistNames.push_back( "h_mass_OS_EE" );				Variables.push_back( "OSMass_DYBin_EE" );		XTitles.push_back( "Invariant Mass(Opposite Sign, EE) [GeV]");

	HistNames.push_back( "h_mass_OS_BB" );				Variables.push_back( "OSMass_M60to120_BB" );		XTitles.push_back( "Invariant Mass(Opposite Sign, BB) [GeV]");
	HistNames.push_back( "h_mass_OS_BE" );				Variables.push_back( "OSMass_M60to120_BE" );		XTitles.push_back( "Invariant Mass(Opposite Sign, BE) [GeV]");
	HistNames.push_back( "h_mass_OS_EE" );				Variables.push_back( "OSMass_M60to120_EE" );		XTitles.push_back( "Invariant Mass(Opposite Sign, EE) [GeV]");

	HistNames.push_back( "h_mass_OS_HLTv4p2_BB" );			Variables.push_back( "OSMass_DYBin_HLTv4p2_BB" );		XTitles.push_back( "Invariant Mass(OS, BB) [GeV] (HLTv4.2)");
	HistNames.push_back( "h_mass_OS_HLTv4p2_BE" );			Variables.push_back( "OSMass_DYBin_HLTv4p2_BE" );		XTitles.push_back( "Invariant Mass(OS, BE) [GeV] (HLTv4.2)");
	HistNames.push_back( "h_mass_OS_HLTv4p2_EE" );			Variables.push_back( "OSMass_DYBin_HLTv4p2_EE" );		XTitles.push_back( "Invariant Mass(OS, EE) [GeV] (HLTv4.2)");
	
	HistNames.push_back( "h_mass_OS_HLTv4p3_BB" );			Variables.push_back( "OSMass_DYBin_HLTv4p3_BB" );		XTitles.push_back( "Invariant Mass(OS, BB) [GeV] (HLTv4.3)");
	HistNames.push_back( "h_mass_OS_HLTv4p3_BE" );			Variables.push_back( "OSMass_DYBin_HLTv4p3_BE" );		XTitles.push_back( "Invariant Mass(OS, BE) [GeV] (HLTv4.3)");
	HistNames.push_back( "h_mass_OS_HLTv4p3_EE" );			Variables.push_back( "OSMass_DYBin_HLTv4p3_EE" );		XTitles.push_back( "Invariant Mass(OS, EE) [GeV] (HLTv4.3)");

	HistNames.push_back( "h_mass_OS_HLTv4p3_Split1" );		Variables.push_back( "OSMass_DYBin_HLTv4p3_Split1" );		XTitles.push_back( "Invariant Mass(OS) [GeV] (HLTv4.3, Split1)");
	HistNames.push_back( "h_mass_OS_HLTv4p3_Split2" );		Variables.push_back( "OSMass_DYBin_HLTv4p3_Split2" );		XTitles.push_back( "Invariant Mass(OS) [GeV] (HLTv4.3, Split2)");
	HistNames.push_back( "h_mass_OS_HLTv4p3_Split3" );		Variables.push_back( "OSMass_DYBin_HLTv4p3_Split3" );		XTitles.push_back( "Invariant Mass(OS) [GeV] (HLTv4.3, Split3)");



	HistNames.push_back( "h_Pt" );				Variables.push_back( "Pt" );				XTitles.push_back( "Muon P_{T} [GeV]");
	
	HistNames.push_back( "h_phi" );				Variables.push_back( "Phi" );				XTitles.push_back( "Muon #phi");

	HistNames.push_back( "h_RelTrkIso" );			Variables.push_back( "RelTrkIso" );			XTitles.push_back( "TrkIso / P_{T}");
	
	
	HistNames.push_back( "h_RelPtError" );			Variables.push_back( "RelPtError" );		XTitles.push_back( "P_{T} Error / P_{T}");
	HistNames.push_back( "h_dxyVTX" );				Variables.push_back( "dxyVTX" );			XTitles.push_back( "dxy(PV) [cm]");
	HistNames.push_back( "h_dzVTX" );				Variables.push_back( "dzVTX" );				XTitles.push_back( "dz(PV) [cm]");
	HistNames.push_back( "h_VtxNormChi2" );						Variables.push_back( "VtxNormChi2" );					XTitles.push_back( "Vertex Normalized #chi^{2}");
	
	



	HistNames.push_back( "h_absdiRap_M20to30" );	Variables.push_back( "absdiRap_M20to30" );		XTitles.push_back( "Dimuon Rapidity (20 < M < 30 [GeV])");
	HistNames.push_back( "h_absdiRap_M30to45" );	Variables.push_back( "absdiRap_M30to45" );		XTitles.push_back( "Dimuon Rapidity (30 < M < 45 [GeV])");
	HistNames.push_back( "h_absdiRap_M45to60" );	Variables.push_back( "absdiRap_M45to60" );		XTitles.push_back( "Dimuon Rapidity (45 < M < 60 [GeV])");
	HistNames.push_back( "h_absdiRap_M60to120" );	Variables.push_back( "absdiRap_M60to120" );		XTitles.push_back( "Dimuon Rapidity (60 < M < 120 [GeV])");
	HistNames.push_back( "h_absdiRap_M120to200" );	Variables.push_back( "absdiRap_M120to200" );		XTitles.push_back( "Dimuon Rapidity (120 < M < 200 [GeV])");
	HistNames.push_back( "h_absdiRap_M200to1500" );	Variables.push_back( "absdiRap_M200to1500" );		XTitles.push_back( "Dimuon Rapidity (200 < M < 1500 [GeV])"); // -- nHist = 8 -- //

	HistNames.push_back( "h_diPt" );			Variables.push_back( "diPt" );				XTitles.push_back( "dimuon P_{T} [GeV]");
	HistNames.push_back( "h_diRap" );			Variables.push_back( "diRapidity" );		XTitles.push_back( "dimuon Rapidity");

	HistNames.push_back( "h_VtxProb" );						Variables.push_back( "VtxProb" );					XTitles.push_back( "Vertex Probability");
	HistNames.push_back( "h_VtxProb_belowM600" );			Variables.push_back( "VtxProb_belowM600" );			XTitles.push_back( "Vertex Probability (M < 600 GeV)");
	HistNames.push_back( "h_VtxProb_overM600" );			Variables.push_back( "VtxProb_overM600" );			XTitles.push_back( "Vertex Probability (M > 600 GeV)");

	HistNames.push_back( "h_VtxNormChi2" );						Variables.push_back( "VtxNormChi2" );					XTitles.push_back( "Vertex Normalized #chi^{2}");
	HistNames.push_back( "h_VtxNormChi2_belowM600" );			Variables.push_back( "VtxNormChi2_belowM600" );			XTitles.push_back( "Vertex Normalized #chi^{2} (M < 600 GeV)");
	HistNames.push_back( "h_VtxNormChi2_overM600" );			Variables.push_back( "VtxNormChi2_overM600" );			XTitles.push_back( "Vertex Normalized #chi^{2} (M > 600 GeV)");

	HistNames.push_back( "h_Pt_M70to90" );		Variables.push_back( "M70to90Pt" );			XTitles.push_back( "Muon P_{T} (70 < M < 90) [GeV]");
	HistNames.push_back( "h_Pt_M90to110" );		Variables.push_back( "M90to110Pt" );		XTitles.push_back( "Muon P_{T} (90 < M < 110) [GeV]");
	HistNames.push_back( "h_Pt_M110toInf" );	Variables.push_back( "M110toInfPt" );		XTitles.push_back( "Muon P_{T} (M > 110) [GeV]");

	HistNames.push_back( "h_lead_Pt" );			Variables.push_back( "LeadPt" );			XTitles.push_back( "Leading Muon P_{T} [GeV]");
	HistNames.push_back( "h_lead_eta" );		Variables.push_back( "LeadEta" );			XTitles.push_back( "Leading Muon #eta");
	HistNames.push_back( "h_lead_phi" );		Variables.push_back( "LeadPhi" );			XTitles.push_back( "Leading Muon #phi");
	HistNames.push_back( "h_sub_Pt" );			Variables.push_back( "SubPt" );				XTitles.push_back( "sub-leading Muon P_{T} [GeV]");
	HistNames.push_back( "h_sub_eta" );			Variables.push_back( "SubEta" );			XTitles.push_back( "sub-leading Muon #eta");
	HistNames.push_back( "h_sub_phi" );			Variables.push_back( "SubPhi" );			XTitles.push_back( "sub-leading Muon #phi");
	HistNames.push_back( "h_Pt_minusCharge" );	Variables.push_back( "MinusChargePt" );		XTitles.push_back( "Muon(mu^{-}) P_{T} [GeV]");
	HistNames.push_back( "h_Pt_plusCharge" );	Variables.push_back( "PlusChargePt" );		XTitles.push_back( "Muon(mu^{+}) P_{T} [GeV]");
	HistNames.push_back( "h_Pt_TrigLeg" );		Variables.push_back( "TrigMatchedPt" );		XTitles.push_back( "Muon(Matched with Trigger) P_{T} [GeV]");
	HistNames.push_back( "h_eta_TrigLeg" );		Variables.push_back( "TrigMatchedEta" );	XTitles.push_back( "Muon(Matched with Trigger) #eta");
	HistNames.push_back( "h_phi_TrigLeg" );		Variables.push_back( "TrigMatchedPhi" );	XTitles.push_back( "Muon(Matched with Trigger) #phi");
	HistNames.push_back( "h_Pt_OtherLeg" );		Variables.push_back( "TrigNOTMatchedPt" );	XTitles.push_back( "Muon(NOT Matched with Trigger) P_{T} [GeV]");
	HistNames.push_back( "h_eta_OtherLeg" );	Variables.push_back( "TrigNOTMatchedEta" );	XTitles.push_back( "Muon(NOT Matched with Trigger) #eta");
	HistNames.push_back( "h_phi_OtherLeg" );	Variables.push_back( "TrigNOTMatchedPhi" );	XTitles.push_back( "Muon(NOT Matched with Trigger) #phi");

	HistNames.push_back( "h_mass" );			Variables.push_back( "Mass" );				XTitles.push_back( "Invariant Mass(#mu#mu) [GeV]");
	// HistNames.push_back( "h_mass_barrel" );		Variables.push_back( "BarrelMass" );		XTitles.push_back( "Invariant Mass(|muon #eta|<0.9) [GeV]");
	HistNames.push_back( "h_mass_SS" );			Variables.push_back( "SSMass" );			XTitles.push_back( "Invariant Mass(Same Sign) [GeV]");


	HistNames.push_back( "h_Pt_M70" );			Variables.push_back( "M70Pt" );				XTitles.push_back( "Muon P_{T} (M < 70) [GeV]");

	
}

Double_t DrawControlPlotTool::Calc_NormFactor()
{
	// -- Get histograms: data -- //
	f_input_Data->cd();
	TH1D *h_data = (TH1D*)f_input_Data->Get( "h_mass_OS_Data" )->Clone();

	// -- Get histograms: MC -- //
	vector< TH1D* > h_MC;
	f_input->cd();
	Int_t nTag = Tag.size();
	for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		h_MC.push_back( (TH1D*)f_input->Get( "h_mass_OS_"+Tag[i_tag] )->Clone() );

	NormalizationToLumi( h_MC, "h_mass_OS");

	Double_t EntryZ_data = Entry_Zpeak( h_data );
	Double_t EntryZ_MC = 0;
	Int_t nMC = h_MC.size();
	for(Int_t i_MC=0; i_MC<nMC; i_MC++)
	{
		Double_t EntryZ = Entry_Zpeak( h_MC[i_MC] );
		cout << "[" << Tag[i_MC] << "] " <<"\tEntryZ(overall Nfactor is not applied): " << EntryZ << endl;

		EntryZ_MC += EntryZ;
	}

	Double_t Nfactor = EntryZ_data / EntryZ_MC;

	printf("(EntryZ_MC, EntryZ_Data, Nfactor) = (%12.1lf, %12.1lf, %.6lf)\n", EntryZ_MC, EntryZ_data, Nfactor);

	return Nfactor;
}

void DrawControlPlotTool::NormalizationToLumi( vector< TH1D* > h_MC, TString Variable )
{
	//////////////////////////////
	// -- Set the luminosity -- //
	//////////////////////////////
	Double_t Luminosity = Lumi;

	if( Variable.Contains("HLTv4p2") )
		Luminosity = Lumi_HLTv4p2;
	else if( Variable.Contains("HLTv4p3") )
	{
		if( Variable.Contains("Split1") )
			Luminosity = Lumi_HLTv4p3_Split1;
		
		else if( Variable.Contains("Split2") )
			Luminosity = Lumi_HLTv4p3_Split2;
		
		else if( Variable.Contains("Split3") )
			Luminosity = Lumi_HLTv4p3_Split3;

		else // -- Total HLTv4.3 luminosity -- //
			Luminosity = Lumi - Lumi_HLTv4p2;
	}
	cout << "Variable: " << Variable << ", Luminosity: " << Luminosity << endl;

	/////////////////////////
	// -- Normalization -- //
	/////////////////////////
	Int_t nMC = h_MC.size();
	for(Int_t i=0; i<nMC; i++)
	{
		Double_t Norm = (Luminosity * Xsec[i]) / nEvents[i];
		// cout << "[Sample: " << Tag[i] << "] Normalization factor to Integrated Luminosity " << Lumi << "/pb: " << Norm << endl;
		h_MC[i]->Scale( Norm );
	}
}

void DrawControlPlotTool::LoopForHistograms(Int_t nHist)
{
	Int_t nLoopHist = nHist;
	Int_t nTotalHist = (Int_t)HistNames.size();
	if( nHist == -1 ) nLoopHist = nTotalHist;
	if( nHist > nTotalHist )
	{
		cout << "nHist > nTotalHist! ... " << nHist << " > " << nTotalHist << endl;
		return;
	}

	for(Int_t i_hist=0; i_hist<nLoopHist; i_hist++)
	{
		// -- Get a histogram: Data -- //
		f_input_Data->cd();
		TH1D *h_data = (TH1D*)f_input_Data->Get( HistNames[i_hist]+"_Data" )->Clone();

		// -- Get histograms: MC -- //
		vector< TH1D* > h_MC;
		f_input->cd();
		Int_t nTag = Tag.size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
			h_MC.push_back( (TH1D*)f_input->Get( HistNames[i_hist]+"_"+Tag[i_tag] )->Clone() );

		this->NormalizationToLumi( h_MC, Variables[i_hist] );

		this->RebinHistograms( *h_data, h_MC, Variables[i_hist] );

		// -- Check the underflow & overflow -- //
		Double_t UnderFlow = h_data->GetBinContent(0);
		Double_t OverFlow = h_data->GetBinContent( h_data->GetNbinsX() + 1 );
		if( UnderFlow != 0 || OverFlow != 0 )
		{
			cout << endl;
			cout << "\tVariable: " << Variables[i_hist] << endl;
			cout << "\t(UnderFlow, OverFlow) = " << "(" << UnderFlow << ", " << OverFlow << ")" << endl;
			cout << endl;
		}

		////////////////////////////////////////////////////////////////////////////////////////
		// -- Store yield histogram && Draw mass distribution (MC-based, data-driven bkg.) -- //
		////////////////////////////////////////////////////////////////////////////////////////
		if( Variables[i_hist] == "OSMass_DYBin" || Variables[i_hist] == "OSMass_DYBin_HLTv4p2" || Variables[i_hist] == "OSMass_DYBin_HLTv4p3" )
		{
			vector< TH1D* > h_bkgs;
			for(Int_t i_tag=0; i_tag<nTag; i_tag++)
			{
				if( !Tag[i_tag].Contains("DYMuMu") ) 
					h_bkgs.push_back( h_MC[i_tag] );
			}
			this->StoreYieldHistogram( h_data, h_bkgs, "MCBasedBkg" );

			if( DrawDataDriven == kTRUE )
			{
				TString Type = "All";
				if( Variables[i_hist] == "OSMass_DYBin_HLTv4p2" ) Type = "HLTv4p2";
				else if( Variables[i_hist] == "OSMass_DYBin_HLTv4p3" ) Type = "HLTv4p3";
				this->DrawMassHistogram_DataDrivenBkg(Type, h_data, h_MC);
			}
		}


		//////////////////////////////////////////
		// -- Set attributes: Data Histogram -- //
		//////////////////////////////////////////
		h_data->SetStats(kFALSE);
		h_data->SetLineColor(kBlack);
		h_data->SetMarkerStyle(20);
		h_data->SetMarkerColor(kBlack);
		h_data->SetMarkerSize(0.5);
		h_data->SetFillColorAlpha(kWhite, 0);

		/////////////////////////////////////////////////////////
		// -- Make MC HStack & Set attributes: MC Histogram -- //
		/////////////////////////////////////////////////////////u
		THStack *hs = new THStack("hs_"+Variables[i_hist], "");

		Int_t nMC = h_MC.size();
		for(Int_t i_MC=0; i_MC<nMC; i_MC++)
		{
			if( NormType == "Zpeak" )
				h_MC[i_MC]->Scale( Nfactor_overall );

			h_MC[i_MC]->SetLineColor( color[i_MC] );
			h_MC[i_MC]->SetFillColor( color[i_MC] );
			h_MC[i_MC]->SetMarkerColor( color[i_MC] );

			hs->Add( h_MC[i_MC] );
		}

		//////////////////////////
		// -- Set the legend -- //
		//////////////////////////
		Double_t xlow = 0.75, ylow = 0.70, xhigh = 0.99, yhigh = 0.94;
		this->SetLegendPosition( Variables[i_hist], xlow, ylow, xhigh, yhigh );

		TLegend *legend = new TLegend(xlow, ylow, xhigh, yhigh);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->AddEntry(h_data, "Data");
		for(Int_t i_MC=nMC-1; i_MC>=0; i_MC--)
		{
			if( Tag[i_MC] == "ttbar" )
				legend->AddEntry(h_MC[i_MC], "ttbar" );
			else if( Tag[i_MC] == "ZZ" )
				legend->AddEntry(h_MC[i_MC], "Diboson" );
			else if( Tag[i_MC] == "WJets" )
				legend->AddEntry(h_MC[i_MC], "WJets" );
			else if( Tag[i_MC] == "DYMuMu_M10to50" )
				legend->AddEntry(h_MC[i_MC], "DYMuMu" );
			else if( Tag[i_MC] == "DYTauTau" )
				legend->AddEntry(h_MC[i_MC], "DYTauTau" );
		}

		// -- Sum of all prediction: for ratio plot -- //
		TH1D *h_totMC = NULL;
		for(Int_t i_MC=0; i_MC<nMC; i_MC++)
		{
			if( h_totMC == NULL )
				h_totMC = (TH1D*)h_MC[i_MC]->Clone();
			else
				h_totMC->Add( h_MC[i_MC] );
		}

		this->DrawCanvas( Variables[i_hist], h_data, h_totMC, hs, legend, XTitles[i_hist]);
	}
}

void DrawControlPlotTool::RebinHistograms( TH1D& h_data, vector< TH1D* > &h_MC, TString Variable )
{
	Int_t nMC = (Int_t)h_MC.size();
	if( Variable == "OSMass") // -- Mass Bin size: 5GeV -- //
	{
		h_data.Rebin(5);
		for(Int_t i=0; i<nMC; i++)
			h_MC[i]->Rebin(5);
	}
	else if( Variable.Contains("OSMass_M60to120") ) // -- Mass Bin size: 2GeV -- //
	{
		h_data.Rebin(2);
		for(Int_t i=0; i<nMC; i++)
			h_MC[i]->Rebin(2);
	}
	else if( Variable.Contains("OSMass_DYBin") ) // -- Mass Bin: Follow DY Mass binning @ 13TeV -- //
	{
		h_data = *(TH1D*)h_data.Rebin(nMassBin, h_data.GetName(), MassBinEdges);
		for(Int_t i=0; i<nMC; i++)
			h_MC[i] = (TH1D*)h_MC[i]->Rebin(nMassBin, h_MC[i]->GetName(), MassBinEdges);
	}
	else if( Variable.Contains("Pt") ) // -- Mass Bin: Follow DY Mass binning @ 13TeV -- //
	{
		h_data.Rebin(2);
		for(Int_t i=0; i<nMC; i++)
			h_MC[i]->Rebin(2);
	}
	else if( Variable == "VtxNormChi2" )
	{
		h_data.Rebin(5);
		for(Int_t i=0; i<nMC; i++)
			h_MC[i]->Rebin(5);
	}
}

void DrawControlPlotTool::DrawBkgRatioPlot( TString Type, TH1D* h_data, vector<TH1D*> h_bkgs, vector<TString> Names )
{
	///////////////////////////////////////////
	// -- Make total background histogram -- //
	///////////////////////////////////////////
	TH1D *h_totBkg = NULL;
	TH1D* h_FR = NULL;
	TH1D* h_ttbarlike = NULL;
	TH1D* h_ZZWZ = NULL;
	TH1D* h_DYTauTau = NULL;

	Int_t nBkg = (Int_t)h_bkgs.size();
	for(Int_t i_bkg=0; i_bkg<nBkg; i_bkg++)
	{
		TH1D* h_temp = h_bkgs[i_bkg];

		// -- total backgrounds -- //
		if( h_totBkg == NULL )
			h_totBkg = (TH1D*)h_temp->Clone();
		else
			h_totBkg->Add( h_temp );

		// -- fake rate -- //
		if( Names[i_bkg] == "DiJet" || Names[i_bkg] == "WJets" )
		{
			if( h_FR == NULL )
				h_FR = (TH1D*)h_temp->Clone();
			else
				h_FR->Add( h_temp );
		}

		// -- ttbar-like bkg -- //
		else if( Names[i_bkg] == "ttbar" || Names[i_bkg] == "tW" || Names[i_bkg] == "WW" )
		{
			if( h_ttbarlike == NULL )
				h_ttbarlike = (TH1D*)h_temp->Clone();
			else
				h_ttbarlike->Add( h_temp );
		}

		// -- WZ and ZZ -- //
		else if( Names[i_bkg] == "WZ" || Names[i_bkg] == "ZZ" )
		{
			if( h_ZZWZ == NULL )
				h_ZZWZ = (TH1D*)h_temp->Clone();
			else
				h_ZZWZ->Add( h_temp );
		}

		// -- DYtautau -- //
		else if( Names[i_bkg] == "DYTauTau" )
			h_DYTauTau = (TH1D*)h_temp->Clone();

	}
	h_totBkg->Sumw2();
	h_FR->Sumw2();
	h_ttbarlike->Sumw2();
	h_ZZWZ->Sumw2();
	h_DYTauTau->Sumw2();

	////////////////////////////////
	// -- Make the ratio plots -- //
	////////////////////////////////
	TH1D* h_BkgRatio_totBkg = (TH1D*)h_data->Clone(); h_BkgRatio_totBkg->Sumw2();
	h_BkgRatio_totBkg->Divide(h_totBkg, h_data);

	TH1D* h_BkgRatio_FR = (TH1D*)h_data->Clone(); h_BkgRatio_FR->Sumw2();
	h_BkgRatio_FR->Divide(h_FR, h_data);

	TH1D* h_BkgRatio_ttbarlike = (TH1D*)h_data->Clone(); h_BkgRatio_ttbarlike->Sumw2();
	h_BkgRatio_ttbarlike->Divide(h_ttbarlike, h_data);

	TH1D* h_BkgRatio_ZZWZ = (TH1D*)h_data->Clone(); h_BkgRatio_ZZWZ->Sumw2();
	h_BkgRatio_ZZWZ->Divide(h_ZZWZ, h_data);

	TH1D* h_BkgRatio_DYTauTau = (TH1D*)h_data->Clone(); h_BkgRatio_DYTauTau->Sumw2();
	h_BkgRatio_DYTauTau->Divide(h_DYTauTau, h_data);

	///////////////////////
	// -- make canvas -- //
	///////////////////////
	TString CanvasName = "c_BkgRatio_DataDrivenBkg_"+Type;
	TCanvas *c = new TCanvas(CanvasName, "", 800, 600);
	c->cd();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetGridx(kFALSE);
	gPad->SetGridy(kFALSE);
	gPad->SetLeftMargin(0.11);
	gPad->SetRightMargin(0.02);
	gPad->SetTopMargin(0.07);
	gPad->SetBottomMargin(0.13);

	// -- draw -- //
	h_BkgRatio_totBkg->Draw("EP");
	h_BkgRatio_FR->Draw("EPSAME");
	h_BkgRatio_ttbarlike->Draw("EPSAME");
	h_BkgRatio_ZZWZ->Draw("EPSAME");
	h_BkgRatio_DYTauTau->Draw("EPSAME");

	// -- make legend & add the histograms -- //
	TLegend *legend = new TLegend(0.7,0.15,0.95,0.4);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	legend->AddEntry( h_BkgRatio_totBkg, "Total");
	legend->AddEntry( h_BkgRatio_FR, "QCD + WJets");
	legend->AddEntry( h_BkgRatio_ttbarlike, "t#bar{t} + tW + WW");
	legend->AddEntry( h_BkgRatio_ZZWZ, "ZZ + WZ");
	legend->AddEntry( h_BkgRatio_DYTauTau, "DY#rightarrow#tau#tau");
	legend->Draw();

	// -- axis settings -- //
	h_BkgRatio_totBkg->SetMinimum(1e-5);
	h_BkgRatio_totBkg->SetMaximum(1.0);
	h_BkgRatio_totBkg->GetYaxis()->SetLabelSize(0.05);
	h_BkgRatio_totBkg->GetYaxis()->SetTitle("Fraction of backgrounds");
	h_BkgRatio_totBkg->GetYaxis()->SetTitleSize(0.045);
	h_BkgRatio_totBkg->GetYaxis()->SetTitleOffset(1.25);
	

	h_BkgRatio_totBkg->GetXaxis()->SetMoreLogLabels();
	h_BkgRatio_totBkg->GetXaxis()->SetNoExponent();
	h_BkgRatio_totBkg->GetXaxis()->SetTitle( "m [GeV]" );
	h_BkgRatio_totBkg->GetXaxis()->SetTitleOffset( 1.2 );
	h_BkgRatio_totBkg->GetXaxis()->SetTitleSize( 0.05 );
	h_BkgRatio_totBkg->GetXaxis()->SetLabelColor(1);
	h_BkgRatio_totBkg->GetXaxis()->SetLabelFont(42);
	h_BkgRatio_totBkg->GetXaxis()->SetLabelOffset(0.007);
	h_BkgRatio_totBkg->GetXaxis()->SetLabelSize(0.05);
	h_BkgRatio_totBkg->GetXaxis()->SetRangeUser(15, 5000);
	// 



	// -- attributes for each histogram -- //
	h_BkgRatio_totBkg->SetMarkerStyle(20);
	h_BkgRatio_totBkg->SetMarkerSize(1);
	h_BkgRatio_totBkg->SetMarkerColor(kBlack);
	h_BkgRatio_totBkg->SetLineColor(kBlack);
	h_BkgRatio_totBkg->SetFillColorAlpha(kWhite, 0);

	h_BkgRatio_FR->SetMarkerStyle(21);
	h_BkgRatio_FR->SetMarkerSize(1);
	h_BkgRatio_FR->SetMarkerColor(kMagenta+2);
	h_BkgRatio_FR->SetLineColor(kMagenta+2);
	h_BkgRatio_FR->SetFillColorAlpha(kWhite, 0);

	h_BkgRatio_ttbarlike->SetMarkerStyle(22);
	h_BkgRatio_ttbarlike->SetMarkerSize(1);
	h_BkgRatio_ttbarlike->SetMarkerColor(kRed);
	h_BkgRatio_ttbarlike->SetLineColor(kRed);
	h_BkgRatio_ttbarlike->SetFillColorAlpha(kWhite, 0);

	h_BkgRatio_ZZWZ->SetMarkerStyle(23);
	h_BkgRatio_ZZWZ->SetMarkerSize(1);
	h_BkgRatio_ZZWZ->SetMarkerColor(kGreen);
	h_BkgRatio_ZZWZ->SetLineColor(kGreen);
	h_BkgRatio_ZZWZ->SetFillColorAlpha(kWhite, 0);

	h_BkgRatio_DYTauTau->SetMarkerStyle(29);
	h_BkgRatio_DYTauTau->SetMarkerSize(1);
	h_BkgRatio_DYTauTau->SetMarkerColor(kBlue-9);
	h_BkgRatio_DYTauTau->SetLineColor(kBlue-9);
	h_BkgRatio_DYTauTau->SetFillColorAlpha(kWhite, 0);

	TLatex latex;
	latex.SetTextSize(0.03);
	latex.DrawLatexNDC(0.83, 0.94, "2.8 fb^{#font[122]{\55}1} (13 TeV)");
	latex.SetTextSize(0.035);
	latex.DrawLatexNDC(0.14, 0.88, "CMS Preliminary");

	c->SaveAs(CanvasName+".pdf");
	// c->SaveAs(CanvasName+".C");
}

void DrawControlPlotTool::StoreYieldHistogram( TH1D* h_data, vector< TH1D* > h_bkgs, TString Type )
{
	TH1D *h_totBkg = NULL;

	Int_t nBkg = (Int_t)h_bkgs.size();
	for(Int_t i_bkg=0; i_bkg<nBkg; i_bkg++)
	{
		if( h_totBkg == NULL )
			h_totBkg = (TH1D*)h_bkgs[i_bkg]->Clone();
		else
			h_totBkg->Add( h_bkgs[i_bkg] );
	}
	h_totBkg->Sumw2();

	TString HistoName = h_data->GetName();
	HistoName.ReplaceAll("mass", "yield");
	HistoName.ReplaceAll("_Data", "_"+Type);

	TH1D *h_yield = (TH1D*)h_data->Clone();
	h_yield->SetName( HistoName );
	h_yield->Sumw2();

	// -- Background subtraction -- //
	h_yield->Add( h_totBkg, -1 );

	// -- Remove negative bins -- //
	for(Int_t i=0; i<h_yield->GetNbinsX(); i++)
	{
		Double_t content = h_yield->GetBinContent(i+1);

		if( content < 0 )
			h_yield->SetBinContent(i+1, 0);
	}

	f_output->cd();
	h_yield->Write();
}

void DrawControlPlotTool::DrawMassHistogram_DataDrivenBkg(TString Type, TH1D *h_data, vector< TH1D* > h_MC)
{
	///////////////////////////////////////////////
	// -- Get the histograms obtained from MC -- //
	///////////////////////////////////////////////
	TH1D *h_SignalMC = NULL;

	Int_t nMC = (Int_t)h_MC.size();
	for(Int_t i_MC=0; i_MC<nMC; i_MC++)
	{
		if( Tag[i_MC].Contains("DYMuMu") ) // -- Signal MC samples -- //
		{
			if( h_SignalMC == NULL )
				h_SignalMC = (TH1D*)h_MC[i_MC]->Clone("h_mass_DYMC_"+Type);
			else
				h_SignalMC->Add( h_MC[i_MC] );
		}
	}

	////////////////////////////////////////////////////////////////
	// -- Bring the histograms estimated by data-driven method -- //
	////////////////////////////////////////////////////////////////
	f_input_bkg_dataDriven = new TFile(FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root");
	TH1D *h_diJet_FR = (TH1D*)f_input_bkg_dataDriven->Get("dijet")->Clone();
	TH1D *h_WJets_FR = (TH1D*)f_input_bkg_dataDriven->Get("wjets")->Clone();
	TH1D *h_ttbar_emu = (TH1D*)f_input_bkg_dataDriven->Get("ttbar")->Clone();
	TH1D *h_tW_emu = (TH1D*)f_input_bkg_dataDriven->Get("tW")->Clone();
	TH1D *h_DYTauTau_emu = (TH1D*)f_input_bkg_dataDriven->Get("DYtautau")->Clone();
	TH1D *h_WW_emu = (TH1D*)f_input_bkg_dataDriven->Get("WW")->Clone();

	TH1D *h_WZ = this->MakeMassHistogram(Type, "WZ");
	TH1D *h_ZZ = this->MakeMassHistogram(Type, "ZZ");

	f_input_bkg_dataDriven->cd();
	TH1D *h_StatUnc_diJet_FR = (TH1D*)f_input_bkg_dataDriven->Get("dijet_stat")->Clone();
	TH1D *h_StatUnc_WJets_FR = (TH1D*)f_input_bkg_dataDriven->Get("wjets_stat")->Clone();
	TH1D *h_StatUnc_ttbar_emu = (TH1D*)f_input_bkg_dataDriven->Get("ttbar_stat")->Clone();
	TH1D *h_StatUnc_tW_emu = (TH1D*)f_input_bkg_dataDriven->Get("tW_stat")->Clone();
	TH1D *h_StatUnc_DYTauTau_emu = (TH1D*)f_input_bkg_dataDriven->Get("DYtautau_stat")->Clone();
	TH1D *h_StatUnc_WW_emu = (TH1D*)f_input_bkg_dataDriven->Get("WW_stat")->Clone();

	ChangeHistError_StatOnlyError(h_diJet_FR, h_StatUnc_diJet_FR);
	ChangeHistError_StatOnlyError(h_WJets_FR, h_StatUnc_WJets_FR);
	ChangeHistError_StatOnlyError(h_ttbar_emu, h_StatUnc_ttbar_emu);
	ChangeHistError_StatOnlyError(h_tW_emu, h_StatUnc_tW_emu);
	ChangeHistError_StatOnlyError(h_DYTauTau_emu, h_StatUnc_DYTauTau_emu);
	ChangeHistError_StatOnlyError(h_WW_emu, h_StatUnc_WW_emu);

	if( Type == "HLTv4p2" || Type == "HLTv4p3" )
	{
		Double_t NormFactor = 0;
		if( Type == "HLTv4p2" )
			NormFactor = Lumi_HLTv4p2 / Lumi;
		else if( Type == "HLTv4p3" )
			NormFactor = (Lumi - Lumi_HLTv4p2) / Lumi;

		h_diJet_FR->Scale( NormFactor );
		h_WJets_FR->Scale( NormFactor );
		h_ttbar_emu->Scale( NormFactor );
		h_tW_emu->Scale( NormFactor );
		h_DYTauTau_emu->Scale( NormFactor );
		h_WW_emu->Scale( NormFactor );
	}

	vector< TH1D* > StackHistos; vector< TString > LegendNames; vector< Int_t > colors;
	StackHistos.push_back( h_diJet_FR ); LegendNames.push_back( "QCD (FR)" ); colors.push_back(kMagenta+2);
	StackHistos.push_back( h_WW_emu ); LegendNames.push_back( "WW (e#mu)" ); colors.push_back(kGreen);
	StackHistos.push_back( h_WZ ); LegendNames.push_back( "WZ" ); colors.push_back(kGreen+2);
	StackHistos.push_back( h_ZZ ); LegendNames.push_back( "ZZ" ); colors.push_back(kGreen+4);
	StackHistos.push_back( h_WJets_FR ); LegendNames.push_back( "WJets (FR)" ); colors.push_back(kBlue);
	StackHistos.push_back( h_tW_emu ); LegendNames.push_back( "tW (e#mu)" ); colors.push_back(kCyan);
	StackHistos.push_back( h_DYTauTau_emu ); LegendNames.push_back( "DYTauTau (e#mu)" ); colors.push_back(kBlue-9);
	StackHistos.push_back( h_ttbar_emu ); LegendNames.push_back( "ttbar (e#mu)" ); colors.push_back(kRed);
	StackHistos.push_back( h_SignalMC ); LegendNames.push_back( "DYMuMu" ); colors.push_back(kOrange);

	//////////////////////////////////////////
	// -- Set attributes: Data Histogram -- //
	//////////////////////////////////////////
	h_data->SetStats(kFALSE);
	h_data->SetLineColor(kBlack);
	h_data->SetMarkerStyle(20);
	h_data->SetMarkerColor(kBlack);
	h_data->SetMarkerSize(0.5);
	h_data->SetFillColorAlpha(kWhite, 0);

	/////////////////////////////////////////////////////////
	// -- Make MC HStack & Set attributes: MC Histogram -- //
	/////////////////////////////////////////////////////////
	THStack *hs = new THStack("hs_OSMass_DYBin_DataDrivenBkg_"+Type, "");

	Int_t nStackHists = (Int_t)StackHistos.size();
	for(Int_t iter=0; iter<nStackHists; iter++)
	{
		StackHistos[iter]->SetLineColor( colors[iter] );
		StackHistos[iter]->SetFillColor( colors[iter] );
		StackHistos[iter]->SetMarkerColor( colors[iter] );
		StackHistos[iter]->SetMarkerSize( 0 );

		hs->Add( StackHistos[iter] );
	}

	//////////////////////////
	// -- Set the legend -- //
	//////////////////////////
	Double_t xlow = 0.75, ylow = 0.70, xhigh = 0.99, yhigh = 0.94;
	this->SetLegendPosition( "OSMass_DYBin_DataDrivenBkg", xlow, ylow, xhigh, yhigh );

	TLegend *legend = new TLegend(xlow, ylow, xhigh, yhigh);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	legend->AddEntry(h_data, "Data");
	for(Int_t iter=nStackHists-1; iter>=0; iter--)
		legend->AddEntry(StackHistos[iter], LegendNames[iter] );

	// -- Sum of all prediction: for ratio plot -- //
	TH1D *h_pred = NULL;
	for(Int_t iter=0; iter<nStackHists; iter++)
	{
		if( h_pred == NULL )
			h_pred = (TH1D*)StackHistos[iter]->Clone();
		else
			h_pred->Add( StackHistos[iter] );
	}

	this->DrawCanvas( "OSMass_DYBin_"+Type+"_DataDrivenBkg", h_data, h_pred, hs, legend, "Invariant Mass(Opposite Sign, "+Type+") [GeV]");

	// -- Store yield histogram -- //
	vector< TH1D* > h_bkgs; vector< TString > Names;

	h_bkgs.push_back( h_diJet_FR ); Names.push_back("DiJet");
	h_bkgs.push_back( h_WW_emu ); Names.push_back("WW");
	h_bkgs.push_back( h_WZ ); Names.push_back("WZ");
	h_bkgs.push_back( h_ZZ ); Names.push_back("ZZ");
	h_bkgs.push_back( h_WJets_FR ); Names.push_back("WJets");
	h_bkgs.push_back( h_tW_emu ); Names.push_back("tW");
	h_bkgs.push_back( h_DYTauTau_emu ); Names.push_back("DYTauTau");
	h_bkgs.push_back( h_ttbar_emu ); Names.push_back("ttbar");
	
	this->StoreYieldHistogram( h_data, h_bkgs, "DataDrivenBkg" );
	this->DrawBkgRatioPlot( Type, h_data, h_bkgs, Names );

	f_output->cd();
	h_SignalMC->Write();

	TFile *f_output2 = TFile::Open("ROOTFile_Histograms_DimuonMassSpectrum_"+Type+".root", "RECREATE");
	f_output2->cd();

	h_data->SetName("h_data");
	h_data->Write();

	h_SignalMC->SetName("h_SignalMC");
	h_SignalMC->Write();

	h_diJet_FR->SetName("h_diJet_FR");
	h_diJet_FR->Write();

	h_WJets_FR->SetName("h_WJets_FR");
	h_WJets_FR->Write();

	h_WZ->SetName("h_WZ");
	h_WZ->Write();

	h_ZZ->SetName("h_ZZ");
	h_ZZ->Write();

	h_WW_emu->SetName("h_WW_emu");
	h_WW_emu->Write();

	h_tW_emu->SetName("h_tW_emu");
	h_tW_emu->Write();

	h_DYTauTau_emu->SetName("h_DYTauTau_emu");
	h_DYTauTau_emu->Write();

	h_ttbar_emu->SetName("h_ttbar_emu");
	h_ttbar_emu->Write();

	delete f_output2;
}

TH1D* DrawControlPlotTool::MakeMassHistogram( TString HLTType, TString Type )
{
	TString FileName = FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root";
	TFile *f_MCBkg = TFile::Open( FileName ); f_MCBkg->cd();

	TString HistName = "";
	if( (HLTType == "HLTv4p2") || (HLTType == "HLTv4p3") )
		HistName = "h_mass_OS_"+HLTType+"_Norm_"+Type;
	else
		HistName = "h_mass_OS_Norm_"+Type;

	TH1D* h_mass = (TH1D*)f_MCBkg->Get( HistName )->Clone();
	h_mass = (TH1D*)h_mass->Rebin(nMassBin, h_mass->GetName(), MassBinEdges);

	delete f_MCBkg;

	// Int_t nTag = (Int_t)Tag.size();
	// for(Int_t i_tag=0; i_tag<nTag; i_tag++)
	// {
	// 	printf("(Tag, Type) = (%s, %s)\n", Tag[i_tag].Data(), Type.Data() );

	// 	if( Tag[i_tag] == Type )
	// 	{
	// 		Double_t Luminosity = Lumi;
	// 		if( HLTType == "HLTv4p2" )
	// 			Luminosity = Lumi_HLTv4p2;
	// 		else if( HLTType == "HLTv4p3" )
	// 			Luminosity = Lumi - Lumi_HLTv4p2;

	// 		Double_t Norm = (Lumi * Xsec[i_tag] ) / nEvents[i_tag];
	// 		printf( "[Normalization factor of %s (HLT Type = %s)] = %lf\n", Tag[i_tag].Data(), HLTType.Data(), Norm );
	// 		h_mass->Scale( Norm );
	// 		break;
	// 	}
	// }

	return h_mass;
}

void DrawControlPlotTool::SetLegendPosition( TString Variable, Double_t &xlow, Double_t &ylow, Double_t &xhigh, Double_t &yhigh)
{
	if( Variable == "Pt" )
		ylow = 0.60;

	else if( Variable == "absdiRap_M60to120" || Variable == "OSMass_M60to120" || Variable.Contains("Eta") || Variable.Contains("Phi") || Variable.Contains("Rapidity"))
	{
		xlow = 0.85; ylow = 0.77; 
		xhigh = 0.99; yhigh = 0.94;
	}
	else if( Variable.Contains("OSMass_DYBin") )
	{
		ylow = 0.60;
		xhigh = 0.95;
	}
}

void DrawControlPlotTool::DrawCanvas( TString Variable, TH1D* h_data, TH1D* h_pred, THStack *hs, TLegend *legend, TString XTitle)
{
	TCanvas *c = new TCanvas( "c_"+Variable, "", 800, 600 );

    c->cd(1);
    ///////////////////
    // -- Top Pad -- //
    ///////////////////
    TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.01,0.99,0.99);
    c1_1->Draw();
    c1_1->cd();
    c1_1->UseCurrentStyle();
    c1_1->SetRightMargin(0.045);
    c1_1->SetBottomMargin(0.3);
    c1_1->SetLogy();

    h_data->GetXaxis()->SetLabelSize(0.000);
    h_data->GetYaxis()->SetTitle("Entry");
    // -- I don't know why TDR style is not applied to Y-axis Title size & Offiset
    h_data->GetYaxis()->SetTitleSize(0.06);
    h_data->GetYaxis()->SetTitleOffset(1.25);
    h_data->GetYaxis()->SetLabelSize(0.05);

    // -- Adjust Y-axis range automatically by calculating maximum value of the histogram -- //
    Double_t MaxValue_Data = h_data->GetBinContent(h_data->GetMaximumBin());
    Double_t MaxValue_totMC = h_pred->GetBinContent(h_pred->GetMaximumBin());
    Double_t MaxValue = 0;
    if( MaxValue_Data > MaxValue_totMC ) MaxValue = MaxValue_Data;
    else MaxValue = MaxValue_totMC;

    Double_t UpperEdge = MaxValue * 4;
    h_data->GetYaxis()->SetRangeUser(0.05, UpperEdge);

    // -- Set the axis range by hand -- //
    this->SetAxisRange( Variable, h_data );
   	
    h_data->SetTitle("");

    TH1D* h_format = (TH1D*)h_data->Clone();
    h_format->Reset("ICES");
    h_format->Draw("");
    hs->Draw("histsame");
    h_data->Draw("EPsame");
    h_format->Draw("axissame");
    legend->Draw();

    //////////////////////
    // -- Bottom Pad -- //
    //////////////////////
    TPad *c1_2 = new TPad("padc1_2","padc1_2",0.01,0.01,0.99,0.3);
    c1_2->Draw();
    c1_2->cd();

    c1_2->SetBottomMargin(0.4);
    c1_2->SetRightMargin(0.035);
    c1_2->SetLeftMargin(0.15);	

    c1_2->SetFillStyle(0);
    c1_2->SetGrid();

    //Make ratio plot
    TH1D *h_ratio = (TH1D*)h_data->Clone();
    h_data->Sumw2(); //h_pred->Sumw2();
    h_ratio->Divide(h_data, h_pred);
    h_ratio->SetTitle("");
    h_ratio->GetXaxis()->SetMoreLogLabels();
    h_ratio->GetXaxis()->SetNoExponent();
    h_ratio->GetXaxis()->SetTitle( XTitle );
    h_ratio->GetXaxis()->SetTitleOffset( 0.9 );
    h_ratio->GetXaxis()->SetTitleSize( 0.2 );
    h_ratio->GetXaxis()->SetLabelColor(1);
    h_ratio->GetXaxis()->SetLabelFont(42);
    h_ratio->GetXaxis()->SetLabelOffset(0.007);
    h_ratio->GetXaxis()->SetLabelSize(0.15);

    h_ratio->GetYaxis()->SetTitle( "data/Pred." );
    h_ratio->GetYaxis()->SetTitleOffset( 0.4 );
    h_ratio->GetYaxis()->SetTitleSize( 0.17);
    
    
    h_ratio->SetMarkerStyle(20);
    h_ratio->SetMarkerSize(0.7);
    h_ratio->SetMarkerColor(kBlack);

    Double_t ratio_max = 1.5;
    Double_t ratio_min = 0.5;
    Double_t LabelSize = 0.11;
    this->SetRatioRangeLabelSize( Variable, ratio_min, ratio_max, LabelSize );

    h_ratio->SetMaximum( ratio_max );
    h_ratio->SetMinimum( ratio_min );
    h_ratio->GetYaxis()->SetLabelSize( LabelSize );

    h_ratio->Draw("e1p");

    // -- flat line = 1.00 -- //
    TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
    f_line->SetLineColor(kRed);
    f_line->SetLineWidth(1);
    f_line->Draw("SAME");	    

    if( Variable.Contains("diPt")  ) { c1_1->SetLogx(); c1_2->SetLogx(); }
    // if( Variable == "OSMass" || Variable == "OSMass_DYBin" ) { c1_1->SetLogx(); c1_2->SetLogx(); }
    if( Variable.Contains("OSMass_DYBin") ) { c1_1->SetLogx(); c1_2->SetLogx(); }

	// f_output->cd();
	// c->Write();
	c->SaveAs("c_"+Variable+".pdf");

}

void DrawControlPlotTool::SetAxisRange( TString Variable, TH1D* h_data)
{
	if( Variable.Contains("OSMass_M60to120") ) { h_data->GetXaxis()->SetRangeUser(60, 120); }
	if( Variable == "Angle" ) { h_data->GetXaxis()->SetRangeUser(0, 4); }
	if( Variable.Contains("Pt") ) { h_data->GetXaxis()->SetRangeUser(0, 1000); }
	// if( Variable.Contains("VtxProb") ||  Variable.Contains("VtxNormChi2") ) { h_data->GetYaxis()->SetRangeUser(5e-3, 1e7); }
	// if( Variable.Contains("Pt")  ) { h_data->GetXaxis()->SetRangeUser(0, 250); h_data->GetYaxis()->SetRangeUser(0.5, 1000000);}
	// if( Variable.Contains("Eta")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 5000000); }
	// if( Variable.Contains("Phi")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 5000000); }
	// if( Variable.Contains("Mass") ) { h_data->GetXaxis()->SetRangeUser(10, 3000); h_data->GetYaxis()->SetRangeUser(0.005, 10000000);}
	
	// if( Variable.Contains("DYBin") ) { h_data->GetXaxis()->SetRangeUser(15, 3000); h_data->GetYaxis()->SetRangeUser(0.005, 10000000);}
	// if( Variable == "OSMass" ) { h_data->GetXaxis()->SetRangeUser(15, 3000); h_data->GetYaxis()->SetRangeUser(0.005, 10000000);}
	// if( Variable.Contains("Angle")  ) { h_data->GetXaxis()->SetRangeUser(0, 4); h_data->GetYaxis()->SetRangeUser(0.5, 1000000); }
	// if( Variable.Contains("Rapidity")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 300000); }
	// if( Variable.Contains("absdiRap")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 500000); }
	// if( Variable.Contains("absdiRap_M60to120")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 2000000); }
	// if( Variable.Contains("SSMass")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 10); }
}

void DrawControlPlotTool::SetRatioRangeLabelSize( TString Variable, Double_t &ratio_min, Double_t &ratio_max, Double_t &LabelSize )
{
	if( Variable.Contains("OSMass_M60to120") || Variable == "diRapidity" || Variable.Contains("Eta") || Variable.Contains("Phi") || Variable == "diPt" )
	{
		ratio_max = 1.2; ratio_min = 0.8; LabelSize = 0.09;
	}
	else if ( Variable.Contains("Pt") )
	{
		ratio_max = 1.2; ratio_min = 0.8;
	}
	else if( Variable.Contains("OSMass_DYBin") || Variable.Contains("absdiRap") ||  Variable == "OSMass" || Variable.Contains("VtxProb") || Variable.Contains("VtxNormChi2") )
	{
		ratio_max = 1.3; ratio_min = 0.7;
	}
}

Double_t DrawControlPlotTool::Entry_Zpeak(TH1D *h)
{
	Double_t Entry_Z = 0;
	Int_t nBins = h->GetNbinsX();
	for(Int_t i=0; i<nBins; i++)
	{
		Double_t i_bin = i+1;
		Double_t BinCenter = h->GetBinCenter(i_bin);
		if( BinCenter > 60 && BinCenter < 120 )
			Entry_Z += h->GetBinContent(i_bin);
	}

	return Entry_Z;
}

void DrawControlPlotTool::GenLevelMassSpectrum()
{
	f_input->cd();
	// -- Setting colors for each histogram -- //
	vector< Int_t > Colors;
	for(Int_t i=1; i<=9; i++)
		Colors.push_back( i );
	Colors.push_back( kOrange );
	Colors.push_back( kViolet );

	// -- Get Dimuon mass distribution from each sample -- //
	vector< TH1D* > h_MC;
	vector< TString > HistTag;
	Int_t nTag = Tag.size();
	for(Int_t i_tag=0; i_tag<nTag; i_tag++)
	{
		
		if( Tag[i_tag].Contains("DYMuMu") ) // -- signal sample -- //
		{
			TH1D *h_temp = NULL;
			h_temp = (TH1D*)f_input->Get( "h_GenMass_"+Tag[i_tag] )->Clone();

			if( h_temp == NULL )
			{
				cout << "No Histograms corresponding to " << Tag[i_tag] << endl;
				return;
			}

			h_temp->Rebin(5);
			Double_t Norm = (Lumi * Xsec[i_tag]) / nEvents[i_tag];
			h_temp->Scale( Norm );

			h_MC.push_back( h_temp );
			HistTag.push_back( Tag[i_tag] );

			// cout << "[Sample: " << Tag[i] << "] Normalization factor to Integrated Luminosity " << Lumi << "/pb: " << Norm << endl;
			
		}
	}

	Int_t nMC = (Int_t)h_MC.size();
	Int_t nColors = (Int_t)Colors.size();
	if( nMC > nColors )
	{
		printf("[ERROR! ... # histograms > # colors!. (nMC, nColors) = (%d, %d)\n", nMC, nColors);
		return;
	}

	THStack *hs = new THStack("hs", "");
	TLegend *legend = new TLegend(0.55, 0.63, 0.95, 0.95);
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);

	for(Int_t i_MC=nMC-1; i_MC>=0; i_MC--)
	{
		h_MC[i_MC]->SetFillColor( Colors[i_MC] );
		h_MC[i_MC]->SetLineColor( Colors[i_MC] );
		h_MC[i_MC]->SetMarkerColor( Colors[i_MC] );
		legend->AddEntry( h_MC[i_MC], HistTag[i_MC] );

		hs->Add( h_MC[i_MC] );
	}

	TString CanvasName = "c_GenMass_SignalMC";
	TCanvas *c_hs = new TCanvas(CanvasName, "", 800, 800);
	c_hs->cd();
	gPad->SetLogy();
	gPad->SetLogx();
	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);

	TH1D* h_format = (TH1D*)h_MC[0]->Clone();
	h_format->Reset("ICES");
	h_format->GetXaxis()->SetRangeUser(10, 4000);
	h_format->GetYaxis()->SetRangeUser(1e-4, 1e8);
	h_format->GetXaxis()->SetNoExponent();
	h_format->GetXaxis()->SetMoreLogLabels();
	h_format->GetXaxis()->SetTitle("Gen-Level Dimuon Mass (isHardProcess) [GeV]");
	h_format->GetYaxis()->SetTitle("# Events");
	h_format->GetYaxis()->SetTitleOffset(1.4);

	h_format->Draw("");
	hs->Draw("histsame");
	h_format->Draw("axissame");
	legend->Draw();

	c_hs->SaveAs(CanvasName+".pdf");
}

void DrawControlPlotTool::ChangeHistError_StatOnlyError(TH1D *h_mass, TH1D *h_StatUnc)
{
	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t StatUnc = h_StatUnc->GetBinContent(i_bin);
		h_mass->SetBinError(i_bin, StatUnc);
	}
}

