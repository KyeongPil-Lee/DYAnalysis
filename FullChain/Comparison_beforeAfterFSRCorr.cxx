// #include <Include/SimplePlotTools.h>
#include "FullChain.h"
#include <iostream>
void Comparison_beforeAfterFSRCorr()
{
  DXSecTool *tool = new DXSecTool();
  tool->Run_FullChain();

  TH1D* h_before = (TH1D*)tool->h_EffSFCorr_4p2->Clone();
  h_before->Add( (TH1D*)tool->h_EffSFCorr_4p3->Clone() );

  TH1D* h_after = (TH1D*)tool->h_FSRCorr->Clone();

  Double_t minRatio = 999;
  Double_t maxRatio = -999;
  Int_t nBin = h_before->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    Double_t before = h_before->GetBinContent(i_bin);
    Double_t after = h_after->GetBinContent(i_bin);
    Double_t ratio = after/before;

    Double_t binEdgeDown = h_before->GetBinLowEdge(i_bin);
    Double_t binEdgeUp = h_before->GetBinLowEdge(i_bin+1);

    cout << "[" << i_bin << "th bin]" 
         << "(" << binEdgeDown << ", " << binEdgeUp << ") "
         << "(before, after, after/before) = ("
         << before << ", "
         << after  << ", "
         << ratio << ")" << endl;

    if( ratio < minRatio ) minRatio = ratio;
    if( ratio > maxRatio ) maxRatio = ratio;
  }
  cout << endl;
  cout << "(min, max) = (" << minRatio << ", " << maxRatio << ")" << endl;
  cout << endl;
}