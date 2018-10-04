#include <TString.h>
#include <TChain.h>

void Print_GenWeight()
{
  vector<TString> vec_tag =
  {
    "M10to50",
    "M10to50_highStat",
    "M50toInf",
    "M100to200",
    "M200to400",
    "M400to500",
    "M500to700",
    "M700to800",
    "M800to1000",
    "M1000to1500",
    "M1500to2000",
    "M2000to3000"
  };

  vector <TString> vec_ntuplePath = 
  {
    "v20160304_76X_MINIAODv2_DYLL_M10to50_25ns/ntuple_skim_1.root",
    "DYntuple_v20170626_76X_MINIAODv2_DYLL_M10to50_25ns_LargeN/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M50toInf_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M100to200_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M200to400_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M400to500_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M500to700_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M700to800_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M800to1000_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M1000to1500_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M1500to2000_25ns/ntuple_skim_1.root",
    "v20160304_76X_MINIAODv2_DYLL_M2000to3000_25ns/ntuple_skim_1.root",
  };

  TString dataBasePath = "/data4/Users/kplee/DYntuple/76X";

  Int_t nSample = (Int_t)(vec_tag.size());
  for(Int_t i=0; i<nSample; i++)
  {
    Double_t genWeight = -1;

    TChain *chain = new TChain("recoTree/DYTree");
    chain->Add(dataBasePath+"/"+vec_ntuplePath[i]);

    chain->SetBranchStatus("*", 0);
    chain->SetBranchStatus("GENEvt_weight", 1);
    chain->SetBranchAddress("GENEvt_weight", &genWeight);

    chain->GetEntry(0);
    printf("[%s] absolute value of weight: %lf\n", vec_tag[i], fabs(genWeight));
  }
}