#include <Include/PlotTools.h>

#define nMassBin 43

// -- all uncertainties are % -- //
class UncTool2016_1D
{
public:
	Double_t MassBinEdges[nMassBin+1];

	TH1D* h_RelUnc_Stat;
	TH1D* h_RelUnc_Acc;
	TH1D* h_RelUnc_EffSF;
	TH1D* h_RelUnc_DetRes;
	TH1D* h_RelUnc_Bkg;
	TH1D* h_RelUnc_FSR;
	TH1D* h_RelUnc_Lumi;
	TH1D* h_RelUnc_Syst; // -- without lumi, acceptance uncertainty -- //


	TString FileName_2015;
	TString HistName_2015_Stat;
	TString HistName_2015_EffSF;
	TString HistName_2015_DetRes;
	TString HistName_2015_Bkg;
	TString HistName_2015_FSR;
	TString HistName_2015_Syst;

	TH1D* h_RelUnc_2015_EffSF;
	TH1D* h_RelUnc_2015_DetRes;
	TH1D* h_RelUnc_2015_Bkg;
	TH1D* h_RelUnc_2015_FSR;

	Double_t Lumi_2016;
	Double_t Lumi_2015;
	Double_t LumiScale_Stat;

	UncTool2016_1D()
	{
		Double_t MassBinEdges_temp[nMassBin+1] = 
		{
			15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
			64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
			110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
			200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
			830, 1000, 1500, 3000
		};

		for(Int_t i=0; i<nMassBin+1; i++)
			MassBinEdges[i] = MassBinEdges_temp[i];

		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		this->FileName_2015 = "ROOTFile_Input_Electron_v3.root";
		this->HistName_2015_Stat = "h_RelStatUnc";
		this->HistName_2015_EffSF = "";
		this->HistName_2015_DetRes = "";
		this->HistName_2015_Bkg = "";
		this->HistName_2015_FSR = "h_RelUnc_Syst_FSR";
		this->HistName_2015_Syst = "";

		this->Lumi_2015 = 2759.017;
		this->Lumi_2016 = 35867.060;
		this->LumiScale_Stat = sqrt( Lumi_2015 / Lumi_2016 );
	}

	void Set_All()
	{
		this->SetUncHist_Stat();
		this->SetUncHist_Lumi();

		this->SetUncHist_DetRes();
		this->SetUncHist_Bkg();

		this->SetUncHist_Acc_Using8TeV();
		this->SetUncHist_EffSF();
		this->SetUncHist_FSR();

		this->SetUncHist_Syst();
	}

