#include <Include/PlotTools.h>

#define nMassRange 6

class UncTool2016_2D
{
public:
	TString TStr_MassRange[nMassRange];
	TH1D* h_RelUnc_Stat[nMassRange];
	TH1D* h_RelUnc_EffSF[nMassRange];
	TH1D* h_RelUnc_DetRes[nMassRange];
	TH1D* h_RelUnc_Bkg[nMassRange];
	TH1D* h_RelUnc_FSR[nMassRange];
	TH1D* h_RelUnc_CollCS[nMassRange];
	TH1D* h_RelUnc_Lumi[nMassRange];
	TH1D* h_RelUnc_Syst[nMassRange];

	Double_t Lumi_2012;
	Double_t Lumi_2016;
	Double_t LumiScale_Stat;

	UncTool2016_2D()
	{
		this->TStr_MassRange[0] = "M20to30";
		this->TStr_MassRange[1] = "M30to45";
		this->TStr_MassRange[2] = "M45to60";
		this->TStr_MassRange[3] = "M60to120";
		this->TStr_MassRange[4] = "M120to200";
		this->TStr_MassRange[5] = "M200to1500";

		this->Lumi_2012 = 19700.000;
		this->Lumi_2016 = 35867.060;
		this->LumiScale_Stat = sqrt( Lumi_2012 / Lumi_2016 );
	}

	void Set_All()
	{
		this->SetUncHist_Stat();
		this->SetUncHist_Lumi();

		this->SetUncHist_SystSource_M20to30();
		this->SetUncHist_SystSource_M30to45();
		this->SetUncHist_SystSource_M45to60();
		this->SetUncHist_SystSource_M60to120();
		this->SetUncHist_SystSource_M120to200();
		this->SetUncHist_SystSource_M200to1500();
	}

