#include "Calc_AccDiff_AfterFitAcc.h"

void Calc_AccDiff_AfterFitAcc()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TFile *f_output = TFile::Open("ROOTFile_Unc_AccDiff_aMCNLO_FEWZ_ByFit.root", "RECREATE");

	vector< TString > vec_Channel = {"Muon", "Electron"};
	// vector< TString > vec_Channel = {"Electron"};

	for( const auto& Channel: vec_Channel )
	{		
		FittingTool *tool = new FittingTool( Channel );
		tool->Fit_All();
		tool->Calc_AccDiff();
		tool->Save( f_output );
	}

	f_output->Close();
}