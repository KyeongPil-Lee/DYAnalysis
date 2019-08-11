// -- while merging the file, the central value of the efficinecy SF are also merged as they have same name
// -- they should be replaced before merging

#include <Include/SimplePlotTools.h>
#include <ElecChannel/Include/DYElectronTools.h>

void Replace_CV()
{
  TString fileName_ref = "bkgChange/1st/ROOTFile_SmearedEffSF_perMassBin_bkgChange_0_to_499.root";
  TH1D* h_mass_effPass_noSF      = PlotTool::Get_Hist(fileName_ref, "h_mass_effPass_noSF");
  TH1D* h_mass_effPass_withSF_cv = PlotTool::Get_Hist(fileName_ref, "h_mass_effPass_withSF_cv");
  TH1D* h_effSF_perMassBin_cv    = PlotTool::Get_Hist(fileName_ref, "h_effSF_perMassBin_cv");

  vector<TString> vec_fileName_target = {
    "ROOTFile_SmearedEffSF_perMassBin_bkgChange.root",
    "ROOTFile_SmearedEffSF_perMassBin_sgnChange.root",
    "ROOTFile_SmearedEffSF_perMassBin_tagChange.root",
    "ROOTFile_SmearedEffSF_perMassBin_nlo.root",
    "ROOTFile_SmearedEffSF_perMassBin_stat.root",
  }
  for( const auto& fileName_target : vec_fileName_target )
  {
    DYTool::ReplaceObject(fileName_target, "h_mass_effPass_noSF",      h_mass_effPass_noSF);
    DYTool::ReplaceObject(fileName_target, "h_mass_effPass_withSF_cv", h_mass_effPass_withSF_cv);
    DYTool::ReplaceObject(fileName_target, "h_effSF_perMassBin_cv",    h_effSF_perMassBin_cv);
    cout << endl;
  }
}