// -- Class for common functions used in DY differential cross section measurement analysis @ 13 TeV -- //
// -- Author: KyoengPil Lee, 05 Dec. 2015 -- //
// -- 2016.10.24 (v01): add SeparateDYLLSample_LHEInfo & Include Object_v01.h and NtupleHandle_v01.h
// -- 2016.11.04 (v02): include v02 headers, SeparateDYLLSample_LHEInfo -> add DYEE case
#pragma once

#include <Include/Object.h>
#include <Include/NtupleHandle.h>

#include <TSystem.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

#define Lumi 2759.017 
#define Lumi_HLTv4p2 843.404 // -- integrated luminosity before Run 257933 -- //
#define Lumi_Elec 2257.998 // -- Golden JSON -- //
#define nMassBin 43


TString GetBasePath()
{
	TString BasePath = gSystem->Getenv("KP_ANALYZER_PATH");
	return BasePath + "/";
}

class DYAnalyzer
{
public:

	TString Path_CommonCodes;
	TString HLT;
	Double_t LeadPtCut;
	Double_t SubPtCut;
	Double_t LeadEtaCut;
	Double_t SubEtaCut;

	Double_t PileUpWeight[52];

	Double_t Eff_RecoID_data[5][4];
	Double_t Eff_RecoID_MC[5][4];

	Double_t Eff_Iso_data[5][4];
	Double_t Eff_Iso_MC[5][4];

	Double_t Eff_HLTv4p2_data[5][4];
	Double_t Eff_HLTv4p2_MC[5][4];

	Double_t Eff_HLTv4p3_data[5][4];
	Double_t Eff_HLTv4p3_MC[5][4];

	// -- Constructor -- //
	DYAnalyzer(TString HLTname);

