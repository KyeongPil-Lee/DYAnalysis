#include <Include/PlotTools.h>
void Comp_DXSec()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_before = ROOTFilePath+"/ROOTFile_Results_DYAnalysis_76X.root";

	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_after = AnalyzerPath+"/FullChain/Local/ROOTFile_Results_DYAnalysis_76X.root";

	vector<TString> vec_HistName =
	{
		"h_DiffXsec_FSRCorr",
		"h_DiffXsec_aMCNLO",
	};

	for(const auto& HistName : vec_HistName )
	{
		TH1D* h_before = Get_Hist( FileName_before, HistName );
		TH1D* h_after = Get_Hist( FileName_after, HistName );

		HistInfo* Hist_before = new HistInfo( kBlack, "Before", h_before );
		HistInfo* Hist_after = new HistInfo( kRed, "After", h_after );

		TString CanvasName = "Local/XCheck_"+HistName;
		DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_after, Hist_before );
		canvas->SetTitle( "m [GeV]", "d#sigma/dm [pb/GeV]", "after/before");
		canvas->SetLegendPosition( 0.60, 0.85, 0.97, 0.95 );
		canvas->SetRatioRange( 0.94, 1.06 );
		canvas->Draw( 1, 1 );
	}
}