#include <Include/PlotTools.h>

class DrawingTool
{
public:
	TString channel;

	TGraphAsymmErrors* g_acc;
	TGraphAsymmErrors* g_eff;
	TGraphAsymmErrors* g_accEff;

	DrawingTool( TString _channel )
	{
		this->channel = _channel;
		this->Init();
	}

	void Draw()
	{
		TString canvasName = "c_AccEff_"+this->channel;
		TCanvas *c = new TCanvas(canvasName, "", 800,600 );
		c->SetLogx();
		c->SetTickx(1);
		c->SetTicky(1);
		c->SetLeftMargin(0.13); // CHANGE
		c->SetRightMargin(0.02);
		c->SetTopMargin(0.08); // CHANGE
		c->SetBottomMargin(0.14); // CHANGE

		g_acc->Draw("AP");
		g_eff->Draw("PSAME");
		g_accEff->Draw("PSAME");

		g_acc->SetTitle("");
		g_eff->SetTitle("");
		g_accEff->SetTitle("");

		// -- set colors -- //
		Int_t ci;
		ci = TColor::GetColor("#0000ff");
		g_acc->SetLineColor(ci);
		g_acc->SetMarkerColor(ci);
		g_acc->SetMarkerSize(0.8);
		g_acc->SetMarkerStyle(20);
		g_acc->SetFillColorAlpha(kWhite, 0);

		ci = TColor::GetColor("#ff0000");
		g_eff->SetLineColor(ci);
		g_eff->SetMarkerColor(ci);
		g_eff->SetMarkerSize(0.8);
		g_eff->SetMarkerStyle(21);
		g_eff->SetFillColorAlpha(kWhite, 0);

		ci = TColor::GetColor(kBlack); // CHANGE
		g_accEff->SetLineColor(ci);
		g_accEff->SetMarkerColor(ci);
		g_accEff->SetMarkerSize(0.9);
		g_accEff->SetMarkerStyle(22);
		g_accEff->SetFillColorAlpha(kWhite, 0);

		g_acc->SetMinimum(0);
		g_acc->SetMaximum(1.1);
		// g_acc->SetStats(0);

		// -- X-axis -- //
		g_acc->GetXaxis()->SetTitle("m (post-FSR) [GeV]");
		g_acc->GetXaxis()->SetMoreLogLabels();
		g_acc->GetXaxis()->SetNoExponent();
		g_acc->GetXaxis()->SetLabelFont(42);
		g_acc->GetXaxis()->SetLabelOffset(0.007);
		g_acc->GetXaxis()->SetLabelSize(0.04);
		g_acc->GetXaxis()->SetTitleSize(0.06);
		g_acc->GetXaxis()->SetTitleOffset(1.00);
		g_acc->GetXaxis()->SetTitleFont(42);
		g_acc->GetXaxis()->SetLimits( 15, 3000 );

		// -- Y-axis -- //
		g_acc->GetYaxis()->SetTitle("Fraction of events");
		g_acc->GetYaxis()->SetLabelFont(42);
		g_acc->GetYaxis()->SetLabelOffset(0.007);
		g_acc->GetYaxis()->SetLabelSize(0.04);
		g_acc->GetYaxis()->SetTitleSize(0.06);
		g_acc->GetYaxis()->SetTitleOffset(0.9);
		g_acc->GetYaxis()->SetTitleFont(42);

		TLegend *legend = new TLegend(0.75, 0.15, 0.97, 0.5 );
		legend->SetBorderSize(0);
		legend->SetFillStyle(0);
		legend->SetTextFont(62);
		legend->AddEntry( g_acc, "#font[122]{A}", "EP" );
		legend->AddEntry( g_eff, "#varepsilon", "EP" );
		legend->AddEntry( g_accEff, "#font[122]{A} #times #varepsilon", "EP" );
		legend->Draw();

		TLatex latex;
		latex.DrawLatexNDC(0.14, 0.935, "#font[62]{#scale[1]{CMS }}#font[42]{#it{Simulation}}");
		latex.DrawLatexNDC(0.86, 0.935, "#font[62]{#scale[0.9]{13 TeV}}");
		if( this->channel == "Muon" )
			latex.DrawLatexNDC(0.80, 0.87, "#scale[0.8]{#gamma* /#font[122]{Z} #rightarrow #mu#mu}");
		if( this->channel == "Electron" )
			latex.DrawLatexNDC(0.80, 0.87, "#scale[0.8]{#gamma* /#font[122]{Z} #rightarrow ee}");

		c->SaveAs(".pdf");
	}

private:
	void Init()
	{
		TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		if( this->channel == "Muon" )
		{
			TString fileName = rootFilePath+"/ROOTFile_AccEff.root";

			this->g_acc = Get_Graph( fileName, "g_Acc" );
			this->g_eff = Get_Graph( fileName, "g_Eff" );
			this->g_accEff = Get_Graph( fileName, "g_AccEff" );
		}
		else if( this->channel == "Electron" )
		{
			TString fileName = rootFilePath+"/ROOTFile_AccEff_Electron.root";

			this->g_acc = Get_Graph( fileName, "g_acc" );
			this->g_eff = Get_Graph( fileName, "g_eff" );
			this->g_accEff = Get_Graph( fileName, "g_accEff" );
		}
		else
			cout << this->channel << " is wrong type!" << endl;

		this->RemoveHorizontalErrors( this->g_acc );
		this->RemoveHorizontalErrors( this->g_eff );
		this->RemoveHorizontalErrors( this->g_accEff );
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

void DrawAccEff()
{
	DrawingTool *tool_Muon = new DrawingTool( "Muon" );
	tool_Muon->Draw();

	DrawingTool *tool_Electron = new DrawingTool( "Electron" );
	tool_Electron->Draw();
}