#include <ElecChannel/Uncertainties/EfficiencySF/v03_ignorePtBinUnc/SmearedEffSF_perMassBin.h>
void SmearedEffSF_bkgChange()
{
  // -- pt bin index
  // -- [10, 20]: 0
  // -- [20, 30]: 1
  // -- [30, 40]: 2
  // -- [40, 50]: 3
  // -- [50, 200]: 4

  Int_t ptBinIndex_ignore = 0;
  SmearedEffSFTool* tool = new SmearedEffSFTool("bkgChange", ptBinIndex_ignore);
  tool->Produce();
}
