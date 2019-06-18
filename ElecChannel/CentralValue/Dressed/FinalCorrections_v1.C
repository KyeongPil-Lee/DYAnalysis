#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TLorentzVector.h>
#include <math.h>
#include "/Users/KyeongPil_Lee/ROOT5/Unfolding/RooUnfold/src/RooUnfoldBayes.h"
#include "/Users/KyeongPil_Lee/ROOT5/Unfolding/RooUnfold/src/RooUnfold.h"


void Calculation_DiffXsec(TH1D* h_yield, RooUnfoldResponse* UnfoldRes1,
			  TH1D* h_EfficiencySF, TH1D* h_AccEff,
			  RooUnfoldResponse* UnfoldRes3, TH1D *h_xSec_dM_FSRCorr)
{


  // -- Unfolding correction for detector resolution: apply on the data before applying Acc*Eff correction -- //
  RooUnfoldBayes *UnfoldBayes_detUn = new RooUnfoldBayes(UnfoldRes1, h_yield, 21); //Unfolding Data Histo
  Unfolded_Data = (TH1D*)UnfoldBayes_detUn->Hreco();
  Unfolded_Data->SetName("h_yield_Unfolded");

  // -- Apply Efficiency correction factors to each Yield -- //
  ApplyFinal_EffCorr(h_yield_Unfolded, h_EfficiencySF, h_yield_Unfolded_SFCorr); //Apply SF Corrections

  // -- Acc*Eff Correction -- //
  ApplyFinal_EffCorr(h_yield_Unfolded_SFCorr, h_AccEff, h_yield_Unfolded_AccEff); //Apply Acc*Eff Corrections

  // -- FSR Correction -- //
  RooUnfoldBayes *UnfoldBayes_FSRUn = new RooUnfoldBayes(UnfoldRes3, h_yield_Unfolded_AccEff, 21);
  FSR_Corrected = (TH1D*)UnfoldBayes_FSRUn->Hreco();
  FSR_Corrected->SetName("h_FSRCorrected");

  // -- Obtain differential cross section -- //
  Obtain_dSigma_dM(h_FSRCorrected, h_xSec_dM_FSRCorr); // -- differential cross section -- //

  h_xSec_dM_FSRCorr->Scale(1/2258.066); // -- cross section -- //

  /*for(int i=1; i<44; i++){
   cout<<h_xSec_dM_FSRCorr->GetBinLowEdge(i)<<"   "<<h_xSec_dM_FSRCorr->GetBinLowEdge(i+1)<<endl;
   cout<<"Xsec: "<<h_xSec_dM_FSRCorr->GetBinContent(i)<<"   "<<"error: "<<h_xSec_dM_FSRCorr->GetBinError(i)<<endl;
   cout<<""<<endl;
  }*/

}


//================= Function to compute Acceptance and Efficiency