	// -- Setup accetpance cuts -- //
	void AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut);

	////////////////////////////
	// -- Setup MC samples -- //
	////////////////////////////
	void SetupMCsamples_v20160412_76X_MINIAODv2_CheckPremix( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
	void SetupMCsamples_v20160309_76X_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
	void SetupMCsamples_v20160131_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
	void SetupMCsamples_v20160117_MiniAOD_JetMET( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
	Bool_t SeparateDYLLSample_isHardProcess(TString Tag, NtupleHandle *ntuple);
	Bool_t SeparateDYLLSample_LHEInfo(TString Tag, NtupleHandle *ntuple);

	// -- outdated -- //
	Bool_t SeparateDYLLSample(TString Tag, NtupleHandle *ntuple);

	//////////////////////////////////
	// -- Setup pileup weighting -- //
	//////////////////////////////////
	void SetupPileUpReWeighting( Bool_t isMC );
	Double_t PileUpWeightValue(Int_t PileUp_MC);

	// -- for 76X -- //
	void SetupPileUpReWeighting_76X( Bool_t isMC );
	Double_t PileUpWeightValue_76X(Int_t PileUp_MC);


	/////////////////////////////////////////
	// -- Setup Efficiency scale factor -- //
	/////////////////////////////////////////
	void SetupEfficiencyScaleFactor();
	void SetupEfficiencyScaleFactor(TString ROOTFileName);
	Double_t EfficiencySF_EventWeight_HLTv4p2(Muon mu1, Muon mu2);
	Double_t EfficiencySF_EventWeight_HLTv4p3(Muon mu1, Muon mu2);
	Int_t FindPtBin(Double_t Pt);
	Int_t FindEtaBin(Double_t eta);

	// -- outdated -- //
	Double_t EfficiencySF_EventWeight(Muon mu1, Muon mu2, NtupleHandle *ntuple);
	Double_t EfficiencySF_EventWeight_RecoIdIso(Muon mu1, Muon mu2);
	
	////////////////////////////
	// -- Event Selections -- //
	////////////////////////////
	Bool_t EventSelection(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Mu50(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_minusDimuonVtxCut(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Zdiff_13TeV(vector< Muon > MuonCollection, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Dijet(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Wjet(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_CheckMoreThanOneDimuonCand(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection, Bool_t& isMoreThanOneCand); // -- output: 2 muons passing event selection conditions -- //

	Bool_t isPassAccCondition_Muon(Muon Mu1, Muon Mu2);
	Bool_t isPassAccCondition_GenLepton(GenLepton genlep1, GenLepton genlep2);
	void CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu);
	void CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep);
	void DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2);

	// -- for electron channel - //
	Bool_t EventSelection_Electron(vector< Electron > ElectronCollection, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t EventSelection_ElectronChannel_NminusPFIso(vector< Electron > ElectronCollection, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t EventSelection_ElectronChannel(vector< Electron > ElectronCollection, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t isPassAccCondition_Electron(Electron Elec1, Electron Elec2);
	Bool_t isPassAccCondition_GenLepton_ECALGAP(GenLepton genlep1, GenLepton genlep2);
	void CompareElectron(Electron *Elec1, Electron *Elec2, Electron *leadElec, Electron *subElec);

	// -- pre-FSR functions -- //
	void PostToPreFSR_byDressedLepton(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection);
	void PostToPreFSR_byDressedLepton_AllPhotons(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection);
	TString DecideFSRType(GenLepton preFSR1, GenLepton preFSR2, GenLepton postFSR1, GenLepton postFSR2);
	Double_t Calc_dR_GenLeptons( GenLepton genlep1, GenLepton genlep2 );
	Double_t Calc_dR_GenLepton_GenOthers( GenLepton genlep1, GenOthers genlep2 );

	// -- miscellaneous -- //
	void GenMatching(TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection);
	void ConvertToTunePInfo( Muon &mu );
	void PrintOutDoubleMuInfo( Muon mu1, Muon mu2 );

	void MakeTChain_fromTextFile(TChain *chain, TString FileName);
};

DYAnalyzer::DYAnalyzer(TString HLTname)
{
	if( HLTname == "None" )
	{
		cout << "===================================================" << endl;
		cout << "[No specific trigger setting ... basic constructor]" << endl;
		cout << "===================================================" << endl;
		
		HLT = "None";
		LeadPtCut = 9999;
		SubPtCut = 9999;
		LeadEtaCut = 9999;
		SubEtaCut = 9999;
	}
	else
	{
		this->AssignAccThreshold(HLTname, &HLT, &LeadPtCut, &SubPtCut, &LeadEtaCut, &SubEtaCut);
		cout << "===========================================================" << endl;
		cout << "Trigger: " << HLT << endl;
		cout << "leading lepton pT Cut: " << LeadPtCut << endl;
		cout << "Sub-leading lepton pT Cut: " << SubPtCut << endl;
		cout << "leading lepton Eta Cut: " << LeadEtaCut << endl;
		cout << "sub-leading lepton Eta Cut: " << SubEtaCut << endl;
		cout << "===========================================================" << endl;
	}

	this->Path_CommonCodes = GetBasePath() + TString::Format("%s", "Include/");
	printf("[Path_CommonCodes = %s]\n", Path_CommonCodes.Data() );
}

void DYAnalyzer::MakeTChain_fromTextFile( TChain *chain, TString FileName)
{
	ifstream openFile( FileName.Data() );
	cout << "===============================" << endl;
	cout << "Read " << FileName << endl;
	cout << "===============================" << endl;
	if( openFile.is_open() )
	{
		string line;
		while(getline(openFile, line))
		{
			cout << line << endl;
			chain->Add(line.data());
		}
		openFile.close();
	}

	cout << "==================================" << endl;
	cout << "All ROOT Files are put into TChain" << endl;
	cout << "==================================" << endl;
}

void DYAnalyzer::AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut)
{
	if( HLTname == "IsoMu20" )
	{
		*HLT = "HLT_IsoMu20_v*";
		*LeadPtCut = 22;
		*SubPtCut = 10;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
	else if( HLTname == "IsoMu20_OR_IsoTkMu20" )
	{
		*HLT = "HLT_IsoMu20_v* || HLT_IsoTkMu20_v*";
		*LeadPtCut = 22;
		*SubPtCut = 10;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
	else if( HLTname == "IsoMu20_OR_IsoTkMu20_HighPtCut" )
	{
		*HLT = "HLT_IsoMu20_v* || HLT_IsoTkMu20_v*";
		*LeadPtCut = 30;
		*SubPtCut = 10;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
	else if( HLTname == "Mu45_eta2p1" )
	{
		*HLT = "HLT_Mu45_eta2p1_v*";
		*LeadPtCut = 46;
		*SubPtCut = 10;
		*LeadEtaCut = 2.1;
		*SubEtaCut = 2.4;
	}
	else if( HLTname == "Mu50" )
	{
		*HLT = "HLT_Mu50_v*";
		*LeadPtCut = 53;
		*SubPtCut = 10;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
	else if( HLTname == "IsoMu20_SymmetricPt25" )
	{
		*HLT = "HLT_IsoMu20_v*";
		*LeadPtCut = 25;
		*SubPtCut = 25;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
	else if( HLTname == "Ele17Ele12" )
	{
		*HLT = "HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*";
		*LeadPtCut = 25;
		*SubPtCut = 15;
		*LeadEtaCut = 2.5;
		*SubEtaCut = 2.5;
	}
	else if( HLTname == "Ele22_eta2p1" )
	{
		*HLT = "HLT_Ele22_eta2p1_WPLoose_Gsf_v*"; // -- Exist only for the data; "HLT_Ele22_eta2p1_WP75_Gsf_v*" should be used for MC
		*LeadPtCut = 25;
		*SubPtCut = 15;
		*LeadEtaCut = 2.1;
		*SubEtaCut = 2.1;
	}
	else if( HLTname == "Ele22_eta2p1_NoEtaCut" )
	{
		*HLT = "HLT_Ele22_eta2p1_WPLoose_Gsf_v*"; // -- Exist only for the data; "HLT_Ele22_eta2p1_WP75_Gsf_v*" should be used for MC
		*LeadPtCut = 25;
		*SubPtCut = 15;
		*LeadEtaCut = 2.5;
		*SubEtaCut = 2.5;
	}
	else if( HLTname == "Pt_30_10_eta_2p5" )
	{
		*HLT = "None"; // -- just for acceptance test -- //
		*LeadPtCut = 30;
		*SubPtCut = 10;
		*LeadEtaCut = 2.5;
		*SubEtaCut = 2.5;
	}
	else if( HLTname == "Ele23_WPLoose" )
	{
		*HLT = "HLT_Ele23_WPLoose_Gsf_v*"; // -- Exist only for the data; "HLT_Ele22_eta2p1_WP75_Gsf_v*" should be used for MC
		*LeadPtCut = 30;
		*SubPtCut = 10;
		*LeadEtaCut = 2.5;
		*SubEtaCut = 2.5;
	}
	else
	{ 
		cout << "Wrong HLT name!: " << HLTname << endl;
		return; 
	}

}

void DYAnalyzer::SetupMCsamples_v20160412_76X_MINIAODv2_CheckPremix( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents )
{
	if( Type == "DYMuMu_PU25" )
	{
		ntupleDirectory->push_back( "Premix/v20160412_76X_MINIAODv2_CheckPremix_CorrectDataSetName_DYJets_Classic_PU25" ); Tag->push_back( "DYMuMu_M50_PU25_Classic" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 8426438.0 );
		ntupleDirectory->push_back( "Premix/v20160412_76X_MINIAODv2_CheckPremix_CorrectDataSetName_DYJets_NonDeterministic_PU25" ); Tag->push_back( "DYMuMu_M50_PU25_NonDet" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 8286714.0 );
	}
	else if( Type == "DYEE_PU25" )
	{
		ntupleDirectory->push_back( "Premix/v20160412_76X_MINIAODv2_CheckPremix_CorrectDataSetName_DYJets_Classic_PU25" ); Tag->push_back( "DYEE_M50_PU25_Classic" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 8439605.0 );
		ntupleDirectory->push_back( "Premix/v20160412_76X_MINIAODv2_CheckPremix_CorrectDataSetName_DYJets_NonDeterministic_PU25" ); Tag->push_back( "DYEE_M50_PU25_NonDet" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 8300442.0 );
	}
}

void DYAnalyzer::SetupMCsamples_v20160309_76X_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents )
{
	if( Type == "Full" )
	{
		// cout << "# events should be adjusted later" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 988416 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16520811.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7467514.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6309713.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 97994304 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6302525.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "aMCNLO" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6302525.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "M100to200" )
	{
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
	}
	else if( Type == "M50to200" )
	{
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6302525.0 ); //nEvents: sum of DYMuMu weights
	}
	else if( Type == "M50toInf" )
	{
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50toInf" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6311695.0 );
	}
	else if( Type == "M10to50_M50toInf" )
	{
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50toInf" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6311695.0 );
	}
	else if( Type == "aMCNLO_M120Cut" )
	{
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to120" ); Xsec->push_back( 1975 ); nEvents->push_back( 6243307.0 );
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M120to200" ); Xsec->push_back( 19.32 ); nEvents->push_back( 55554.0 );
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 2.731 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights
	}
	else if( Type == "Full_Include_M100to200" )
	{
		// cout << "# events should be adjusted later" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 988416 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16520811.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7467514.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6309713.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 97994304 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 6061181.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "Full_NoHighMass" )
	{
		// cout << "# events should be adjusted later" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 988416 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16520811.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7467514.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6309713.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 97994304 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50toInf" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6311695.0 ); // -- sum of weight should be updated! -- //
	}
	else if( Type == "Full_Powheg" )
	{
		// cout << "# events should be adjusted later" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 988416 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16520811.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7467514.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6309713.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 97994304 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160519_76X_MINIAODv2_Resubmit4_AdjustRunTime_ZMuMuPowheg_M50to120_25ns" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back(2971982.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M120to200_25ns" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back(99999.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M200to400_25ns" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back(99999.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M400to800_25ns" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back(99600.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M800to1400_25ns" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back(97600.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M1400to2300_25ns" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back(99200.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M2300to3500_25ns" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160525_76X_MINIAODv2_Resubmit_HighMass_ZMuMuPowheg_M3500to4500_25ns" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160525_76X_MINIAODv2_Resubmit_HighMass_ZMuMuPowheg_M4500to6000_25ns" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M6000toInf_25ns" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back(99200.0);
	}
	else if( Type == "Full_M120Cut" )
	{
		// cout << "# events should be adjusted later" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 988416 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16520811.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7467514.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6309713.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 97994304 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to120" ); Xsec->push_back( 1975 ); nEvents->push_back( 6243307.0 );
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M120to200" ); Xsec->push_back( 19.32 ); nEvents->push_back( 55554.0 );
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 2.731 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "aMCNLO_Include_M100to200")
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 6061181.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "Madgraph" )
	{
		ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M5to50_25ns" ); Tag->push_back( "Madgraph_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 631905.0 );
		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M50toInf_25ns" ); Tag->push_back( "Madgraph_M50toInf" ); Xsec->push_back( 6014/3.0 ); nEvents->push_back( 3003455.0 );

		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M5to50_25ns" ); Tag->push_back( "Madgraph_M5to50" ); Xsec->push_back( 7160/3.0 ); nEvents->push_back( 2782834.0 );
		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M50toInf_25ns" ); Tag->push_back( "Madgraph_M50toInf" ); Xsec->push_back( 4895/3.0 ); nEvents->push_back( 3003455.0 );

		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M50toInf_25ns" ); Tag->push_back( "Madgraph_M50to150" ); Xsec->push_back( (4895 - 6.58)/3.0 ); nEvents->push_back( 3003455.0 ); 
		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M150toInf_25ns" ); Tag->push_back( "Madgraph_M150toInf" ); Xsec->push_back( 6.58/3.0 ); nEvents->push_back( 1.0 );

	}
	else if( Type == "MadgraphPowheg" ) // -- for estimation of syst. from unfolding -- //
	{
		ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M5to50_25ns" ); Tag->push_back( "Madgraph_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 631905.0 );
		ntupleDirectory->push_back( "76X/v20160519_76X_MINIAODv2_Resubmit4_AdjustRunTime_ZMuMuPowheg_M50to120_25ns" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back(2971982.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M120to200_25ns" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back(99999.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M200to400_25ns" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back(99999.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M400to800_25ns" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back(99600.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M800to1400_25ns" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back(97600.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M1400to2300_25ns" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back(99200.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M2300to3500_25ns" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160525_76X_MINIAODv2_Resubmit_HighMass_ZMuMuPowheg_M3500to4500_25ns" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160525_76X_MINIAODv2_Resubmit_HighMass_ZMuMuPowheg_M4500to6000_25ns" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M6000toInf_25ns" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back(99200.0);


		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M5to50_25ns" ); Tag->push_back( "Madgraph_M5to50" ); Xsec->push_back( 7160/3.0 ); nEvents->push_back( 2782834.0 );
		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M50toInf_25ns" ); Tag->push_back( "Madgraph_M50toInf" ); Xsec->push_back( 4895/3.0 ); nEvents->push_back( 3003455.0 );

		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M50toInf_25ns" ); Tag->push_back( "Madgraph_M50to150" ); Xsec->push_back( (4895 - 6.58)/3.0 ); nEvents->push_back( 3003455.0 ); 
		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M150toInf_25ns" ); Tag->push_back( "Madgraph_M150toInf" ); Xsec->push_back( 6.58/3.0 ); nEvents->push_back( 1.0 );

	}
	else if( Type == "Powheg" ) // -- for estimation of syst. from unfolding -- //
	{
		ntupleDirectory->push_back( "76X/v20160519_76X_MINIAODv2_Resubmit4_AdjustRunTime_ZMuMuPowheg_M50to120_25ns" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back(2971982.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M120to200_25ns" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back(99999.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M200to400_25ns" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back(99999.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M400to800_25ns" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back(99600.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M800to1400_25ns" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back(97600.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M1400to2300_25ns" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back(99200.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M2300to3500_25ns" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160525_76X_MINIAODv2_Resubmit_HighMass_ZMuMuPowheg_M3500to4500_25ns" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160525_76X_MINIAODv2_Resubmit_HighMass_ZMuMuPowheg_M4500to6000_25ns" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "76X/v20160404_76X_MINIAODv2_ZMuMuPowheg_M6000toInf_25ns" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back(99200.0);


		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M5to50_25ns" ); Tag->push_back( "Madgraph_M5to50" ); Xsec->push_back( 7160/3.0 ); nEvents->push_back( 2782834.0 );
		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M50toInf_25ns" ); Tag->push_back( "Madgraph_M50toInf" ); Xsec->push_back( 4895/3.0 ); nEvents->push_back( 3003455.0 );

		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M50toInf_25ns" ); Tag->push_back( "Madgraph_M50to150" ); Xsec->push_back( (4895 - 6.58)/3.0 ); nEvents->push_back( 3003455.0 ); 
		// ntupleDirectory->push_back( "76X/v20160520_76X_MINIAODv2_Madgraph_LO_M150toInf_25ns" ); Tag->push_back( "Madgraph_M150toInf" ); Xsec->push_back( 6.58/3.0 ); nEvents->push_back( 1.0 );

	}
	else if( Type == "Full_AdditionalSF" )
	{
		// cout << "# events should be adjusted later" << endl;
		// -- Background Samples -- //
		
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 988416 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16520811.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7467514.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6309713.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 97994304 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 6061181.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights

		Double_t SF = 1.033504;
		Int_t nTag = (Int_t)Tag->size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag->at(i_tag).Contains("DYMuMu") && !Tag->at(i_tag).Contains("DYMuMu_M10to50") && !Tag->at(i_tag).Contains("DYMuMu_M50to100") )
			{
				Double_t xSec_before = Xsec->at(i_tag);

				Xsec->at(i_tag) = Xsec->at(i_tag) * SF;

				printf("[Cross section of %s] %lf -> %lf\n", Tag->at(i_tag).Data(), xSec_before, Xsec->at(i_tag) );
			}
		}
		cout << endl;
	}
	else if( Type == "aMCNLO_AdditionalSF" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 6061181.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights

		Double_t SF = 1.033504;
		Int_t nTag = (Int_t)Tag->size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag->at(i_tag).Contains("DYMuMu") && !Tag->at(i_tag).Contains("DYMuMu_M10to50") && !Tag->at(i_tag).Contains("DYMuMu_M50to100") )
			{
				Double_t xSec_before = Xsec->at(i_tag);

				Xsec->at(i_tag) = Xsec->at(i_tag) * SF;

				printf("[Cross section of %s] %lf -> %lf\n", Tag->at(i_tag).Data(), xSec_before, Xsec->at(i_tag) );
			}
		}
		cout << endl;
	}
	else if( Type == "aMCNLO_ee_AdditionalSF" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYEE_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506928.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYEE_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 6058012.0 ); //nEvents: sum of DYEE weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYEE_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 228151.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYEE_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 171446.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYEE_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50421.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYEE_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47674.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYEE_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 45913.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYEE_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 44256.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYEE_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40438.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYEE_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37287.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYEE_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23315.0 ); //nEvents: sum of DYEE weights

		Double_t SF = 1.033504;
		Int_t nTag = (Int_t)Tag->size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag->at(i_tag).Contains("DYEE") && !Tag->at(i_tag).Contains("DYEE_M10to50") && !Tag->at(i_tag).Contains("DYEE_M50to100") )
			{
				Double_t xSec_before = Xsec->at(i_tag);

				Xsec->at(i_tag) = Xsec->at(i_tag) * SF;

				printf("[Cross section of %s] %lf -> %lf\n", Tag->at(i_tag).Data(), xSec_before, Xsec->at(i_tag) );
			}
		}
		cout << endl;
	}
	else if( Type == "aMCNLO_ee_XSecFromRidhi" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYEE_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7506928.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYEE_M50to100" ); Xsec->push_back( 1929.666667 ); nEvents->push_back( 6058012.0 ); //nEvents: sum of DYEE weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYEE_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 228151.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYEE_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 171446.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYEE_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50421.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYEE_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47674.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYEE_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 45913.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYEE_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 44256.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYEE_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40438.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYEE_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37287.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYEE_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23315.0 ); //nEvents: sum of DYEE weights
	}
	else if( Type == "PIBkg" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160714_76X_MINIAODv2_PhotonIndBkg_M15to60" ); Tag->push_back( "PIBkg_M15to60" ); Xsec->push_back( 62.3 ); nEvents->push_back( 9997.0 );
		ntupleDirectory->push_back( "76X/v20160713_76X_MINIAODv2_PhotonIndBkg_M60to500" ); Tag->push_back( "PIBkg_M60to500" ); Xsec->push_back( 4.17 ); nEvents->push_back( 9998.0 );
		ntupleDirectory->push_back( "76X/v20160713_76X_MINIAODv2_PhotonIndBkg_M500toInf" ); Tag->push_back( "PIBkg_M500toInf" ); Xsec->push_back( 0.015 ); nEvents->push_back( 9998.0 );
	}
	else if( Type == "MCBkg" )
	{
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
	}
	else if( Type == "Diboson" )
	{
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 988416 );
	}
	else if( Type == "WJets" )
	{
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16520811.0 ); //nEvents: sum of weights
	}
	else if( Type == "DYTauTau" )
	{
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7467514.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6309713.0 ); //nEvents: sum of DYTauTau weights
	}
	else if( Type == "ttbar" )
	{
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 97994304 );
	}
	else if( Type == "aMCNLO_AdditionalSF_PDFUnc" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7486166.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 6167092.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 33493.0 ); //nEvents: sum of DYMuMu weights

		Double_t SF = 1.033504;
		Int_t nTag = (Int_t)Tag->size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag->at(i_tag).Contains("DYMuMu") && !Tag->at(i_tag).Contains("DYMuMu_M10to50") && !Tag->at(i_tag).Contains("DYMuMu_M50to100") )
			{
				Double_t xSec_before = Xsec->at(i_tag);

				Xsec->at(i_tag) = Xsec->at(i_tag) * SF;

				printf("[Cross section of %s] %lf -> %lf\n", Tag->at(i_tag).Data(), xSec_before, Xsec->at(i_tag) );
			}
		}
		cout << endl;
	}
	else if( Type == "aMCNLO_ee_AdditionalSF_PDFUnc" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M10to50_25ns" ); Tag->push_back( "DYEE_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7486174.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M50toInf_25ns" ); Tag->push_back( "DYEE_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 6163930.0 ); //nEvents: sum of DYEE weights
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M100to200_25ns" ); Tag->push_back( "DYEE_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 228151.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M200to400_25ns" ); Tag->push_back( "DYEE_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 171446.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M400to500_25ns" ); Tag->push_back( "DYEE_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50421.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M500to700_25ns" ); Tag->push_back( "DYEE_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47674.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M700to800_25ns" ); Tag->push_back( "DYEE_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 45913.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M800to1000_25ns" ); Tag->push_back( "DYEE_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 44256.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYEE_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40438.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYEE_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37287.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYEE_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 34121.0 ); //nEvents: sum of DYEE weights

		Double_t SF = 1.033504;
		Int_t nTag = (Int_t)Tag->size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag->at(i_tag).Contains("DYMuMu") && !Tag->at(i_tag).Contains("DYEE_M10to50") && !Tag->at(i_tag).Contains("DYEE_M50to100") )
			{
				Double_t xSec_before = Xsec->at(i_tag);

				Xsec->at(i_tag) = Xsec->at(i_tag) * SF;

				printf("[Cross section of %s] %lf -> %lf\n", Tag->at(i_tag).Data(), xSec_before, Xsec->at(i_tag) );
			}
		}
		cout << endl;
	}
	else if( Type == "aMCNLO_AdditionalSF_LargeN" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M10to50_25ns_LargeN" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 18606204.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M50toInf_25ns_LargeN" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 25987688.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights

		Double_t SF = 1.033504;
		Int_t nTag = (Int_t)Tag->size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag->at(i_tag).Contains("DYMuMu") && !Tag->at(i_tag).Contains("DYMuMu_M10to50") && !Tag->at(i_tag).Contains("DYMuMu_M50to100") )
			{
				Double_t xSec_before = Xsec->at(i_tag);

				Xsec->at(i_tag) = Xsec->at(i_tag) * SF;

				printf("[Cross section of %s] %lf -> %lf\n", Tag->at(i_tag).Data(), xSec_before, Xsec->at(i_tag) );
			}
		}
		cout << endl;
	}
	else if( Type == "aMCNLO_ee_AdditionalSF_LargeN" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M10to50_25ns_LargeN" ); Tag->push_back( "DYEE_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 18612156.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M50toInf_25ns_LargeN" ); Tag->push_back( "DYEE_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 25997467.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYEE_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 228151.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYEE_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 171446.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYEE_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50421.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYEE_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47674.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYEE_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 45913.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYEE_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 44256.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYEE_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40438.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYEE_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37287.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYEE_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23315.0 ); //nEvents: sum of DYEE weights

		Double_t SF = 1.033504;
		Int_t nTag = (Int_t)Tag->size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag->at(i_tag).Contains("DYEE") && !Tag->at(i_tag).Contains("DYEE_M10to50") && !Tag->at(i_tag).Contains("DYEE_M50to100") )
			{
				Double_t xSec_before = Xsec->at(i_tag);

				Xsec->at(i_tag) = Xsec->at(i_tag) * SF;

				printf("[Cross section of %s] %lf -> %lf\n", Tag->at(i_tag).Data(), xSec_before, Xsec->at(i_tag) );
			}
		}
		cout << endl;
	}
	else if( Type == "aMCNLO_ee_AdditionalSF_LargeN_XSecFromRidhi" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M10to50_25ns_LargeN" ); Tag->push_back( "DYEE_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 18612156.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M50toInf_25ns_LargeN" ); Tag->push_back( "DYEE_M50to100" ); Xsec->push_back( 1929.666667 ); nEvents->push_back( 25997467.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYEE_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 228151.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYEE_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 171446.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYEE_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50421.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYEE_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47674.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYEE_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 45913.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYEE_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 44256.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYEE_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40438.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYEE_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37287.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYEE_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23315.0 ); //nEvents: sum of DYEE weights
	}
	else if( Type == "aMCNLO_ee_AdditionalSF_LargeN_M10to50_XSecFromRidhi" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M10to50_25ns_LargeN" ); Tag->push_back( "DYEE_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 18612156.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYEE_M50to100" ); Xsec->push_back( 1929.666667 ); nEvents->push_back( 6058012.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYEE_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 228151.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYEE_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 171446.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYEE_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50421.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYEE_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47674.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYEE_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 45913.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYEE_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 44256.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYEE_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40438.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYEE_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37287.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYEE_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23315.0 ); //nEvents: sum of DYEE weights
	}
	else if( Type == "aMCNLO_ee_AdditionalSF_LargeN_M10to50" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M10to50_25ns_LargeN" ); Tag->push_back( "DYEE_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 18612156.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYEE_M50to100" ); Xsec->push_back( 5869.58346/3.0 ); nEvents->push_back( 6058012.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYEE_M100to200" ); Xsec->push_back( 226/3.0 ); nEvents->push_back( 228151.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYEE_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 171446.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYEE_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 50421.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYEE_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 47674.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYEE_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 45913.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYEE_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 44256.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYEE_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 40438.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYEE_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 37287.0 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYEE_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 23315.0 ); //nEvents: sum of DYEE weights

		Double_t SF = 1.033504;
		Int_t nTag = (Int_t)Tag->size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag->at(i_tag).Contains("DYEE") && !Tag->at(i_tag).Contains("DYEE_M10to50") && !Tag->at(i_tag).Contains("DYEE_M50to100") )
			{
				Double_t xSec_before = Xsec->at(i_tag);

				Xsec->at(i_tag) = Xsec->at(i_tag) * SF;

				printf("[Cross section of %s] %lf -> %lf\n", Tag->at(i_tag).Data(), xSec_before, Xsec->at(i_tag) );
			}
		}
		cout << endl;
	}
	else if( Type == "aMCNLO_NNLOxSec")
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 6016.88 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 1873.52 ); nEvents->push_back( 6061181.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 76.2401 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 2.67606 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.139728 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.0792496 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.0123176 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.01042 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.00552772 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.000741613 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.000178737 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights
	}
	else if( Type == "Full_NNLOxSecForDY")
	{
		// -- Background Samples -- //
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 985598 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 999996 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 988416 );
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16520811.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7467514.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6309713.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "76X/v20160303_76X_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 97994304 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 6016.88 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 1873.52 ); nEvents->push_back( 6061181.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 76.2401 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 2.67606 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.139728 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.0792496 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.0123176 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.01042 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.00552772 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.000741613 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.000178737 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights
	}
	else if( Type == "aMCNLO_NLO_XSDB")
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18810.0/3.0 ); nEvents->push_back( 7506956.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to100" ); Xsec->push_back( 5941.0/3.0 ); nEvents->push_back( 6061181.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M100to200_25ns" ); Tag->push_back( "DYMuMu_M100to200" ); Xsec->push_back( 226.6/3.0 ); nEvents->push_back( 227522.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.77/3.0 ); nEvents->push_back( 170955.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.4065/3.0 ); nEvents->push_back( 50136.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.2334/3.0 ); nEvents->push_back( 47833.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.03614/3.0 ); nEvents->push_back( 44740.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03047/3.0 ); nEvents->push_back( 43496.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.01636/3.0 ); nEvents->push_back( 40783.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.00218/3.0 ); nEvents->push_back( 37176.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "76X/v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.0005156/3.0 ); nEvents->push_back( 23078.0 ); //nEvents: sum of DYMuMu weights
	}
	else
		cout << "Wrong Type!" << endl;
}

