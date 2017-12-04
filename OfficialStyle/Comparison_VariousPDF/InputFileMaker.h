#include "Comparison_VariousPDF.cxx"
#include <Include/PlotTools.h>

class Tool_InputFileMaker
{
public:
	TString ROOTFilePath;
	TString AnalyzerPath;
	
	Tool_InputFileMaker()
	{
		this->ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		this->AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	}

	void MakeInputFile_EE( TFile *f_output )
	{
		this->FillROOTFile_Data_EE( f_output );
		this->FillROOTFile_NNPDF( f_output );
		this->FillROOTFile_OtherPDFs( f_output );
	}

	void MakeInputFile_MM( TFile *f_output )
	{
		this->FillROOTFile_Data_MM( f_output );
		this->FillROOTFile_NNPDF( f_output );
		this->FillROOTFile_OtherPDFs( f_output );
	}

	void MakeInputFile_LL( TFile *f_output )
	{
		this->FillROOTFile_Data_LL( f_output );
		this->FillROOTFile_NNPDF( f_output );
		this->FillROOTFile_OtherPDFs( f_output );
	}
protected:
	void FillROOTFile_Data_LL( TFile *f_output )
	{
		TString FileName_CenV = this->ROOTFilePath + "/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
		// -- differential cross section: convert TH1F to TH1D -- //
		TFile *f_input = TFile::Open( FileName_CenV );
		f_input->cd();
		TH1F* h_temp = (TH1F*)f_input->Get( "h1Combi" )->Clone();
		TH1D* h_DXSec = new TH1D();
		h_temp->Copy( *h_DXSec );
		h_DXSec->SetTitle("");
		h_DXSec->SetName(HistName_Data);
		///////////////////////////////////////////

		TH1D* h_AbsStatUnc = Get_Hist( FileName_CenV, "h1_dCS_from_statYield" );
		TH1D* h_RelStatUnc = ConvertHist_AbsToRel( h_DXSec, h_AbsStatUnc );
		h_RelStatUnc->SetName( HistName_RelStatUnc );
		TH1D* h_RelTotUnc = Extract_RelUnc( h_DXSec, HistName_RelTotUnc );

		h_DXSec = this->Rebin_MassBinEdges_Above200( h_DXSec );
		h_RelStatUnc = this->Rebin_MassBinEdges_Above200( h_RelStatUnc );
		h_RelTotUnc = this->Rebin_MassBinEdges_Above200( h_RelTotUnc );

		f_output->cd();
		h_DXSec->Write( HistName_Data );
		h_RelStatUnc->Write( HistName_RelStatUnc );
		h_RelTotUnc->Write( HistName_RelTotUnc );
	}

	void FillROOTFile_Data_MM( TFile *f_output )
	{
		TString FileName_MM = this->ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
		TH1D* h_DXSec = Get_Hist( FileName_MM, "h_DiffXsec_FSRCorr", HistName_Data );

		TH1D* h_RelStatUnc = Get_Hist( FileName_MM, "h_RelStatUnc_Percent", HistName_RelStatUnc );
		h_RelStatUnc->Scale( 0.01 );
		
		// TH1D* h_RelExpSystUnc = Get_Hist( FileName_MM, "h_RelSysUnc_Tot_Percent" );
		// TH1D* h_RelAccUnc = Get_Hist( FileName_MM, "h_RelSysUnc_Acc._Percent" );
		// TH1D* h_RelSystUnc = QuadSum_NoError( h_RelExpSystUnc, h_RelAccUnc );
		// h_RelSystUnc->Scale( 0.01 );

		// TH1D* h_RelTotUnc = QuadSum_NoError( h_RelSystUnc, h_RelStatUnc );
		// h_RelTotUnc->SetName( HistName_RelTotUnc );

		TH1D* h_RelTotUnc = Extract_RelUnc( h_DXSec ); // -- include stat. + syst. + lumi -- //
		h_RelTotUnc->SetName( HistName_RelTotUnc );

		h_DXSec = this->Rebin_MassBinEdges_Above200( h_DXSec );
		h_RelStatUnc = this->Rebin_MassBinEdges_Above200( h_RelStatUnc );
		h_RelTotUnc = this->Rebin_MassBinEdges_Above200( h_RelTotUnc );

		f_output->cd();

		h_DXSec->Write();
		h_RelStatUnc->Write();
		h_RelTotUnc->Write();
	}

