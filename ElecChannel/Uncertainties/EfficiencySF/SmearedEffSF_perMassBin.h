#include <ElecChannel/EfficiencySF/TnPEfficiency.h>

#define nEffMap 2000
// #define nEffMap 10

class SmearedEffSFTool
{
public:
  TString uncType_;

  TH1D* h_mass_effPass_noSF_;

  // -- cv = central value
  TnPEfficiency* tnpEff_cv_;
  TH1D* h_mass_effPass_withSF_cv_;
  TH1D* h_effSF_perMassBin_cv_;

  TnPEfficiency* tnpEff_smeared_[nEffMap];
  TH1D* h_mass_effPass_withSF_smeared_[nEffMap];
  TH1D* h_effSF_perMassBin_smeared_[nEffMap];

  Bool_t doFlipFlop_ = kFALSE;

  SmearedEffSFTool(TString uncType)
  {
    uncType_ = uncType;
    Init();
  }

  SmearedEffSFTool(TString uncType, Bool_t doFlipFlop )
  {
    uncType_ = uncType;
    doFlipFlop_ = doFlipFlop;
    Init();
  }

  void Produce()
  {
    ProduceMassHist();
    Save();
  }

private:
  void Init()
  {
    h_mass_effPass_noSF_ = DYTool::MakeHist_DXSecBin("h_mass_effPass_noSF");

    tnpEff_cv_ = new TnPEfficiency();

    h_mass_effPass_withSF_cv_ = DYTool::MakeHist_DXSecBin("h_mass_effPass_withSF_cv");
    h_effSF_perMassBin_cv_ = nullptr; // -- will be made at the end of ProduceMassHist

    for(Int_t i=0; i<nEffMap; i++)
    {
      tnpEff_smeared_[i] = new TnPEfficiency();
      if( doFlipFlop_ ) tnpEff_smeared_[i]->doFlipFlop_ = kTRUE;
      tnpEff_smeared_[i]->SmearingEff_GivenUncType(uncType_);

      TString numbering = TString::Format("%d", i);
      h_mass_effPass_withSF_smeared_[i] = DYTool::MakeHist_DXSecBin("h_mass_effPass_withSF_smeared_"+numbering);
      h_effSF_perMassBin_smeared_[i] = nullptr;
    }
  }

  void ProduceMassHist()
  {
    TString dataPath = gSystem->Getenv("KP_DATA_PATH");
    TFile *f_ntuple = TFile::Open(dataPath+"/DY_forEff_M10to3000.root");
    Double_t pt1, pt2, eta1, eta2, diM, lumiWeight, genWeight;

    TTree *ntuple;
    ntuple = (TTree*)f_ntuple->Get("tree");
    Long64_t nTotEvent = ntuple->GetEntries();
    cout << "Total # events: "<< nTotEvent << endl;
    cout << endl;

    ntuple->SetBranchAddress("Ele1PT",&pt1);
    ntuple->SetBranchAddress("Ele1Eta",&eta1);
    ntuple->SetBranchAddress("Ele2PT",&pt2);
    ntuple->SetBranchAddress("Ele2Eta",&eta2);
    ntuple->SetBranchAddress("ZMass",&diM);
    ntuple->SetBranchAddress("lumiWeights",&lumiWeight);
    ntuple->SetBranchAddress("genWeights",&genWeight);

    Int_t nEvent = ntuple->GetEntries();
    cout << "nEvent: " << nEvent << endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      DYTool::loadBar(i_ev+1, nEvent, 100, 100);

      ntuple->GetEvent(i_ev);

      h_mass_effPass_noSF_->Fill( diM, lumiWeight*genWeight );

      Double_t eventSF_cv = tnpEff_cv_->EfficiencySF_EventWeight( pt1, eta1, pt2, eta2 );
      h_mass_effPass_withSF_cv_->Fill( diM, lumiWeight*genWeight*eventSF_cv );

      for(Int_t i=0; i<nEffMap; i++)
      {
        Double_t eventSF_smeared = tnpEff_smeared_[i]->EfficiencySF_EventWeight( pt1, eta1, pt2, eta2 );
        h_mass_effPass_withSF_smeared_[i]->Fill( diM, lumiWeight*genWeight*eventSF_smeared );
      }
    } // -- end of event iteration

    h_effSF_perMassBin_cv_ = (TH1D*)h_mass_effPass_noSF_->Clone();
    h_effSF_perMassBin_cv_->SetName("h_effSF_perMassBin_cv");
    h_effSF_perMassBin_cv_->Divide( h_mass_effPass_withSF_cv_, h_mass_effPass_noSF_ );

    for(Int_t i=0; i<nEffMap; i++)
    {
      h_effSF_perMassBin_smeared_[i] = (TH1D*)h_mass_effPass_noSF_->Clone();

      TString numbering = TString::Format("%d", i);
      h_effSF_perMassBin_smeared_[i]->SetName("h_effSF_perMassBin_smeared_"+numbering);
      h_effSF_perMassBin_smeared_[i]->Divide( h_mass_effPass_withSF_smeared_[i], h_mass_effPass_noSF_ );
    }

    cout << "[SmearedEffSFTool::ProduceMassHist] Done" << endl;
  }

  void Save()
  {
    TString fileName = "ROOTFile_SmearedEffSF_perMassBin_"+uncType_+".root";
    TFile *f_output = TFile::Open(fileName, "RECREATE");
    f_output->cd();
    h_mass_effPass_noSF_->Write();
    h_mass_effPass_withSF_cv_->Write();
    h_effSF_perMassBin_cv_->Write();

    for(Int_t i=0; i<nEffMap; i++)
      h_effSF_perMassBin_smeared_[i]->Write();

    f_output->Close();
    cout << "Output file: " << fileName << endl;
  }

};

void SmearedEffSF_perMassBin()
{
  SmearedEffSFTool* tool = new SmearedEffSFTool("bkgChange");
  tool->Produce();
}