void DYAnalyzer::SetupMCsamples_v20160131_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents )
{
	if( Type == "Full" )
	{
		// -- Background Samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 996944 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 978512 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 993640 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16541203.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7255646.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6419292.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 19757182 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7293818.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6413327.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18497.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 17143.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 17397.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 15827.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 14742.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160130_MINIAODv2_DYLL_M1000to1500_25ns_Resubmit" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 14381.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 13855.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 12376.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "Full_NoHighMass" )
	{
		// -- Background Samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 996944 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 978512 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 993640 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16541203.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7255646.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6419292.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 19757182 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7293818.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6422093.0 ); //nEvents: sum of DYMuMu weights
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18497.0 ); //nEvents: sum of DYMuMu weights 
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 17143.0 ); //nEvents: sum of DYMuMu weights 
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 17397.0 ); //nEvents: sum of DYMuMu weights 
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 15827.0 ); //nEvents: sum of DYMuMu weights 
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 14742.0 ); //nEvents: sum of DYMuMu weights 
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160130_MINIAODv2_DYLL_M1000to1500_25ns_Resubmit" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 14381.0 ); //nEvents: sum of DYMuMu weights 
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 13855.0 ); //nEvents: sum of DYMuMu weights 
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 12376.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "M50_M200to400" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6422093.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18497.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "aMCNLO" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7293818.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6413327.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18497.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 17143.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 17397.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 15827.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 14742.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160130_MINIAODv2_DYLL_M1000to1500_25ns_Resubmit" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 14381.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 13855.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 12376.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "Powheg" )
	{
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M50to120_25ns" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back(2836871);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M120to200_25ns" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M200to400_25ns" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M400to800_25ns" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M800to1400_25ns" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M1400to2300_25ns" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back(99600);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M2300to3500_25ns" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M3500to4500_25ns" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M4500to6000_25ns" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZMuMuPowheg_M6000toInf_25ns" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back(100000);
	}
	else if( Type == "Full_withoutM200to400" )
	{
		// -- Background Samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 996944 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 978512 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 993640 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16541203.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7255646.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6419292.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 19757182 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7293818.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to400" ); Xsec->push_back( 6103.25346/3.0 ); nEvents->push_back( 6413327.0 ); //nEvents: sum of DYMuMu weights
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18497.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 17143.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 17397.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 15827.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 14742.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160130_MINIAODv2_DYLL_M1000to1500_25ns_Resubmit" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 14381.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 13855.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 12376.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "aMCNLO_withoutM200to400" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7293818.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to400" ); Xsec->push_back( 6103.25346/3.0 ); nEvents->push_back( 6413327.0 ); //nEvents: sum of DYMuMu weights
		// ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18497.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 17143.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 17397.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 15827.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 14742.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160130_MINIAODv2_DYLL_M1000to1500_25ns_Resubmit" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 14381.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 13855.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 12376.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "aMCNLO_M50toInf" )
	{
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50toInf" ); Xsec->push_back( 2008.4 ); nEvents->push_back( 6422093.0 ); //nEvents: sum of DYMuMu weights
	}
	else if( Type == "aMCNLO_M200to400" )
	{
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18497.0 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "aMCNLO_DYEE" )
	{
		// cout << "Warning: # events should be adjusted using Sum weights of DYEE events (current one: DYMuMu SumWeights)" << endl;
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYEE_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7.29361e+06 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYEE_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6.40938e+06 ); //nEvents: sum of DYEE weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYEE_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18348 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYEE_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 17410 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYEE_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 17245 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYEE_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 16120 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYEE_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 14397 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160130_MINIAODv2_DYLL_M1000to1500_25ns_Resubmit" ); Tag->push_back( "DYEE_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 13857 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYEE_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 13495 ); //nEvents: sum of DYEE weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYEE_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 12859 ); //nEvents: sum of DYEE weights 
	}
	else if( Type == "aMCNLO_FEWZxSec" )
	{
		// xSec of M10-50 and M50 sample: aMC@NLO -- //
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7293818.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6413327.0 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 2.59583 ); nEvents->push_back( 18497.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.136235 ); nEvents->push_back( 17143.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.0775862 ); nEvents->push_back( 17397.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.0121251 ); nEvents->push_back( 15827.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.010281 ); nEvents->push_back( 14742.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160130_MINIAODv2_DYLL_M1000to1500_25ns_Resubmit" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.00546713 ); nEvents->push_back( 14381.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.000735022 ); nEvents->push_back( 13855.0 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160123_MINIAODv2_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.000176089 ); nEvents->push_back( 12376.0 ); //nEvents: sum of DYMuMu weights 
	}
	else
		cout << "Wrong Type!" << endl;

	return;
}

