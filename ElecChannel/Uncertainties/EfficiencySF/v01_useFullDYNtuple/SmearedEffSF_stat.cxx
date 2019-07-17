#include "SmearedEffSF_perMassBin.h"
void SmearedEffSF_stat()
{
  SmearedEffSFTool* tool = new SmearedEffSFTool("stat");
  tool->Produce();
}