	void FillROOTFile_Data_EE( TFile *f_output )
	{
		TString FileName = this->ROOTFilePath + "/DiffXsec_Electron_v8.root";
		TH1D* h_DXSec = Get_Hist( FileName, "h_DiffXSec", HistName_Data );
		TH1D* h_RelStatUnc = Get_Hist( FileName, "h_RelUnc_Stat", HistName_RelStatUnc ); h_RelStatUnc->Scale( 0.01 );
		// TH1D* h_RelTotUnc = Get_Hist( FileName, "h_RelUnc_Total", HistName_RelTotUnc ); h_RelTotUnc->Scale( 0.01 );

		TH1D* h_RelSystUnc = Get_Hist( FileName, "h_RelUnc_Syst" ); h_RelSystUnc->Scale(0.01);
		TH1D* h_RelTotUnc = QuadSum_NoError( h_RelStatUnc, h_RelSystUnc );
		TH1D* h_RelLumiUnc = this->MakeHist_RelLumiUnc( h_DXSec, 0.023 );
		h_RelTotUnc = QuadSum_NoError( h_RelTotUnc, h_RelLumiUnc ); // -- add up lumi. uncertainty -- //
		h_RelTotUnc->SetName( HistName_RelTotUnc );

		h_DXSec = this->Rebin_MassBinEdges_Above200( h_DXSec );
		h_RelStatUnc = this->Rebin_MassBinEdges_Above200( h_RelStatUnc );
		h_RelTotUnc = this->Rebin_MassBinEdges_Above200( h_RelTotUnc );

		f_output->cd();

		h_DXSec->Write();
		h_RelStatUnc->Write();
		h_RelTotUnc->Write();
	}

	void FillROOTFile_NNPDF( TFile *f_output )
	{
		// -- without PI -- //
		TString FileName_MM = this->ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
		TH1D* h_DXSec = Get_Hist(FileName_MM, "h_DiffXsec_FEWZ_NNPDF_NNLO", HistName_NNPDF);

		// // -- with PI -- //
		// TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		// TString FileName_Data = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYXSecNNLO_WithPI.root";
		// TH1D* h_DXSec = Get_Hist( FileName_Data, "h_DYDiffXSecNNLO_WithPI", HistName_NNPDF );

		TH1D* h_RelTotUnc = Extract_RelUnc( h_DXSec );
		h_RelTotUnc->SetName( HistName_NNPDF_RelUnc );

		h_DXSec = this->Rebin_MassBinEdges_Above200( h_DXSec );
		h_RelTotUnc = this->Rebin_MassBinEdges_Above200( h_RelTotUnc );

		f_output->cd();

		h_DXSec->Write();
		h_RelTotUnc->Write();
	}

