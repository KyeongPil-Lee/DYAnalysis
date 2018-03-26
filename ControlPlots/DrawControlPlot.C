#include "DrawControlPlotTool.h"

void DrawControlPlot(Bool_t DrawDataDriven = kTRUE, TString NormType = "Lumi" )
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );
	
	DrawControlPlotTool *tool = new DrawControlPlotTool(DrawDataDriven, NormType);
	// tool->GenLevelMassSpectrum();
	tool->SetupHistogramNames();
	tool->LoopForHistograms(-1);
}

