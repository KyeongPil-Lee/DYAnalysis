#include <ElecChannel/EfficiencySF/TnPEfficiency.h>

// #define nEffMap 2000
// #define nEffMap 10

class SmearedEffSFTool
{
public:
  TString uncType_;

  Int_t nEffMap_ = 2000;

  // -- index used for smeared eff.SF histogram name
  Bool_t setIndexRange_ = kFALSE;
  Int_t index_start_ = -1;
  Int_t index_end_ = -1;

  TH1D* h_mass_effPass_noSF_;

  // -- cv = central value
  TnPEfficiency* tnpEff_cv_;
  TH1D* h_mass_effPass_withSF_cv_;
  TH1D* h_effSF_perMassBin_cv_;

  vector<TnPEfficiency*> vec_tnpEff_smeared_;
  vector<TH1D*> vec_hist_mass_effPass_withSF_smeared_;
  vector<TH1D*> vec_hist_effSF_perMassBin_smeared_;

  Bool_t doFlipFlop_ = kFALSE;

  TString fileName_reco_ = "";
  TString fileName_ID_   = "";
  TString fileName_trig_ = "";
  Bool_t usePrivateTnP_ = kFALSE;

  TString fileName_output_ = "";

  SmearedEffSFTool(TString uncType)
  {
    uncType_ = uncType;
  }

  SmearedEffSFTool(TString uncType, Bool_t doFlipFlop )
  {
    uncType_ = uncType;
    doFlipFlop_ = doFlipFlop;
  }

  SmearedEffSFTool(TString uncType, TString fileName_reco, TString fileName_ID, TString fileName_trig )
  {
    usePrivateTnP_ = kTRUE;

    uncType_ = uncType;

    fileName_reco_ = fileName_reco;
    fileName_ID_   = fileName_ID;
    fileName_trig_ = fileName_trig;

    cout << "use private TnP root file" << endl;
    cout << "  reconstruction efficiency: " << fileName_reco_ << endl;
    cout << "  ID efficiency: "             << fileName_ID_   << endl;
    cout << "  Trigger efficiency: "        << fileName_trig_ << endl;
  }

  void Set_nEffMap(Double_t n)
  {
    nEffMap_ = n;
    printf("[SmearedEffSFTool::Set_nEffMap] nEffMap = %d\n", nEffMap_);
  }

  void Set_IndexRange(Int_t i_start, Int_t i_end)
  {
    setIndexRange_ = kTRUE;
    index_start_ = i_start;
    index_end_   = i_end;
    nEffMap_ = (i_end - i_start) + 1;

    printf("[SmearedEffSFTool::Set_IndexRange] the range of the index: %d < index < %d (nEffMap = %d)\n", index_start_, index_end_, nEffMap_);
  }

  void Set_OutputFileName(TString name )
  {
    fileName_output_ = name;
  }

  void Produce()
  {
    Init();
    ProduceMassHist();
    Save();
  }

private:
  void Init()
  {
    h_mass_effPass_noSF_ = DYTool::MakeHist_DXSecBin("h_mass_effPass_noSF");

    if( !usePrivateTnP_ ) tnpEff_cv_ = new TnPEfficiency();
    else                  tnpEff_cv_ = new TnPEfficiency(fileName_reco_, fileName_ID_, fileName_trig_);

    h_mass_effPass_withSF_cv_ = DYTool::MakeHist_DXSecBin("h_mass_effPass_withSF_cv");
    h_effSF_perMassBin_cv_ = nullptr; // -- will be made at the end of ProduceMassHist

    for(Int_t i=0; i<nEffMap_; i++)
    {
      TnPEfficiency* tnpEff_smeared_ = nullptr;
      if( !usePrivateTnP_ ) tnpEff_smeared_ = new TnPEfficiency();
      else                  tnpEff_smeared_ = new TnPEfficiency(fileName_reco_, fileName_ID_, fileName_trig_);

      if( doFlipFlop_ ) tnpEff_smeared_->doFlipFlop_ = kTRUE;
      tnpEff_smeared_->SmearingEff_GivenUncType(uncType_);
      vec_tnpEff_smeared_.push_back( tnpEff_smeared_ );


      TString numbering = "";
      if( setIndexRange_ ) numbering = TString::Format("%d", index_start_ + i);
      else                 numbering = TString::Format("%d", i);
      TH1D* h_mass_effPass_withSF_smeared_ = DYTool::MakeHist_DXSecBin("h_mass_effPass_withSF_smeared_"+numbering);
      vec_hist_mass_effPass_withSF_smeared_.push_back( h_mass_effPass_withSF_smeared_ );
    }

    cout << "[SmearedEffSFTool::Init] Generation of " << nEffMap_ << " randomized efficiency maps: done" << endl;
    cout << endl;
  }