	void DrawPlots()
	{
		// this->DrawSummaryPlot(0);
		// this->DrawSummaryPlot(1);
		// this->DrawSummaryPlot(2);
		for(Int_t i_mr=0; i_mr<nMassRange; i_mr++)
		{
			this->DrawSummaryPlot(i_mr);
		}
	}

protected:
	void DrawSummaryPlot(Int_t i_mr )
	{
		// -- stat. unc -- //
		HistInfo* Hist_RelUnc_Stat = new HistInfo(kBlack, "Statistical", this->h_RelUnc_Stat[i_mr] );
		Hist_RelUnc_Stat->h->SetMarkerStyle(20);

		HistInfo* Hist_RelUnc_Syst = new HistInfo(kRed, "Total Systematic", this->h_RelUnc_Syst[i_mr] );
		Hist_RelUnc_Syst->h->SetMarkerStyle(kFullSquare);

		HistInfo* Hist_RelUnc_EffSF = new HistInfo( TColor::GetColor("#0000ff"), "Efficiency SF", this->h_RelUnc_EffSF[i_mr] );
		Hist_RelUnc_EffSF->h->SetMarkerStyle(25);

		HistInfo* Hist_RelUnc_DetRes = new HistInfo( TColor::GetColor("#cc00ff"), "Detector Res.", this->h_RelUnc_DetRes[i_mr] );
		Hist_RelUnc_DetRes->h->SetMarkerStyle(26);

		HistInfo* Hist_RelUnc_Bkg = new HistInfo( TColor::GetColor("#00cc00"), "Background", this->h_RelUnc_Bkg[i_mr] );
		Hist_RelUnc_Bkg->h->SetMarkerStyle(27);
		Hist_RelUnc_Bkg->h->SetMinimum(0.001);

		HistInfo* Hist_RelUnc_FSR = new HistInfo( TColor::GetColor("#9999ff"), "FSR", this->h_RelUnc_FSR[i_mr] );
		Hist_RelUnc_FSR->h->SetMarkerStyle(28);

		HistInfo* Hist_RelUnc_CollCS = new HistInfo( 7, "Coll.CS", this->h_RelUnc_CollCS[i_mr] );
		Hist_RelUnc_CollCS->h->SetMarkerStyle(34);

		HistInfo* Hist_RelUnc_Lumi = new HistInfo( TColor::GetColor("#ff9933"), "Luminosity", this->h_RelUnc_Lumi[i_mr]);
		Hist_RelUnc_Lumi->h->SetMarkerStyle(22);

		TCanvas *c;
		TString CanvasName = "Local/Summary_2016Unc_Electron_2D_"+TStr_MassRange[i_mr];
		SetCanvas_Square( c, CanvasName, 0, 1, 800, 700 );

		c->cd();

		Hist_RelUnc_Stat->Draw("HISTLPSAME");
		Hist_RelUnc_Lumi->Draw("HISTLPSAME");
		Hist_RelUnc_CollCS->Draw("HISTLPSAME");
		Hist_RelUnc_FSR->Draw("HISTLPSAME");
		Hist_RelUnc_Bkg->Draw("HISTLPSAME");
		Hist_RelUnc_DetRes->Draw("HISTLPSAME");
		Hist_RelUnc_EffSF->Draw("HISTLPSAME");
		Hist_RelUnc_Syst->Draw("HISTLPSAME");

		TLegend *legend;
		SetLegend( legend, 0.15, 0.15, 0.965, 0.35 );
		legend->SetNColumns(4);

		Hist_RelUnc_Stat->AddToLegend( legend );
		Hist_RelUnc_Lumi->AddToLegend( legend );
		Hist_RelUnc_Syst->AddToLegend( legend );
		Hist_RelUnc_EffSF->AddToLegend( legend );
		Hist_RelUnc_DetRes->AddToLegend( legend );
		Hist_RelUnc_Bkg->AddToLegend( legend );
		Hist_RelUnc_FSR->AddToLegend( legend );
		Hist_RelUnc_CollCS->AddToLegend( legend );
		legend->Draw();

		SetHistFormat_SinglePad( Hist_RelUnc_Stat->h, "|y(ee)|", "Rel. Unc. (%)" );
		Hist_RelUnc_Stat->h->GetYaxis()->SetRangeUser(2e-2, 30);
		if( i_mr == 3 ) Hist_RelUnc_Stat->h->GetYaxis()->SetRangeUser(1e-4, 10);

		TLatex latex;
		Latex_Preliminary(latex, 35.9, 13);
		TString Info_MassRange = "";
		if( TStr_MassRange[i_mr] == "M20to30" ) Info_MassRange = "20 < M < 30 GeV";
		if( TStr_MassRange[i_mr] == "M30to45" ) Info_MassRange = "30 < M < 45 GeV";
		if( TStr_MassRange[i_mr] == "M45to60" ) Info_MassRange = "45 < M < 60 GeV";
		if( TStr_MassRange[i_mr] == "M60to120" ) Info_MassRange = "60 < M < 120 GeV";
		if( TStr_MassRange[i_mr] == "M120to200" ) Info_MassRange = "120 < M < 200 GeV";
		if( TStr_MassRange[i_mr] == "M200to1500" ) Info_MassRange = "200 < M < 1500 GeV";
		latex.DrawLatexNDC( 0.16, 0.91, "#scale[0.8]{#font[42]{"+Info_MassRange+"}}");

		c->SetLogy();
		c->SaveAs(".pdf");
	}

	void SetUncHist_Stat()
	{
		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		TString FileName = AnalyzerPath+"/FeasibilityStudy_2016/StatUnc/ROOTFile_RelStatUnc_2016_Electron.root";
		for(Int_t i=0; i<nMassRange; i++)
		{
			TString HistName = TString::Format("h_RelStatUnc_absdiRap_%s_DY", TStr_MassRange[i].Data());
			TH1D* h = Get_Hist( FileName, HistName );
			this->h_RelUnc_Stat[i] = (TH1D*)h->Clone();
			h_RelUnc_Stat[i]->Scale(100); // -- convert to % -- //
		}
	}

	void SetUncHist_Lumi()
	{
		Double_t RelUnc_Lumi = 2.5;

		for(Int_t i_mr=0; i_mr<nMassRange; i_mr++)
		{
			this->h_RelUnc_Lumi[i_mr] = (TH1D*)h_RelUnc_Stat[i_mr]->Clone();
			Int_t nBin = this->h_RelUnc_Lumi[i_mr]->GetNbinsX();
			for(Int_t i=0; i<nBin; i++)
			{
				Int_t i_bin = i+1;
				this->h_RelUnc_Lumi[i_mr]->SetBinContent(i_bin, RelUnc_Lumi);
				this->h_RelUnc_Lumi[i_mr]->SetBinError(i_bin, 0);
			}
		}
	}