void DYAnalyzer::SetupMCsamples_v20160117_MiniAOD_JetMET( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents )
{
	if( Type == "Full" )
	{
		// -- Background Samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZZ_25ns" ); Tag->push_back( "ZZ" ); Xsec->push_back( 16.523 ); nEvents->push_back( 996168 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_WZ_25ns" ); Tag->push_back( "WZ" ); Xsec->push_back( 47.13 ); nEvents->push_back( 991232 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_WW_25ns" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 994416 );
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_WJets_25ns" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 16518173 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M10to50_25ns" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7418362 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M50toInf_25ns" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6430407 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ttbar_25ns" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 19899492 );
		
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7418362 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6430407 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18339 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 17143 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 17397 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 15827 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 6951 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 14381 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 13855 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 12376 ); //nEvents: sum of DYMuMu weights 
	}
	else if( Type == "aMCNLO" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M10to50_25ns" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7418362 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M50toInf_25ns" ); Tag->push_back( "DYMuMu_M50to200" ); Xsec->push_back( 6095.58346/3.0 ); nEvents->push_back( 6430407 ); //nEvents: sum of DYMuMu weights
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M200to400_25ns" ); Tag->push_back( "DYMuMu_M200to400" ); Xsec->push_back( 7.67/3.0 ); nEvents->push_back( 18339 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M400to500_25ns" ); Tag->push_back( "DYMuMu_M400to500" ); Xsec->push_back( 0.423/3.0 ); nEvents->push_back( 17143 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M500to700_25ns" ); Tag->push_back( "DYMuMu_M500to700" ); Xsec->push_back( 0.24/3.0 ); nEvents->push_back( 17397 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M700to800_25ns" ); Tag->push_back( "DYMuMu_M700to800" ); Xsec->push_back( 0.035/3.0 ); nEvents->push_back( 15827 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M800to1000_25ns" ); Tag->push_back( "DYMuMu_M800to1000" ); Xsec->push_back( 0.03/3.0 ); nEvents->push_back( 6951 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M1000to1500_25ns" ); Tag->push_back( "DYMuMu_M1000to1500" ); Xsec->push_back( 0.016/3.0 ); nEvents->push_back( 14381 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M1500to2000_25ns" ); Tag->push_back( "DYMuMu_M1500to2000" ); Xsec->push_back( 0.002/3.0 ); nEvents->push_back( 13855 ); //nEvents: sum of DYMuMu weights 
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_DYLL_M2000to3000_25ns" ); Tag->push_back( "DYMuMu_M2000to3000" ); Xsec->push_back( 0.00054/3.0 ); nEvents->push_back( 12376 ); //nEvents: sum of DYMuMu weights
	}
	else if( Type == "Powheg" )
	{
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M50to120_25ns" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back(2848071);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M120to200_25ns" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M200to400_25ns" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M400to800_25ns" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M800to1400_25ns" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M1400to2300_25ns" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back(99600);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M2300to3500_25ns" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M3500to4500_25ns" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M4500to6000_25ns" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back(100000);
		ntupleDirectory->push_back( "Spring15DR/25ns/v20160102_MINIAOD_AddJetMET_ZMuMuPowheg_M6000toInf_25ns" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back(100000);
	}
	else
		cout << "Wrong Type!" << endl;

	return;
}

