#include <Include/PlotTools.h>

void RemoveLumiError( TH1D* h )
{
	Int_t nBin = h->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t value = h->GetBinContent(i_bin);
		Double_t errorRel_before = h->GetBinError(i_bin) / value;
		Double_t errorRel_after = sqrt( errorRel_before*errorRel_before - 0.027*0.027 );

		h->SetBinError(i_bin, errorRel_after*value );
	}
}

void Calc_Chi2( TString Type1, TH1D* h1, TString Type2, TH1D* h2 )
{
	Double_t NormChi2_total = 0;
	TH1D* h_chi2 = (TH1D*)h1->Clone("h_chi2");
	Int_t nBin = h1->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t value1 = h1->GetBinContent(i_bin);
		Double_t error1 = h1->GetBinError(i_bin);

		Double_t value2 = h2->GetBinContent(i_bin);
		Double_t error2 = h2->GetBinError(i_bin);

		Double_t diff = fabs( value1 - value2 );
		Double_t errorSum = sqrt( error1*error1 + error2*error2);

		Double_t chi2 = sqrt( (diff*diff) / (errorSum*errorSum) );

		printf("[%d bin (%4.0lf < M < %4.0lf): chi2 = %.3lf] %s: (value, error) = (%lf, %lf), %s: (value, error) = (%lf, %lf), diff = %lf, errorSum = %lf\n", 
			i_bin, h1->GetBinLowEdge(i_bin), h1->GetBinLowEdge(i_bin+1), chi2, Type1.Data(), value1, error1, Type2.Data(), value2, error2, diff, errorSum );

		h_chi2->SetBinContent(i_bin, chi2 );
		h_chi2->SetBinError(i_bin, 0);

		NormChi2_total = NormChi2_total + chi2;
	}

	NormChi2_total = NormChi2_total / nBin;


	// -- draw canvas -- //
	HistInfo *Hist = new HistInfo( kBlack, "#chi^{2}");
	Hist->Set_Histogram( h_chi2 );
	Hist->Set();

	TCanvas *c;
	SetCanvas_Square( c, TString::Format("c_chi2_%s_%s", Type1.Data(), Type2.Data()) );
	c->cd();
	gPad->SetLogx();
	// gPad->SetLogy();

	Hist->h->Draw("LPSAME");
	TString YTitle = "";
	if( Type1 == "elec" && Type2 == "mu" ) YTitle = "#chi^{2} (ee, #mu#mu)";
	else if( Type2 == "elec" ) YTitle = "#chi^{2} (Theory, ee)";
	else if( Type2 == "mu" ) YTitle = "#chi^{2} (Theory, #mu#mu)";

	SetHistFormat_SinglePad( Hist->h, "m [GeV]", YTitle);
	Hist->h->GetXaxis()->SetNoExponent();
	Hist->h->GetXaxis()->SetMoreLogLabels();
	Hist->h->GetYaxis()->SetRangeUser(0, 4);

	TLatex latex;
	if( Type1 == "elec" && Type2 == "mu" )
	{
		Latex_Preliminary_NoDataInfo( latex );
		latex.DrawLatexNDC( 0.55, 0.96, "#font[42]{#scale[0.8]{2.3 fb^{-1} (ee)}}");
		latex.DrawLatexNDC( 0.75, 0.96, "#font[42]{#scale[0.8]{2.8 fb^{-1} (#mu#mu)}}");
	}
	else if( Type2 == "elec" )
		Latex_Preliminary( latex, 2.3, 13 );
	else if( Type2 == "mu" )
		Latex_Preliminary( latex, 2.8, 13 );

	TString TStr_Info = TString::Format("#chi^{2}/ndof = %.3lf", NormChi2_total);
	latex.DrawLatexNDC( 0.65, 0.90, "#font[42]{#scale[0.8]{"+TStr_Info+"}}");

	c->SaveAs(".pdf");
}

