// #include <ElecChannel/Uncertainties/EfficiencySF/SmearedEffSF_perMassBin.h>
#include "SmearedEffSF_perMassBin_500.h"

void SmearedEffSF_tagChange()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

  TString fileName_reco = "./TnPResult/ROOTFile_OldEff_NewUnc_reco.root";
  TString fileName_ID   = "./TnPResult/ROOTFile_OldEff_NewUnc_ID.root";
  TString fileName_trig = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult/ROOTFile_trigSF_SMP17001.root";

  SmearedEffSFTool* tool = new SmearedEffSFTool("tagChange", fileName_reco, fileName_ID, fileName_trig);
  tool->Produce();
}