Bool_t DYAnalyzer::SeparateDYLLSample_isHardProcess(TString Tag, NtupleHandle *ntuple)
{
	Bool_t GenFlag = kFALSE;

	// -- Seperate DYMuMu events from DYTauTau  -- //
	if( Tag.Contains("DYMuMu") || Tag.Contains("PIBkg") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 muons from hard-process -- //
		{
			if( Tag == "DYMuMu_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 200 )
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M50to400" ) // -- Select only evetns withtin 50 < M < 400 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 400 )
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M50to100" || Tag == "DYMuMu_Photos_M50to100" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 100 )
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M50to120" )
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 120 )
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M120to200" )
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M > 120 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	else if( Tag.Contains("DYEE") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isElectron() && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 electrons from hard-process -- //
		{
			if( Tag == "DYEE_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 200 )
					GenFlag = kTRUE;
			}
			if( Tag.Contains("M50to100") ) // -- Select only evetns withtin 50 < M < 100 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 100 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	// -- Separate DYTauTau events from MuMu events -- //
	else if( Tag.Contains("DYTauTau") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( abs(genlep.ID) == 15 && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 taus from hard-process -- //
		{
			GenFlag = kTRUE;
		}
	}
	// -- Madgraph sample -- //
	else if( Tag.Contains("Madgraph") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 muons from hard-process -- //
		{
			if( Tag == "Madgraph_M50to150" ) // -- Select only evetns withtin 50 < M < 150 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 150 )
					GenFlag = kTRUE;
			}
			else if( Tag == "Madgraph_M10to50" )
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M > 10 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	// -- other cases(e.g. ttbar, WJets, Diboson...): pass
	else
		GenFlag = kTRUE; 

	return GenFlag;
}

Bool_t DYAnalyzer::SeparateDYLLSample_LHEInfo(TString Tag, NtupleHandle *ntuple)
{
	Bool_t GenFlag = kFALSE;

	// -- Seperate DYMuMu events -- //
	if( Tag.Contains("DYMuMu") || Tag.Contains("DYEE") )
	{
		Double_t LeptonID = 0;
		if( Tag.Contains("DYMuMu") ) LeptonID = 13;
		else if( Tag.Contains("DYEE") ) LeptonID = 11;
		
		vector<LHELepton> LHELeptonCollection;
		Int_t nLHELepton = ntuple->nLHEParticle;
		for(Int_t i_lhe=0; i_lhe<nLHELepton; i_lhe++)
		{
			LHELepton lhelep;
			lhelep.FillFromNtuple(ntuple, i_lhe);
			if( fabs(lhelep.ID) == LeptonID )
				LHELeptonCollection.push_back( lhelep );

			if( LHELeptonCollection.size() == 2 ) // -- Select the events containing 2 LHE muons -- //
			{
				if( Tag.Contains("M50to100") ) // -- Select only evetns withtin 50 < M < 100 -- //
				{
					TLorentzVector v1 = LHELeptonCollection[0].Momentum;
					TLorentzVector v2 = LHELeptonCollection[1].Momentum;
					Double_t M = (v1 + v2).M();
					if( M < 100 )
						GenFlag = kTRUE;
				}
				else
					GenFlag = kTRUE;
			}
		}
	}
	else
		GenFlag = kTRUE;
	
	return GenFlag;
}

Bool_t DYAnalyzer::SeparateDYLLSample(TString Tag, NtupleHandle *ntuple)
{
	Bool_t GenFlag = kFALSE;

	// -- Seperate DYMuMu events from DYTauTau  -- //
	if( Tag.Contains("DYMuMu") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 muons from hard-process -- //
		{
			if( Tag == "DYMuMu_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 200 )
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M50to400" ) // -- Select only evetns withtin 50 < M < 400 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 400 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	else if( Tag.Contains("DYEE") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isElectron() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 electrons from hard-process -- //
		{
			if( Tag == "DYEE_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 200 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	// -- Separate DYTauTau events from MuMu events -- //
	else if( Tag.Contains("DYTauTau") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( abs(genlep.ID) == 15 && genlep.fromHardProcessDecayed )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 taus from hard-process -- //
		{
			GenFlag = kTRUE;
		}
	}
	// -- other cases(e.g. ttbar, WJets, Diboson...): pass
	else
		GenFlag = kTRUE; 

	return GenFlag;
}

void DYAnalyzer::SetupPileUpReWeighting( Bool_t isMC )
{
	if( isMC == kFALSE ) // -- for data -- //
	{
		for(Int_t i=0; i<52; i++)
			PileUpWeight[i] = 1;

		return;
	}
	
	// -- Only for the MC -- //
	TFile *f = new TFile(Path_CommonCodes + "ROOTFile_PUReWeight_v20160208_2nd_71mb.root");
	f->cd();
	TH1D *h_weight = (TH1D*)f->Get("h_PUReWeights");
	if( h_weight == NULL )
	{
		cout << "ERROR! ... No Weight histogram!"<< endl;
		return;
	}

	for(Int_t i=0; i<52; i++)
	{
		Int_t i_bin = i+1;
		PileUpWeight[i] = h_weight->GetBinContent(i_bin);
	}
}

Double_t DYAnalyzer::PileUpWeightValue(Int_t PileUp_MC)
{
	if( PileUp_MC < 0 || PileUp_MC > 51 )
	{
		cout << "[PileUp_MC = " << PileUp_MC << "]: NO CORRESPONDING PU Weight! ... it returns 0" << endl;
		return 0;
	}
	return PileUpWeight[PileUp_MC];
}

void DYAnalyzer::SetupPileUpReWeighting_76X( Bool_t isMC )
{
	if( isMC == kFALSE ) // -- for data -- //
	{
		for(Int_t i=0; i<50; i++)
			PileUpWeight[i] = 1;

		return;
	}
	
	// -- Only for the MC -- //
	TString FileLocation = Path_CommonCodes + "ROOTFile_PUReWeight_76X_v20160404_71mb.root";
	TFile *f = new TFile(FileLocation);
	f->cd();
	TH1D *h_weight = (TH1D*)f->Get("h_PUReWeights");
	if( h_weight == NULL )
	{
		cout << "ERROR! ... No Weight histogram!"<< endl;
		return;
	}

	for(Int_t i=0; i<50; i++)
	{
		Int_t i_bin = i+1;
		PileUpWeight[i] = h_weight->GetBinContent(i_bin);
	}
}

Double_t DYAnalyzer::PileUpWeightValue_76X(Int_t PileUp_MC)
{
	if( PileUp_MC < 0 || PileUp_MC > 49 )
	{
		cout << "[PileUp_MC = " << PileUp_MC << "]: NO CORRESPONDING PU Weight! ... it returns 0" << endl;
		return 0;
	}
	return PileUpWeight[PileUp_MC];
}

void DYAnalyzer::SetupEfficiencyScaleFactor()
{
	TString Location_TnP = Path_CommonCodes + "ROOTFile_TagProbeEfficiency_v20160329.root";
	cout << "[Tag&Probe efficiency is from " << Location_TnP << " (Default, 74X)]" << endl;
	
	TFile *f = new TFile( Location_TnP );
	TH2D *h_RecoID_data = (TH2D*)f->Get("h_2D_Eff_RecoID_Data");
	TH2D *h_RecoID_MC = (TH2D*)f->Get("h_2D_Eff_RecoID_MC");

	TH2D *h_Iso_data = (TH2D*)f->Get("h_2D_Eff_Iso_Data");
	TH2D *h_Iso_MC = (TH2D*)f->Get("h_2D_Eff_Iso_MC");

	TH2D *h_HLTv4p2_data = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_Data");
	TH2D *h_HLTv4p2_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_MC");

	TH2D *h_HLTv4p3_data = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_Data");
	TH2D *h_HLTv4p3_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_MC");


	Int_t nEtaBins = h_RecoID_data->GetNbinsX();
	Int_t nPtBins = h_RecoID_data->GetNbinsY();

	for(Int_t iter_x = 0; iter_x < nEtaBins; iter_x++)
	{
		for(Int_t iter_y = 0; iter_y < nPtBins; iter_y++)
		{
			Int_t i_etabin = iter_x + 1;
			Int_t i_ptbin = iter_y + 1;

			Double_t RecoID_data = h_RecoID_data->GetBinContent(i_etabin, i_ptbin);
			Double_t RecoID_MC = h_RecoID_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t Iso_data = h_Iso_data->GetBinContent(i_etabin, i_ptbin);
			Double_t Iso_MC = h_Iso_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t HLTv4p2_data = h_HLTv4p2_data->GetBinContent(i_etabin, i_ptbin);
			Double_t HLTv4p2_MC = h_HLTv4p2_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t HLTv4p3_data = h_HLTv4p3_data->GetBinContent(i_etabin, i_ptbin);
			Double_t HLTv4p3_MC = h_HLTv4p3_MC->GetBinContent(i_etabin, i_ptbin);

			Eff_RecoID_data[iter_x][iter_y] = RecoID_data;
			Eff_RecoID_MC[iter_x][iter_y] = RecoID_MC;

			Eff_Iso_data[iter_x][iter_y] = Iso_data;
			Eff_Iso_MC[iter_x][iter_y] = Iso_MC;

			Eff_HLTv4p2_data[iter_x][iter_y] = HLTv4p2_data;
			Eff_HLTv4p2_MC[iter_x][iter_y] = HLTv4p2_MC;

			Eff_HLTv4p3_data[iter_x][iter_y] = HLTv4p3_data;
			Eff_HLTv4p3_MC[iter_x][iter_y] = HLTv4p3_MC;
		}
	}
	cout << "Setting for efficiency correction factors is completed" << endl;
}

void DYAnalyzer::SetupEfficiencyScaleFactor(TString ROOTFileName)
{
	TString Location_TnP = this->Path_CommonCodes + ROOTFileName;
	cout << "[Tag&Probe efficiency is from " << Location_TnP << "]" << endl; 

	TFile *f = new TFile( Location_TnP );

	TH2D *h_RecoID_data = (TH2D*)f->Get("h_2D_Eff_RecoID_Data");
	TH2D *h_RecoID_MC = (TH2D*)f->Get("h_2D_Eff_RecoID_MC");

	TH2D *h_Iso_data = (TH2D*)f->Get("h_2D_Eff_Iso_Data");
	TH2D *h_Iso_MC = (TH2D*)f->Get("h_2D_Eff_Iso_MC");

	TH2D *h_HLTv4p2_data = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_Data");
	TH2D *h_HLTv4p2_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_MC");

	TH2D *h_HLTv4p3_data = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_Data");
	TH2D *h_HLTv4p3_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_MC");


	Int_t nEtaBins = h_RecoID_data->GetNbinsX();
	Int_t nPtBins = h_RecoID_data->GetNbinsY();

	for(Int_t iter_x = 0; iter_x < nEtaBins; iter_x++)
	{
		for(Int_t iter_y = 0; iter_y < nPtBins; iter_y++)
		{
			Int_t i_etabin = iter_x + 1;
			Int_t i_ptbin = iter_y + 1;

			Double_t RecoID_data = h_RecoID_data->GetBinContent(i_etabin, i_ptbin);
			Double_t RecoID_MC = h_RecoID_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t Iso_data = h_Iso_data->GetBinContent(i_etabin, i_ptbin);
			Double_t Iso_MC = h_Iso_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t HLTv4p2_data = h_HLTv4p2_data->GetBinContent(i_etabin, i_ptbin);
			Double_t HLTv4p2_MC = h_HLTv4p2_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t HLTv4p3_data = h_HLTv4p3_data->GetBinContent(i_etabin, i_ptbin);
			Double_t HLTv4p3_MC = h_HLTv4p3_MC->GetBinContent(i_etabin, i_ptbin);

			Eff_RecoID_data[iter_x][iter_y] = RecoID_data;
			Eff_RecoID_MC[iter_x][iter_y] = RecoID_MC;

			Eff_Iso_data[iter_x][iter_y] = Iso_data;
			Eff_Iso_MC[iter_x][iter_y] = Iso_MC;

			Eff_HLTv4p2_data[iter_x][iter_y] = HLTv4p2_data;
			Eff_HLTv4p2_MC[iter_x][iter_y] = HLTv4p2_MC;

			Eff_HLTv4p3_data[iter_x][iter_y] = HLTv4p3_data;
			Eff_HLTv4p3_MC[iter_x][iter_y] = HLTv4p3_MC;
		}
	}
	cout << "Setting for efficiency correction factors is completed" << endl;
}

Double_t DYAnalyzer::EfficiencySF_EventWeight_HLTv4p2(Muon mu1, Muon mu2)
{
	Double_t weight = -999;

	// -- Muon1 -- //
	Double_t Pt1 = mu1.Pt;
	Double_t eta1 = mu1.eta;

	Int_t ptbin1 = FindPtBin( Pt1 );
	Int_t etabin1 = FindEtaBin( eta1 );

	if( ptbin1 == 9999 || etabin1 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
		return -999;
	}

	Double_t Eff_muon1_data = Eff_RecoID_data[etabin1][ptbin1] * Eff_Iso_data[etabin1][ptbin1];
	Double_t Eff_muon1_MC = Eff_RecoID_MC[etabin1][ptbin1] * Eff_Iso_MC[etabin1][ptbin1];

	// -- Muon2 -- //
	Double_t Pt2 = mu2.Pt;
	Double_t eta2 = mu2.eta;

	Int_t ptbin2 = FindPtBin( Pt2 );
	Int_t etabin2 = FindEtaBin( eta2 );

	if( ptbin2 == 9999 || etabin2 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
		return -999;
	}
	Double_t Eff_muon2_data = Eff_RecoID_data[etabin2][ptbin2] * Eff_Iso_data[etabin2][ptbin2];
	Double_t Eff_muon2_MC = Eff_RecoID_MC[etabin2][ptbin2] * Eff_Iso_MC[etabin2][ptbin2];


	Double_t Eff_EventTrig_data = 0;
	Double_t Eff_EventTrig_MC = 0;

	Double_t Eff_Trig_muon1_data = Eff_HLTv4p2_data[etabin1][ptbin1];
	Double_t Eff_Trig_muon2_data = Eff_HLTv4p2_data[etabin2][ptbin2];
	Eff_EventTrig_data = Eff_Trig_muon1_data + Eff_Trig_muon2_data - Eff_Trig_muon1_data * Eff_Trig_muon2_data;

	Double_t Eff_Trig_muon1_MC = Eff_HLTv4p2_MC[etabin1][ptbin1];
	Double_t Eff_Trig_muon2_MC = Eff_HLTv4p2_MC[etabin2][ptbin2];
	Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;

	Double_t Eff_data_all = Eff_muon1_data * Eff_muon2_data * Eff_EventTrig_data;
	Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC * Eff_EventTrig_MC;

	// cout << "Eff_data_all: " << Eff_data_all << ", Eff_MC_all: " << Eff_MC_all << endl;
	weight = Eff_data_all / Eff_MC_all;

	if( weight > 2 )
	{
		printf("[Data]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin1][ptbin1], Eff_Iso_data[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin2][ptbin2], Eff_Iso_data[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_data, Eff_data_all);

		printf("[MC]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin1][ptbin1], Eff_Iso_MC[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin2][ptbin2], Eff_Iso_MC[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_MC, Eff_MC_all);

		printf("(ptbin1, etabin1, ptbin2, etabin2): (%d, %d, %d, %d)\n", ptbin1, etabin1, ptbin2, etabin2);
		
		printf("[SF] Weight = %.3lf\n", weight);
	}
	return weight;
}

Double_t DYAnalyzer::EfficiencySF_EventWeight_HLTv4p3(Muon mu1, Muon mu2)
{
	Double_t weight = -999;

	// -- Muon1 -- //
	Double_t Pt1 = mu1.Pt;
	Double_t eta1 = mu1.eta;

	Int_t ptbin1 = FindPtBin( Pt1 );
	Int_t etabin1 = FindEtaBin( eta1 );

	if( ptbin1 == 9999 || etabin1 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
		return -999;
	}

	Double_t Eff_muon1_data = Eff_RecoID_data[etabin1][ptbin1] * Eff_Iso_data[etabin1][ptbin1];
	Double_t Eff_muon1_MC = Eff_RecoID_MC[etabin1][ptbin1] * Eff_Iso_MC[etabin1][ptbin1];

	// -- Muon2 -- //
	Double_t Pt2 = mu2.Pt;
	Double_t eta2 = mu2.eta;

	Int_t ptbin2 = FindPtBin( Pt2 );
	Int_t etabin2 = FindEtaBin( eta2 );

	if( ptbin2 == 9999 || etabin2 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
		return -999;
	}
	Double_t Eff_muon2_data = Eff_RecoID_data[etabin2][ptbin2] * Eff_Iso_data[etabin2][ptbin2];
	Double_t Eff_muon2_MC = Eff_RecoID_MC[etabin2][ptbin2] * Eff_Iso_MC[etabin2][ptbin2];


	Double_t Eff_EventTrig_data = 0;
	Double_t Eff_EventTrig_MC = 0;

	Double_t Eff_Trig_muon1_data = Eff_HLTv4p3_data[etabin1][ptbin1];
	Double_t Eff_Trig_muon2_data = Eff_HLTv4p3_data[etabin2][ptbin2];
	Eff_EventTrig_data = Eff_Trig_muon1_data + Eff_Trig_muon2_data - Eff_Trig_muon1_data * Eff_Trig_muon2_data;

	Double_t Eff_Trig_muon1_MC = Eff_HLTv4p3_MC[etabin1][ptbin1];
	Double_t Eff_Trig_muon2_MC = Eff_HLTv4p3_MC[etabin2][ptbin2];
	Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;

	Double_t Eff_data_all = Eff_muon1_data * Eff_muon2_data * Eff_EventTrig_data;
	Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC * Eff_EventTrig_MC;

	// cout << "Eff_data_all: " << Eff_data_all << ", Eff_MC_all: " << Eff_MC_all << endl;
	weight = Eff_data_all / Eff_MC_all;

	if( weight > 2 )
	{
		printf("[Data]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin1][ptbin1], Eff_Iso_data[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin2][ptbin2], Eff_Iso_data[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_data, Eff_data_all);

		printf("[MC]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin1][ptbin1], Eff_Iso_MC[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin2][ptbin2], Eff_Iso_MC[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_MC, Eff_MC_all);

		printf("(ptbin1, etabin1, ptbin2, etabin2): (%d, %d, %d, %d)\n", ptbin1, etabin1, ptbin2, etabin2);
		
		printf("[SF] Weight = %.3lf\n", weight);
	}
	return weight;
}

Int_t DYAnalyzer::FindPtBin(Double_t Pt)
{
	const Int_t nPtBins = 4;
	Double_t PtBinEdges[nPtBins+1] = {10, 22, 40, 70, 250};

	Int_t ptbin = 9999;

	// -- if Pt is larger than the largest Pt bin edge, SF is same with the value for the last bin -- // 
	if( Pt > PtBinEdges[nPtBins] )
		ptbin = nPtBins-1;
	else
	{
		for(Int_t i=0; i<nPtBins; i++)
		{
			if( Pt > PtBinEdges[i] && Pt < PtBinEdges[i+1] )
			{
				ptbin = i;
				break;
			}
		}
	}

	return ptbin;
}

Int_t DYAnalyzer::FindEtaBin(Double_t eta)
{
	const Int_t nEtaBins = 5;
	Double_t EtaBinEdges[nEtaBins+1] = {-2.4, -1.2, -0.3, 0.3, 1.2, 2.4};

	Int_t etabin = 9999;

	for(Int_t i=0; i<nEtaBins; i++)
	{
		if( eta > EtaBinEdges[i] && eta < EtaBinEdges[i+1] )
		{
			etabin = i;
			break;
		}
	}

	return etabin;
}

Double_t DYAnalyzer::EfficiencySF_EventWeight(Muon mu1, Muon mu2, NtupleHandle *ntuple)
{
	Double_t weight = -999;

	// -- Muon1 -- //
	Double_t Pt1 = mu1.Pt;
	Double_t eta1 = mu1.eta;

	Int_t ptbin1 = FindPtBin( Pt1 );
	Int_t etabin1 = FindEtaBin( eta1 );

	if( ptbin1 == 9999 || etabin1 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
		return -999;
	}

	Double_t Eff_muon1_data = Eff_RecoID_data[etabin1][ptbin1] * Eff_Iso_data[etabin1][ptbin1];
	Double_t Eff_muon1_MC = Eff_RecoID_MC[etabin1][ptbin1] * Eff_Iso_MC[etabin1][ptbin1];

	// -- Muon2 -- //
	Double_t Pt2 = mu2.Pt;
	Double_t eta2 = mu2.eta;

	Int_t ptbin2 = FindPtBin( Pt2 );
	Int_t etabin2 = FindEtaBin( eta2 );

	if( ptbin2 == 9999 || etabin2 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
		return -999;
	}
	Double_t Eff_muon2_data = Eff_RecoID_data[etabin2][ptbin2] * Eff_Iso_data[etabin2][ptbin2];
	Double_t Eff_muon2_MC = Eff_RecoID_MC[etabin2][ptbin2] * Eff_Iso_MC[etabin2][ptbin2];

	Bool_t isHLTv4p2 = kFALSE;
	if( ntuple->runNum < 257932.5 )
		isHLTv4p2 = kTRUE;

	Double_t Eff_EventTrig_data = 0;
	Double_t Eff_EventTrig_MC = 0;
	if( isHLTv4p2 )
	{
		Double_t Eff_Trig_muon1_data = Eff_HLTv4p2_data[etabin1][ptbin1];
		Double_t Eff_Trig_muon2_data = Eff_HLTv4p2_data[etabin2][ptbin2];
		Eff_EventTrig_data = Eff_Trig_muon1_data + Eff_Trig_muon2_data - Eff_Trig_muon1_data * Eff_Trig_muon2_data;

		Double_t Eff_Trig_muon1_MC = Eff_HLTv4p2_MC[etabin1][ptbin1];
		Double_t Eff_Trig_muon2_MC = Eff_HLTv4p2_MC[etabin2][ptbin2];
		Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;
	}
	else
	{
		Double_t Eff_Trig_muon1_data = Eff_HLTv4p3_data[etabin1][ptbin1];
		Double_t Eff_Trig_muon2_data = Eff_HLTv4p3_data[etabin2][ptbin2];
		Eff_EventTrig_data = Eff_Trig_muon1_data + Eff_Trig_muon2_data - Eff_Trig_muon1_data * Eff_Trig_muon2_data;

		Double_t Eff_Trig_muon1_MC = Eff_HLTv4p3_MC[etabin1][ptbin1];
		Double_t Eff_Trig_muon2_MC = Eff_HLTv4p3_MC[etabin2][ptbin2];
		Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;
	}

	Double_t Eff_data_all = Eff_muon1_data * Eff_muon2_data * Eff_EventTrig_data;
	Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC * Eff_EventTrig_MC;

	// cout << "Eff_data_all: " << Eff_data_all << ", Eff_MC_all: " << Eff_MC_all << endl;
	weight = Eff_data_all / Eff_MC_all;

	if( weight > 2 )
	{
		printf("[Data]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin1][ptbin1], Eff_Iso_data[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin2][ptbin2], Eff_Iso_data[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_data, Eff_data_all);

		printf("[MC]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin1][ptbin1], Eff_Iso_MC[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin2][ptbin2], Eff_Iso_MC[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_MC, Eff_MC_all);

		printf("(ptbin1, etabin1, ptbin2, etabin2): (%d, %d, %d, %d)\n", ptbin1, etabin1, ptbin2, etabin2);
		
		printf("[SF] Weight = %.3lf\n", weight);
	}
	return weight;
}

Double_t DYAnalyzer::EfficiencySF_EventWeight_RecoIdIso(Muon mu1, Muon mu2)
{
	Double_t weight = -999;

	// -- Muon1 -- //
	Double_t Pt1 = mu1.Pt;
	Double_t eta1 = mu1.eta;

	Int_t ptbin1 = FindPtBin( Pt1 );
	Int_t etabin1 = FindEtaBin( eta1 );

	if( ptbin1 == 9999 || etabin1 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
		return -999;
	}

	Double_t Eff_muon1_data = Eff_RecoID_data[etabin1][ptbin1] * Eff_Iso_data[etabin1][ptbin1];
	Double_t Eff_muon1_MC = Eff_RecoID_MC[etabin1][ptbin1] * Eff_Iso_MC[etabin1][ptbin1];

	// -- Muon2 -- //
	Double_t Pt2 = mu2.Pt;
	Double_t eta2 = mu2.eta;

	Int_t ptbin2 = FindPtBin( Pt2 );
	Int_t etabin2 = FindEtaBin( eta2 );

	if( ptbin2 == 9999 || etabin2 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
		return -999;
	}
	Double_t Eff_muon2_data = Eff_RecoID_data[etabin2][ptbin2] * Eff_Iso_data[etabin2][ptbin2];
	Double_t Eff_muon2_MC = Eff_RecoID_MC[etabin2][ptbin2] * Eff_Iso_MC[etabin2][ptbin2];

	Double_t Eff_data_all = Eff_muon1_data * Eff_muon2_data;
	Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC;

	weight = Eff_data_all / Eff_MC_all;

	return weight;
}

Bool_t DYAnalyzer::EventSelection(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() && MuonCollection[j].trkiso < 0.10)
	        QMuonCollection.push_back( MuonCollection[j] );
	}

	// -- Check the existence of at least one muon matched with HLT-object -- //
	Bool_t isExistHLTMatchedMuon = kFALSE;
	for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
	{
		Muon mu = QMuonCollection[i_mu];
		if( mu.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || mu.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
		{
			isExistHLTMatchedMuon = kTRUE;
			break;
		}
	}

	if( isExistHLTMatchedMuon == kTRUE )
	{
		Int_t nQMuons = (Int_t)QMuonCollection.size();
		if( nQMuons == 2)
		{
			Muon recolep1 = QMuonCollection[0];
			Muon recolep2 = QMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			Bool_t isOS = kFALSE;
			if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
		else if( nQMuons > 2 )
		{
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
			{
				Muon Mu = QMuonCollection[i_mu];

				// -- at least 1 muon should be matched with HLT objects in best pair -- //
				if( Mu.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || Mu.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
				{
					// -- Mu in this loop: QMuon Matched with HLT object -- //

					// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
					for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
					{
						Muon Mu_jth = QMuonCollection[j_mu];

						if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
						{
							// -- Check that this pair is within acceptance -- //
							Bool_t isPassAcc = kFALSE;
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

							if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
							{
								Double_t VtxProb_temp = -999;
								Double_t VtxNormChi2_temp = 999;
								DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

								// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
								if( VtxNormChi2_temp < VtxNormChi2_BestPair )
								{
									VtxNormChi2_BestPair = VtxNormChi2_temp;
									mu1_BestPair = Mu;
									mu2_BestPair = Mu_jth;
								}
							}
						}
					} // -- end of the loop for j_mu (finding for second muon)
				}
			} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

			if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
			{
				TLorentzVector reco_v1 = mu1_BestPair.Momentum;
				TLorentzVector reco_v2 = mu2_BestPair.Momentum;
				Double_t reco_M = (reco_v1 + reco_v2).M();

				// -- 3D open angle is calculated using inner track information -- //
				// -- 3D open angle -- //
				Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

				Bool_t isOS = kFALSE;
				if( mu1_BestPair.charge != mu2_BestPair.charge ) isOS = kTRUE;

				if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}

// -- Test using the trigger without isolation condition: HLT_Mu50_v* -- //
Bool_t DYAnalyzer::EventSelection_Mu50(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() && MuonCollection[j].trkiso < 0.10)
	        QMuonCollection.push_back( MuonCollection[j] );
	}

	// -- Check the existence of at least one muon matched with HLT-object -- //
	Bool_t isExistHLTMatchedMuon = kFALSE;
	for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
	{
		Muon mu = QMuonCollection[i_mu];
		if( mu.isTrigMatched(ntuple, HLT) )
		{
			isExistHLTMatchedMuon = kTRUE;
			break;
		}
	}

	if( isExistHLTMatchedMuon == kTRUE )
	{
		Int_t nQMuons = (Int_t)QMuonCollection.size();
		if( nQMuons == 2)
		{
			Muon recolep1 = QMuonCollection[0];
			Muon recolep2 = QMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
		else if( nQMuons > 2 )
		{
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
			{
				Muon Mu = QMuonCollection[i_mu];

				// -- at least 1 muon should be matched with HLT objects in best pair -- //
				if( Mu.isTrigMatched(ntuple, HLT) )
				{
					// -- Mu in this loop: QMuon Matched with HLT object -- //

					// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
					for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
					{
						Muon Mu_jth = QMuonCollection[j_mu];

						if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
						{
							// -- Check that this pair is within acceptance -- //
							Bool_t isPassAcc = kFALSE;
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

							if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
							{
								Double_t VtxProb_temp = -999;
								Double_t VtxNormChi2_temp = 999;
								DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

								// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
								if( VtxNormChi2_temp < VtxNormChi2_BestPair )
								{
									VtxNormChi2_BestPair = VtxNormChi2_temp;
									mu1_BestPair = Mu;
									mu2_BestPair = Mu_jth;
								}
							}
						}
					} // -- end of the loop for j_mu (finding for second muon)
				}
			} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

			if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
			{
				TLorentzVector reco_v1 = mu1_BestPair.Momentum;
				TLorentzVector reco_v2 = mu2_BestPair.Momentum;
				Double_t reco_M = (reco_v1 + reco_v2).M();

				// -- 3D open angle is calculated using inner track information -- //
				// -- 3D open angle -- //
				Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

				if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}


Bool_t DYAnalyzer::EventSelection_minusDimuonVtxCut(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() && MuonCollection[j].trkiso < 0.10)
	        QMuonCollection.push_back( MuonCollection[j] );
	}

	// -- Check the existence of at least one muon matched with HLT-object -- //
	Bool_t isExistHLTMatchedMuon = kFALSE;
	for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
	{
		Muon mu = QMuonCollection[i_mu];
		if( mu.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || mu.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
		{
			isExistHLTMatchedMuon = kTRUE;
			break;
		}
	}

	if( isExistHLTMatchedMuon == kTRUE )
	{
		Int_t nQMuons = (Int_t)QMuonCollection.size();
		if( nQMuons == 2)
		{
			Muon recolep1 = QMuonCollection[0];
			Muon recolep2 = QMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			// if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && Angle < TMath::Pi() - 0.005 )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
		else if( nQMuons > 2 )
		{
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
			{
				Muon Mu = QMuonCollection[i_mu];

				// -- at least 1 muon should be matched with HLT objects in best pair -- //
				if( Mu.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || Mu.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
				{
					// -- Mu in this loop: QMuon Matched with HLT object -- //

					// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
					for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
					{
						Muon Mu_jth = QMuonCollection[j_mu];

						if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
						{
							// -- Check that this pair is within acceptance -- //
							Bool_t isPassAcc = kFALSE;
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

							if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
							{
								Double_t VtxProb_temp = -999;
								Double_t VtxNormChi2_temp = 999;
								DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

								// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
								if( VtxNormChi2_temp < VtxNormChi2_BestPair )
								{
									VtxNormChi2_BestPair = VtxNormChi2_temp;
									mu1_BestPair = Mu;
									mu2_BestPair = Mu_jth;
								}
							}
						}
					} // -- end of the loop for j_mu (finding for second muon)
				}
			} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

			if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
			{
				TLorentzVector reco_v1 = mu1_BestPair.Momentum;
				TLorentzVector reco_v2 = mu2_BestPair.Momentum;
				Double_t reco_M = (reco_v1 + reco_v2).M();

				// -- 3D open angle is calculated using inner track information -- //
				// -- 3D open angle -- //
				Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

				// if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 )
				if( reco_M > 10 && Angle < TMath::Pi() - 0.005 )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}

// -- Event selection used for differential Z cross section measurement @ 13TeV -- // 
Bool_t DYAnalyzer::EventSelection_Zdiff_13TeV(vector< Muon > MuonCollection, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isTightMuon() && MuonCollection[j].trkiso < 0.10) // -- Iso should be changed with PFIso(dBeta) in order to be same selection with Z-diff x-section measurement
	        QMuonCollection.push_back( MuonCollection[j] );
	}

	Int_t nQMuons = (Int_t)QMuonCollection.size();
	if( nQMuons == 2)
	{
		Muon recolep1 = QMuonCollection[0];
		Muon recolep2 = QMuonCollection[1];

		// -- Check the Accpetance -- //
		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

		// -- Opposite sign condition -- //
		Bool_t isOppositeSign = kFALSE;
		if( recolep1.charge != recolep2.charge )
			isOppositeSign = kTRUE;

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 60 && reco_M < 120 && isPassAcc == kTRUE && isOppositeSign == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedMuonCollection->push_back( recolep1 );
			SelectedMuonCollection->push_back( recolep2 );
		}
	}
	else if( nQMuons > 2 )
	{
		// -- More then 2 Qualified Muon: Select the muons with highest pT -- // 
		Double_t Pt_leading = 0;
		Muon LeadingMuon;
		Double_t i_leading = 0;
		for(Int_t i_mu1 = 0; i_mu1 < nQMuons; i_mu1++)
		{
			Muon Mu = QMuonCollection[i_mu1];

			// printf("%dth Muon: Pt = %.3lf\n", i_mu1, Mu.Pt);

			if( Mu.Pt > Pt_leading )
			{
				Pt_leading = Mu.Pt;
				LeadingMuon	= Mu;
				i_leading = i_mu1;
			}
		}

		Double_t Pt_sub = 0;
		Muon SubMuon;
		for(Int_t i_mu2=0; i_mu2 < nQMuons; i_mu2++)
		{
			if( i_mu2 == i_leading ) continue;

			Muon Mu = QMuonCollection[i_mu2];

			if( Mu.Pt > Pt_sub )
			{
				Pt_sub = Mu.Pt;
				SubMuon	= Mu;
			}
		}

		// printf("\t(Pt_leading, Pt_sub) = (%.3lf, %.3lf)\n", LeadingMuon.Pt, SubMuon.Pt);

		// -- Check the Accpetance -- //
		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Muon(LeadingMuon, SubMuon);

		// -- Opposite sign condition -- //
		Bool_t isOppositeSign = kFALSE;
		if( LeadingMuon.charge != SubMuon.charge )
			isOppositeSign = kTRUE;

		Double_t reco_M = (LeadingMuon.Momentum + SubMuon.Momentum).M();

		if( reco_M > 60 && reco_M < 120 && isPassAcc == kTRUE && isOppositeSign == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedMuonCollection->push_back( LeadingMuon );
			SelectedMuonCollection->push_back( SubMuon );
		}

	} // -- End of else if( nQMuons > 2 ) -- //

	return isPassEventSelection;
}

Bool_t DYAnalyzer::isPassAccCondition_Muon(Muon Mu1, Muon Mu2)
{
	Bool_t isPassAcc = kFALSE;
	Muon leadMu, subMu;
	CompareMuon(&Mu1, &Mu2, &leadMu, &subMu);
	if( leadMu.Pt > LeadPtCut && fabs(leadMu.eta) < LeadEtaCut && 
		subMu.Pt  > SubPtCut  && fabs(subMu.eta)  < SubEtaCut )
		isPassAcc = kTRUE;

	return isPassAcc;
}

Bool_t DYAnalyzer::isPassAccCondition_GenLepton(GenLepton genlep1, GenLepton genlep2)
{
	Bool_t isPassAcc = kFALSE;

	GenLepton leadGenLep, subGenLep;
	CompareGenLepton(&genlep1, &genlep2, &leadGenLep, &subGenLep);
	
	if( leadGenLep.Pt > LeadPtCut && fabs(leadGenLep.eta) < LeadEtaCut &&
		subGenLep.Pt  > SubPtCut  && fabs(subGenLep.eta) < SubEtaCut )
		isPassAcc = 1;

	return isPassAcc;
}

void DYAnalyzer::CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu)
{
    if( Mu1->Pt > Mu2->Pt )
    {
        *leadMu = *Mu1;
        *subMu = *Mu2;
    }
    else
    {
        *leadMu = *Mu2;
        *subMu = *Mu1;
    }
}

void DYAnalyzer::CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep)
{
	if( genlep1->Pt > genlep2->Pt )
	{
		*leadgenlep = *genlep1;
		*subgenlep = *genlep2;
	}
	else
	{
		*leadgenlep = *genlep2;
		*subgenlep = *genlep1;
	}
}

void DYAnalyzer::DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2)
{
	vector<double> *PtCollection1 = ntuple->vtxTrkCkt1Pt;
	vector<double> *PtCollection2 = ntuple->vtxTrkCkt2Pt;
	vector<double> *VtxProbCollection = ntuple->vtxTrkProb;

	Int_t NPt1 = (Int_t)PtCollection1->size();
	Int_t NPt2 = (Int_t)PtCollection2->size();
	Int_t NProb = (Int_t)VtxProbCollection->size();

	if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb ) 
		cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

	// cout << "Pt1: " << Pt1 << " Pt2: " << Pt2 << endl;

	for(Int_t i=0; i<NProb; i++)
	{
		// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
		if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
		{
			*VtxProb = VtxProbCollection->at(i);
			*VtxNormChi2 = ntuple->vtxTrkChi2->at(i) / ntuple->vtxTrkNdof->at(i);
			break;
		}
	}

	return;
}

// -- Event selecton for the electron channel (test) -- //
Bool_t DYAnalyzer::EventSelection_Electron(vector< Electron > ElectronCollection, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns() && elec.ecalDriven == 1 && elec.Pt > 15 )
			QElectronCollection.push_back( ElectronCollection[j] );
	}

	Int_t nQElectrons = (Int_t)QElectronCollection.size();

	// cout << "# qualified electrons: " << nQElectrons << endl;

	if( nQElectrons == 2 )
	{
		Electron recolep1 = QElectronCollection[0];
		Electron recolep2 = QElectronCollection[1];

		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 10 && isPassAcc == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedElectronCollection->push_back( recolep1 );
			SelectedElectronCollection->push_back( recolep2 );
		}
	}
	return isPassEventSelection;

}

// -- Event selecton for the electron channel (2016.02.11) -- //
Bool_t DYAnalyzer::EventSelection_ElectronChannel(vector< Electron > ElectronCollection, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns() && elec.ecalDriven == 1 
			&& elec.Pt > SubPtCut && fabs(elec.etaSC) < SubEtaCut && !( fabs(elec.etaSC) > 1.4442 && fabs(elec.etaSC) < 1.566 ) )
			QElectronCollection.push_back( ElectronCollection[j] );
	}

	Int_t nQElectrons = (Int_t)QElectronCollection.size();

	// cout << "# qualified electrons: " << nQElectrons << endl;

	if( nQElectrons == 2 )
	{
		Electron recolep1 = QElectronCollection[0];
		Electron recolep2 = QElectronCollection[1];

		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 10 && isPassAcc == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedElectronCollection->push_back( recolep1 );
			SelectedElectronCollection->push_back( recolep2 );
		}
	}
	return isPassEventSelection;

}

// -- Event selecton for the electron channel (2016.02.11) -- //
Bool_t DYAnalyzer::EventSelection_ElectronChannel_NminusPFIso(vector< Electron > ElectronCollection, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns_minus_PFIso() && elec.ecalDriven == 1 
			&& elec.Pt > SubPtCut && fabs(elec.etaSC) < SubEtaCut && !( fabs(elec.etaSC) > 1.4442 && fabs(elec.etaSC) < 1.566 ) )
			QElectronCollection.push_back( ElectronCollection[j] );
	}

	Int_t nQElectrons = (Int_t)QElectronCollection.size();

	// cout << "# qualified electrons: " << nQElectrons << endl;

	if( nQElectrons == 2 )
	{
		Electron recolep1 = QElectronCollection[0];
		Electron recolep2 = QElectronCollection[1];

		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 10 && isPassAcc == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedElectronCollection->push_back( recolep1 );
			SelectedElectronCollection->push_back( recolep2 );
		}
	}
	return isPassEventSelection;

}

Bool_t DYAnalyzer::isPassAccCondition_Electron(Electron Elec1, Electron Elec2)
{
	Bool_t isPassAcc = kFALSE;
	Electron leadElec, subElec;
	CompareElectron(&Elec1, &Elec2, &leadElec, &subElec);
	if( leadElec.Pt > LeadPtCut && fabs(leadElec.etaSC) < LeadEtaCut && !( fabs(leadElec.etaSC) > 1.4442 && fabs(leadElec.etaSC) < 1.566 ) &&
		subElec.Pt  > SubPtCut  && fabs(subElec.etaSC)  < SubEtaCut && !( fabs(subElec.etaSC) > 1.4442 && fabs(subElec.etaSC) < 1.566 ) )
		isPassAcc = kTRUE;

	return isPassAcc;
}


Bool_t DYAnalyzer::isPassAccCondition_GenLepton_ECALGAP(GenLepton genlep1, GenLepton genlep2)
{
	Bool_t isPassAcc = kFALSE;

	GenLepton leadGenLep, subGenLep;
	CompareGenLepton(&genlep1, &genlep2, &leadGenLep, &subGenLep);
	
	if( leadGenLep.Pt > LeadPtCut && fabs(leadGenLep.eta) < LeadEtaCut && !( fabs(leadGenLep.eta) > 1.4442 && fabs(leadGenLep.eta) < 1.566 ) &&
		subGenLep.Pt  > SubPtCut  && fabs(subGenLep.eta) < SubEtaCut && !( fabs(subGenLep.eta) > 1.4442 && fabs(subGenLep.eta) < 1.566 ) )
		isPassAcc = 1;

	return isPassAcc;
}

void DYAnalyzer::CompareElectron(Electron *Elec1, Electron *Elec2, Electron *leadElec, Electron *subElec)
{
    if( Elec1->Pt > Elec2->Pt )
    {
        *leadElec = *Elec1;
        *subElec = *Elec2;
    }
    else
    {
        *leadElec = *Elec2;
        *subElec = *Elec1;
    }
}

void DYAnalyzer::PostToPreFSR_byDressedLepton(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection)
{
	TLorentzVector genlep_Mom_postFSR = genlep_postFSR->Momentum;

	TLorentzVector SumPhotonMom;
	SumPhotonMom.SetPxPyPzE(0,0,0,0);

	Int_t NGenOthers = ntuple->nGenOthers;
	for(Int_t i_gen=0; i_gen<NGenOthers; i_gen++)
	{
		GenOthers genlep;
		genlep.FillFromNtuple(ntuple, i_gen);

		// -- Only for the photons whose mother is muon or anti-muon -- //
		if( fabs(genlep.ID) == 22 && fabs(genlep.Mother) == 13)
		{
			
			Double_t dR = Calc_dR_GenLepton_GenOthers(*genlep_postFSR, genlep);

			// -- Sum of all photon's momentum near the post-FSR muon -- //
			if( dR < dRCut )
			{
				SumPhotonMom  = SumPhotonMom + genlep.Momentum;
				GenPhotonCollection->push_back( genlep );
			}
		}
	}

	// -- Momentum(pre-FSR) = Momentum(post-FSR) + Sum of all Photon's momentum near the post-FSR muon -- //
	genlep_preFSR->Momentum = genlep_Mom_postFSR + SumPhotonMom;
	genlep_preFSR->Et = genlep_preFSR->Momentum.Et();
	genlep_preFSR->Pt = genlep_preFSR->Momentum.Pt();
	genlep_preFSR->eta = genlep_preFSR->Momentum.Eta();
	genlep_preFSR->phi = genlep_preFSR->Momentum.Phi();
	genlep_preFSR->Px = genlep_preFSR->Momentum.Px();
	genlep_preFSR->Py = genlep_preFSR->Momentum.Py();
	genlep_preFSR->Pz = genlep_preFSR->Momentum.Pz();
}

void DYAnalyzer::PostToPreFSR_byDressedLepton_AllPhotons(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection)
{
	TLorentzVector genlep_Mom_postFSR = genlep_postFSR->Momentum;

	TLorentzVector SumPhotonMom;
	SumPhotonMom.SetPxPyPzE(0,0,0,0);

	Int_t NGenOthers = ntuple->nGenOthers;
	for(Int_t i_gen=0; i_gen<NGenOthers; i_gen++)
	{
		GenOthers genlep;
		genlep.FillFromNtuple(ntuple, i_gen);

		// -- all photons within dR < 0.1 -- //
		// if( fabs(genlep.ID) == 22 && fabs(genlep.Mother) == 13)
		if( fabs(genlep.ID) == 22 )
		{
			
			Double_t dR = Calc_dR_GenLepton_GenOthers(*genlep_postFSR, genlep);

			// -- Sum of all photon's momentum near the post-FSR muon -- //
			if( dR < dRCut )
			{
				SumPhotonMom  = SumPhotonMom + genlep.Momentum;
				GenPhotonCollection->push_back( genlep );
			}
		}
	}

	// -- Momentum(pre-FSR) = Momentum(post-FSR) + Sum of all Photon's momentum near the post-FSR muon -- //
	genlep_preFSR->Momentum = genlep_Mom_postFSR + SumPhotonMom;
	genlep_preFSR->Et = genlep_preFSR->Momentum.Et();
	genlep_preFSR->Pt = genlep_preFSR->Momentum.Pt();
	genlep_preFSR->eta = genlep_preFSR->Momentum.Eta();
	genlep_preFSR->phi = genlep_preFSR->Momentum.Phi();
	genlep_preFSR->Px = genlep_preFSR->Momentum.Px();
	genlep_preFSR->Py = genlep_preFSR->Momentum.Py();
	genlep_preFSR->Pz = genlep_preFSR->Momentum.Pz();
}

TString DYAnalyzer::DecideFSRType(GenLepton preFSR1, GenLepton preFSR2, GenLepton postFSR1, GenLepton postFSR2)
{
	TString FSRType = "";

	Bool_t isPassAcc_preFSREvent = kFALSE;
	isPassAcc_preFSREvent = isPassAccCondition_GenLepton(preFSR1, preFSR2);

	Bool_t isPassAcc_postFSREvent = kFALSE;
	isPassAcc_postFSREvent = isPassAccCondition_GenLepton(postFSR1, postFSR2);


	if( isPassAcc_preFSREvent == kFALSE && isPassAcc_postFSREvent == kTRUE )
		FSRType = "A";

	else if( isPassAcc_preFSREvent == kTRUE && isPassAcc_postFSREvent == kTRUE)
		FSRType = "B";
	
	else if( isPassAcc_preFSREvent == kTRUE && isPassAcc_postFSREvent == kFALSE)
		FSRType = "C";

	else if( isPassAcc_preFSREvent == kFALSE && isPassAcc_postFSREvent == kFALSE)
		FSRType = "D";
	else
	{
		cout << "ERROR: NO FSR TYPE CORRESPONDING TO THIS EVENT" << endl;
		FSRType = "NOTAssigned";
	}

	return FSRType;
}

Double_t DYAnalyzer::Calc_dR_GenLeptons( GenLepton genlep1, GenLepton genlep2 )
{
	Double_t eta1 = genlep1.eta;
	Double_t phi1 = genlep1.phi;

	Double_t eta2 = genlep2.eta;
	Double_t phi2 = genlep2.phi;

	Double_t diff_eta = eta1 - eta2;
	Double_t diff_phi = phi1 - phi2;

	Double_t dR = sqrt( diff_eta * diff_eta + diff_phi * diff_phi );
	return dR;
}

Double_t DYAnalyzer::Calc_dR_GenLepton_GenOthers( GenLepton genlep1, GenOthers genlep2 )
{
	Double_t eta1 = genlep1.eta;
	Double_t phi1 = genlep1.phi;

	Double_t eta2 = genlep2.eta;
	Double_t phi2 = genlep2.phi;

	Double_t diff_eta = eta1 - eta2;
	Double_t diff_phi = phi1 - phi2;

	Double_t dR = sqrt( diff_eta * diff_eta + diff_phi * diff_phi );
	return dR;
}

void DYAnalyzer::GenMatching(TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection)
{
	vector<GenLepton> GenLeptonCollection;
	Int_t NGenLeptons = ntuple->gnpair;

	if( MuonType == "PromptFinalState" )
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isPromptFinalState )
				GenLeptonCollection.push_back( genlep );
		}
	}
	else if( MuonType == "fromTau")
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isDirectPromptTauDecayProductFinalState )
				GenLeptonCollection.push_back( genlep );
		}

	}
	else if( MuonType == "fromHardProcess" )
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}
	}
	else
	{
		cout << "Incorrect MuonType!" << endl;
		return;
	}

	//Give Acceptance Cuts
	if( GenLeptonCollection.size() >= 2 )
	{
		GenLepton leadGenLep, subGenLep;
		CompareGenLepton(&GenLeptonCollection[0], &GenLeptonCollection[1], &leadGenLep, &subGenLep);
		if( !(leadGenLep.Pt > LeadPtCut && subGenLep.Pt > SubPtCut && abs(leadGenLep.eta) < LeadEtaCut && abs(subGenLep.eta) < SubEtaCut) )
			GenLeptonCollection.clear();
	}


	
	Int_t NMuons = (Int_t)MuonCollection->size();
	vector<Muon> RecoMuonCollection;
	//Copy all muons in MuonCollection into RecoMuonCollection
	for(Int_t i_mu=0; i_mu<NMuons; i_mu++)
		RecoMuonCollection.push_back( MuonCollection->at(i_mu) );

	MuonCollection->clear();

	Int_t NGen = (Int_t)GenLeptonCollection.size();
	for(Int_t i_gen=0; i_gen<NGen; i_gen++)
	{
		GenLepton genlep = GenLeptonCollection[i_gen];
		Double_t gen_Pt = genlep.Pt;
		Double_t gen_eta = genlep.eta;
		Double_t gen_phi = genlep.phi;

		Int_t i_matched = -1;
		Double_t dPtMin = 1e10;
		for(Int_t i_reco=0; i_reco<NMuons; i_reco++)
		{
			Muon mu = RecoMuonCollection[i_reco];
			Double_t reco_Pt = mu.Pt;
			Double_t reco_eta = mu.eta;
			Double_t reco_phi = mu.phi;

			Double_t dR = sqrt( (gen_eta-reco_eta)*(gen_eta-reco_eta) + (gen_phi-reco_phi)*(gen_phi-reco_phi) );
			Double_t dPt = fabs(gen_Pt - reco_Pt);
			if( dR < 0.3 )
			{
				if( dPt < dPtMin )
				{
					i_matched = i_reco;
					dPtMin = dPt;
				}
			}
		}

		if( i_matched != -1 )
			MuonCollection->push_back( RecoMuonCollection[i_matched] );
	}

	return;
}

void DYAnalyzer::ConvertToTunePInfo( Muon &mu )
{
	// -- Use TuneP information -- //
	mu.Pt = mu.TuneP_pT;
	mu.eta = mu.TuneP_eta;
	mu.phi = mu.TuneP_phi;

	Double_t Px = mu.TuneP_Px;
	Double_t Py = mu.TuneP_Py;
	Double_t Pz = mu.TuneP_Pz;
	Double_t E = sqrt( Px*Px + Py*Py + Pz*Pz + M_Mu*M_Mu );
	mu.Momentum.SetPxPyPzE( Px, Py, Pz, E );
}

void DYAnalyzer::PrintOutDoubleMuInfo( Muon mu1, Muon mu2 )
{
	printf("\t[Muon1] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", mu1.Pt, mu1.eta, mu1.phi, mu1.charge);
	printf("\t[Muon2] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", mu2.Pt, mu2.eta, mu2.phi, mu2.charge);
	Double_t reco_M = ( mu1.Momentum + mu2.Momentum ).M();
	printf("\t\tDilepton Mass: %10.5lf\n", reco_M);

}

Bool_t DYAnalyzer::EventSelection_Dijet(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > PassingMuonCollection;
	vector< Muon > FailingMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() )
	    {
	    	if( MuonCollection[j].trkiso < 0.10 )
	    		PassingMuonCollection.push_back( MuonCollection[j] );
	    	else
	    		FailingMuonCollection.push_back( MuonCollection[j] );
	    }
	}

	Int_t nFailMuon = (Int_t)FailingMuonCollection.size();

	if( nFailMuon >= 2 ) // -- Dijet events: contains more than 2 failing muons regardless of # passing muons -- // 
	{
		if( nFailMuon == 2 )
		{
			Muon recolep1 = FailingMuonCollection[0];
			Muon recolep2 = FailingMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			Bool_t isOS = kFALSE;
			if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}

		} // -- end of if( nFailMuon == 2 ) -- //
		else // -- # failing muons > 2 -- // 
		{
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nFailMuon; i_mu++)
			{
				Muon Mu = FailingMuonCollection[i_mu];

				// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
				for(Int_t j_mu=0; j_mu<nFailMuon; j_mu++)
				{
					Muon Mu_jth = FailingMuonCollection[j_mu];

					if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
					{
						// -- Check that this pair is within acceptance -- //
						Bool_t isPassAcc = kFALSE;
						isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

						if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
						{
							Double_t VtxProb_temp = -999;
							Double_t VtxNormChi2_temp = 999;
							DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

							// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
							if( VtxNormChi2_temp < VtxNormChi2_BestPair )
							{
								VtxNormChi2_BestPair = VtxNormChi2_temp;
								mu1_BestPair = Mu;
								mu2_BestPair = Mu_jth;
							}
						}
					}
				} // -- end of the loop for j_mu (finding for second muon)
			} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

			if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
			{
				TLorentzVector reco_v1 = mu1_BestPair.Momentum;
				TLorentzVector reco_v2 = mu2_BestPair.Momentum;
				Double_t reco_M = (reco_v1 + reco_v2).M();

				// -- 3D open angle is calculated using inner track information -- //
				// -- 3D open angle -- //
				Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

				Bool_t isOS = kFALSE;
				if( mu1_BestPair.charge != mu2_BestPair.charge ) isOS = kTRUE;

				if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- end of (# failing muons > 2) case -- //

	} // -- end of if( nFailMuon >= 2 ) -- //

	return isPassEventSelection;
}

Bool_t DYAnalyzer::EventSelection_Wjet(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > PassingMuonCollection;
	vector< Muon > FailingMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() )
	    {
	    	if( MuonCollection[j].trkiso < 0.10 )
	    		PassingMuonCollection.push_back( MuonCollection[j] );
	    	else
	    		FailingMuonCollection.push_back( MuonCollection[j] );
	    }
	}

	Int_t nFailMuon = (Int_t)FailingMuonCollection.size();
	Int_t nPassMuon = (Int_t)PassingMuonCollection.size();

	if( nFailMuon == 1 && nPassMuon == 1) // -- W+Jets events: exactly (# pass muon , # fail muon ) = (1, 1) -- //
	{
		Muon recolep1 = PassingMuonCollection[0]; // -- first one: passing muon -- //
		Muon recolep2 = FailingMuonCollection[1]; // -- second one: failing muon -- //

		// -- Check the Accpetance -- //
		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		Double_t VtxProb = -999;
		Double_t VtxNormChi2 = 999;
		DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

		TLorentzVector inner_v1 = recolep1.Momentum_Inner;
		TLorentzVector inner_v2 = recolep2.Momentum_Inner;

		// -- 3D open angle -- //
		Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

		Bool_t isOS = kFALSE;
		if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

		// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
		if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedMuonCollection->push_back( recolep1 ); // -- first one: passing muon -- //
			SelectedMuonCollection->push_back( recolep2 ); // -- second one: failing muon -- //
		}
	}

	return isPassEventSelection;
}