	void FillROOTFile_OtherPDFs( TFile *f_output )
	{
		TString BasePath = this->AnalyzerPath + "/TheoryValues/NNLO_VariousPDF_Above200GeV";
		TH1D* h_DXSec_CT = Get_Hist(BasePath+"/CT14/ROOTFile_DY_FEWZ_NNLO_CT14nnlo.root", "h_DiffXsec", HistName_CT);
		h_DXSec_CT = this->MergeLastTwoBins( h_DXSec_CT );
		this->AssignScaleUnc_3Percent( h_DXSec_CT );
		// this->Add_PIContribution_Above200( h_DXSec_CT );
		TH1D* h_RelTotUnc_CT = Extract_RelUnc( h_DXSec_CT );
		h_RelTotUnc_CT->SetName( HistName_CT_RelUnc );

		TH1D* h_DXSec_MMHT = Get_Hist(BasePath+"/MMHT2014/ROOTFile_DY_FEWZ_NNLO_MMHT2014nnlo68cl.root", "h_DiffXsec", HistName_MMHT);
		h_DXSec_MMHT = this->MergeLastTwoBins( h_DXSec_MMHT );
		this->AssignScaleUnc_3Percent( h_DXSec_MMHT );
		// this->Add_PIContribution_Above200( h_DXSec_MMHT );
		TH1D* h_RelTotUnc_MMHT = Extract_RelUnc( h_DXSec_MMHT );
		h_RelTotUnc_MMHT->SetName( HistName_MMHT_RelUnc );

		TH1D* h_DXSec_HERAPDF = Get_Hist(BasePath+"/HERAPDF/ROOTFile_DY_FEWZ_NNLO_HERAPDF15NNLO_EIG.root", "h_DiffXsec", HistName_HERAPDF);
		h_DXSec_HERAPDF = this->MergeLastTwoBins( h_DXSec_HERAPDF );
		this->AssignScaleUnc_3Percent( h_DXSec_HERAPDF );
		// this->Add_PIContribution_Above200( h_DXSec_HERAPDF );
		TH1D* h_RelTotUnc_HERAPDF = Extract_RelUnc( h_DXSec_HERAPDF );
		h_RelTotUnc_HERAPDF->SetName( HistName_HERAPDF_RelUnc );

		TH1D* h_DXSec_ABM = Get_Hist(BasePath+"/ABM12LHC/ROOTFile_DY_FEWZ_NNLO_abm12lhc_5_nnlo.root", "h_DiffXsec", HistName_ABM);
		h_DXSec_ABM = this->MergeLastTwoBins( h_DXSec_ABM );
		this->AssignScaleUnc_3Percent( h_DXSec_ABM );
		// this->Add_PIContribution_Above200( h_DXSec_ABM );
		TH1D* h_RelTotUnc_ABM = Extract_RelUnc( h_DXSec_ABM );
		h_RelTotUnc_ABM->SetName( HistName_ABM_RelUnc );

		f_output->cd();

		h_DXSec_CT->Write();
		h_RelTotUnc_CT->Write();

		h_DXSec_MMHT->Write();
		h_RelTotUnc_MMHT->Write();

		h_DXSec_HERAPDF->Write();
		h_RelTotUnc_HERAPDF->Write();

		h_DXSec_ABM->Write();
		h_RelTotUnc_ABM->Write();
	}

	TH1D* MergeLastTwoBins( TH1D* h )
	{
		// const Int_t _nMassBin = 15;
		// Double_t MassBinEdges[_nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1200, 1500, 2000, 3000};

		const Int_t _nMassBin = 13;
		Double_t MassBinEdges[_nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

		TString HistName = h->GetName();
		// TH1D* h_Merge = new TH1D(HistName, "", _nMassBin, MassBinEdges);
		TH1D* h_Merge = (TH1D*)h->Clone(); // -- in order to maintain attributes -- //
		h_Merge = this->Rebin_MassBinEdges_Above200( h_Merge );

		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t BinCenter = h->GetBinCenter(i_bin);

			if( BinCenter < 1000 )
			{
				Double_t value = h->GetBinContent(i_bin);
				Double_t error = h->GetBinError(i_bin);

				h_Merge->SetBinContent(i_bin, value);
				h_Merge->SetBinError(i_bin, error);
			}
		}

		Double_t XSec_M1000to1200 = h->GetBinContent(12) * h->GetBinWidth(12);
		Double_t XSecErr_M1000to1200 = h->GetBinError(12) * h->GetBinWidth(12);

		Double_t XSec_M1200to1500 = h->GetBinContent(13) * h->GetBinWidth(13);
		Double_t XSecErr_M1200to1500 = h->GetBinError(13) * h->GetBinWidth(13);

		Double_t XSec_M1500to2000 = h->GetBinContent(14) * h->GetBinWidth(14);
		Double_t XSecErr_M1500to2000 = h->GetBinError(14) * h->GetBinWidth(14);

		Double_t XSec_M2000to3000 = h->GetBinContent(15) * h->GetBinWidth(15);
		Double_t XSecErr_M2000to3000 = h->GetBinError(15) * h->GetBinWidth(15);

		Double_t DiffXSec_M1000to1500 = (XSec_M1000to1200 + XSec_M1200to1500) / 500.0;
		Double_t DiffXSecErr_M1000to1500 = this->QuadratureSum( XSecErr_M1000to1200, XSecErr_M1200to1500 ) / 500.0; // -- assume no correlation -- //

		Double_t DiffXSec_M1500to3000 = (XSec_M1500to2000 + XSec_M2000to3000) / 1500.0;
		Double_t DiffXSecErr_M1500to3000 = this->QuadratureSum( XSecErr_M1500to2000, XSecErr_M2000to3000 ) / 1500.0;

		h_Merge->SetBinContent( 12, DiffXSec_M1000to1500 );
		h_Merge->SetBinError( 12, DiffXSecErr_M1000to1500 );

		h_Merge->SetBinContent( 13, DiffXSec_M1500to3000 );
		h_Merge->SetBinError( 13, DiffXSecErr_M1500to3000 );

		return h_Merge;
	}

