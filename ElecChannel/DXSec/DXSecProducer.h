// -- remove dependence on the header files... to be complied under ROOT5 for RooUnfold usage

#include <TSystem.h>
#include <TFile.h>
#include <TH1D.h>
#include <TString.h>

#include <src/RooUnfold.h>
#include <src/RooUnfoldResponse.h>
#include <src/RooUnfoldBayes.h>

class DXSecProducer
{
public:
  TString analyzerPath_;
  TString basePath_;

  // -- input for diff. x-sections
  TH1D* h_yield_;
  RooUnfoldResponse* rooUnfoldRes_detRes_;
  TH1D* h_accEff_;
  TH1D* h_effSF_;
  RooUnfoldResponse* rooUnfoldRes_FSR_;

  // -- output
  TH1D* h_dXSec_;

  DXSecProducer()
  {
    Init();
  }

  void Produce()
  {
    Calc_dXSec();
  }

  TH1D* DXSecHist() { return h_dXSec_; }

  void UpdateEffSF( TH1D* h_effSF_new )
  {
    delete h_effSF_;
    h_effSF_ = (TH1D*)h_effSF_new->Clone();
  }

private:
  void Calc_dXSec()
  {
    // -- Unfolding correction for detector resolution: apply on the data before applying Acc*Eff correction -- //
    RooUnfoldBayes *UnfoldBayes_detUn = new RooUnfoldBayes(rooUnfoldRes_detRes_, h_yield_, 21); //Unfolding Data Histo
    TH1D* Unfolded_Data = (TH1D*)UnfoldBayes_detUn->Hreco();
    Unfolded_Data->SetName("h_yield_Unfolded");

    // -- Apply Efficiency correction factors to each Yield -- //
    TH1D* h_yield_Unfolded_SFCorr = ApplyFinal_EffCorr(Unfolded_Data, h_effSF_); //Apply SF Corrections

    // -- Acc*Eff Correction -- //
    TH1D* h_yield_Unfolded_AccEff = ApplyFinal_EffCorr(h_yield_Unfolded_SFCorr, h_accEff_); //Apply Acc*Eff Corrections

    // -- FSR Correction -- //
    RooUnfoldBayes *UnfoldBayes_FSRUn = new RooUnfoldBayes(rooUnfoldRes_FSR_, h_yield_Unfolded_AccEff, 21);
    TH1D* FSR_Corrected = (TH1D*)UnfoldBayes_FSRUn->Hreco();
    FSR_Corrected->SetName("h_FSRCorrected");

    // -- Obtain differential cross section -- //
    h_dXSec_ = DivideEachBin_ByBinWidth(FSR_Corrected); // -- differential cross section -- //    
    h_dXSec_->Scale(1/2258.066); // -- cross section -- //
  }
  void Init()
  {
    TH1::AddDirectory(kFALSE);

    analyzerPath_ = gSystem->Getenv("KP_ANALYZER_PATH");
    basePath_     = analyzerPath_+"/ElecChannel/Code_Ridhi/CentralValue/Dressed";

    Init_Yield();
    Init_RooUnfoldRes();
    Init_AccEff();
    Init_EffSF();
  }

  void Init_Yield()
  {

    TFile *f3 = TFile::Open(basePath_+"/SingleElectron_UnScaleCorr_new.root"); //SingleElectron_UnScaleCorr_new_case2.root
    TFile *f4 = TFile::Open(basePath_+"/estimated_EEMass_v2.root");
    TFile *f5 = TFile::Open(basePath_+"/Est_fmMC_diBoson.root");
    TFile *f6 = TFile::Open(basePath_+"/dijet_wjet_BkgFR.root");

    TH1D *hData  = (TH1D*)f3->Get("dieleMass");
    TH1D *hEMu   = (TH1D*)f4->Get("estimated_EEMass");
    TH1D *hWZ    = (TH1D*)f5->Get("h_wz_corr_Mass");
    TH1D *hZZ    = (TH1D*)f5->Get("h_zz_corr_Mass");
    TH1D *hDijet = (TH1D*)f6->Get("DiJet_fromData_ControlRegion");
    TH1D *hWjet  = (TH1D*)f6->Get("WJets_fromData_ControlRegion");

    h_yield_ = MakeYieldHist(hData, hEMu, hWZ, hZZ, hDijet, hWjet);

    delete f3;
    delete f4;
    delete f5;
    delete f6;
    delete hData;
    delete hEMu;
    delete hWZ;
    delete hZZ;
    delete hDijet;
    delete hWjet;
  }

  void Init_RooUnfoldRes()
  {
    TFile *f7 = TFile::Open(basePath_+"/DYEE_M10to3000_detUnfold.root");
    rooUnfoldRes_detRes_ = (RooUnfoldResponse *)f7->Get("response"); // default sample

    TFile *f9 = TFile::Open(basePath_+"/DYEE_M10to3000_FSRUnfold.root");
    rooUnfoldRes_FSR_ = (RooUnfoldResponse *)f9->Get("response"); // FSR default sample

    delete f7;
    delete f9;
  }

