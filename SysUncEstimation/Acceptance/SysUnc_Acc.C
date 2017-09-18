#include <Include/MyCanvas.C>
#include <Include/PlotTools.h>

void MakeHistogram_XSec(TH1D *h, Double_t *value, Double_t *error);
Double_t Calc_WeightedAverage_Zpeak(TH1D* h_mass, TH1D* h_RelSysUnc);
void Calc_RelSysUnc_Alpha( TH1D* h_xSec_alpha_0118, TH1D* h_xSec_alpha_0117, TH1D* h_xSec_alpha_0119, TH1D* h_RelSysUnc );
void DrawCanvas_SysUnc(TH1D* h1, TH1D* h2, TH1D* h3, TH1D *h_tot, TFile *f_output);
void TotRelSysUnc( TH1D *h1, TH1D *h2, TH1D* h3, TH1D *h_tot );
void SysUnc_Acc()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};


	////////////////////////////////
	// -- Uncertainty from PDF -- //
	////////////////////////////////
	TString FileName_PDF = "ROOTFile_AccWithUnc_FEWZ_PDF4LHC15.root";
	TH1D* h_RelSysUnc_PDF_Percent = Get_Hist( FileName_PDF, "h_RelSysUnc_NNLO_Percent", "h_RelSysUnc_PDF_Percent" );
	// TH1D* h_RelSysUnc_PDF_Percent = (TH1D*)h_RelSysUnc_NNLO_Percent->Clone("h_RelSysUnc_PDF_Percent");

	///////////////////////////////////////////////////////////////
	// -- Uncertainty from variation of the coupling constant -- //
	///////////////////////////////////////////////////////////////
	Double_t xSec_0118[nMassBin+2];
	Double_t error_0118[nMassBin+2];
	Double_t xSec_0117[nMassBin+2];
	Double_t error_0117[nMassBin+2];
	Double_t xSec_0119[nMassBin+2];
	Double_t error_0119[nMassBin+2];

	FILE *fl;
	Double_t LowerEdge;
	Double_t UpperEdge;
	Double_t xSec_0118_temp;
	Double_t error_0118_temp;
	Double_t xSec_0117_temp;
	Double_t error_0117_temp;	
	Double_t xSec_0119_temp;
	Double_t error_0119_temp;


	printf("\n==========[AccErrors_CouplingConstant.txt]==========\n");
	if ((fl = fopen("AccErrors_CouplingConstant.txt", "r")) != NULL)
	{
		Int_t i=0;

		while(fscanf(fl, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", &LowerEdge, &UpperEdge, &xSec_0118_temp, &error_0118_temp, &xSec_0117_temp, &error_0117_temp, &xSec_0119_temp, &error_0119_temp) != EOF )
		{
			xSec_0118[i] = xSec_0118_temp;
			error_0118[i] = error_0118_temp;
			xSec_0117[i] = xSec_0117_temp;
			error_0117[i] = error_0117_temp;
			xSec_0119[i] = xSec_0119_temp;
			error_0119[i] = error_0119_temp;

			printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", LowerEdge, UpperEdge, xSec_0118[i], error_0118[i], xSec_0117[i], error_0117[i], xSec_0119[i], error_0119[i]);
			i++;

			if( i == 45 )
				break;
		}
	}
	fclose(fl);

	TH1D* h_xSec_alpha_0118 = new TH1D("h_xSec_alpha_0118", "", nMassBin, MassBinEdges);
	MakeHistogram_XSec( h_xSec_alpha_0118, xSec_0118, error_0118 );

	TH1D* h_xSec_alpha_0117 = new TH1D("h_xSec_alpha_0117", "", nMassBin, MassBinEdges);
	MakeHistogram_XSec( h_xSec_alpha_0117, xSec_0117, error_0117 );

	TH1D* h_xSec_alpha_0119 = new TH1D("h_xSec_alpha_0119", "", nMassBin, MassBinEdges);
	MakeHistogram_XSec( h_xSec_alpha_0119, xSec_0119, error_0119 );

	// MyCanvas *myc3 = new MyCanvas("c_NLOXsec_VariationOfAlpha", "Dimuon mass [GeV]", "#sigma [pb] (NLO)");
	// myc3->SetLogx(1);
	// myc3->SetLogy(0);
	// myc3->SetRatioRange(0.98, 1.02);
	// myc3->CanvasWithThreeHistogramsRatioPlot( (TH1D*)h_xSec_alpha_0117->Clone(), (TH1D*)h_xSec_alpha_0119->Clone(), (TH1D*)h_xSec_alpha_0118->Clone(), "#alpha = 0.117", "#alpha = 0.119", "#alpha = 0.118 (C.V.)", "Ratio to 0.118");
	// myc3->PrintCanvas();

	TH1D *h_RelSysUnc_Alpha_Percent = (TH1D*)h_xSec_alpha_0118->Clone("h_RelSysUnc_Alpha_Percent");
	Calc_RelSysUnc_Alpha( h_xSec_alpha_0118, h_xSec_alpha_0117, h_xSec_alpha_0119, h_RelSysUnc_Alpha_Percent );

	////////////////////////////////////////////////////////////////////////
	// -- Uncertainty from acceptance difference between aMC@NLO and FEWZ //
	////////////////////////////////////////////////////////////////////////
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	// TString FileName_AccDiff = AnalyzerPath+"/SysUncEstimation/Acceptance/AccDiff_aMCNLO_FEWZ/ROOTFile_SysUnc_DiffWithFEWZ.root";
	// TH1D* h_RelSysUnc_AccDiff_Percent = Get_Hist(FileName_AccDiff, "h_RelUnc", "h_RelSysUnc_AccDiff_Percent");
	// h_RelSysUnc_AccDiff_Percent->Scale( 100 );
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_AccDiff = ROOTFilePath+"/ROOTFile_Unc_AccDiff_aMCNLO_FEWZ_ByFit.root";
	TString HistName_AccDiff = "h_AccDiff_ByFit_Muon";
	TH1D* h_RelSysUnc_AccDiff_Percent = Get_Hist( FileName_AccDiff, HistName_AccDiff);
	h_RelSysUnc_AccDiff_Percent->Scale( 100 );

	// -- save -- //
	TFile *f_output = new TFile("ROOTFile_SysUnc_Acceptance.root", "RECREATE");
	f_output->cd();
	
	h_RelSysUnc_PDF_Percent->Write();

	h_xSec_alpha_0118->Write();
	h_xSec_alpha_0117->Write();
	h_xSec_alpha_0119->Write();
	// myc3->c->Write();
	h_RelSysUnc_Alpha_Percent->Write();
	h_RelSysUnc_AccDiff_Percent->Write();

	TH1D* h_RelSysUnc_Tot_Percent = (TH1D*)h_RelSysUnc_PDF_Percent->Clone("h_RelSysUnc_Tot_Percent");
	TotRelSysUnc(h_RelSysUnc_PDF_Percent, h_RelSysUnc_Alpha_Percent, h_RelSysUnc_AccDiff_Percent, h_RelSysUnc_Tot_Percent);

	DrawCanvas_SysUnc(h_RelSysUnc_PDF_Percent, h_RelSysUnc_Alpha_Percent, h_RelSysUnc_AccDiff_Percent, h_RelSysUnc_Tot_Percent, f_output);
	h_RelSysUnc_Tot_Percent->Write();

	// TString FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles_76X/" + version;
	TString FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");
	TFile *f_input = new TFile(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root"); f_input->cd();
	TH1D *h_MC_postFSR = (TH1D*)f_input->Get("h_DYMC_Gen_postFSR")->Clone();

	cout << "\n======[from PDF uncertainty]=======" << endl;
	Double_t RelUnc_Zpeak_PDF = Calc_WeightedAverage_Zpeak(h_MC_postFSR, h_RelSysUnc_PDF_Percent);

	cout << "\n======[from alpha uncertainty]=======" << endl;
	Double_t RelUnc_Zpeak_Alpha = Calc_WeightedAverage_Zpeak(h_MC_postFSR, h_RelSysUnc_Alpha_Percent);

	cout << "\n======[from diff. with NNLO uncertainty]=======" << endl;
	Double_t RelUnc_Zpeak_AccDiff = Calc_WeightedAverage_Zpeak(h_MC_postFSR, h_RelSysUnc_AccDiff_Percent);

	cout << "\n======[total uncertainty]=======" << endl;
	Double_t RelUnc_Zpeak_Tot = Calc_WeightedAverage_Zpeak(h_MC_postFSR, h_RelSysUnc_Tot_Percent);

	SaveAsHist_OneContent( RelUnc_Zpeak_PDF/100.0, "h_RelUnc_Zpeak_PDF", f_output );
	SaveAsHist_OneContent( RelUnc_Zpeak_Alpha/100.0, "h_RelUnc_Zpeak_Alpha", f_output );
	SaveAsHist_OneContent( RelUnc_Zpeak_AccDiff/100.0, "h_RelUnc_Zpeak_AccDiff", f_output );
	SaveAsHist_OneContent( RelUnc_Zpeak_Tot/100.0, "h_RelUnc_Zpeak_Tot", f_output );
}

Double_t Calc_WeightedAverage_Zpeak(TH1D* h_mass, TH1D* h_RelSysUnc)
{
	Double_t nEvent_Zpeak = 0;
	Int_t nBin = h_RelSysUnc->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Int_t BinCenter = h_mass->GetBinCenter(i_bin);

		if( BinCenter > 60 && BinCenter < 120 )
		{
			Double_t nEvent = h_mass->GetBinContent(i_bin);
			nEvent_Zpeak += nEvent;
		}
	}

	printf("[# events in Z-peak region: %.1lf]\n", nEvent_Zpeak);

	Double_t Sum_Weighted = 0;
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Int_t BinCenter = h_mass->GetBinCenter(i_bin);

		if( BinCenter > 60 && BinCenter < 120 )
		{
			Double_t nEvent = h_mass->GetBinContent(i_bin);
			Double_t weight = nEvent / nEvent_Zpeak;

			Double_t Unc = h_RelSysUnc->GetBinContent(i_bin);

			Sum_Weighted += weight * Unc;
		}
	}

	printf("[Weighted average of the uncertainty in Z-peak region: %.5lf (%%)]\n", Sum_Weighted);

	return Sum_Weighted;
}

