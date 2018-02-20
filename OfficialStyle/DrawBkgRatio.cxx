#include <Include/PlotTools.h>

class DrawingTool
{
public:
	TString channel;

	// TH1D* h_data;
	// TH1D* h_fakes;
	// TH1D* h_ttbarLike; // -- ttbar, tW, WW -- //
	// TH1D* h_ZZWZ;
	// TH1D* h_DYtautau;
	// TH1D* h_totBkg;

	TH1D* h_ratio_fakes;
	TH1D* h_ratio_ttbarLike;
	TH1D* h_ratio_ZZWZ;
	TH1D* h_ratio_DYtautau;
	TH1D* h_ratio_totBkg;

	DrawingTool( TString _channel )
	{
		this->channel = _channel;
		this->Init();
	}

	void Draw()
	{
		// -- Draw canvas -- //
		HistInfo *Hist_totBkg = new HistInfo( kBlack, "Total", h_ratio_totBkg );
		Hist_totBkg->h->SetMarkerStyle( 20 );
		Hist_totBkg->h->SetMarkerSize( 1 );

		HistInfo *Hist_fakes = new HistInfo( kMagenta+2, "QCD + WJets", h_ratio_fakes );
		Hist_fakes->h->SetMarkerStyle( 21 );
		Hist_fakes->h->SetMarkerSize( 1 );

		HistInfo *Hist_ttbarLike = new HistInfo( kRed, "t#bar{t} + tW + WW", h_ratio_ttbarLike );
		Hist_ttbarLike->h->SetMarkerStyle(22);
		Hist_ttbarLike->h->SetMarkerSize( 1 );

		HistInfo *Hist_ZZWZ = new HistInfo( kGreen, "ZZ + WZ", h_ratio_ZZWZ );
		Hist_ZZWZ->h->SetMarkerStyle(23);
		Hist_ZZWZ->h->SetMarkerSize( 1 );

		HistInfo *Hist_DYtautau = new HistInfo( kBlue-9, "DY#rightarrow#tau#tau", h_ratio_DYtautau );
		Hist_DYtautau->h->SetMarkerStyle(29);
		Hist_DYtautau->h->SetMarkerSize( 1 );

		TString CanvasName = "Local/"+this->channel+"_BkgRatio";
		TCanvas *c;
		SetCanvas_Square(c, CanvasName, 1, 1, 800, 700);
		c->cd();

		Hist_totBkg->Draw("EPSAME");
		Hist_fakes->Draw("EPSAME");
		Hist_ttbarLike->Draw("EPSAME");
		Hist_ZZWZ->Draw("EPSAME");
		Hist_DYtautau->Draw("EPSAME");

		TLegend *legend;
		SetLegend(legend, 0.7, 0.15, 0.95, 0.4);
		Hist_totBkg->AddToLegend ( legend );
		Hist_fakes->AddToLegend ( legend );
		Hist_ttbarLike->AddToLegend ( legend );
		Hist_ZZWZ->AddToLegend ( legend );
		Hist_DYtautau->AddToLegend ( legend );
		legend->Draw();

		SetHistFormat_SinglePad( Hist_totBkg->h, "m [GeV]", "Fraction of backgrounds" );
		Hist_totBkg->h->GetYaxis()->SetRangeUser(1e-5, 1);

		TLatex latex;
		latex.DrawLatexNDC(0.14, 0.96, "#scale[0.8]{#font[62]{CMS }#font[42]{#it{Supplementary}}}");
		if( this->channel == "Muon" )
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");
		else if( this->channel == "Electron" )
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (13 TeV)}}");

		c->SaveAs(".pdf");

		// Print_Histogram( Hist_totBkg->h );
	}

