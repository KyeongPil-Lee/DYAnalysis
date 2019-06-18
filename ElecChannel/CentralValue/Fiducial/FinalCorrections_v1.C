// ---------------------------------------- post-FSR and Fiducial cross section results ---------------------------------------------- //

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TLorentzVector.h>
#include <math.h>
#include "/Users/KyeongPil_Lee/ROOT5/Unfolding/RooUnfold/src/RooUnfoldBayes.h"
#include "/Users/KyeongPil_Lee/ROOT5/Unfolding/RooUnfold/src/RooUnfold.h"


void Calculation_DiffXsec(TH1D* h_yield, RooUnfoldResponse* UnfoldRes1,
			  TH1D* h_EfficiencySF, TH1D* h_EffCorr, TH1D *h_xSec_dM_FSRCorr)
//void Calculation_DiffXsec(TH1D* h_yield, RooUnfoldResponse* UnfoldRes1,
//			  TH1D* h_EfficiencySF, TH1D *h_xSec_dM_FSRCorr)
{


  // -- Unfolding correction for detector resolution: apply on the data before applying Acc*Eff correction -- //
  RooUnfoldBayes *UnfoldBayes1 = new RooUnfoldBayes(UnfoldRes1, h_yield, 21); //Unfolding Data Histo
  Unfolded_Data = (TH1D*)UnfoldBayes1->Hreco();
  Unfolded_Data->SetName("h_yield_Unfolded");

  // -- Apply Efficiency correction factors to each Yield -- //
  ApplyFinal_EffCorr(h_yield_Unfolded, h_EfficiencySF, h_yield_Unfolded_SFCorr); //Apply SF Corrections

  // -- Eff Correction -- //
  ApplyFinal_EffCorr(h_yield_Unfolded_SFCorr, h_EffCorr, h_yield_Unfolded_EffCorr); //Apply Eff Corrections

  // -- Obtain differential cross section -- //
  Obtain_dSigma_dM(h_yield_Unfolded_EffCorr, h_xSec_dM_FSRCorr); // -- differential cross section -- //
  //Obtain_dSigma_dM(h_yield_Unfolded_SFCorr, h_xSec_dM_FSRCorr); // -- differential cross section -- //

  h_xSec_dM_FSRCorr->Scale(1/2258.066); // -- cross section -- //

}

//================= Function to compute Efficiency

void Calc_Eff(TH1D *Total, TH1D *Pass, TH1D *h) {
  int n = Pass->GetNbinsX();
	
  for(int i=1;i<n+1; i++){
    double low  = Pass->GetBinLowEdge(i);
    double high = Pass->GetBinLowEdge(i+1);
	
    double num = Pass->GetBinContent(i);
    double den = Total->GetBinContent(i);
    double denerr = (Total->GetBinContent(i)*Total->GetBinContent(i))/(Total->GetBinError(i)*Total->GetBinError(i));
    double Val = num/den;
    double errVal = sqrt(Val*(1-Val)/denerr);
	
    h->SetBinContent(i, Val);
    h->SetBinError(i, errVal);
			
    //cout<<low<<"   "<<high<<endl;
    //cout<<h->GetBinContent(i)<<endl;
    //cout<<""<<endl;
			
  }
}

//======================= Function to compute SF and Error

void Calc_SFCorr(TH1D *h_den, TH1D *h_num, TH1D *h_EfficiencySF) {

  int n = h_num->GetNbinsX();
	
  for(int i=1;i<n+1; i++){
    double numEff = h_num->GetBinContent(i);
    double numErr = h_num->GetBinError(i);
    double denEff = h_den->GetBinContent(i);
    double denErr = h_den->GetBinError(i);

    double scalefactor = numEff/denEff;
    double denerr1 = (h_den->GetBinContent(i)*h_den->GetBinContent(i))/(h_den->GetBinError(i)*h_den->GetBinError(i));
    double SFErr = sqrt(scalefactor*(1-scalefactor)/denerr1);

    //double SFErr = scalefactor * sqrt((denErr/denEff)*(denErr/denEff) + (numErr/numEff)*(numErr/numEff));

    //cout<<h_num->GetBinLowEdge(i)<<"   "<<h_num->GetBinLowEdge(i+1)<<endl;
    //cout<<"SF: "<<scalefactor<<endl;
    //cout<<""<<endl;

    h_EfficiencySF->SetBinContent(i, scalefactor);
    h_EfficiencySF->SetBinError(i, SFErr);
  }
}


