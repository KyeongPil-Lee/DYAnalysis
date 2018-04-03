#include <Include/PlotTools.h>

class CompTool
{
public:
	TGraphAsymmErrors *g_acc_old;
	TGraphAsymmErrors *g_eff_old;
	TGraphAsymmErrors *g_accEff_old;

	TGraphAsymmErrors *g_acc_new;
	TGraphAsymmErrors *g_eff_new;
	TGraphAsymmErrors *g_accEff_new;

	CompTool()
	{
		this->Init();
	}

	void Draw_All()
	{
		this->Draw( g_acc_old, g_acc_new, "Acc" );
		this->Draw( g_eff_old, g_eff_new, "Eff" );
		this->Draw( g_accEff_old, g_accEff_new, "AccEff" );
	}

private:
	void Draw( TGraphAsymmErrors* g_old, TGraphAsymmErrors* g_new, TString type )
	{
		// -- Draw canvas
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString canvasName = "Local/c_" + type;
		SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 1, 0 );

		c->cd();
		TopPad->cd();

		g_old->Draw("APSAME");
		g_new->Draw("PSAME");

		g_old->SetMarkerStyle(20);
		g_old->SetMarkerColor(kBlack);
		g_old->SetLineColor(kBlack);
		g_old->SetFillColorAlpha(kWhite, 0); 

		g_new->SetMarkerStyle(20);
		g_new->SetMarkerColor(kRed);
		g_new->SetLineColor(kRed);
		g_new->SetFillColorAlpha(kWhite, 0); 

		TString YTitle = "";
		if( type == "Acc" ) YTitle = "Acceptance";
		if( type == "Eff" ) YTitle = "Efficiency";
		if( type == "AccEff" ) YTitle = "Acc. * Eff.";
		SetGraphFormat_TopPad( g_old, YTitle );
		g_old->GetYaxis()->SetRangeUser(0, 1.1);

		TLegend *legend;
		SetLegend( legend, 0.60, 0.32, 0.95, 0.45 );
		legend->AddEntry( g_old, "Before" );
		legend->AddEntry( g_new, "After" );
		legend->Draw();

		c->cd();
		BottomPad->cd();

		TGraphAsymmErrors* g_ratio = MakeGraph_Ratio( g_new, g_old );

		g_ratio->Draw("APSAME");

		SetGraphFormat_BottomPad( g_ratio, "m [GeV]", "After/Before", 0.99, 1.01 );

		TF1 *f_line;
		DrawLine(f_line);

		Print_Graph( g_ratio );

		c->SaveAs(".pdf");
	}

	void Init()
	{
		TString rootFilePath_old = "./Local/OldROOTFiles";
		TString rootFilePath_new = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileNameBase = "ROOTFile_AccEff.root";

		TString fileName_old = rootFilePath_old + "/" + fileNameBase;
		TString fileName_new = rootFilePath_new + "/" + fileNameBase;

		this->g_acc_old = Get_Graph( fileName_old, "g_Acc");
		this->g_eff_old = Get_Graph( fileName_old, "g_Eff");
		this->g_accEff_old = Get_Graph( fileName_old, "g_AccEff");

		this->g_acc_new = Get_Graph( fileName_new, "g_Acc");
		this->g_eff_new = Get_Graph( fileName_new, "g_Eff");
		this->g_accEff_new = Get_Graph( fileName_new, "g_AccEff");
	}

};

void AccEff()
{
	CompTool *tool = new CompTool();
	tool->Draw_All();
}