void MakeHistogram_XSec(TH1D *h, Double_t *value, Double_t *error)
{
	Int_t nBin = h->GetNbinsX(); // -- nbin = 43 -- //
	for(Int_t i=0; i<nBin-2; i++)
	{
		Int_t i_bin = i+1;
		h->SetBinContent( i_bin, value[i] );
		h->SetBinError( i_bin, error[i] );
	}

	// -- [1000, 1200], [1200, 1500] -> [1000, 1500] -- //
	Double_t value_Bin42 = value[41] + value[42];
	Double_t error_Bin42 = sqrt( error[41]*error[41] + error[42]*error[42] );

	h->SetBinContent( 42, value_Bin42 );
	h->SetBinError( 42, error_Bin42 );

	// -- [1500, 2000], [2000, 3000] -> [1500, 3000] -- //
	Double_t value_Bin43 = value[43] + value[44];
	Double_t error_Bin43 = sqrt( error[43]*error[43] + error[44]*error[44] );

	h->SetBinContent( 43, value_Bin43 );
	h->SetBinError( 43, error_Bin43 );
}

void Calc_RelSysUnc_Alpha( TH1D* h_xSec_alpha_0118, TH1D* h_xSec_alpha_0117, TH1D* h_xSec_alpha_0119, TH1D* h_RelSysUnc )
{
	Int_t nBin = h_xSec_alpha_0118->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t value_0118 = h_xSec_alpha_0118->GetBinContent(i_bin);
		Double_t value_0119 = h_xSec_alpha_0119->GetBinContent(i_bin);
		Double_t value_0117 = h_xSec_alpha_0117->GetBinContent(i_bin);

		Double_t error_0119 = fabs(value_0118 - value_0119 ) / value_0118;
		Double_t error_0117 = fabs(value_0118 - value_0117 ) / value_0118;

		Double_t error = 0;
		if( error_0119 > error_0117 ) 
			error = error_0119;
		else 
			error = error_0117;

		h_RelSysUnc->SetBinContent(i_bin, error);
		h_RelSysUnc->SetBinError(i_bin, 0);
	}
	h_RelSysUnc->Scale( 100 ); // -- convert to % -- //
}

