#include <Include/PlotTools.h>

class Tool_DrawRelUnc
{
public:
	TString channel;
	TCanvas *c;

	TString FileLocation;

	HistInfo* Hist_RelStatUnc;
	HistInfo* Hist_RelSystUnc_Tot;
	HistInfo* Hist_RelSystUnc_EffSF;
	HistInfo* Hist_RelSystUnc_DetRes;
	HistInfo* Hist_RelSystUnc_Bkg;
	HistInfo* Hist_RelSystUnc_FSR;
	HistInfo* Hist_RelTheoUnc_Acc;
	HistInfo* Hist_RelLumiUnc;

	Tool_DrawRelUnc( TString _channel )
	{
		this->channel = _channel;
		this->FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");
	}
	

	void DrawCanvas()
	{
		this->Get_Histograms();

		TString CanvasName = this->channel+"_SysUnc_All";
		this->c = new TCanvas(CanvasName, "",0,0,800,700);
		this->Setup_Canvas();
		this->c->cd();

		this->Hist_RelStatUnc->h->Draw("HISTLP");
		this->Hist_RelLumiUnc->h->Draw("HISTLPSAME");
		this->Hist_RelTheoUnc_Acc->h->Draw("HISTLPSAME");
		this->Hist_RelSystUnc_EffSF->h->Draw("HISTLPSAME");
		this->Hist_RelSystUnc_DetRes->h->Draw("HISTLPSAME");
		this->Hist_RelSystUnc_Bkg->h->Draw("HISTLPSAME");
		this->Hist_RelSystUnc_FSR->h->Draw("HISTLPSAME");
		this->Hist_RelSystUnc_Tot->h->Draw("HISTLPSAME");

		this->SetAxisFormat( this->Hist_RelStatUnc->h );

		// Print_Histogram( Hist_RelStatUnc->h, kTRUE );
		// Print_Histogram( Hist_RelLumiUnc->h, kTRUE );
		// Print_Histogram( Hist_RelTheoUnc_Acc->h, kTRUE );
		// Print_Histogram( Hist_RelSystUnc_EffSF->h, kTRUE );
		// Print_Histogram( Hist_RelSystUnc_DetRes->h, kTRUE );
		// Print_Histogram( Hist_RelSystUnc_Bkg->h, kTRUE );
		// Print_Histogram( Hist_RelSystUnc_FSR->h, kTRUE );
		// Print_Histogram( Hist_RelSystUnc_Tot->h, kTRUE );

		TLegend *legend1 = this->Get_Legend1();
		legend1->Draw();

		TLegend *legend2 = this->Get_Legend2();
		legend2->Draw();

		TLatex latex;
		this->DrawLatex( latex );

		c->SaveAs("Local/"+CanvasName+".pdf");

		TString PlotPath = gSystem->Getenv("KP_PLOT_PATH");
		c->SaveAs(PlotPath+"/"+CanvasName+".pdf");
	}
protected:
	void Get_Histograms()
	{
		TString FileName = "";
		if( this->channel == "Muon" )
			FileName = FileLocation + "/ROOTFile_DiffXSec_FullUnc.root";
		else if( this->channel == "Electron" )
			FileName = FileLocation + "/ROOTFile_RelUnc_All_Electron.root";

		TString HistName_RelStatUnc = "h_RelStatUnc_Percent";
		TString HistName_RelSystUnc_Tot = "h_RelSysUnc_Tot_Percent";
		TString HistName_RelSystUnc_EffSF = "h_RelSysUnc_Eff.SF._Percent";
		TString HistName_RelSystUnc_DetRes = "h_RelSysUnc_Det.Res_Percent";
		TString HistName_RelSystUnc_Bkg = "h_RelSysUnc_Bkg.Est._Percent";
		TString HistName_RelSystUnc_FSR = "h_RelSysUnc_FSR_Percent";
		TString HistName_RelTheoUnc_Acc = "h_RelSysUnc_Acc._Percent";
		TString HistName_RelLumiUnc = "h_RelLumiUnc_Percent";

		// -- stat. unc -- //
		TH1D* h_RelStatUnc = Get_Hist( FileName, HistName_RelStatUnc );
		this->Hist_RelStatUnc = new HistInfo(kBlack, "Statistical", h_RelStatUnc );
		this->Hist_RelStatUnc->h->SetMarkerStyle(20);
		this->Hist_RelStatUnc->h->SetMarkerSize(1);

		TH1D* h_RelSystUnc_Tot = Get_Hist( FileName, HistName_RelSystUnc_Tot );
		this->Hist_RelSystUnc_Tot = new HistInfo(kRed, "Total Systematic", h_RelSystUnc_Tot );
		this->Hist_RelSystUnc_Tot->h->SetMarkerStyle(kFullSquare);
		this->Hist_RelSystUnc_Tot->h->SetMarkerSize(1);

		TH1D* h_RelSystUnc_EffSF = Get_Hist( FileName, HistName_RelSystUnc_EffSF );
		this->Hist_RelSystUnc_EffSF = new HistInfo( TColor::GetColor("#0000ff"), "Efficiency SF", h_RelSystUnc_EffSF );
		this->Hist_RelSystUnc_EffSF->h->SetMarkerStyle(25);
		this->Hist_RelSystUnc_EffSF->h->SetMarkerSize(1);

		TH1D* h_RelSystUnc_DetRes = Get_Hist( FileName, HistName_RelSystUnc_DetRes );
		this->Hist_RelSystUnc_DetRes = new HistInfo( TColor::GetColor("#cc00ff"), "Detector Res.", h_RelSystUnc_DetRes );
		this->Hist_RelSystUnc_DetRes->h->SetMarkerStyle(26);
		this->Hist_RelSystUnc_DetRes->h->SetMarkerSize(1);

		TH1D* h_RelSystUnc_Bkg = Get_Hist( FileName, HistName_RelSystUnc_Bkg );
		this->Hist_RelSystUnc_Bkg = new HistInfo( TColor::GetColor("#00cc00"), "Background", h_RelSystUnc_Bkg );
		this->Hist_RelSystUnc_Bkg->h->SetMarkerStyle(27);
		this->Hist_RelSystUnc_Bkg->h->SetMarkerSize(1);
		this->Hist_RelSystUnc_Bkg->h->SetMinimum(0.001);

		TH1D* h_RelSystUnc_FSR = Get_Hist( FileName, HistName_RelSystUnc_FSR );
		this->Hist_RelSystUnc_FSR = new HistInfo( TColor::GetColor("#9999ff"), "FSR", h_RelSystUnc_FSR );
		this->Hist_RelSystUnc_FSR->h->SetMarkerStyle(28);
		this->Hist_RelSystUnc_FSR->h->SetMarkerSize(1);

		TH1D* h_RelTheoUnc_Acc = Get_Hist( FileName, HistName_RelTheoUnc_Acc );
		this->Hist_RelTheoUnc_Acc = new HistInfo( TColor::GetColor("#99ff99"), "Acceptance", h_RelTheoUnc_Acc );
		this->Hist_RelTheoUnc_Acc->h->SetMarkerStyle(24);
		this->Hist_RelTheoUnc_Acc->h->SetMarkerSize(1);

		TH1D* h_RelLumiUnc = Get_Hist( FileName, HistName_RelLumiUnc );
		this->Hist_RelLumiUnc = new HistInfo( TColor::GetColor("#ff9933"), "Luminosity", h_RelLumiUnc );
		this->Hist_RelLumiUnc->h->SetMarkerStyle(22);
		this->Hist_RelLumiUnc->h->SetMarkerSize(1);
	}

