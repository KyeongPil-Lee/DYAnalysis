#include <ElecChannel/CovMatrix/EfficiencySF/SmearedDXSecProducer.h>

void SmearedDXSec_EffSF()
{
  // vector<TString> vec_uncType = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};
  vector<TString> vec_uncType = {"bkgChange"};

  for(const auto& uncType : vec_uncType )
  {
    SmearedDXSecProducer* producer = new SmearedDXSecProducer(uncType);
    // producer->Produce();
  }
}