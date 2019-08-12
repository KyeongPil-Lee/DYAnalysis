#include <ElecChannel/CovMatrix/EfficiencySF/CovROOTFileMaker.h>

void MakeSummaryROOTFile()
{
  CovROOTFileMaker* maker = new CovROOTFileMaker();
  // maker->SetInputFile("bkgChange", "./ROOTFile_CovMProducer_bkgChange.root");
  maker->Make();
}
