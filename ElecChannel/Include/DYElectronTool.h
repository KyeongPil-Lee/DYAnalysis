#include <ElecChannel/Include/DYElectron.h>
#include <TH1D.h>
#include <TString.h>
#include <iostream>

namespace DYTool
{
  Double_t LUMI_ELE = 2258.066; // -- /pb

  vector<DYGenEle_preFSR> GetAllGenEle_preFSR(NtupleHandle_Ele* ntuple, Double_t minPt = -1)
  {
    vector<DYGenEle_preFSR> vec_ele;

    Int_t nEle = (Int_t)ntuple->genEle_preFSR_pt->size();
    for(Int_t i_ele=0; i_ele<nEle; i_ele++)
    {
      DYGenEle_preFSR ele(ntuple, i_ele);
      if( ele.pt > minPt )
        vec_ele.push_back( ele );
    }

    return vec_ele;
  }

  vector<DYGenEle_postFSR> GetAllGenEle_postFSR(NtupleHandle_Ele* ntuple, Double_t minPt = -1)
  {
    vector<DYGenEle_postFSR> vec_ele;

    Int_t nEle = (Int_t)ntuple->genEle_postFSR_pt->size();
    for(Int_t i_ele=0; i_ele<nEle; i_ele++)
    {
      DYGenEle_postFSR ele(ntuple, i_ele);
      if( ele.pt > minPt )
        vec_ele.push_back( ele );
    }

    return vec_ele;
  }

  vector<DYElectron> GetAllElectron(NtupleHandle_Ele* ntuple, Double_t minPt = -1 )
  {
    vector<DYElectron> vec_ele;
    Int_t nEle = ntuple->nEle;
    for(Int_t i_ele=0; i_ele<nEle; i_ele++)
    {
      DYElectron ele(ntuple, i_ele);
      if( ele.pt > minPt )
        vec_ele.push_back( ele );
    }
    return vec_ele;
  }

  TH1D* MakeHist_DXSecBin(TString histName)
  {
    Double_t arr_massBinEdge[44] = {15,20,25,30,35,40,45,50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,126,133,141,150,160,171,185,200,220,243,273,320,380,440,510,600,700,830,1000,1500,3000};
    return new TH1D(histName, "", 43, arr_massBinEdge);
  }

  static inline void loadBar(int x, int n, int r, int w)
  {
      // Only update r times.
      if( x == n )
        cout << endl;

      if ( x % (n/r +1) != 0 ) return;

   
      // Calculuate the ratio of complete-to-incomplete.
      float ratio = x/(float)n;
      int   c     = ratio * w;
   
      // Show the percentage complete.
      printf("%3d%% [", (int)(ratio*100) );
   
      // Show the load bar.
      for (int x=0; x<c; x++) cout << "=";
   
      for (int x=c; x<w; x++) cout << " ";
   
      // ANSI Control codes to go back to the
      // previous line and clear it.
    cout << "]\r" << flush;
  }
};
// -- end of namespace