  void ProduceMassHist()
  {
    TString dataPath = gSystem->Getenv("KP_DATA_PATH");
    TFile *f_ntuple = TFile::Open(dataPath+"/DY_forEff_M10to3000.root");
    Double_t pt1, pt2, eta1, eta2, diM, lumiWeight, genWeight, puWeight;

    TTree *ntuple;
    ntuple = (TTree*)f_ntuple->Get("tree");

    ntuple->SetBranchAddress("Ele1PT",&pt1);
    ntuple->SetBranchAddress("Ele1Eta",&eta1);
    ntuple->SetBranchAddress("Ele2PT",&pt2);
    ntuple->SetBranchAddress("Ele2Eta",&eta2);
    ntuple->SetBranchAddress("ZMass",&diM);
    ntuple->SetBranchAddress("lumiWeights",&lumiWeight);
    ntuple->SetBranchAddress("genWeights",&genWeight);
    ntuple->SetBranchAddress("PUWeights",&puWeight);

    Long64_t nEvent = ntuple->GetEntries();
    cout << "nEvent: " << nEvent << endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      DYTool::loadBar(i_ev+1, nEvent, 100, 100);

      ntuple->GetEvent(i_ev);

      h_mass_effPass_noSF_->Fill( diM, lumiWeight*genWeight*puWeight );

      Double_t eventSF_cv = tnpEff_cv_->EfficiencySF_EventWeight( pt1, eta1, pt2, eta2 );
      h_mass_effPass_withSF_cv_->Fill( diM, lumiWeight*genWeight*puWeight*eventSF_cv );

      for(Int_t i=0; i<nEffMap_; i++)
      {
        Double_t eventSF_smeared = vec_tnpEff_smeared_[i]->EfficiencySF_EventWeight( pt1, eta1, pt2, eta2 );
        vec_hist_mass_effPass_withSF_smeared_[i]->Fill( diM, lumiWeight*genWeight*puWeight*eventSF_smeared );
      }
    } // -- end of event iteration

    h_effSF_perMassBin_cv_ = (TH1D*)h_mass_effPass_noSF_->Clone();
    h_effSF_perMassBin_cv_->SetName("h_effSF_perMassBin_cv");
    h_effSF_perMassBin_cv_->Divide( h_mass_effPass_withSF_cv_, h_mass_effPass_noSF_ );

    for(Int_t i=0; i<nEffMap_; i++)
    {
      TH1D* h_effSF_perMassBin_smeared = (TH1D*)h_mass_effPass_noSF_->Clone();

      TString numbering = "";
      if( setIndexRange_ ) numbering = TString::Format("%d", index_start_ + i);
      else                 numbering = TString::Format("%d", i);
      h_effSF_perMassBin_smeared->SetName("h_effSF_perMassBin_smeared_"+numbering);
      h_effSF_perMassBin_smeared->Divide( vec_hist_mass_effPass_withSF_smeared_[i], h_mass_effPass_noSF_ );
      vec_hist_effSF_perMassBin_smeared_.push_back( h_effSF_perMassBin_smeared );
    }

    cout << "[SmearedEffSFTool::ProduceMassHist] Done" << endl;
  }

  void Save()
  {
    TString fileName = "";
    if( fileName_output_ == "" )
    {
      if(setIndexRange_) fileName = "ROOTFile_SmearedEffSF_perMassBin_"+uncType_+TString::Format("_%d_to_%d.root", index_start_, index_end_);
      else               fileName = "ROOTFile_SmearedEffSF_perMassBin_"+uncType_+".root";
    }
    else fileName = fileName_output_;

    TFile *f_output = TFile::Open(fileName, "RECREATE");
    f_output->cd();
    h_mass_effPass_noSF_->Write();
    h_mass_effPass_withSF_cv_->Write();
    h_effSF_perMassBin_cv_->Write();

    for(Int_t i=0; i<nEffMap_; i++)
      vec_hist_effSF_perMassBin_smeared_[i]->Write();

    f_output->Close();
    cout << "Output file: " << fileName << endl;
  }

};

void SmearedEffSF_perMassBin()
{
  SmearedEffSFTool* tool = new SmearedEffSFTool("bkgChange");
  tool->Set_IndexRange(500, 509);
  tool->Set_OutputFileName( "ROOTFile_SmearedEffSF_perMassBin_TEST.root" );
  tool->Produce();
}