//======================== Function to compute Bkg Subtracted Histogram
void ObtainYieldHistogram(TH1D *h_Data, TH1D *h_EMu, TH1D *h_WZ, TH1D *h_ZZ, TH1D *h_Dijet, TH1D *h_Wjet, TH1D *Final) {

  int n = Final->GetNbinsX();
	
  for(int i=1;i<n+1; i++){
	
    double data_org = h_Data->GetBinContent(i);
    double data_org_err = h_Data->GetBinError(i);

    //cout<<h_Data->GetBinLowEdge(i)<<"   "<<h_Data->GetBinLowEdge(i+1)<<endl;
    //cout<<"before subtraction: "<<data_org<<endl;
		
    double emu = h_EMu->GetBinContent(i);
    double emu_err = h_EMu->GetBinError(i);

    double wz  = h_WZ->GetBinContent(i);
    double wz_err  = h_WZ->GetBinError(i);

    double zz  = h_ZZ->GetBinContent(i);
    double zz_err  = h_ZZ->GetBinError(i);
		
    double qcd = h_Dijet->GetBinContent(i);
    double qcd_err = h_Dijet->GetBinError(i);
	
    double wjet = h_Wjet->GetBinContent(i);
    double wjet_err = h_Wjet->GetBinError(i);
		
    double data_bkgsub = data_org - (emu+wz+zz+wjet+qcd);
    double data_bkgsub_err = sqrt((data_org_err*data_org_err) + (emu_err*emu_err) + (wz_err*wz_err) + (zz_err*zz_err) + (wjet_err*wjet_err) + (qcd_err*qcd_err));

    //cout<<"after subtraction:  "<<data_bkgsub<<endl;
    //cout<<""<<endl;
	
    Final->SetBinContent(i, data_bkgsub);
    Final->SetBinError(i, data_bkgsub_err);

  }
}


//==================== Function to apply SF corrections
void ApplyFinal_EffCorr(TH1D *unfold, TH1D *effCorr, TH1D *effCorrFinal) {

  int n = effCorrFinal->GetNbinsX();
	
  for(int i=1;i<n+1; i++){
    double unfoldData = unfold->GetBinContent(i);
    double effCorrec  = effCorr->GetBinContent(i);
		
    double unfoldDataErr = unfold->GetBinError(i);
    double effCorrecErr  = effCorr->GetBinError(i);
		
    double FinalCorr = unfoldData/effCorrec;
    double FinalCorrErr = unfoldData/effCorrec * sqrt((unfoldDataErr/unfoldData)*(unfoldDataErr/unfoldData)+(effCorrecErr/effCorrec)*(effCorrecErr/effCorrec));

    effCorrFinal->SetBinContent(i,FinalCorr);
    effCorrFinal->SetBinError(i,FinalCorrErr);

  }
}


//==================== Function to calculate diff XSec
void Obtain_dSigma_dM(TH1D *h, TH1D *h1) {

  int n = h->GetNbinsX();
	
  for(int i=1;i<n+1; i++){
    double BinWidth = h->GetBinWidth(i);
		
    double xSec = h->GetBinContent(i);
    double xSec_dM = xSec/BinWidth;
		
    double error_before = h->GetBinError(i);
    double error_after = error_before/BinWidth;
	
    h1->SetBinContent(i, xSec_dM);
    h1->SetBinError(i, error_after);
  }
}


