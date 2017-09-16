#include <Include/DYAnalyzer.h> // -- for nMassBin -- //
#include <Include/MyCanvas.C>
#include <Include/PlotTools.h>

class Comparison
{
public:
	Double_t MassBinEdges[nMassBin+1];

	TFile *f_amcnlo;
	TFile *f_fewz;
	TFile *f_input;

	TH1D *h_DiffXsec_FEWZ_NLO;
	TH1D *h_DiffXsec_WithinAcc_FEWZ_NLO;
	TH1D *h_DiffXsec_FEWZ_NNLO;
	TH1D *h_DiffXsec_WithinAcc_FEWZ_NNLO;

	TH1D *h_Acc_FEWZ_NLO;
	TH1D *h_Acc_FEWZ_NNLO;

	TH1D *h_Acc_FEWZ_NLO_PDF4LHC15;
	TH1D *h_Acc_FEWZ_NNLO_PDF4LHC15;

	TH1D *h_Acc_aMCNLO;
	TH1D *h_Acc_aMCNLO_postFSR;

	Comparison()
	{
		TH1::AddDirectory(kFALSE);

		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		for(Int_t i=0; i<nMassBin+1; i++)
			MassBinEdges[i] = MassBinEdges_temp[i];

		f_amcnlo = TFile::Open("ROOTFile_Histograms_Mass_WithinAcc_aMCNLO.root");

		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		f_fewz = new TFile(ROOTFilePath+"/ROOTFile_xSec_Theory.root");
		f_fewz->cd();
		h_DiffXsec_FEWZ_NLO = (TH1D*)f_fewz->Get("h_DiffXsec_FEWZ_NNPDF_NLO")->Clone();
		h_DiffXsec_FEWZ_NNLO = (TH1D*)f_fewz->Get("h_DiffXsec_FEWZ_NNPDF_NNLO")->Clone();

		f_input = new TFile("ROOTFile_DiffXsec_WithinAcc_All.root");
		f_input->cd();
		TH1D* h_DiffXsec_WithinAcc_FEWZ_NLO_oldBin = (TH1D*)f_input->Get("h_DiffXsec_WithinAcc_FEWZ_NLO")->Clone();
		TH1D* h_DiffXsec_WithinAcc_FEWZ_NNLO_oldBin = (TH1D*)f_input->Get("h_DiffXsec_WithinAcc_FEWZ_NNLO")->Clone();

		this->h_DiffXsec_WithinAcc_FEWZ_NLO = this->Merge_HighMassBin_DiffXSec( h_DiffXsec_WithinAcc_FEWZ_NLO_oldBin );
		this->h_DiffXsec_WithinAcc_FEWZ_NNLO = this->Merge_HighMassBin_DiffXSec( h_DiffXsec_WithinAcc_FEWZ_NNLO_oldBin );
	}

	void Get_Acceptance_PDF4LHC()
	{
		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		TString FileName_PDF = AnalyzerPath + "/SysUncEstimation/Acceptance/ROOTFile_AccWithUnc_FEWZ_PDF4LHC15.root";
		this->h_Acc_FEWZ_NLO_PDF4LHC15 = Get_Hist( FileName_PDF, "h_Acc_NLO" );
		this->h_Acc_FEWZ_NNLO_PDF4LHC15 = Get_Hist( FileName_PDF, "h_Acc_NNLO" );
	}

	void Plot_Acc_aMCNLO_vs_FEWZ_PDF4LHC()
	{
		MyCanvas *myc = new MyCanvas("c_Acc_aMCNLO_vs_FEWZ_PDF4LHC15", "Dimuon Mass [GeV]", "Acceptance");
		myc->SetLogx(1);
		myc->SetRatioRange(0.4, 1.2);
		myc->SetLegendPosition( 0.55, 0.32, 0.95, 0.47 );
		myc->CanvasWithThreeHistogramsRatioPlot( h_Acc_FEWZ_NLO_PDF4LHC15, h_Acc_FEWZ_NNLO_PDF4LHC15, h_Acc_aMCNLO, 
			"Acc. (FEWZ, NLO)", "Acc. (FEWZ, NNLO)", "Acc. (aMC@NLO)", "FEWZ/aMC@NLO", kGreen+1, kBlue, kRed);
		// myc->c->cd();
		// myc->TopPad->cd();
		// TLatex latex;
		// latex.DrawLatexNDC(0.17, 0.90, "#font[42]{#scale[0.8]{FEWZ: PDF4LHC15}}");
		// latex.DrawLatexNDC(0.17, 0.86, "#font[42]{#scale[0.6]{Dressed level}}");
		myc->PrintCanvas();
	}