Bool_t DYAnalyzer::EventSelection_CheckMoreThanOneDimuonCand(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection, Bool_t& isMoreThanOneCand) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;
	isMoreThanOneCand = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() && MuonCollection[j].trkiso < 0.10)
	        QMuonCollection.push_back( MuonCollection[j] );
	}

	// -- Check the existence of at least one muon matched with HLT-object -- //
	Bool_t isExistHLTMatchedMuon = kFALSE;
	for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
	{
		Muon mu = QMuonCollection[i_mu];
		if( mu.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || mu.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
		{
			isExistHLTMatchedMuon = kTRUE;
			break;
		}
	}

	if( isExistHLTMatchedMuon == kTRUE )
	{
		Int_t nQMuons = (Int_t)QMuonCollection.size();
		if( nQMuons == 2)
		{
			Muon recolep1 = QMuonCollection[0];
			Muon recolep2 = QMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			Bool_t isOS = kFALSE;
			if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
		else if( nQMuons > 2 )
		{
			isMoreThanOneCand = kTRUE;
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
			{
				Muon Mu = QMuonCollection[i_mu];

				// -- at least 1 muon should be matched with HLT objects in best pair -- //
				if( Mu.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || Mu.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
				{
					// -- Mu in this loop: QMuon Matched with HLT object -- //

					// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
					for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
					{
						Muon Mu_jth = QMuonCollection[j_mu];

						if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
						{
							// -- Check that this pair is within acceptance -- //
							Bool_t isPassAcc = kFALSE;
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

							if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
							{
								Double_t VtxProb_temp = -999;
								Double_t VtxNormChi2_temp = 999;
								DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

								// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
								if( VtxNormChi2_temp < VtxNormChi2_BestPair )
								{
									VtxNormChi2_BestPair = VtxNormChi2_temp;
									mu1_BestPair = Mu;
									mu2_BestPair = Mu_jth;
								}
							}
						}
					} // -- end of the loop for j_mu (finding for second muon)
				}
			} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

			if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
			{
				TLorentzVector reco_v1 = mu1_BestPair.Momentum;
				TLorentzVector reco_v2 = mu2_BestPair.Momentum;
				Double_t reco_M = (reco_v1 + reco_v2).M();

				// -- 3D open angle is calculated using inner track information -- //
				// -- 3D open angle -- //
				Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

				Bool_t isOS = kFALSE;
				if( mu1_BestPair.charge != mu2_BestPair.charge ) isOS = kTRUE;

				if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}





