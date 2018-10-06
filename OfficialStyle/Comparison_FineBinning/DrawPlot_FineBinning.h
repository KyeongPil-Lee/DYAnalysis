#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>

class DrawingTool
{
public:
	TString ChannelType; // -- Dielectron, Dimuon, Combined -- //

	TString GraphName_CenV_Shifted;
	TString HistName_CenV;
	TString HistName_RelStatUnc;
	TString HistName_RelTotUnc;
	TString HistName_NNLO_DYBin;
	TString HistName_NNLO_FineBin;
	TString HistName_RelTheoUnc;

	TGraphAsymmErrors* g_CenV_Shifted;
	TH1D* h_CenV;
	TH1D* h_RelStatUnc;
	TH1D* h_RelTotUnc;
	TH1D* h_NNLO_DYBin;
	TH1D* h_NNLO_FineBin;
	TH1D* h_RelTheoUnc;

	DrawingTool()
	{
		this->ChannelType = "";

		this->GraphName_CenV_Shifted = "g_DiffXSec_xShifted";
		this->HistName_CenV = "h_DiffXSec";
		this->HistName_RelStatUnc = "h_RelStatUnc";
		this->HistName_RelTotUnc = "h_RelTotUnc";
		this->HistName_NNLO_DYBin = "h_NNLO_DYBin";
		this->HistName_NNLO_FineBin = "h_NNLO_FineBin";
		this->HistName_RelTheoUnc = "h_RelTheoUnc";

		this->h_CenV = NULL;
		this->h_RelStatUnc = NULL;
		this->h_RelTotUnc = NULL;
		this->h_NNLO_DYBin = NULL;
		this->h_NNLO_FineBin = NULL;
		this->h_RelTheoUnc = NULL;

		TH1::AddDirectory(kFALSE);
	}

	void MakeInputFile_Dimuon()
	{
		this->Load_TheoryHistograms();

		this->g_CenV_Shifted = Get_Graph( "ROOTFile_Histograms_XDataPoint.root", "g_DiffXSec_xShifted", this->GraphName_CenV_Shifted );

		TString FileName_Data = GetBasePath() + "Include/Results_ROOTFiles_76X/ROOTFile_DiffXSec_FullUnc.root";
		// this->h_CenV = Get_Hist( FileName_Data, "h_DiffXsec_FSRCorr_woLumi", this->HistName_CenV );
		this->h_CenV = Get_Hist( FileName_Data, "h_DiffXsec_FSRCorr", this->HistName_CenV );
		this->h_RelStatUnc = Get_Hist( FileName_Data, "h_RelStatUnc_Percent", this->HistName_RelStatUnc );
		this->h_RelStatUnc->Scale( 0.01 );
		this->h_RelTotUnc = Extract_RelUnc( this->h_CenV, this->HistName_RelTotUnc );

		this->MakeInputFile( "Dimuon" );
	}

	void MakeInputFile_Combined()
	{
		this->Load_TheoryHistograms();

		TString FileName_CenV = GetBasePath() + "Include/Results_ROOTFiles_76X/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
		// -- convert TH1F to TH1D -- //
		TFile *f_input = TFile::Open( FileName_CenV );
		f_input->cd();
		TH1F* h_temp = (TH1F*)f_input->Get( "h1Combi" )->Clone();
		this->h_CenV = new TH1D();
		h_temp->Copy( *this->h_CenV );
		this->h_CenV->SetTitle("");
		this->h_CenV->SetName(this->HistName_CenV);
		///////////////////////////////////////////

		this->g_CenV_Shifted = Get_Graph( "ROOTFile_Histograms_XDataPoint.root", "g_DiffXSec_xShifted", this->GraphName_CenV_Shifted );
		this->ChangeValues_ShiftedGraph( this->g_CenV_Shifted, this->h_CenV );

		TH1D* h_AbsStatUnc = Get_Hist( FileName_CenV, "h1_dCS_from_statYield" );
		this->h_RelStatUnc = ConvertHist_AbsToRel( this->h_CenV, h_AbsStatUnc );
		this->h_RelStatUnc->SetName( this->HistName_RelStatUnc );
		this->h_RelTotUnc = Extract_RelUnc( this->h_CenV, this->HistName_RelTotUnc );

		this->MakeInputFile( "Combined" );
	}

