#include "SmearedEffSF_perMassBin.h"
void SmearedEffSF_nlo()
{
  SmearedEffSFTool* tool = new SmearedEffSFTool("nlo");
  tool->Produce();
}