	void Setup_Canvas()
	{
		this->c->Range(0.7446481,-1.71506,3.620936,2.425239);
		this->c->SetFillColor(0);
		this->c->SetBorderMode(0);
		this->c->SetBorderSize(2);
		this->c->SetLogx();
		this->c->SetLogy();
		this->c->SetLeftMargin(0.12);
		this->c->SetRightMargin(0.05);
		this->c->SetTopMargin(0.08);
		this->c->SetBottomMargin(0.15);
		this->c->SetFrameBorderMode(0);
		this->c->SetFrameBorderMode(0);
	}

	TLegend* Get_Legend1()
	{
		TLegend *legtot = new TLegend(0.15,0.83,0.40,0.91,NULL,"brNDC");
		legtot->SetBorderSize(0);
		legtot->SetLineColor(1);
		legtot->SetLineStyle(1);
		legtot->SetLineWidth(1);
		legtot->SetFillColor(0);
		legtot->SetFillStyle(0);
		legtot->AddEntry( this->Hist_RelStatUnc->h, this->Hist_RelStatUnc->LegendName, "lpf" );
		legtot->AddEntry( this->Hist_RelLumiUnc->h, this->Hist_RelLumiUnc->LegendName, "lpf" );

		return legtot;
	}

	TLegend* Get_Legend2()
	{
		TLegend *leg = new TLegend(0.35,0.67,0.60,0.91,NULL,"brNDC");
		leg->SetBorderSize(0);
		leg->SetLineColor(1);
		leg->SetLineStyle(1);
		leg->SetLineWidth(1);
		leg->SetFillColor(0);
		leg->SetFillStyle(0);
		leg->AddEntry( this->Hist_RelSystUnc_Tot->h, this->Hist_RelSystUnc_Tot->LegendName,"lpf");
		leg->AddEntry( this->Hist_RelTheoUnc_Acc->h, this->Hist_RelTheoUnc_Acc->LegendName,"lpf");
		leg->AddEntry( this->Hist_RelSystUnc_EffSF->h, this->Hist_RelSystUnc_EffSF->LegendName,"lpf");
		leg->AddEntry( this->Hist_RelSystUnc_DetRes->h, this->Hist_RelSystUnc_DetRes->LegendName,"lpf");
		leg->AddEntry( this->Hist_RelSystUnc_Bkg->h, this->Hist_RelSystUnc_Bkg->LegendName,"lpf");
		leg->AddEntry( this->Hist_RelSystUnc_FSR->h, this->Hist_RelSystUnc_FSR->LegendName,"lpf");

		return leg;
	}

