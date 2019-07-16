#include <ElecChannel/Include/DYElectronTool.h>

class TreeReader
{
public:
  TChain* chain_;

  TString fileName_;
  TString treeName_;

  TreeReader(TString fileName, TString treeName)
  {
    fileName_ = fileName;
    treeName_ = treeName;
    Init();
  }

  void Print()
  {
    NtupleHandle_Ele* ntuple = new NtupleHandle_Ele(chain_);
    Int_t nEvent = chain_->GetEntries();
    cout << "nEvent: " << nEvent << endl;

    nEvent = 1000;

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      ntuple->GetEvent(i_ev);

      printf("=========================\n");
      printf("[%d event] (run, lumi, event) = (%f, %f, %f)\n", i_ev, ntuple->runNum, ntuple->lumiBlockNum, ntuple->eventNum);

      vector<DYGenEle_preFSR> vec_genEle_preFSR = DYTool::GetAllGenEle_preFSR( ntuple );
      Int_t nEle_preFSR = (Int_t)vec_genEle_preFSR.size();
      cout << "+++ gen electron (pre-FSR) +++ -> # elec = " << nEle_preFSR << endl;
      for(const auto& ele : vec_genEle_preFSR )
        printf("  (pt, eta, phi) = (%lf, %lf, %lf)\n", ele.pt, ele.eta, ele.phi);
      cout << endl;


      vector<DYGenEle_postFSR> vec_genEle_postFSR = DYTool::GetAllGenEle_postFSR( ntuple );
      Int_t nEle_postFSR = (Int_t)vec_genEle_postFSR.size();
      cout << "+++ gen electron (post-FSR) +++ -> # elec = " << nEle_postFSR << endl;
      for(const auto& ele : vec_genEle_postFSR )
        printf("  (pt, eta, phi) = (%lf, %lf, %lf)\n", ele.pt, ele.eta, ele.phi);
      cout << endl;

      vector<DYElectron> vec_ele = DYTool::GetAllElectron( ntuple );
      Int_t nEle = (Int_t)vec_ele.size();
      cout << "+++ reco electron +++ -> # elec = " << nEle << endl;
      for(const auto& ele : vec_ele )
        printf("  (pt, eta, phi) = (%lf, %lf, %lf)\n", ele.pt, ele.eta, ele.phi);
      cout << endl;


      printf("=========================\n\n");
    }
  }

private:
  void Init()
  {
    chain_ = new TChain(treeName_);
    chain_->Add(fileName_);
  }

};

void PrintEleInfo()
{
  TString fileName = "DY_2000to3000.root";
  TreeReader* reader = new TreeReader(fileName, "ntupler/ElectronTree");
  reader->Print();
}