void DrawCavnas_Comparison_DiffXSec( TString Type1, HistInfo *Hist1, TString Type2, HistInfo *Hist2 )
{
	// -- canvas -- //
	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, TString::Format( "c_Comparison_%s_%s", Type1.Data(), Type2.Data() ) , TopPad, BottomPad, 1, 1 );
	c->cd();
	TopPad->cd();

	Hist1->h->Draw("EPSAME");
	Hist2->h->Draw("EPSAME");

	SetHistFormat_TopPad( Hist1->h, "d#sigma/dm [pb/GeV]");
	if( Type1 == "elec" && Type2 == "mu" )
	{
		Hist1->h->SetLineColor(kRed);
		Hist1->h->SetMarkerColor(kRed);
	}
	Hist1->h->GetYaxis()->SetRangeUser(2e-7, 5e+2);

	TLegend *legend;
	SetLegend( legend, 0.60, 0.80, 0.95, 0.95 );
	legend->AddEntry( Hist1->h, Hist1->LegendName );
	legend->AddEntry( Hist2->h, Hist2->LegendName );
	legend->Draw();	

	TLatex latex;
	if( Type1 == "elec" && Type2 == "mu" )
	{
		Latex_Preliminary_NoDataInfo( latex );
		latex.DrawLatexNDC( 0.55, 0.96, "#font[42]{#scale[0.8]{2.3 fb^{-1} (ee)}}");
		latex.DrawLatexNDC( 0.75, 0.96, "#font[42]{#scale[0.8]{2.8 fb^{-1} (#mu#mu)}}");
	}
	else if( Type2 == "elec" )
		Latex_Preliminary( latex, 2.3, 13 );
	else if( Type2 == "mu" )
		Latex_Preliminary( latex, 2.8, 13 );

	c->cd();
	BottomPad->cd();

	Hist2->Calc_RatioHist_Denominator( Hist1->h );
	Hist2->h_ratio->Draw("EPSAME");

	TString RatioTitle = "Data / Theory";
	if( Type1 == "elec" && Type2 == "mu" )
		RatioTitle = "#mu#mu / ee";
	SetHistFormat_BottomPad( Hist2->h_ratio, "m [GeV]", RatioTitle, 0.4, 1.6);
	Hist2->h_ratio->SetNdivisions(506);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}

void Compatibility_TwoChannel()
{
	TString FileName_Theory = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20170106_1st_UpdateSyst_fromApproval/ROOTFile_xSec_Theory.root";
	TString HistName_Theory = "h_DiffXsec_FEWZ_NNPDF_NNLO";

	TString FileName_ee = "./diffXsec_Electron.root";
	TString HistName_ee = "h_xSec_dM_FSRCorr";

	TString FileName_mm = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20170106_1st_UpdateSyst_fromApproval/ROOTFile_DiffXSec_FullUnc.root";
	TString HistName_mm = "h_DiffXsec_FSRCorr";

	HistInfo *Hist_Theory = new HistInfo( kRed, "FEWZ (NNLO, NNPDF3.0)" );
	Hist_Theory->Set_FileName_ObjectName( FileName_Theory, HistName_Theory );
	Hist_Theory->Set();

	HistInfo *Hist_ee = new HistInfo( kBlack, "ee" );
	Hist_ee->Set_FileName_ObjectName( FileName_ee, HistName_ee );
	Hist_ee->Set();

	HistInfo *Hist_mm = new HistInfo( kBlack, "#mu#mu" );
	Hist_mm->Set_FileName_ObjectName( FileName_mm, HistName_mm );
	Hist_mm->Set();
	RemoveLumiError( Hist_mm->h );

	DrawCavnas_Comparison_DiffXSec( "Theory", Hist_Theory, "elec", Hist_ee );
	DrawCavnas_Comparison_DiffXSec( "Theory", Hist_Theory, "mu", Hist_mm );
	DrawCavnas_Comparison_DiffXSec( "elec", Hist_ee, "mu", Hist_mm );

	Calc_Chi2( "Theory", Hist_Theory->h, "elec", Hist_ee->h );
	Calc_Chi2( "Theory",  Hist_Theory->h, "mu", Hist_mm->h );
	Calc_Chi2( "elec", Hist_ee->h, "mu", Hist_mm->h );
}