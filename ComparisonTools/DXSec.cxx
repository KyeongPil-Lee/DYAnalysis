#include <Include/PlotTools.h>

class CompTool
{
public:
	TH1D* h_DXSec_old;
	TH1D* h_DXSec_new;

	TH1D* h_DXSecFid_old;
	TH1D* h_DXSecFid_new;

	CompTool()
	{
		this->Init();
	}

	void Draw_All()
	{
		this->Draw( this->h_DXSec_old, this->h_DXSec_new, 0 );
		this->Draw( this->h_DXSecFid_old, this->h_DXSecFid_new, 1 );
	}

private:
	void Draw( TH1D* h_old, TH1D* h_new, Bool_t isFiducial )
	{
		// -- Draw canvas
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString canvasName = "Local/c_DXSec";
		if( isFiducial ) canvasName =  canvasName + "_fiducial";
		SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 1, 1 );

		c->cd();
		TopPad->cd();

		h_old->Draw("EPSAME");
		h_new->Draw("EPSAME");

		h_old->SetStats(kFALSE);
		h_old->SetMarkerStyle(20);
		h_old->SetMarkerColor(kBlack);
		h_old->SetLineColor(kBlack);
		h_old->SetFillColorAlpha(kWhite, 0); 
		h_old->SetTitle("");

		h_new->SetStats(kFALSE);
		h_new->SetMarkerStyle(20);
		h_new->SetMarkerColor(kRed);
		h_new->SetLineColor(kRed);
		h_new->SetFillColorAlpha(kWhite, 0); 
		h_new->SetTitle("");

		SetHistFormat_TopPad( h_old, "d#sigma/dm [pb/GeV]");
		h_old->GetYaxis()->SetRangeUser(5e-8, 1e3);

		TLegend *legend;
		SetLegend( legend, 0.60, 0.82, 0.95, 0.95 );
		legend->AddEntry( h_old, "Before" );
		legend->AddEntry( h_new, "After" );
		legend->Draw();

		TLatex latex;
		if( isFiducial )
			latex.DrawLatexNDC(0.16, 0.35, "#scale[0.6]{post-FSR, fiducial region}");
		else
			latex.DrawLatexNDC(0.16, 0.35, "#scale[0.6]{FSR-corrected (dressed), full-phase space}");

		c->cd();
		BottomPad->cd();

		TH1D* h_ratio = (TH1D*)h_old->Clone("h_ratio");
		h_ratio->Divide( h_new, h_old );

		h_ratio->Draw("EPSAME");

		SetHistFormat_BottomPad( h_ratio, "m [GeV]", "After/Before", 0.99, 1.01 );

		TF1 *f_line;
		DrawLine(f_line);

		Print_Histogram( h_ratio );

		c->SaveAs(".pdf");
	}

	void Init()
	{
		TString rootFilePath_old = "./Local/OldROOTFiles";
		TString rootFilePath_new = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileNameBase = "ROOTFile_Results_DYAnalysis_76X.root";

		TString fileName_old = rootFilePath_old + "/" + fileNameBase;
		TString fileName_new = rootFilePath_new + "/" + fileNameBase;

		this->h_DXSec_old = Get_Hist( fileName_old, "h_DiffXsec_FSRCorr");
		this->h_DXSec_new = Get_Hist( fileName_new, "h_DiffXsec_FSRCorr");

		this->h_DXSecFid_old = Get_Hist( fileName_old, "h_FpoF_DiffXsec_Data");
		this->h_DXSecFid_new = Get_Hist( fileName_new, "h_FpoF_DiffXsec_Data");
	}

};

void DXSec()
{
	CompTool *tool = new CompTool();
	tool->Draw_All();
}