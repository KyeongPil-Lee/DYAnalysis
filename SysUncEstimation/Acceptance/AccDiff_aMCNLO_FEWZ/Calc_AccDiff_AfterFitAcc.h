#include <TMath.h>
#include <Include/PlotTools.h>

Double_t FitFunc_Low(Double_t* x, Double_t* par)
{
	Double_t arg1 = par[0];

	Double_t ExpArg = (par[2] - x[0]) / par[3];
	Double_t arg2 = (-1)*par[1]*TMath::Exp( ExpArg );

	Double_t arg3 = par[4]*TMath::Power( x[0], par[5] );

	return arg1 + arg2 + arg3;
}

Double_t FitFunc_High(Double_t* x, Double_t* par)
{
	Double_t arg1 = par[0];

	Double_t arg2 = par[1] / TMath::Power( x[0]+par[2] , 3);

	Double_t arg3 = x[0]*x[0] / par[3];

	return arg1 + arg2 + arg3;
}

class FittingTool
{
public:
	TString TStr_Channel;

	TString AnalyzerPath;

	TH1D* h_Acc_aMCNLO;
	TH1D* h_Acc_FEWZ;

	TH1D* h_Acc_aMCNLO_ByFit;
	TH1D* h_Acc_FEWZ_ByFit;

	TH1D* h_AccDiff_ByFit;

	FittingTool( TString _TStr_Channel )
	{
		this->AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

		this->TStr_Channel = _TStr_Channel;
		this->Get_Histogram();
	}

	void Fit_All()
	{
		this->Fit_Acc( this->h_Acc_aMCNLO, "aMCNLO" );
		this->Fit_Acc( this->h_Acc_FEWZ, "FEWZ" );
	}

	void Calc_AccDiff()
	{
		this->Calc_AccDiff_ByFit();
		this->Compare_AccDiff_BinByBin_ByFit();
	}

	void Save( TFile *f_output )
	{
		f_output->cd();
		this->h_Acc_aMCNLO->SetName("h_Acc_aMCNLO_"+this->TStr_Channel);
		this->h_Acc_aMCNLO->Write();
		this->h_Acc_FEWZ->SetName("h_Acc_FEWZ_"+this->TStr_Channel);
		this->h_Acc_FEWZ->Write();

		this->h_Acc_aMCNLO_ByFit->SetName("h_Acc_aMCNLO_ByFit_"+this->TStr_Channel);
		this->h_Acc_aMCNLO_ByFit->Write();
		this->h_Acc_FEWZ_ByFit->SetName("h_Acc_FEWZ_ByFit_"+this->TStr_Channel);
		this->h_Acc_FEWZ_ByFit->Write();

		this->h_AccDiff_ByFit->SetName("h_AccDiff_ByFit_"+this->TStr_Channel);
		this->h_AccDiff_ByFit->Write();
	}

protected:
	void Fit_Acc( TH1D* h_Acc, TString Type )
	{
		Double_t MassCut1 = 40; Double_t dMass1 = 5; // -- distance with the next bin -- //
		if( this->TStr_Channel == "Electron" )
		{
			MassCut1 = 60;
			dMass1 = 4;
		}
		Double_t MassCut2 = 200; Double_t dMass2 = 20;
		TH1D* h_Acc_withFit_Low = this->Fit_Polynomial( h_Acc, 15, MassCut1+dMass1 );
		TH1D* h_Acc_withFit_Middle = this->Fit_UserFunc( h_Acc, MassCut1, MassCut2+dMass2 );
		TH1D* h_Acc_withFit_High = this->Fit_UserFunc( h_Acc, MassCut2, 3000 );

		TF1 *FitFunc_Low = h_Acc_withFit_Low->GetFunction("pol3");
		FitFunc_Low->SetLineWidth(3);
		FitFunc_Low->SetLineColor(kRed);

		TF1 *FitFunc_Middle = h_Acc_withFit_Middle->GetFunction("FitFunc_Low");
		FitFunc_Middle->SetLineWidth(3);
		FitFunc_Middle->SetLineColor(kBlue);

		TF1 *FitFunc_High = h_Acc_withFit_High->GetFunction("FitFunc_Low");
		FitFunc_High->SetLineWidth(3);
		FitFunc_High->SetLineColor(kGreen+2);

		TH1D* h_Acc_ByFit = this->MakeHistogram_Acc_ByFit( FitFunc_Low, FitFunc_Middle, FitFunc_High, MassCut1, MassCut2 );
		if( Type == "aMCNLO" )
			this->h_Acc_aMCNLO_ByFit = (TH1D*)h_Acc_ByFit->Clone();
		else if( Type == "FEWZ" )
			this->h_Acc_FEWZ_ByFit = (TH1D*)h_Acc_ByFit->Clone();

		TH1D* h_RelDiff_withFit = this->CalcDiff_Bin_vs_Fit( h_Acc, h_Acc_ByFit );

		///////////////////////
		// -- draw canvas -- //
		///////////////////////
		gStyle->SetOptFit(1111);
		gStyle->SetOptStat(1000000001); // -- only prints the name of histogram -- //

		HistInfo *Hist_Low = new HistInfo( kBlack, Type, h_Acc_withFit_Low );
		HistInfo *Hist_Middle = new HistInfo( kBlack, Type, h_Acc_withFit_Middle );
		HistInfo *Hist_High = new HistInfo( kBlack, Type, h_Acc_withFit_High );

		// -- set name -- //
		Hist_Low->h->SetName( TString::Format("15 < M < %.0lf GeV", MassCut1) );
		Hist_Low->h->SetStats(kTRUE);

		Hist_Middle->h->SetName( TString::Format("%.0lf < M < %.0lf GeV", MassCut1, MassCut2) );
		Hist_Middle->h->SetStats(kTRUE);

		Hist_High->h->SetName( TString::Format("%.0lf < M < 3000 GeV", MassCut2) );
		Hist_High->h->SetStats(kTRUE);

		// -- canvas -- //
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString CanvasName = "Local/Acc_AfterFit_" + Type + "_" + this->TStr_Channel;

		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 1, 0 );
		c->cd();
		TopPad->cd();
		Hist_Low->Draw("EPSAME");
		Hist_Middle->Draw("EPSAMES");
		Hist_High->Draw("EPSAMES");
		SetHistFormat_TopPad( Hist_Low->h, "Acceptance" );
		Hist_Low->h->SetStats(kTRUE);

