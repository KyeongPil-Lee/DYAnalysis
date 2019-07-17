#include "SmearedEffSF_perMassBin.h"
void SmearedEffSF_bkgChange()
{
  SmearedEffSFTool* tool = new SmearedEffSFTool("nlo");
  tool->Produce();
}
