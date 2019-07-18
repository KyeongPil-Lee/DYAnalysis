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
    ProduceMassHist();
    Save();
  }

private:
  void Init()
  {
    h_mass_accTotal_       = DYTool::MakeHist_DXSecBin("h_mass_accTotal");
    h_mass_accPass_        = DYTool::MakeHist_DXSecBin("h_mass_accPass");
    h_mass_effPass_noSF_   = DYTool::MakeHist_DXSecBin("h_mass_effPass_noSF");
    h_mass_effPass_withSF_ = DYTool::MakeHist_DXSecBin("h_mass_effPass_withSF");

    h_mass_accTotal_->Sumw2();
    h_mass_accPass_->Sumw2();
    h_mass_effPass_noSF_->Sumw2();
    h_mass_effPass_withSF_->Sumw2();

    h_effSF_perMassBin_ = nullptr; // -- will be made at the end of ProduceMassHist
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

    // -- TnP efficiency information
    TnPEfficiency* tnpEff = new TnPEfficiency();

    for(Long64_t i_ev = 0; i_ev < nTotEvent; i_ev++)
    {
      DYTool::loadBar(i_ev+1, nTotEvent, 100, 100);

      ntuple->GetEntry(i_ev);

      h_mass_effPass_noSF_->Fill( diM, lumiWeight*genWeight );

      Double_t eventSF = tnpEff->EfficiencySF_EventWeight( pt1, eta1, pt2, eta2 );
      h_mass_effPass_withSF_->Fill( diM, lumiWeight*genWeight*eventSF );
    }

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