void FinalCorrections_v1() {

  gSystem->Load("libRooUnfold");

  //Acceptance & Efficiency
  TFile *f0 = TFile::Open("DYEE_AccEff_M10to3000.root");
	
  TH1D *h_EffPass = (TH1D*)f0->Get("h_mass_EffPass");
  TH1D *h_EffTotal = (TH1D*)f0->Get("h_mass_EffTotal");

  //Scale Factors
  TFile *f1 = TFile::Open("ROOTFile_EffSF.root");
  TH1D *h_mass_num = (TH1D*)f1->Get("hist_mass_num");
  TH1D *h_mass_den = (TH1D*)f1->Get("hist_mass_den");
	
  //============ Data & Data-Driven background
  TFile *f2 = TFile::Open("SingleElectron_UnScaleCorr_new.root"); //SingleElectron_UnScaleCorr_new_case2.root
  TFile *f3 = TFile::Open("estimated_EEMass_v2.root");
  TFile *f4 = TFile::Open("Est_fmMC_diBoson.root");
  TFile *f5 = TFile::Open("dijet_wjet_BkgFR.root");

  TH1D *hData  = (TH1D*)f2->Get("dieleMass");
  TH1D *hEMu   = (TH1D*)f3->Get("estimated_EEMass");
  TH1D *hWZ    = (TH1D*)f4->Get("h_wz_corr_Mass");
  TH1D *hZZ    = (TH1D*)f4->Get("h_zz_corr_Mass");
  TH1D *hDijet = (TH1D*)f5->Get("DiJet_fromData_ControlRegion");
  TH1D *hWjet  = (TH1D*)f5->Get("WJets_fromData_ControlRegion");
	
  //============ Unfolding
  TFile *f6 = TFile::Open("DYEE_M10to3000_detUnfold.root");
  RooUnfoldResponse *UnfoldRes = (RooUnfoldResponse *)f6->Get("response"); // default sample
  TH1D *h_diffXsec_aMCNLO = (TH1D*)UnfoldRes->Htruth();

  const Int_t nMassBin = 43;
  Double_t MassBinEdges[44] = {15,20,25,30,35,40,45,50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,
			       126,133,141,150,160,171,185,200,220,243,273,320,380,
			       440,510,600,700,830,1000,1500,3000};

  TFile *file = new TFile("FinalCorr_postFSR_v11.root","RECREATE"); // Nominal

  //TFile *file = new TFile("FinalCorr_postFSR_v1_Test1.root","RECREATE");
  //TFile *file = new TFile("FinalCorr_postFSR_v1_Test2.root","RECREATE");
  //TFile *file = new TFile("FinalCorr_postFSR_v1_Test3.root","RECREATE");
  //TFile *file = new TFile("FinalCorr_postFSR_v1_Test4.root","RECREATE");

  //TFile *file = new TFile("FinalCorr_altSig_detUnfold.root","RECREATE");
	
  // Histograms declaration
  //*****************************************************************************************************

  TH1D *h_EffCorr = new TH1D("h_EffCorr", "", nMassBin, MassBinEdges);
  TH1D *h_EfficiencySF = new TH1D("h_EfficiencySF", "", nMassBin, MassBinEdges);

  TH1D *h_yield = new TH1D("h_yield", "", nMassBin, MassBinEdges);
  TH1D *h_yield_Unfolded_SFCorr = new TH1D("h_yield_Unfolded_SFCorr", "", nMassBin, MassBinEdges);
  TH1D *h_yield_Unfolded_EffCorr = new TH1D("h_yield_Unfolded_EffCorr", "", nMassBin, MassBinEdges);

  TH1D *h_xSec_dM_FSRCorr = new TH1D("h_xSec_dM_FSRCorr", "", nMassBin, MassBinEdges);
  TH1D *h_xSec_dM_FSRCorr_aMCNLO = new TH1D("h_xSec_dM_FSRCorr_aMC@NLO", "", nMassBin, MassBinEdges);
	
	
  // Call Functions
  //***********************************************************************************************************
  Calc_Eff(h_EffTotal, h_EffPass, h_EffCorr); //Computing Efficiency

  Calc_SFCorr(h_mass_den, h_mass_num, h_EfficiencySF);
	
  ObtainYieldHistogram(hData, hEMu, hWZ, hZZ, hDijet, hWjet, h_yield); //Background Subtracted Data Histo
	
  Calculation_DiffXsec(h_yield, UnfoldRes, h_EfficiencySF, h_EffCorr, h_xSec_dM_FSRCorr);
  //Calculation_DiffXsec(h_yield, UnfoldRes, h_EfficiencySF, h_xSec_dM_FSRCorr);

  Obtain_dSigma_dM(h_diffXsec_aMCNLO, h_xSec_dM_FSRCorr_aMCNLO);
  h_xSec_dM_FSRCorr_aMCNLO->Scale(1/2258.066 ); // -- cross section aMC@NLO -- //
	

  //==================== Write histograms in File ===============================

  h_EfficiencySF->Write();
  h_yield->Write();
  h_yield_Unfolded->Write();
  h_yield_Unfolded_SFCorr->Write();
  h_yield_Unfolded_EffCorr->Write();
  h_xSec_dM_FSRCorr->Write();
  h_xSec_dM_FSRCorr_aMCNLO->Write();
	
  file->Close();

} // FinalCorrections_v1 ends
