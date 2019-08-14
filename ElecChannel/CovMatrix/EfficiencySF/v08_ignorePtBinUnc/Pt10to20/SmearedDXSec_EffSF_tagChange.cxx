#include <ElecChannel/CovMatrix/EfficiencySF/SmearedDXSecProducer.h>
#include <iostream>

void SmearedDXSec_EffSF_tagChange()
{
  // vector<TString> vec_uncType = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};
  // -- initialize to fit ROOT5
  TString ptBinRange = "Pt10to20";
  TString uncType = "tagChange";

  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString inputFile = analyzerPath+TString::Format("/ElecChannel/Uncertainties/EfficiencySF/v03_ignorePtBinUnc/%s/ROOTFile_SmearedEffSF_perMassBin_%s.root", ptBinRange.Data(), uncType.Data());
  SmearedDXSecProducer* producer = new SmearedDXSecProducer(uncType, inputFile);
  producer->Produce();
}