#include <Include/PlotTools.h>
#include <TPad.h>

TString HistName_Data = "h_DXSec";
TString HistName_RelStatUnc = "h_RelStatUnc";
TString HistName_RelTotUnc = "h_RelTotUnc";

TString HistName_NNPDF = "h_DXSec_NNPDF";
TString HistName_NNPDF_RelUnc = "h_RelTotUnc_NNPDF";
Int_t color_NNPDF = kRed;

TString HistName_CT = "h_DXSec_CT";
TString HistName_CT_RelUnc = "h_RelTotUnc_CT";
Int_t color_CT = kViolet;

TString HistName_MMHT = "h_DXSec_MMHT";
TString HistName_MMHT_RelUnc = "h_RelTotUnc_MMHT";
Int_t color_MMHT = kBlue;

TString HistName_HERAPDF = "h_DXSec_HERAPDF";
TString HistName_HERAPDF_RelUnc = "h_RelTotUnc_HERAPDF";
Int_t color_HERAPDF = kGreen+2;

TString HistName_ABM = "h_DXSec_ABM";
TString HistName_ABM_RelUnc = "h_RelTotUnc_ABM";
Int_t color_ABM = kOrange+2;

Double_t small = 0.000001; // -- margin between Pads -- //

Double_t yTitleSize_pixel = 15;
Double_t yTitleOffset = 0.35;

Double_t yLabelSize_pixel = 19;
Double_t yLabelOffset_pixel = 1;

// Double_t xTickWidth = 0.1;

void DrawOnPad_Data_vs_MC( TVirtualPad *Pad, HistInfo *Hist_Data_StatUnc, HistInfo *Hist_Data_TotUnc, HistInfo *Hist_Theory, Bool_t isLast = kFALSE )
{
	Pad->SetTopMargin(small);
	Pad->SetBottomMargin(small);
	Pad->SetLeftMargin(0.08);
	Pad->SetRightMargin(0.045);
	Pad->SetLogx();
	Pad->SetTickx();
	Hist_Theory->h_ratio->Draw("E2PSAME");
	Hist_Data_TotUnc->h_ratio->Draw("E2PSAME");
	Hist_Data_StatUnc->h_ratio->Draw("E1PSAME");

	TAxis *X_axis = Hist_Theory->h_ratio->GetXaxis();
	X_axis->SetMoreLogLabels();
	X_axis->SetNoExponent();
	X_axis->SetTitle( "" );
	X_axis->SetTitleOffset( 0.0 );
	X_axis->SetTitleSize( 0.0 );
	X_axis->SetLabelColor(1);
	X_axis->SetLabelFont(42);
	X_axis->SetLabelOffset(0.01);
	X_axis->SetLabelSize(0.13);
	// X_axis->SetTickLength( xTickWidth );

	TAxis *Y_axis = Hist_Theory->h_ratio->GetYaxis();
	Double_t ratio_Min = 0.31;
	Double_t ratio_Max = 1.69;
	Y_axis->SetTitle( Hist_Theory->LegendName+"/Data" );
	Y_axis->CenterTitle();
	Y_axis->SetTitleOffset( 0.45 );
	Y_axis->SetTitleSize( yTitleSize_pixel / (Pad->GetWh()*Pad->GetAbsHNDC()) );
	// Y_axis->SetTitleOffset( yTitleOffset_pixel / (Pad->GetWh()*Pad->GetAbsHNDC()) );
	Y_axis->SetTitleOffset( yTitleOffset );

	Y_axis->SetLabelOffset(0.005);
	Y_axis->SetLabelSize( yLabelSize_pixel / (Pad->GetWh()*Pad->GetAbsHNDC()) );
	Y_axis->SetRangeUser( ratio_Min, ratio_Max );
	Y_axis->SetNdivisions( 505 );
	// Hist_Theory->h_ratio->SetMinimum( 0.45 );
	// Hist_Theory->h_ratio->SetMaximum( 1.55 );
	// Y_axis->SetRangeUser( 0.45, 1.55 );

	TLegend *legend;
	if( isLast )
		SetLegend( legend, 0.10, 0.39, 0.50, 0.52);
	else
		SetLegend( legend, 0.10, 0.05, 0.50, 0.25);
	legend->SetTextFont(62);
	legend->AddEntry( Hist_Theory->h_ratio, Hist_Theory->LegendName );
	legend->Draw();
	
}

