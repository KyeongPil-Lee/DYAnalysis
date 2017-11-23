// -- creation time: 20 June, 2017.
// -- calculate the central value with full corrections -- //
// -- (it should be updated to calculate fiducial cross section also) -- //
#include <src/RooUnfold.cxx>
#include <src/RooUnfoldResponse.cxx>
#include <src/RooUnfoldBayes.cxx>
#include <src/RooUnfoldInvert.cxx>
#include <TStyle.h>
#include <Include/PlotTools.h>
#include <Include/DYAnalyzer.h> // -- nMassBin, Lumi, Lumi_HLTv4p2 is defined -- //

class DXSecTool
{
public:
	Double_t MassBinEdges[nMassBin+1];
	TString ROOTFilePath;

	// -- necessary inputs -- //
	TH1D* h_data_4p2;
	TH1D* h_data_4p3;

	TH1D* h_bkg_4p2;
	TH1D* h_bkg_4p3;

	RooUnfoldResponse* UnfoldRes_DetRes;

	TGraphAsymmErrors *g_AccEff;

	TGraphAsymmErrors* g_EffSF_4p2;
	TGraphAsymmErrors* g_EffSF_4p3;

	RooUnfoldResponse* UnfoldRes_FSR;

	// -- intermediate steps -- //
	TH1D* h_BkgSub_4p2;
	TH1D* h_BkgSub_4p3;

	TH1D* h_Unfolded_4p2;
	TH1D* h_Unfolded_4p3;

	TH1D* h_AccEffCorr_4p2;
	TH1D* h_AccEffCorr_4p3;

	TH1D* h_EffSFCorr_4p2;
	TH1D* h_EffSFCorr_4p3;

	TH1D* h_FSRCorr;

	// -- final result -- //
	TH1D* h_DiffXSec;

	DXSecTool()
	{
		TH1::AddDirectory(kFALSE);

		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		for(Int_t i=0; i<nMassBin+1; i++)
			this->MassBinEdges[i] = MassBinEdges_temp[i];

		this->ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

		this->Setup_Default(); // -- default setting for necessary inputs to calculate central value -- //
	}

	void Setup_ObservedYield( TH1D* _h_data_4p2, TH1D* _h_data_4p3 )
	{
		this->h_data_4p2 = (TH1D*)(_h_data_4p2)->Clone();
		this->h_data_4p3 = (TH1D*)(_h_data_4p3)->Clone();
	}

	void Setup_Bkg( TH1D* _h_bkg_4p2, TH1D* _h_bkg_4p3 )
	{
		this->h_bkg_4p2 = (TH1D*)(_h_bkg_4p2)->Clone();
		this->h_bkg_4p3 = (TH1D*)(_h_bkg_4p3)->Clone();
	}

	// -- calc. final cross section with current setting -- //
	void Run_FullChain()
	{
		this->Get_Yield_BkgSub();
		this->Get_Yield_Unfolded();
		this->Get_Yield_AccEff();
		this->Get_Yield_EffSF();
		this->Get_Yield_FSR();
		this->Get_DXSec();
	}

