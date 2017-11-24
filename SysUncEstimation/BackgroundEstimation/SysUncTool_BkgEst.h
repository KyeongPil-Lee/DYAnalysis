#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooGlobalFunc.h>
#include <RooPlot.h>
#include <TStyle.h>
#include <TRandom3.h>

#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>
#include <FullChain/FullChain.h>
#define nIter 1000
using namespace RooFit;

Double_t MassBinEdges2015[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
									 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
									 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
									 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
									 830, 1000, 1500, 3000};

class HistogramContainer
{
public:
	TString Type;

	TH1D* h_nEvent;
	TH1D* h_AbsUnc_Stat;
	TH1D* h_AbsUnc_Syst;

	TH1D* h_nEvent_4p2;
	TH1D* h_AbsUnc_Stat_4p2;
	TH1D* h_AbsUnc_Syst_4p2;

	TH1D* h_nEvent_4p3;
	TH1D* h_AbsUnc_Stat_4p3;
	TH1D* h_AbsUnc_Syst_4p3;

	HistogramContainer(TString _Type, Bool_t isDataDriven = kTRUE )
	{
		this->Type = _Type;

		if( isDataDriven )
			this->SetHistsDataDrivenBkg();
		else // -- MC-bkg. case -- //
			this->SetHistsMCBkg();
	}

	void Save( TFile *f_output )
	{
		f_output->cd();
	}

private:
	void SetHistsDataDrivenBkg()
	{
		TString FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileName = FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root";
		this->h_nEvent = Get_Hist( fileName, this->Type );
		this->h_AbsUnc_Stat = Get_Hist( fileName, this->Type+"_stat" );
		this->h_AbsUnc_Syst = Get_Hist( fileName, this->Type+"_systematic" );

		this->SetHist4p24p3_ByLumiRatio();
	}

