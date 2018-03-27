#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>
#include <stdio.h>
#include <TGraphAsymmErrors.h>
#include <vector>
#include <TROOT.h>
#include <TLatex.h>

#include <src/RooUnfold.h>
#include <src/RooUnfoldResponse.h>
#include <src/RooUnfoldInvert.h>

#include <Include/MyCanvas.C>

void CalculateFractionPerBin(TH2D *h_nEvents, TH1* h_Truth, TH2D *h_Response);
TH2D* Transpose( TH2D* h_2D );
void MakeCanvas_RespM( TH2D *h_RespM );

void DrawClosureTest_FSR()
{
	gROOT->SetBatch( kTRUE );
	// gSystem->Load("/home/kplee/Unfolding/libRooUnfold.so");

	TString FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");
	TFile *f_input = new TFile(FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root");

	///////////////////////////
	// -- Response Matrix -- //
	///////////////////////////
	TH1D *h_Truth_RooUnfold = (TH1D*)f_input->Get("h_Truth_RooUnfold");
	TH2D* h_nEvents = (TH2D*)f_input->Get("h_RespM_RooUnfold");
	h_nEvents->SetName("h_nEvents");
	TH2D* h_RespM = (TH2D*)h_nEvents->Clone("h_RespM");
	CalculateFractionPerBin(h_nEvents, h_Truth_RooUnfold, h_RespM);
	MakeCanvas_RespM( h_RespM );

	////////////////////////
	// -- Closure Test -- //
	////////////////////////
	RooUnfoldResponse *UnfoldRes = (RooUnfoldResponse*)f_input->Get("UnfoldRes");
	TH1D *h_Measured_RooUnfold = (TH1D*)f_input->Get("h_Measured_RooUnfold");

	// RooUnfoldBayes *UnfoldBayes = new RooUnfoldBayes(UnfoldRes, h_Measured_RooUnfold);
	// TH1D *h_UnfoldedMC = (TH1D*)UnfoldBayes->Hreco();

	RooUnfoldInvert *UnfoldInvert = new RooUnfoldInvert(UnfoldRes, h_Measured_RooUnfold);
	TH1D *h_UnfoldedMC = (TH1D*)UnfoldInvert->Hreco();

	MyCanvas *myc = new MyCanvas("c_ClosureTest", "Gen-Level Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx();
	myc->SetLogy(0);
	myc->SetRatioRange(0.7, 1.3);
	myc->CanvasWithThreeHistogramsRatioPlot( h_Measured_RooUnfold, h_UnfoldedMC, h_Truth_RooUnfold, 
											 "post-FSR", "Unfolded", "pre-FSR", "Ratio to pre-FSR",
											 kBlue, kGreen+1, kRed);
	myc->PrintCanvas();

	// -- Check the detail number -- //
	TH1D *h_ratio = (TH1D*)(myc->h_ratio2)->Clone();
	Int_t nBin = h_ratio->GetNbinsX();
	Bool_t isPassTest = kTRUE;
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t ratio = h_ratio->GetBinContent(i_bin);
		if( (ratio-1) > 1e-5 )
		{
			Double_t lowerEdge = h_ratio->GetBinLowEdge(i_bin);
			Double_t upperEdge = h_ratio->GetBinLowEdge(i_bin+1);
			printf("[%2dth bin] (lowerEdge, upperEdge) = (%.3lf, %.3lf) (Unfolded/Truth) = %.6lf\n", i_bin, lowerEdge, upperEdge, ratio);
			isPassTest = kFALSE;
		}
	}
	cout << "==============================================" << endl;
	if( isPassTest ) 
		cout << "[Pass the closure test]" << endl;
	else
		cout << "[Closure test is FAILED! ... Check the details!]" << endl;
	cout << "==============================================" << endl;


}

void CalculateFractionPerBin(TH2D *h_nEvents, TH1* h_Truth, TH2D *h_Response)
{
	Int_t nBins = h_Truth->GetNbinsX();

	for(Int_t i_genbin=0; i_genbin <= nBins+1; i_genbin++) // -- Include under/overflow -- //
	{
		Double_t SumN_truth = h_Truth->GetBinContent(i_genbin);

		for(Int_t i_recobin=0; i_recobin <= nBins+1; i_recobin++) // -- Include under/overflow -- //
		{
			Double_t nEvent = h_nEvents->GetBinContent(i_recobin, i_genbin);

			Double_t fraction = 0;
			if( SumN_truth == 0 )
				fraction = 0;
			else
				fraction = nEvent / SumN_truth;
			
			if( fraction < 0 && fabs(fraction) < 1e-3 ) fraction = 0;

			h_Response->SetBinContent( i_recobin, i_genbin, fraction );
		}
	}
}

void MakeCanvas_RespM( TH2D *h_RespM )
{
	TH2D *hT_RespM = Transpose( h_RespM );

	TCanvas *c = new TCanvas("c_RespM_FSR", "", 800, 800);
	gPad->SetGridx();
	gPad->SetGridy();
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetLogz();

	gPad->SetLeftMargin(0.14);
	gPad->SetRightMargin(0.12);
	gPad->SetTopMargin(0.05);
	gPad->SetBottomMargin(0.1);

	hT_RespM->Draw("COLZ");

	hT_RespM->SetTitle("");
	hT_RespM->SetStats(kFALSE);

	hT_RespM->GetXaxis()->SetTitle("generated mass (dressed lepton) [GeV]");
	hT_RespM->GetXaxis()->SetNoExponent();
	hT_RespM->GetXaxis()->SetMoreLogLabels();
	hT_RespM->GetYaxis()->SetTitle("generated mass (post-FSR) [GeV]");
	hT_RespM->GetYaxis()->SetNoExponent();
	hT_RespM->GetYaxis()->SetMoreLogLabels();
	hT_RespM->GetYaxis()->SetTitleOffset(2);

	TLatex latex;
	latex.SetTextSize(0.03);
	latex.DrawLatexNDC(0.79, 0.96, "13 TeV");
	latex.SetTextSize(0.035);
	latex.DrawLatexNDC(0.17, 0.895, "CMS Simulation");

	c->SaveAs(".pdf");
}

TH2D* Transpose( TH2D* h_2D )
{
	TH2D* h_2D_Transposed = (TH2D*)h_2D->Clone();
	h_2D_Transposed->SetDirectory(0);

	Int_t nBin_X = h_2D->GetNbinsX();
	Int_t nBin_Y = h_2D->GetNbinsY();

	if(nBin_X != nBin_Y )
	{
		printf("(nBin_X, nBin_Y) = (%d, %d): this is not squared matrix! ... transpose for this matrix is not provided by this function\n", nBin_X, nBin_Y);
		return NULL;
	}

	for(Int_t iter_x=0; iter_x<nBin_X; iter_x++)
	{
		Int_t i_binX = iter_x+1;
		for(Int_t iter_y=0; iter_y<nBin_Y; iter_y++)
		{
			Int_t i_binY = iter_y+1;

			Double_t value_before = h_2D->GetBinContent(i_binX, i_binY);

			h_2D_Transposed->SetBinContent(i_binY, i_binX, value_before);
		}
	}

	return h_2D_Transposed;
}