	void SetAxisFormat( TH1D* h )
	{
		h->GetXaxis()->SetTitle("m [GeV]");
		h->GetXaxis()->SetRange(1,43);
		h->GetXaxis()->SetMoreLogLabels();
		h->GetXaxis()->SetNoExponent();
		h->GetXaxis()->SetLabelFont(42);
		h->GetXaxis()->SetLabelOffset(0.007);
		h->GetXaxis()->SetLabelSize(0.05);
		h->GetXaxis()->SetTitleOffset(0.95);
		h->GetXaxis()->SetTitleSize(0.06);
		h->GetXaxis()->SetTitleFont(42);
		h->GetYaxis()->SetTitle("Relative Uncertainty (%)");
		h->GetYaxis()->SetLabelFont(42);
		h->GetYaxis()->SetLabelOffset(0.007);
		h->GetYaxis()->SetLabelSize(0.05);
		h->GetYaxis()->SetTitleSize(0.06);
		h->GetYaxis()->SetTitleOffset(0.9);
		h->GetYaxis()->SetTitleFont(42);
		h->GetYaxis()->SetRangeUser(4e-2, 3e+2);
	}

	void DrawLatex( TLatex &latex )
	{
		latex.DrawLatexNDC(0.14, 0.935, "#font[62]{#scale[1]{CMS}}");
		if( this->channel == "Muon" )
			latex.DrawLatexNDC(0.72, 0.935, "#font[62]{#scale[0.8]{2.8 fb^{-1} (13 TeV)}}");
		else if( this->channel == "Electron" )
			latex.DrawLatexNDC(0.72, 0.935, "#font[62]{#scale[0.8]{2.3 fb^{-1} (13 TeV)}}");
	}

};

void DrawRelUnc_Summary_LogY()
{
	Tool_DrawRelUnc *tool_Muon = new Tool_DrawRelUnc("Muon");
	tool_Muon->DrawCanvas();

	Tool_DrawRelUnc *tool_Electron = new Tool_DrawRelUnc("Electron");
	tool_Electron->DrawCanvas();
}