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
		this->Lumi_2016 = 35863.818+41526.280;
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

		// this->SetUncHist_DetRes();
		// this->SetUncHist_Bkg();

		// this->SetUncHist_Acc_Using8TeV();
		// this->SetUncHist_EffSF();
		// this->SetUncHist_FSR();

		// this->SetUncHist_Syst();
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

		HistInfo* Hist_RelUnc_Lumi = new HistInfo( TColor::GetColor("#ff9933"), "Luminosity", this->h_RelUnc_Lumi[i_mr]);
		Hist_RelUnc_Lumi->h->SetMarkerStyle(22);

		TCanvas *c;
		TString CanvasName = "Local/Summary_2016Unc_2D_"+TStr_MassRange[i_mr];
		SetCanvas_Square( c, CanvasName, 0, 1, 800, 700 );

		c->cd();

		Hist_RelUnc_Stat->Draw("HISTLPSAME");
		Hist_RelUnc_Lumi->Draw("HISTLPSAME");
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
		legend->Draw();

		SetHistFormat_SinglePad( Hist_RelUnc_Stat->h, "|y(#mu#mu)|", "Rel. Unc. (%)" );
		Hist_RelUnc_Stat->h->GetYaxis()->SetRangeUser(2e-2, 30);
		if( i_mr == 3 ) Hist_RelUnc_Stat->h->GetYaxis()->SetRangeUser(1e-4, 10);

		TLatex latex;
		Latex_Preliminary(latex, 77.4, 13);
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
		TString FileName = AnalyzerPath+"/FeasibilityStudy/Data2016and17/StatUnc/ROOTFile_RelStatUnc_2016and17_Muon.root";
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
			1.62, 1.65, 1.67, 1.68, 1.69, 1.67, 1.64, 1.60, 1.53, 1.46, 
			1.39, 1.33, 1.30, 1.29, 1.30, 1.33, 1.35, 1.37, 1.40, 1.44, 
			1.50, 1.60, 1.72, 1.87
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			1.45, 1.85, 1.87, 1.81, 1.78, 1.76, 2.22, 2.33, 1.57, 2.16, 
			2.09, 2.10, 1.83, 2.24, 2.20, 3.13, 2.76, 2.56, 2.35, 2.67, 
			5.61, 5.01, 6.25, 3.94 
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			2.26, 2.48, 2.39, 2.36, 2.36, 2.44, 2.34, 2.37, 2.50, 2.48, 
			2.40, 2.40, 2.35, 2.24, 2.28, 2.27, 2.16, 2.12, 2.11, 2.27, 
			2.21, 2.29, 2.61, 3.87
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.46, 0.45, 0.43, 0.41, 0.39, 0.37, 0.35, 0.33, 0.31, 0.30,
			0.29, 0.27, 0.25, 0.23, 0.22, 0.22, 0.24, 0.29, 0.37, 0.48,
			0.59, 0.71, 0.83, 0.95
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR );
	}

	void SetUncHist_SystSource_M30to45()
	{
		Int_t i_mr = 1;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			1.11, 1.12, 1.17, 1.22, 1.26, 1.28, 1.29, 1.27, 1.24, 1.18, 
			1.11, 1.06, 1.02, 1.01, 1.02, 1.05, 1.07, 1.08, 1.08, 1.08, 
			1.09, 1.13, 1.18, 1.25
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.43, 0.51, 0.50, 0.52, 0.52, 0.53, 0.51, 0.54, 0.52, 0.58, 
			0.57, 0.62, 0.64, 0.69, 0.68, 0.70, 0.73, 0.73, 0.70, 0.74, 
			0.79, 0.86, 1.05, 1.91

		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			1.44, 1.43, 1.44, 1.43, 1.43, 1.45, 1.45, 1.46, 1.49, 1.50, 
			1.51, 1.52, 1.53, 1.53, 1.58, 1.62, 1.63, 1.66, 1.68, 1.71,
			1.70, 1.74, 1.85, 0.93
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.49, 0.52, 0.53, 0.53, 0.51, 0.47, 0.43, 0.38, 0.34, 0.31, 
			0.28, 0.26, 0.25, 0.26, 0.29, 0.34, 0.42, 0.53, 0.66, 0.80,
			0.93, 1.05, 1.16, 1.26
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR );
	}

	void SetUncHist_SystSource_M45to60()
	{
		Int_t i_mr = 2;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			1.13, 1.00, 0.89, 0.79, 0.71, 0.65, 0.62, 0.61, 0.62, 0.64, 
			0.67, 0.69, 0.72, 0.74, 0.75, 0.76, 0.76, 0.75, 0.74, 0.72, 
			0.70, 0.68, 0.67, 0.67
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.81, 0.93, 0.91, 1.00, 0.96, 0.97, 0.96, 1.01, 1.04, 1.05, 
			1.03, 1.04, 1.13, 1.14, 1.13, 1.19, 1.19, 1.29, 1.37, 1.34, 
			1.55, 1.88, 2.32, 3.74
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			1.34, 1.31, 1.29, 1.31, 1.30, 1.32, 1.32, 1.33, 1.32, 1.32, 
			1.28, 1.24, 1.19, 1.09, 1.01, 0.93, 0.86, 0.80, 0.75, 0.79, 
			0.80, 0.87, 1.01, 1.40

		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.48, 0.73, 0.93, 1.04, 1.04, 0.96, 0.83, 0.70, 0.60, 0.54, 
			0.52, 0.53, 0.55, 0.60, 0.71, 0.91, 1.21, 1.63, 2.12, 2.63, 
			3.12, 3.55, 3.90, 4.21
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR );
	}

	void SetUncHist_SystSource_M60to120()
	{
		Int_t i_mr = 3;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			0.72, 0.68, 0.63, 0.58, 0.53, 0.49, 0.44, 0.40, 0.38, 0.36, 
			0.36, 0.38, 0.40, 0.44, 0.48, 0.53, 0.58, 0.64, 0.68, 0.73, 
			0.77, 0.80, 0.84, 0.87
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.06, 0.09, 0.09, 0.09, 0.10, 0.10, 0.09, 0.10, 0.10, 0.10, 
			0.10, 0.10, 0.10, 0.11, 0.11, 0.12, 0.12, 0.11, 0.11, 0.12, 
			0.12, 0.12, 0.14, 0.22
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 
			0.04, 0.04, 0.04, 0.04, 0.04, 0.05, 0.05, 0.06, 0.06, 0.07, 
			0.08, 0.09, 0.11, 0.06
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.00, 0.01, 0.02, 0.02, 0.02, 0.02, 0.01, 0.02, 0.02, 0.02,
			0.03, 0.05, 0.06, 0.08, 0.10, 0.13, 0.16, 0.20, 0.23, 0.27,
			0.31, 0.35, 0.39, 0.43
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR );
	}

	void SetUncHist_SystSource_M120to200()
	{
		Int_t i_mr = 4;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			0.43, 0.41, 0.38, 0.38, 0.40, 0.42, 0.44, 0.47, 0.50, 0.51, 
			0.52, 0.52, 0.52, 0.54, 0.58, 0.65, 0.75, 0.85, 0.92, 0.98, 
			1.04, 1.32, 1.72, 1.85
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.86, 1.06, 1.06, 1.02, 1.05, 1.05, 1.07, 1.12, 1.13, 1.11, 
			1.20, 1.20, 1.17, 1.22, 1.35, 1.27, 1.44, 1.55, 1.54, 1.61, 
			1.77, 2.25, 2.57, 4.69
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			1.25, 1.40, 1.47, 1.52, 1.49, 1.47, 1.39, 1.33, 1.29, 1.23, 
			1.17, 1.13, 1.08, 1.06, 0.98, 0.96, 0.92, 0.88, 0.86, 0.84, 
			0.84, 0.94, 1.14, 1.03
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.30, 0.36, 0.40, 0.41, 0.38, 0.33, 0.27, 0.21, 0.17, 0.15, 
			0.14, 0.13, 0.13, 0.14, 0.15, 0.17, 0.21, 0.25, 0.29, 0.33, 
			0.36, 0.39, 0.40, 0.40
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR );
	}

	void SetUncHist_SystSource_M200to1500()
	{
		Int_t i_mr = 5;

		vector<Double_t> vec_RelUnc8TeV_EffSF = 
		{
			1.70, 1.66, 1.65, 1.64, 1.67, 1.79, 1.93, 2.19, 2.69, 3.38, 
			4.38, 7.56
		};

		vector<Double_t> vec_RelUnc8TeV_DetRes = 
		{
			0.66, 0.80, 0.72, 0.78, 0.82, 0.85, 0.96, 1.10, 1.19, 1.62,
			2.19, 5.25
		};

		vector<Double_t> vec_RelUnc8TeV_Bkg =
		{
			4.82, 4.38, 3.88, 3.25, 2.72, 2.39, 2.06, 1.73, 1.48, 1.54,
			1.77, 1.22
		};

		vector<Double_t> vec_RelUnc8TeV_FSR =
		{
			0.21, 0.18, 0.17, 0.17, 0.19, 0.23, 0.29, 0.36, 0.43, 0.50,
			0.57, 0.64
		};

		this->SetUncHist_MassRange( i_mr, vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR );
	}

	void SetUncHist_MassRange( Int_t i_mr, vector<Double_t> vec_RelUnc8TeV_EffSF, vector<Double_t> vec_RelUnc8TeV_DetRes, vector<Double_t> vec_RelUnc8TeV_Bkg, vector<Double_t> vec_RelUnc8TeV_FSR )
	{
		vector<Double_t> vec_RelUnc8TeV_Syst = this->QuadSum_vector( vec_RelUnc8TeV_EffSF, vec_RelUnc8TeV_DetRes, vec_RelUnc8TeV_Bkg, vec_RelUnc8TeV_FSR );

		this->h_RelUnc_EffSF[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_EffSF );
		this->h_RelUnc_DetRes[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_DetRes );
		this->h_RelUnc_Bkg[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_Bkg );
		this->h_RelUnc_FSR[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_FSR );
		this->h_RelUnc_Syst[i_mr] = this->MakeHist_FromArray( vec_RelUnc8TeV_Syst );

		this->h_RelUnc_DetRes[i_mr]->Scale( this->LumiScale_Stat );
		this->h_RelUnc_Bkg[i_mr]->Scale( this->LumiScale_Stat );
	}

	vector<Double_t> QuadSum_vector( vector<Double_t> vec1, vector<Double_t> vec2, vector<Double_t> vec3, vector<Double_t> vec4 )
	{
		vector<Double_t> vec_QuadSum;
		Int_t nEntry = (Int_t)vec1.size();
		for(Int_t i=0; i<nEntry; i++)
		{
			Double_t unc1 = vec1[i];
			Double_t unc2 = vec2[i];
			Double_t unc3 = vec3[i];
			Double_t unc4 = vec4[i];

			Double_t QuadSum = sqrt( unc1*unc1 + unc2*unc2 + unc3*unc3 + unc4*unc4 );

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

void Estimate2016_2D()
{
	UncTool2016_2D* tool = new UncTool2016_2D();
	tool->Set_All();
	tool->DrawPlots();
}