  void Init_AccEff()
  {
    TFile *f1 = TFile::Open(basePath_+"/DYEE_AccEff_M10to3000.root");

    TH1D *h_AccPass = (TH1D*)f1->Get("h_mass_AccPass");
    TH1D *h_AccTotal = (TH1D*)f1->Get("h_mass_AccTotal");
    
    TH1D *h_EffPass = (TH1D*)f1->Get("h_mass_EffPass");
    TH1D *h_EffTotal = (TH1D*)f1->Get("h_mass_EffTotal");

    h_accEff_ = Calc_AccEff(h_AccTotal, h_AccPass, h_EffTotal, h_EffPass); //Computing Acceptance * Eff

    delete f1;
    delete h_AccPass;
    delete h_AccTotal;
    delete h_EffPass;
    delete h_EffTotal;
  }

  void Init_EffSF()
  {

    TString fileName = analyzerPath_+"/ElecChannel/EfficiencySF/ROOTFile_EfficiencySF_perMassBin.root";
    TString histName = "h_effSF_perMassBin";
    h_effSF_ = Get_Hist(fileName, histName);
  }

  // -- values in Ridhi's input file
  // -- it is based on the old TnP efficiency (official value by E/gamma POG)
  void Init_EffSF_Old()
  {
    TFile *f2 = TFile::Open(basePath_+"/ROOTFile_EffSF.root");
    TH1D *h_mass_num = (TH1D*)f2->Get("hist_mass_num");
    TH1D *h_mass_den = (TH1D*)f2->Get("hist_mass_den");

    h_effSF_ = Calc_SFCorr(h_mass_den, h_mass_num);

    delete f2;
    delete h_mass_num;
    delete h_mass_den;
  }

  TH1D* Calc_SFCorr(TH1D *h_den, TH1D *h_num)
  {
    TH1D* h_EfficiencySF = MakeHist_DXSecBin("h_effSF");

    int n = h_num->GetNbinsX();
    
    for(int i=1;i<n+1; i++)
    {
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

    return h_EfficiencySF;
  }

  TH1D* Calc_AccEff(TH1D *AccTotal, TH1D *AccPass, TH1D *EffTotal, TH1D *EffPass)
  {
    TH1D* AccEff = MakeHist_DXSecBin("h_accEff");

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

    return AccEff;
  }

  TH1D* MakeYieldHist(TH1D *h_Data, TH1D *h_EMu, TH1D *h_WZ, TH1D *h_ZZ, TH1D *h_Dijet, TH1D *h_Wjet)
  {
    TH1D* h_final = MakeHist_DXSecBin("h_yield");

    int n = h_final->GetNbinsX();
    
    for(int i=1;i<n+1; i++)
    {    
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
    
      h_final->SetBinContent(i, data_bkgsub);
      h_final->SetBinError(i, data_bkgsub_err);
    }

    return h_final;
  }

  TH1D* ApplyFinal_EffCorr(TH1D *unfold, TH1D *effCorr)
  {
    TH1D* effCorrFinal = MakeHist_DXSecBin("effCorrFinal");
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

    return effCorrFinal;
  }

  TH1D* DivideEachBin_ByBinWidth( TH1D* h, TString HistName = "" )
  {
    TH1D* h_return = (TH1D*)h->Clone();
    if( HistName != "" )
      h_return->SetName(HistName);

    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t Entry_before = h->GetBinContent(i_bin);
      Double_t Error_before = h->GetBinError(i_bin);
      Double_t BinWidth = h->GetBinWidth(i_bin);

      Double_t Entry_after = Entry_before / BinWidth;
      Double_t Error_after = Error_before / BinWidth;

      h_return->SetBinContent(i_bin, Entry_after);
      h_return->SetBinError(i_bin, Error_after);
    }

    return h_return;
  }

  TH1D* Get_Hist(TString fileName, TString histName, TString histName_new = "" )
  {
    TH1::AddDirectory(kFALSE);

    TFile* f_input = TFile::Open( fileName );
    TH1D* h_temp = (TH1D*)f_input->Get(histName)->Clone();
    if( histName_new != "" )
      h_temp->SetName( histName_new );

    f_input->Close();

    return h_temp;
  }

  TH1D* MakeHist_DXSecBin(TString histName)
  {
    Double_t arr_massBinEdge[44] = {15,20,25,30,35,40,45,50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,126,133,141,150,160,171,185,200,220,243,273,320,380,440,510,600,700,830,1000,1500,3000};
    return new TH1D(histName, "", 43, arr_massBinEdge);
  }

};