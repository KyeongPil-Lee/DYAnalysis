#include <Include/PlotTools.h>

class Drawer
{
public:
	TString Type;
	TString ROOTFilePath;
	TString AnalyzerPath;

	TH1D* h_data;
	TH1D* h_DY;
	TH1D* h_DYPI;

	TH1D* h_RelTotUnc;
	TH1D* h_RelStatUnc;
	TH1D* h_RelTheoUnc;

	TH1D* h_Ratio_StatUnc;
	TH1D* h_Ratio_TotUnc;
	TH1D* h_Ratio_noPI;
	TH1D* h_Ratio_withPI;

	Drawer( TString _Type = "Muon" )
	{
		this->ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		this->AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

		this->Type = _Type;
		if( Type == "Muon" )
			this->Set_Histograms_Muon();
		else if( Type == "Electron" )
			this->Set_Histograms_Electron();
		else if( Type == "Combined" )
			this->Set_Histograms_Combined();
		else
			cout << "Type = " << Type << " is not proper type!" << endl;
	}

	void Draw()
	{
		this->Draw_Below200GeV();
		this->Draw_Above200GeV();
	}

	void Draw_Below200GeV()
	{
		TCanvas *c;
		TString CanvasName = this->Type+"_ratio_theoryData_Below200GeV";
		SetCanvas_Square( c, CanvasName, 1, 0 );

		TH1D* h_TotUnc_temp = this->Rebin_Below200GeV( this->h_Ratio_TotUnc );
		TH1D* h_StatUnc_temp = this->Rebin_Below200GeV( this->h_Ratio_StatUnc );
		// TH1D* h_ratio_withPI_temp = this->Rebin_Below200GeV( this->h_Ratio_withPI );
		TH1D* h_ratio_noPI_temp = this->Rebin_Below200GeV( this->h_Ratio_noPI );

		h_TotUnc_temp->Draw("E2PSAME");
		h_StatUnc_temp->Draw("E1PSAME");
		// h_ratio_withPI_temp->Draw("EPSAME");
		h_ratio_noPI_temp->Draw("E2SAME");

		h_TotUnc_temp->SetTitle("");
		SetHistFormat_SinglePad(h_TotUnc_temp, "m [GeV]", "Theory/Data");
		h_TotUnc_temp->GetYaxis()->SetRangeUser(0.7, 1.3);

		TLegend *legend;
		SetLegend( legend, 0.15, 0.15, 0.55, 0.27 );
		legend->SetNColumns( 2 );
		legend->AddEntry( h_StatUnc_temp, "Stat. Unc.");
		legend->AddEntry( h_TotUnc_temp, "Tot. Unc.");
		legend->AddEntry( h_ratio_noPI_temp, "Theory(w/o PI)/Data");
		// legend->AddEntry( h_ratio_withPI_temp, "Theory(w/ PI)/Data");
		legend->Draw();

		TLatex latex;
		DrawLatexNDC( latex );

		c->SaveAs(".pdf");
		TString PlotPath = gSystem->Getenv("KP_PLOT_PATH");
		c->SaveAs(PlotPath+"/"+CanvasName+".pdf");

	}

	void Draw_Above200GeV()
	{
		TCanvas *c;
		TString CanvasName = this->Type+"_ratio_theoryData_Above200GeV";
		SetCanvas_Square( c, CanvasName, 1, 0 );

		TH1D* h_TotUnc_temp = this->Rebin_Above200GeV( this->h_Ratio_TotUnc );
		TH1D* h_StatUnc_temp = this->Rebin_Above200GeV( this->h_Ratio_StatUnc );
		TH1D* h_ratio_withPI_temp = this->Rebin_Above200GeV( this->h_Ratio_withPI );
		TH1D* h_ratio_noPI_temp = this->Rebin_Above200GeV( this->h_Ratio_noPI );

		h_TotUnc_temp->Draw("E2PSAME");
		h_StatUnc_temp->Draw("E1PSAME");
		h_ratio_withPI_temp->Draw("EPSAME");
		h_ratio_noPI_temp->Draw("E2SAME");

		h_TotUnc_temp->SetTitle("");
		SetHistFormat_SinglePad(h_TotUnc_temp, "m [GeV]", "Theory/Data");
		h_TotUnc_temp->GetYaxis()->SetRangeUser(0.0, 3.0);

		TLegend *legend;
		SetLegend( legend, 0.15, 0.15, 0.55, 0.27 );
		legend->SetNColumns( 2 );
		legend->AddEntry( h_StatUnc_temp, "Stat. Unc.");
		legend->AddEntry( h_TotUnc_temp, "Tot. Unc.");
		legend->AddEntry( h_ratio_noPI_temp, "Theory(w/o PI)/Data");
		legend->AddEntry( h_ratio_withPI_temp, "Theory(w/ PI)/Data");
		legend->Draw();

		TLatex latex;
		DrawLatexNDC( latex );

		c->SaveAs(".pdf");
		TString PlotPath = gSystem->Getenv("KP_PLOT_PATH");
		c->SaveAs(PlotPath+"/"+CanvasName+".pdf");
	}
protected:
	void Set_Histograms_Theory()
	{
		TString FileName_DY = ROOTFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
		// TString FileName_DYPI = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYXSecNNLO_WithPI.root"; // -- MRST2004qed -- //
		TString FileName_DYPI = AnalyzerPath + "/TheoryValues/NLO_LUXqed_DYPI_PI/ROOTFile_DYXSecNNLO_WithPI.root"; // -- LUX qed -- //

		this->h_DY = Get_Hist(FileName_DY, "h_DiffXsec_FEWZ_NNPDF_NNLO");
		this->h_DYPI = Get_Hist(FileName_DYPI, "h_DYDiffXSecNNLO_WithPI");
		this->h_RelTheoUnc = Extract_RelUnc( h_DY, "h_RelTheoUnc_DYwithoutPI");
	}

