#include <ElecChannel/Include/DYElectronTool.h>

class SumWeightTool
{
public:
  TString ntuplePath_;
  Bool_t DoTruncate_M100_;

  SumWeightTool(TString path)
  {
    ntuplePath_ = path;
    DoTruncate_M100_ = kFALSE;
  }

  void Calculate()
  {
    TChain *chain = new TChain("ntupler/ElectronTree");
    chain->Add(ntuplePath_);

    NtupleHandle_Ele *ntuple = new NtupleHandle_Ele(chain);

    Int_t nEvent = chain->GetEntries();
    cout << "nEvent: " << nEvent << endl;

    Double_t sumWeight = 0;
    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      ntuple->GetEvent(i_ev);

      vector<DYGenEle_preFSR> vec_ele = DYTool::GetAllGenEle_preFSR(ntuple);

      if( DoTruncate_M100_ )
      {
        if( vec_ele.size() == 2 )
        {
          Double_t mass = (vec_ele[0].vec_P + vec_ele[1].vec_P).M();
          if( mass < 100 )
            sumWeight += ntuple->genWeight;
        }
      }
      else
      {
        if( vec_ele.size() == 2 ) // -- DY->ee event
          sumWeight += ntuple->genWeight;
      }

    }

    cout << "[SumWeightTool::Calculate] ntuplePath = " << ntuplePath_ << endl;
    cout << "  -> sum of weight: " << sumWeight << endl;
  }

private:

};

void sumWeight_ele(TString ntuplePath)
{
  SumWeightTool* tool = new SumWeightTool(ntuplePath);
  tool->Calculate();
}

void sumWeight_ele()
{
  sumWeight_ele("/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/Include/Example/DY_2000to3000.root");
}