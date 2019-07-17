#include <ElecChannel/Uncertainties/EfficiencySF/SmearedEffSF_perMassBin.h>
void SmearedEffSF_bkgChange()
{
  Bool_t doFlipFlop = kTRUE;
  SmearedEffSFTool* tool = new SmearedEffSFTool("bkgChange", doFlipFlop);
  tool->Produce();
}
