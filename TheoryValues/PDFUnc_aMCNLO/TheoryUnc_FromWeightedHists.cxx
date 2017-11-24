#include "TheoryUnc_FromWeightedHists.h"

void TheoryUnc_FromWeightedHists()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TFile *f_output = TFile::Open("./Local/ROOTFile_TheoryUnc_aMCNLO.root", "RECREATE");

	Tool_TheoryUnc *tool_Muon_dressed = new Tool_TheoryUnc("Muon", "dressed");
	tool_Muon_dressed->Calc_TheoryUnc();
	tool_Muon_dressed->Save( f_output );

	Tool_TheoryUnc *tool_Muon_withinAcc = new Tool_TheoryUnc("Muon", "withinAcc");
	tool_Muon_withinAcc->Calc_TheoryUnc();
	tool_Muon_withinAcc->Save( f_output );

	Tool_TheoryUnc *tool_Electron_dressed = new Tool_TheoryUnc("Electron", "dressed");
	tool_Electron_dressed->Calc_TheoryUnc();
	tool_Electron_dressed->Save( f_output );

	Tool_TheoryUnc *tool_Electron_withinAcc = new Tool_TheoryUnc("Electron", "withinAcc");
	tool_Electron_withinAcc->Calc_TheoryUnc();
	tool_Electron_withinAcc->Save( f_output );
}