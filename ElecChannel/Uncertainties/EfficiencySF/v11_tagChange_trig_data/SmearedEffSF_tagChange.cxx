#include "SmearedEffSF_perMassBin.h"

void SmearedEffSF_tagChange()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString baseDir = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult";

  TString fileName_reco = baseDir+"/ROOTFile_RecoSF_April2019.root";
  TString fileName_ID   = baseDir+"/ROOTFile_IDSF_April2019.root";
  TString fileName_trig = "./TnPResult/ROOTFile_trigSF_SMP17001.root";

  SmearedEffSFTool* tool = new SmearedEffSFTool("tagChange", fileName_reco, fileName_ID, fileName_trig);
  tool->Produce();
}
