#include <Include/PlotTools.h>

class CompTool
{
public:
	TString histName;
	Int_t i_canvas;

	TH1D* h_old;
	TH1D* h_new;

	CompTool( TString _histName, Int_t _i_canvas )
	{
		this->histName = _histName;
		this->i_canvas = _i_canvas;
		this->Init();
	}

	void Draw()
	{
		// -- Draw canvas
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString canvasName = TString::Format("Local/c%02d_%s", i_canvas, histName.Data());
		SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 1, 1 );

		c->cd();
		TopPad->cd();

		h_old->Draw("EPSAME");
		h_new->Draw("EPSAME");

		h_old->SetStats(kFALSE);
		h_old->SetMarkerStyle(20);
		h_old->SetMarkerSize(1);
		h_old->SetMarkerColor(kBlack);
		h_old->SetLineColor(kBlack);
		h_old->SetFillColorAlpha(kWhite, 0); 
		h_old->SetTitle("");

		h_new->SetStats(kFALSE);
		h_new->SetMarkerStyle(20);
		h_new->SetMarkerSize(1);
		h_new->SetMarkerColor(kRed);
		h_new->SetLineColor(kRed);
		h_new->SetFillColorAlpha(kWhite, 0); 
		h_new->SetTitle("");

		SetHistFormat_TopPad( h_old, "Entries per bin");
		// h_old->GetYaxis()->SetRangeUser(5e-8, 1e3);

		TLegend *legend;
		SetLegend( legend, 0.60, 0.82, 0.95, 0.95 );
		legend->AddEntry( h_old, "Before" );
		legend->AddEntry( h_new, "After" );
		legend->Draw();

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

private:

	void Init()
	{
		TString rootFilePath_old = "./Local/OldROOTFiles";
		TString rootFilePath_new = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileNameBase = "ROOTFile_Results_DYAnalysis_76X.root";

		TString fileName_old = rootFilePath_old + "/" + fileNameBase;
		TString fileName_new = rootFilePath_new + "/" + fileNameBase;

		this->h_old = Get_Hist( fileName_old, this->histName);
		this->h_new = Get_Hist( fileName_new, this->histName);
	}
};


void EachCorrectionStep()
{
	vector<TString> vec_histName = 
	{
		"h_yield_Raw",
		"h_yield_Raw_HLTv4p2",
		"h_yield_Raw_HLTv4p3",
		"h_yield_Unfolded",
		"h_yield_HLTv4p2_Unfolded",
		"h_yield_HLTv4p3_Unfolded",
		"h_yield_Unfolded_AccEff",
		"h_yield_HLTv4p2_Unfolded_AccEff",
		"h_yield_HLTv4p3_Unfolded_AccEff",
		"h_yield_EffCorr",
		"h_yield_HLTv4p2_EffCorr",
		"h_yield_HLTv4p3_EffCorr",
		"h_yield_FSRCorr"
	};

	Int_t nHist = vec_histName.size();
	for(Int_t i=0; i<nHist; i++)
	{
		CompTool *tool = new CompTool( vec_histName[i], i );
		tool->Draw();
	}
}