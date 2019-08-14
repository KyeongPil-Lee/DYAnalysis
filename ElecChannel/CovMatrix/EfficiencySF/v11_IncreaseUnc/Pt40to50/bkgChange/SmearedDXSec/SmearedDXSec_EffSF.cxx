#include <ElecChannel/CovMatrix/EfficiencySF/SmearedDXSecProducer.h>
#include <iostream>

void SmearedDXSec_EffSF()
{
  // vector<TString> vec_uncType = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};
  // -- initialize to fit ROOT5
  TString uncType = "bkgChange";
  TString fileName_effSF = "../SmearedEffPerMassBin/ROOTFile_SmearedEffSF_perMassBin_"+uncType+".root";
  SmearedDXSecProducer* producer = new SmearedDXSecProducer(uncType, fileName_effSF);
  producer->Produce();
}