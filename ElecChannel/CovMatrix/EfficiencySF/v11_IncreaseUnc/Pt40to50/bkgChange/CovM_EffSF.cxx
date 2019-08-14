#include <ElecChannel/CovMatrix/EfficiencySF/CovMProducer.h>

void CovM_EffSF()
{
  TString uncType = "bkgChange";
  TString fileName_dXSec = "./SmearedDXSec/ROOTFile_SmearedDXSecProducer_"+uncType+".root";

  CovMProducer* producer = new CovMProducer(uncType, fileName_dXSec);
  producer->Produce();
}