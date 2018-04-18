#include <Include/PlotTools.h>

class DrawingTool
{
public:
	TH1D* h_leadPt_M15to20;
	TH1D* h_subPt_M15to20;

	TH1D* h_leadPt_M25to30;
	TH1D* h_subPt_M25to30;

	Int_t nRebin;

	DrawingTool()
	{
		this->Init();
	}

	void DrawAll()
	{
		this->Draw( h_leadPt_M15to20, h_leadPt_M25to30, "leadPt" );
		this->Draw( h_subPt_M15to20, h_subPt_M25to30, "subPt" );
	}

private:
	void Draw(TH1D* h_M15to20, TH1D* h_M25to30, TString type )
	{
		// -- Draw canvas
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString canvasName = "Local/c_"+type;
		SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 0, 0 );

		Double_t mean_M15to20 = h_M15to20->GetMean();
		Double_t mean_M25to30 = h_M25to30->GetMean();

		h_M15to20->Scale( 1.0 / h_M15to20->Integral() );
		h_M25to30->Scale( 1.0 / h_M25to30->Integral() );

		c->cd();
		TopPad->cd();

		h_M15to20->Draw("EPSAME");
		h_M25to30->Draw("EPSAME");

		h_M15to20->SetStats(kFALSE);
		h_M15to20->SetMarkerStyle(20);
		h_M15to20->SetMarkerColor(kBlack);
		h_M15to20->SetLineColor(kBlack);
		h_M15to20->SetFillColorAlpha(kWhite, 0); 
		h_M15to20->SetTitle("");

		h_M25to30->SetStats(kFALSE);
		h_M25to30->SetMarkerStyle(20);
		h_M25to30->SetMarkerColor(kRed);
		h_M25to30->SetLineColor(kRed);
		h_M25to30->SetFillColorAlpha(kWhite, 0); 
		h_M25to30->SetTitle("");

		SetHistFormat_TopPad( h_M15to20, TString::Format("Entry (norm. to 1) / %d GeV", this->nRebin) );
		if( type == "leadPt" ) 	h_M15to20->GetXaxis()->SetRangeUser(20, 50);
		if( type == "subPt" ) 	h_M15to20->GetXaxis()->SetRangeUser(5, 35);
		h_M15to20->GetYaxis()->SetRangeUser(0, 0.2);


		TLegend *legend;
		SetLegend( legend, 0.45, 0.82, 0.95, 0.95 );
		legend->AddEntry( h_M15to20, TString::Format("15 < M < 20 GeV (mean P_{T} = %.2lf GeV)", mean_M15to20) );
		legend->AddEntry( h_M25to30, TString::Format("25 < M < 30 GeV (mean P_{T} = %.2lf GeV)", mean_M25to30) );
		legend->Draw();

		TLatex latex;
		TString leptonInfo = "";
		if( type == "leadPt" ) leptonInfo = "Leading lepton P_{T}";
		if( type == "subPt" ) leptonInfo = "Sub-leading lepton P_{T}";
		latex.DrawLatexNDC(0.16, 0.91, "#font[42]{#scale[0.6]{"+leptonInfo+"}}");

		c->cd();
		BottomPad->cd();

		TH1D* h_ratio = (TH1D*)h_M15to20->Clone("h_ratio");
		h_ratio->Divide( h_M25to30, h_M15to20 );

		h_ratio->Draw("EPSAME");

		SetHistFormat_BottomPad( h_ratio, "P_{T}(#mu) [GeV]", "Red/Black", 0.4, 1.6 );

		TF1 *f_line;
		DrawLine(f_line);

		// Print_Histogram( h_ratio );

		c->SaveAs(".pdf");
	}

	void Init()
	{
		TString rootFileName = "./Local/ROOTFile_PtSpectrum_massBin.root";
		this->h_leadPt_M15to20 = Get_Hist(rootFileName, "h_leadPt_M15to20" );
		this->h_subPt_M15to20 = Get_Hist(rootFileName, "h_subPt_M15to20" );

		this->h_leadPt_M25to30 = Get_Hist(rootFileName, "h_leadPt_M25to30" );
		this->h_subPt_M25to30 = Get_Hist(rootFileName, "h_subPt_M25to30" );

		this->nRebin = 1;

		h_leadPt_M15to20->Rebin( this->nRebin );
		h_leadPt_M25to30->Rebin( this->nRebin );

		h_subPt_M15to20->Rebin( this->nRebin );
		h_subPt_M25to30->Rebin( this->nRebin );

	}
};

void DrawPlot()
{
	DrawingTool tool;
	tool.DrawAll();
}