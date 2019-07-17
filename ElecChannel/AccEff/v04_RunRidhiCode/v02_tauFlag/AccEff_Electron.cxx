#include <ElecChannel/Include/DYElectronTool.h>

class AccEffTool
{
public:
  vector<TH1D*> vec_hist_mass_accTotal_;
  vector<TH1D*> vec_hist_mass_accPass_;
  vector<TH1D*> vec_hist_mass_effPass_;

  TH1D* h_mass_accTotal_all_;
  TH1D* h_mass_accPass_all_;
  TH1D* h_mass_effPass_all_;

  AccEffTool()
  {

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
  void ProduceMassHist(DYTool::SampleInfo sampleInfo)
  {
    cout << "[AccEffTool::ProduceMassHist] ntuple = " << sampleInfo.ntuplePath_ << endl;

    TChain *chain = new TChain("ntupler/ElectronTree");
    chain->Add(sampleInfo.ntuplePath_);

    NtupleHandle_Ele *ntuple = new NtupleHandle_Ele(chain);

    // -- setup for weights
    Double_t normFactor = sampleInfo.normFactor();
    DYTool::PUWeightTool* puWeightTool = new DYTool::PUWeightTool();

    // -- setup for the histograms
    TString massRangeInfo = TString::Format("%.0lfto%.0lf", sampleInfo.minM_, sampleInfo.maxM_);
    TH1D* h_mass_accTotal = DYTool::MakeHist_DXSecBin("h_mass_accTotal_"+massRangeInfo);
    TH1D* h_mass_accPass  = DYTool::MakeHist_DXSecBin("h_mass_accPass_"+massRangeInfo);
    TH1D* h_mass_effPass  = DYTool::MakeHist_DXSecBin("h_mass_effPass_"+massRangeInfo);

    // -- event loop
    Int_t nEvent = chain->GetEntries();
    cout << "nEvent: " << nEvent << endl;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      DYTool::loadBar(i_ev+1, nEvent, 100, 100);

      ntuple->GetEvent(i_ev);

      Double_t genWeight = ntuple->genWeight;
      Double_t PUWeight = puWeightTool->Weight(ntuple->nPU);

      Bool_t doTrancate = kFALSE;
      if( sampleInfo.maxM_ == 100 ) doTrancate = kTRUE;

      if( DYTool::IsDYEEEvent(ntuple, doTrancate) )
      {
        if( ntuple->tauFlag )
        {
          cout << "It is DYEE event, but tauFlag == 1 ... ignored" << endl;
          continue;
        }

        Bool_t flag_passAcc = kFALSE;

        vector<DYGenEle_postFSR> vec_genEle = DYTool::GetAllGenEle_postFSR(ntuple);

        Double_t m_postFSR = (vec_genEle[0].vec_P + vec_genEle[1].vec_P).M();

        flag_passAcc = DYTool::DoPassAcc_ee( vec_genEle[0].pt, vec_genEle[0].eta, vec_genEle[1].pt, vec_genEle[1].eta );
        if( flag_passAcc )
        {
          h_mass_accTotal->Fill(m_postFSR, genWeight*normFactor);
          h_mass_accPass->Fill(m_postFSR, genWeight*normFactor);
        }
        else
          h_mass_accTotal->Fill(m_postFSR, genWeight*normFactor);

        // -- check the efficiency only for the event passing acceptance
        if( flag_passAcc )
        {
          vector<DYElectron> vec_recoEle;
          Bool_t flag_passEff = DYTool::IsDYEECandidate( ntuple, vec_recoEle );

          if( flag_passEff )
            h_mass_effPass->Fill( m_postFSR, genWeight*normFactor*PUWeight );

        } // -- end of if( flag_passAcc )

      } // -- end of if( IsDYEEEvent(ntuple, doTrancate) )

    } // -- end of event iteration

    vec_hist_mass_accTotal_.push_back( h_mass_accTotal );
    vec_hist_mass_accPass_.push_back( h_mass_accPass );
    vec_hist_mass_effPass_.push_back( h_mass_effPass );
  }

  void Save()
  {
    TFile *f_output = TFile::Open("ROOTFile_AccEff_Electron.root", "RECREATE");

    Int_t nHist = (Int_t)vec_hist_mass_effPass_.size();
    for(Int_t i=0; i<nHist; i++)
    {
      vec_hist_mass_accTotal_[i]->Write();
      vec_hist_mass_accPass_[i]->Write();
      vec_hist_mass_effPass_[i]->Write();

      if( i == 0 )
      {
        h_mass_accTotal_all_ = (TH1D*)vec_hist_mass_accTotal_[i]->Clone();
        h_mass_accTotal_all_->SetName("h_mass_accTotal_all");

        h_mass_accPass_all_ = (TH1D*)vec_hist_mass_accPass_[i]->Clone();
        h_mass_accPass_all_->SetName("h_mass_accPass_all");

        h_mass_effPass_all_ = (TH1D*)vec_hist_mass_effPass_[i]->Clone();
        h_mass_effPass_all_->SetName("h_mass_effPass_all");
      }
      else
      {
        h_mass_accTotal_all_->Add(vec_hist_mass_accTotal_[i]);
        h_mass_accPass_all_->Add(vec_hist_mass_accPass_[i]);
        h_mass_effPass_all_->Add(vec_hist_mass_effPass_[i]);
      }
    }

    h_mass_accTotal_all_->Write();
    h_mass_accPass_all_->Write();
    h_mass_effPass_all_->Write();

    cout << "[AccEffTool::Save] All histograms are saved in ROOTFile_AccEff_Electron.root" << endl;

    f_output->Close();
  }
};


void AccEff_Electron()
{
  AccEffTool* tool = new AccEffTool();
  tool->Produce();
  // tool->Produce_LocalTest();
}