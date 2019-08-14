#include <ElecChannel/CovMatrix/EfficiencySF/SmearedDXSecProducer.h>
#include <iostream>

void SmearedDXSec_EffSF()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_effSF = analyzerPath+"/ElecChannel/Uncertainties/EfficiencySF/v02_FlipFlop_bkgChange/ROOTFile_SmearedEffSF_perMassBin_bkgChange.root";

  SmearedDXSecProducer* producer = new SmearedDXSecProducer("bkgChange", fileName_effSF);
  producer->Produce();
}