	void SetHist4p24p3_ByLumiRatio()
	{
		Double_t LumiRatio4p2 = Lumi_HLTv4p2 / Lumi;
		this->h_nEvent_4p2 = (TH1D*)this->h_nEvent->Clone();
		this->h_nEvent_4p2->Scale( LumiRatio4p2 );

		this->h_nEvent_4p3 = (TH1D*)this->h_nEvent->Clone();
		this->h_nEvent_4p3->Scale( 1-LumiRatio4p2 );

		// -- will be modified in next loop -- //
		this->h_AbsUnc_Stat_4p2 = (TH1D*)this->h_AbsUnc_Stat->Clone();
		this->h_AbsUnc_Syst_4p2 = (TH1D*)this->h_AbsUnc_Syst->Clone();
		this->h_AbsUnc_Stat_4p3 = (TH1D*)this->h_AbsUnc_Stat->Clone();
		this->h_AbsUnc_Syst_4p3 = (TH1D*)this->h_AbsUnc_Syst->Clone();

		Int_t nBin = this->h_nEvent->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t nEvent = this->h_nEvent->GetBinContent(i_bin);
			Double_t AbsUnc_Stat = this->h_AbsUnc_Stat->GetBinContent(i_bin);
			Double_t AbsUnc_Syst = this->h_AbsUnc_Syst->GetBinContent(i_bin);

			Double_t RelUnc_Stat = 0;
			Double_t RelUnc_Syst = 0;
			if( nEvent != 0 )
			{
				RelUnc_Stat = AbsUnc_Stat / nEvent;
				RelUnc_Syst = AbsUnc_Syst / nEvent;
			}

			Double_t nEvent_4p2 = this->h_nEvent_4p2->GetBinContent(i_bin);
			Double_t nEvent_4p3 = this->h_nEvent_4p3->GetBinContent(i_bin);

			Double_t AbsUnc_Stat_4p2 = nEvent_4p2*RelUnc_Stat;
			Double_t AbsUnc_Syst_4p2 = nEvent_4p2*RelUnc_Syst;

			Double_t AbsUnc_Stat_4p3 = nEvent_4p3*RelUnc_Stat;
			Double_t AbsUnc_Syst_4p3 = nEvent_4p3*RelUnc_Syst;

			this->h_AbsUnc_Stat_4p2->SetBinContent(i_bin, AbsUnc_Stat_4p2 );
			this->h_AbsUnc_Syst_4p2->SetBinContent(i_bin, AbsUnc_Syst_4p2 );

			this->h_AbsUnc_Stat_4p3->SetBinContent(i_bin, AbsUnc_Stat_4p3 );
			this->h_AbsUnc_Syst_4p3->SetBinContent(i_bin, AbsUnc_Syst_4p3 );
		}
	}

	void SetHistsMCBkg()
	{
		this->h_nEvent_4p2 = this->GetHistogram_MCBkg( "HLTv4p2", this->Type);
		this->h_nEvent_4p3 = this->GetHistogram_MCBkg( "HLTv4p3", this->Type);

		// -- will be filled with the correct values later -- //
		this->h_AbsUnc_Stat_4p2 = (TH1D*)this->h_nEvent_4p2->Clone();
		this->h_AbsUnc_Syst_4p2 = (TH1D*)this->h_nEvent_4p2->Clone();
		this->h_AbsUnc_Stat_4p3 = (TH1D*)this->h_nEvent_4p3->Clone();
		this->h_AbsUnc_Syst_4p3 = (TH1D*)this->h_nEvent_4p3->Clone();

		Double_t RelUnc_xSec_WZ = 0.4052; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-13-011/index.html -- //
		Double_t RelUnc_xSec_ZZ = 0.1087; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-13-005/index.html -- //
		Double_t RelUnc_xSec = 0;
		if( this->Type == "WZ" ) RelUnc_xSec = RelUnc_xSec_WZ;
		else if( this->Type == "ZZ" ) RelUnc_xSec = RelUnc_xSec_ZZ;
		else
		{
			cout << "There's no corresponding cross section uncertainty for " << this->Type << "! ... please check" << endl;
			return;
		}

		Int_t nBin = this->h_nEvent_4p2->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t AbsUnc_Stat_4p2 = this->h_nEvent_4p2->GetBinError(i_bin);
			Double_t AbsUnc_Stat_4p3 = this->h_nEvent_4p3->GetBinError(i_bin);

			this->h_AbsUnc_Stat_4p2->SetBinContent(i_bin, AbsUnc_Stat_4p2);
			this->h_AbsUnc_Stat_4p2->SetBinError(i_bin, 0);
			this->h_AbsUnc_Stat_4p3->SetBinContent(i_bin, AbsUnc_Stat_4p3);
			this->h_AbsUnc_Stat_4p3->SetBinError(i_bin, 0);

			Double_t nEvent_4p2 = this->h_nEvent_4p2->GetBinContent(i_bin);
			Double_t nEvent_4p3 = this->h_nEvent_4p3->GetBinContent(i_bin);
			Double_t AbsUnc_Syst_4p2 = RelUnc_xSec * nEvent_4p2;
			Double_t AbsUnc_Syst_4p3 = RelUnc_xSec * nEvent_4p3;

			this->h_AbsUnc_Syst_4p2->SetBinContent(i_bin, AbsUnc_Syst_4p2);
			this->h_AbsUnc_Syst_4p2->SetBinError(i_bin, 0);
			this->h_AbsUnc_Syst_4p3->SetBinContent(i_bin, AbsUnc_Syst_4p3);
			this->h_AbsUnc_Syst_4p3->SetBinError(i_bin, 0);
		}

	}

	TH1D* GetHistogram_MCBkg( TString HLTType, TString Type )
	{
		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString FileName = ROOTFilePath + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root";

		TString HistName = "";
		if( (HLTType == "HLTv4p2") || (HLTType == "HLTv4p3") )
			HistName = "h_mass_OS_"+HLTType+"_Norm_"+Type;
		else
			HistName = "h_mass_OS_Norm_"+Type;

		TH1D* h_mass = Get_Hist(FileName, HistName);
		h_mass = this->Rebin_DYBin( h_mass );

		return h_mass;
	}

	TH1D* Rebin_DYBin( TH1D* h_before )
	{
		return (TH1D*)h_before->Rebin(nMassBin, h_before->GetName(), MassBinEdges2015);
	}
};

class Randomizer
{
public:
	HistogramContainer* hists;

	vector< TH1D* > vec_RanHistBkg_4p2;
	vector< TH1D* > vec_RanHistBkg_4p3;

	TH1D* h_XSecCV; // -- for x-check -- //
	vector< TH1D* > vec_RanHistXSec;

	TH1D* h_RelUnc;

	Randomizer( HistogramContainer* _hists )
	{
		this->hists = _hists;
	}

	void MakeRandomizedHists(TString type )
	{
		if( type == "Stat" )
			this->Randomization_UnCorrBtwBins_Stat();
		else if( type == "Syst" )
			this->Randomization_CorrBtwBins_Syst();
	}