	void Set_Histograms_Muon()
	{
		this->Set_Histograms_Theory();

		TString FileName_Data = ROOTFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
		this->h_data = Get_Hist(FileName_Data, "h_DiffXsec_FSRCorr");
		this->h_RelTotUnc = Extract_RelUnc( h_data, "h_RelTotUnc" );
		this->h_RelStatUnc = Get_Hist( FileName_Data, "h_RelStatUnc_Percent" ); this->h_RelStatUnc->Scale( 1.0 / 100 );
		this->Set_Histograms_Ratio();
	}

	void Set_Histograms_Electron()
	{
		this->Set_Histograms_Theory();

		TString FileName_Data = ROOTFilePath + "/DiffXsec_Electron_v7.root";
		this->h_data = Get_Hist(FileName_Data, "h_DiffXSec");
		this->h_RelStatUnc = Get_Hist( FileName_Data, "h_RelUnc_Stat" ); this->h_RelStatUnc->Scale( 1.0 / 100 );

		// this->h_RelTotUnc = Get_Hist( FileName_Data, "h_RelUnc_Total" ); this->h_RelTotUnc->Scale( 1.0 / 100 );
		TH1D* h_RelSystUnc = Get_Hist( FileName_Data, "h_RelUnc_Syst" ); h_RelSystUnc->Scale(0.01);
		this->h_RelTotUnc = QuadSum_NoError( this->h_RelStatUnc, h_RelSystUnc );
		TH1D* h_RelLumiUnc = this->MakeHist_RelLumiUnc( 0.023 );
		this->h_RelTotUnc = QuadSum_NoError( this->h_RelTotUnc, h_RelLumiUnc ); // -- add up lumi. uncertainty -- //

		this->Set_Histograms_Ratio();
	}

	void Set_Histograms_Combined()
	{
		this->Set_Histograms_Theory();
		// -- fill -- //

		TString FileName_CenV = this->ROOTFilePath + "/dyll-combi-_corr_wLumi_inpYieldUnc_v3a.root";
		// -- convert TH1F to TH1D -- //
		TFile *f_input = TFile::Open( FileName_CenV );
		f_input->cd();
		TH1F* h_temp = (TH1F*)f_input->Get( "h1Combi" )->Clone();
		this->h_data = new TH1D();
		h_temp->Copy( *this->h_data );
		this->h_data->SetTitle("");
		///////////////////////////////////////////

		TH1D* h_AbsStatUnc = Get_Hist( FileName_CenV, "h1_dCS_from_statYield" );
		this->h_RelStatUnc = ConvertHist_AbsToRel( this->h_data, h_AbsStatUnc );
		this->h_RelTotUnc = Extract_RelUnc( this->h_data );
		
		////////////////
		this->Set_Histograms_Ratio();
	}

