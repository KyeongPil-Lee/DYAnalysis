#include <ElecChannel/Uncertainties/EfficiencySF/v04_ignoreEffType/SmearedEffSF_perMassBin.h>
void SmearedEffSF_bkgChange()
{
  TString effType_ignore = "reco";
  SmearedEffSFTool* tool = new SmearedEffSFTool("bkgChange", effType_ignore);
  tool->Produce();
}
