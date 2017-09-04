#include <Include/PlotTools.h>
#include <Include/DYAnalyzer.h> // -- include Lumi -- //

class HistStyle
{
public:
	TString Tag; // -- exact name used to get histogram from root file -- //
	TString Name; // -- description of histogram -- //

	TString XTitle;
	TString YTitle;

	Int_t nRebin;

	Bool_t Flag_XRange;
	Double_t xMin;
	Double_t xMax;

	Bool_t Flag_YRange;
	Double_t yMin;
	Double_t yMax;

	Bool_t Flag_RatioRange;
	Double_t ratioMin;
	Double_t ratioMax;

	Bool_t Flag_LogX;
	Bool_t Flag_LogY;

	HistStyle() { this->Init(); };
	HistStyle( TString _Tag, TString _Name ) : HistStyle()
	{
		this->Tag = _Tag;
		this->Name = _Name;
	}

	void Set_LogX( Bool_t Flag = kTRUE ) { this->Flag_LogX = Flag; }
	void Set_LogY( Bool_t Flag = kTRUE ) { this->Flag_LogY = Flag; }

	void Set_Titles( TString _XTitle, TString _YTitle )
	{
		this->XTitle = _XTitle;
		this->YTitle = _YTitle;
	}

	void Set_nRebin( Int_t _nRebin ) { this->nRebin = _nRebin; }

	void Set_XRange( Double_t _Min, Double_t _Max )
	{
		this->Flag_XRange = kTRUE;
		this->xMin = _Min;
		this->xMax = _Max;
	}

	void Set_YRange( Double_t _Min, Double_t _Max )
	{
		this->Flag_YRange = kTRUE;
		this->yMin = _Min;
		this->yMax = _Max;
	}

private:
	void Init()
	{
		Flag_XRange = kFALSE;
		Flag_YRange = kFALSE;
		Flag_RatioRange = kFALSE;

		Flag_LogX = kFALSE;
		Flag_LogY = kFALSE;
	}

};

class SampleInfo
{
public:
	TString Tag;
	TString Name; // -- TLatex format -- //
	Bool_t Flag_MC;

	Bool_t Flag_MultiSamples;
	vector< SampleInfo* > vec_SubSampleInfo;

	TString FileName;
	Double_t XSec;
	Double_t nEvent_Generated;
	Double_t NormFactor;

	Int_t Color;

	SampleInfo()
	{
		this->Init();
	}

	SampleInfo( TString _Tag, TString _Name, Bool_t _Flag_MC )  : SampleInfo()
	{
		this->Tag = _Tag;
		this->Name = _Name;
		this->Flag_MC = _Flag_MC;
	}

	void Set_FileName( TString _FileName ) { this->FileName = _FileName; }
	void Set_XSec( Double_t _XSec ) { this->XSec = _XSec; }
	void Set_nEvent_Generated( Double_t _nEvent_Generated ) { this->nEvent_Generated = _nEvent_Generated; }
	void Set_Color( Int_t _color ) { this->Color = _color; }

	void Set_MultiSamples( SampleInfo* SInfo )
	{
		this->Flag_MultiSamples = kTRUE;
		this->Color = SInfo->Color;
		this->vec_SubSampleInfo.push_back( SInfo );
	}

	void Calc_NormFactor( Double_t _LUMI )
	{
		if( !this->Flag_MC )
		{
			cout << "[Calc_NormFactor] should not be run on the data!" << endl;
			this->NormFactor = -999;
			return;
		}

		Double_t nEvent_Predicted = _LUMI * XSec;
		this->NormFactor = nEvent_Predicted / nEvent_Generated;
		printf("[%s] norm. factor to lumi = %lf: %lf\n", this->Tag.Data(), _LUMI, this->NormFactor);
	}

private:
	void Init()
	{
		Flag_MultiSamples = kFALSE;
	}


};

class DrawingTool
{
public:
	vector< SampleInfo* > vec_BkgInfo;
	// vector< SampleInfo* > vec_SigInfo;

	SampleInfo* SignalInfo;	
	SampleInfo* DataInfo;

	vector< HistStyle* > vec_HistStyle;