	TH1D* FinalResult()
	{
		return h_DiffXSec;
	}

protected:
	void Get_Yield_BkgSub()
	{
		// -- background subtraction: HLT v4.2 -- //
		this->h_BkgSub_4p2 = (TH1D*)this->h_data_4p2->Clone(); h_BkgSub_4p2->Sumw2();
		this->h_BkgSub_4p2->Add( h_bkg_4p2, -1 );

		// -- background subtraction: HLT v4.3 -- //
		this->h_BkgSub_4p3 = (TH1D*)this->h_data_4p3->Clone(); h_BkgSub_4p3->Sumw2();
		this->h_BkgSub_4p3->Add( h_bkg_4p3, -1 );

		// -- Remove negative bins -- //
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			// -- HLT v4.2 -- //
			Double_t nEvent_4p2 = this->h_BkgSub_4p2->GetBinContent(i_bin);
			if( nEvent_4p2 < 0 )
			{
				this->h_BkgSub_4p2->SetBinContent(i_bin, 0);
				this->h_BkgSub_4p2->SetBinError(i_bin, 0);
			}

			// -- HLT v4.3 -- //
			Double_t nEvent_4p3 = this->h_BkgSub_4p3->GetBinContent(i_bin);
			if( nEvent_4p3 < 0 )
			{
				this->h_BkgSub_4p3->SetBinContent(i_bin, 0);
				this->h_BkgSub_4p3->SetBinError(i_bin, 0);
			}
		}
	}

	void Get_Yield_Unfolded()
	{
		// -- HLTv4.2 -- //
		RooUnfoldResponse *UnfoldRes_HLTv4p2 = (RooUnfoldResponse*)this->UnfoldRes_DetRes->Clone();
		RooUnfoldBayes *UnfoldBayes_HLTv4p2 = new RooUnfoldBayes(UnfoldRes_HLTv4p2, this->h_BkgSub_4p2, 17); // -- iteration until convergence -- //
		this->h_Unfolded_4p2 = (TH1D*)UnfoldBayes_HLTv4p2->Hreco();

		// -- HLTv4.3 -- //
		RooUnfoldResponse *UnfoldRes_HLTv4p3 = (RooUnfoldResponse*)this->UnfoldRes_DetRes->Clone();
		RooUnfoldBayes *UnfoldBayes_HLTv4p3 = new RooUnfoldBayes(UnfoldRes_HLTv4p3, this->h_BkgSub_4p3, 17); // -- iteration until convergence -- //
		this->h_Unfolded_4p3 = (TH1D*)UnfoldBayes_HLTv4p3->Hreco();
	}

	void Get_Yield_AccEff()
	{
		// -- HLTv4p2 -- //
		this->h_AccEffCorr_4p2 = (TH1D*)this->h_Unfolded_4p2->Clone();
		this->Correction_AccEff( this->h_AccEffCorr_4p2, this->h_Unfolded_4p2, this->g_AccEff );

		// -- HLTv4p3 -- //
		this->h_AccEffCorr_4p3 = (TH1D*)this->h_Unfolded_4p3->Clone();
		this->Correction_AccEff( this->h_AccEffCorr_4p3, this->h_Unfolded_4p3, this->g_AccEff );
	}

	void Get_Yield_EffSF()
	{
		this->h_EffSFCorr_4p2 = (TH1D*)this->h_AccEffCorr_4p2->Clone();
		this->ApplyEffCorr_Yield( this->h_EffSFCorr_4p2, this->h_AccEffCorr_4p2, this->g_EffSF_4p2 );

		this->h_EffSFCorr_4p3 = (TH1D*)this->h_AccEffCorr_4p3->Clone();
		this->ApplyEffCorr_Yield( this->h_EffSFCorr_4p3, this->h_AccEffCorr_4p3, this->g_EffSF_4p3 );
	}

	void Get_Yield_FSR()
	{
		TH1D* h_EffSFCorr = (TH1D*)this->h_EffSFCorr_4p2->Clone();
		h_EffSFCorr->Add( this->h_EffSFCorr_4p3 );

		// -- use inverting method after following recommendation by stat.committee -- //
		RooUnfoldInvert *UnfoldInvert_FSR = new RooUnfoldInvert(UnfoldRes_FSR, h_EffSFCorr); 
		this->h_FSRCorr = (TH1D*)UnfoldInvert_FSR->Hreco();
	}

	void Get_DXSec()
	{
		TH1D* h_XSec = (TH1D*)this->h_FSRCorr->Clone();
		h_XSec->Scale( 1.0 / Lumi );
		this->h_DiffXSec = DivideEachBin_ByBinWidth( h_XSec );
	}

	void Setup_Default()
	{
		this->Setup_Default_ObservedYield();
		this->Setup_Default_Background();
		this->Setup_Default_Unfolding_DetRes();
		this->Setup_Default_AccEff();
		this->Setup_Default_EffSF();
		this->Setup_Default_Unfolding_FSR();
	}

	void Setup_Default_ObservedYield()
	{
		TString FileName = this->ROOTFilePath+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root";

		TH1D* h_data_4p2_temp = Get_Hist( FileName, "h_mass_OS_HLTv4p2_Data" ); h_data_4p2_temp->Sumw2();
		TH1D* h_data_4p3_temp = Get_Hist( FileName, "h_mass_OS_HLTv4p3_Data" ); h_data_4p3_temp->Sumw2();

		this->h_data_4p2 = this->Rebin_DYBin( h_data_4p2_temp );
		this->h_data_4p3 = this->Rebin_DYBin( h_data_4p3_temp );
	}

	// -- data driven bkg. (most of bkg.) + MC bkg. (WZ, ZZ only) -- //
	void Setup_Default_Background()
	{
		// -- MC bkg. after applying efficiency SF -- //
		TH1D *h_WZ_4p2 = this->Get_Histogram_MCBkg("HLTv4p2", "WZ");
		TH1D *h_ZZ_4p2 = this->Get_Histogram_MCBkg("HLTv4p2", "ZZ");

		TH1D *h_WZ_4p3 = this->Get_Histogram_MCBkg("HLTv4p3", "WZ");
		TH1D *h_ZZ_4p3 = this->Get_Histogram_MCBkg("HLTv4p3", "ZZ");

		// --- data-driven backgrounds -- //
		TFile* f_input_bkg_dataDriven = TFile::Open(this->ROOTFilePath + "/ROOTFile_Bkg_DataDrivenMethod.root");
		TH1D *h_diJet_FR = (TH1D*)f_input_bkg_dataDriven->Get("dijet")->Clone();
		TH1D *h_WJets_FR = (TH1D*)f_input_bkg_dataDriven->Get("wjets")->Clone();
		TH1D *h_ttbar_emu = (TH1D*)f_input_bkg_dataDriven->Get("ttbar")->Clone();
		TH1D *h_tW_emu = (TH1D*)f_input_bkg_dataDriven->Get("tW")->Clone();
		TH1D *h_DYTauTau_emu = (TH1D*)f_input_bkg_dataDriven->Get("DYtautau")->Clone();
		TH1D *h_WW_emu = (TH1D*)f_input_bkg_dataDriven->Get("WW")->Clone();

		f_input_bkg_dataDriven->Close();

		TH1D* h_bkg_noMCBkg = (TH1D*)h_diJet_FR->Clone();
		h_bkg_noMCBkg->Add( h_WJets_FR );
		h_bkg_noMCBkg->Add( h_ttbar_emu );
		h_bkg_noMCBkg->Add( h_tW_emu );
		h_bkg_noMCBkg->Add( h_DYTauTau_emu );
		h_bkg_noMCBkg->Add( h_WW_emu );

		this->h_bkg_4p2 = (TH1D*)h_bkg_noMCBkg->Clone();
		h_bkg_4p2->Scale( Lumi_HLTv4p2 / Lumi );
		h_bkg_4p2->Add( h_WZ_4p2 ); // -- add up MC bkg. -- //
		h_bkg_4p2->Add( h_ZZ_4p2 );

		this->h_bkg_4p3 = (TH1D*)h_bkg_noMCBkg->Clone();
		h_bkg_4p3->Scale( (Lumi - Lumi_HLTv4p2) / Lumi );
		h_bkg_4p3->Add( h_WZ_4p3 ); // -- add up MC bkg. -- //
		h_bkg_4p3->Add( h_ZZ_4p3 );
	}

	void Setup_Default_Unfolding_DetRes()
	{
		TString FileName = this->ROOTFilePath+"/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root";
		TFile *f_Unfold = TFile::Open( FileName );

		this->UnfoldRes_DetRes = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
		f_Unfold->Close();
	}

	void Setup_Default_AccEff()
	{
		TFile *f_AccEff = TFile::Open(this->ROOTFilePath+"/ROOTFile_AccEff.root"); f_AccEff->cd();
		this->g_AccEff = (TGraphAsymmErrors*)f_AccEff->Get("g_AccEff")->Clone();

		f_AccEff->Close();
	}

	void Setup_Default_EffSF()
	{
		TFile *f_AccEff = TFile::Open(this->ROOTFilePath + "/ROOTFile_AccEff.root"); f_AccEff->cd();

		TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff")->Clone();
		TGraphAsymmErrors *g_Eff_Corr_4p2 = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff_Corr_HLTv4p2")->Clone();
		TGraphAsymmErrors *g_Eff_Corr_4p3 = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff_Corr_HLTv4p3")->Clone();
		
		f_AccEff->Close();

		// -- Calculate efficiency scale factor for each mass bin: SF = Corrected Eff / Un-corrected Eff -- //
		this->g_EffSF_4p2 = MakeGraph_Ratio( g_Eff_Corr_4p2, g_Eff );
		this->g_EffSF_4p3 = MakeGraph_Ratio( g_Eff_Corr_4p3, g_Eff );
	}

	void Setup_Default_Unfolding_FSR()
	{
		TFile *f_FSR = TFile::Open(this->ROOTFilePath + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root"); f_FSR->cd();
		this->UnfoldRes_FSR = (RooUnfoldResponse*)f_FSR->Get("UnfoldRes")->Clone();
		
		f_FSR->Close();
	}

	TH1D* Get_Histogram_MCBkg( TString HLTType, TString Type )
	{
		TString FileName = this->ROOTFilePath + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root";
		TFile *f_MCBkg = TFile::Open( FileName ); f_MCBkg->cd();

		TString HistName = "";
		if( (HLTType == "HLTv4p2") || (HLTType == "HLTv4p3") )
			HistName = "h_mass_OS_"+HLTType+"_Norm_"+Type;
		else
			HistName = "h_mass_OS_Norm_"+Type;

		TH1D* h_mass = (TH1D*)f_MCBkg->Get( HistName )->Clone();
		h_mass = this->Rebin_DYBin( h_mass );

		f_MCBkg->Close();

		return h_mass;
	}

	void Correction_AccEff(TH1D *h_yield_AccEff, TH1D *h_yield, TGraphAsymmErrors *g_AccEff)
	{
		Int_t nBins = h_yield->GetNbinsX();
		for(Int_t i=0; i<nBins; i++)
		{
			Int_t i_bin = i+1;

			Double_t x_AccEff, y_AccEff;
			g_AccEff->GetPoint(i, x_AccEff, y_AccEff);

			Double_t Yield = h_yield->GetBinContent(i_bin);
			Double_t Yield_AfterAccEff = Yield / y_AccEff;

			Double_t Error = h_yield->GetBinError(i_bin);
			Double_t Error_AfterAccEff = Error / y_AccEff;

			// -- Set the central value -- //
			h_yield_AccEff->SetBinContent( i_bin, Yield_AfterAccEff );
			h_yield_AccEff->SetBinError( i_bin, Error_AfterAccEff );
		}
	}

	void ApplyEffCorr_Yield(TH1D *h_yield_EffCorr, TH1D *h_yield, TGraphAsymmErrors* g_EffCorr)
	{
		Int_t NPoints = g_EffCorr->GetN();
		Int_t nBins = h_yield->GetNbinsX();

		if( NPoints != nBins )
		{
			cout << "# Points in EffCorr != # Bins of yield histogram!" << endl;
			return;
		}

		for(Int_t i_p=0; i_p<NPoints; i_p++)
		{
			// -- Get g_EffCorr point -- //
			Double_t mass, EffCorr;
			g_EffCorr->GetPoint(i_p, mass, EffCorr);

			Int_t i_bin = i_p + 1;
			Double_t yield = h_yield->GetBinContent(i_bin);
			Double_t error_yield = h_yield->GetBinError(i_bin);

			Double_t yield_EffCorr = yield / EffCorr;
			Double_t error_yield_EffCorr = error_yield / EffCorr;

			h_yield_EffCorr->SetBinContent(i_bin, yield_EffCorr);
			h_yield_EffCorr->SetBinError(i_bin, error_yield_EffCorr);
		}
	}

	TH1D* Rebin_DYBin( TH1D* h_before )
	{
		return (TH1D*)h_before->Rebin(nMassBin, h_before->GetName(), this->MassBinEdges);
	}
};

