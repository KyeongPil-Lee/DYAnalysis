#include <Include/PlotTools.h>

void XCheck_Update()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName = "ROOTFile_DiffXSec_FullUnc.root";
	TString FileName_Before = ROOTFilePath+"/"+FileName;
	TString FileName_After = "Local/"+FileName;

	TString HistName = "h_DiffXsec_FSRCorr"; // -- data DY diff. cross-section -- //
	TString HistName_Before = HistName;
	TString HistName_After = HistName;

	TH1D* h_Before = Get_Hist( FileName_Before, HistName_Before );
	TH1D* h_After = Get_Hist( FileName_After, HistName_After );

	HistInfo* Hist_Before = new HistInfo( kBlack, "Before", h_Before );
	HistInfo* Hist_After = new HistInfo( kRed, "After", h_After );

	TString CanvasName = "./Local/XCheck_BeforeAfter";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_After, Hist_Before );
	canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "After/Before");
	canvas->SetRatioRange( 0.94, 1.06 );
	canvas->SetLegendPosition( 0.7, 0.8, 0.95, 0.95 );
	canvas->SetLatex( "Preliminary", 2.8, 13 );
	canvas->Draw(1,1);
}