		gPad->Update(); // -- essential to draw stat box first on the pad -- //
		TPaveStats *StatBox_Low = (TPaveStats*)Hist_Low->h->GetListOfFunctions()->FindObject("stats");
		StatBox_Low->SetX1NDC(0.17);
		StatBox_Low->SetX2NDC(0.47);
		StatBox_Low->SetY1NDC(0.68);
		StatBox_Low->SetY2NDC(0.89);

		TPaveStats *StatBox_Middle = (TPaveStats*)Hist_Middle->h->GetListOfFunctions()->FindObject("stats");
		StatBox_Middle->SetX1NDC(0.40);
		StatBox_Middle->SetX2NDC(0.67);
		StatBox_Middle->SetY1NDC(0.32);
		StatBox_Middle->SetY2NDC(0.57);

		TPaveStats *StatBox_High = (TPaveStats*)Hist_High->h->GetListOfFunctions()->FindObject("stats");
		StatBox_High->SetX1NDC(0.67);
		StatBox_High->SetX2NDC(0.95);
		StatBox_High->SetY1NDC(0.32);
		StatBox_High->SetY2NDC(0.57);

		TLatex latex;
		Latex_Simulation( latex );

		if( Type == "aMCNLO" )
			latex.DrawLatexNDC(0.16, 0.91, "#scale[0.8]{#font[42]{aMC@NLO}}");
		else if( Type == "FEWZ" )
			latex.DrawLatexNDC(0.16, 0.91, "#scale[0.8]{#font[42]{FEWZ (NNLO)}}");

		c->cd();
		BottomPad->cd();
		HistInfo* Hist_RelDiff = new HistInfo( kBlack, "", h_RelDiff_withFit );
		Hist_RelDiff->Draw("LPSAME");
		SetHistFormat_BottomPad( Hist_RelDiff->h, "m [GeV]", "(Bin-Fit)/Bin", -0.12, 0.12 );

		TF1* line;
		DrawLine( line );

		c->SaveAs(".pdf");

		// -- details in each edge -- //
		c->cd(); TopPad->cd(); TopPad->SetLogx(kFALSE);
		c->cd(); BottomPad->cd(); BottomPad->SetLogx(kFALSE);
		Hist_Low->h->SetStats(kFALSE);
		Hist_Middle->h->SetStats(kFALSE);
		Hist_High->h->SetStats(kFALSE);

		Hist_Low->h->GetXaxis()->SetRangeUser( MassCut1-15, MassCut1+15 );
		Hist_RelDiff->h->GetXaxis()->SetRangeUser( MassCut1-15, MassCut1+15 );
		c->SaveAs(CanvasName+"_Edge1.pdf");