	void CalcAcc_aMCNLO()
	{
		f_amcnlo->cd();
		TH1D *h_mass_preFSR_tot = (TH1D*)f_amcnlo->Get("h_mass_preFSR_tot")->Clone(); h_mass_preFSR_tot->Sumw2();
		TH1D *h_mass_preFSR_WithinAcc_tot = (TH1D*)f_amcnlo->Get("h_mass_preFSR_WithinAcc_tot")->Clone(); h_mass_preFSR_WithinAcc_tot->Sumw2();

		h_Acc_aMCNLO = (TH1D*)h_mass_preFSR_tot->Clone();
		h_Acc_aMCNLO->Divide( h_mass_preFSR_WithinAcc_tot, h_mass_preFSR_tot );
		// TEfficiency *TEff_aMCNLO = new TEfficiency( *h_mass_preFSR_WithinAcc_tot, *h_mass_preFSR_tot );
		// TGraphAsymmErrors* g_Acc_aMCNLO = (TGraphAsymmErrors*)TEff_aMCNLO->CreateGraph()->Clone();
		// h_Acc_aMCNLO = Convert_GraphToHist( g_Acc_aMCNLO );

		TH1D *h_mass_postFSR_tot = (TH1D*)f_amcnlo->Get("h_mass_postFSR_tot")->Clone(); h_mass_postFSR_tot->Sumw2();
		TH1D *h_mass_postFSR_WithinAcc_tot = (TH1D*)f_amcnlo->Get("h_mass_postFSR_WithinAcc_tot")->Clone(); h_mass_postFSR_WithinAcc_tot->Sumw2();

		h_Acc_aMCNLO_postFSR = (TH1D*)h_mass_postFSR_tot->Clone();
		h_Acc_aMCNLO_postFSR->Divide( h_mass_postFSR_WithinAcc_tot, h_mass_postFSR_tot );
		// TEfficiency *TEff_aMCNLO_postFSR = new TEfficiency( *h_mass_postFSR_WithinAcc_tot, *h_mass_postFSR_tot );
		// TGraphAsymmErrors* g_Acc_aMCNLO_postFSR = (TGraphAsymmErrors*)TEff_aMCNLO->CreateGraph()->Clone();
		// h_Acc_aMCNLO_postFSR = Convert_GraphToHist( g_Acc_aMCNLO_postFSR );
	}

	void CalcAcc_FEWZ()
	{
		h_Acc_FEWZ_NLO = (TH1D*)h_DiffXsec_FEWZ_NLO->Clone();
		h_Acc_FEWZ_NLO->Divide(h_DiffXsec_WithinAcc_FEWZ_NLO, h_DiffXsec_FEWZ_NLO);

		h_Acc_FEWZ_NNLO = (TH1D*)h_DiffXsec_FEWZ_NNLO->Clone();
		h_Acc_FEWZ_NNLO->Divide(h_DiffXsec_WithinAcc_FEWZ_NNLO, h_DiffXsec_FEWZ_NNLO);
	}

	void SaveResults_Unc()
	{
		TH1D* h_RelUnc = (TH1D*)h_Acc_FEWZ_NNLO->Clone("h_RelUnc");
		Int_t nBin = h_RelUnc->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t Acc_aMCNLO = this->h_Acc_aMCNLO->GetBinContent(i_bin);
			Double_t Acc_FEWZ = this->h_Acc_FEWZ_NNLO_PDF4LHC15->GetBinContent(i_bin);
			Double_t RelUnc = fabs( Acc_FEWZ - Acc_aMCNLO ) / Acc_aMCNLO; // -- DEN = aMC@NLO, because our acceptance is based on aMC@NLO value -- //

			h_RelUnc->SetBinContent(i_bin, RelUnc);
			h_RelUnc->SetBinError(i_bin, 0);
		}

