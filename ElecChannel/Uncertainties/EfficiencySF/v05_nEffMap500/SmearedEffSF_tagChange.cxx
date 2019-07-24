#include "SmearedEffSF_perMassBin.h"
void SmearedEffSF_tagChange()
{
  SmearedEffSFTool* tool = new SmearedEffSFTool("tagChange");
  tool->Produce();
}
