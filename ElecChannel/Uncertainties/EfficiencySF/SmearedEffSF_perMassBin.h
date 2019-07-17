#include <ElecChannel/EfficiencySF/TnPEfficiency.h>

#define nEffMap 2000

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

  SmearedEffSFTool(TString uncType)
  {
    uncType_ = uncType;
    Init();
  }

  void Produce()
  {
    vector<DYTool::SampleInfo> vec_sampleInfo_signal = DYTool::GetSampleInfo_Signal();
    for(const auto& sampleInfo : vec_sampleInfo_signal )
      ProduceMassHist(sampleInfo);

    Save();
  }

  void Produce_LocalTest()
  {
    DYTool::SampleInfo sampleInfo("/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/Include/Example/DY_2000to3000.root", 0.00054/3, 159.616, 2000, 3000);
    ProduceMassHist(sampleInfo);

    Save();
  }

private:
  void Init()
  {
    h_mass_effPass_noSF_      = DYTool::MakeHist_DXSecBin("h_mass_effPass_noSF");

    tnpEff_cv_ = new TnPEfficiency();

    h_mass_effPass_withSF_cv_ = DYTool::MakeHist_DXSecBin("h_mass_effPass_withSF_cv");
    h_effSF_perMassBin_cv_ = nullptr; // -- will be made at the end of ProduceMassHist

    for(Int_t i=0; i<nEffMap; i++)
    {
      tnpEff_smeared_[i] = new TnPEfficiency();
      tnpEff_smeared_[i]->SmearingEff_GivenUncType(uncType_);

      TString numbering = TString::Format("%d", i);
      h_mass_effPass_withSF_smeared_[i] = DYTool::MakeHist_DXSecBin("h_mass_effPass_withSF_smeared_"+numbering);
      h_effSF_perMassBin_smeared_[i] = nullptr;
    }
  }

  void ProduceMassHist(DYTool::SampleInfo sampleInfo)
  {
    cout << "[EffSFTool::ProduceMassHist] ntuple = " << sampleInfo.ntuplePath_ << endl;

    TChain *chain = new TChain("ntupler/ElectronTree");
    chain->Add(sampleInfo.ntuplePath_);

    NtupleHandle_Ele *ntuple = new NtupleHandle_Ele(chain);

    // -- setup for weights
    Double_t normFactor = sampleInfo.normFactor();
    // DYTool::PUWeightTool* puWeightTool = new DYTool::PUWeightTool(); // -- PU weight is not used as it will be canceled anyway

    // -- TnP efficiency information
    TnPEfficiency* tnpEff = new TnPEfficiency();

    Int_t nEvent = chain->GetEntries();
    cout << "nEvent: " << nEvent << endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      DYTool::loadBar(i_ev+1, nEvent, 100, 100);

      ntuple->GetEvent(i_ev);

      Double_t genWeight = ntuple->genWeight;
      // Double_t PUWeight = puWeightTool->Weight(ntuple->nPU);

      Bool_t doTrancate = kFALSE;
      if( sampleInfo.maxM_ == 100 ) doTrancate = kTRUE;

      if( DYTool::IsDYEEEvent(ntuple, doTrancate) )
      {
        Bool_t flag_passAcc = kFALSE;

        vector<DYGenEle_postFSR> vec_genEle = DYTool::GetAllGenEle_postFSR(ntuple);

        Double_t m_postFSR = (vec_genEle[0].vec_P + vec_genEle[1].vec_P).M();

        flag_passAcc = DYTool::DoPassAcc_ee( vec_genEle[0].pt, vec_genEle[0].eta, vec_genEle[1].pt, vec_genEle[1].eta );

        // -- check the efficiency only for the event passing acceptance
        if( flag_passAcc )
        {
          vector<DYElectron> vec_recoEle;
          Bool_t flag_passEff = DYTool::IsDYEECandidate( ntuple, vec_recoEle );

          if( flag_passEff )
          {
            h_mass_effPass_noSF_->Fill( m_postFSR, genWeight*normFactor );

            Double_t eventSF_cv = tnpEff_cv_->EfficiencySF_EventWeight( vec_recoEle[0].pt, vec_recoEle[0].eta, vec_recoEle[1].pt, vec_recoEle[1].eta );
            h_mass_effPass_withSF_cv_->Fill( m_postFSR, genWeight*normFactor*eventSF_cv );

            for(Int_t i=0; i<nEffMap; i++)
            {
              Double_t eventSF_smeared = tnpEff_smeared_[i]->EfficiencySF_EventWeight( vec_recoEle[0].pt, vec_recoEle[0].eta, vec_recoEle[1].pt, vec_recoEle[1].eta );
              h_mass_effPass_withSF_smeared_[i]->Fill( m_postFSR, genWeight*normFactor*eventSF_smeared );
            }
          }
        } // -- end of if( flag_passAcc )
      } // -- end of if( DYTool::IsDYEEEvent(ntuple, doTrancate) )
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
  // tool->Produce();
  tool->Produce_LocalTest();
}