void Calc_Acc_Eff(TH1D *Total, TH1D *Pass, TH1D *h) {
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
	

void Calc_AccEff(TH1D *AccTotal, TH1D *AccPass, TH1D *EffTotal, TH1D *EffPass, TH1D *AccEff) {
  int n = AccPass->GetNbinsX();
	
  for(int i=1;i<n+1; i++){
    double low  = AccPass->GetBinLowEdge(i);
    double high = AccPass->GetBinLowEdge(i+1);
	
    double numAcc = AccPass->GetBinContent(i);
    double denAcc = AccTotal->GetBinContent(i);
    double denerr = (AccTotal->GetBinContent(i)*AccTotal->GetBinContent(i))/(AccTotal->GetBinError(i)*AccTotal->GetBinError(i));
    double Acc = numAcc/denAcc;
    double errAcc = sqrt(Acc*(1-Acc)/denerr);
	
    double numEff = EffPass->GetBinContent(i);
    double denEff = EffTotal->GetBinContent(i);
    double denerr1 = (EffTotal->GetBinContent(i)*EffTotal->GetBinContent(i))/(EffTotal->GetBinError(i)*EffTotal->GetBinError(i));
    double Eff = numEff/denEff;
    double errEff = sqrt(Eff*(1-Eff)/denerr1);
	
    double totalEff = Acc * Eff;
    double totalErr = totalEff * sqrt((errAcc/Acc)*(errAcc/Acc) + (errEff/Eff)*(errEff/Eff));
	
    AccEff->SetBinContent(i, totalEff);
    AccEff->SetBinError(i, totalErr);
		
  }
}


void Calc_AccEff(TH1D *Acc, TH1D *Eff, TH1D *AccEff) {
  int n = Acc->GetNbinsX();
	
  for(int i=1;i<n+1; i++){
    double low  = Acc->GetBinLowEdge(i);
    double high = Acc->GetBinLowEdge(i+1);
	
    double acc = Acc->GetBinContent(i);
    double erracc = Acc->GetBinError(i);

    double eff = Eff->GetBinContent(i);
    double erreff = Eff->GetBinError(i);
	
    double total = acc * eff;
    double totalerr = total * sqrt((erracc/acc)*(erracc/acc) + (erreff/eff)*(erreff/eff));
	
    AccEff->SetBinContent(i, total);
    AccEff->SetBinError(i, totalerr);
		
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

    //cout<<"error: "<<data_org_err<<"   "<<emu_err<<"   "<<wz_err<<"   "<<zz_err<<"   "<<wjet_err<<"   "<<qcd_err<<endl;

    //cout<<"after subtraction:  "<<data_bkgsub<<"   "<<data_bkgsub_err<<endl;
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

    //cout<<h1->GetBinLowEdge(i)<<"   "<<h1->GetBinLowEdge(i+1)<<endl;
    //cout<<"Xsec:" <<xSec_dM<<"   "<<"error: "<<error_after<<endl;

  }
}


void FinalCorrections_v1() {

  //gSystem->Load("libRooUnfold");

  //Acceptance & Efficiency
  TFile *f1 = TFile::Open("DYEE_AccEff_M10to3000.root");
  //TFile *f1 = TFile::Open("DYEE_AccEff_Fit_M10to3000.root");
	
  TH1D *h_AccPass = (TH1D*)f1->Get("h_mass_AccPass");
  TH1D *h_AccTotal = (TH1D*)f1->Get("h_mass_AccTotal");
	
  TH1D *h_EffPass = (TH1D*)f1->Get("h_mass_EffPass");
  TH1D *h_EffTotal = (TH1D*)f1->Get("h_mass_EffTotal");

  //TH1D *h_Acc = (TH1D*)f1->Get("h_Acc_Fit");
  //TH1D *h_Eff = (TH1D*)f1->Get("h_Eff");

  //Scale Factors
  TFile *f2 = TFile::Open("ROOTFile_EffSF.root");
  TH1D *h_mass_num = (TH1D*)f2->Get("hist_mass_num");
  TH1D *h_mass_den = (TH1D*)f2->Get("hist_mass_den");
	
  //============ Data & Data-Driven background
  TFile *f3 = TFile::Open("SingleElectron_UnScaleCorr_new.root"); //SingleElectron_UnScaleCorr_new_case2.root
  TFile *f4 = TFile::Open("estimated_EEMass_v2.root");
  TFile *f5 = TFile::Open("Est_fmMC_diBoson.root");
  TFile *f6 = TFile::Open("dijet_wjet_BkgFR.root");

  TH1D *hData  = (TH1D*)f3->Get("dieleMass");
  TH1D *hEMu   = (TH1D*)f4->Get("estimated_EEMass");
  TH1D *hWZ    = (TH1D*)f5->Get("h_wz_corr_Mass");
  TH1D *hZZ    = (TH1D*)f5->Get("h_zz_corr_Mass");
  TH1D *hDijet = (TH1D*)f6->Get("DiJet_fromData_ControlRegion");
  TH1D *hWjet  = (TH1D*)f6->Get("WJets_fromData_ControlRegion");
	
  //============ Unfolding
  TFile *f7 = TFile::Open("DYEE_M10to3000_detUnfold.root");
  RooUnfoldResponse *UnfoldRes1 = (RooUnfoldResponse *)f7->Get("response"); // default sample

  //TFile *f8 = TFile::Open("DYEE_M10to3500_detUnfold.root");
  TFile *f8 = TFile::Open("DYEE_M10to3000_NNLO_MC_altSig_detUnfold.root");
  RooUnfoldResponse *UnfoldRes2 = (RooUnfoldResponse *)f8->Get("response2"); // alternate sample

  TFile *f9 = TFile::Open("DYEE_M10to3000_FSRUnfold.root");
  RooUnfoldResponse *UnfoldRes3 = (RooUnfoldResponse *)f9->Get("response"); // FSR default sample

  TFile *f10 = TFile::Open("DYEE_M10toInf_FSRUnfold.root");
  //TFile *f10 = TFile::Open("/home/ridhi/Work/Analysis/76X/Unfolding/FSRCorr/PHOTOS/HighMass_Samples/v_07Dec/DYEE_M10to3000_FSRUnfold.root");
  RooUnfoldResponse *UnfoldRes4 = (RooUnfoldResponse *)f10->Get("response"); // FSR alternate sample
	
  TFile *f11 = TFile::Open("ROOTFile_DYXSecNNLO_WithPI.root"); // ROOTFile_xSec_Theory.root
  TH1D *h_diffXsec_FEWZ_WithoutPI = (TH1D*)f11->Get("h_DYDiffXSecNNLO_WithoutPI");

  TFile *f12 = TFile::Open("ROOTFile_xSec_aMC@NLO.root");
  TH1D *h_diffXsec_aMCNLO = (TH1D*)f12->Get("h_aMC@NLO_preFSR");
	
  const Int_t nMassBin = 43;
  Double_t MassBinEdges[44] = {15,20,25,30,35,40,45,50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,
			       126,133,141,150,160,171,185,200,220,243,273,320,380,
			       440,510,600,700,830,1000,1500,3000};

  TFile *file = new TFile("FinalCorr_v11.root","RECREATE"); // Nominal

  //TFile *file = new TFile("FinalCorr_v1_AccFit.root","RECREATE"); // Acceptance from Fitting

  //TFile *file = new TFile("FinalCorr_v1_Test1.root","RECREATE"); // Test
  //TFile *file = new TFile("FinalCorr_v1_Test2.root","RECREATE"); // Test
  //TFile *file = new TFile("FinalCorr_v1_Test3.root","RECREATE"); // Test
  //TFile *file = new TFile("FinalCorr_v1_Test4.root","RECREATE"); // Test

  //TFile *file = new TFile("FinalCorr_altSig_detUnfold.root","RECREATE");
  //TFile *file = new TFile("FinalCorr_NNLO_MC_altSig_detUnfold.root","RECREATE");

  //TFile *file = new TFile("FinalCorr_altSig_FSRUnfold.root","RECREATE");
  //TFile *file = new TFile("FinalCorr_altSig_FSRUnfold_HighMass_Samples.root","RECREATE");
	
  // Histograms declaration
  //*****************************************************************************************************

  TH1D *h_AccCorr = new TH1D("h_AccCorr", "", nMassBin, MassBinEdges);
  TH1D *h_EffCorr = new TH1D("h_EffCorr", "", nMassBin, MassBinEdges);
  TH1D *h_AccEff  = new TH1D("h_AccEff", "", nMassBin, MassBinEdges);
	
  TH1D *h_EfficiencySF = new TH1D("h_EfficiencySF", "", nMassBin, MassBinEdges);

  TH1D *h_yield = new TH1D("h_yield", "", nMassBin, MassBinEdges);
  TH1D *h_yield_Unfolded_SFCorr = new TH1D("h_yield_Unfolded_SFCorr", "", nMassBin, MassBinEdges);
  TH1D *h_yield_Unfolded_AccEff = new TH1D("h_yield_Unfolded_AccEff", "", nMassBin, MassBinEdges);

  TH1D *h_xSec_dM_FSRCorr = new TH1D("h_xSec_dM_FSRCorr", "", nMassBin, MassBinEdges);
  TH1D *h_xSec_dM_FSRCorr_aMCNLO = new TH1D("h_xSec_dM_FSRCorr_aMC@NLO", "", nMassBin, MassBinEdges);
	
	
  // Call Functions
  //***********************************************************************************************************
  Calc_Acc_Eff(h_AccTotal, h_AccPass, h_AccCorr); //Computing Acceptance
  Calc_Acc_Eff(h_EffTotal, h_EffPass, h_EffCorr); //Computing Efficiency
  Calc_AccEff(h_AccTotal, h_AccPass, h_EffTotal, h_EffPass, h_AccEff); //Computing Acceptance * Eff
  //Calc_AccEff(h_Acc, h_Eff, h_AccEff); //Computing Acceptance * Eff

  Calc_SFCorr(h_mass_den, h_mass_num, h_EfficiencySF);
	
  ObtainYieldHistogram(hData, hEMu, hWZ, hZZ, hDijet, hWjet, h_yield); //Background Subtracted Data Histo
	
  Calculation_DiffXsec(h_yield, UnfoldRes1, h_EfficiencySF, h_AccEff, 
		       UnfoldRes3, h_xSec_dM_FSRCorr);

  Obtain_dSigma_dM(h_diffXsec_aMCNLO, h_xSec_dM_FSRCorr_aMCNLO);
  h_xSec_dM_FSRCorr_aMCNLO->Scale(1/2258.066); // -- cross section aMC@NLO -- //
	

  //==================== Write histograms in File ===============================

  h_AccCorr->Write();
  h_EffCorr->Write();
  h_AccEff->Write();
  h_EfficiencySF->Write();
  h_yield->Write();
  h_yield_Unfolded->Write();
  h_yield_Unfolded_SFCorr->Write();
  h_yield_Unfolded_AccEff->Write();
  h_FSRCorrected->Write();
  h_xSec_dM_FSRCorr->Write();
  h_diffXsec_FEWZ_WithoutPI->Write();
  h_xSec_dM_FSRCorr_aMCNLO->Write();

  //h_yield->Print("all");
	
  file->Close();

} // FinalCorrections_v1 ends