void DrawCanvas_SysUnc(TH1D* h1, TH1D* h2, TH1D* h3, TH1D *h_tot, TFile* f_output)
{
	TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_Acc", "", 800, 800);
	c_RelUnc->cd();

	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);
	gPad->SetLeftMargin(0.15);
	gPad->SetLogx();
	gPad->SetGridx();
	gPad->SetGridy();

	h1->Draw("HISTLP");
	h2->Draw("HISTLPSAME");
	h3->Draw("HISTLPSAME");
	h_tot->Draw("HISTLPSAME");

	h1->SetStats(kFALSE);
	h1->GetXaxis()->SetLabelSize(0.04);
	h1->GetXaxis()->SetTitleSize(0.04);
	h1->GetXaxis()->SetNoExponent();
	h1->GetXaxis()->SetMoreLogLabels();
	h1->GetYaxis()->SetTitleOffset(1.2);
	h1->GetYaxis()->SetRangeUser(0, 12);

	h1->SetXTitle( "Dimuon Mass [GeV]");
	h1->SetYTitle( "Relative Uncertainty (%)");

	h1->SetMarkerStyle(20);
	h1->SetMarkerColor(kBlue);
	h1->SetLineColor(kBlue);
	h1->SetFillColorAlpha( kWhite, 0);

	h2->SetStats(kFALSE);
	h2->SetMarkerStyle(20);
	h2->SetMarkerColor(kGreen+1);
	h2->SetLineColor(kGreen+1);
	h2->SetFillColorAlpha( kWhite, 0);

	h3->SetStats(kFALSE);
	h3->SetMarkerStyle(20);
	h3->SetMarkerColor(kBlack);
	h3->SetLineColor(kBlack);
	h3->SetFillColorAlpha( kWhite, 0);

	h_tot->SetStats(kFALSE);
	h_tot->SetMarkerStyle(20);
	h_tot->SetMarkerColor(kRed);
	h_tot->SetLineColor(kRed);
	h_tot->SetFillColorAlpha( kWhite, 0);

	TLegend *legend = new TLegend(0.35, 0.60, 0.95, 0.95);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->AddEntry( h1, "Syst. from PDF");
	legend->AddEntry( h2, "Syst. from variation of #alpha");
	legend->AddEntry( h3, "Syst. from diff. with NNLO (FEWZ)");
	legend->AddEntry( h_tot, "Total Syst. from Acceptance" );
	legend->Draw();

	c_RelUnc->SaveAs( "./Local/c_RelSysUnc_Acc.pdf" );

	f_output->cd();
	c_RelUnc->Write();
}

void TotRelSysUnc( TH1D *h1, TH1D *h2, TH1D* h3, TH1D *h_tot )
{
	Int_t nBin = h1->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t unc1 = h1->GetBinContent(i_bin);
		Double_t unc2 = h2->GetBinContent(i_bin);
		Double_t unc3 = h3->GetBinContent(i_bin);

		Double_t totunc = sqrt( unc1 * unc1 + unc2 * unc2 + unc3 * unc3);

		h_tot->SetBinContent(i_bin, totunc);
		h_tot->SetBinError(i_bin, 0);
	}
}
