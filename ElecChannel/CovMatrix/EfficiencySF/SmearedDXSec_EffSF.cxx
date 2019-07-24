#include <ElecChannel/CovMatrix/EfficiencySF/SmearedDXSecProducer.h>
#include <iostream>

void SmearedDXSec_EffSF()
{
  // vector<TString> vec_uncType = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};
  // -- initialize to fit ROOT5
  vector<TString> vec_uncType;
  vec_uncType.push_back( "bkgChange" );
  vec_uncType.push_back( "sgnChange" );
  vec_uncType.push_back( "tagChange" );
  vec_uncType.push_back( "nlo" );
  vec_uncType.push_back( "stat" );

  Int_t nUnc = (Int_t)vec_uncType.size();
  for(Int_t i=0; i<nUnc; i++)
  {
    TString uncType = vec_uncType[i];
    cout << "uncType: " << uncType << endl;
  }
  
  for(Int_t i=0; i<nUnc; i++)
  {
    TString uncType = vec_uncType[i];
    SmearedDXSecProducer* producer = new SmearedDXSecProducer(uncType);
    producer->Produce();
  }
}