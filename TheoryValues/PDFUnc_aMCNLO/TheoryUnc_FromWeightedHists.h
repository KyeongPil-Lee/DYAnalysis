#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"

using namespace RooFit;

#include <Include/PlotTools.h>
#include <Include/DYAnalyzer.h> // -- for Lumi -- //

#define nWeight 111

class Tool_TheoryUnc
{
public:
	TString Tag;
	TString Type_Channel;
	TString Type_Hist;
	Double_t Luminosity;

	TString FileName_Input;
	TString HistName_CV;
	TString HistName_Weighted[nWeight];

	TH1D* h_CV; // -- diff.x-section -- //
	vector< TH1D* > vec_HistWeighted; // -- diff.x-section -- //

	TH1D* h_RelUnc_ScaleVar;
	TH1D* h_RelUnc_PDFReplica;
	TH1D* h_RelUnc_AlphaSVar;

	TH1D* h_RelUnc_Syst;
	TH1D* h_RelUnc_Stat;
	TH1D* h_RelUnc_Tot;

	Double_t yMax_UncPlot;

	Tool_TheoryUnc()
	{
		TH1::AddDirectory(kFALSE);
	}

	Tool_TheoryUnc( TString _Channel, TString _Hist ) : Tool_TheoryUnc()
	{
		this->Type_Channel = _Channel;
		this->Type_Hist = _Hist;
		this->Tag = this->Type_Channel+"_"+this->Type_Hist;

		this->Setup();
	}

	void Calc_TheoryUnc()
	{
		this->Get_Hist_DXSec();
		this->Calc_SystUnc_ScaleVar();
		this->Calc_SystUnc_PDFReplica();
		this->Calc_SystUnc_AlphaSVar();
		this->Calc_SystUnc();
		this->Calc_StatUnc();
		this->Draw_SummaryPlot();

		// if( this->Type_Channel == "Muon" ) this->ClosureTest();
		// this->Calc_SystUnc_GaussianFit();
	}

	void Save( TFile *f_output )
	{
		f_output->cd();
		f_output->mkdir( this->Tag ); f_output->cd( this->Tag );

		this->h_RelUnc_ScaleVar->SetName("h_RelUnc_ScaleVar");
		this->h_RelUnc_ScaleVar->Write();

		this->h_RelUnc_PDFReplica->SetName("h_RelUnc_PDFReplica");
		this->h_RelUnc_PDFReplica->Write();

		this->h_RelUnc_AlphaSVar->SetName("h_RelUnc_AlphaSVar");
		this->h_RelUnc_AlphaSVar->Write();

		this->h_RelUnc_Syst->SetName("h_RelUnc_Syst");
		this->h_RelUnc_Syst->Write();

		this->h_RelUnc_Stat->SetName("h_RelUnc_Stat");
		this->h_RelUnc_Stat->Write();

		this->h_RelUnc_Tot->SetName("h_RelUnc_Tot");
		this->h_RelUnc_Tot->Write();
	}

protected:
	void Calc_SystUnc()
	{

		this->h_RelUnc_Syst = (TH1D*)h_CV->Clone();
		this->h_RelUnc_Syst->Reset("ICES");
		Int_t nBin = h_CV->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t RelUnc_ScaleVar = this->h_RelUnc_ScaleVar->GetBinContent(i_bin);
			Double_t RelUnc_PDFReplica = this->h_RelUnc_PDFReplica->GetBinContent(i_bin);
			Double_t RelUnc_AlphaSVar = this->h_RelUnc_AlphaSVar->GetBinContent(i_bin);

			Double_t RelUnc_Tot = sqrt(
				RelUnc_ScaleVar*RelUnc_ScaleVar+
				RelUnc_PDFReplica*RelUnc_PDFReplica+
				RelUnc_AlphaSVar*RelUnc_AlphaSVar );

			this->h_RelUnc_Syst->SetBinContent(i_bin, RelUnc_Tot );
			this->h_RelUnc_Syst->SetBinError(i_bin, 0 );
		}