	void CalcXSec_All()
	{
		this->h_XSecCV = this->CalcXSec( hists->h_nEvent_4p2, hists->h_nEvent_4p3 );

		for(Int_t iter=0; iter<nIter; iter++)
		{
			TH1D* h_XSec = this->CalcXSec( this->vec_RanHistBkg_4p2[iter], this->vec_RanHistBkg_4p3[iter] );
			this->vec_RanHistXSec.push_back( h_XSec );
		}
	}

	void FitGaussian_GetSigma( TString type )
	{
		this->h_RelUnc = new TH1D("h_RelUnc_"+type, "", nMassBin, MassBinEdges2015);

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			TTree *tree = this->makeTTree(i_bin);
			tree->Draw("RelDiff >> htemp");
			TH1F *hTemp = (TH1F*)gPad->GetPrimitive("htemp");
			Double_t rms = hTemp->GetRMS();
			delete hTemp;

			Double_t BinCenter = ( MassBinEdges2015[i] + MassBinEdges2015[i+1] ) / 2.0;
			// Double_t RangeMax = 0.1;
			// Double_t Sigma_Init = 0.01;
			Double_t RangeMax = 4*rms;
			Double_t Sigma_Init = rms;
			// this->Set_FitInitValues( BinCenter, RangeMax, Sigma_Init );

			RooRealVar RelDiff("RelDiff","(#sigma_{Smeared} - #sigma_{CV}) / #sigma_{CV}", (-1)*RangeMax, RangeMax);
			RooDataSet data("data","data", tree, RelDiff) ; // -- Name of the variable should be same with the branch name in the tree -- //

			// --- Make plot of binned dataset showing Poisson error bars (RooFit default)
			RooPlot* frame = RelDiff.frame( Title(TString::Format("%.0lf < M < %.0lf (%02d bin)", MassBinEdges2015[i], MassBinEdges2015[i+1], i_bin)) );
			
			// -- Fit a Gaussian p.d.f to the data
			RooRealVar mean("mean", "mean", 0, -2, 2) ;
			RooRealVar sigma("sigma", "sigma", Sigma_Init, 0.00001, 2);
			RooGaussian gauss("gauss", "gauss", RelDiff, mean, sigma);
			gauss.fitTo(data);

			data.plotOn(frame, Binning(50));
			gauss.plotOn(frame);
			gauss.paramOn(frame,Layout(0.6, 0.9, 0.9));
			frame->getAttText()->SetTextSize(0.02);

			TString CanvasName = TString::Format("c_%s_RelDiff_Bin%02d", type.Data(), i_bin);

			TCanvas *c = new TCanvas(CanvasName, "", 700, 700); c->cd();
			frame->Draw();

			c->SaveAs(CanvasName+".pdf");
			
			Double_t Sigma = sigma.getVal();
			// Double_t SigmaErr = sigma.getError();

			this->h_RelUnc->SetBinContent(i_bin, Sigma);
			this->h_RelUnc->SetBinError(i_bin, 0);
		}

		this->CrossCheck_CV( type );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();
		this->h_RelUnc->Write();
	}

	void CrossCheck_CV( TString type )
	{
		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileName_Ref = ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
		TH1D* h_Ref = Get_Hist(fileName_Ref, "h_DiffXsec_FSRCorr");
		HistInfo *hist_Ref = new HistInfo( kRed, "Reference", h_Ref);
		HistInfo *hist_CV = new HistInfo( kBlack, "C.V.", this->h_XSecCV );
		hist_CV->CalcRatio_DEN( hist_Ref->h );


		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString canvasName = "./Local/c_CrossCheck_DXSec_"+type;
		SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 1, 1 );

		c->cd();
		TopPad->cd();

		hist_CV->Draw("EPSAME");
		hist_Ref->Draw("EPSAME");
		SetHistFormat_TopPad( hist_CV->h, "d#sigma/dm [pb/GeV]");

		TLegend *legend;
		SetLegend( legend, 0.70, 0.82, 0.97, 0.95);
		hist_CV->AddToLegend( legend );
		hist_Ref->AddToLegend( legend );
		legend->Draw();

		c->cd();
		BottomPad->cd();

		hist_CV->DrawRatio( "EPSAME" );
		SetHistFormat_BottomPad( hist_CV->h_ratio, "m [GeV]", "C.V/Ref", 0.9, 1.1 );
		hist_CV->h_ratio->GetYaxis()->SetNdivisions( 005 );

		c->SaveAs(".pdf");	
	}

