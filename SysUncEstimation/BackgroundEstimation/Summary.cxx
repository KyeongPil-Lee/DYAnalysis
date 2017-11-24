#include <Include/PlotTools.h>
// -- drawing plots & make the final root file -- //
class SummaryTool
{
public:
	TString fileName;
	TString outputFileName;

	TH1D* h_RelUnc_ttbar_Stat;
	TH1D* h_RelUnc_ttbar_Syst;
	TH1D* h_RelUnc_ttbar_Tot;

	TH1D* h_RelUnc_tW_Stat;
	TH1D* h_RelUnc_tW_Syst;
	TH1D* h_RelUnc_tW_Tot;

	TH1D* h_RelUnc_DYtautau_Stat;
	TH1D* h_RelUnc_DYtautau_Syst;
	TH1D* h_RelUnc_DYtautau_Tot;

	TH1D* h_RelUnc_wjets_Stat;
	TH1D* h_RelUnc_wjets_Syst;
	TH1D* h_RelUnc_wjets_Tot;

	TH1D* h_RelUnc_dijet_Stat;
	TH1D* h_RelUnc_dijet_Syst;
	TH1D* h_RelUnc_dijet_Tot;

	TH1D* h_RelUnc_WW_Stat;
	TH1D* h_RelUnc_WW_Syst;
	TH1D* h_RelUnc_WW_Tot;

	TH1D* h_RelUnc_WZ_Stat;
	TH1D* h_RelUnc_WZ_Syst;
	TH1D* h_RelUnc_WZ_Tot;

	TH1D* h_RelUnc_ZZ_Stat;
	TH1D* h_RelUnc_ZZ_Syst;
	TH1D* h_RelUnc_ZZ_Tot;

	TH1D* h_RelUnc_emu_Stat;
	TH1D* h_RelUnc_emu_Syst;
	TH1D* h_RelUnc_emu_Tot;

	TH1D* h_RelUnc_FR_Stat;
	TH1D* h_RelUnc_FR_Syst;
	TH1D* h_RelUnc_FR_Tot;

	TH1D* h_RelUnc_MC_Stat;
	TH1D* h_RelUnc_MC_Syst;
	TH1D* h_RelUnc_MC_Tot;

	// -- final result: will be loaded from the other code. so that's why naming convention is different -- //
	TH1D* h_RelUnc_Stat_Total;
	TH1D* h_RelUnc_Syst_Total;
	TH1D* h_RelUnc_Tot_Total;


	SummaryTool(TString _fileName )
	{
		this->fileName = _fileName;
		this->Init();
	}

