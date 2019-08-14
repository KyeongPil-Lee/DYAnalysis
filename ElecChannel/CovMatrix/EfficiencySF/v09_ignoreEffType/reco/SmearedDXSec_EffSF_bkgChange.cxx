#include <ElecChannel/CovMatrix/EfficiencySF/SmearedDXSecProducer.h>
#include <iostream>

void SmearedDXSec_EffSF_bkgChange()
{
  // vector<TString> vec_uncType = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};
  // -- initialize to fit ROOT5
  TString effType = "reco";
  TString uncType = "bkgChange";

  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString inputFile = analyzerPath+TString::Format("/ElecChannel/Uncertainties/EfficiencySF/v04_ignoreEffType/%s/ROOTFile_SmearedEffSF_perMassBin_%s.root", effType.Data(), uncType.Data());
  SmearedDXSecProducer* producer = new SmearedDXSecProducer(uncType, inputFile);
  producer->Produce();
}