void Comparison_VariousPDF(TString TStr_Channel = "LL")
{
	TString FileName = TString::Format("./Local/ROOTFile_Input_Comparison_VariousPDF_%s.root", TStr_Channel.Data() );
	printf( "[FileName = %s]\n", FileName.Data() );

	// -- data x-section with stat. unc. only -- //
	TH1D* h_DXSec = Get_Hist( FileName, HistName_Data );
	HistInfo *Hist_Data_StatUnc = new HistInfo( kBlack, "Data", h_DXSec );
	Hist_Data_StatUnc->CalcRatio_DEN( Hist_Data_StatUnc->h ); // -- ratio = 1 -- //

	TH1D* h_RelStatUnc = Get_Hist( FileName, HistName_RelStatUnc );
	AssignErrors( Hist_Data_StatUnc->h, h_RelStatUnc, kFALSE);
	AssignErrors( Hist_Data_StatUnc->h_ratio, h_RelStatUnc, kFALSE);
	Hist_Data_StatUnc->h_ratio->SetMarkerStyle(20);
	// Hist_Data_StatUnc->h_ratio->SetMarkerSize(0.8);
	Hist_Data_StatUnc->h_ratio->SetMarkerSize(0);
	Hist_Data_StatUnc->h_ratio->SetLineWidth(1);
	
	// Print_Histogram( Hist_Data_StatUnc->h_ratio );

	// -- data x-section with stat. + syst. unc. without lumi. -- //
	HistInfo *Hist_Data_TotUnc = new HistInfo( kGray+1, "Data", h_DXSec );
	Hist_Data_TotUnc->CalcRatio_DEN( Hist_Data_TotUnc->h ); // -- ratio = 1 -- //

	TH1D* h_RelTotUnc = Get_Hist( FileName, HistName_RelTotUnc );
	AssignErrors( Hist_Data_TotUnc->h, h_RelTotUnc, kFALSE);
	AssignErrors( Hist_Data_TotUnc->h_ratio, h_RelTotUnc, kFALSE );
	Hist_Data_TotUnc->h_ratio->SetMarkerColorAlpha( kGray+1, 0 );
	Hist_Data_TotUnc->h_ratio->SetFillColorAlpha( kGray+1, 1 );
	gStyle->SetHatchesSpacing( 1.5 );
	Hist_Data_TotUnc->h_ratio->SetFillStyle( 3354 );
	

	// Print_Histogram( Hist_Data_TotUnc->h_ratio );

	// -- NNPDF 3.0 -- //
	TH1D* h_NNPDF = Get_Hist( FileName, HistName_NNPDF );
	HistInfo *Hist_NNPDF = new HistInfo( color_NNPDF, "NNPDF 3.0", h_NNPDF );
	Hist_NNPDF->CalcRatio_DEN( Hist_Data_TotUnc->h );

	TH1D* h_NNPDF_RelUnc = Get_Hist( FileName, HistName_NNPDF_RelUnc );
	AssignErrors( Hist_NNPDF->h_ratio, h_NNPDF_RelUnc, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
	// Hist_NNPDF->h_ratio->SetMarkerColorAlpha( color_NNPDF, 0 );
	Hist_NNPDF->h_ratio->SetMarkerStyle( kOpenTriangleDown );
 	Hist_NNPDF->h_ratio->SetMarkerColor( color_NNPDF );
	Hist_NNPDF->h_ratio->SetLineColorAlpha( color_NNPDF, 0 );
	Hist_NNPDF->h_ratio->SetFillColorAlpha( color_NNPDF, 0.4 );
	Hist_NNPDF->h_ratio->SetFillStyle( 1001 );

	// Print_Histogram( Hist_NNPDF->h_ratio );

	// -- CT14 -- //
	TH1D* h_CT = Get_Hist( FileName, HistName_CT );
	HistInfo *Hist_CT = new HistInfo( color_CT, "CT14", h_CT );
	Hist_CT->CalcRatio_DEN( Hist_Data_TotUnc->h );

	TH1D* h_CT_RelUnc = Get_Hist( FileName, HistName_CT_RelUnc );
	AssignErrors( Hist_CT->h_ratio, h_CT_RelUnc, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
	// Hist_CT->h_ratio->SetMarkerColorAlpha( color_CT, 0 );
	Hist_CT->h_ratio->SetMarkerStyle( kOpenTriangleUp );
 	Hist_CT->h_ratio->SetMarkerColor( color_CT );
	Hist_CT->h_ratio->SetLineColorAlpha( color_CT, 0 );
	Hist_CT->h_ratio->SetFillColorAlpha( color_CT, 0.4 );

	// -- MMHT2014 -- //
	TH1D* h_MMHT = Get_Hist( FileName, HistName_MMHT );
	HistInfo *Hist_MMHT = new HistInfo( color_MMHT, "MMHT2014", h_MMHT );
	Hist_MMHT->CalcRatio_DEN( Hist_Data_TotUnc->h );

	TH1D* h_MMHT_RelUnc = Get_Hist( FileName, HistName_MMHT_RelUnc );
	AssignErrors( Hist_MMHT->h_ratio, h_MMHT_RelUnc, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
	// Hist_MMHT->h_ratio->SetMarkerColorAlpha( color_MMHT, 0 );
	Hist_MMHT->h_ratio->SetMarkerStyle( kOpenCircle );
 	Hist_MMHT->h_ratio->SetMarkerColor( color_MMHT );
	Hist_MMHT->h_ratio->SetLineColorAlpha( color_MMHT, 0 );
	Hist_MMHT->h_ratio->SetFillColorAlpha( color_MMHT, 0.4 );

	// -- HERAPDF15 -- //
	TH1D* h_HERAPDF = Get_Hist( FileName, HistName_HERAPDF );
	HistInfo *Hist_HERAPDF = new HistInfo( color_HERAPDF, "HERAPDF15", h_HERAPDF );
	Hist_HERAPDF->CalcRatio_DEN( Hist_Data_TotUnc->h );

	TH1D* h_HERAPDF_RelUnc = Get_Hist( FileName, HistName_HERAPDF_RelUnc );
	AssignErrors( Hist_HERAPDF->h_ratio, h_HERAPDF_RelUnc, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
	// Hist_HERAPDF->h_ratio->SetMarkerColorAlpha( color_HERAPDF, 0 );
	Hist_HERAPDF->h_ratio->SetMarkerStyle( kOpenSquare );
	Hist_HERAPDF->h_ratio->SetMarkerColor( color_HERAPDF );
	Hist_HERAPDF->h_ratio->SetLineColorAlpha( color_HERAPDF, 0 );
	Hist_HERAPDF->h_ratio->SetFillColorAlpha( color_HERAPDF, 0.4 );

	// -- ABM12LHC -- //
	TH1D* h_ABM = Get_Hist( FileName, HistName_ABM );
	HistInfo *Hist_ABM = new HistInfo( color_ABM, "ABM12LHC", h_ABM );
	Hist_ABM->CalcRatio_DEN( Hist_Data_TotUnc->h );

	TH1D* h_ABM_RelUnc = Get_Hist( FileName, HistName_ABM_RelUnc );
	AssignErrors( Hist_ABM->h_ratio, h_ABM_RelUnc, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
	Hist_ABM->h_ratio->SetMarkerColorAlpha( color_ABM, 0 );
	Hist_ABM->h_ratio->SetMarkerStyle( kOpenDiamond );
	Hist_ABM->h_ratio->SetMarkerColor( color_ABM );
	// Hist_ABM->h_ratio->SetMarkerSize( 1 );
	Hist_ABM->h_ratio->SetLineColorAlpha( color_ABM, 0 );
	Hist_ABM->h_ratio->SetFillColorAlpha( color_ABM, 0.4 );

	// -- Draw Canvas -- //
	TCanvas *c;
	// TString CanvasName = "c_Comparison_VariousPDF_"+TStr_Channel;
	TString channelType = "";
	if( TStr_Channel == "LL" ) channelType = "Combined";
	if( TStr_Channel == "MM" ) channelType = "Muon";
	if( TStr_Channel == "EE" ) channelType = "Electron";
	TString CanvasName = TString::Format("%s_RatioWithData_VariousPDF", channelType.Data());
	SetCanvas_Square( c, "./Local/"+CanvasName );

	gStyle->SetPadBorderMode(0);
	gStyle->SetFrameBorderMode(0);

	// -- just for drawing latex -- //
	BigPad = new TPad("BigPad","BigPad", 0.01, 0.01, 0.99, 0.99 );
	BigPad->Draw();
	BigPad->cd();
	BigPad->SetTopMargin(0.05);
	BigPad->SetBottomMargin(0.05);
	// BigPad->SetFillStyle(4050);


	Double_t UpperMargin = 0.05;
	Double_t LowerMargin = 0.1;

	// -- ySize_LastPad * LowerMargin_LastPad = LowerMargin -- //
	Double_t ySize_Pads = 0;
	Double_t ySize_LastPad = 0;
	Double_t LowerMargin_LastPad = 0;

	// -- 5*ySize_Pads = 1-UpperMargin-LowerMargin -- //
	ySize_Pads = ( 1 - UpperMargin - LowerMargin ) / 5.0;

	ySize_LastPad = ySize_Pads + LowerMargin;

	// -- ySize_LastPad * LowerMargin_LastPad = LowerMargin -- //
	LowerMargin_LastPad = LowerMargin / ySize_LastPad;

	printf("[ySize] (Pads, LastPads) = (%.3lf, %.3lf), [LowerMargin_LastPad = %.3lf]\n", ySize_Pads, ySize_LastPad, LowerMargin_LastPad);

	// -- inner pad with top and bottom margin -- //
	TPad *MultiPad = new TPad("MultiPad", "MultiPad", 0.01, ySize_LastPad, 0.99, 1-UpperMargin );
	MultiPad->Draw();
	MultiPad->cd();
	// MultiPad->SetBottomMargin(0.1);
	MultiPad->Divide(1, 4, small, small);

	MultiPad->cd(1);
	DrawOnPad_Data_vs_MC( gPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_NNPDF );

	MultiPad->cd(2);
	DrawOnPad_Data_vs_MC( gPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_CT );

	MultiPad->cd(3);
	DrawOnPad_Data_vs_MC( gPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_MMHT );

	MultiPad->cd(4);
	DrawOnPad_Data_vs_MC( gPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_HERAPDF );

	BigPad->cd();
	TPad *LastPad = new TPad( "LastPad", "LastPad", 0.01, 0.00, 0.99, ySize_LastPad );
	LastPad->Draw();
	LastPad->cd();
	DrawOnPad_Data_vs_MC( LastPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_ABM, kTRUE );
	LastPad->SetBottomMargin( LowerMargin_LastPad );
	Hist_ABM->h_ratio->GetYaxis()->SetTitleOffset( yTitleOffset * (ySize_LastPad / ySize_Pads) );
	Hist_ABM->h_ratio->GetXaxis()->SetTitleSize( 0.2 );
	Hist_ABM->h_ratio->GetXaxis()->SetTitleOffset( 0.8 );
	Hist_ABM->h_ratio->GetXaxis()->SetTitle( "m [GeV]" );
	TLegend *legend_lastPad = (TLegend*)LastPad->GetPrimitive("TPave");
	legend_lastPad->SetY1NDC(0.3);
	legend_lastPad->SetY2NDC(0.6);

	BigPad->cd();
	TLatex latex;
	Double_t lumi = 2.8;
	if( TStr_Channel == "EE" ) lumi = 2.3;
	Int_t E_CM = 13;

	// latex.DrawLatexNDC(0.09, 0.96, "#font[62]{CMS}");
	// latex.DrawLatexNDC(0.19, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
	latex.DrawLatexNDC(0.09, 0.96, "#font[62]{#scale[0.8]{CMS}}");
	// latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");

	if( TStr_Channel == "MM" || TStr_Channel == "EE" )
		latex.DrawLatexNDC(0.69, 0.96, "#font[62]{#scale[0.7]{"+TString::Format("%.1lf fb^{-1} (%d TeV)", lumi, E_CM)+"}}");
	else if( TStr_Channel == "LL" )
		latex.DrawLatexNDC( 0.46, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (ee)  2.8 fb^{-1} (#mu#mu) (13 TeV)}}");

	// latex.DrawLatexNDC( 0.76, 0.004, "#font[42]{#scale[0.8]{m (#mu#mu) [GeV]}}");

	c->SaveAs(".pdf");
	TString PlotPath = gSystem->Getenv("KP_PLOT_PATH");
	c->SaveAs(PlotPath+"/"+CanvasName+".pdf");




	// BigPad->cd();
	// Pad1 = new TPad("Pad1", "Pad1", 0.001, 0.05, 0.999, 0.23);
	// Pad1->Draw();
	// Pad1->cd();
	// Pad1->SetTopMargin(0.0);
	// Pad1->SetBottomMargin(0.0);
	// Pad1->SetLeftMargin(0.13);
	// Pad1->SetRightMargin(0.045);
	// Pad1->SetBorderSize(0);
	// // Pad1->SetFillStyle(4100);
	// Hist_Data_StatUnc->h->Draw("E1PSAME");

	// BigPad->cd();
	// Pad2 = new TPad("Pad2", "Pad2", 0.001, 0.23, 0.999, 0.41);
	// Pad2->Draw();
	// Pad2->cd();
	// Pad2->SetBorderSize(0);
	// Pad2->SetTopMargin(0.0);
	// Pad2->SetBottomMargin(0.0);
	// Pad2->SetLeftMargin(0.13);
	// Pad2->SetRightMargin(0.045);
	// Hist_Data_StatUnc->h->Draw("E1PSAME");

	// gROOT->Reset();




}