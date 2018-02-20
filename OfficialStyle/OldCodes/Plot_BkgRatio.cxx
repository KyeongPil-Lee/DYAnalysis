#include <Include/PlotTools.h>

void Plot_BkgRatio()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TH1::AddDirectory(kFALSE);

	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName = ROOTFilePath + "/ROOTFile_Histograms_DimuonMassSpectrum_All.root"; // -- histograms contains only statistical uncertainty (no syst. unc. from data-driven bkg.) -- //

	TH1D* h_data = Get_Hist( FileName, "h_data" );
	TH1D* h_diJet_FR = Get_Hist( FileName, "h_diJet_FR" );
	TH1D* h_WJets_FR = Get_Hist( FileName, "h_WJets_FR" );
	TH1D* h_WZ = Get_Hist( FileName, "h_WZ" );
	TH1D* h_ZZ = Get_Hist( FileName, "h_ZZ" );
	TH1D* h_WW_emu = Get_Hist( FileName, "h_WW_emu" );
	TH1D* h_tW_emu = Get_Hist( FileName, "h_tW_emu" );
	TH1D* h_DYTauTau_emu = Get_Hist( FileName, "h_DYTauTau_emu" );
	TH1D* h_ttbar_emu = Get_Hist( FileName, "h_ttbar_emu" );

	TH1D *h_totBkg = NULL;
	TH1D* h_FR = NULL;
	TH1D* h_ttbarlike = NULL;
	TH1D* h_ZZWZ = NULL;
	TH1D* h_DYTauTau = NULL;

	// -- bkg. from FR -- //
	h_diJet_FR->Sumw2(); h_WJets_FR->Sumw2();
	h_FR = (TH1D*)h_diJet_FR->Clone(); 
	h_FR->Add( h_WJets_FR );

	// -- bkg. form ttbar-like -- //
	h_ttbar_emu->Sumw2(); h_tW_emu->Sumw2(); h_WW_emu->Sumw2();
	h_ttbarlike = (TH1D*)h_ttbar_emu->Clone();
	h_ttbarlike->Add( h_tW_emu );
	h_ttbarlike->Add( h_WW_emu );

	// -- bkg. from MC -- //
	h_ZZ->Sumw2();	h_WZ->Sumw2();
	h_ZZWZ = (TH1D*)h_ZZ->Clone();
	h_ZZWZ->Add( h_WZ );

	// -- DY -> tautau -- //
	h_DYTauTau = (TH1D*)h_DYTauTau_emu->Clone();

	// -- total bkg. -- //
	h_totBkg = (TH1D*)h_FR->Clone();
	h_totBkg->Add( h_ttbarlike );
	h_totBkg->Add( h_ZZWZ );
	h_totBkg->Add( h_DYTauTau );


	// -- bkg. ratio -- //
	TH1D* h_Ratio_totBkg = (TH1D*)h_data->Clone();
	h_Ratio_totBkg->Divide( h_totBkg, h_data );

	TH1D* h_Ratio_FR = (TH1D*)h_data->Clone();
	h_Ratio_FR->Divide( h_FR, h_data );

	TH1D* h_Ratio_ttbarlike = (TH1D*)h_data->Clone();
	h_Ratio_ttbarlike->Divide( h_ttbarlike, h_data );

	TH1D* h_Ratio_ZZWZ = (TH1D*)h_data->Clone();
	h_Ratio_ZZWZ->Divide( h_ZZWZ, h_data );

	TH1D* h_Ratio_DYTauTau = (TH1D*)h_data->Clone();
	h_Ratio_DYTauTau->Divide( h_DYTauTau, h_data );

	// -- Draw canvas -- //
	HistInfo *Hist_totBkg = new HistInfo( kBlack, "Total", h_Ratio_totBkg );
	Hist_totBkg->h->SetMarkerStyle( 20 );
	Hist_totBkg->h->SetMarkerSize( 1 );

	HistInfo *Hist_FR = new HistInfo( kMagenta+2, "QCD + WJets", h_Ratio_FR );
	Hist_FR->h->SetMarkerStyle( 21 );
	Hist_FR->h->SetMarkerSize( 1 );

	HistInfo *Hist_ttbarlike = new HistInfo( kRed, "t#bar{t} + tW + WW", h_Ratio_ttbarlike );
	Hist_ttbarlike->h->SetMarkerStyle(22);
	Hist_ttbarlike->h->SetMarkerSize( 1 );

	HistInfo *Hist_ZZWZ = new HistInfo( kGreen, "ZZ + WZ", h_Ratio_ZZWZ );
	Hist_ZZWZ->h->SetMarkerStyle(23);
	Hist_ZZWZ->h->SetMarkerSize( 1 );

	HistInfo *Hist_DYTauTau = new HistInfo( kBlue-9, "DY#rightarrow#tau#tau", h_Ratio_DYTauTau );
	Hist_DYTauTau->h->SetMarkerStyle(29);
	Hist_DYTauTau->h->SetMarkerSize( 1 );

	TString CanvasName = "Local/Muon_BkgRatio";
	TCanvas *c;
	SetCanvas_Square(c, CanvasName, 1, 1, 800, 700);
	c->cd();

	Hist_totBkg->Draw("EPSAME");
	Hist_FR->Draw("EPSAME");
	Hist_ttbarlike->Draw("EPSAME");
	Hist_ZZWZ->Draw("EPSAME");
	Hist_DYTauTau->Draw("EPSAME");

	TLegend *legend;
	SetLegend(legend, 0.7, 0.15, 0.95, 0.4);
	Hist_totBkg->AddToLegend ( legend );
	Hist_FR->AddToLegend ( legend );
	Hist_ttbarlike->AddToLegend ( legend );
	Hist_ZZWZ->AddToLegend ( legend );
	Hist_DYTauTau->AddToLegend ( legend );
	legend->Draw();

	SetHistFormat_SinglePad( Hist_totBkg->h, "mass [GeV]", "Fraction of backgrounds" );
	Hist_totBkg->h->GetYaxis()->SetRangeUser(1e-5, 1);

	TLatex latex;
	latex.SetTextSize(0.03);
	latex.DrawLatexNDC(0.785, 0.96, "2.8 fb^{#font[122]{\55}1} (13 TeV)");
	latex.SetTextSize(0.035);
	latex.DrawLatexNDC(0.15, 0.90, "CMS Preliminary");

	c->SaveAs(".pdf");
	c->SaveAs(".C");
}