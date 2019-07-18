#include "CovMProducer.h"

void CovM_EffSF()
{
  vec_uncType = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};

  for(const auto& uncType : vec_uncType )
  {
    CovMProducer* producer = new CovMProducer(uncType);
    producer->Produce();
  }
}