	void Set_Histograms_Ratio()
	{
		this->h_Ratio_StatUnc = (TH1D*)h_data->Clone();
		this->h_Ratio_StatUnc->Divide( this->h_Ratio_StatUnc, this->h_Ratio_StatUnc ); // -- ratio = 1 line -- //
		AssignErrors( this->h_Ratio_StatUnc, this->h_RelStatUnc );
		this->h_Ratio_StatUnc->SetMarkerStyle(20);
		this->h_Ratio_StatUnc->SetMarkerColor(kBlack);
		this->h_Ratio_StatUnc->SetLineColor(kBlack);

		this->h_Ratio_TotUnc = (TH1D*)this->h_Ratio_StatUnc->Clone();
		AssignErrors( this->h_Ratio_TotUnc, h_RelTotUnc );
		this->h_Ratio_TotUnc->SetMarkerColorAlpha(kWhite, 0);
		this->h_Ratio_TotUnc->SetLineColorAlpha(kWhite, 0);
		this->h_Ratio_TotUnc->SetFillColorAlpha( kGray+1, 1 );
		this->h_Ratio_TotUnc->SetFillStyle( 3354 );

		this->h_Ratio_noPI = (TH1D*)this->h_DY->Clone();
		this->h_Ratio_noPI->Divide( this->h_DY, this->h_data ); 
		AssignErrors( this->h_Ratio_noPI, h_RelTheoUnc ); // -- theory / data, only theoretical uncertainty -- //
		// this->h_Ratio_noPI->SetMarkerColorAlpha(kWhite, 0);
		this->h_Ratio_noPI->SetMarkerColorAlpha(kWhite, 0);
		this->h_Ratio_noPI->SetLineColorAlpha(kWhite, 0);
		this->h_Ratio_noPI->SetFillColorAlpha( kBlue, 0.3 );
		
		this->h_Ratio_withPI = (TH1D*)this->h_DYPI->Clone();
		this->h_Ratio_withPI->Divide( this->h_DYPI, this->h_data );
		AssignErrors_NoError( this->h_Ratio_withPI ); // -- theory/data, uncertainty will not be displayed -- //
		this->h_Ratio_withPI->SetMarkerStyle(20);
		this->h_Ratio_withPI->SetMarkerSize(0);
		this->h_Ratio_withPI->SetLineColor(kRed);
		this->h_Ratio_withPI->SetLineStyle( 2 );
		this->h_Ratio_withPI->SetLineWidth( 3 );
		this->h_Ratio_withPI->SetFillColorAlpha(kWhite, 0);
	}

	void AssignErrors_NoError( TH1D* h )
	{
		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			h->SetBinError(i_bin, 1e-30);
		}
	}

	TH1D* Rebin_Below200GeV( TH1D* h )
	{
		const Int_t _nMassBin = 30;
		Double_t MassBinEdges[_nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200};

		return (TH1D*)h->Rebin(_nMassBin, h->GetName(), MassBinEdges);
	}

	TH1D* Rebin_Above200GeV( TH1D* h )
	{
		const Int_t _nMassBin = 13;
		Double_t MassBinEdges[_nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

		return (TH1D*)h->Rebin(_nMassBin, h->GetName(), MassBinEdges);
	}

	void DrawLatexNDC( TLatex &latex )
	{
		Double_t lumi_MM = 2.8;
		Double_t lumi_EE = 2.3;

		latex.DrawLatexNDC(0.14, 0.96, "#font[62]{#scale[0.8]{CMS Preliminary}}");

		if( this->Type == "Muon" )
		{
			latex.DrawLatexNDC(0.70, 0.96, TString::Format("#font[42]{#scale[0.8]{%.1lf fb^{-1} (13 TeV)}}", lumi_MM ) );
			latex.DrawLatexNDC(0.16, 0.90, "#font[42]{#scale[0.9]{Z/#gamma* #rightarrow #mu^{+}#mu^{-}}}");
		}
		else if( this->Type == "Electron" )
		{
			latex.DrawLatexNDC(0.70, 0.96, TString::Format("#font[42]{#scale[0.8]{%.1lf fb^{-1} (13 TeV)}}", lumi_EE ) );
			latex.DrawLatexNDC(0.16, 0.90, "#font[42]{#scale[0.9]{Z/#gamma* #rightarrow e^{+}e^{-}}}");
		}
		else if( this->Type == "Combined" )
		{
			latex.DrawLatexNDC(0.48, 0.96, TString::Format("#font[42]{#scale[0.7]{%.1lf fb^{-1} (ee) %.1lf fb^{-1} (#mu#mu)} #scale[0.8]{(13 TeV)}}", lumi_EE, lumi_MM) );
			latex.DrawLatexNDC(0.16, 0.90, "#font[42]{#scale[0.9]{Z/#gamma* #rightarrow e^{+}e^{-}, #mu^{+}#mu^{-}}}");
		}
	}

	TH1D* MakeHist_RelLumiUnc( Double_t RelLumiUnc )
	{
		if( this->h_data == NULL )
		{
			cout << "[MakeHist_RelLumiUnc] h_data should be set first (to have same binning)" << endl;
			return NULL;
		}

		TH1D* h_RelLumiUnc = (TH1D*)this->h_data->Clone();
		Int_t nBin = h_RelLumiUnc->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			h_RelLumiUnc->SetBinContent(i_bin, RelLumiUnc );
			h_RelLumiUnc->SetBinError(i_bin, 0);
		}

		return h_RelLumiUnc;
	}
};