	void Draw_All()
	{
		vector<TString> vec_StatSyst = {"Total", "Stat.", "Syst."};
		vector<TH1D*> vec_Hist_ttbar = { this->h_RelUnc_ttbar_Tot, this->h_RelUnc_ttbar_Stat, this->h_RelUnc_ttbar_Syst };
		this->MakeCanvas_SysUnc( "ttbar", vec_Hist_ttbar, vec_StatSyst );

		vector<TH1D*> vec_Hist_tW = { this->h_RelUnc_tW_Tot, this->h_RelUnc_tW_Stat, this->h_RelUnc_tW_Syst };
		this->MakeCanvas_SysUnc( "tW", vec_Hist_tW, vec_StatSyst );

		vector<TH1D*> vec_Hist_DYtautau = { this->h_RelUnc_DYtautau_Tot, this->h_RelUnc_DYtautau_Stat, this->h_RelUnc_DYtautau_Syst };
		this->MakeCanvas_SysUnc( "DYtautau", vec_Hist_DYtautau, vec_StatSyst );

		vector<TH1D*> vec_Hist_wjets = { this->h_RelUnc_wjets_Tot, this->h_RelUnc_wjets_Stat, this->h_RelUnc_wjets_Syst };
		this->MakeCanvas_SysUnc( "wjets", vec_Hist_wjets, vec_StatSyst );

		vector<TH1D*> vec_Hist_dijet = { this->h_RelUnc_dijet_Tot, this->h_RelUnc_dijet_Stat, this->h_RelUnc_dijet_Syst };
		this->MakeCanvas_SysUnc( "dijet", vec_Hist_dijet, vec_StatSyst );

		vector<TH1D*> vec_Hist_WW = { this->h_RelUnc_WW_Tot, this->h_RelUnc_WW_Stat, this->h_RelUnc_WW_Syst };
		this->MakeCanvas_SysUnc( "WW", vec_Hist_WW, vec_StatSyst );

		vector<TH1D*> vec_Hist_WZ = { this->h_RelUnc_WZ_Tot, this->h_RelUnc_WZ_Stat, this->h_RelUnc_WZ_Syst };
		this->MakeCanvas_SysUnc( "WZ", vec_Hist_WZ, vec_StatSyst );

		vector<TH1D*> vec_Hist_ZZ = { this->h_RelUnc_ZZ_Tot, this->h_RelUnc_ZZ_Stat, this->h_RelUnc_ZZ_Syst };
		this->MakeCanvas_SysUnc( "ZZ", vec_Hist_ZZ, vec_StatSyst );

		// -- canvas for emu backgrounds -- //
		vector<TString> vec_emu = {"Total", "t#bar{t}", "tW+#bar{t}W", "Z/#gamma*#rightarrow#tau#tau", "WW"};
		vector<TH1D*> vec_Hist_emu = 
		{ 
			this->h_RelUnc_emu_Tot,
			this->h_RelUnc_ttbar_Tot,
			this->h_RelUnc_tW_Tot,
			this->h_RelUnc_DYtautau_Tot,
			this->h_RelUnc_WW_Tot
		};
		this->MakeCanvas_SysUnc( "emu", vec_Hist_emu, vec_emu ); // -- convert to % also -- //

		// -- canvas for FR backgrounds -- //
		vector<TString> vec_FR = {"Total", "W+jets", "QCD"};
		vector<TH1D*> vec_Hist_FR = 
		{ 
			this->h_RelUnc_FR_Tot,
			this->h_RelUnc_wjets_Tot,
			this->h_RelUnc_dijet_Tot
		};
		this->MakeCanvas_SysUnc( "FR", vec_Hist_FR, vec_FR ); // -- convert to % also -- //

		// -- canvas for MC backgrounds -- //
		vector<TString> vec_MC = {"Total", "WZ", "ZZ"};
		vector<TH1D*> vec_Hist_MC = 
		{ 
			this->h_RelUnc_MC_Tot,
			this->h_RelUnc_WZ_Tot,
			this->h_RelUnc_ZZ_Tot
		};
		this->MakeCanvas_SysUnc( "MC", vec_Hist_MC, vec_MC ); // -- convert to % also -- //

		// -- canvas for total backgrounds -- //
		vector<TString> vec_Total = {"Total", "MC-based bkg.", "e#mu method", "FR method"};
		vector<TH1D*> vec_Hist_Total = 
		{ 
			this->h_RelUnc_Tot_Total,
			this->h_RelUnc_MC_Tot,
			this->h_RelUnc_emu_Tot,
			this->h_RelUnc_FR_Tot
		};
		this->MakeCanvas_SysUnc( "Total", vec_Hist_Total, vec_Total ); // -- convert to % also -- //

		// -- canvas for stat & syst uncertainties -- //
		vector<TH1D*> vec_Hist_TotalStatSyst = 
		{ 
			this->h_RelUnc_Tot_Total,
			this->h_RelUnc_Stat_Total,
			this->h_RelUnc_Syst_Total
		};
		this->MakeCanvas_SysUnc( "TotalStatSyst", vec_Hist_TotalStatSyst, vec_StatSyst ); // -- convert to % also -- //
	}

