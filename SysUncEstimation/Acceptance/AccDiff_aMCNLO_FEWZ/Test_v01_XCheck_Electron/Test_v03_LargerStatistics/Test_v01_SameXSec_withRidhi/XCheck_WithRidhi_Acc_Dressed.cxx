#include <TEfficiency.h>
#include <Include/PlotTools.h>

void XCheck_WithRidhi_Acc_Dressed()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TString FileName_Ridhi = "../../Test_v01_XCheck_Ridhi/DYEE_AccEff_M10to3000.root";
	TString HistName_AccTotal = "h_mass_AccTotal";
	TString HistName_AccPass = "h_mass_AccPass";

	TH1D* h_AccTotal = Get_Hist( FileName_Ridhi, HistName_AccTotal );
	TH1D* h_AccPass = Get_Hist( FileName_Ridhi, HistName_AccPass );

	TEfficiency *TEff_Ridhi = new TEfficiency( *h_AccPass, *h_AccTotal );
	TGraphAsymmErrors *g_Ridhi = (TGraphAsymmErrors*)TEff_Ridhi->CreateGraph()->Clone();

	TH1D* h_Ridhi = Convert_GraphToHist( g_Ridhi );

	TString FileName_KP = "ROOTFile_Acc_Dressed_EE.root";
	TGraphAsymmErrors* g_KP = Get_Graph( FileName_KP, "g_Acc" );
	TH1D* h_KP = Convert_GraphToHist( g_KP );

	HistInfo *Hist_Ridhi = new HistInfo( kGreen+2, "Ridhi", h_Ridhi );
	HistInfo *Hist_KP = new HistInfo( kBlue, "KP (largeN for M10-50 & M50-inf)", h_KP );

	TString CanvasName = "Local/c_Acc_Dressed_KP_vs_Ridhi";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_KP, Hist_Ridhi);
	canvas->SetTitle("m [GeV]", "Acceptance (dressed)", "KP/Ridhi");
	canvas->SetLegendPosition( 0.40, 0.32, 0.97, 0.45 );
	canvas->SetRatioRange( 0.97, 1.03 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw( 1, 0 );
	canvas->c->cd();
	canvas->TopPad->cd();

	canvas->latex.DrawLatexNDC( 0.15, 0.915, "#scale[0.6]{#font[42]{aMC@NLO}}");	
	canvas->latex.DrawLatexNDC( 0.15, 0.88, "#scale[0.5]{#font[42]{P_{T}^{lead} > 30 GeV, P_{T}^{sub} > 10 GeV, |#eta| < 2.5}}");
	canvas->latex.DrawLatexNDC( 0.15, 0.85, "#scale[0.5]{#font[42]{& ECAL gap (1.4442 < |#eta| < 1.566) is excluded}}");
	canvas->c->SaveAs(".pdf");
}