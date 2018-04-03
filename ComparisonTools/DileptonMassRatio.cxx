#include <Include/PlotTools.h>

class CompTool
{
public:
	TH1D* h_ratio_old;
	TH1D* h_ratio_new;

	CompTool()
	{
		this->Init();
	}

	void Draw()
	{
		TCanvas *c;
		TString canvasName = "Local/c_DileptonMassRatio";
		SetCanvas_Square( c, canvasName, 1, 0 );
		c->cd();

		this->h_ratio_old->Draw("EPSAME");
		this->h_ratio_new->Draw("EPSAME");

		this->h_ratio_old->SetLineColor(kBlack);
		this->h_ratio_old->SetMarkerStyle(20);
		this->h_ratio_old->SetMarkerSize(1);
		this->h_ratio_old->SetMarkerColor(kBlack);

		this->h_ratio_new->SetLineColor(kRed);
		this->h_ratio_new->SetMarkerStyle(20);
		this->h_ratio_new->SetMarkerSize(1);
		this->h_ratio_new->SetMarkerColor(kRed);

		SetHistFormat_SinglePad( this->h_ratio_old, "m [GeV]", "Data/MC" );
		this->h_ratio_old->GetYaxis()->SetRangeUser(0.4, 1.3);

		TLegend *legend;
		SetLegend( legend, 0.16, 0.15, 0.40, 0.32 );
		legend->AddEntry( this->h_ratio_old, "Before" );
		legend->AddEntry( this->h_ratio_new, "After" );
		legend->Draw();

		TLatex latex;
		latex.DrawLatexNDC(0.16, 0.91, "#font[42]{#scale[0.7]{Data/MC in dilepton mass distribution}}");

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}
private:

	void Init()
	{
		TString rootFilePath_old = "./Local/OldROOTFiles";
		TString rootFilePath_new = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileNameBase = "ROOTFile_Histograms_DimuonMassSpectrum_All.root";

		TString fileName_old = rootFilePath_old + "/" + fileNameBase;
		TString fileName_new = rootFilePath_new + "/" + fileNameBase;

		this->h_ratio_old = this->RatioHist( fileName_old );
		this->h_ratio_new = this->RatioHist( fileName_new );
	}

	TH1D* RatioHist( TString fileName )
	{
		TH1D* h_data = Get_Hist( fileName, "h_data" );
		h_data->Sumw2();

		TH1D* h_DY = Get_Hist( fileName, "h_SignalMC");
		h_DY->Sumw2();

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

		TH1D* h_top	= (TH1D*)h_ttbar->Clone();
		h_top->Add( h_tW_emu );

		TH1D* h_EW = (TH1D*)h_WW->Clone();
		h_EW->Add( h_WZ );
		h_EW->Add( h_ZZ );
		h_EW->Add( h_DYtautau );

		// -- fakes -- //
		TH1D* h_WJets = Get_Hist( fileName, "h_WJets_FR" );
		h_WJets->Sumw2();
		TH1D* h_QCD = Get_Hist( fileName, "h_diJet_FR" );
		h_QCD->Sumw2();
		TH1D* h_fakes = (TH1D*)h_WJets->Clone();
		h_fakes->Add( h_QCD );

		// -- total prediction -- //
		TH1D* h_pred = (TH1D*)h_fakes->Clone();
		h_pred->Add(h_EW);
		h_pred->Add(h_top);
		h_pred->Add(h_DY);
		h_pred->Sumw2();

		// -- ratio plot -- //
		TH1D* h_ratio = (TH1D*)h_data->Clone();
		h_ratio->Reset("ICES");
		h_ratio->Divide(h_data, h_pred);

		return h_ratio;
	}
};

void DileptonMassRatio()
{
	CompTool *tool = new CompTool();
	tool->Draw();
}