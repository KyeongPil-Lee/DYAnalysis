#include <ElecChannel/Uncertainties/EfficiencySF/SmearedEffSF_perMassBin.h>

void SmearedEffSF_GivenType_IndexRange(TString uncType, Int_t i_start, Int_t i_end )
{
  SmearedEffSFTool* tool = new SmearedEffSFTool(uncType);
  tool->Set_IndexRange(i_start, i_end);
  tool->Produce();
}