	void Save( TFile *f_output )
	{
		this->h_RelUnc_Stat_Total->SetName("h_RelUnc_Stat_Total");
		this->h_RelUnc_Stat_Total->Write();

		this->h_RelUnc_Syst_Total->SetName("h_RelUnc_Syst_Total");
		this->h_RelUnc_Syst_Total->Write();

		this->h_RelUnc_Tot_Total->SetName("h_RelUnc_Tot_Total");
		this->h_RelUnc_Tot_Total->Write();


		this->h_RelUnc_emu_Stat->SetName("h_RelUnc_emu_Stat");
		this->h_RelUnc_emu_Stat->Write();
		this->h_RelUnc_emu_Syst->SetName("h_RelUnc_emu_Syst");
		this->h_RelUnc_emu_Syst->Write();
		this->h_RelUnc_emu_Tot->SetName("h_RelUnc_emu_Tot");
		this->h_RelUnc_emu_Tot->Write();

		this->h_RelUnc_FR_Stat->SetName("h_RelUnc_FR_Stat");
		this->h_RelUnc_FR_Stat->Write();
		this->h_RelUnc_FR_Syst->SetName("h_RelUnc_FR_Syst");
		this->h_RelUnc_FR_Syst->Write();
		this->h_RelUnc_FR_Tot->SetName("h_RelUnc_FR_Tot");
		this->h_RelUnc_FR_Tot->Write();

		this->h_RelUnc_MC_Stat->SetName("h_RelUnc_MC_Stat");
		this->h_RelUnc_MC_Stat->Write();
		this->h_RelUnc_MC_Syst->SetName("h_RelUnc_MC_Syst");
		this->h_RelUnc_MC_Syst->Write();
		this->h_RelUnc_MC_Tot->SetName("h_RelUnc_MC_Tot");
		this->h_RelUnc_MC_Tot->Write();
	}

private:
	void Init()
	{
		this->outputFileName = "ROOTFile_SysUnc_BkgEst.root";

		this->h_RelUnc_ttbar_Stat = Get_Hist( this->fileName, "h_RelUnc_ttbar_Stat");
		this->h_RelUnc_ttbar_Syst = Get_Hist( this->fileName, "h_RelUnc_ttbar_Syst");
		this->h_RelUnc_ttbar_Tot = QuadSum_NoError( this->h_RelUnc_ttbar_Stat, this->h_RelUnc_ttbar_Syst );

		this->h_RelUnc_tW_Stat = Get_Hist( this->fileName, "h_RelUnc_tW_Stat");
		this->h_RelUnc_tW_Syst = Get_Hist( this->fileName, "h_RelUnc_tW_Syst");
		this->h_RelUnc_tW_Tot = QuadSum_NoError( this->h_RelUnc_tW_Stat, this->h_RelUnc_tW_Syst );

		this->h_RelUnc_DYtautau_Stat = Get_Hist( this->fileName, "h_RelUnc_DYtautau_Stat");
		this->h_RelUnc_DYtautau_Syst = Get_Hist( this->fileName, "h_RelUnc_DYtautau_Syst");
		this->h_RelUnc_DYtautau_Tot = QuadSum_NoError( this->h_RelUnc_DYtautau_Stat, this->h_RelUnc_DYtautau_Syst );

		this->h_RelUnc_wjets_Stat = Get_Hist( this->fileName, "h_RelUnc_wjets_Stat");
		this->h_RelUnc_wjets_Syst = Get_Hist( this->fileName, "h_RelUnc_wjets_Syst");
		this->h_RelUnc_wjets_Tot = QuadSum_NoError( this->h_RelUnc_wjets_Stat, this->h_RelUnc_wjets_Syst );

		this->h_RelUnc_dijet_Stat = Get_Hist( this->fileName, "h_RelUnc_dijet_Stat");
		this->h_RelUnc_dijet_Syst = Get_Hist( this->fileName, "h_RelUnc_dijet_Syst");
		this->h_RelUnc_dijet_Tot = QuadSum_NoError( this->h_RelUnc_dijet_Stat, this->h_RelUnc_dijet_Syst );

		this->h_RelUnc_WW_Stat = Get_Hist( this->fileName, "h_RelUnc_WW_Stat");
		this->h_RelUnc_WW_Syst = Get_Hist( this->fileName, "h_RelUnc_WW_Syst");
		this->h_RelUnc_WW_Tot = QuadSum_NoError( this->h_RelUnc_WW_Stat, this->h_RelUnc_WW_Syst );

		this->h_RelUnc_WZ_Stat = Get_Hist( this->fileName, "h_RelUnc_WZ_Stat");
		this->h_RelUnc_WZ_Syst = Get_Hist( this->fileName, "h_RelUnc_WZ_Syst");
		this->h_RelUnc_WZ_Tot = QuadSum_NoError( this->h_RelUnc_WZ_Stat, this->h_RelUnc_WZ_Syst );

		this->h_RelUnc_ZZ_Stat = Get_Hist( this->fileName, "h_RelUnc_ZZ_Stat");
		this->h_RelUnc_ZZ_Syst = Get_Hist( this->fileName, "h_RelUnc_ZZ_Syst");
		this->h_RelUnc_ZZ_Tot = QuadSum_NoError( this->h_RelUnc_ZZ_Stat, this->h_RelUnc_ZZ_Syst );

		this->h_RelUnc_ttbar_Stat->Scale( 100 );
		this->h_RelUnc_ttbar_Syst->Scale( 100 );
		this->h_RelUnc_ttbar_Tot->Scale( 100 );

		this->h_RelUnc_tW_Stat->Scale( 100 );
		this->h_RelUnc_tW_Syst->Scale( 100 );
		this->h_RelUnc_tW_Tot->Scale( 100 );

		this->h_RelUnc_DYtautau_Stat->Scale( 100 );
		this->h_RelUnc_DYtautau_Syst->Scale( 100 );
		this->h_RelUnc_DYtautau_Tot->Scale( 100 );

		this->h_RelUnc_wjets_Stat->Scale( 100 );
		this->h_RelUnc_wjets_Syst->Scale( 100 );
		this->h_RelUnc_wjets_Tot->Scale( 100 );

		this->h_RelUnc_dijet_Stat->Scale( 100 );
		this->h_RelUnc_dijet_Syst->Scale( 100 );
		this->h_RelUnc_dijet_Tot->Scale( 100 );

		this->h_RelUnc_WW_Stat->Scale( 100 );
		this->h_RelUnc_WW_Syst->Scale( 100 );
		this->h_RelUnc_WW_Tot->Scale( 100 );

		this->h_RelUnc_WZ_Stat->Scale( 100 );
		this->h_RelUnc_WZ_Syst->Scale( 100 );
		this->h_RelUnc_WZ_Tot->Scale( 100 );

		this->h_RelUnc_ZZ_Stat->Scale( 100 );
		this->h_RelUnc_ZZ_Syst->Scale( 100 );
		this->h_RelUnc_ZZ_Tot->Scale( 100 );

		vector<TH1D*> vec_Hists_emu_Stat =
		{
			this->h_RelUnc_ttbar_Stat,
			this->h_RelUnc_tW_Stat,
			this->h_RelUnc_DYtautau_Stat,
			this->h_RelUnc_WW_Stat
		};
		this->h_RelUnc_emu_Stat = this->MakeQuadSumHistogram( vec_Hists_emu_Stat );

		vector<TH1D*> vec_Hists_emu_Syst =
		{
			this->h_RelUnc_ttbar_Syst,
			this->h_RelUnc_tW_Syst,
			this->h_RelUnc_DYtautau_Syst,
			this->h_RelUnc_WW_Syst
		};
		this->h_RelUnc_emu_Syst = this->MakeQuadSumHistogram( vec_Hists_emu_Syst );

		vector<TH1D*> vec_Hists_emu_Tot =
		{
			this->h_RelUnc_ttbar_Tot,
			this->h_RelUnc_tW_Tot,
			this->h_RelUnc_DYtautau_Tot,
			this->h_RelUnc_WW_Tot
		};
		this->h_RelUnc_emu_Tot = this->MakeQuadSumHistogram( vec_Hists_emu_Tot );


		vector<TH1D*> vec_Hists_FR_Stat = { this->h_RelUnc_wjets_Stat, this->h_RelUnc_dijet_Stat };
		this->h_RelUnc_FR_Stat = this->MakeQuadSumHistogram( vec_Hists_FR_Stat );

		vector<TH1D*> vec_Hists_FR_Syst = { this->h_RelUnc_wjets_Syst, this->h_RelUnc_dijet_Syst };
		this->h_RelUnc_FR_Syst = this->MakeQuadSumHistogram( vec_Hists_FR_Syst );

		vector<TH1D*> vec_Hists_FR_Tot = { this->h_RelUnc_wjets_Tot, this->h_RelUnc_dijet_Tot };
		this->h_RelUnc_FR_Tot = this->MakeQuadSumHistogram( vec_Hists_FR_Tot );


		vector<TH1D*> vec_Hists_MC_Stat = { this->h_RelUnc_WZ_Stat, this->h_RelUnc_ZZ_Stat };
		this->h_RelUnc_MC_Stat = this->MakeQuadSumHistogram( vec_Hists_MC_Stat );

		vector<TH1D*> vec_Hists_MC_Syst = { this->h_RelUnc_WZ_Syst, this->h_RelUnc_ZZ_Syst };
		this->h_RelUnc_MC_Syst = this->MakeQuadSumHistogram( vec_Hists_MC_Syst );

		vector<TH1D*> vec_Hists_MC_Tot = { this->h_RelUnc_WZ_Tot, this->h_RelUnc_ZZ_Tot };
		this->h_RelUnc_MC_Tot = this->MakeQuadSumHistogram( vec_Hists_MC_Tot );


		vector<TH1D*> vec_Hist_All_Stat = { this->h_RelUnc_emu_Stat, this->h_RelUnc_FR_Stat, this->h_RelUnc_MC_Stat };
		this->h_RelUnc_Stat_Total = this->MakeQuadSumHistogram( vec_Hist_All_Stat );

		vector<TH1D*> vec_Hist_All_Syst = { this->h_RelUnc_emu_Syst, this->h_RelUnc_FR_Syst, this->h_RelUnc_MC_Syst };
		this->h_RelUnc_Syst_Total = this->MakeQuadSumHistogram( vec_Hist_All_Syst );

		vector<TH1D*> vec_Hist_All_Tot = { this->h_RelUnc_emu_Tot, this->h_RelUnc_FR_Tot, this->h_RelUnc_MC_Tot };
		this->h_RelUnc_Tot_Total = this->MakeQuadSumHistogram( vec_Hist_All_Tot );
	}

