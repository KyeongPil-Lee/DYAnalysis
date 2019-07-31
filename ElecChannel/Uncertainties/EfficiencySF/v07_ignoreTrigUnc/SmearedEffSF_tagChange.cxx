#include <ElecChannel/Uncertainties/EfficiencySF/SmearedEffSF_perMassBin.h>

void SmearedEffSF_tagChange()
{
  TString fileName_reco = "./TnPResult/ROOTFile_RecoSF_April2019.root";
  TString fileName_ID   = "./TnPResult/ROOTFile_IDSF_April2019.root";
  TString fileName_trig = "./TnPResult/ROOTFile_trigSF_SMP17001.root";

  SmearedEffSFTool* tool = new SmearedEffSFTool("tagChange", fileName_reco, fileName_ID, fileName_trig);
  tool->Produce();
}