	// -- should be cleared in each histogram loop -- //
	HistInfo* Hist_Data;
	HistInfo* Hist_Signal;
	vector< HistInfo* > vec_Hist_Bkg;

	DrawingTool()
	{
		this->Set_SampleInfo();
		this->Set_HistStyle();
		TH1::AddDirectory(kFALSE);
	}

	void Draw_All()
	{
		for(const auto &HS : this->vec_HistStyle )
			this->Draw_Hist( HS );
	}

	void Save( TFile *f_output )
	{

	}

private:
	void Draw_Hist( HistStyle* HS )
	{
		this->Set_HistInfo( HS );

		// -- MC Stack -- //
		THStack* hs_MC = this->Make_MCStack();

		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString CanvasName = "c_"+HS->Tag;
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, HS->Flag_LogX, HS->Flag_LogY );

		c->cd();
		TopPad->cd();

		// Print_Histogram( this->Hist_Data->h );

		TH1D* h_format = (TH1D*)this->Hist_Data->h->Clone();
		h_format->Reset("ICES");

		h_format->Draw("");
		hs_MC->Draw("HISTSAME");
		this->Hist_Data->Draw("EPSAME");
		h_format->Draw("AXISSAME");

		// Print_Histogram( this->Hist_Data->h );

		SetHistFormat_TopPad( h_format, HS->YTitle );
		if( HS->Flag_XRange ) h_format->GetXaxis()->SetRangeUser( HS->xMin, HS->xMax );
		if( HS->Flag_YRange ) h_format->GetYaxis()->SetRangeUser( HS->yMin, HS->yMax );

