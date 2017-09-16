#include <fstream>
#include <iostream>
#include <stdio.h>

// #include </Users/KyeongPil_Lee/Codes/DYAnalysisCodes/tdrstyle.C>
#include </Users/KyeongPil_Lee/Codes/DYAnalysisCodes/MyCanvas.C>

void Comparison_DiffXsecWithinAcc_FEWZ_vs_aMCNLO()
{
	// setTDRStyle();
	// tdrStyle->SetErrorX(0.5);

	const Int_t nMassBin = 45;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1200, 1500, 2000, 3000};
	

	Double_t xSec_FullAcc_FEWZ[nMassBin];
	Double_t xSec_WithinAcc_FEWZ[nMassBin];
	Double_t xSec_WithinAcc_FEWZ_NNLO[nMassBin];

	FILE *fl;
	Double_t LowerEdge;
	Double_t UpperEdge;
	Double_t xSec_FullAcc_temp;
	Double_t xSec_WithinAcc_temp;
	Double_t xSec_WithinAcc_NNLO_temp;

	// -- file: https://twiki.cern.ch/twiki/pub/CMS/EWKDrellYan2015/dy_mm_13tev_noPI_NLO_WNLO_4pi_accdy_twiki.txt -- //
	if ((fl = fopen("DiffXsec_FEWZ_NLO_NNLO.txt", "r")) != NULL)
	{
		Int_t i=0;

		while( fscanf(fl, "%lf\t%lf\t%lf\t%lf\t%lf\n", &LowerEdge, &UpperEdge, &xSec_FullAcc_temp, &xSec_WithinAcc_temp, &xSec_WithinAcc_NNLO_temp) != EOF )
		{
			xSec_FullAcc_FEWZ[i] = xSec_FullAcc_temp;
			xSec_WithinAcc_FEWZ[i] = xSec_WithinAcc_temp;
			xSec_WithinAcc_FEWZ_NNLO[i] = xSec_WithinAcc_NNLO_temp;

			printf("%lf\t%lf\t%lf\t%lf\t%lf\n", LowerEdge, UpperEdge, xSec_FullAcc_FEWZ[i], xSec_WithinAcc_FEWZ[i], xSec_WithinAcc_FEWZ_NNLO[i]);
			i++;

			if( i == 45 )
				break;
		}
	}
	fclose(fl);

	TH1D *h_FullAcc_FEWZ = new TH1D("h_FullAcc_FEWZ", "", nMassBin, MassBinEdges);
	TH1D *h_WithinAcc_FEWZ = new TH1D("h_WithinAcc_FEWZ", "", nMassBin, MassBinEdges);
	TH1D *h_WithinAcc_FEWZ_NNLO = new TH1D("h_WithinAcc_FEWZ_NNLO", "", nMassBin, MassBinEdges);

	MakeDiffXsecHisto( h_FullAcc_FEWZ, xSec_FullAcc_FEWZ );
	MakeDiffXsecHisto( h_WithinAcc_FEWZ, xSec_WithinAcc_FEWZ );
	MakeDiffXsecHisto( h_WithinAcc_FEWZ_NNLO, xSec_WithinAcc_FEWZ_NNLO );

	// h_WithinAcc_FEWZ->Draw();

	TFile *f_input = new TFile("ROOTFile_DiffXsec_WithinAcc_aMCNLO.root");
	TH1D *h_mass_WithinAcc_aMCNLO = (TH1D*)f_input->Get("h_mass_preFSR_WithinAcc_tot")->Clone();

	TH1D* h_WithinAcc_aMCNLO = h_mass_WithinAcc_aMCNLO->Clone();
	h_WithinAcc_aMCNLO->Scale( 1 / 2832.673 );
	Int_t nBin = h_WithinAcc_aMCNLO->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t xSec = h_WithinAcc_aMCNLO->GetBinContent(i_bin);
		Double_t xSec_error = h_WithinAcc_aMCNLO->GetBinError(i_bin);
		Double_t BinWidth = h_WithinAcc_aMCNLO->GetBinWidth(i_bin);

		Double_t DiffxSec = xSec / BinWidth;
		Double_t DiffxSec_error = xSec_error / BinWidth;

		h_WithinAcc_aMCNLO->SetBinContent(i_bin, DiffxSec);
		h_WithinAcc_aMCNLO->SetBinError(i_bin, DiffxSec_error);
	}

	MyCanvas *myc = new MyCanvas("c_DiffXsec_WithinAcc_FEWZ_vs_aMCNLO", "Dimuon Mass (preFSR) [GeV]", "d#sigma/dM [pb/GeV]");
	myc->SetLogx();
	myc->SetLogy(kFALSE);
	myc->SetRatioRange(0, 2.5);
	myc->CanvasWithHistogramsRatioPlot( (TH1D*)h_WithinAcc_aMCNLO->Clone(), (TH1D*)h_WithinAcc_FEWZ->Clone(), "aMC@NLO", "FEWZ", "aMC@NLO/FEWZ");
	myc->PrintCanvas();

	MyCanvas *myc2 = new MyCanvas("c_DiffXsec_WithinAcc_FEWZNNLO_vs_aMCNLO", "Dimuon Mass (preFSR) [GeV]", "d#sigma/dM [pb/GeV]");
	myc2->SetLogx();
	myc2->SetLogy(kFALSE);
	myc2->CanvasWithHistogramsRatioPlot( h_WithinAcc_aMCNLO, (TH1D*)h_WithinAcc_FEWZ_NNLO->Clone(), "aMC@NLO", "FEWZ (NNLO)", "aMC@NLO/FEWZ");
	myc2->PrintCanvas();

	MyCanvas *myc3 = new MyCanvas("c_DiffXsec_WithinAcc_FEWZ_NLO_vs_NNLO", "Dimuon Mass (preFSR) [GeV]", "d#sigma/dM [pb/GeV]");
	myc3->SetLogx();
	myc3->SetLogy(kFALSE);
	myc3->SetRatioRange(0, 2.5);
	myc3->CanvasWithHistogramsRatioPlot( h_WithinAcc_FEWZ_NNLO, h_WithinAcc_FEWZ, "FEWZ (NNLO)", "FEWZ (NLO)", "NNLO/NLO");
	myc3->PrintCanvas();

	TFile *f_output = new TFile("ROOTFile_DiffXsec_WithinAcc_All.root", "RECREATE"); f_output->cd();
	h_WithinAcc_aMCNLO->SetName("h_DiffXsec_WithinAcc_aMCNLO");
	h_WithinAcc_aMCNLO->Write();
	h_WithinAcc_FEWZ->SetName("h_DiffXsec_WithinAcc_FEWZ_NLO");
	h_WithinAcc_FEWZ->Write();
	h_WithinAcc_FEWZ_NNLO->SetName("h_DiffXsec_WithinAcc_FEWZ_NNLO");
	h_WithinAcc_FEWZ_NNLO->Write();
}

void MakeDiffXsecHisto( TH1D *h, Double_t *xSec )
{
	Int_t nBins = h->GetNbinsX();
	for(Int_t i=0; i<nBins; i++)
	{
		Int_t i_bin = i+1;
		Double_t BinWidth = h->GetBinWidth(i_bin);

		Double_t DiffxSec = xSec[i] / BinWidth;

		h->SetBinContent(i_bin, DiffxSec);
		h->SetBinError(i_bin, 0);
	}
}