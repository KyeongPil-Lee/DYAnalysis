#include <TEfficiency.h>
#include <Include/PlotTools.h>

void Comparison_MassHist( TH1D* h_KP, TH1D* h_Ridhi, TString FSRType = "Dressed" );

void XCheck_WithRidhi_Acc_Dressed()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	// TString FileName_Ridhi = AnalyzerPath+"/SysUncEstimation/Acceptance/AccDiff_aMCNLO_FEWZ/Test_v01_XCheck_Electron/Test_v01_XCheck_Ridhi/DYEE_AccEff_M10to3000.root";
	TString FileName_Ridhi = AnalyzerPath+"/SysUncEstimation/Acceptance/AccDiff_aMCNLO_FEWZ/Test_v01_XCheck_Electron/Test_v03_LargerStatistics/Test_v04_Pt_eta/ROOTFile_DYEE_AccTest_Input_Electron_Ridhi_v2.root";
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
	HistInfo *Hist_KP = new HistInfo( kBlue, "KP (largeN for M10-50)", h_KP );

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


	// // -- mass histogram itself -- //
	Double_t Lumi_MuonPhys = 2759.017;
	Double_t Lumi_Golden = 2257.998;
	Double_t scale = Lumi_Golden / Lumi_MuonPhys;
	TH1D* h_AccTotal_KP = Get_Hist( FileName_KP, "h_AccTotal" );
	TH1D* h_AccPass_KP = Get_Hist( FileName_KP, "h_AccPass" );
	h_AccTotal_KP->Scale( scale );
	h_AccPass_KP->Scale( scale );

	Comparison_MassHist( h_AccTotal_KP, h_AccTotal );
	Comparison_MassHist( h_AccPass_KP, h_AccPass );

	// TString FileName_postFSR_Ridhi = "DYEE_AccEff_M10to3000_postFSR_Ridhi.root";
	// TH1D* h_AccTotal_postFSR_Ridhi = Get_Hist( FileName_postFSR_Ridhi, "h_mass_AccTotal");
	// TH1D* h_AccPass_postFSR_Ridhi = Get_Hist( FileName_postFSR_Ridhi, "h_mass_AccPass");

	// TH1D* h_AccTotal_postFSR_KP = Get_Hist( FileName_KP, "h_AccTotal_postFSR" );
	// TH1D* h_AccPass_postFSR_KP = Get_Hist( FileName_KP, "h_AccPass_postFSR" );
	// h_AccTotal_postFSR_KP->Scale( scale );
	// h_AccPass_postFSR_KP->Scale( scale );

	// Comparison_MassHist( h_AccTotal_postFSR_KP, h_AccTotal_postFSR_Ridhi, "postFSR" );
	// Comparison_MassHist( h_AccPass_postFSR_KP, h_AccPass_postFSR_Ridhi, "postFSR" );
}

void Comparison_MassHist( TH1D* h_KP, TH1D* h_Ridhi, TString FSRType = "Dressed" )
{
	TString HistType = "";
	TString HistName = h_KP->GetName();
	if( HistName.Contains("AccTotal") )
		HistType = "AccTotal";
	if( HistName.Contains("AccPass") )
		HistType = "AccPass";

	HistInfo *Hist_Ridhi = new HistInfo( kGreen+2, "Ridhi", h_Ridhi );
	HistInfo *Hist_KP = new HistInfo( kBlue, "KP ", h_KP );

	TString CanvasName = "Local/c_Mass_Dressed_KP_vs_Ridhi_"+HistType + "_" + FSRType;
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_KP, Hist_Ridhi);
	canvas->SetTitle("m [GeV]", "# events", "KP/Ridhi");
	canvas->SetLegendPosition( 0.40, 0.32, 0.97, 0.45 );
	canvas->SetRatioRange( 0.96, 1.04 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw( 1, 1 );
	canvas->c->cd();
	canvas->TopPad->cd();
	canvas->c->SaveAs(".pdf");

	cout << "HistType: " << HistType << endl;
	Print_Histogram( Hist_KP->h_ratio );
}