		this->Draw_RelUnc_Syst();
	}

	void Draw_RelUnc_Syst()
	{
		HistInfo* Hist_RelUnc_Tot = new HistInfo( kBlue, "Total Syst.", this->h_RelUnc_Syst );
		HistInfo* Hist_RelUnc_ScaleVar = new HistInfo( kGreen+2, "Scale variation", this->h_RelUnc_ScaleVar);
		HistInfo* Hist_RelUnc_PDFReplica = new HistInfo( kViolet, "PDF replica", this->h_RelUnc_PDFReplica );
		HistInfo* Hist_RelUnc_AlphaSVar = new HistInfo( kOrange+2, "#alpha_{S} variation", this->h_RelUnc_AlphaSVar);

		TString CanvasName = "Local/c_RelUnc_Syst_"+this->Tag;
		TCanvas *c;
		SetCanvas_Square( c, CanvasName, 1, 0 );
		c->cd();
		TLegend *legend;
		SetLegend( legend, 0.70, 0.82, 0.95, 0.95 );
		Hist_RelUnc_Tot->Draw("HISTLPSAME");
		Hist_RelUnc_ScaleVar->Draw("HISTLPSAME");
		Hist_RelUnc_PDFReplica->Draw("HISTLPSAME");
		Hist_RelUnc_AlphaSVar->Draw("HISTLPSAME");
		Hist_RelUnc_Tot->AddToLegend( legend );
		Hist_RelUnc_ScaleVar->AddToLegend( legend );
		Hist_RelUnc_PDFReplica->AddToLegend( legend );
		Hist_RelUnc_AlphaSVar->AddToLegend( legend );
		legend->Draw();
		SetHistFormat_SinglePad( Hist_RelUnc_Tot->h, "m [GeV]", "Rel. unc." );
		Hist_RelUnc_Tot->h->GetYaxis()->SetRangeUser(0, 0.25);

		c->SaveAs(".pdf");
	}

	void Calc_SystUnc_AlphaSVar()
	{
		Int_t i_117 = 109;
		Int_t i_119 = 110;

		HistInfo* Hist_117 = new HistInfo( kGreen+2, "#alpha_{S}=0.117", this->vec_HistWeighted[i_117]);
		HistInfo* Hist_119 = new HistInfo( kBlue, "#alpha_{S}=0.119", this->vec_HistWeighted[i_119]);
		vector< HistInfo* > vec_AlphaSVarHist;
		vec_AlphaSVarHist.push_back( Hist_117 );
		vec_AlphaSVarHist.push_back( Hist_119 );
		this->Draw_DXSec_AlphaSVar( vec_AlphaSVarHist );

		Int_t nBin = this->h_CV->GetNbinsX();
		this->h_RelUnc_AlphaSVar = (TH1D*)h_CV->Clone();
		this->h_RelUnc_AlphaSVar->Reset("ICES");
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t Value_CV = this->h_CV->GetBinContent(i_bin);
			Double_t Value_119 = Hist_119->h->GetBinContent(i_bin);
			Double_t Value_117 = Hist_117->h->GetBinContent(i_bin);

			Double_t AbsUnc = (Value_119 - Value_117 ) / 2.0;
			Double_t RelUnc = AbsUnc / Value_CV;

			printf("[%02d bin] (Value_119, Value_117, AbsUnc, RelUnc) = (%lf, %lf, %lf, %lf)\n", 
				i_bin, Value_119, Value_117, AbsUnc, RelUnc);

			this->h_RelUnc_AlphaSVar->SetBinContent(i_bin, RelUnc);
			this->h_RelUnc_AlphaSVar->SetBinError(i_bin, 0);
		}
	}

	void Draw_DXSec_AlphaSVar(vector< HistInfo* > vec_AlphaSVarHist)
	{
		HistInfo* Hist_CV = new HistInfo( kRed, "Central value", this->h_CV );
		for( const auto& AlphaSVarHist : vec_AlphaSVarHist )
			AlphaSVarHist->CalcRatio_DEN( Hist_CV->h );

		TString CanvasName = "Local/c_DXSec_AlphaSVar_"+this->Tag;
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 1, 1 );
		c->cd();
		TopPad->cd();
		TLegend *legend;
		SetLegend( legend, 0.70, 0.82, 0.95, 0.95 );
		Hist_CV->Draw("EPSAME");
		Hist_CV->AddToLegend( legend );
		for( const auto& AlphaSVarHist : vec_AlphaSVarHist )
		{
			AlphaSVarHist->Draw("EPSAME");
			AlphaSVarHist->AddToLegend( legend );
		}
		legend->Draw();
		SetHistFormat_TopPad( Hist_CV->h, "d#sigma/dm [pb/GeV]" );

		c->cd();
		BottomPad->cd();

		for( const auto& AlphaSVarHist : vec_AlphaSVarHist )
			AlphaSVarHist->DrawRatio( "EPSAME" );
		SetHistFormat_BottomPad( vec_AlphaSVarHist[0]->h_ratio, "m [GeV]", "#alpha_{S}-var./C.V.", 0.94, 1.06 );

		TF1* f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}

	void Calc_SystUnc_PDFReplica()
	{
		Int_t i_begin = 9;
		Int_t i_end = 108;
		vector<TH1D*> vec_PDFReplicaHist;
		for(Int_t i_hist = i_begin; i_hist <= i_end; i_hist++)
			vec_PDFReplicaHist.push_back( this->vec_HistWeighted[i_hist] );
	
		Int_t nHist = (Int_t)vec_PDFReplicaHist.size();
		cout << "# hists from PDF replica: " << nHist << endl;

		// -- take the RMS -- //
		Int_t nBin = this->h_CV->GetNbinsX();
		this->h_RelUnc_PDFReplica = (TH1D*)h_CV->Clone();
		this->h_RelUnc_PDFReplica->Reset("ICES");
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t Value_CV = this->h_CV->GetBinContent(i_bin);

			Double_t Sum_PDFReplica = 0;
			vector< Double_t > vec_PDFReplica;
			for( const auto& PDFReplicaHist : vec_PDFReplicaHist )
			{
				Double_t Value_Replica = PDFReplicaHist->GetBinContent(i_bin);
				vec_PDFReplica.push_back( Value_Replica );
				Sum_PDFReplica = Sum_PDFReplica + Value_Replica;

				// printf("(Value_CV, Value_Replica) = (%lf, %lf)\n", Value_CV, Value_Replica ); 
			}

			Double_t Mean_PDFReplica = Sum_PDFReplica / nHist;
			Double_t SquareSum = 0;
			for( const auto& Value_PDFReplica : vec_PDFReplica )
				SquareSum = SquareSum + (Value_PDFReplica - Mean_PDFReplica)*(Value_PDFReplica - Mean_PDFReplica);

			Double_t RMS_PDFReplica = sqrt(SquareSum / nHist);
			Double_t RelUnc = RMS_PDFReplica/Value_CV;

			printf("[%02d bin] (Mean, RMS) = (%lf, %lf (%lf %%))\n", i_bin, Mean_PDFReplica, RMS_PDFReplica, RelUnc*100 );

			this->h_RelUnc_PDFReplica->SetBinContent(i_bin, RelUnc);
			this->h_RelUnc_PDFReplica->SetBinError(i_bin, 0);
		}
	}

	void Calc_SystUnc_ScaleVar()
	{
		HistInfo* Hist_Var1 = new HistInfo( kBlack, "1#mu_{R}, 2#mu_{F}", vec_HistWeighted[1]);
		HistInfo* Hist_Var2 = new HistInfo( kGreen+2, "1#mu_{R}, 0.5#mu_{F}", vec_HistWeighted[2]);
		HistInfo* Hist_Var3 = new HistInfo( kBlue, "2#mu_{R}, 1#mu_{F}", vec_HistWeighted[3]);
		HistInfo* Hist_Var4 = new HistInfo( kCyan, "2#mu_{R}, 2#mu_{F}", vec_HistWeighted[4]);
		HistInfo* Hist_Var5 = new HistInfo( kOrange+2, "0.5#mu_{R}, 1#mu_{F}", vec_HistWeighted[6]);
		HistInfo* Hist_Var6 = new HistInfo( kViolet, "0.5#mu_{R}, 0.5#mu_{F}", vec_HistWeighted[8] );
		vector< HistInfo* > vec_ScaleVarHist =
		{
			Hist_Var1, Hist_Var2, Hist_Var3,
			Hist_Var4, Hist_Var5, Hist_Var6
		};

		this->Draw_DXSec_ScaleVar( vec_ScaleVarHist );

		// -- calc. uncertainty: take the largest one (envelop) -- //
		this->h_RelUnc_ScaleVar = (TH1D*)this->h_CV->Clone();
		this->h_RelUnc_ScaleVar->Reset("ICES");
		Int_t nBin = this->h_CV->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t DXSec_CV = h_CV->GetBinContent(i_bin);
			Double_t RelDiff_Max = 0;
			for( const auto& ScaleVarHist : vec_ScaleVarHist )
			{
				Double_t DXSec_ScaleVar = ScaleVarHist->h->GetBinContent(i_bin);
				Double_t RelDiff_temp = fabs( DXSec_ScaleVar - DXSec_CV ) / DXSec_CV;
				if( RelDiff_temp > RelDiff_Max )
					RelDiff_Max = RelDiff_temp;
			}

			this->h_RelUnc_ScaleVar->SetBinContent(i_bin, RelDiff_Max);
			this->h_RelUnc_ScaleVar->SetBinError(i_bin, 0);
		}
	}

	void Draw_DXSec_ScaleVar(vector< HistInfo* > vec_ScaleVarHist)
	{
		HistInfo* Hist_CV = new HistInfo( kRed, "Central value", this->h_CV );
		for( const auto& ScaleVarHist : vec_ScaleVarHist )
			ScaleVarHist->CalcRatio_DEN( Hist_CV->h );

		TString CanvasName = "Local/c_DXSec_ScaleVar_"+this->Tag;
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 1, 1 );
		c->cd();
		TopPad->cd();
		TLegend *legend;
		SetLegend( legend, 0.7, 0.65, 0.95, 0.95 );
		Hist_CV->Draw("EPSAME");
		Hist_CV->AddToLegend( legend );
		for( const auto& ScaleVarHist : vec_ScaleVarHist )
		{
			ScaleVarHist->Draw("EPSAME");
			ScaleVarHist->AddToLegend( legend );
		}
		legend->Draw();
		SetHistFormat_TopPad( Hist_CV->h, "d#sigma/dm [pb/GeV]" );

		c->cd();
		BottomPad->cd();

		for( const auto& ScaleVarHist : vec_ScaleVarHist )
			ScaleVarHist->DrawRatio( "EPSAME" );
		SetHistFormat_BottomPad( vec_ScaleVarHist[0]->h_ratio, "m [GeV]", "Scale-var./C.V.", 0.8, 1.15 );

		TF1* f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}

	void Calc_StatUnc()
	{
		// TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		// TString FileName = ROOTFilePath+"/ROOTFile_Results_DYAnalysis_76X.root";
		// TString HistName = "h_DYMC_Gen_preFSR";
		// if( this->Type_Hist == "withinAcc" ) HistName = "h_DYMC_Gen_postFSR_WithinAcc";

		// TH1D* h_Mass_GEN = Get_Hist( FileName, HistName );
		// this->h_RelUnc_Stat = Extract_RelUnc( h_Mass_GEN );
		
		this->h_RelUnc_Stat = Extract_RelUnc( this->h_CV );
	}

	void Draw_SummaryPlot()
	{
		this->h_RelUnc_Tot = QuadSum_NoError( this->h_RelUnc_Stat, this->h_RelUnc_Syst );

		HistInfo* Hist_Stat = new HistInfo( kBlack, "Stat.", this->h_RelUnc_Stat );
		HistInfo* Hist_Syst = new HistInfo( kBlue, "PDF Replica + Scale + #alpha_{S}", this->h_RelUnc_Syst );
		HistInfo* Hist_Tot = new HistInfo( kRed, "Total (quad. sum)", this->h_RelUnc_Tot );

		// -- convert to % -- //
		// Hist_Stat->h->Scale( 100 );
		// Hist_Syst->h->Scale( 100 );
		// Hist_Tot->h->Scale( 100 );

		TString CanvasName = "Local/c_Summary_RelUnc_"+this->Tag;
		TCanvas *c;
		TLegend *legend;

		SetCanvas_Square( c, CanvasName, 1, 0 );
		SetLegend(legend, 0.57, 0.75, 0.94, 0.95 );
		c->cd();

		Hist_Stat->Draw("HISTLPSAME");
		Hist_Syst->Draw("HISTLPSAME");
		Hist_Tot->Draw("HISTLPSAME");

		SetHistFormat_SinglePad( Hist_Stat->h, "m [GeV]", "Rel. unc.");
		Hist_Stat->h->GetYaxis()->SetRangeUser(0, 0.25);

		Hist_Stat->AddToLegend( legend );
		Hist_Syst->AddToLegend( legend );
		Hist_Tot->AddToLegend( legend );

		legend->Draw();

		TLatex latex;
		Latex_Simulation( latex );

		c->SaveAs(".pdf");
	}

	void ClosureTest()
	{
		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString FileName_Ref = ROOTFilePath+"/ROOTFile_Results_DYAnalysis_76X.root";
		// TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		// TString FileName_Ref = AnalyzerPath+"/FSRCorrection/Local/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root";

		TString HistName_Ref = "";
		if( this->Type_Hist == "withinAcc" ) HistName_Ref = "h_FpoF_DiffXsec_aMCNLO";
		if( this->Type_Hist == "postFSR" ) HistName_Ref = "h_DYMC_Gen_postFSR";
		if( this->Type_Hist == "dressed" ) HistName_Ref = "h_DiffXsec_aMCNLO";

		// if( this->Type_Hist == "postFSR" ) HistName_Ref = "h_mass_postFSR";
		// if( this->Type_Hist == "dressed" ) HistName_Ref = "h_mass_preFSR";

		TH1D* h_DXSec_Ref = Get_Hist( FileName_Ref, HistName_Ref );
		if( this->Type_Hist == "postFSR" ) h_DXSec_Ref = this->Convert_MassHist_to_DXSec( h_DXSec_Ref );
		// if( this->Type_Hist == "dressed" ) h_DXSec_Ref = this->Convert_MassHist_to_DXSec( h_DXSec_Ref );

		HistInfo* Hist_this = new HistInfo( kBlack, "This", this->h_CV );
		HistInfo* Hist_ref = new HistInfo( kRed, "Ref.", h_DXSec_Ref );

		TString CanvasName = "Local/XCheck_"+this->Tag;
		DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_this, Hist_ref );
		canvas->SetTitle( "m [GeV]", "d#sigma/dm [pb/GeV]", "this/ref.");
		canvas->SetLegendPosition( 0.60, 0.85, 0.97, 0.95 );
		canvas->SetRatioRange( 0.94, 1.06 );
		canvas->Draw( 1, 1 );
	}

	void Setup()
	{
		this->FileName_Input = TString::Format( "ROOTFile_Hists_ForPDFUnc_XSec_aMCNLO_%s.root", this->Type_Channel.Data() );
		if( this->Type_Channel == "Muon" ) this->Luminosity = Lumi;
		if( this->Type_Channel == "Electron" ) this->Luminosity = 2257.998;

		this->HistName_CV = TString::Format("h_mass_%s_CV_All", this->Type_Hist.Data() );
		for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
		{
			TString TStr_Number = TString::Format("%03d", i_weight);
			this->HistName_Weighted[i_weight] = TString::Format("h_mass_%s_%s_All", this->Type_Hist.Data(), TStr_Number.Data() );
		}

		this->yMax_UncPlot = 0.2;		
	}

	void Get_Hist_DXSec()
	{
		TH1D* h_mass_CV = Get_Hist( this->FileName_Input, this->HistName_CV );
		this->h_CV = this->Convert_MassHist_to_DXSec( h_mass_CV );

		for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
		{
			TH1D* h_mass_Weighted = Get_Hist( this->FileName_Input, this->HistName_Weighted[i_weight] );
			TH1D* h_Weighted = this->Convert_MassHist_to_DXSec( h_mass_Weighted );
			vec_HistWeighted.push_back( h_Weighted );
		}
	}

	TH1D* Convert_MassHist_to_DXSec( TH1D* h_mass )
	{
		TH1D* h_DXSec = (TH1D*)h_mass->Clone();
		h_DXSec->Scale( 1.0 / this->Luminosity );
		h_DXSec = DivideEachBin_ByBinWidth( h_DXSec );

		return h_DXSec;
	}
};