private:
	void Randomization_UnCorrBtwBins_Stat()
	{
		TRandom3 eran;
		eran.SetSeed(0);

		for(Int_t iter=0; iter<nIter; iter++)
		{
			TH1D *h_temp_4p2 = (TH1D*)this->hists->h_nEvent_4p2->Clone();
			TH1D *h_temp_4p3 = (TH1D*)this->hists->h_nEvent_4p3->Clone();

			Int_t nBin = h_temp_4p2->GetNbinsX();
			for(Int_t i=0; i<nBin; i++)
			{
				Int_t i_bin = i+1;
				
				// -- 4.2 & 4.3 should move to same direction -- //
				Double_t RandomValue = eran.Gaus(0.0, 1.0);

				Double_t nEventCV_4p2 = this->hists->h_nEvent_4p2->GetBinContent(i_bin);
				Double_t AbsUnc_4p2 = this->hists->h_AbsUnc_Stat_4p2->GetBinContent(i_bin);
				Double_t dN_4p2 = RandomValue * AbsUnc_4p2;
				h_temp_4p2->SetBinContent(i_bin, nEventCV_4p2+dN_4p2);
				h_temp_4p2->SetBinError(i_bin, 0); // -- error is not necessary for these histograms -- //

				Double_t nEventCV_4p3 = this->hists->h_nEvent_4p3->GetBinContent(i_bin);
				Double_t AbsUnc_4p3 = this->hists->h_AbsUnc_Stat_4p3->GetBinContent(i_bin);
				Double_t dN_4p3 = RandomValue * AbsUnc_4p3;
				h_temp_4p3->SetBinContent(i_bin, nEventCV_4p3+dN_4p3);
				h_temp_4p3->SetBinError(i_bin, 0); // -- error is not necessary for these histograms -- //
			}

			this->vec_RanHistBkg_4p2.push_back( h_temp_4p2 );
			this->vec_RanHistBkg_4p3.push_back( h_temp_4p3 );
		}
	}

	void Randomization_CorrBtwBins_Syst()
	{
		TRandom3 eran;
		eran.SetSeed(0);

		for(Int_t iter=0; iter<nIter; iter++)
		{
			// -- use same random value for all bins: move to same direction at once -- //
			Double_t RandomValue = eran.Gaus(0.0, 1.0);

			TH1D *h_temp_4p2 = (TH1D*)this->hists->h_nEvent_4p2->Clone();
			TH1D *h_temp_4p3 = (TH1D*)this->hists->h_nEvent_4p3->Clone();

			Int_t nBin = h_temp_4p2->GetNbinsX();
			for(Int_t i=0; i<nBin; i++)
			{
				Int_t i_bin = i+1;
		
				Double_t nEventCV_4p2 = this->hists->h_nEvent_4p2->GetBinContent(i_bin);
				Double_t AbsUnc_4p2 = this->hists->h_AbsUnc_Syst_4p2->GetBinContent(i_bin);
				Double_t dN_4p2 = RandomValue * AbsUnc_4p2;
				h_temp_4p2->SetBinContent(i_bin, nEventCV_4p2+dN_4p2);
				h_temp_4p2->SetBinError(i_bin, 0); // -- error is not necessary for these histograms -- //

				Double_t nEventCV_4p3 = this->hists->h_nEvent_4p3->GetBinContent(i_bin);
				Double_t AbsUnc_4p3 = this->hists->h_AbsUnc_Syst_4p3->GetBinContent(i_bin);
				Double_t dN_4p3 = RandomValue * AbsUnc_4p3;
				h_temp_4p3->SetBinContent(i_bin, nEventCV_4p3+dN_4p3);
				h_temp_4p3->SetBinError(i_bin, 0); // -- error is not necessary for these histograms -- //
			}

			this->vec_RanHistBkg_4p2.push_back( h_temp_4p2 );
			this->vec_RanHistBkg_4p3.push_back( h_temp_4p3 );
		}
	}

	TH1D* CalcXSec( TH1D* h_bkgRand_4p2, TH1D* h_bkgRand_4p3 )
	{
		DXSecTool* tool = new DXSecTool();

		TH1D* h_bkg_4p2_original = (TH1D*)tool->h_bkg_4p2->Clone();
		TH1D* h_bkg_4p3_original = (TH1D*)tool->h_bkg_4p3->Clone();

		TH1D* h_bkg_4p2_new = (TH1D*)h_bkg_4p2_original->Clone();
		h_bkg_4p2_new->Add( this->hists->h_nEvent_4p2, -1 ); // -- subtract original histogram -- //
		h_bkg_4p2_new->Add( h_bkgRand_4p2, 1 );

		TH1D* h_bkg_4p3_new = (TH1D*)h_bkg_4p3_original->Clone();
		h_bkg_4p3_new->Add( this->hists->h_nEvent_4p3, -1 ); // -- subtract original histogram -- //
		h_bkg_4p3_new->Add( h_bkgRand_4p3, 1 );

		tool->Setup_Bkg( h_bkg_4p2_new, h_bkg_4p3_new );
		tool->Run_FullChain();

		return tool->FinalResult();
	}

	TTree* makeTTree(Int_t i_bin) 
	{
		TTree* tree = new TTree("tree","tree");

		Double_t* RelDiff = new Double_t;

		tree->Branch("RelDiff", RelDiff, "RelDiff/D");

		Double_t value_CV = this->h_XSecCV->GetBinContent(i_bin);

		for(Int_t iter=0; iter<nIter; iter++)
		{
			Double_t value_Smeared = this->vec_RanHistXSec[iter]->GetBinContent(i_bin);
			*RelDiff = ( value_Smeared - value_CV ) / value_CV;

			// printf("[\t%d th RelDiff = %lf]\n", i, *RelDiff);

			tree->Fill();
		}

		return tree;
	}
};