	void MakeCanvas_SysUnc( TString Type, vector< TH1D* > Histos, vector< TString > Names )
	{
		TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_"+Type, "", 800, 800);
		c_RelUnc->cd();

		gPad->SetTopMargin(0.05);
		gPad->SetBottomMargin(0.13);
		gPad->SetRightMargin(0.03);
		gPad->SetLeftMargin(0.13);

		gPad->SetLogx();
		gPad->SetGridx();
		gPad->SetGridy();

		Int_t Colors[6] = {kRed, kBlack, kBlue, kGreen+1, kOrange+1, kViolet};
		Int_t nHist = (Int_t)Histos.size();
		if( nHist > 6 )
		{
			cout << "The number of colors is smaller than the number of histograms!" << endl;
			return;
		}

		for(Int_t i_hist=0; i_hist<nHist; i_hist++)
		{
			TString DrawOption = "HISTLPSAME";
			if( i_hist == 0 )
				DrawOption = "HISTLP";
			Histos[i_hist]->Draw( DrawOption );
		}
		Histos[0]->Draw("HISTLPSAME");

		Histos[0]->GetXaxis()->SetLabelSize(0.04);
		Histos[0]->GetXaxis()->SetTitleSize(0.04);
		Histos[0]->GetXaxis()->SetNoExponent();
		Histos[0]->GetXaxis()->SetMoreLogLabels();
		Histos[0]->GetYaxis()->SetTitleOffset(1.4);

		Double_t yMax = 25;
		if( Type == "emu") yMax = 16;
		else if( Type == "FR") yMax = 3;
		else if( Type == "Total") yMax = 40;
		else if( Type == "TotalStatSyst") yMax = 40;
		else if( Type == "MC") yMax = 40;
		// else if( Type == "total" ) yMax = 80;

		Histos[0]->GetYaxis()->SetRangeUser(0, yMax);

		Histos[0]->SetXTitle( "m (#mu#mu) [GeV]");
		Histos[0]->SetYTitle( "Rel. Unc. (%)");

		Double_t yMinLegend = 0.75;
		TLegend *legend = new TLegend(0.15, yMinLegend, 0.55, 0.90);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		for(Int_t i_hist=0; i_hist<nHist; i_hist++)
		{
			Histos[i_hist]->SetStats(kFALSE);
			Histos[i_hist]->SetMarkerStyle(20);
			Histos[i_hist]->SetMarkerColor( Colors[i_hist] );
			Histos[i_hist]->SetLineColor( Colors[i_hist] );
			Histos[i_hist]->SetFillColorAlpha( kWhite, 0);

			legend->AddEntry( Histos[i_hist], Names[i_hist] );
		}
		legend->Draw();

		TLatex latex;

		latex.DrawLatexNDC(0.70, 0.96, "#font[42]{#scale[0.8]{2.8 fb^{-1} (13 TeV)}}" );
		latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
		latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

		TString plotInfo = "";
		if( Type == "ttbar" ) plotInfo = "t#bar{t}";
		if( Type == "tW" ) plotInfo = "tW+#bar{t}W";
		if( Type == "DYtautau" ) plotInfo = "Z/#gamma*#rightarrow#tau#tau";
		if( Type == "wjets" ) plotInfo = "W+jets";
		if( Type == "dijet" ) plotInfo = "QCD";
		if( Type == "WW" ) plotInfo = "WW";
		if( Type == "WZ" ) plotInfo = "WZ";
		if( Type == "ZZ" ) plotInfo = "ZZ";

		latex.DrawLatexNDC(0.16, 0.91, "#font[42]{#scale[0.6]{"+plotInfo+"}}" );

		c_RelUnc->SaveAs( ".pdf" );
	}

