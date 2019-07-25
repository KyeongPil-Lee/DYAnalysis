#pragma once

#include <ElecChannel/DXSec/DXSecProducer.h>
#include <vector>
// -- when using ROOT5 for RooUnfold, SimplePlotTools.h makes error ... it should be excluded in the code
// -- when using ROOT6 (v6.02) in muon server, RooUnfold doesn't work... :(
// #include <ElecChannel/Include/DYElectronTool.h>

#define nEffMap 2000

class SmearedDXSecProducer
{
public:
  TString uncType_;
  TString fileName_effSF_;

  TH1D* h_dXSec_cv_;
  vector<TH1D*> vec_hist_smearedDXSec_;

  SmearedDXSecProducer(TString uncType)
  {
    uncType_ = uncType;
    fileName_effSF_ = "";
    Init();
  }

  SmearedDXSecProducer(TString uncType, TString fileName_effSF)
  {
    uncType_ = uncType;
    fileName_effSF_ = fileName_effSF;
    Init();
  }

  void Produce()
  {
    Save();
  }

private:
  void Init()
  {
    vec_hist_smearedDXSec_.clear();
    Generate_SmearedDXSec();
  }

  void Generate_SmearedDXSec()
  {
    if( fileName_effSF_ == "" )
    {
      TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
      TString fileName_effSF_ = analyzerPath+"/ElecChannel/Uncertainties/EfficiencySF/ROOTFile_SmearedEffSF_perMassBin_"+uncType_+".root";
    }

    TH1D* h_effSF_cv = Get_Hist( fileName_effSF_, "h_effSF_perMassBin_cv" );
    DXSecProducer* dXSecProducer_cv = new DXSecProducer();
    dXSecProducer_cv->UpdateEffSF( h_effSF_cv );
    dXSecProducer_cv->Produce();
    h_dXSec_cv_ = dXSecProducer_cv->DXSecHist();
    h_dXSec_cv_->SetName("h_dXSec_cv");

    for(Int_t i=0; i<nEffMap; i++)
    {
      TString numbering = TString::Format("%d", i);
      TString histName_smearedEffSF = "h_effSF_perMassBin_smeared_"+numbering;
      TH1D* h_smearedEffSF = Get_Hist( fileName_effSF_, histName_smearedEffSF );

      DXSecProducer* dXSecProducer = new DXSecProducer();
      dXSecProducer->UpdateEffSF( h_smearedEffSF );
      dXSecProducer->Produce();
      TH1D* h_dXSec_temp = dXSecProducer->DXSecHist();
      h_dXSec_temp->SetName("h_dXSec_smeared_"+numbering);
      vec_hist_smearedDXSec_.push_back( h_dXSec_temp );
    }
  }

  void Save()
  {
    TFile *f_output = TFile::Open("ROOTFile_SmearedDXSecProducer_"+uncType_+".root", "RECREATE");
    f_output->cd();

    h_dXSec_cv_->Write();
    for(Int_t i=0; i<nEffMap; i++)
      vec_hist_smearedDXSec_[i]->Write();

    f_output->Close();
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
};
