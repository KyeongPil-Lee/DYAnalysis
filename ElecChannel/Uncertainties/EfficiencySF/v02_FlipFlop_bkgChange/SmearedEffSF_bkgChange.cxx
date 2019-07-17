#include "SmearedEffSF_perMassBin.h"
void SmearedEffSF_bkgChange()
{
  SmearedEffSFTool* tool = new SmearedEffSFTool("bkgChange");
  tool->doFlipFlop_ = kTRUE;
  tool->Produce();
}
