#include <Include/PlotTools.h>

#define nIter 17 // -- # iterations for iterative D'Agstini's method

void ClosureTest()
{
	TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString fileName = rootFilePath + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root";
	TFile *f_input = TFile::Open(fileName);

	// -- histograms
	TH1D* h_measured = Get_Hist( fileName, "h_Measured_RooUnfold" );
	TH1D* h_truth = Get_Hist( fileName, "h_Truth_RooUnfold" );

	// -- unfold MC reco. histogram
	RooUnfoldResponse *rooResp = (RooUnfoldResponse*)f_input->Get("h_RecoMass_h_GenMass")->Clone();
	RooUnfoldBayes *rooBayes = new RooUnfoldBayes(rooResp, h_measured, nIter);
	TH1D *h_unfolded = (TH1D*)rooBayes->Hreco();

	// -- Draw canvas
	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	TString canvasName = "Local/c_ClosureTest";
	SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 1, 1 );

	c->cd();
	TopPad->cd();

	h_truth->Draw("EPSAME");
	h_measured->Draw("EPSAME");
	h_unfolded->Draw("EPSAME");

	SetHistFormat_TopPad( h_truth, "Entries per bin");

	TH1D* h_ratio_measured = (TH1D*)h_truth->Clone("h_ratio_measured");
	h_ratio_measured->Divide( h_measured, h_truth );

	TH1D* h_ratio_unfolded = (TH1D*)h_truth->Clone("h_ratio_unfolded");
	h_ratio_unfolded->Divide( h_unfolded, h_truth );

	c->cd();
	BottomPad->cd();

	h_ratio_measured->Draw("EPSAME");
	h_ratio_unfolded->Draw("EPSAME");

	SetHistFormat_BottomPad( h_ratio_measured, "m [GeV]", "Ratio to truth", 0.6, 1.4 );

	TF1 *f_line;
	DrawLine(f_line);

	Print_Histogram( h_ratio_unfolded );

	c->SaveAs(".pdf");
}