#include <Include/PlotTools.h>

class Tool_DrawRelUnc
{
public:
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

	Tool_DrawRelUnc()
	{
		this->FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");
	}
	

	void DrawCanvas()
	{
		this->Get_Histograms();

		this->c = new TCanvas("Muon_SysUnc_All", "",0,0,800,700);
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

		// Hist_RelStatUnc->h->GetYaxis()->SetRangeUser(1e-2, 1);
		// c->SaveAs(".pdf");
		// c->SetLogy();
		// this->RemoveNegativeBin ( this->Hist_RelSystUnc_Bkg->h );
		// c->SaveAs(".pdf");

		// Hist_RelStatUnc->h->GetYaxis()->SetRangeUser(4e-2, 250);

		c->SaveAs(".pdf");
	}
protected:
	void Get_Histograms()
	{
		TString FileName = FileLocation + "/ROOTFile_DiffXSec_FullUnc.root";

		// -- stat. unc -- //
		this->Hist_RelStatUnc = new HistInfo(kBlack, "Statistical");
		this->Hist_RelStatUnc->Set_FileName_ObjectName( FileName, "h_RelStatUnc_Percent");
		this->Hist_RelStatUnc->Set();
		this->Hist_RelStatUnc->h->SetMarkerStyle(20);

		// -- total systematic uncertainty -- //
		this->Hist_RelSystUnc_Tot = new HistInfo(kRed, "Total Systematic");
		this->Hist_RelSystUnc_Tot->Set_FileName_ObjectName( FileName, "h_RelSysUnc_Tot_Percent");
		this->Hist_RelSystUnc_Tot->Set();
		this->Hist_RelSystUnc_Tot->h->SetMarkerStyle(kFullSquare);

		// -- uncertainty from efficiency scale factor -- //
		this->Hist_RelSystUnc_EffSF = new HistInfo( TColor::GetColor("#0000ff"), "Efficiency SF");
		this->Hist_RelSystUnc_EffSF->Set_FileName_ObjectName( FileName, "h_RelSysUnc_Eff.SF._Percent");
		this->Hist_RelSystUnc_EffSF->Set();
		this->Hist_RelSystUnc_EffSF->h->SetMarkerStyle(25);

		// -- uncertainty from detector resolution -- //
		this->Hist_RelSystUnc_DetRes = new HistInfo( TColor::GetColor("#cc00ff"), "Detector Res.");
		this->Hist_RelSystUnc_DetRes->Set_FileName_ObjectName( FileName, "h_RelSysUnc_Det.Res_Percent");
		this->Hist_RelSystUnc_DetRes->Set();
		this->Hist_RelSystUnc_DetRes->h->SetMarkerStyle(26);

		// -- uncertainty from backgrounds -- //
		this->Hist_RelSystUnc_Bkg = new HistInfo( TColor::GetColor("#00cc00"), "Background");
		this->Hist_RelSystUnc_Bkg->Set_FileName_ObjectName( FileName, "h_RelSysUnc_Bkg.Est._Percent");
		this->Hist_RelSystUnc_Bkg->Set();
		this->Hist_RelSystUnc_Bkg->h->SetMarkerStyle(27);
		this->Hist_RelSystUnc_Bkg->h->SetMinimum(0.001);

		// -- uncertainty from FSR correction -- //
		this->Hist_RelSystUnc_FSR = new HistInfo( TColor::GetColor("#9999ff"), "FSR");
		this->Hist_RelSystUnc_FSR->Set_FileName_ObjectName( FileName, "h_RelSysUnc_FSR_Percent");
		this->Hist_RelSystUnc_FSR->Set();
		this->Hist_RelSystUnc_FSR->h->SetMarkerStyle(28);

		// -- uncertainty from acceptance correction -- //
		this->Hist_RelTheoUnc_Acc = new HistInfo( TColor::GetColor("#99ff99"), "Acceptance");
		this->Hist_RelTheoUnc_Acc->Set_FileName_ObjectName( FileName, "h_RelSysUnc_Acc._Percent");
		this->Hist_RelTheoUnc_Acc->Set();
		this->Hist_RelTheoUnc_Acc->h->SetMarkerStyle(24);

		// -- uncertainty from luminosity -- //
		this->Hist_RelLumiUnc = new HistInfo( TColor::GetColor("#ff9933"), "Luminosity");
		this->Hist_RelLumiUnc->Set_FileName_ObjectName( FileName, "h_RelLumiUnc_Percent");
		this->Hist_RelLumiUnc->Set();
		this->Hist_RelLumiUnc->h->SetMarkerStyle(22);
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
		TLegend *legtot = new TLegend(0.18,0.77,0.43,0.85,NULL,"brNDC");
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
		TLegend *leg = new TLegend(0.37,0.61,0.67,0.85,NULL,"brNDC");
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
		latex.SetTextSize(0.04);
		latex.DrawLatexNDC( 0.725, 0.93, "2.8 fb^{#font[122]{\55}1} (13 TeV)");

		latex.SetTextSize(0.045);
		latex.DrawLatexNDC( 0.14, 0.87, "CMS Preliminary" );
	}

	void RemoveNegativeBin( TH1D* h)
	{
		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t value = h->GetBinContent(i_bin);
			if( value < 0 )
			{
				printf("Find negative value! ... value = %lf\n", value);
				h->SetBinContent(i_bin, 1e-10);
			}
		}
	}

};

void DrawRelUnc_Summary_LogY()
{
	Tool_DrawRelUnc *tool = new Tool_DrawRelUnc();
	tool->DrawCanvas();
}