	TH1D* MakeQuadSumHistogram( vector<TH1D*> vec_Hists )
	{
		TH1D* h_combined = (TH1D*)vec_Hists[0]->Clone();

		Int_t nHist = (Int_t)vec_Hists.size();

		// -- combine uncertainty histograms: quadrature sum -- //
		Int_t nBin = h_combined->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t RelUnc_Sum2 = 0;
			for(Int_t i_hist=0; i_hist<nHist; i_hist++)
			{
				Double_t RelUnc_temp = vec_Hists[i_hist]->GetBinContent(i_bin);
				RelUnc_Sum2 = RelUnc_Sum2 + RelUnc_temp*RelUnc_temp;
			}

			Double_t RelUnc = sqrt( RelUnc_Sum2 );
			h_combined->SetBinContent(i_bin, RelUnc );
		}

		return h_combined;
	}
};

void Summary()
{
	TString inputFileName = "ROOTFile_RelUnc_BkgEst_CorrelationBtwBins.root";
	SummaryTool* tool = new SummaryTool( inputFileName );
	tool->Draw_All();

	TString outputFileName = "ROOTFile_SysUnc_BkgEst.root";
	TFile *f_output = TFile::Open("ROOTFile_SysUnc_BkgEst.root", "RECREATE");
	tool->Save( f_output );
	f_output->Close();
}