	void SetUncHist_SystSource_M20to30()
	{
		Int_t i_mr = 0; // -- first bin -- //

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			6.23, 6.02, 5.72, 5.65, 5.08, 4.99, 4.64, 4.60, 4.41, 4.26,
			4.05, 4.32, 4.52, 4.70, 4.85, 4.97, 4.68, 4.85, 4.83, 5.24,
			5.76, 6.36, 7.87, 14.32
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			1.21, 1.57, 1.55, 1.52, 1.49, 1.55, 1.68, 1.71, 1.47, 1.83, 
			1.75, 1.94, 1.94, 2.19, 2.50, 2.61, 2.57, 2.66, 2.81, 2.79,
			3.83, 4.03, 4.99, 9.41
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			2.74, 3.20, 3.38, 3.35, 2.91, 2.89, 3.45, 3.22, 2.98, 3.12,
			3.24, 2.64, 2.75, 2.38, 2.25, 2.10, 2.31, 2.75, 2.03, 1.61,
			1.54, 1.41, 3.76, 3.07
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.72, 0.75, 0.78, 0.75, 0.74, 0.74, 0.75, 0.79, 0.83, 0.76,
			0.75, 0.70, 0.80, 0.71, 0.75, 0.77, 0.77, 0.72, 0.84, 0.77,
			0.80, 0.92, 1.07, 1.68
		};

