#include <Include/PlotTools.h>
#include <Include/DYAnalyzer.h>
#include <Include/MyCanvas.C>
#include <TLatex.h>

class SysUncTool_Combine
{
public:
	TString FileLocation;
	Double_t MassBinEdges[nMassBin+1];

	vector< TString > Sources;
	vector< TString > FileNames;
	vector< TString > HistNames;
	vector< TH1D* > Histos_Unc; // -- Syst. only -- //
	vector< Int_t > Colors;
	vector< Int_t > Markers;
	vector< TString > LegendName;

	TH1D *h_RelSysUnc_Tot;

	TH1D *h_RelStatUnc;
	TH1D *h_RelLumiUnc;
	TH1D *h_RelTotUnc;

	TCanvas *c_RelUnc;

	TFile *f_output;

	// -- data differential cross section -- //
	TH1D* h_data;
	TH1D* h_data_StatUnc;

	// -- MC cross section -- //
	TH1D* h_FEWZ;
	TH1D* h_aMCNLO;

	TString CanvasName;
	TString LegendName_Theory;

	SysUncTool_Combine()
	{
		FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");

		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		for(Int_t i=0; i<nMassBin+1; i++)
			MassBinEdges[i] = MassBinEdges_temp[i];

		h_RelSysUnc_Tot = new TH1D("h_RelSysUnc_Tot_Percent", "", nMassBin, MassBinEdges);
		h_RelTotUnc = new TH1D("h_RelTotUnc_Percent", "", nMassBin, MassBinEdges);

		TH1::AddDirectory(kFALSE);
	}

	void SetupSources()
	{
		Sources.push_back( "Acc."); FileNames.push_back("ROOTFile_SysUnc_Acceptance.root"); HistNames.push_back("h_RelSysUnc_NNLO_Percent"); Colors.push_back( kGreen-9 ); Markers.push_back( 24 );
		Sources.push_back( "Bkg. Est."); FileNames.push_back("ROOTFile_SysUnc_BkgEst.root"); HistNames.push_back("h_SysUnc_BkgEst_Percent"); Colors.push_back( kGreen+1 ); Markers.push_back( 27 );
		Sources.push_back( "Eff. SF."); FileNames.push_back("ROOTFile_SysUnc_EffSF.root"); HistNames.push_back("h_RelSysUnc_Tot"); Colors.push_back( kBlue ); Markers.push_back( 25 );
		Sources.push_back( "Det. Res."); FileNames.push_back("ROOTFile_SysUnc_DetRes.root"); HistNames.push_back("h_RelSysUnc_Tot"); Colors.push_back( kViolet ); Markers.push_back( 26 );
		Sources.push_back( "FSR"); FileNames.push_back("ROOTFile_SysUnc_FSR.root"); HistNames.push_back("h_RelSysUnc_Tot"); Colors.push_back( kBlue-9 );  Markers.push_back( 28 );

		LegendName.push_back( "Acceptance" );
		LegendName.push_back( "Background" );
		LegendName.push_back( "Efficiency SF" );
		LegendName.push_back( "Detector Res." );
		LegendName.push_back( "FSR" );

	}

