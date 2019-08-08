#include "SmearedEffSF_perMassBin.h"

void SmearedEffSF_sgnChange()
{
  SmearedEffSFTool* tool = new SmearedEffSFTool("sgnChange");
  tool->Produce();
}