		Hist_Low->h->GetXaxis()->SetRangeUser( MassCut2-50, MassCut2+50 );
		Hist_RelDiff->h->GetXaxis()->SetRangeUser( MassCut2-50, MassCut2+50 );
		c->SaveAs(CanvasName+"_Edge2.pdf");
	}

	TH1D* Fit_Polynomial( TH1D* h, Double_t LowerEdge, Double_t UpperEdge )
	{
		TH1D* h_withFit = (TH1D*)h->Clone();
		h_withFit->Fit( "pol3", "R", "", LowerEdge, UpperEdge );

		return h_withFit;
	}

	TH1D* Fit_UserFunc( TH1D* h, Double_t LowerEdge, Double_t UpperEdge )
	{
		TH1D* h_withFit = (TH1D*)h->Clone();

		TF1 *func;
		func = new TF1("FitFunc_Low", FitFunc_Low, LowerEdge, UpperEdge, 6);
		func->SetParameter(0, 1); func->SetParName(0, "p0");
		func->SetParameter(1, -1); func->SetParName(1, "p1");
		func->SetParameter(2, 50); func->SetParName(2, "p2");
		func->SetParameter(3, 87); func->SetParName(3, "p3");
		func->SetParameter(4, -11); func->SetParName(4, "p4");
		func->SetParameter(5, -0.5); func->SetParName(5, "p5");

		// -- test -- //
		// func->FixParameter(3,0);
		// func->FixParameter(1,0);
		// func->FixParameter(2,0);
		// func->FixParameter(3,0);
		// func->FixParameter(4,0);
		// func->FixParameter(5,0);

		h_withFit->Fit( "FitFunc_Low", "R", "", LowerEdge, UpperEdge );

		return h_withFit;
	}

	void Get_Histogram()
	{
		if( this->TStr_Channel == "Muon" )
		{
			TString FileName_aMCNLO = this->AnalyzerPath+"/SysUncEstimation/Acceptance/AccDiff_aMCNLO_FEWZ/ROOTFile_SysUnc_DiffWithFEWZ.root";
			this->h_Acc_aMCNLO = Get_Hist( FileName_aMCNLO, "h_Acc_aMCNLO");

			TString FileName_FEWZ = FileName_aMCNLO;
			this->h_Acc_FEWZ = Get_Hist( FileName_FEWZ, "h_Acc_FEWZ_NNLO_PDF4LHC15");
		}
		else if( this->TStr_Channel == "Electron" )
		{
			TString FileName_aMCNLO = "ROOTFile_Acc_Dressed_EE.root";
			TGraphAsymmErrors* g_aMCNLO = Get_Graph( FileName_aMCNLO, "g_Acc" );
			this->h_Acc_aMCNLO = Convert_GraphToHist( g_aMCNLO );

			TString FileName_FEWZ = "ROOTFile_Acc_Electron_FEWZ_PDF4LHC15.root";
			this->h_Acc_FEWZ = Get_Hist( FileName_FEWZ, "h_Acc" );
		}
	}

	TH1D* MakeHistogram_Acc_ByFit( TF1* FitFunc_Low, TF1* FitFunc_Middle, TF1* FitFunc_High, Double_t MassCut1, Double_t MassCut2 )
	{
		TH1D* h_Acc_ByFit = (TH1D*)this->h_Acc_FEWZ->Clone();

		Int_t nBin = h_Acc_ByFit->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t BinCenter = h_Acc_ByFit->GetBinCenter(i_bin);

			Double_t Acc_Fit = 0;
			if( BinCenter < MassCut1 )
				Acc_Fit = FitFunc_Low->Eval( BinCenter );
			else if( BinCenter < MassCut2 ) // -- MassCut1 < M < MassCut2  -- //
				Acc_Fit = FitFunc_Middle->Eval( BinCenter );
			else
				Acc_Fit = FitFunc_High->Eval( BinCenter );

			h_Acc_ByFit->SetBinContent(i_bin, Acc_Fit);
			h_Acc_ByFit->SetBinError(i_bin, 0);
		}

		return h_Acc_ByFit;
	}

	TH1D* CalcDiff_Bin_vs_Fit( TH1D* h_Acc, TH1D* h_Acc_ByFit )
	{
		TH1D* h_Diff_WithFit = (TH1D*)h_Acc->Clone();

		Int_t nBin = h_Acc->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t Acc_Bin = h_Acc->GetBinContent(i_bin);
			Double_t Acc_Fit = h_Acc_ByFit->GetBinContent(i_bin);

			Double_t RelDiff = (Acc_Fit - Acc_Bin ) / Acc_Bin;
			h_Diff_WithFit->SetBinContent(i_bin, RelDiff);
			h_Diff_WithFit->SetBinError(i_bin, 0);

			printf("[%02d bin] (Bin, Fit) = (%.5lf, %.5lf)-> Rel.Diff = %.5lf %%\n", i_bin, Acc_Bin, Acc_Fit, RelDiff*100);
		}

		return h_Diff_WithFit;
	}

	void Calc_AccDiff_ByFit()
	{
		this->h_AccDiff_ByFit = (TH1D*)this->h_Acc_FEWZ->Clone();
		Int_t nBin = this->h_AccDiff_ByFit->GetNbinsX();

		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t Acc_aMCNLO = this->h_Acc_aMCNLO_ByFit->GetBinContent(i_bin);
			Double_t Acc_FEWZ = this->h_Acc_FEWZ_ByFit->GetBinContent(i_bin);

			Double_t RelDiff = fabs( (Acc_FEWZ - Acc_aMCNLO ) / Acc_aMCNLO );

			this->h_AccDiff_ByFit->SetBinContent(i_bin, RelDiff);
			this->h_AccDiff_ByFit->SetBinError(i_bin, 0);
		}
	}

	void Compare_AccDiff_BinByBin_ByFit()
	{
		// TString FileName_BinByBin = AnalyzerPath + "/SysUncEstimation/Acceptance/AccDiff_aMCNLO_FEWZ/ROOTFile_SysUnc_DiffWithFEWZ.root";
		// TH1D* h_BinByBin = Get_Hist( FileName_BinByBin, "h_RelUnc" );

		TH1D* h_BinByBin = this->Calc_AccDiff_BinByBin();

		HistInfo* Hist_BinByBin = new HistInfo( kBlack, "Bin by Bin", h_BinByBin );
		HistInfo* Hist_ByFit = new HistInfo( kRed, "By Fit", this->h_AccDiff_ByFit );
		Hist_ByFit->CalcRatio_DEN( Hist_BinByBin->h );

		TString CanvasName = "Local/AccDiff_BinByBin_vs_ByFit_"+this->TStr_Channel;
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 1, 0 );

		c->cd();
		TopPad->cd();

		Hist_BinByBin->Draw("LPSAME");
		Hist_ByFit->Draw("LPSAME");
		SetHistFormat_TopPad( Hist_BinByBin->h, "Rel. Unc.");
		Hist_BinByBin->h->GetYaxis()->SetRangeUser( 0, 0.20 );

		TLegend *legend;
		SetLegend( legend, 0.60, 0.78, 0.97, 0.88);
		Hist_BinByBin->AddToLegend( legend );
		Hist_ByFit->AddToLegend( legend );
		legend->Draw();

		TLatex latex;
		Latex_Simulation( latex );
		latex.DrawLatexNDC(0.16, 0.915, "#scale[0.6]{#font[42]{Uncertainty from the acc. diff. with FEWZ}}");
		if( this->TStr_Channel == "Muon" )
			latex.DrawLatexNDC(0.16, 0.885, "#scale[0.6]{#font[42]{#mu channel}}");
		else if( this->TStr_Channel == "Electron" )
			latex.DrawLatexNDC(0.16, 0.885, "#scale[0.6]{#font[42]{e channel}}");

		c->cd();
		BottomPad->cd();
		Hist_ByFit->DrawRatio("LPSAME");
		SetHistFormat_BottomPad( Hist_ByFit->h_ratio, "m [GeV]", "Fit/BinByBin", 0, 2.5 );

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}

	TH1D* Calc_AccDiff_BinByBin()
	{
		TH1D* h_RelDiff = (TH1D*)this->h_Acc_aMCNLO->Clone();

		Int_t nBin = this->h_Acc_aMCNLO->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t Acc_aMCNLO = this->h_Acc_aMCNLO->GetBinContent(i_bin);
			Double_t Acc_FEWZ = this->h_Acc_FEWZ->GetBinContent(i_bin);

			Double_t RelDiff = fabs(Acc_FEWZ - Acc_aMCNLO) / Acc_aMCNLO;

			h_RelDiff->SetBinContent(i_bin, RelDiff);
			h_RelDiff->SetBinError(i_bin, 0);
		}

		return h_RelDiff;
	}
};