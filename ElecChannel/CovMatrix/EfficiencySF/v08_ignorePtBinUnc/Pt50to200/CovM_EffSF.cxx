#include <ElecChannel/CovMatrix/EfficiencySF/CovMProducer.h>

void CovM_EffSF()
{
  // vector<TString> vec_uncType = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};
  vector<TString> vec_uncType = {"bkgChange", "tagChange"};

  for(const auto& uncType : vec_uncType )
  {
    CovMProducer* producer = new CovMProducer(uncType);
    // producer->Validation();
    producer->Produce();
  }
}