	TH1D* Rebin_MassBinEdges_Above200( TH1D* h )
	{
		const Int_t _nMassBin = 13;
		Double_t MassBinEdges[_nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

		return (TH1D*)h->Rebin(_nMassBin, h->GetName(), MassBinEdges);
	}

	Double_t QuadratureSum( Double_t a, Double_t b )
	{
		return sqrt( a*a + b*b );
	}

	void AssignScaleUnc_3Percent( TH1D* h )
	{
		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t DiffXSec = h->GetBinContent(i_bin);
			Double_t Err_before = h->GetBinError(i_bin);
			Double_t Err_ScaleVar = DiffXSec * 0.03; // -- 3% for all mass bins -- //

			Double_t Err_after = sqrt(Err_before*Err_before + Err_ScaleVar*Err_ScaleVar);
			h->SetBinError(i_bin, Err_after);
		}
	}

	void Add_PIContribution_Above200( TH1D* h_DXSec )
	{
		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		TString FileName_Ratio = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYPI_DY_FEWZ.root";
		TH1D* h_ratio = Get_Hist(FileName_Ratio, "h_ratio");
		h_ratio = this->Rebin_MassBinEdges_Above200( h_ratio );

		Int_t nBin = h_DXSec->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t NNLOXSec = h_DXSec->GetBinContent(i_bin);
			Double_t RelErr_NNLOXSec = h_DXSec->GetBinError(i_bin) / NNLOXSec;

			Double_t Ratio = h_ratio->GetBinContent(i_bin);
			Double_t RelErr_ratio = h_ratio->GetBinError(i_bin) / Ratio;

			Double_t NNLOXSec_DYPI = NNLOXSec * Ratio;
			// -- error propagation -- //
			Double_t RelErr_total = sqrt(RelErr_NNLOXSec*RelErr_NNLOXSec + RelErr_ratio*RelErr_ratio);

			h_DXSec->SetBinContent(i_bin, NNLOXSec_DYPI);
			h_DXSec->SetBinError(i_bin, RelErr_total * NNLOXSec_DYPI); // -- absolute error -- //
		}
	}

	TH1D* MakeHist_RelLumiUnc( TH1D* h_format, Double_t RelLumiUnc )
	{
		TH1D* h_RelLumiUnc = (TH1D*)h_format->Clone();
		Int_t nBin = h_RelLumiUnc->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			h_RelLumiUnc->SetBinContent(i_bin, RelLumiUnc );
			h_RelLumiUnc->SetBinError(i_bin, 0);
		}

		return h_RelLumiUnc;
	}
};