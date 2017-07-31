#include "Calc_AccDiff_AfterFitAcc.h"

void Calc_AccDiff_AfterFitAcc()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TFile *f_output = TFile::Open("")
	// vector< TString > vec_Channel = {"Muon", "Electron"};
	vector< TString > vec_Channel = {"Electron"};

	for( const auto& Channel: vec_Channel )
	{		
		FittingTool *tool = new FittingTool( Channel );
		tool->Fit_All();
		tool->Calc_AccDiff();
		tool->Save();
	}
}