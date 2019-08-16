#include <Include/SimplePlotTools.h>

void PrintMinMax()
{
  TString fileName = "ROOTFile_EfficiencySF_perMassBin.root";
  TString histName = "h_effSF_perMassBin";

  TH1D* h_effSF = PlotTool::Get_Hist(fileName, histName);
  Double_t min = h_effSF->GetMinimum();
  Double_t max = h_effSF->GetMaximum();

  printf("(min, max) = (%lf, %lf)\n", min, max);
}