	void MakeInputFile_Dielectron()
	{
		this->Load_TheoryHistograms();

		TString FileName = GetBasePath() + "Include/Results_ROOTFiles_76X/DiffXsec_Electron_v8.root";

		this->h_CenV = Get_Hist( FileName, "h_DiffXSec", this->HistName_CenV );
		// Print_Histogram( this->h_CenV );
		this->h_RelStatUnc = Get_Hist( FileName, "h_RelUnc_Stat", this->HistName_RelStatUnc ); h_RelStatUnc->Scale(0.01);

		TH1D* h_RelSystUnc = Get_Hist( FileName, "h_RelUnc_Syst" ); h_RelSystUnc->Scale(0.01);
		this->h_RelTotUnc = QuadSum_NoError( this->h_RelStatUnc, h_RelSystUnc );
		TH1D* h_RelLumiUnc = this->MakeHist_RelLumiUnc( 0.023 );
		this->h_RelTotUnc = QuadSum_NoError( this->h_RelTotUnc, h_RelLumiUnc ); // -- add up lumi. uncertainty -- //
		this->h_RelTotUnc->SetName( this->HistName_RelTotUnc );

		AssignErrors( this->h_CenV, this->h_RelTotUnc );
		// Print_Histogram( this->h_CenV );

		this->g_CenV_Shifted = Get_Graph( "ROOTFile_Histograms_XDataPoint.root", "g_DiffXSec_xShifted", this->GraphName_CenV_Shifted );
		this->ChangeValues_ShiftedGraph( this->g_CenV_Shifted, this->h_CenV );

		// Print_Graph( this->g_CenV_Shifted );

		this->MakeInputFile( "Dielectron" );
	}

	void Load_Histograms( TString _ChannelType )
	{
		this->InitPointers_AfterUse();

		this->ChannelType = _ChannelType;
		TString FileName = "./Local/ROOTFile_Input_"+this->ChannelType+".root";

		this->g_CenV_Shifted = Get_Graph(FileName, this->GraphName_CenV_Shifted );
		this->h_CenV = Get_Hist(FileName, this->HistName_CenV );
		this->h_RelStatUnc = Get_Hist(FileName, this->HistName_RelStatUnc );
		this->h_RelTotUnc = Get_Hist(FileName, this->HistName_RelTotUnc );
		this->h_NNLO_DYBin = Get_Hist(FileName, this->HistName_NNLO_DYBin );
		this->h_NNLO_FineBin = Get_Hist(FileName, this->HistName_NNLO_FineBin );
		this->h_RelTheoUnc = Get_Hist(FileName, this->HistName_RelTheoUnc );
	}