		TFile *f_output = TFile::Open("ROOTFile_SysUnc_DiffWithFEWZ.root", "RECREATE");
		f_output->cd();
		h_RelUnc->Write();
		this->h_Acc_aMCNLO->Write("h_Acc_aMCNLO");
		this->h_Acc_aMCNLO_postFSR->Write("h_Acc_aMCNLO_postFSR");
		this->h_Acc_FEWZ_NLO_PDF4LHC15->Write("h_Acc_FEWZ_NLO_PDF4LHC15");
		this->h_Acc_FEWZ_NNLO_PDF4LHC15->Write("h_Acc_FEWZ_NNLO_PDF4LHC15");
	}

	void Plot_Acc_aMCNLO_vs_FEWZ()
	{
		MyCanvas *myc = new MyCanvas("c_Acc_aMCNLO_vs_FEWZ", "Dimuon Mass [GeV]", "Acceptance");
		myc->SetLogx(1);
		myc->SetRatioRange(0.4, 1.2);
		myc->SetLegendPosition( 0.55, 0.32, 0.95, 0.47 );
		myc->CanvasWithThreeHistogramsRatioPlot( h_Acc_FEWZ_NLO, h_Acc_FEWZ_NNLO, h_Acc_aMCNLO, 
			"Acc. (FEWZ, NLO)", "Acc. (FEWZ, NNLO)", "Acc. (aMC@NLO)", "FEWZ/aMC@NLO", kGreen+1, kBlue, kRed);
		myc->PrintCanvas();
	}



	void Plot_Acc_aMCNLO_preFSR_vs_postFSR()
	{
		MyCanvas *myc = new MyCanvas("c_Acc_aMCNLO_preFSR_vs_postFSR", "Dimuon Mass [GeV]", "Acceptance");
		myc->SetLogx(1);
		myc->SetRatioRange(0.9, 1.1);
		myc->SetLegendPosition( 0.55, 0.32, 0.95, 0.47 );
		myc->CanvasWithHistogramsRatioPlot(h_Acc_aMCNLO_postFSR, h_Acc_aMCNLO, "Acc. (aMC@NLO, post-FSR)", "Acc. (aMC@NLO, pre-FSR)", "post-FSR/pre-FSR");
		myc->c->cd();
		myc->TopPad->cd();
		myc->TopPad->SetGridx();
		myc->TopPad->SetGridy();
		myc->PrintCanvas();
	}

	TH1D* Merge_HighMassBin_DiffXSec( TH1D* h_DiffXSec )
	{
		TH1D* h_XSec = MultiplyEachBin_byBinWidth( h_DiffXSec );
		TH1D* h_XSec_Merged = this->Merge_HighMassBin( h_XSec );
		TH1D* h_DiffXSec_Merged = DivideEachBin_ByBinWidth( h_XSec_Merged );

		return h_DiffXSec_Merged;
	}

	// -- [1000, 1200] (42th), [1200, 1500] (43th) => [1000, 1500] -- //
	// -- [1500, 2000] (44th), [2000, 3000] (45th) => [1500, 3000] -- //
	TH1D* Merge_HighMassBin( TH1D* h )
	{
		TH1D *h_return = new TH1D(h->GetName(), "", nMassBin, MassBinEdges );

		// -- [1000, 1200] (42th), [1200, 1500] (43th) => [1000, 1500] -- //
		Double_t Entry_M1000to1200 = h->GetBinContent(42);
		Double_t Entry_M1200to1500 = h->GetBinContent(43);

		Double_t Error_M1000to1200 = h->GetBinError(42);
		Double_t Error_M1200to1500 = h->GetBinError(43);

		// -- merging -- //
		Double_t Entry_M1000to1500 = Entry_M1000to1200 + Entry_M1200to1500;
		Double_t Error_M1000to1500 = sqrt( Error_M1000to1200*Error_M1000to1200 + Error_M1200to1500*Error_M1200to1500 );

		// -- [1500, 2000] (44th), [2000, 3000] (45th) => [1500, 3000] -- //
		Double_t Entry_M1500to2000 = h->GetBinContent(44);
		Double_t Entry_M2000to3000 = h->GetBinContent(45);

		Double_t Error_M1500to2000 = h->GetBinError(44);
		Double_t Error_M2000to3000 = h->GetBinError(45);

		// -- merging -- //
		Double_t Entry_M1500to3000 = Entry_M1500to2000 + Entry_M2000to3000;
		Double_t Error_M1500to3000 = sqrt( Error_M1500to2000*Error_M1500to2000 + Error_M2000to3000*Error_M2000to3000 );

		for(Int_t i=0; i<41; i++)
		{
			Int_t i_bin = i+1;

			Double_t Entry = h->GetBinContent(i_bin);
			Double_t Error = h->GetBinError(i_bin);

			h_return->SetBinContent(i_bin, Entry);
			h_return->SetBinError(i_bin, Error);
		}

		h_return->SetBinContent(42, Entry_M1000to1500);
		h_return->SetBinError(42, Error_M1000to1500);

		h_return->SetBinContent(43, Entry_M1500to3000);
		h_return->SetBinError(43, Error_M1500to3000);

		return h_return;
	}
};

void CalcUnc_AccDiff_FEWZ_vs_aMCNLO()
{
	Comparison *comp = new Comparison();
	comp->CalcAcc_aMCNLO();
	comp->Plot_Acc_aMCNLO_preFSR_vs_postFSR();

	comp->CalcAcc_FEWZ();
	comp->Plot_Acc_aMCNLO_vs_FEWZ();

	comp->Get_Acceptance_PDF4LHC();
	comp->Plot_Acc_aMCNLO_vs_FEWZ_PDF4LHC();

	comp->SaveResults_Unc();


}