class SysUncTool
{
public:
	TString sampleName;

	vector< HistogramContainer* > vec_Hists;	
	HistogramContainer* Hists;
	// HistogramContainer* Hists_ttbar;
	// HistogramContainer* Hists_tW;
	// HistogramContainer* Hists_DYtautau;
	// HistogramContainer* Hists_WJets;
	// HistogramContainer* Hists_QCD;
	// HistogramContainer* Hists_WW;
	// HistogramContainer* Hists_WZ;
	// HistogramContainer* Hists_ZZ;


	SysUncTool(TString _sampleName)
	{
		this->sampleName = _sampleName;
		this->Init();
	}

	void Estimate()
	{
		TFile *f_output = TFile::Open("ROOTFile_RelUnc_BkgEst_CorrelationBtwBins_"+this->sampleName+".root", "RECREATE");
		for( const auto& hists : vec_Hists )
		{
			TString sampleName = hists->Type;
			Randomizer* randomizer_Stat = new Randomizer( hists );
			randomizer_Stat->MakeRandomizedHists( "Stat" );
			randomizer_Stat->CalcXSec_All();
			randomizer_Stat->FitGaussian_GetSigma( sampleName+"_Stat" );
			randomizer_Stat->Save( f_output );

			Randomizer* randomizer_Syst = new Randomizer( hists );
			randomizer_Syst->MakeRandomizedHists( "Syst" );
			randomizer_Syst->CalcXSec_All();
			randomizer_Syst->FitGaussian_GetSigma(sampleName+"_Syst");
			randomizer_Syst->Save( f_output );
		}

		f_output->Close();
	}

private:
	void Init()
	{
		Bool_t isDataDriven = kTRUE;
		if( this->sampleName == "WZ" || this->sampleName == "ZZ" ) isDataDriven = kFALSE;

		this->Hists = new HistogramContainer( this->sampleName, isDataDriven ); vec_Hists.push_back( Hists );

		// this->Hists_ttbar = new HistogramContainer( "ttbar" ); vec_Hists.push_back( Hists_ttbar );
		// this->Hists_tW = new HistogramContainer( "tW" ); vec_Hists.push_back( Hists_tW );
		// this->Hists_DYtautau = new HistogramContainer( "DYtautau" ); vec_Hists.push_back( Hists_DYtautau );
		// this->Hists_WJets = new HistogramContainer( "wjets" ); vec_Hists.push_back( Hists_WJets );
		// this->Hists_QCD = new HistogramContainer( "dijet" ); vec_Hists.push_back( Hists_QCD );
		// this->Hists_WW = new HistogramContainer( "WW" ); vec_Hists.push_back( Hists_WW );

		// this->Hists_WZ = new HistogramContainer( "WZ", kFALSE ); vec_Hists.push_back( Hists_WZ );
		// this->Hists_ZZ = new HistogramContainer( "ZZ", kFALSE ); vec_Hists.push_back( Hists_ZZ );
	}
};