	void DrawCanvas( Double_t xMin = 15, Double_t xMax = 3000 )
	{
		// TString CanvasName = TString::Format("Comp_%s_vs_NNLO_%.0lfto%.0lf", this->ChannelType.Data(), xMin, xMax);
		TString channelType2 = "";
		if( this->ChannelType == "Dimuon") channelType2 = "Muon";
		if( this->ChannelType == "Dielectron") channelType2 = "Electron";
		if( this->ChannelType == "Combined") channelType2 = "Comb";

		TString CanvasName = TString::Format("%s_data_xShifted_vs_NNLO", channelType2.Data() );
		if( !(xMin == 15 && xMax == 3000) )
			CanvasName = CanvasName + TString::Format("_%.0lfto%.0lf", xMin, xMax);

		TCanvas *c = new TCanvas("./Local/"+CanvasName, "", 800, 800);
		c->cd();

		TPad *TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
		TopPad->Draw();
		TopPad->cd();

		TopPad->SetTopMargin(0.05);
		TopPad->SetBottomMargin(0.305);
		TopPad->SetRightMargin(0.03);
		TopPad->SetLeftMargin(0.13);
		if( xMax == 3000 )
			TopPad->SetLogx();
		TopPad->SetLogy();

		this->h_NNLO_FineBin->Draw("HISTL");
		this->g_CenV_Shifted->Draw("EPSAME");

		this->h_NNLO_FineBin->SetXTitle("m (#mu#mu) [GeV]");
		this->h_NNLO_FineBin->SetYTitle("d#sigma/dm [pb/GeV]");

		this->h_NNLO_FineBin->GetXaxis()->SetNoExponent();
		this->h_NNLO_FineBin->GetXaxis()->SetMoreLogLabels();
		this->h_NNLO_FineBin->GetXaxis()->SetRangeUser(xMin, xMax);
		this->h_NNLO_FineBin->GetXaxis()->SetLabelSize(0);

		this->h_NNLO_FineBin->GetYaxis()->SetTitleSize(0.05);

		this->h_NNLO_FineBin->SetLineColor(kBlue);
		this->h_NNLO_FineBin->SetLineWidth(1);
		this->h_NNLO_FineBin->SetMarkerColor(kBlue);
		this->h_NNLO_FineBin->SetMarkerStyle(20);
		this->h_NNLO_FineBin->SetFillColorAlpha(kWhite, 0);

		this->g_CenV_Shifted->SetLineColor(kBlack);
		this->g_CenV_Shifted->SetMarkerColor(kBlack);
		this->g_CenV_Shifted->SetMarkerStyle(20);
		this->g_CenV_Shifted->SetFillColorAlpha(kWhite, 0);

		TLegend *legend = new TLegend(0.15, 0.32, 0.60, 0.47);
		legend->SetBorderSize(0);
		legend->SetFillStyle(0);
		legend->SetTextFont(62);
		legend->AddEntry( this->g_CenV_Shifted, "Data" );

		TH1D* h_forLegend = (TH1D*)this->h_NNLO_FineBin->Clone();
		h_forLegend->SetFillColorAlpha( kBlue, 1 );
		legend->AddEntry( h_forLegend, "FEWZ (NNLO, NNPDF3.0)");
		legend->Draw();

		TLatex latex; DrawLatexNDC( latex );

		// -- ratio plot -- //
		TH1D* h_StatUnc = (TH1D*)this->h_CenV->Clone("h_Stat");
		h_StatUnc->Divide( this->h_CenV, this->h_CenV ); // -- central value = 1 -- //
		AssignErrors( h_StatUnc, this->h_RelStatUnc );

		TH1D* h_TotUnc = (TH1D*)h_StatUnc->Clone();
		AssignErrors( h_TotUnc, this->h_RelTotUnc );

		TH1D* h_ratio = (TH1D*)this->h_CenV->Clone("h_ratio");
		// h_ratio->Divide( this->h_CenV, this->h_NNLO_DYBin );
		h_ratio->Divide( this->h_NNLO_DYBin, this->h_CenV );
		AssignErrors( h_ratio, this->h_RelTheoUnc );

		TGraphAsymmErrors* g_StatUnc = this->Convert_XShiftGraph( this->g_CenV_Shifted, h_StatUnc );
		TGraphAsymmErrors* g_TotUnc = this->Convert_XShiftGraph( this->g_CenV_Shifted, h_TotUnc );
		TGraphAsymmErrors* g_ratio = this->Convert_XShiftGraph( this->g_CenV_Shifted, h_ratio );

		RemoveHorizontalErrors( this->g_CenV_Shifted );

		TPad *BottomPad = new TPad("BottomPad","BottomPad",0.01,0.01,0.99,0.3);
		BottomPad->Draw();
		BottomPad->cd();

		BottomPad->SetTopMargin(0.01);
		BottomPad->SetBottomMargin(0.4);
		BottomPad->SetRightMargin(0.02);
		BottomPad->SetLeftMargin(0.12);
		if( xMax == 3000 )
			BottomPad->SetLogx();

		h_ratio->Draw("AXIS");
		g_ratio->Draw("E2SAME");
		g_TotUnc->Draw("E2PSAME");
		g_StatUnc->Draw("E1PSAME");

		h_ratio->SetTitle("");
		h_ratio->SetLineColor(kBlack);
		h_ratio->SetMarkerStyle(20);
		h_ratio->SetMarkerSize(1);
		h_ratio->SetMarkerColor(kBlack);
		// h_ratio->SetStats(kFALSE);

		h_ratio->GetXaxis()->SetTitle("m [GeV]");
		h_ratio->GetXaxis()->SetTitleOffset( 0.9 );
		h_ratio->GetXaxis()->SetTitleSize( 0.2 );
		h_ratio->GetXaxis()->SetLabelColor(1);
		h_ratio->GetXaxis()->SetLabelFont(42);
		h_ratio->GetXaxis()->SetLabelOffset(0.007);
		h_ratio->GetXaxis()->SetLabelSize(0.14);
		h_ratio->GetXaxis()->SetMoreLogLabels(); 
		h_ratio->GetXaxis()->SetNoExponent();
		h_ratio->GetXaxis()->SetRangeUser( xMin, xMax );

		// h_ratio->GetYaxis()->SetTitle( "Data/Theory" );
		h_ratio->GetYaxis()->SetTitle( "Theory/Data" );
		h_ratio->GetYaxis()->SetTitleOffset( 0.4 );
		h_ratio->GetYaxis()->SetTitleSize( 0.15 );
		h_ratio->GetYaxis()->SetLabelSize( 0.12 );
		h_ratio->GetYaxis()->SetRangeUser( 0.31, 1.69 );
		h_ratio->GetYaxis()->SetNdivisions( 503 );

		g_ratio->SetFillColorAlpha( kBlue, 0.4 );
		g_ratio->SetLineColorAlpha( kBlue, 0 );
		g_ratio->SetMarkerColor( kBlue );
		g_ratio->SetMarkerStyle( 32 );
		g_ratio->SetFillStyle( 1001 );

		gStyle->SetHatchesSpacing( 1.5 );
		g_TotUnc->SetFillColorAlpha( kGray+1, 1 );
		g_TotUnc->SetFillStyle( 3354 );
		g_TotUnc->SetMarkerColorAlpha(kBlack, 0);
		g_TotUnc->SetLineColorAlpha(kGray+1, 0);
		
		g_StatUnc->SetMarkerStyle(20);
		g_StatUnc->SetMarkerColor(kBlack);
		g_StatUnc->SetMarkerSize(0);
		g_StatUnc->SetLineColor(kBlack);
		g_StatUnc->SetLineWidth(1);

		TLegend *legend_ratio = new TLegend( 0.15, 0.41, 0.60, 0.56 );
		legend_ratio->SetBorderSize(0);
		legend_ratio->SetFillStyle(0);
		legend_ratio->SetTextFont(62);
		legend_ratio->SetNColumns(3);
		legend_ratio->AddEntry( g_StatUnc, "Stat. unc." );
		legend_ratio->AddEntry( g_TotUnc, "Tot. unc." );
		TGraphAsymmErrors* g_ratio_ForLegend = (TGraphAsymmErrors*)g_ratio->Clone();
		g_ratio_ForLegend->SetMarkerColorAlpha(kWhite, 0 );
		legend_ratio->AddEntry( g_ratio_ForLegend, "Theo. unc." );
		legend_ratio->Draw();


		// -- flat line = 1.00 -- //
		TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		f_line->SetLineColor(kRed);
		f_line->SetLineWidth(1);
		f_line->Draw("SAME");

		g_ratio->Draw("EPSAME");

		c->SaveAs(".pdf");
		// c->SaveAs(".C");
		TString PlotPath = gSystem->Getenv("KP_PLOT_PATH");
		c->SaveAs(PlotPath+"/"+CanvasName+".pdf");
	}

protected:
	void Load_TheoryHistograms()
	{
		TString FileName_NNLO = GetBasePath() + "TheoryValues/NNLO_NNPDF30_FineBinning/ROOTFile_DY_FEWZ_NNLO.root";
		this->h_NNLO_FineBin = Get_Hist( FileName_NNLO, "h_DiffXsec", this->HistName_NNLO_FineBin );

		// -- without PI -- //
		TString FileName_Data = GetBasePath() + "Include/Results_ROOTFiles_76X/ROOTFile_DiffXSec_FullUnc.root";
		this->h_NNLO_DYBin = Get_Hist( FileName_Data, "h_DiffXsec_FEWZ_NNPDF_NNLO", this->HistName_NNLO_DYBin );
		
		// -- with PI -- //
		// TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		// TString FileName_Data = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYXSecNNLO_WithPI.root";
		// this->h_NNLO_DYBin = Get_Hist( FileName_Data, "h_DYDiffXSecNNLO_WithPI", this->HistName_NNLO_DYBin );

		this->h_RelTheoUnc = Extract_RelUnc( this->h_NNLO_DYBin, this->HistName_RelTheoUnc );
	}