		// -- TLegend -- //
		TLegend *legend = this->Make_Legend();
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );

		c->cd();
		BottomPad->cd();

		TH1D* h_totMC = (TH1D*)this->Hist_Signal->h->Clone();
		for( const auto& Hist_Bkg : this->vec_Hist_Bkg )
			h_totMC->Add( Hist_Bkg->h );

		TH1D* h_ratio = (TH1D*)Hist_Data->h->Clone();
		this->Hist_Data->h->Sumw2();
		h_totMC->Sumw2();
		h_ratio->Divide( this->Hist_Data->h, h_totMC ); // -- data/MC -- //

		h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( h_ratio, HS->XTitle, "Data/MC", 0.4, 1.6 );

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}

	void Set_HistInfo( HistStyle *HS )
	{
		// -- initialization -- //
		// delete Hist_Data;
		// delete Hist_Signal;
		this->Hist_Data = NULL;
		this->Hist_Signal = NULL;
		vec_Hist_Bkg.clear();

		// -- data -- //
		TH1D* h_Data = this->Make_Hist( HS, this->DataInfo );
		Print_Histogram( h_Data );
		this->Hist_Data = new HistInfo( this->DataInfo->Color, this->DataInfo->Name, h_Data );

		// -- signal MC -- //
		TH1D* h_signal = this->Make_Hist( HS, this->SignalInfo );
		this->Hist_Signal = new HistInfo( SignalInfo->Color, SignalInfo->Name, h_signal );

		// -- background MCs -- //
		for( const auto& BkgInfo : this->vec_BkgInfo )
		{
			TH1D* h_bkg = this->Make_Hist( HS, BkgInfo );
			HistInfo* Hist_Bkg = new HistInfo( BkgInfo->Color, BkgInfo->Name, h_bkg );
			this->vec_Hist_Bkg.push_back( Hist_Bkg );
		}

	}

	THStack* Make_MCStack()
	{
		THStack *hs = new THStack("hs_MC", "");
		for( const auto& Hist_Bkg : this->vec_Hist_Bkg )
		{
			Hist_Bkg->h->SetFillColorAlpha( Hist_Bkg->Color, 1 );
			hs->Add( Hist_Bkg->h );
		}

		this->Hist_Signal->h->SetFillColorAlpha( this->Hist_Signal->Color, 1 );
		hs->Add( this->Hist_Signal->h );

		return hs;
	}

	TLegend* Make_Legend()
	{
		TLegend *legend;
		SetLegend( legend );
		this->Hist_Data->AddToLegend( legend );
		this->Hist_Signal->AddToLegend( legend );
		for (auto rit = this->vec_Hist_Bkg.rbegin(); rit < this->vec_Hist_Bkg.rend(); ++rit) // -- reverse order -- //
			(*rit)->AddToLegend( legend );

		return legend;
	}

	TH1D* Make_Hist( HistStyle* HS, SampleInfo* SInfo )
	{
		TH1D* h = NULL;

		if( SInfo->Flag_MultiSamples )
		{
			// -- loop over sub-samples & add up -- //
			for( const auto& SubSInfo : SInfo->vec_SubSampleInfo )
			{
				TString HistName = HS->Tag + "_" + SubSInfo->Tag;
				TH1D* h_sub = Get_Hist( SubSInfo->FileName, HistName );
				if( SubSInfo->Flag_MC )
					h_sub->Scale( SubSInfo->NormFactor );

				if( h == NULL )
					h = (TH1D*)h_sub->Clone();
				else
					h->Add( h_sub );
			}
		}
		else
		{
			TString HistName = HS->Tag + "_" + SInfo->Tag;
			h = Get_Hist( SInfo->FileName, HistName );
			if( SInfo->Flag_MC )
				h->Scale( SInfo->NormFactor );
		}

		//-- DY-Bin --//
		if( HS->Name.Contains( "DY Bin" ) )
		{
			// const Int_t nMassBin = 43;
			Double_t MassBinEdges[nMassBin+1] = 
			{
				15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
				64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
				110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
				200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
				830, 1000, 1500, 3000
			};

			h = (TH1D*)h->Rebin(nMassBin, h->GetName(), MassBinEdges);
		}

		return h;
	}

	void Set_SampleInfo()
	{
		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString CommonInput_MC = ROOTFilePath+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root";

		// -- dibosons -- //
		SampleInfo* SInfo_ZZ = new SampleInfo( "ZZ", "ZZ", kTRUE );
		SInfo_ZZ->Set_FileName( CommonInput_MC );
		SInfo_ZZ->Set_XSec( 16.523 );
		SInfo_ZZ->Set_nEvent_Generated( 985598 );
		SInfo_ZZ->Calc_NormFactor( Lumi );
		SInfo_ZZ->Set_Color( kGreen );
		// this->vec_BkgInfo.push_back( SInfo_ZZ );

		SampleInfo* SInfo_WZ = new SampleInfo( "WZ", "WZ", kTRUE );
		SInfo_WZ->Set_FileName( CommonInput_MC );
		SInfo_WZ->Set_XSec( 47.13 );
		SInfo_WZ->Set_nEvent_Generated( 999996 );
		SInfo_WZ->Calc_NormFactor( Lumi );
		SInfo_WZ->Set_Color( kGreen );
		// this->vec_BkgInfo.push_back( SInfo_WZ );

		SampleInfo* SInfo_WW = new SampleInfo( "WW", "WW", kTRUE );
		SInfo_WW->Set_FileName( CommonInput_MC );
		SInfo_WW->Set_XSec( 118.7 );
		SInfo_WW->Set_nEvent_Generated( 988416 );
		SInfo_WW->Calc_NormFactor( Lumi );
		SInfo_WW->Set_Color( kGreen );
		// this->vec_BkgInfo.push_back( SInfo_WW );

		SampleInfo* SInfo_Diboson = new SampleInfo( "Diboson", "Diboson", kTRUE );
		SInfo_Diboson->Set_MultiSamples( SInfo_ZZ );
		SInfo_Diboson->Set_MultiSamples( SInfo_WZ );
		SInfo_Diboson->Set_MultiSamples( SInfo_WW );
		this->vec_BkgInfo.push_back( SInfo_Diboson );

		// -- W-jets -- //
		SampleInfo* SInfo_WJets = new SampleInfo( "WJets", "W+jets", kTRUE );
		SInfo_WJets->Set_FileName( CommonInput_MC );
		SInfo_WJets->Set_XSec( 6.15e4 );
		SInfo_WJets->Set_nEvent_Generated( 16520811 );
		SInfo_WJets->Calc_NormFactor( Lumi );
		SInfo_WJets->Set_Color( kBlue );
		this->vec_BkgInfo.push_back( SInfo_WJets );

		// -- DYTauTau, M10-50 -- //
		SampleInfo* SInfo_DYTauTau_M10to50 = new SampleInfo( "DYTauTau_M10to50", "Z/#gamma* #rightarrow #tau#tau", kTRUE );
		SInfo_DYTauTau_M10to50->Set_FileName( CommonInput_MC );
		SInfo_DYTauTau_M10to50->Set_XSec( 18610.0/3.0 );
		SInfo_DYTauTau_M10to50->Set_nEvent_Generated( 7467514.0 );
		SInfo_DYTauTau_M10to50->Calc_NormFactor( Lumi );
		SInfo_DYTauTau_M10to50->Set_Color( kBlue-9 );
		// this->vec_BkgInfo.push_back( SInfo_DYTauTau_M10to50 );

		// -- DYTauTau, M50-inf -- //
		SampleInfo* SInfo_DYTauTau_M50toInf = new SampleInfo( "DYTauTau", "Z/#gamma* #rightarrow #tau#tau", kTRUE );
		SInfo_DYTauTau_M50toInf->Set_FileName( CommonInput_MC );
		SInfo_DYTauTau_M50toInf->Set_XSec( 6104/3.0 );
		SInfo_DYTauTau_M50toInf->Set_nEvent_Generated( 6309713.0 );
		SInfo_DYTauTau_M50toInf->Calc_NormFactor( Lumi );
		SInfo_DYTauTau_M50toInf->Set_Color( kBlue-9 );
		// this->vec_BkgInfo.push_back( SInfo_DYTauTau_M50toInf );

		SampleInfo* SInfo_DYTauTau = new SampleInfo( "DYTauTau", "Z/#gamma* #rightarrow #tau#tau", kTRUE );
		SInfo_DYTauTau->Set_MultiSamples( SInfo_DYTauTau_M10to50 );
		SInfo_DYTauTau->Set_MultiSamples( SInfo_DYTauTau_M50toInf );
		this->vec_BkgInfo.push_back( SInfo_DYTauTau );

		// -- ttbar -- //
		SampleInfo* SInfo_ttbar = new SampleInfo( "ttbar", "t#bar{t}", kTRUE );
		SInfo_ttbar->Set_FileName( CommonInput_MC );
		SInfo_ttbar->Set_XSec( 831.76 );
		SInfo_ttbar->Set_nEvent_Generated( 97994304 );
		SInfo_ttbar->Calc_NormFactor( Lumi );
		SInfo_ttbar->Set_Color( kRed );
		this->vec_BkgInfo.push_back( SInfo_ttbar );

		///////////////////
		// -- Signals -- //
		///////////////////
		Int_t Color_Signal = kOrange;

		SampleInfo* SInfo_DYMuMu_M10to50 = new SampleInfo( "DYMuMu_M10to50", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M10to50->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M10to50->Set_XSec( 18610.0/3.0 );
		SInfo_DYMuMu_M10to50->Set_nEvent_Generated( 7506956 );
		SInfo_DYMuMu_M10to50->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M10to50->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M10to50 );

		SampleInfo* SInfo_DYMuMu_M50to100 = new SampleInfo( "DYMuMu_M50to100", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M50to100->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M50to100->Set_XSec( 5869.58346/3.0 );
		SInfo_DYMuMu_M50to100->Set_nEvent_Generated( 6061181 );
		SInfo_DYMuMu_M50to100->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M50to100->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M50to100 );

		SampleInfo* SInfo_DYMuMu_M100to200 = new SampleInfo( "DYMuMu_M100to200", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M100to200->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M100to200->Set_XSec( 226/3.0 );
		SInfo_DYMuMu_M100to200->Set_nEvent_Generated( 227522 );
		SInfo_DYMuMu_M100to200->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M100to200->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M100to200 );

		SampleInfo* SInfo_DYMuMu_M200to400 = new SampleInfo( "DYMuMu_M200to400", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M200to400->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M200to400->Set_XSec( 7.67/3.0 );
		SInfo_DYMuMu_M200to400->Set_nEvent_Generated( 170955 );
		SInfo_DYMuMu_M200to400->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M200to400->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M200to400 );

		SampleInfo* SInfo_DYMuMu_M400to500 = new SampleInfo( "DYMuMu_M400to500", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M400to500->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M400to500->Set_XSec( 0.423/3.0 );
		SInfo_DYMuMu_M400to500->Set_nEvent_Generated( 50136 );
		SInfo_DYMuMu_M400to500->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M400to500->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M400to500 );

		SampleInfo* SInfo_DYMuMu_M500to700 = new SampleInfo( "DYMuMu_M500to700", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M500to700->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M500to700->Set_XSec( 0.24/3.0 );
		SInfo_DYMuMu_M500to700->Set_nEvent_Generated( 47833 );
		SInfo_DYMuMu_M500to700->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M500to700->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M500to700 );

		SampleInfo* SInfo_DYMuMu_M700to800 = new SampleInfo( "DYMuMu_M700to800", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M700to800->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M700to800->Set_XSec( 0.035/3.0 );
		SInfo_DYMuMu_M700to800->Set_nEvent_Generated( 44740 );
		SInfo_DYMuMu_M700to800->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M700to800->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M700to800 );

		SampleInfo* SInfo_DYMuMu_M800to1000 = new SampleInfo( "DYMuMu_M800to1000", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M800to1000->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M800to1000->Set_XSec( 0.03/3.0 );
		SInfo_DYMuMu_M800to1000->Set_nEvent_Generated( 43496 );
		SInfo_DYMuMu_M800to1000->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M800to1000->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M800to1000 );

		SampleInfo* SInfo_DYMuMu_M1000to1500 = new SampleInfo( "DYMuMu_M1000to1500", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M1000to1500->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M1000to1500->Set_XSec( 0.016/3.0 );
		SInfo_DYMuMu_M1000to1500->Set_nEvent_Generated( 40783 );
		SInfo_DYMuMu_M1000to1500->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M1000to1500->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M1000to1500 );

		SampleInfo* SInfo_DYMuMu_M1500to2000 = new SampleInfo( "DYMuMu_M1500to2000", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M1500to2000->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M1500to2000->Set_XSec( 0.002/3.0 );
		SInfo_DYMuMu_M1500to2000->Set_nEvent_Generated( 37176 );
		SInfo_DYMuMu_M1500to2000->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M1500to2000->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M1500to2000 );

		SampleInfo* SInfo_DYMuMu_M2000to3000 = new SampleInfo( "DYMuMu_M2000to3000", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		SInfo_DYMuMu_M2000to3000->Set_FileName( CommonInput_MC );
		SInfo_DYMuMu_M2000to3000->Set_XSec( 0.00054/3.0 );
		SInfo_DYMuMu_M2000to3000->Set_nEvent_Generated( 23078 );
		SInfo_DYMuMu_M2000to3000->Calc_NormFactor( Lumi );
		SInfo_DYMuMu_M2000to3000->Set_Color( Color_Signal );
		// this->vec_SigInfo.push_back( SInfo_DYMuMu_M2000to3000 );

		this->SignalInfo = new SampleInfo( "DYMuMu", "Z/#gamma* #rightarrow #mu#mu", kTRUE );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M10to50 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M50to100 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M100to200 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M200to400 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M400to500 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M500to700 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M700to800 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M800to1000 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M1000to1500 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M1500to2000 );
		this->SignalInfo->Set_MultiSamples( SInfo_DYMuMu_M2000to3000 );

		// -- data -- //
		this->DataInfo = new SampleInfo( "Data", "Data", kFALSE );
		this->DataInfo->Set_FileName( ROOTFilePath+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root" );
		this->DataInfo->Set_Color( kBlack );
	}

	void Set_HistStyle()
	{
		HistStyle *Hist_OSMass = new HistStyle( "h_mass_OS", "Dilepton mass (opposite sign, DY Bin)" );
		Hist_OSMass->Set_LogX();
		Hist_OSMass->Set_LogY();
		Hist_OSMass->Set_YRange(5e-2, 2e6);
		Hist_OSMass->Set_Titles( "m [GeV]", "Entries per bins");
		this->vec_HistStyle.push_back( Hist_OSMass );
	}
};