		// -- pileup effect -- //
		vector<Double_t> vec_RelUnc8TeV_CollCS =
		{
			2.03, 1.72, 2.11, 1.52, 2.22, 1.06, 2.06, 2.37, 1.55, 1.41,
			1.63, 1.64, 1.19, 1.63, 2.01, 1.77, 1.84, 2.21, 2.30, 2.13,
			2.76, 2.42, 3.25, 1.17
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR, vec_RelUnc8TeV_CollCS );
	}

	void SetUncHist_SystSource_M30to45()
	{
		Int_t i_mr = 1;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			5.97, 5.91, 5.71, 5.34, 5.03, 4.63, 4.19, 3.90, 3.69, 3.75, 
			3.86, 4.01, 4.04, 4.09, 4.18, 4.16, 4.19, 4.17, 4.03, 3.97,
			4.14, 4.66, 5.52, 8.98
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.57, 0.64, 0.62, 0.63, 0.64, 0.65, 0.63, 0.70, 0.68, 0.76,
			0.76, 0.86, 0.91, 1.02, 1.09, 1.08, 1.13, 1.19, 1.19, 1.23,
			1.32, 1.46, 1.77, 2.83
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			1.20, 1.11, 1.04, 1.08, 1.18, 1.11, 1.07, 1.16, 1.22, 1.21, 
			1.12, 1.26, 1.23, 1.21, 1.24, 1.19, 1.25, 1.15, 1.23, 1.21,
			1.16, 1.30, 1.59, 2.69
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.55, 0.53, 0.55, 0.54, 0.55, 0.54, 0.51, 0.52, 0.52, 0.52,
			0.52, 0.54, 0.51, 0.50, 0.57, 0.55, 0.53, 0.54, 0.49, 0.57,
			0.53, 0.51, 0.74, 1.02
		};

		// -- pileup effect -- //
		vector<Double_t> vec_RelUnc8TeV_CollCS =
		{
			2.22, 2.34, 2.14, 2.12, 2.00, 2.18, 2.21, 1.84, 1.95, 1.96,
			1.94, 1.64, 1.72, 1.92, 1.75, 2.31, 2.04, 2.04, 2.20, 2.61,
			3.06, 2.69, 4.30, 5.22
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR, vec_RelUnc8TeV_CollCS );
	}

	void SetUncHist_SystSource_M45to60()
	{
		Int_t i_mr = 2;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			3.54, 3.42, 3.30, 3.16, 2.94, 2.87, 2.76, 2.74, 2.80, 2.79,
			2.85, 2.93, 2.93, 2.93, 2.97, 3.09, 3.30, 3.41, 3.47, 3.57,
			4.25, 4.87, 6.11, 8.67

		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.89, 0.95, 0.90, 1.01, 0.98, 1.04, 1.00, 1.04, 1.13, 1.17,
			1.12, 1.21, 1.24, 1.30, 1.32, 1.30, 1.43, 1.58, 1.67, 1.73,
			1.91, 2.39, 3.09, 4.58
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			1.54, 1.54, 1.48, 1.59, 1.57, 1.62, 1.66, 1.64, 1.76, 1.63,
			1.62, 1.57, 1.65, 1.61, 1.34, 1.31, 1.39, 1.38, 1.38, 1.76,
			1.59, 1.94, 2.00, 3.01
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.64, 0.64, 0.65, 0.70, 0.68, 0.69, 0.66, 0.69, 0.70, 0.68,
			0.68, 0.68, 0.65, 0.65, 0.69, 0.73, 0.72, 0.76, 0.81, 0.88,
			1.01, 1.25, 1.53, 2.01
		};

		// -- pileup effect -- //
		vector<Double_t> vec_RelUnc8TeV_CollCS =
		{
			1.47, 1.51, 1.55, 1.41, 1.66, 1.54, 1.28, 1.51, 1.16, 1.67,
			1.28, 1.45, 1.43, 1.40, 1.84, 1.29, 1.66, 2.06, 1.72, 1.84,
			2.42, 2.04, 2.17, 3.04
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR, vec_RelUnc8TeV_CollCS );
	}

	void SetUncHist_SystSource_M60to120()
	{
		Int_t i_mr = 3;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			0.82, 0.78, 0.72, 0.67, 0.66, 0.66, 0.66, 0.66, 0.66, 0.67, 
			0.65, 0.60, 0.56, 0.52, 0.51, 0.52, 0.54, 0.62, 0.63, 0.72,
			1.07, 1.23, 1.37, 1.63
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.06, 0.09, 0.09, 0.09, 0.09, 0.09, 0.09, 0.10, 0.10, 0.10, 
			0.10, 0.10, 0.11, 0.11, 0.11, 0.12, 0.12, 0.13, 0.13, 0.14,
			0.14, 0.15, 0.17, 0.26
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			0.09, 0.08, 0.09, 0.09, 0.09, 0.09, 0.08, 0.08, 0.08, 0.08,
			0.08, 0.08, 0.07, 0.07, 0.08, 0.09, 0.09, 0.10, 0.10, 0.12,
			0.12, 0.15, 0.20, 0.29

		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.29, 0.30, 0.30, 0.30, 0.30, 0.30, 0.31, 0.31, 0.32, 0.31,
			0.31, 0.31, 0.31, 0.31, 0.32, 0.33, 0.33, 0.32, 0.32, 0.32,
			0.35, 0.32, 0.36, 0.39 
		};

		// -- pileup effect -- //
		vector<Double_t> vec_RelUnc8TeV_CollCS =
		{
			0.44, 0.50, 0.50, 0.50, 0.51, 0.54, 0.55, 0.57, 0.53, 0.59,
			0.58, 0.60, 0.59, 0.55, 0.54, 0.54, 0.60, 0.55, 0.56, 0.56,
			0.70, 0.79, 0.84, 0.99
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR, vec_RelUnc8TeV_CollCS );
	}

	void SetUncHist_SystSource_M120to200()
	{
		Int_t i_mr = 4;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			1.26, 1.26, 1.20, 1.20, 1.20, 1.21, 1.19, 1.24, 1.21, 1.20, 
			1.25, 1.26, 1.32, 1.49, 1.56, 1.67, 1.83, 2.03, 2.31, 2.61,
			3.12, 3.74, 5.02, 10.39
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.78, 0.92, 0.92, 0.93, 0.89, 0.93, 0.97, 0.96, 0.95, 0.97,
			1.05, 1.01, 1.08, 1.11, 1.23, 1.34, 1.47, 1.56, 1.62, 1.73,
			2.20, 2.75, 4.95
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			1.66, 1.62, 1.58, 1.57, 1.49, 1.49, 1.49, 1.51, 1.46, 1.28,
			1.25, 1.27, 1.11, 1.08, 0.95, 1.10, 1.23, 1.11, 1.34, 1.44,
			1.76, 1.91, 4.02
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.64, 0.76, 0.70, 0.78, 0.77, 0.79, 0.79, 0.78, 0.75, 0.78,
			0.80, 0.81, 0.79, 0.80, 0.82, 0.87, 0.90, 0.95, 1.00, 1.08,
			1.28, 1.40, 1.73, 3.12
		};

		// -- pileup effect -- //
		vector<Double_t> vec_RelUnc8TeV_CollCS =
		{
			0.14, 0.20, 0.37, 0.33, 0.11, 0.07, 0.24, 0.23, 0.23, 0.07,
			0.33, 0.27, 0.07, 0.44, 0.37, 0.56, 0.17, 0.33, 0.18, 0.76,
			0.47, 0.77, 0.73, 1.38
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR, vec_RelUnc8TeV_CollCS );
	}

	void SetUncHist_SystSource_M200to1500()
	{
		Int_t i_mr = 5;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			1.11, 1.10, 1.12, 1.08, 1.12, 1.15, 1.27, 1.42, 1.76, 2.36,
			3.42, 5.95
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.62, 0.73, 0.65, 0.73, 0.74, 0.79, 0.89, 1.02, 1.13, 1.51,
			2.11, 4.15
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			4.49, 4.57, 4.31, 3.42, 3.34, 3.14, 2.67, 2.31, 2.47, 2.92,
			3.74, 7.64
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.57, 0.62, 0.60, 0.63, 0.64, 0.70, 0.67, 0.72, 0.83, 1.00,
			1.26, 2.37
		};

		// -- pileup effect -- //
		vector<Double_t> vec_RelUnc8TeV_CollCS =
		{
			0.22, 0.04, 0.16, 0.09, 0.06, 0.20, 0.17, 0.16, 0.36, 0.20,
			0.67, 1.76
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR, vec_RelUnc8TeV_CollCS );
	}

	void SetUncHist_MassRange( Int_t i_mr, vector<Double_t> vec_RelUnc8TeV_EffSF, vector<Double_t> vec_RelUnc8TeV_DetRes, vector<Double_t> vec_RelUnc8TeV_Bkg, vector<Double_t> vec_RelUnc8TeV_FSR, vector<Double_t> vec_RelUnc8TeV_CollCS )
	{
		vector<Double_t> vec_RelUnc8TeV_Syst = this->QuadSum_vector( vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR, vec_RelUnc8TeV_CollCS );

		this->h_RelUnc_EffSF[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_EffSF );
		this->h_RelUnc_DetRes[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_DetRes );
		this->h_RelUnc_Bkg[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_Bkg );
		this->h_RelUnc_FSR[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_FSR );
		this->h_RelUnc_CollCS[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_CollCS );
		this->h_RelUnc_Syst[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_Syst );

		this->h_RelUnc_DetRes[i_mr]->Scale( this->LumiScale_Stat );
		this->h_RelUnc_Bkg[i_mr]->Scale( this->LumiScale_Stat );
	}

	vector<Double_t> QuadSum_vector( vector<Double_t> vec1, vector<Double_t> vec2, vector<Double_t> vec3, vector<Double_t> vec4, vector<Double_t> vec5 )
	{
		vector<Double_t> vec_QuadSum;
		Int_t nEntry = (Int_t)vec1.size();
		for(Int_t i=0; i<nEntry; i++)
		{
			Double_t unc1 = vec1[i];
			Double_t unc2 = vec2[i];
			Double_t unc3 = vec3[i];
			Double_t unc4 = vec4[i];
			Double_t unc5 = vec5[i];

			Double_t QuadSum = sqrt( unc1*unc1 + unc2*unc2 + unc3*unc3 + unc4*unc4 + unc5*unc5 );

			vec_QuadSum.push_back( QuadSum );
			printf("QuadSum: %.2lf\n", QuadSum );
		}

		return vec_QuadSum;
	}

	TH1D* MakeHist_FromArray( vector<Double_t> vec_RelUnc )
	{
		Int_t nRapBin = vec_RelUnc.size();
		TH1D* h = new TH1D("h", "", nRapBin, 0, 2.4 );
		for(Int_t i=0; i<nRapBin; i++)
		{
			Int_t i_bin = i+1;
			h->SetBinContent(i_bin, vec_RelUnc[i]);
			h->SetBinError(i_bin, 0);
		}

		return h;
	}
};

void Estimate2016_2D_Electron()
{
	UncTool2016_2D* tool = new UncTool2016_2D();
	tool->Set_All();
	tool->DrawPlots();
}