	void InitPointers_AfterUse()
	{
		delete this->h_CenV;
		delete this->h_RelStatUnc;
		delete this->h_RelTotUnc;
		delete this->h_NNLO_DYBin;
		delete this->h_NNLO_FineBin;
		delete this->h_RelTheoUnc;

		this->h_CenV = NULL;
		this->h_RelStatUnc = NULL;
		this->h_RelTotUnc = NULL;
		this->h_NNLO_DYBin = NULL;
		this->h_NNLO_FineBin = NULL;
		this->h_RelTheoUnc = NULL;
	}

	void MakeInputFile( TString ChannelType )
	{
		TFile *f_output = TFile::Open("./Local/ROOTFile_Input_"+ChannelType+".root", "RECREATE");
		f_output->cd();

		this->g_CenV_Shifted->Write();
		this->h_CenV->Write();
		this->h_RelStatUnc->Write();
		this->h_RelTotUnc->Write();
		this->h_NNLO_DYBin->Write();
		this->h_NNLO_FineBin->Write();
		this->h_RelTheoUnc->Write();

		f_output->Close();

		this->InitPointers_AfterUse();
	}

	void DrawLatexNDC( TLatex &latex )
	{
		Double_t lumi_MM = 2.8;
		Double_t lumi_EE = 2.3;

		// latex.DrawLatexNDC(0.14, 0.965, "#font[62]{CMS}");
		// latex.DrawLatexNDC(0.25, 0.965, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
		latex.DrawLatexNDC(0.14, 0.96, "#font[62]{#scale[0.8]{CMS}}");

		if( this->ChannelType == "Dimuon" )
		{
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");
			latex.DrawLatexNDC(0.65, 0.90, "#font[42]{#scale[0.9]{Z/#gamma* #rightarrow #mu^{+}#mu^{-}}}");
		}
		else if( this->ChannelType == "Dielectron" )
		{
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (13 TeV)}}");
			latex.DrawLatexNDC(0.65, 0.90, "#font[42]{#scale[0.9]{Z/#gamma* #rightarrow e^{+}e^{-}}}");
		}
		else if( this->ChannelType == "Combined" )
		{
			latex.DrawLatexNDC(0.48, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (ee) 2.8 fb^{-1} (#mu#mu) (13 TeV)}}");
			// latex.DrawLatexNDC(0.48, 0.96, TString::Format("#font[42]{#scale[0.7]{%.1lf fb^{-1} (ee) %.1lf fb^{-1} (#mu#mu)} #scale[0.8]{(13 TeV)}}", lumi_EE, lumi_MM) );
			latex.DrawLatexNDC(0.65, 0.90, "#font[42]{#scale[0.9]{Z/#gamma* #rightarrow e^{+}e^{-}, #mu^{+}#mu^{-}}}");
		}
	}

	TGraphAsymmErrors* Convert_XShiftGraph( TGraphAsymmErrors* g_data, TH1D* h_ratio )
	{
		TGraphAsymmErrors* g_ratio = (TGraphAsymmErrors*)g_data->Clone("g_ratio");

		Int_t nPoint = g_data->GetN();
		for(Int_t i=0; i<nPoint; i++)
		{
			Double_t x, y, xErr_low, xErr_high;
			g_data->GetPoint( i, x, y );
			xErr_low = g_data->GetErrorXlow(i);
			xErr_high = g_data->GetErrorXhigh(i);

			Int_t i_bin = i+1;
			Double_t ratio = h_ratio->GetBinContent(i_bin);
			Double_t error = h_ratio->GetBinError(i_bin);

			g_ratio->SetPoint(i, x, ratio );
			g_ratio->SetPointError( i, xErr_low, xErr_high, error, error );
		}

		return g_ratio;
	}

	void ChangeValues_ShiftedGraph( TGraphAsymmErrors* g_data, TH1D* h_otherChannel )
	{
		Int_t nPoint = g_data->GetN();
		Int_t nBin = h_otherChannel->GetNbinsX();
		if( nPoint != nBin )
		{
			printf("[nPoint != nBin ... check the details] (nPoint, nBin) = (%d, %d)\n", nPoint, nBin);
			return;
		}

		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t DiffXSec = h_otherChannel->GetBinContent(i_bin);
			Double_t Err_DiffXSec = h_otherChannel->GetBinError(i_bin);

			Double_t x, y;
			g_data->GetPoint( i, x, y);

			g_data->SetPoint( i, x, DiffXSec );
			g_data->SetPointEYlow( i, Err_DiffXSec );
			g_data->SetPointEYhigh( i, Err_DiffXSec );
		}
	}

	TH1D* MakeHist_RelLumiUnc( Double_t RelLumiUnc )
	{
		if( this->h_CenV == NULL )
		{
			cout << "[MakeHist_RelLumiUnc] h_CenV should be set first (to have same binning)" << endl;
			return NULL;
		}

		TH1D* h_RelLumiUnc = (TH1D*)this->h_CenV->Clone();
		Int_t nBin = h_RelLumiUnc->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			h_RelLumiUnc->SetBinContent(i_bin, RelLumiUnc );
			h_RelLumiUnc->SetBinError(i_bin, 0);
		}

		return h_RelLumiUnc;
	}

	void RemoveHorizontalErrors( TGraphAsymmErrors* g )
	{
		Int_t nPoint = g->GetN();
		for(Int_t i=0; i<nPoint; i++)
		{
			g->SetPointEXhigh(i, 0);
			g->SetPointEXlow(i, 0);
		}
	}
};