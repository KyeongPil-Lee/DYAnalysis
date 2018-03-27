#include <Include/PlotTools.h>

void ClosureTest()
{
	TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString fileName = rootFilePath + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root";
	TFile *f_input = TFile::Open(fileName);

	// -- histograms
	TH1D* h_measured = Get_Hist( fileName, "h_Measured_RooUnfold" );
	TH1D* h_truth = Get_Hist( fileName, "h_Truth_RooUnfold" );

	// -- unfold MC reco. histogram
	RooUnfoldResponse *rooResp = (RooUnfoldResponse*)f_input->Get("h_RecoMass_h_GenMass")->Clone();
	RooUnfoldInvert *rooInvert = new RooUnfoldInvert(rooResp, h_measured);
	TH1D *h_unfolded = (TH1D*)rooInvert->Hreco();

	// -- Draw canvas
	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	TString canvasName = "Local/c_ClosureTest";
	SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 1, 1 );

	c->cd();
	TopPad->cd();

	h_truth->Draw("EPSAME");
	h_measured->Draw("EPSAME");
	h_unfolded->Draw("EPSAME");

	h_truth->SetStats(kFALSE);
	h_truth->SetMarkerStyle(20);
	h_truth->SetMarkerColor(kRed);
	h_truth->SetLineColor(kRed);

	h_measured->SetStats(kFALSE);
	h_measured->SetMarkerStyle(20);
	h_measured->SetMarkerColor(kBlue);
	h_measured->SetLineColor(kBlue);

	h_unfolded->SetStats(kFALSE);
	h_unfolded->SetMarkerStyle(20);
	h_unfolded->SetMarkerColor(kGreen+2);
	h_unfolded->SetLineColor(kGreen+2);

	SetHistFormat_TopPad( h_truth, "Entries per bin");

	TLegend *legend;
	SetLegend( legend, 0.70, 0.72, 0.95, 0.95 );
	legend->AddEntry( h_truth, "Truth" );
	legend->AddEntry( h_measured, "Measured (MC)" );
	legend->AddEntry( h_unfolded, "Unfolded (MC)" );
	legend->Draw();

	TLatex latex;
	Latex_Simulation( latex );
	latex.DrawLatexNDC(0.16, 0.91, "#scale[0.7]{#font[42]{Closure test (unfolding correction for FSR)}}");

	c->cd();
	BottomPad->cd();

	TH1D* h_ratio_measured = (TH1D*)h_measured->Clone("h_ratio_measured");
	h_ratio_measured->Divide( h_measured, h_truth );

	TH1D* h_ratio_unfolded = (TH1D*)h_unfolded->Clone("h_ratio_unfolded");
	h_ratio_unfolded->Divide( h_unfolded, h_truth );

	h_ratio_measured->Draw("EPSAME");
	h_ratio_unfolded->Draw("EPSAME");

	SetHistFormat_BottomPad( h_ratio_measured, "m [GeV]", "Ratio to truth", 0.6, 1.4 );

	TF1 *f_line;
	DrawLine(f_line);

	Print_Histogram( h_ratio_unfolded );

	Bool_t isFound = kFALSE;
	Int_t nBin = h_ratio_unfolded->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t value = h_ratio_unfolded->GetBinContent(i_bin);
		if( (value - 1.0) > 1e-3 )
		{
			isFound = kTRUE;
			Double_t lowerEdge = h_ratio_unfolded->GetBinLowEdge(i_bin);
			Double_t upperEdge = h_ratio_unfolded->GetBinLowEdge(i_bin+1);

			printf("[%.0lf to %.0lf] unfolded/truth = %lf: not equal to 1 ... please check\n",
				lowerEdge, upperEdge, value );
		}
	}

	if( !isFound ) cout << "Pass the closure test" << endl;

	c->SaveAs(".pdf");
}