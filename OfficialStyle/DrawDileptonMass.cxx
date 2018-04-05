#include <Include/PlotTools.h>

class DrawingTool
{
public:
	TString channel;

	TH1D* h_data;
	TH1D* h_fakes;
	TH1D* h_EW;
	TH1D* h_top;
	TH1D* h_DY;

	TH1D* h_pred; // -- total prediction -- //
	TH1D* h_ratio;

	DrawingTool( TString _channel )
	{
		channel = _channel;
		this->Init();
	}

	void Draw()
	{
		gStyle->SetOptStat(0);
		gStyle->SetOptTitle(0);

		TString canvasName = this->channel+"_Mass_DYBin";
		TCanvas *c = new TCanvas(canvasName, "", 800, 700);
		c->SetTickx(1);
		c->SetTicky(1);
		c->SetLeftMargin(0.16);
		c->SetRightMargin(0.02);
		c->SetTopMargin(0.08);
		c->SetBottomMargin(0.14);

		TPad *TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
		TopPad->Draw();
		TopPad->cd();
		TopPad->Range(0.7100599, -11.71078, 3.622756, 3.443786);
		TopPad->SetLogx();
		TopPad->SetLogy();
		TopPad->SetTickx(1);
		TopPad->SetTicky(1);
		TopPad->SetLeftMargin(0.13);
		TopPad->SetRightMargin(0.05);
		TopPad->SetTopMargin(0.05);
		TopPad->SetBottomMargin(0.32);

		h_DY->SetFillColor(kOrange-2);
		h_DY->SetLineColor(kOrange+3);
		h_top->SetFillColor(kRed+2);
		h_top->SetLineColor(kRed+4);
		h_EW->SetFillColor(kOrange+10);
		h_EW->SetLineColor(kOrange+3);
		h_fakes->SetFillColor(kViolet-5);
		h_fakes->SetLineColor(kOrange+3);

		THStack* hs = new THStack("hs", "hs");
		hs->Add(h_fakes, "hist");
		hs->Add(h_EW, "hist");
		hs->Add(h_top, "hist");
		hs->Add(h_DY, "hist");

		h_data->SetMarkerStyle(20);
		h_data->SetMarkerSize(1.0);

		h_data->GetXaxis()->SetTitle("m [GeV]");
		h_data->GetXaxis()->SetMoreLogLabels();
		h_data->GetXaxis()->SetNoExponent();
		h_data->GetXaxis()->SetLabelFont(42);
		h_data->GetXaxis()->SetLabelSize(0);
		h_data->GetXaxis()->SetTitleSize(0);
		h_data->GetXaxis()->SetTitleFont(42);

		h_data->GetYaxis()->SetTitle("Entries per bin");
		h_data->GetYaxis()->SetLabelFont(42);
		h_data->GetYaxis()->SetLabelSize(0.035);
		h_data->GetYaxis()->SetTitleSize(0.06);
		h_data->GetYaxis()->SetTitleOffset(0.9);
		h_data->GetYaxis()->SetTitleFont(42);
		h_data->GetZaxis()->SetLabelFont(42);
		h_data->GetZaxis()->SetLabelSize(0.035);
		h_data->GetZaxis()->SetTitleSize(0.035);
		h_data->GetZaxis()->SetTitleFont(42);
		h_data->SetMinimum(0.1);
		h_data->SetMaximum(3e6);

		h_data->Draw("E1P");
		hs->Draw("histsame");
		h_data->Draw("E1Psame");

		TLine grid_;
		grid_.SetLineColor(kGray+2);
		grid_.SetLineStyle(kSolid);
		for( size_t ii=0; ii<43; ii++ )
		{
			Int_t i_bin = ii+1;
			Double_t binEdge = h_data->GetBinLowEdge(i_bin);
			grid_.DrawLine(binEdge, 0.1, binEdge, h_pred->GetBinContent(ii+1) );
		}

		TLegend *legend = new TLegend(0.70, 0.70, 0.90, 0.90);
		legend->SetBorderSize(0);
		// legend->SetLineColor(1);
		// legend->SetLineStyle(1);
		// legend->SetLineWidth(1);
		// legend->SetFillColor(0);
		legend->SetFillStyle(0);
		legend->SetTextFont(62);

		legend->AddEntry( h_data,"data" );
		if( this->channel == "Muon" )
			legend->AddEntry( h_DY, "#gamma* /#font[122]{Z} #rightarrow #mu#mu", "f" );
		else if( this->channel == "Electron" )
			legend->AddEntry( h_DY, "#gamma* /#font[122]{Z} #rightarrow ee", "f" );
		legend->AddEntry( h_top, "t#bar{t}+tW+#bar{t}W", "f" );
		legend->AddEntry( h_EW, "EW", "f" );
		legend->AddEntry( h_fakes, "Misid.", "f" );
		legend->Draw();

		TLatex latex;
		latex.DrawLatexNDC(0.14, 0.96, "#font[62]{#scale[0.8]{CMS}}");
		if( this->channel == "Muon" )
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");
		else if( this->channel == "Electron" )
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (13 TeV)}}");

		TPad *bottomPad = new TPad("bottomPad", "bottomPad", 0.01, 0.01, 0.99, 0.3);
		bottomPad->Draw();
		bottomPad->cd();
		bottomPad->Range(0.7499746, -0.2272727, 3.590752, 1.590909);
		// bottomPad->SetFillColor(0);
		// bottomPad->SetBorderMode(0);
		// bottomPad->SetBorderSize(2);
		bottomPad->SetLogx();
		//bottomPad->SetGridx();
		//bottomPad->SetGridy();
		bottomPad->SetTickx(1);
		bottomPad->SetTicky(1);
		bottomPad->SetLeftMargin(0.122);
		bottomPad->SetRightMargin(0.04);
		bottomPad->SetTopMargin(0.05);
		bottomPad->SetBottomMargin(0.4);
		// bottomPad->SetFrameFillStyle(0);
		// bottomPad->SetFrameBorderMode(0);
		// bottomPad->SetFrameFillStyle(0);
		// bottomPad->SetFrameBorderMode(0);

		h_ratio->SetMinimum(0.45);
		h_ratio->SetMaximum(1.55);
		// h_ratio->SetEntries(129);
		// h_ratio->SetDirectory(0);
		h_ratio->SetStats(0);

		// h_ratio->SetFillColor(kBlack);
		// h_ratio->SetFillStyle(1001);
		h_ratio->SetMarkerStyle(20);
		h_ratio->SetMarkerSize(1);
		h_ratio->GetXaxis()->SetTitle("m [GeV]");
		// h_ratio->GetXaxis()->SetRange(1,43);
		h_ratio->GetXaxis()->SetMoreLogLabels();
		h_ratio->GetXaxis()->SetNoExponent();
		h_ratio->GetXaxis()->SetLabelFont(42);
		h_ratio->GetXaxis()->SetLabelOffset(0.007);
		h_ratio->GetXaxis()->SetLabelSize(0.15);
		h_ratio->GetXaxis()->SetTitleSize(0.2);
		h_ratio->GetXaxis()->SetTitleOffset(0.9);
		h_ratio->GetXaxis()->SetTitleFont(42);
		h_ratio->GetYaxis()->SetTitle("Data/(DY+Bkg)");
		h_ratio->GetYaxis()->SetLabelFont(42);
		h_ratio->GetYaxis()->SetLabelSize(0.07);
		h_ratio->GetYaxis()->SetTitleSize(0.1);
		h_ratio->GetYaxis()->SetTitleOffset(0.4);
		h_ratio->GetYaxis()->SetTitleFont(42);
		// h_ratio->GetYaxis()->SetRangeUser(42);
		// h_ratio->GetZaxis()->SetLabelFont(42);
		// h_ratio->GetZaxis()->SetLabelSize(0.035);
		// h_ratio->GetZaxis()->SetTitleSize(0.035);
		// h_ratio->GetZaxis()->SetTitleFont(42);
		h_ratio->Draw("E1PL");
		
		TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		// f_line->SetFillColor(19);
		// f_line->SetFillStyle(0);
		f_line->SetMarkerStyle(20);

		f_line->SetLineColor(kSpring-1);
		f_line->SetLineStyle(2);
		f_line->SetLineWidth(1);
		// f_line->GetXaxis()->SetLabelFont(42);
		// f_line->GetXaxis()->SetLabelOffset(0.007);
		// f_line->GetXaxis()->SetLabelSize(0.05);
		// f_line->GetXaxis()->SetTitleSize(0.06);
		// f_line->GetXaxis()->SetTitleOffset(0.9);
		// f_line->GetXaxis()->SetTitleFont(42);
		// f_line->GetYaxis()->SetLabelFont(42);
		// f_line->GetYaxis()->SetLabelOffset(0.007);
		// f_line->GetYaxis()->SetLabelSize(0.05);
		// f_line->GetYaxis()->SetTitleSize(0.06);
		// f_line->GetYaxis()->SetTitleOffset(1.25);
		// f_line->GetYaxis()->SetTitleFont(42);
		f_line->Draw("SAME");
		h_ratio->Draw("E1PLsame");

		TLine gridRatio;
		gridRatio.SetLineColor(kGray+2);
		gridRatio.SetLineStyle(2);
		for( size_t ii=0; ii<44; ii++ )
		{
			Int_t i_bin = ii+1;
			Double_t binEdge = h_data->GetBinLowEdge(i_bin);
			gridRatio.DrawLine(binEdge, 0.45, binEdge, 1.55);
		}

		// c1->cd();
		// c1->Modified();
		// c1->cd();
		// c1->SetSelected(c1);







		c->SaveAs(".pdf");
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

		this->h_data = Get_Hist( fileName, "h_data" );
		this->h_data->Sumw2();

		this->h_DY = Get_Hist( fileName, "h_SignalMC");
		this->h_DY->Sumw2();

		// -- top -- //
		TH1D* h_ttbar = Get_Hist( fileName, "h_ttbar_emu" );
		h_ttbar->Sumw2();
		TH1D* h_tW_emu = Get_Hist( fileName, "h_tW_emu");
		h_tW_emu->Sumw2();

		// -- EW -- //
		TH1D* h_WW = Get_Hist( fileName, "h_WW_emu" );
		h_WW->Sumw2();
		TH1D* h_WZ = Get_Hist( fileName, "h_WZ" );
		h_WZ->Sumw2();
		TH1D* h_ZZ = Get_Hist( fileName, "h_ZZ" );
		h_ZZ->Sumw2();
		TH1D* h_DYtautau = Get_Hist( fileName, "h_DYTauTau_emu" );
		h_DYtautau->Sumw2();

		this->HotFix_ElectronChannel( h_tW_emu, h_WW );

		this->h_top	= (TH1D*)h_ttbar->Clone();
		this->h_top->Add( h_tW_emu );

		this->h_EW = (TH1D*)h_WW->Clone();
		this->h_EW->Add( h_WZ );
		this->h_EW->Add( h_ZZ );
		this->h_EW->Add( h_DYtautau );

		// -- fakes -- //
		TH1D* h_WJets = Get_Hist( fileName, "h_WJets_FR" );
		h_WJets->Sumw2();
		TH1D* h_QCD = Get_Hist( fileName, "h_diJet_FR" );
		h_QCD->Sumw2();
		this->h_fakes = (TH1D*)h_WJets->Clone();
		this->h_fakes->Add( h_QCD );

		// -- total prediction -- //
		this->h_pred = (TH1D*)h_fakes->Clone();
		this->h_pred->Add(h_EW);
		this->h_pred->Add(h_top);
		this->h_pred->Add(h_DY);
		this->h_pred->Sumw2();

		// -- ratio plot -- //
		this->h_ratio = (TH1D*)h_data->Clone();
		this->h_ratio->Reset("ICES");
		this->h_ratio->Divide(this->h_data, this->h_pred);
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

void DrawDileptonMass()
{
	DrawingTool* tool_Muon = new DrawingTool("Muon");
	tool_Muon->Draw();

	DrawingTool *tool_Electron = new DrawingTool("Electron");
	tool_Electron->Draw();
}