	void PrintAll()
	{
		Int_t nSource = (Int_t)Sources.size();
		printf("                     ");
		for(Int_t i_source=0; i_source<nSource; i_source++)
		{
			if( !Sources[i_source].Contains("Acc") )
				printf("%10s", Sources[i_source].Data() );
		}
		printf("%10s", "TotSys" );
		printf("%10s", "Acc.");
		printf("\n");

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t lowerEdge = h_RelTotUnc->GetBinLowEdge(i_bin);
			Double_t upperEdge = h_RelTotUnc->GetBinLowEdge(i_bin+1);

			printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

			Double_t Unc_Acc = 0;
			for(Int_t i_source=0; i_source<nSource; i_source++)
			{
				if( Sources[i_source].Contains("Acc") )
					Unc_Acc = Histos_Unc[i_source]->GetBinContent(i_bin);
				else
					printf("%10.2lf", Histos_Unc[i_source]->GetBinContent(i_bin) );

			}
			printf("%10.2lf", h_RelSysUnc_Tot->GetBinContent(i_bin) );
			printf("%10.2lf", Unc_Acc );
			printf("\n");
		}

	}

	void MakeHistogram_LumiUnc( Double_t RelLumiUnc = 0.023 )
	{
		h_RelLumiUnc = new TH1D("h_RelLumiUnc_Percent", "", nMassBin, MassBinEdges);
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			h_RelLumiUnc->SetBinContent(i_bin, RelLumiUnc);
			h_RelLumiUnc->SetBinError(i_bin, 0);
		}

		h_RelLumiUnc->Scale( 100 ); // -- convert to % -- //
	}

	void GetHistograms( TString Type = "All" )
	{
		TFile *f_StatUnc = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root" );
		TString HistName = "h_RelUnc_Stat";
		if( Type == "FpoF") 
			HistName = "h_FpoF_RelUnc_Stat";
		
		h_RelStatUnc = (TH1D*)f_StatUnc->Get( HistName )->Clone();
		h_RelStatUnc->Scale( 100 ); // -- convert to % -- // 

		delete f_StatUnc;

		Int_t nSource = (Int_t)Sources.size();
		for(Int_t i=0; i<nSource; i++)
		{
			TFile *f_input = TFile::Open( FileLocation + "/" + FileNames[i] );
			Histos_Unc.push_back( (TH1D*)f_input->Get( HistNames[i] )->Clone() );

			delete f_input;
		}
	}

	void MakeHistogram_TotalUnc()
	{
		Int_t nSource = (Int_t)Sources.size();

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t SysUnc = 0;
			Double_t TotUnc = 0;

			for(Int_t i_source=0; i_source<nSource; i_source++) // -- systematic uncertainty sources -- //
			{
				Double_t unc = Histos_Unc[i_source]->GetBinContent(i_bin);

				if( !Sources[i_source].Contains("Acc") ) // -- Acceptance is not included in total syst. -- //
					SysUnc += unc*unc;

				TotUnc += unc*unc; // -- but included in total uncertainty -- //
			}

			SysUnc = sqrt(SysUnc);
			h_RelSysUnc_Tot->SetBinContent(i_bin, SysUnc);
			h_RelSysUnc_Tot->SetBinError(i_bin, 0);

			// -- Add up stat. -- //
			Double_t StatUnc = h_RelStatUnc->GetBinContent(i_bin);
			TotUnc += StatUnc * StatUnc;

			// -- add up lumi. -- //
			Double_t LumiUnc = h_RelLumiUnc->GetBinContent(i_bin);
			TotUnc += LumiUnc * LumiUnc;

			TotUnc = sqrt(TotUnc);
			h_RelTotUnc->SetBinContent(i_bin, TotUnc);
			h_RelTotUnc->SetBinError(i_bin, 0);
		}
	}

	void MakeCanvas(Bool_t FpoF = kFALSE)
	{
		TString CanvasName = "c_RelUnc_Summary";
		if( FpoF )
			CanvasName.ReplaceAll("RelUnc_", "FpoF_RelUnc_");

		c_RelUnc = new TCanvas(CanvasName, "", 1000, 800);
		c_RelUnc->cd();

		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.05);
		gPad->SetTopMargin(0.08);
		gPad->SetBottomMargin(0.15);
		gPad->SetLogx();

		h_RelStatUnc->Draw("HISTLP");
		h_RelLumiUnc->Draw("HISTLPSAME");

		Int_t nSource = (Int_t)Sources.size();
		for(Int_t i=0; i<nSource; i++)
			Histos_Unc[i]->Draw("HISTLPSAME");
		
		h_RelSysUnc_Tot->Draw("HISTLPSAME");

		h_RelStatUnc->SetStats(kFALSE);
		h_RelStatUnc->GetXaxis()->SetLabelSize(0.05);
		h_RelStatUnc->GetXaxis()->SetTitleSize(0.06);
		h_RelStatUnc->GetXaxis()->SetTitleOffset(0.95);
		h_RelStatUnc->GetXaxis()->SetNoExponent();
		h_RelStatUnc->GetXaxis()->SetMoreLogLabels();
		h_RelStatUnc->GetYaxis()->SetLabelSize(0.05);
		h_RelStatUnc->GetYaxis()->SetTitleOffset(0.9);
		// h_RelStatUnc->GetYaxis()->SetRangeUser(0, 200);

		h_RelStatUnc->SetXTitle( "m [GeV]");
		h_RelStatUnc->SetYTitle( "Relative Uncertainty (%)");

		h_RelStatUnc->SetMarkerStyle(20);
		h_RelStatUnc->SetMarkerColor(kBlack);
		h_RelStatUnc->SetLineColor(kBlack);
		h_RelStatUnc->SetFillColorAlpha( kWhite, 0);

		h_RelLumiUnc->SetMarkerStyle(22);
		h_RelLumiUnc->SetMarkerColor(kOrange+1);
		h_RelLumiUnc->SetLineColor(kOrange+1);
		h_RelLumiUnc->SetFillColorAlpha(kWhite, 0);

		for(Int_t i=0; i<nSource; i++)
		{
			Histos_Unc[i]->SetStats(kFALSE);
			Histos_Unc[i]->SetMarkerStyle( Markers[i] );
			Histos_Unc[i]->SetMarkerColor( Colors[i] );
			Histos_Unc[i]->SetLineColor( Colors[i] );
			Histos_Unc[i]->SetFillColorAlpha( kWhite, 0);
		}

		h_RelSysUnc_Tot->SetStats(kFALSE);
		h_RelSysUnc_Tot->SetMarkerStyle(21);
		h_RelSysUnc_Tot->SetMarkerColor(kRed);
		h_RelSysUnc_Tot->SetLineColor(kRed);
		h_RelSysUnc_Tot->SetFillColorAlpha( kWhite, 0);

		TLegend *legend1 = new TLegend(0.20, 0.78, 0.45, 0.85);
		legend1->SetFillStyle(0);
		legend1->SetBorderSize(0);
		legend1->AddEntry( h_RelStatUnc, "Statistical");
		legend1->AddEntry( h_RelLumiUnc, "Luminosity");
		legend1->Draw();

		TLegend *legend2 = new TLegend(0.40, 0.65, 0.75, 0.85);
		legend2->SetFillStyle(0);
		legend2->SetBorderSize(0);
		legend2->AddEntry( h_RelSysUnc_Tot, "Total Systematic" );
		for(Int_t i=0; i<nSource; i++)
			legend2->AddEntry( Histos_Unc[i], LegendName[i] );

		legend2->Draw();

		TLatex latex;
		latex.SetTextSize(0.04);
		latex.DrawLatexNDC( 0.74, 0.93, "2.8 fb^{#font[122]{\55}1} (13 TeV)");

		latex.SetTextSize(0.045);
		latex.DrawLatexNDC( 0.15, 0.87, "CMS Preliminary" );
		
		c_RelUnc->SaveAs( CanvasName+".pdf" );

		h_RelStatUnc->GetXaxis()->SetRangeUser(15, 200);
		h_RelStatUnc->GetYaxis()->SetRangeUser(0, 7.5);
		c_RelUnc->SaveAs( CanvasName+"_M15to200.pdf");

		h_RelStatUnc->GetXaxis()->SetRangeUser(15, 3000);
		h_RelStatUnc->GetYaxis()->SetRangeUser(0, 20);
		// legend->SetX1NDC(0.15);
		// legend->SetX2NDC(0.55);
		c_RelUnc->SaveAs( CanvasName+"_YRange_0to20.pdf");

		gPad->SetLogy();
		h_RelStatUnc->GetYaxis()->SetRangeUser(0.05, 200);
		c_RelUnc->SaveAs( CanvasName+"_LogY.pdf");
		c_RelUnc->SaveAs( CanvasName+"_LogY.C");

		// h_RelStatUnc->GetXaxis()->SetRangeUser(15, 3000);
		// h_RelStatUnc->GetYaxis()->SetRangeUser(0, 200);
		// legend->SetX1NDC(0.55);
		// legend->SetX2NDC(0.95);
	}

	void GetDiffXsecHistogram(TString Type)
	{
		TFile *f_data = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root"); f_data->cd();

		// -- data histogram -- //
		if( Type.Contains("FpoF") )
			this->h_data = (TH1D*)f_data->Get("h_FpoF_DiffXsec_Data")->Clone();
		else
			this->h_data = (TH1D*)f_data->Get("h_DiffXsec_FSRCorr")->Clone();

		// -- set errors -- //
		this->SetErrors( h_data, h_RelTotUnc );

		// -- stat.only histogram -- //
		TString HistName = h_data->GetName();
		HistName = HistName + "_StatOnly";
		this->h_data_StatUnc = (TH1D*)h_data->Clone( HistName );
		this->SetErrors( h_data_StatUnc, h_RelStatUnc );

		// -- mc histogram -- //
		if( Type.Contains("FpoF") )
		{
			// -- FEWZ: not prepared yet -- //
			this->h_FEWZ = NULL; 

			 // -- aMC@NLO -- // 
			f_data->cd();
			this->h_aMCNLO = (TH1D*)f_data->Get("h_FpoF_DiffXsec_aMCNLO")->Clone();
		}
		else
		{
			// -- FEWZ -- //
			// -- no PI -- //
			TFile *f_theory = TFile::Open(FileLocation + "/ROOTFile_xSec_Theory.root"); 
			f_theory->cd();
			this->h_FEWZ = (TH1D*)f_theory->Get("h_DiffXsec_FEWZ_NNPDF_NNLO")->Clone();

			// -- with PI -- //
			// TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
			// TString FileName_Data = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYXSecNNLO_WithPI.root";
			// this->h_FEWZ = Get_Hist( FileName_Data, "h_DYDiffXSecNNLO_WithPI" );

			// -- aMC@NLO -- //
			f_data->cd();
			this->h_aMCNLO = (TH1D*)f_data->Get("h_DiffXsec_aMCNLO")->Clone();
		}

		TString Type_DXSec = "dressed";
		if( Type.Contains("FpoF") ) Type_DXSec = "withinAcc";
		this->AssignTheoryUnc_aMCNLO( this->h_aMCNLO, Type_DXSec );
		// this->AssignScaleUnc_3Percent( this->h_aMCNLO );
		// this->Add_PIContribution( this->h_aMCNLO );		
	}

	void AssignTheoryUnc_aMCNLO( TH1D* h_aMCNLO, TString Type_DXSec )
	{
		TString Tag = "Muon_"+Type_DXSec;
		TString FileName_TheoUnc = this->FileLocation+"/ROOTFile_TheoryUnc_aMCNLO.root";
		// -- quad.sum of scale, alphas, PDF replica uncertainty -- //
		TH1D* h_RelUnc_Tot = Get_Hist( FileName_TheoUnc, Tag+"/h_RelUnc_Tot");

		Int_t nBin = h_aMCNLO->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t DXSec = h_aMCNLO->GetBinContent(i_bin);
			Double_t RelUnc = h_RelUnc_Tot->GetBinContent(i_bin);
			Double_t AbsUnc = DXSec * RelUnc;
			h_aMCNLO->SetBinError(i_bin, AbsUnc);
		}
	}

	void DrawCanvas_DiffXsec_All( TString Type )
	{
		this->GetDiffXsecHistogram( Type );

		if( Type == "FpoF" )
		{
			this->SetStrings( "c_FpoF_DiffXsec_Data_vs_aMCNLO_AllUnc", "aMC@NLO" );
			this->DrawCanvas_DiffXsec( h_data, h_aMCNLO );
		}
		else
		{
			this->SetStrings( "c_DiffXsec_Data_vs_NNLO_AllUnc", "FEWZ (NNLO QCD + NLO EW)" );
			this->DrawCanvas_DiffXsec( h_data, h_FEWZ );

			this->SetStrings( "c_DiffXsec_Data_vs_aMCNLO_AllUnc", "aMC@NLO" );
			this->DrawCanvas_DiffXsec( h_data, h_aMCNLO );

			this->DrawCanvas_DiffXsec_aMCNLO_FEWZ();
		}
	}

	void SetStrings(TString _CanvasName, TString _LegendName)
	{
		this->CanvasName = _CanvasName;
		this->LegendName_Theory = _LegendName;
	}

	void SetBasicOptions_Canvas( TCanvas *c )
	{
		c->Range(0,0,1,1);
		c->SetFillColor(0);
		c->SetBorderMode(0);
		c->SetBorderSize(2);
		//c->SetGridx();
		//c->SetGridy();
		c->SetTickx(1);
		c->SetTicky(1);
		c->SetLeftMargin(0.16);
		c->SetRightMargin(0.02);
		c->SetTopMargin(0.08);
		c->SetBottomMargin(0.14);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);
	}

	void SetBasicOptions_Pad( TPad *pad )
	{
		pad->SetFrameFillStyle(0);
		pad->SetFrameBorderMode(0);
		pad->SetFrameFillStyle(0);
		pad->SetFrameBorderMode(0);
	}

	void DrawCanvas_DiffXsec( TH1D* h_data, TH1D* h_theory )
	{
		///////////////////////
		// -- Draw Canvas -- //
		///////////////////////
		TCanvas *c = new TCanvas(this->CanvasName, "", 0, 0, 800, 700);
		// c->SetTopMargin(0.08);
		// c->SetBottomMargin(0.14);
		this->SetBasicOptions_Canvas( c );
		c->cd();

		gStyle->SetOptFit(1);
		gStyle->SetOptStat(0);
		gStyle->SetOptTitle(0);

		// -- Top Pad -- //
		TPad *TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
		TopPad->Draw();
		TopPad->cd();

		gPad->SetLogx();
		gPad->SetLogy();

		// -- ensure additional space at the bottom side for ratio plot -- //
		TopPad->SetLeftMargin(0.13);
		TopPad->SetRightMargin(0.05);
		TopPad->SetTopMargin(0.05);
		TopPad->SetBottomMargin(0.32);
		this->SetBasicOptions_Pad( TopPad );


		h_data->Draw("E1P");
		h_theory->Draw("E1PSAME");
		h_data->Draw("E1PSAME");
		// h_data->Draw("AXISSAME");
		// h_data->Draw("E1PSAME");
		// h_data_StatUnc->Draw("E1PSAME");


		// -- General Setting for both plots -- //
		h_data->SetXTitle( "m [GeV]" );
		h_data->SetYTitle( "d#sigma/dm [pb/GeV]" );


		h_data->SetStats(kFALSE);
		h_theory->SetStats(kFALSE);
		h_data_StatUnc->SetStats(kFALSE);


		// -- Setting for h1 -- //
		h_data->SetLineColor(kBlack);
		h_data->SetLineWidth(1);
		h_data->SetMarkerColor(kBlack);
		h_data->SetMarkerSize(1);
		h_data->SetMarkerStyle(20);
		h_data->SetFillColorAlpha(kWhite, 0);


		// -- Setting for h2 -- //
		h_theory->SetLineColor(kRed);
		h_theory->SetLineWidth(1);
		h_theory->SetMarkerColor(kRed);
		h_theory->SetMarkerSize(1);
		h_theory->SetMarkerStyle(26);
		h_theory->SetFillColorAlpha(kWhite, 0);
		h_theory->SetFillStyle( 3144 );


		// -- Setting for h3 -- //
		h_data_StatUnc->SetLineColor(kBlack);
		h_data_StatUnc->SetLineWidth(1);
		h_data_StatUnc->SetMarkerColor(kBlack);
		h_data_StatUnc->SetMarkerSize(1);
		h_data_StatUnc->SetMarkerStyle(20);
		h_data_StatUnc->SetFillColorAlpha(kWhite, 0);


		// -- X-axis Setting -- //
		h_data->GetXaxis()->SetLabelSize(0);
		h_data->GetXaxis()->SetTitleSize(0);
		h_data->GetXaxis()->SetNoExponent(); 
		h_data->GetXaxis()->SetMoreLogLabels();
		h_data->GetXaxis()->SetRangeUser( 15, 3000 );
		h_data->SetMinimum(5e-8);
		h_data->SetMaximum(4000);

		// -- Y-axis Setting -- //
		h_data->GetYaxis()->SetTitleSize(0.06);
		h_data->GetYaxis()->SetTitleOffset(0.9);


		// -- Add Legend -- //
		TLegend *legend;
		if( this->LegendName_Theory.Contains("FEWZ") )
			legend = new TLegend(0.15, 0.35, 0.55, 0.55);
		else
			legend = new TLegend(0.15, 0.35, 0.35, 0.55);

		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->AddEntry(h_data, "Data");
		legend->AddEntry(h_theory, this->LegendName_Theory);
		legend->Draw();


		// -- write down some sentences -- //
		TLatex latex;
		latex.SetTextSize(0.03);
		latex.DrawLatexNDC(0.79, 0.96, "2.8 fb^{#font[122]{\55}1} (13 TeV)");
		latex.SetTextSize(0.035);
		latex.DrawLatexNDC(0.15, 0.91, "CMS Preliminary");
		latex.SetTextSize(0.045);
		latex.DrawLatexNDC(0.65, 0.85, "#gamma* /#font[122]{Z} #rightarrow #mu#mu");

		// TPaveText *pave = new TPaveText(1200,7900,2000,7950); // CHANGE
		// pave->SetFillColor(0);
		// TText* text = pave->AddText("2.8 fb^{#font[122]{\55}1} (13 TeV)");
		// text->SetTextSize(0.03);
		// pave->Draw("0");
		// TPaveText *ptitle = new TPaveText(25,600,70,2500); // CHANGE
		// ptitle->SetFillColor(0);
		// TText* ttitle = ptitle->AddText("CMS Preliminary");
		// ttitle->SetTextSize(0.035);
		// ptitle->Draw("0");
		// TPaveText *pch = new TPaveText(500,100,700,200); // CHANGE
		// pch->SetFillColor(0);
		// TText* tch = pch->AddText("#gamma* /#font[122]{Z} #rightarrow #mu#mu");
		// tch->SetTextSize(0.045);
		// pch->Draw("0");


		//////////////////////
		// -- Bottom Pad -- //
		//////////////////////
		TPad *bottomPad = new TPad("bottomPad","bottomPad",0.01,0.01,0.99,0.3);
		bottomPad->Draw();
		bottomPad->cd();

		bottomPad->SetLeftMargin(0.122);
		bottomPad->SetRightMargin(0.04);
		bottomPad->SetTopMargin(0.05);
		bottomPad->SetBottomMargin(0.4);
		this->SetBasicOptions_Pad( bottomPad );

		gPad->SetLogx();

		// -- Make Ratio plot1: Data / FEWZ & -- //
		TH1D* h_ratio = (TH1D*)h_data->Clone();
		h_ratio->SetName("h_ratio");
		this->MakeRatioHistogram( h_data, h_theory, h_ratio );

		// -- Make Ratio plot1: Data (OnlyStat.) / FEWZ & -- //
		TH1D* h_theory_NoError = (TH1D*)h_theory->Clone();
		Int_t nBin = h_theory_NoError->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			h_theory_NoError->SetBinError(i_bin, 0);
		}
		TH1D* h_ratio_StatUnc = (TH1D*)h_data_StatUnc->Clone();
		h_ratio_StatUnc->SetName("h_ratio");
		this->MakeRatioHistogram( h_data_StatUnc, h_theory_NoError, h_ratio_StatUnc );

		for(Int_t i_bin=1; i_bin<=nMassBin; i_bin++)
		{
			Double_t LowerEdge = h_ratio->GetBinLowEdge(i_bin);
			Double_t UpperEdge = h_ratio->GetBinLowEdge(i_bin+1);

			printf("[%.1lf, %.1lf] (ratio_error, ratio_StatOnly) = (%.3lf, %.3lf)\n", LowerEdge, UpperEdge, h_ratio->GetBinError(i_bin), h_ratio_StatUnc->GetBinError(i_bin) );
		}

		// -- flat line = 1.00 -- //
		TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		f_line->SetLineColor(kSpring-1);
		f_line->SetLineStyle(2);
		f_line->SetLineWidth(1);
		
		// -- draw -- //
		h_ratio->Draw("AXIS");
		f_line->Draw("SAME");
		h_ratio->Draw("E2PSAME");
		h_ratio_StatUnc->Draw("E2PSAME");

		TLegend *legend_ratio = new TLegend(0.15, 0.75, 0.55, 0.95);
		legend_ratio->SetFillStyle(0);
		legend_ratio->SetBorderSize(0);
		legend_ratio->SetTextFont(62);
		legend_ratio->AddEntry( h_ratio, "Total", "f");
		legend_ratio->AddEntry( h_ratio_StatUnc, "Stat.", "f");
		legend_ratio->Draw();

		// -- General Setting -- //
		h_ratio->SetLineColor(kBlack);
		h_ratio->SetMarkerStyle(20);
		h_ratio->SetMarkerSize(1);
		h_ratio->SetMarkerColor(kBlack);
		h_ratio->SetFillColorAlpha(kRed-9, 0.5);
		h_ratio->SetFillStyle(1001);
		h_ratio->SetStats(kFALSE);

		h_ratio_StatUnc->SetLineColor(kBlue);
		h_ratio_StatUnc->SetMarkerStyle(20);
		h_ratio_StatUnc->SetMarkerSize(1);
		h_ratio_StatUnc->SetMarkerColor(kBlack);
		h_ratio_StatUnc->SetFillColorAlpha(kBlue-9, 1);
		h_ratio_StatUnc->SetFillStyle(3144);

		h_ratio_StatUnc->SetStats(kFALSE);

		// -- X-axis Setting -- //
		TString XTitle = "m [GeV]";

		h_ratio->GetXaxis()->SetTitle( XTitle );
		h_ratio->GetXaxis()->SetTitleOffset( 0.9 );
		h_ratio->GetXaxis()->SetTitleSize( 0.2 );
		h_ratio->GetXaxis()->SetLabelColor(1);
		h_ratio->GetXaxis()->SetLabelFont(42);
		h_ratio->GetXaxis()->SetLabelOffset(0.007);
		h_ratio->GetXaxis()->SetLabelSize(0.15);
		h_ratio->GetXaxis()->SetMoreLogLabels(); 
		h_ratio->GetXaxis()->SetNoExponent();

		// -- Y-axis Setting -- //
		TString RatioName = "Data/Theory";
		if( LegendName_Theory == "aMC@NLO" )
			RatioName = "Data/aMC@NLO";
		else if( LegendName_Theory.Contains("FEWZ") )
			RatioName = "Data/FEWZ";
		
		h_ratio->GetYaxis()->SetTitle( RatioName );
		h_ratio->GetYaxis()->SetTitleOffset( 0.4 );
		h_ratio->GetYaxis()->SetTitleSize( 0.1);
		h_ratio->GetYaxis()->SetLabelSize( 0.07 );
		h_ratio->GetYaxis()->SetRangeUser( 0.25, 1.75 );

		// h_ratio->Draw("E2PSAME");
		// h_ratio_StatUnc->Draw("E2PSAME");

		c->SaveAs(CanvasName + ".pdf");
		c->SaveAs(CanvasName + ".C");


		TCanvas *c_ratio = new TCanvas("c_ratio", "", 800, 800); c_ratio->cd();
		TPad *RatioPad = (TPad*)bottomPad->Clone();
		RatioPad->SetPad(0.01,0.01,0.99,0.99);
		RatioPad->SetBottomMargin(0.12);
		RatioPad->SetRightMargin(0.03);
		RatioPad->SetLeftMargin(0.12);

		TH1D* h_ratio2 = (TH1D*)RatioPad->GetPrimitive("h_ratio");
		h_ratio2->GetXaxis()->SetTitleOffset(1.2);
		h_ratio2->GetXaxis()->SetTitleSize(0.05);
		h_ratio2->GetXaxis()->SetLabelSize(0.05);
		h_ratio2->GetYaxis()->SetRangeUser(0, 2);
		h_ratio2->GetYaxis()->SetTitleSize(0.05);
		h_ratio2->GetYaxis()->SetTitleOffset(1);
		h_ratio2->GetYaxis()->SetLabelSize(0.05);
		RatioPad->Draw();

		TString CanvasName_Ratio = CanvasName;
		CanvasName_Ratio.ReplaceAll("c_", "c_ratio");
		c_ratio->SaveAs(CanvasName_Ratio+".pdf");

		// c_ratio->SaveAs("c_ratio_"+Type+".pdf");
	}

	void DrawCanvas_DiffXsec_aMCNLO_FEWZ()
	{
		///////////////////////
		// -- Draw Canvas -- //
		///////////////////////
		TCanvas *c = new TCanvas("c_DiffXsec_Data_vs_Theory", "", 0, 0, 800, 700);
		// c->SetTopMargin(0.08);
		// c->SetBottomMargin(0.14);
		this->SetBasicOptions_Canvas( c );
		c->cd();

		gStyle->SetOptFit(1);
		gStyle->SetOptStat(0);
		gStyle->SetOptTitle(0);

		// -- Top Pad -- //
		TPad *TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
		TopPad->Draw();
		TopPad->cd();

		gPad->SetLogx();
		gPad->SetLogy();

		// -- ensure additional space at the bottom side for ratio plot -- //
		TopPad->SetLeftMargin(0.13);
		TopPad->SetRightMargin(0.05);
		TopPad->SetTopMargin(0.05);
		TopPad->SetBottomMargin(0.40);
		this->SetBasicOptions_Pad( TopPad );


		h_data->Draw("E1P");
		h_FEWZ->Draw("E1PSAME");
		h_aMCNLO->Draw("E1PSAME");
		h_data->Draw("E1PSAME");

		// -- General Setting for both plots -- //
		h_data->SetXTitle( "m [GeV]" );
		h_data->SetYTitle( "d#sigma/dm [pb/GeV]" );

		h_data->SetStats(kFALSE);
		h_FEWZ->SetStats(kFALSE);
		h_aMCNLO->SetStats(kFALSE);

		// -- Setting for h1 -- //
		h_data->SetLineColor(kBlack);
		h_data->SetLineWidth(1);
		h_data->SetMarkerColor(kBlack);
		h_data->SetMarkerSize(1);
		h_data->SetMarkerStyle(20);
		h_data->SetFillColorAlpha(kWhite, 0);

		// -- Setting for h2 -- //
		h_FEWZ->SetLineColor(kGreen);
		h_FEWZ->SetLineWidth(1);
		h_FEWZ->SetMarkerColor(kGreen);
		h_FEWZ->SetMarkerSize(1);
		h_FEWZ->SetMarkerStyle(32);
		h_FEWZ->SetFillColorAlpha(kWhite, 0);
		h_FEWZ->SetFillStyle( 3144 );

		// -- Setting for h3 -- //
		h_aMCNLO->SetLineColor(kRed);
		h_aMCNLO->SetLineWidth(1);
		h_aMCNLO->SetMarkerColor(kRed);
		h_aMCNLO->SetMarkerSize(1);
		h_aMCNLO->SetMarkerStyle(26);
		h_aMCNLO->SetFillColorAlpha(kWhite, 0);
		h_aMCNLO->SetFillStyle( 3144 );

		// -- X-axis Setting -- //
		h_data->GetXaxis()->SetLabelSize(0);
		h_data->GetXaxis()->SetTitleSize(0);
		h_data->GetXaxis()->SetNoExponent(); 
		h_data->GetXaxis()->SetMoreLogLabels();
		h_data->GetXaxis()->SetRangeUser( 15, 3000 );

		// -- Y-axis Setting -- //
		h_data->GetYaxis()->SetTitleSize(0.06);
		h_data->GetYaxis()->SetTitleOffset(0.9);
		h_data->SetMinimum(5e-8);
		h_data->SetMaximum(4000);

		// -- Add Legend -- //
		TLegend *legend = new TLegend(0.15, 0.43, 0.55, 0.63);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->AddEntry(h_data, "Data");
		legend->AddEntry(h_aMCNLO, "aMC@NLO");
		legend->AddEntry(h_FEWZ, "FEWZ (NNLO QCD + NLO EW)");
		legend->Draw();

		// -- write down some sentences -- //
		TLatex latex;
		latex.SetTextSize(0.03);
		latex.DrawLatexNDC(0.79, 0.96, "2.8 fb^{#font[122]{\55}1} (13 TeV)");
		latex.SetTextSize(0.035);
		latex.DrawLatexNDC(0.15, 0.91, "CMS Preliminary");
		latex.SetTextSize(0.045);
		latex.DrawLatexNDC(0.65, 0.85, "#gamma* /#font[122]{Z} #rightarrow #mu#mu");

		//////////////////////////////////
		// -- Bottom Pad1: Data/aMC@NLO -- //
		//////////////////////////////////
		TPad *bottomPad1 = new TPad("bottomPad1","bottomPad1",0.01,0.25,0.99,0.40);
		bottomPad1->Draw();
		bottomPad1->cd();

		bottomPad1->SetLeftMargin(0.122);
		bottomPad1->SetRightMargin(0.04);
		bottomPad1->SetTopMargin(0.05);
		bottomPad1->SetBottomMargin(0.01);
		this->SetBasicOptions_Pad( bottomPad1 );

		gPad->SetLogx();

		// -- Make Ratio plot1: Data / FEWZ & -- //
		TH1D* h_ratio1 = (TH1D*)h_data->Clone();
		h_ratio1->SetName("h_ratio1");
		this->MakeRatioHistogram( h_data, h_aMCNLO, h_ratio1 );

		// -- Make Ratio plot1: Data (OnlyStat.) / FEWZ -- //
		TH1D* h_aMCNLO_NoError = (TH1D*)h_aMCNLO->Clone();
		Int_t nBin1 = h_aMCNLO_NoError->GetNbinsX();
		for(Int_t i=0; i<nBin1; i++)
		{
			Int_t i_bin = i+1;
			h_aMCNLO_NoError->SetBinError(i_bin, 0);
		}
		TH1D* h_ratio1_StatUnc = (TH1D*)h_data_StatUnc->Clone();
		h_ratio1_StatUnc->SetName("h_ratio1");
		this->MakeRatioHistogram( h_data_StatUnc, h_aMCNLO_NoError, h_ratio1_StatUnc );

		// -- flat line = 1.00 -- //
		TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		f_line->SetLineColor(kSpring-1);
		f_line->SetLineStyle(2);
		f_line->SetLineWidth(1);
		
		// -- draw -- //
		h_ratio1->Draw("AXIS");
		f_line->Draw("SAME");
		h_ratio1->Draw("E2PSAME");
		h_ratio1_StatUnc->Draw("E2PSAME");

		TLegend *legend_ratio = new TLegend(0.15, 0.65, 0.55, 0.92);
		legend_ratio->SetFillStyle(0);
		legend_ratio->SetBorderSize(0);
		legend_ratio->SetTextFont(62);
		legend_ratio->AddEntry( h_ratio1, "Total", "f");
		legend_ratio->AddEntry( h_ratio1_StatUnc, "Stat.", "f");
		legend_ratio->Draw();

		// -- General Setting -- //
		h_ratio1->SetLineColor(kBlack);
		h_ratio1->SetMarkerStyle(20);
		h_ratio1->SetMarkerSize(1);
		h_ratio1->SetMarkerColor(kBlack);
		h_ratio1->SetFillColorAlpha(kRed-9, 0.5);
		h_ratio1->SetFillStyle(1001);
		h_ratio1->SetStats(kFALSE);

		h_ratio1_StatUnc->SetLineColor(kBlue);
		h_ratio1_StatUnc->SetMarkerStyle(20);
		h_ratio1_StatUnc->SetMarkerSize(1);
		h_ratio1_StatUnc->SetMarkerColor(kBlack);
		h_ratio1_StatUnc->SetFillColorAlpha(kBlue-9, 1);
		h_ratio1_StatUnc->SetFillStyle(3144);

		h_ratio1_StatUnc->SetStats(kFALSE);

		// -- Y-axis Setting -- //
		h_ratio1->GetYaxis()->SetTitle( "Data/aMC@NLO" );
		h_ratio1->GetYaxis()->SetTitleOffset( 0.30 );
		h_ratio1->GetYaxis()->SetTitleSize( 0.13 );
		h_ratio1->GetYaxis()->SetLabelSize( 0.10 );
		h_ratio1->GetYaxis()->SetRangeUser( 0.25, 1.75 );

		//////////////////////////////////
		// -- Bottom Pad2: Data/FEWZ -- //
		//////////////////////////////////
		TopPad->cd();
		TPad *bottomPad2 = new TPad("bottomPad2","bottomPad2",0.01,0.01,0.99,0.25);
		bottomPad2->Draw();
		bottomPad2->cd();

		bottomPad2->SetLeftMargin(0.122);
		bottomPad2->SetRightMargin(0.04);
		bottomPad2->SetTopMargin(0.05);
		bottomPad2->SetBottomMargin(0.4);
		this->SetBasicOptions_Pad( bottomPad2 );

		gPad->SetLogx();

		// -- Make Ratio plot1: Data / FEWZ & -- //
		TH1D* h_ratio2 = (TH1D*)h_data->Clone();
		h_ratio2->SetName("h_ratio2");
		this->MakeRatioHistogram( h_data, h_FEWZ, h_ratio2 );

		// -- Make Ratio plot1: Data (OnlyStat.) / FEWZ -- //
		TH1D* h_FEWZ_NoError = (TH1D*)h_FEWZ->Clone();
		Int_t nBin2 = h_FEWZ_NoError->GetNbinsX();
		for(Int_t i=0; i<nBin2; i++)
		{
			Int_t i_bin = i+1;
			h_FEWZ_NoError->SetBinError(i_bin, 0);
		}
		TH1D* h_ratio2_StatUnc = (TH1D*)h_data_StatUnc->Clone();
		h_ratio2_StatUnc->SetName("h_ratio2");
		this->MakeRatioHistogram( h_data_StatUnc, h_FEWZ_NoError, h_ratio2_StatUnc );

		// // -- flat line = 1.00 -- //
		// TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		// f_line->SetLineColor(kSpring-1);
		// f_line->SetLineStyle(2);
		// f_line->SetLineWidth(1);
		
		// -- draw -- //
		h_ratio2->Draw("AXIS");
		f_line->Draw("SAME");
		h_ratio2->Draw("E2PSAME");
		h_ratio2_StatUnc->Draw("E2PSAME");

		// -- General Setting -- //
		h_ratio2->SetLineColor(kBlack);
		h_ratio2->SetMarkerStyle(20);
		h_ratio2->SetMarkerSize(1);
		h_ratio2->SetMarkerColor(kBlack);
		h_ratio2->SetFillColorAlpha(kRed-9, 0.5);
		h_ratio2->SetFillStyle(1001);
		h_ratio2->SetStats(kFALSE);

		h_ratio2_StatUnc->SetLineColor(kBlue);
		h_ratio2_StatUnc->SetMarkerStyle(20);
		h_ratio2_StatUnc->SetMarkerSize(1);
		h_ratio2_StatUnc->SetMarkerColor(kBlack);
		h_ratio2_StatUnc->SetFillColorAlpha(kBlue-9, 1);
		h_ratio2_StatUnc->SetFillStyle(3144);

		h_ratio2_StatUnc->SetStats(kFALSE);

		// -- X-axis Setting -- //
		TString XTitle = "m [GeV]";

		h_ratio2->GetXaxis()->SetTitle( XTitle );
		h_ratio2->GetXaxis()->SetTitleOffset( 0.9 );
		h_ratio2->GetXaxis()->SetTitleSize( 0.2 );
		h_ratio2->GetXaxis()->SetLabelColor(1);
		h_ratio2->GetXaxis()->SetLabelFont(42);
		h_ratio2->GetXaxis()->SetLabelOffset(0.007);
		h_ratio2->GetXaxis()->SetLabelSize(0.15);
		h_ratio2->GetXaxis()->SetMoreLogLabels(); 
		h_ratio2->GetXaxis()->SetNoExponent();

		// -- Y-axis Setting -- //
		h_ratio2->GetYaxis()->SetTitle( "Data/FEWZ" );
		h_ratio2->GetYaxis()->SetTitleOffset( 0.4 );
		h_ratio2->GetYaxis()->SetTitleSize( 0.1);
		h_ratio2->GetYaxis()->SetLabelSize( 0.07 );
		h_ratio2->GetYaxis()->SetRangeUser( 0.25, 1.75 );

		c->SaveAs("c_DiffXsec_Data_vs_Theory.pdf");
		c->SaveAs("c_DiffXsec_Data_vs_Theory.C");
	}

	void DrawDiffXsec_WithAllUnc(TString Type)
	{
		// setTDRStyle();
		// tdrStyle->SetErrorX(0.5);
 
		// -- Get the data histogram -- //
		TFile *f_data = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root"); f_data->cd();
		if( Type.Contains("FpoF") )
			this->h_data = (TH1D*)f_data->Get("h_FpoF_DiffXsec_Data")->Clone();
		else
			this->h_data = (TH1D*)f_data->Get("h_DiffXsec_FSRCorr")->Clone();

		this->SetErrors( h_data, h_RelTotUnc );

		TString HistName = h_data->GetName();
		HistName = HistName + "_StatOnly";
		TH1D *h_data_StatUnc = (TH1D*)h_data->Clone( HistName );
		this->SetErrors( h_data_StatUnc, h_RelStatUnc );

		if( Type.Contains("Powheg") )
		{
			h_data = this->MakeHistogram_over50GeV( h_data );
			h_data_StatUnc = this->MakeHistogram_over50GeV( h_data_StatUnc );
		}

		// -- Get the theory histogram -- //
		TH1D* h_theory = NULL;
		TFile *f_theory = TFile::Open(FileLocation + "/ROOTFile_xSec_Theory.root"); f_theory->cd();

		TString CanvasName = "";
		TString LegendName_Theory = "";
		if( Type == "FEWZ" )
		{
			f_theory->cd();
			h_theory = (TH1D*)f_theory->Get("h_DiffXsec_FEWZ_NNPDF_NNLO")->Clone();
			CanvasName = "c_DiffXsec_Data_vs_NNLO_AllUnc";
			LegendName_Theory = "FEWZ (NNLO, NNPDF)";
		}
		else if( Type == "aMCNLO" )
		{
			h_theory = (TH1D*)f_data->Get("h_DiffXsec_aMCNLO")->Clone();
			CanvasName = "c_DiffXsec_Data_vs_aMCNLO_AllUnc";
			LegendName_Theory = "aMC@NLO";
		}
		else if( Type == "FpoF_aMCNLO" )
		{
			h_theory = (TH1D*)f_data->Get("h_FpoF_DiffXsec_aMCNLO")->Clone();
			CanvasName = "c_FpoF_DiffXsec_Data_vs_aMCNLO_AllUnc";
			LegendName_Theory = "aMC@NLO";
		}
		else if( Type == "FpoF_Powheg" )
		{
			TFile *f_powheg = TFile::Open("/home/kplee/DYAnalysis_76X/AccEff/Test_v20160712_1st_Powheg/ROOTFile_Histogram_Acc_Eff_Powheg_IsoMu20_OR_IsoTkMu20.root");
			h_theory = (TH1D*)f_powheg->Get("h_mass_AccPass")->Clone();
			h_theory->Scale( 1 / Lumi );
			this->Obtain_dSigma_dM( h_theory );
			CanvasName = "c_FpoF_DiffXsec_Data_vs_Powheg_AllUnc";
			LegendName_Theory = "Powheg";
		}

		///////////////////////
		// -- Draw Canvas -- //
		///////////////////////
		TCanvas *c = new TCanvas(CanvasName, "", 1000, 1000);
		c->cd();
		// -- Top Pad -- //
		TPad *TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
		TopPad->Draw();
		TopPad->cd();

		gPad->SetLogx();
		gPad->SetLogy();

		// -- ensure additional space at the bottom side for ratio plot -- //
		TopPad->SetBottomMargin(0.32);
		TopPad->SetRightMargin(0.05);

		h_data->Draw("E1P");
		h_theory->Draw("E1PSAME");
		h_data->Draw("AXISSAME");
		h_data->Draw("E1PSAME");
		h_data_StatUnc->Draw("E1PSAME");

		// -- General Setting for both plots -- //
		h_data->SetXTitle( "Dimuon Mass [GeV]" );
		h_data->SetYTitle( "d#sigma/dm [pb/GeV]" );

		h_data->SetStats(kFALSE);
		h_theory->SetStats(kFALSE);
		h_data_StatUnc->SetStats(kFALSE);

		// -- Setting for h1 -- //
		h_data->SetLineColor(kBlack);
		h_data->SetLineWidth(1);
		h_data->SetMarkerColor(kBlack);
		h_data->SetMarkerSize(1);
		h_data->SetMarkerStyle(20);
		h_data->SetFillColorAlpha(kWhite, 0);

		// -- Setting for h2 -- //
		h_theory->SetLineColor(kRed);
		h_theory->SetLineWidth(1);
		h_theory->SetMarkerColor(kRed);
		h_theory->SetMarkerSize(1);
		h_theory->SetMarkerStyle(20);
		h_theory->SetFillColorAlpha(kWhite, 0);

		// -- Setting for h3 -- //
		h_data_StatUnc->SetLineColor(kBlack);
		h_data_StatUnc->SetLineWidth(1);
		h_data_StatUnc->SetMarkerColor(kBlack);
		h_data_StatUnc->SetMarkerSize(1);
		h_data_StatUnc->SetMarkerStyle(20);
		h_data_StatUnc->SetFillColorAlpha(kWhite, 0);

		// -- X-axis Setting -- //
		h_data->GetXaxis()->SetLabelSize(0);
		h_data->GetXaxis()->SetTitleSize(0);
		h_data->GetXaxis()->SetNoExponent(); 
		h_data->GetXaxis()->SetMoreLogLabels();
		h_data->GetXaxis()->SetRangeUser( 15, 3000 );
		if( Type.Contains("Powheg") )
			h_data->GetXaxis()->SetRangeUser( 50, 3000 );

		// -- Y-axis Setting -- //
		h_data->GetYaxis()->SetTitleSize(0.06);
		h_data->GetYaxis()->SetTitleOffset(1.25);

		// -- Add Legend -- //
		TLegend *legend = new TLegend(0.55, 0.75, 0.95, 0.95);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->AddEntry(h_data, "Data");
		legend->AddEntry(h_theory, LegendName_Theory);
		legend->Draw();

		//////////////////////
		// -- Bottom Pad -- //
		//////////////////////
		TPad *bottomPad = new TPad("bottomPad","bottomPad",0.01,0.01,0.99,0.3);
		bottomPad->Draw();
		bottomPad->cd();

		bottomPad->SetBottomMargin(0.4);
		bottomPad->SetRightMargin(0.04);
		bottomPad->SetLeftMargin(0.15);	

		gPad->SetLogx();

		// -- Make Ratio plot1: Data / FEWZ & -- //
		TH1D* h_ratio = (TH1D*)h_data->Clone();
		h_ratio->SetName("h_ratio");
		this->MakeRatioHistogram( h_data, h_theory, h_ratio );

		// -- Make Ratio plot1: Data (OnlyStat.) / FEWZ & -- //
		TH1D* h_theory_NoError = (TH1D*)h_theory->Clone();
		Int_t nBin = h_theory_NoError->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			h_theory_NoError->SetBinError(i_bin, 0);
		}
		TH1D* h_ratio_StatUnc = (TH1D*)h_data_StatUnc->Clone();
		h_ratio_StatUnc->SetName("h_ratio");
		this->MakeRatioHistogram( h_data_StatUnc, h_theory_NoError, h_ratio_StatUnc );

		for(Int_t i_bin=1; i_bin<=nMassBin; i_bin++)
		{
			Double_t LowerEdge = h_ratio->GetBinLowEdge(i_bin);
			Double_t UpperEdge = h_ratio->GetBinLowEdge(i_bin+1);

			printf("[%.1lf, %.1lf] (ratio_error, ratio_StatOnly) = (%.3lf, %.3lf)\n", LowerEdge, UpperEdge, h_ratio->GetBinError(i_bin), h_ratio_StatUnc->GetBinError(i_bin) );
		}

		h_ratio->Draw("E2P");
		h_ratio_StatUnc->Draw("E2PSAME");

		TLegend *legend_ratio = new TLegend(0.15, 0.75, 0.55, 0.95);
		legend_ratio->SetFillStyle(0);
		legend_ratio->SetBorderSize(0);
		legend_ratio->AddEntry( h_ratio, "Total");
		legend_ratio->AddEntry( h_ratio_StatUnc, "Stat.");
		legend_ratio->Draw();

		// -- General Setting -- //
		h_ratio->SetLineColor(kBlack);
		h_ratio->SetMarkerStyle(20);
		h_ratio->SetMarkerSize(1);
		h_ratio->SetMarkerColor(kBlack);
		h_ratio->SetFillColorAlpha(kRed-9, 0.5);
		h_ratio->SetStats(kFALSE);

		h_ratio_StatUnc->SetLineColor(kBlue);
		h_ratio_StatUnc->SetMarkerStyle(20);
		h_ratio_StatUnc->SetMarkerSize(1);
		h_ratio_StatUnc->SetMarkerColor(kBlack);
		h_ratio_StatUnc->SetFillColorAlpha(kBlue-9, 1);
		h_ratio_StatUnc->SetFillStyle(3144);

		h_ratio_StatUnc->SetStats(kFALSE);

		// -- X-axis Setting -- //
		TString XTitle = "Dimuon Mass [GeV]";
		if( Type.Contains("FpoF") )
			XTitle = "Dimuon Mass (fiducial, post-FSR) [GeV]";

		h_ratio->GetXaxis()->SetTitle( XTitle );
		h_ratio->GetXaxis()->SetTitleOffset( 0.9 );
		h_ratio->GetXaxis()->SetTitleSize( 0.2 );
		h_ratio->GetXaxis()->SetLabelColor(1);
		h_ratio->GetXaxis()->SetLabelFont(42);
		h_ratio->GetXaxis()->SetLabelOffset(0.007);
		h_ratio->GetXaxis()->SetLabelSize(0.15);
		h_ratio->GetXaxis()->SetMoreLogLabels(); 
		h_ratio->GetXaxis()->SetNoExponent();

		// -- Y-axis Setting -- //
		h_ratio->GetYaxis()->SetTitle( "Data/Theory" );
		h_ratio->GetYaxis()->SetTitleOffset( 0.4 );
		h_ratio->GetYaxis()->SetTitleSize( 0.1);
		h_ratio->GetYaxis()->SetLabelSize( 0.07 );
		h_ratio->GetYaxis()->SetRangeUser( 0.5, 1.5 );

		// -- flat line = 1.00 -- //
		TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		f_line->SetLineColor(kRed);
		f_line->SetLineWidth(1);
		f_line->Draw("SAME");

		h_ratio->Draw("E2PSAME");
		h_ratio_StatUnc->Draw("E2PSAME");

		c->SaveAs(CanvasName + ".pdf");
		c->SaveAs(CanvasName + ".C");

		TCanvas *c_ratio = new TCanvas("c_ratio", "", 800, 800); c_ratio->cd();
		TPad *RatioPad = (TPad*)bottomPad->Clone();
		RatioPad->SetPad(0.01,0.01,0.99,0.99);
		RatioPad->SetBottomMargin(0.12);
		RatioPad->SetRightMargin(0.03);
		RatioPad->SetLeftMargin(0.12);

		TH1D* h_ratio2 = (TH1D*)RatioPad->GetPrimitive("h_ratio");
		h_ratio2->GetXaxis()->SetTitleOffset(1.2);
		h_ratio2->GetXaxis()->SetTitleSize(0.05);
		h_ratio2->GetXaxis()->SetLabelSize(0.05);
		h_ratio2->GetYaxis()->SetRangeUser(0, 2);
		h_ratio2->GetYaxis()->SetTitleSize(0.05);
		h_ratio2->GetYaxis()->SetTitleOffset(1);
		h_ratio2->GetYaxis()->SetLabelSize(0.05);
		RatioPad->Draw();

		// c_ratio->SaveAs("c_ratio.C");
	}

	void Comparison_FEWZ_vs_aMCNLO()
	{
		TFile *f_aMCNLO = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root"); f_aMCNLO->cd();
		TH1D* h_aMCNLO = (TH1D*)f_aMCNLO->Get("h_DiffXsec_aMCNLO")->Clone();

		TFile *f_FEWZ = TFile::Open(FileLocation + "/ROOTFile_xSec_Theory.root"); f_FEWZ->cd();
		TH1D* h_FEWZ = (TH1D*)f_FEWZ->Get("h_DiffXsec_FEWZ_NNPDF_NNLO")->Clone();

		MyCanvas *myc = new MyCanvas("myc_DiffXsec_aMCNLO_vs_FEWZ", "Dimuon Mass [GeV]", "d#sigma/dm [pb/GeV]");
		myc->SetLogx(1);
		myc->SetLogy(0);
		myc->SetRatioRange(0.9, 1.1);
		myc->CanvasWithHistogramsRatioPlot( h_aMCNLO, h_FEWZ, "aMC@NLO", "FEWZ (NNLO, NNPDF)", "aMC@NLO/FEWZ");
		myc->PrintCanvas();

		delete f_aMCNLO;
		delete f_FEWZ;
	}

	void SetErrors( TH1D *h, TH1D *h_RelUnc )
	{
		Int_t nBins = h->GetNbinsX();
		for(Int_t i=0; i<nBins; i++)
		{
			Int_t i_bin = i+1;
			Double_t DiffxSec = h->GetBinContent(i_bin);
			Double_t RelUnc_Stat = h_RelUnc->GetBinContent(i_bin) / 100.0 ; // -- convert from % to ordinary number -- //
			Double_t AbsUnc_Stat = DiffxSec * RelUnc_Stat;

			h->SetBinError( i_bin, AbsUnc_Stat );
		}
	}

	void MakeRatioHistogram( TH1D *h1, TH1D *h2, TH1D *h_ratio )
	{
		Int_t nBins = h1->GetNbinsX();
		for(Int_t i=0; i<nBins; i++)
		{
			Int_t i_bin = i+1;
			Double_t value1 = h1->GetBinContent(i_bin);
			Double_t value2 = h2->GetBinContent(i_bin);

			if( value2 == 0 )
			{
				cout << "ERROR!: value2 == 0!" << endl;
				return;
			}

			Double_t ratio = value1 / value2;

			Double_t error1 = h1->GetBinError(i_bin);
			Double_t error2 = h2->GetBinError(i_bin);
			Double_t error_ratio = this->Error_PropagatedAoverB(value1, error1, value2, error2);

			h_ratio->SetBinContent( i_bin, ratio );
			h_ratio->SetBinError( i_bin, error_ratio );
		}
	}

	Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B)
	{
		Double_t ratio_A = (sigma_A) / A;
		Double_t ratio_B = (sigma_B) / B;

		Double_t errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;

		return (A/B) * sqrt(errorSquare);
	}

	void SaveResults( Bool_t FpoF = kFALSE )
	{
		TString FileName = "ROOTFile_DiffXSec_FullUnc.root";
		if( FpoF ) 
			FileName.ReplaceAll( "ROOTFile_", "ROOTFile_FpoF_" );

		f_output = new TFile(FileName, "RECREATE"); f_output->cd();

		h_RelStatUnc->SetName("h_RelStatUnc_Percent");
		h_RelStatUnc->Write();

		Int_t nSource = (Int_t)Sources.size();
		for(Int_t i=0; i<nSource; i++)
		{
			Histos_Unc[i]->SetName( "h_RelSysUnc_"+Sources[i]+"_Percent" );
			Histos_Unc[i]->Write();
		}
		h_RelSysUnc_Tot->Write();
		h_RelLumiUnc->Write();
		h_RelTotUnc->Write();

		c_RelUnc->Write();

		this->h_data->Write();
		this->h_data_StatUnc->Write();
		TH1D* h_data_woLumi = this->MakeDiffXSecPlot_woLumiUnc();
		h_data_woLumi->Write();

		this->h_aMCNLO->Write();
		if( this->h_FEWZ != NULL )
			this->h_FEWZ->Write();
	}

	TH1D* MakeDiffXSecPlot_woLumiUnc()
	{
		TString HistName = this->h_data->GetName();
		TH1D* h_data_woLumi = (TH1D*)this->h_data->Clone(HistName+"_woLumi");
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t value = this->h_data->GetBinContent(i_bin);
			Double_t AbsTotUnc = this->h_data->GetBinError(i_bin);
			Double_t AbsLumiUnc = (this->h_RelLumiUnc->GetBinContent(i_bin) / 100.0) * value;

			Double_t AbsTotUnc_woLumi = sqrt( AbsTotUnc*AbsTotUnc - AbsLumiUnc*AbsLumiUnc );

			h_data_woLumi->SetBinError(i_bin, AbsTotUnc_woLumi );
		}

		return h_data_woLumi;
	}

	void PrintFinalResults()
	{
		printf("                     ");
		printf("%15s", "dSigma/dM" );
		printf("%15s", "Stat.");
		printf("%15s", "Exp.");
		printf("%15s", "Theo.");
		printf("%15s", "Total");
		printf("\n");

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t lowerEdge = h_RelTotUnc->GetBinLowEdge(i_bin);
			Double_t upperEdge = h_RelTotUnc->GetBinLowEdge(i_bin+1);

			printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

			Double_t DiffXsec = h_data->GetBinContent(i_bin);

			Double_t RelUnc_stat = h_RelStatUnc->GetBinContent(i_bin);
			Double_t unc_stat = (RelUnc_stat * DiffXsec) / 100.0;

			Double_t RelUnc_Exp_syst = h_RelSysUnc_Tot->GetBinContent(i_bin);
			Double_t RelUnc_Exp_lumi = h_RelLumiUnc->GetBinContent(i_bin);
			Double_t RelUnc_Exp = sqrt( RelUnc_Exp_syst*RelUnc_Exp_syst + RelUnc_Exp_lumi*RelUnc_Exp_lumi );
			Double_t unc_Exp = ( RelUnc_Exp * DiffXsec ) / 100.0;

			Double_t RelUnc_Theo = 0;
			Int_t nSource = (Int_t)Sources.size();
			for(Int_t i_source=0; i_source<nSource; i_source++)
			{
				if( Sources[i_source].Contains("Acc") )
					RelUnc_Theo = Histos_Unc[i_source]->GetBinContent(i_bin);
			}

			Double_t unc_Theo = (RelUnc_Theo * DiffXsec) / 100.0;

			Double_t RelUnc_Tot = h_RelTotUnc->GetBinContent(i_bin);
			Double_t unc_total = (RelUnc_Tot * DiffXsec) / 100.0;

			printf("%15.1e", DiffXsec );
			printf("%15.1e", unc_stat );
			printf("%15.1e", unc_Exp );
			printf("%15.1e", unc_Theo );
			printf("%15.1e", unc_total );

			// printf("%15.2e", DiffXsec );
			// printf("%15.2lf", RelUnc_stat );
			// printf("%15.2lf", RelUnc_Exp );
			// printf("%15.2lf", RelUnc_Theo );
			// printf("%15.2lf", RelUnc_Tot );

			printf("\n");
		}
	}

	void FpoF_PrintFinalResults()
	{
		printf("                     ");
		printf("%15s", "dSigma/dM" );
		printf("%15s", "Stat.");
		printf("%15s", "Exp.");
		// printf("%15s", "Theo.");
		printf("%15s", "Total");
		printf("\n");

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t lowerEdge = h_RelTotUnc->GetBinLowEdge(i_bin);
			Double_t upperEdge = h_RelTotUnc->GetBinLowEdge(i_bin+1);

			printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

			Double_t DiffXsec = h_data->GetBinContent(i_bin);

			Double_t RelUnc_stat = h_RelStatUnc->GetBinContent(i_bin);
			Double_t unc_stat = (RelUnc_stat * DiffXsec) / 100.0;

			Double_t RelUnc_Exp_syst = h_RelSysUnc_Tot->GetBinContent(i_bin);
			Double_t RelUnc_Exp_lumi = h_RelLumiUnc->GetBinContent(i_bin);
			Double_t RelUnc_Exp = sqrt( RelUnc_Exp_syst*RelUnc_Exp_syst + RelUnc_Exp_lumi*RelUnc_Exp_lumi );
			Double_t unc_Exp = ( RelUnc_Exp * DiffXsec ) / 100.0;

			// Double_t RelUnc_Theo = 0;
			// Int_t nSource = (Int_t)Sources.size();
			// for(Int_t i_source=0; i_source<nSource; i_source++)
			// {
			// 	if( Sources[i_source].Contains("Acc") )
			// 		RelUnc_Theo = Histos_Unc[i_source]->GetBinContent(i_bin);
			// }

			// Double_t unc_Theo = (RelUnc_Theo * DiffXsec) / 100.0;

			Double_t RelUnc_Tot = h_RelTotUnc->GetBinContent(i_bin);
			Double_t unc_total = (RelUnc_Tot * DiffXsec) / 100.0;

			printf("%15.1e", DiffXsec );
			printf("%15.1e", unc_stat );
			printf("%15.1e", unc_Exp );
			// printf("%15.1e", unc_Theo );
			printf("%15.1e", unc_total );

			// printf("%15.2e", DiffXsec );
			// printf("%15.2lf", RelUnc_stat );
			// printf("%15.2lf", RelUnc_Exp );
			// printf("%15.2lf", RelUnc_Theo );
			// printf("%15.2lf", RelUnc_Tot );

			printf("\n");
		}
	}

	void Print_SysUnc_texFormat()
	{
		cout << "===========================================================================" << endl;		
		Int_t nSource = (Int_t)Sources.size();
		printf("                     ");
		printf("%10s", "Mom.Corr" );
		for(Int_t i_source=0; i_source<nSource; i_source++)
		{
			if( !Sources[i_source].Contains("Acc") )
				printf("%10s", Sources[i_source].Data() );
		}
		printf("%10s", "TotSys" );
		printf("%10s", "Acc.");
		printf("\n");

		// -- syst. from momentum scale -- //
		TFile *f_MomCorr = TFile::Open(FileLocation+"/ROOTFile_SysUnc_DetRes.root");
		TH1D* h_Syst_MomCorr = (TH1D*)f_MomCorr->Get("h_RelSysUnc_MomCorr_Percent")->Clone();

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t lowerEdge = h_RelTotUnc->GetBinLowEdge(i_bin);
			Double_t upperEdge = h_RelTotUnc->GetBinLowEdge(i_bin+1);
			
			TString str_syst = TString::Format("%.0d-%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);
			str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_Syst_MomCorr->GetBinContent(i_bin) ); // -- Syst. form Mom.Corr -- //
			
			Double_t Unc_Acc = 0;
			for(Int_t i_source=0; i_source<nSource; i_source++)
			{
				if( Sources[i_source].Contains("Acc") )
					Unc_Acc = Histos_Unc[i_source]->GetBinContent(i_bin);
				else
					str_syst = str_syst + TString::Format(" & $  %.2lf  $", Histos_Unc[i_source]->GetBinContent(i_bin) );
			}

			str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_RelSysUnc_Tot->GetBinContent(i_bin) );
			str_syst = str_syst + TString::Format(" & $  %.2lf  $", Unc_Acc );
			str_syst = str_syst + "\\\\";

			printf( "%s\n\\hline\n", str_syst.Data() );
		}
		cout << "===========================================================================" << endl;
	}

	// void Print_FpoF_SysUnc_texFormat()
	// {
	// 	cout << "===========================================================================" << endl;		
	// 	Int_t nSource = (Int_t)Sources.size();
	// 	printf("                     ");
	// 	printf("%10s", "Mom.Corr" );
	// 	for(Int_t i_source=0; i_source<nSource; i_source++)
	// 			printf("%10s", Sources[i_source].Data() );

	// 	printf("%10s", "TotSys" );
	// 	// printf("%10s", "Acc.");
	// 	printf("\n");

	// 	// -- syst. from momentum scale -- //
	// 	TFile *f_MomCorr = TFile::Open(FileLocation+"/ROOTFile_SysUnc_DetRes.root");
	// 	TH1D* h_Syst_MomCorr = (TH1D*)f_MomCorr->Get("h_RelSysUnc_MomCorr_Percent")->Clone();

	// 	for(Int_t i=0; i<nMassBin; i++)
	// 	{
	// 		Int_t i_bin = i+1;
	// 		Double_t lowerEdge = h_RelTotUnc->GetBinLowEdge(i_bin);
	// 		Double_t upperEdge = h_RelTotUnc->GetBinLowEdge(i_bin+1);

			
	// 		TString str_syst = TString::Format("%.0d-%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);
	// 		str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_Syst_MomCorr->GetBinContent(i_bin) ); // -- Syst. form Mom.Corr -- //
			
	// 		Double_t Unc_Acc = 0;
	// 		for(Int_t i_source=0; i_source<nSource; i_source++)
	// 		{
	// 			if( Sources[i_source].Contains("Acc") )
	// 				Unc_Acc = Histos_Unc[i_source]->GetBinContent(i_bin);
	// 			else
	// 				str_syst = str_syst + TString::Format(" & $  %.2lf  $", Histos_Unc[i_source]->GetBinContent(i_bin) );
	// 		}

	// 		str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_RelSysUnc_Tot->GetBinContent(i_bin) );
	// 		str_syst = str_syst + "\\\\";

	// 		printf( "%s\n\\hline\n", str_syst.Data() );
	// 	}
	// 	cout << "===========================================================================" << endl;
	// }

	void PrintFinalResults_texFormat()
	{
		cout << "===========================================================================" << endl;
		printf("                     ");
		printf("%15s", "dSigma/dM" );
		printf("%15s", "Stat.");
		printf("%15s", "Exp.");
		printf("%15s", "Theo.");
		printf("%15s", "Total");
		printf("\n");

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t lowerEdge = h_RelTotUnc->GetBinLowEdge(i_bin);
			Double_t upperEdge = h_RelTotUnc->GetBinLowEdge(i_bin+1);

			TString str_result = TString::Format("%.0d-%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);

			// -- central value -- //
			Double_t DiffXsec = h_data->GetBinContent(i_bin);

			// -- stat. -- //
			Double_t RelUnc_stat = h_RelStatUnc->GetBinContent(i_bin);
			Double_t unc_stat = (RelUnc_stat * DiffXsec) / 100.0;

			// -- Exp. -- //
			Double_t RelUnc_Exp_syst = h_RelSysUnc_Tot->GetBinContent(i_bin);
			Double_t RelUnc_Exp_lumi = h_RelLumiUnc->GetBinContent(i_bin);
			Double_t RelUnc_Exp = sqrt( RelUnc_Exp_syst*RelUnc_Exp_syst + RelUnc_Exp_lumi*RelUnc_Exp_lumi );
			Double_t unc_Exp = ( RelUnc_Exp * DiffXsec ) / 100.0;

			// -- theory -- //
			Double_t RelUnc_Theo = 0;
			Int_t nSource = (Int_t)Sources.size();
			for(Int_t i_source=0; i_source<nSource; i_source++)
			{
				if( Sources[i_source].Contains("Acc") )
					RelUnc_Theo = Histos_Unc[i_source]->GetBinContent(i_bin);
			}

			Double_t unc_Theo = (RelUnc_Theo * DiffXsec) / 100.0;

			// -- all -- //
			Double_t RelUnc_Tot = h_RelTotUnc->GetBinContent(i_bin);
			Double_t unc_total = (RelUnc_Tot * DiffXsec) / 100.0;

			str_result = str_result + Transform_TexFormat( DiffXsec );
			str_result = str_result + Transform_TexFormat( unc_stat );
			str_result = str_result + Transform_TexFormat( unc_Exp );
			str_result = str_result + Transform_TexFormat( unc_Theo );
			str_result = str_result + Transform_TexFormat( unc_total );
			str_result = str_result + "\\\\";

			printf( "%s\n\\hline\n", str_result.Data() );
		}
		cout << "===========================================================================" << endl;
	}

	void FpoF_PrintFinalResults_texFormat()
	{
		cout << "===========================================================================" << endl;
		printf("                     ");
		printf("%15s", "dSigma/dM" );
		printf("%15s", "Stat.");
		printf("%15s", "Exp.");
		printf("%15s", "Total");
		printf("\n");

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t lowerEdge = h_RelTotUnc->GetBinLowEdge(i_bin);
			Double_t upperEdge = h_RelTotUnc->GetBinLowEdge(i_bin+1);

			TString str_result = TString::Format("%.0d-%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);

			// -- central value -- //
			Double_t DiffXsec = h_data->GetBinContent(i_bin);

			// -- stat. -- //
			Double_t RelUnc_stat = h_RelStatUnc->GetBinContent(i_bin);
			Double_t unc_stat = (RelUnc_stat * DiffXsec) / 100.0;

			// -- Exp. -- //
			Double_t RelUnc_Exp_syst = h_RelSysUnc_Tot->GetBinContent(i_bin);
			Double_t RelUnc_Exp_lumi = h_RelLumiUnc->GetBinContent(i_bin);
			Double_t RelUnc_Exp = sqrt( RelUnc_Exp_syst*RelUnc_Exp_syst + RelUnc_Exp_lumi*RelUnc_Exp_lumi );
			Double_t unc_Exp = ( RelUnc_Exp * DiffXsec ) / 100.0;

			// -- theory -- //
			// Double_t RelUnc_Theo = 0;
			// Int_t nSource = (Int_t)Sources.size();
			// for(Int_t i_source=0; i_source<nSource; i_source++)
			// {
			// 	if( Sources[i_source].Contains("Acc") )
			// 		RelUnc_Theo = Histos_Unc[i_source]->GetBinContent(i_bin);
			// }

			// Double_t unc_Theo = (RelUnc_Theo * DiffXsec) / 100.0;

			// -- all -- //
			Double_t RelUnc_Tot = h_RelTotUnc->GetBinContent(i_bin);
			Double_t unc_total = (RelUnc_Tot * DiffXsec) / 100.0;

			str_result = str_result + Transform_TexFormat( DiffXsec );
			str_result = str_result + Transform_TexFormat( unc_stat );
			str_result = str_result + Transform_TexFormat( unc_Exp );
			// str_result = str_result + Transform_TexFormat( unc_Theo );
			str_result = str_result + Transform_TexFormat( unc_total );
			str_result = str_result + "\\\\";

			printf( "%s\n\\hline\n", str_result.Data() );
		}
		cout << "===========================================================================" << endl;
	}

	TString Transform_TexFormat( Double_t value )
	{
		Int_t Exponent = Calc_Exponent( value );
		Double_t front = value / TMath::Power(10, Exponent);
		if( front >= 9.95 && front < 10.05 ) // -- if rounded off to 10.0: change it to 1.0 x 10^1
		{
			front = 1.0;
			Exponent = Exponent + 1;
		}

		TString str = TString::Format(" & $  %.1lf \\times 10^{%d}  $", front, Exponent);
		return str;
	}

	Int_t Calc_Exponent( Double_t x )
	{
		if( x >= 1 )
			return (Int_t)TMath::Log10(x);

		else if( x > 0 && x < 1 )
			return (Int_t)TMath::Log10(x) - 1;
		
		else
		{
			cout << "Can not calculate the exponent of negative value!";
			return -999;
		}
	}

	void Smoothing( TH1D* h, Double_t lowerEdge, Double_t upperEdge )
	{
		h->GetXaxis()->SetRangeUser(lowerEdge, upperEdge);
		h->Smooth(10, "R");

		h->GetXaxis()->SetRangeUser(15, 3000);
	}

	void ApplySmooth()
	{
		Int_t nSource = (Int_t)Sources.size();
		for(Int_t i_source=0; i_source<nSource; i_source++)
		{
			if( Sources[i_source] == "Det.Res" )
			{
				this->Smoothing( Histos_Unc[i_source], 15, 700 );
				cout << "Systematics from the detector resoltuion is smoothed" << endl;
			}
			else if( Sources[i_source] == "FSR" )
			{
				this->Smoothing( Histos_Unc[i_source], 15, 3000 );
				cout << "Systematics from the FSR correction is smoothed" << endl;
			}
			else if( Sources[i_source] == "Bkg.Est." )
			{
				this->Smoothing( Histos_Unc[i_source], 101, 3000 );
				cout << "Systematics from the background estimation is smoothed" << endl;
			}
		}
	}

	void Obtain_dSigma_dM(TH1D *h)
	{
		Int_t nBins = h->GetNbinsX();
		for(Int_t i=0; i<nBins; i++)
		{
			Int_t i_bin = i+1;
			Double_t BinWidth = h->GetBinWidth(i_bin);

			Double_t low = h->GetBinLowEdge(i_bin);
			Double_t high = h->GetBinLowEdge(i_bin + 1);

			Double_t xSec = h->GetBinContent(i_bin);
			Double_t xSec_dM = xSec / BinWidth;

			Double_t error_before = h->GetBinError(i_bin);
			Double_t error_after = error_before / BinWidth;

			h->SetBinContent(i_bin, xSec_dM);
			h->SetBinError(i_bin, error_after);

			// printf("%2dth bin [%5.lf, %5.lf] (xSec, BinWidth, dSigma/dM) = (%15.9lf, %6.1lf, %15.9lf), (error_before, error_after) = (%8.5lf, %8.5lf)\n", 
				// i_bin, low, high, xSec, BinWidth, xSec_dM, error_before, error_after );
		}
	}

	TH1D* MakeHistogram_over50GeV( TH1D* h )
	{
		Double_t MassBinEdges2[nMassBin+1-7] = {50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		TH1D *h2 = new TH1D(h->GetName(), "", nMassBin-7, MassBinEdges2);

		Int_t nBin = h->GetNbinsX();
		Int_t i_bin2 = 1;
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t BinCenter = h->GetBinCenter( i_bin );

			if( BinCenter > 50 )
			{
				h2->SetBinContent( i_bin2, h->GetBinContent(i_bin) );
				h2->SetBinError( i_bin2, h->GetBinError(i_bin) );
				i_bin2++;
			}
		}

		return h2;
	}

	void AssignScaleUnc_3Percent( TH1D* h )
	{
		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t DiffXSec = h->GetBinContent(i_bin);
			Double_t Err_before = h->GetBinError(i_bin);
			Double_t Err_ScaleVar = DiffXSec * 0.03; // -- 3% for all mass bins -- //

			Double_t Err_after = sqrt(Err_before*Err_before + Err_ScaleVar*Err_ScaleVar);
			h->SetBinError(i_bin, Err_after);
		}
	}

	void Add_PIContribution( TH1D* h_DXSec )
	{
		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		TString FileName_Ratio = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYPI_DY_FEWZ.root";
		TH1D* h_ratio = Get_Hist(FileName_Ratio, "h_ratio");

		Int_t nBin = h_DXSec->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t DXSec = h_DXSec->GetBinContent(i_bin);
			Double_t RelErr_DXSec = h_DXSec->GetBinError(i_bin) / DXSec;

			Double_t Ratio = h_ratio->GetBinContent(i_bin);
			Double_t RelErr_ratio = h_ratio->GetBinError(i_bin) / Ratio;

			Double_t DXSec_DYPI = DXSec * Ratio;
			// -- error propagation -- //
			Double_t RelErr_total = sqrt(RelErr_DXSec*RelErr_DXSec + RelErr_ratio*RelErr_ratio);

			h_DXSec->SetBinContent(i_bin, DXSec_DYPI);
			h_DXSec->SetBinError(i_bin, RelErr_total * DXSec_DYPI); // -- absolute error -- //
		}
	}
};