	void DrawPlots()
	{
		this->DrawSummaryPlot();

		this->DrawComparisonPlot_Stat();
		this->DrawComparisonPlot_Syst();
	}

protected:
	void DrawSummaryPlot()
	{
		// -- stat. unc -- //
		HistInfo* Hist_RelUnc_Stat = new HistInfo(kBlack, "Statistical", this->h_RelUnc_Stat );
		Hist_RelUnc_Stat->h->SetMarkerStyle(20);

		HistInfo* Hist_RelUnc_Syst = new HistInfo(kRed, "Total Systematic", this->h_RelUnc_Syst );
		Hist_RelUnc_Syst->h->SetMarkerStyle(kFullSquare);

		HistInfo* Hist_RelUnc_EffSF = new HistInfo( TColor::GetColor("#0000ff"), "Efficiency SF", this->h_RelUnc_EffSF );
		Hist_RelUnc_EffSF->h->SetMarkerStyle(25);

		HistInfo* Hist_RelUnc_DetRes = new HistInfo( TColor::GetColor("#cc00ff"), "Detector Res.", this->h_RelUnc_DetRes );
		Hist_RelUnc_DetRes->h->SetMarkerStyle(26);

		HistInfo* Hist_RelUnc_Bkg = new HistInfo( TColor::GetColor("#00cc00"), "Background", this->h_RelUnc_Bkg );
		Hist_RelUnc_Bkg->h->SetMarkerStyle(27);
		Hist_RelUnc_Bkg->h->SetMinimum(0.001);

		HistInfo* Hist_RelUnc_FSR = new HistInfo( TColor::GetColor("#9999ff"), "FSR", this->h_RelUnc_FSR );
		Hist_RelUnc_FSR->h->SetMarkerStyle(28);

		HistInfo* Hist_RelUnc_Acc = new HistInfo( TColor::GetColor("#99ff99"), "Acceptance", this->h_RelUnc_Acc );
		Hist_RelUnc_Acc->h->SetMarkerStyle(24);

		HistInfo* Hist_RelUnc_Lumi = new HistInfo( TColor::GetColor("#ff9933"), "Luminosity", this->h_RelUnc_Lumi );
		Hist_RelUnc_Lumi->h->SetMarkerStyle(22);

		TCanvas *c;
		TString CanvasName = "Local/Summary_2016Unc_Electron_1D";
		SetCanvas_Square( c, CanvasName, 1, 1, 800, 700 );

		c->cd();

		Hist_RelUnc_Stat->Draw("HISTLPSAME");
		Hist_RelUnc_Lumi->Draw("HISTLPSAME");
		Hist_RelUnc_Acc->Draw("HISTLPSAME");
		Hist_RelUnc_FSR->Draw("HISTLPSAME");
		Hist_RelUnc_Bkg->Draw("HISTLPSAME");
		Hist_RelUnc_DetRes->Draw("HISTLPSAME");
		Hist_RelUnc_EffSF->Draw("HISTLPSAME");
		Hist_RelUnc_Syst->Draw("HISTLPSAME");

		TLegend *legend;
		SetLegend( legend, 0.48, 0.15, 0.965, 0.50 );
		legend->SetNColumns(2);

		Hist_RelUnc_Stat->AddToLegend( legend );
		Hist_RelUnc_Lumi->AddToLegend( legend );
		Hist_RelUnc_Syst->AddToLegend( legend );
		Hist_RelUnc_Acc->AddToLegend( legend );
		Hist_RelUnc_EffSF->AddToLegend( legend );
		Hist_RelUnc_DetRes->AddToLegend( legend );
		Hist_RelUnc_Bkg->AddToLegend( legend );
		Hist_RelUnc_FSR->AddToLegend( legend );
		legend->Draw();

		SetHistFormat_SinglePad( Hist_RelUnc_Stat->h, "m [GeV]", "Rel. Unc. (%)" );
		Hist_RelUnc_Stat->h->GetYaxis()->SetRangeUser(5e-3, 1e2);

		TLatex latex;
		Latex_Preliminary(latex, 35.9, 13);

		// TLatex latex;
		// latex.SetTextSize(0.04);
		// latex.DrawLatexNDC( 0.725, 0.93, "35.9 fb^{#font[122]{\55}1} (13 TeV)");

		// latex.SetTextSize(0.045);
		// latex.DrawLatexNDC( 0.14, 0.87, "CMS Preliminary" );

		c->SetLogy();
		c->SaveAs(".pdf");
	}

	void DrawComparisonPlot_Stat()
	{
		TH1D* h_RelUnc_2015 = Get_Hist( this->FileName_2015, this->HistName_2015_Stat );
		this->DrawComparisonPlot( h_RelUnc_2015, this->h_RelUnc_Stat, "Stat" );
	}

	void DrawComparisonPlot_Syst()
	{
		TH1D* h_RelUnc_2015 = (TH1D*)this->h_RelUnc_Lumi->Clone();
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t unc1 = this->h_RelUnc_2015_EffSF->GetBinContent(i_bin);
			Double_t unc2 = this->h_RelUnc_2015_DetRes->GetBinContent(i_bin);
			Double_t unc3 = this->h_RelUnc_2015_Bkg->GetBinContent(i_bin);
			Double_t unc4 = this->h_RelUnc_2015_FSR->GetBinContent(i_bin);

			Double_t QuadSum = sqrt( unc1*unc1 + unc2*unc2 + unc3*unc3 + unc4*unc4 );

			h_RelUnc_2015->SetBinContent(i_bin, QuadSum);
			h_RelUnc_2015->SetBinError(i_bin, 0);
		}

