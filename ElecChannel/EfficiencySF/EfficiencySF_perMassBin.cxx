#include <ElecChannel/EfficiencySF/TnPEfficiency.h>

class EffSFTool
{
public:
  TH1D* h_mass_accTotal_;
  TH1D* h_mass_accPass_;
  TH1D* h_mass_effPass_noSF_;
  TH1D* h_mass_effPass_withSF_;
  TH1D* h_effSF_perMassBin_;

  EffSFTool()
  {
    Init();
  }

  void Produce()
  {
    vector<DYTool::SampleInfo> vec_sampleInfo_signal = DYTool::GetSampleInfo_Signal();
    for(const auto& sampleInfo : vec_sampleInfo_signal )
      ProduceMassHist(sampleInfo);
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
    h_mass_accTotal_       = DYTool::MakeHist_DXSecBin("h_mass_accTotal");
    h_mass_accPass_        = DYTool::MakeHist_DXSecBin("h_mass_accPass");
    h_mass_effPass_noSF_   = DYTool::MakeHist_DXSecBin("h_mass_effPass_noSF");
    h_mass_effPass_withSF_ = DYTool::MakeHist_DXSecBin("h_mass_effPass_withSF");
    h_effSF_perMassBin_ = nullptr; // -- will be made at the end of ProduceMassHist
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
        if( flag_passAcc )
        {
          h_mass_accTotal_->Fill(m_postFSR, genWeight*normFactor);
          h_mass_accPass_->Fill(m_postFSR, genWeight*normFactor);
        }
        else
          h_mass_accTotal_->Fill(m_postFSR, genWeight*normFactor);

        // -- check the efficiency only for the event passing acceptance
        if( flag_passAcc )
        {
          vector<DYElectron> vec_recoEle;
          Bool_t flag_passEff = DYTool::IsDYEECandidate( ntuple, vec_recoEle );

          if( flag_passEff )
          {
            h_mass_effPass_noSF_->Fill( m_postFSR, genWeight*normFactor );

            Double_t eventSF = tnpEff->EfficiencySF_EventWeight( vec_recoEle[0].pt, vec_recoEle[0].eta, vec_recoEle[1].pt, vec_recoEle[1].eta );
            h_mass_effPass_withSF_->Fill( m_postFSR, genWeight*normFactor*eventSF );
          }
        } // -- end of if( flag_passAcc )
      } // -- end of if( DYTool::IsDYEEEvent(ntuple, doTrancate) )
    } // -- end of event iteration

    h_effSF_perMassBin_ = (TH1D*)h_mass_effPass_withSF_->Clone();
    h_effSF_perMassBin_->SetName("h_effSF_perMassBin");
    h_effSF_perMassBin_->Divide( h_mass_effPass_withSF_, h_mass_effPass_noSF_ );

    cout << "[EffSFTool::ProduceMassHist] Done" << endl;
  }

  void Save()
  {
    TFile *f_output = TFile::Open("ROOTFile_EfficiencySF_perMassBin.root", "RECREATE");
    f_output->cd();
    h_mass_accTotal_->Write();
    h_mass_accPass_->Write();
    h_mass_effPass_noSF_->Write();
    h_mass_effPass_withSF_->Write();
    h_effSF_perMassBin_->Write();
    f_output->Close();
    cout << "Output file: ROOTFile_EfficiencySF_perMassBin.root" << endl;
  }

};

void EfficiencySF_perMassBin()
{
  EffSFTool* tool = new EffSFTool();
  tool->Produce();
  // tool->Produce_LocalTest();
}