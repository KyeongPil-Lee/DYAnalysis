#include <ElecChannel/Uncertainties/EfficiencySF/v04_ignoreEffType/SmearedEffSF_perMassBin.h>
void SmearedEffSF_tagChange()
{
  TString effType_ignore = "reco";
  SmearedEffSFTool* tool = new SmearedEffSFTool("tagChange", effType_ignore);
  tool->Produce();
}