private:
	void Init()
	{
		TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileName = "";
		if( this->channel == "Muon" )
		{
			fileName = rootFilePath+"/ROOTFile_Histograms_DimuonMassSpectrum_All.root";
		}
		else if( this->channel == "Electron" )
		{
			fileName = rootFilePath+"/ROOTFile_Histograms_DielectronMassSpectrum.root";
		}

		TH1D* h_data = Get_Hist( fileName, "h_data" );
		h_data->Sumw2();

		// -- h_fakes -- //
		TH1D* h_WJets = Get_Hist( fileName, "h_WJets_FR" );
		h_WJets->Sumw2();
		TH1D* h_QCD = Get_Hist( fileName, "h_diJet_FR" );
		h_QCD->Sumw2();
		TH1D* h_fakes = (TH1D*)h_WJets->Clone();
		h_fakes->Add( h_QCD );

		// -- h_ttbarLike -- //
		TH1D* h_ttbar = Get_Hist( fileName, "h_ttbar_emu" );
		h_ttbar->Sumw2();
		TH1D* h_tW_emu = Get_Hist( fileName, "h_tW_emu");
		h_tW_emu->Sumw2();
		TH1D* h_WW = Get_Hist( fileName, "h_WW_emu" );
		h_WW->Sumw2();
		this->HotFix_ElectronChannel( h_tW_emu, h_WW );

		TH1D* h_ttbarLike = (TH1D*)h_ttbar->Clone();
		h_ttbarLike->Add( h_tW_emu );
		h_ttbarLike->Add( h_WW );

		// cout << "h_ttbar" << endl;
		// Print_Histogram( h_ttbar );

		// cout << "h_tW_emu" << endl;
		// Print_Histogram( h_tW_emu );

		// cout << "h_WW" << endl;
		// Print_Histogram( h_WW );

		// -- h_ZZWZ -- //
		TH1D* h_WZ = Get_Hist( fileName, "h_WZ" );
		h_WZ->Sumw2();
		TH1D* h_ZZ = Get_Hist( fileName, "h_ZZ" );
		h_ZZ->Sumw2();
		TH1D* h_ZZWZ = (TH1D*)h_WZ->Clone();
		h_ZZWZ->Add( h_ZZ );

		// -- DY->tautau -- //
		TH1D* h_DYtautau = Get_Hist( fileName, "h_DYTauTau_emu" );
		h_DYtautau->Sumw2();


		// -- total bkg. -- //
		TH1D* h_totBkg = (TH1D*)h_fakes->Clone();
		h_totBkg->Add( h_ttbarLike );
		h_totBkg->Add( h_ZZWZ );
		h_totBkg->Add( h_DYtautau );

		// cout << "h_ttbarLike" << endl;
		// Print_Histogram( h_ttbarLike );

		// cout << "h_ZZWZ" << endl;
		// Print_Histogram( h_ZZWZ );

		// cout << "h_DYtautau" << endl;
		// Print_Histogram( h_DYtautau );

		// -- bkg. ratio -- //
		this->h_ratio_totBkg = (TH1D*)h_data->Clone();
		h_ratio_totBkg->Divide( h_totBkg, h_data );

		// cout << "h_data" << endl;
		// Print_Histogram( h_data );

		// cout << "h_totBkg" << endl;
		// Print_Histogram( h_totBkg );

		// cout << "h_ratio_totBkg" << endl;
		// Print_Histogram( h_ratio_totBkg );

		this->h_ratio_fakes = (TH1D*)h_data->Clone();
		h_ratio_fakes->Divide( h_fakes, h_data );

		this->h_ratio_ttbarLike = (TH1D*)h_data->Clone();
		h_ratio_ttbarLike->Divide( h_ttbarLike, h_data );

		this->h_ratio_ZZWZ = (TH1D*)h_data->Clone();
		h_ratio_ZZWZ->Divide( h_ZZWZ, h_data );

		this->h_ratio_DYtautau = (TH1D*)h_data->Clone();
		h_ratio_DYtautau->Divide( h_DYtautau, h_data );
	}

	void HotFix_ElectronChannel( TH1D* h_tW_emu, TH1D* h_WW )
	{
		if( this->channel == "Electron" )
		{
			h_tW_emu->SetBinError(42, 0 );
			h_WW->SetBinError(42, 0 );
		}
	}

};

void DrawBkgRatio()
{
	DrawingTool* tool_Muon = new DrawingTool( "Muon" );
	tool_Muon->Draw();

	DrawingTool* tool_Electron = new DrawingTool( "Electron" );
	tool_Electron->Draw();
}