		this->DrawComparisonPlot( h_RelUnc_2015, this->h_RelUnc_Syst, "Syst" );

	}

	void DrawComparisonPlot( TH1D* h_2015, TH1D* h_2016, TString Type )
	{
		HistInfo *Hist_2015 = new HistInfo( kGreen+2, "2015 data", h_2015 );
		HistInfo *Hist_2016 = new HistInfo( kBlue, "2016 data ", h_2016 );

		TString CanvasName = "Local/2015_vs_2016_Electron_"+Type;
		DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_2016, Hist_2015);

		canvas->SetTitle("m [GeV]", "Rel. Unc (%)", "2016/2015");
		canvas->SetLegendPosition( 0.60, 0.32, 0.97, 0.45 );
		canvas->SetRatioRange( 0, 1.01 );
		if( Type == "Stat" ) canvas->SetYRange( 5e-3, 2e2 );
		if( Type == "Syst" ) canvas->SetYRange( 0.5, 2e2 );
		
		canvas->SetDrawOption( "HISTLPSAME" );
		canvas->SetLatex( "NoDataInfo" );

		canvas->Draw( 1, 1 );

		canvas->c->cd();
		canvas->TopPad->cd();
		
		TString TStr_Type = "";
		if( Type == "Stat" ) TStr_Type = "Statistical uncertainty";
		if( Type == "Syst" ) TStr_Type = "Systematic uncertainty (w/o lumi, acc unc.)";

		canvas->latex.DrawLatexNDC(0.16, 0.91, "#scale[0.8]{#font[42]{"+TStr_Type+"}}");
		canvas->c->SaveAs(".pdf");
	}

	void SetUncHist_Syst()
	{
		this->h_RelUnc_Syst = new TH1D("h_RelUnc_Syst", "", nMassBin, MassBinEdges);

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t RelUnc_EffSF = h_RelUnc_EffSF->GetBinContent(i_bin);
			Double_t RelUnc_DetRes = h_RelUnc_DetRes->GetBinContent(i_bin);
			Double_t RelUnc_Bkg = h_RelUnc_Bkg->GetBinContent(i_bin);
			Double_t RelUnc_FSR = h_RelUnc_FSR->GetBinContent(i_bin);

			Double_t RelUnc_Syst = sqrt(RelUnc_EffSF*RelUnc_EffSF + RelUnc_DetRes*RelUnc_DetRes + RelUnc_Bkg*RelUnc_Bkg + RelUnc_FSR*RelUnc_FSR);

			this->h_RelUnc_Syst->SetBinContent(i_bin, RelUnc_Syst);
			this->h_RelUnc_Syst->SetBinError(i_bin, 0);
		}
	}

	void SetUncHist_Lumi()
	{
		this->h_RelUnc_Lumi = new TH1D("h_RelUnc_Lumi", "", nMassBin, this->MassBinEdges);
		Double_t RelUnc_Lumi = 2.5;
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			this->h_RelUnc_Lumi->SetBinContent(i_bin, RelUnc_Lumi);
			this->h_RelUnc_Lumi->SetBinError(i_bin, 0);
		}
	}

	void SetUncHist_Stat()
	{
		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		TString FileName = AnalyzerPath+"/FeasibilityStudy_2016/StatUnc/ROOTFile_RelStatUnc_2016_Electron.root";
		TString HistName = "h_RelStatUnc_mass_DY";
		this->h_RelUnc_Stat = Get_Hist(FileName, HistName);
		this->h_RelUnc_Stat->Scale( 100 );
	}

	void SetUncHist_DetRes()
	{
		TH1D* h_RelUnc_2015_StatPart = Get_Hist( this->FileName_2015, "h_RelUnc_Stat_DetRes" );
		TH1D* h_RelUnc_2015_SystPart = Get_Hist( this->FileName_2015, "h_RelUnc_Syst_DetRes" );
		TH1D* h_RelUnc_2015 = QuadSum_NoError( h_RelUnc_2015_StatPart, h_RelUnc_2015_SystPart );
		this->h_RelUnc_2015_DetRes = (TH1D*)h_RelUnc_2015->Clone();

		h_RelUnc_2015->Scale( this->LumiScale_Stat );
		this->h_RelUnc_DetRes = (TH1D*)h_RelUnc_2015->Clone();
	}

	void SetUncHist_Bkg()
	{
		TH1D* h_RelUnc_2015_StatPart = Get_Hist( this->FileName_2015, "h_RelUnc_Stat_BkgEst" );
		TH1D* h_RelUnc_2015_SystPart = Get_Hist( this->FileName_2015, "h_RelUnc_Syst_BkgEst" );
		TH1D* h_RelUnc_2015 = QuadSum_NoError( h_RelUnc_2015_StatPart, h_RelUnc_2015_SystPart );
		this->h_RelUnc_2015_Bkg = (TH1D*)h_RelUnc_2015->Clone();

		h_RelUnc_2015->Scale( this->LumiScale_Stat );
		this->h_RelUnc_Bkg = (TH1D*)h_RelUnc_2015->Clone();
	}

	void SetUncHist_EffSF()
	{
		TH1D* h_RelUnc_2015_StatPart = Get_Hist( this->FileName_2015, "h_RelUnc_Stat_EffSF" );
		TH1D* h_RelUnc_2015_SystPart = Get_Hist( this->FileName_2015, "h_RelUnc_Syst_EffSF" );
		TH1D* h_RelUnc_2015 = QuadSum_NoError( h_RelUnc_2015_StatPart, h_RelUnc_2015_SystPart );
		this->h_RelUnc_2015_EffSF = (TH1D*)h_RelUnc_2015->Clone();

		this->h_RelUnc_EffSF = (TH1D*)h_RelUnc_2015->Clone();
	}

	void SetUncHist_FSR()
	{
		TH1D* h_RelUnc_2015 = Get_Hist( this->FileName_2015, this->HistName_2015_FSR );
		this->h_RelUnc_2015_FSR = (TH1D*)h_RelUnc_2015->Clone();

		this->h_RelUnc_FSR = (TH1D*)h_RelUnc_2015->Clone();
	}

	void SetUncHist_Acc_Using8TeV()
	{
		// -- % -- //
		Double_t RelUnc_AccPDF[nMassBin] =
		{
			4.24, 3.82, 3.60, 3.36, 3.13, 2.91, 2.69, 2.48, 2.29, 2.13, 
			1.99, 1.87, 1.77, 1.67, 1.62, 1.60, 1.62, 1.67, 1.74, 1.83, 
			1.93, 2.04, 2.16, 2.31, 2.45, 2.62, 2.80, 2.98, 3.14, 3.34, 
			3.41, 3.57, 3.74, 3.89, 4.07, 4.24, 4.41, 4.65, 4.91, 4.91, 
			4.91, 5.20, 5.50
		};

		Double_t RelUnc_Modeling[nMassBin] = 
		{
			2.01, 1.65, 1.31, 1.00, 0.72, 0.49, 0.30, 0.17, 0.08, 0.02, 
			0.00, 0.00, 0.01, 0.03, 0.06, 0.10, 0.14, 0.18, 0.23, 0.27, 
			0.32, 0.37, 0.41, 0.46, 0.49, 0.53, 0.56, 0.59, 0.61, 0.65, 
			0.70, 0.76, 0.85, 0.96, 1.10, 1.28, 1.48, 1.70, 1.95, 1.95, 
			1.95, 2.20, 2.42
		};

		TH1D* h_RelUnc_AccPDF = this->MakeHist_FromArray( RelUnc_AccPDF );
		TH1D* h_RelUnc_Modeling = this->MakeHist_FromArray( RelUnc_Modeling );

		this->h_RelUnc_Acc = QuadSum_NoError( h_RelUnc_AccPDF, h_RelUnc_Modeling );
	}

	TH1D* MakeHist_FromArray( Double_t Array[nMassBin] )
	{
		TH1D* h = new TH1D("h", "", nMassBin, MassBinEdges );
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			h->SetBinContent(i_bin, Array[i]);
			h->SetBinError(i_bin, 0);
		}

		return h;
	}

};

void Estimate2016_1D_Electron()
{
	UncTool2016_1D* tool = new UncTool2016_1D();
	tool->Set_All();
	tool->DrawPlots();	
}