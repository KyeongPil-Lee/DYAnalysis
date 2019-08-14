#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>

enum ColorCode
{
	kStat = kBlack,
	kStatSyst = kGray+1,
	kFEWZ = kBlue,
	kaMCNLO = kRed
};

class DrawingTool
{
public:
	TString ROOTFilePath;
	TString Type_Channel;

	TH1D* h_data;
	TH1D* h_data_StatOnly;
	TH1D* h_aMCNLO;
	TH1D* h_FEWZ;

	TH1D* h_Ratio_StatUnc; // -- Data / Data with Stat. only -- //
	TH1D* h_Ratio_TotUnc; // -- Data / Data with Stat. + Syst. + Lumi (= Experimental total unc.) -- //

	TH1D* h_Ratio_aMCNLO; // -- aMCNLO / Data with theory uncertainty only -- //
	TH1D* h_Ratio_FEWZ; // -- FEWZ / Data with theory uncertainty only -- //

	// -- Fiducial, post-FSR -- //
	TH1D* h_FpoF_data;
	TH1D* h_FpoF_data_StatOnly;
	TH1D* h_FpoF_aMCNLO;
	TH1D* h_FpoF_Ratio_StatUnc;
	TH1D* h_FpoF_Ratio_TotUnc;
	TH1D* h_FpoF_Ratio_aMCNLO;

	DrawingTool()
	{
		this->Init();
	}

	void Set_Channel(TString _Type_Channel)
	{
		this->Type_Channel = _Type_Channel;
	}

	void Draw_FpoF()
	{
		///////////////////////
		// -- Draw Canvas -- //
		///////////////////////
		gStyle->SetErrorX(0); // -- remove horizontal error bar in the plot

		TString CanvasName = "Local/c_FpoF_DXSec_"+this->Type_Channel;
		TCanvas *c = new TCanvas(CanvasName, "", 0, 0, 800, 700);
		// c->SetTopMargin(0.08);
		// c->SetBottomMargin(0.14);
		this->SetBasicOptions_Canvas( c );
		c->cd();

		gStyle->SetOptFit(1);
		gStyle->SetOptStat(0);
		gStyle->SetOptTitle(0);

		// -- Top Pad -- //
		TPad *TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
		TopPad->Draw();
		TopPad->cd();

		gPad->SetLogx();
		gPad->SetLogy();

		// -- ensure additional space at the bottom side for ratio plot -- //
		TopPad->SetLeftMargin(0.13);
		TopPad->SetRightMargin(0.05);
		TopPad->SetTopMargin(0.05);
		TopPad->SetBottomMargin(0.32);
		this->SetBasicOptions_Pad( TopPad );


		this->h_FpoF_data->Draw("AXIS");
		this->h_FpoF_aMCNLO->Draw("E1PSAME");
		this->h_FpoF_data->Draw("E1PSAME");

		// -- General Setting for both plots -- //
		this->h_FpoF_data->SetXTitle( "m [GeV]" );
		this->h_FpoF_data->SetYTitle( "d#sigma/dm [pb/GeV]" );

		this->h_FpoF_data->SetStats(kFALSE);
		this->h_FpoF_aMCNLO->SetStats(kFALSE);

		// -- Setting for h1 -- //
		this->h_FpoF_data->SetLineColor(kStat);
		this->h_FpoF_data->SetLineWidth(1);
		this->h_FpoF_data->SetMarkerColor(kStat);
		this->h_FpoF_data->SetMarkerSize(1);
		this->h_FpoF_data->SetMarkerStyle(20);
		this->h_FpoF_data->SetFillColorAlpha(kWhite, 0);

		// -- Setting for h2 -- //
		this->h_FpoF_aMCNLO->SetLineColor(kaMCNLO);
		this->h_FpoF_aMCNLO->SetLineWidth(1);
		this->h_FpoF_aMCNLO->SetMarkerColor(kaMCNLO);
		this->h_FpoF_aMCNLO->SetMarkerSize(1);
		this->h_FpoF_aMCNLO->SetMarkerStyle(26);
		this->h_FpoF_aMCNLO->SetFillColorAlpha(kWhite, 0);
		this->h_FpoF_aMCNLO->SetFillStyle( 3144 );

		// -- X-axis Setting -- //
		this->h_FpoF_data->GetXaxis()->SetLabelSize(0);
		this->h_FpoF_data->GetXaxis()->SetTitleSize(0);
		this->h_FpoF_data->GetXaxis()->SetNoExponent(); 
		this->h_FpoF_data->GetXaxis()->SetMoreLogLabels();
		this->h_FpoF_data->GetXaxis()->SetRangeUser( 15, 3000 );
		this->h_FpoF_data->SetMinimum(5e-8);
		this->h_FpoF_data->SetMaximum(4000);

		// -- Y-axis Setting -- //
		this->h_FpoF_data->GetYaxis()->SetTitleSize(0.06);
		this->h_FpoF_data->GetYaxis()->SetTitleOffset(0.9);

		// -- Add Legend -- //
		TLegend *legend;
		legend = new TLegend(0.15, 0.35, 0.50, 0.60 );

		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->SetTextFont(62);
		legend->AddEntry(this->h_FpoF_data, "Data", "EP");
		legend->AddEntry(this->h_FpoF_aMCNLO, "aMC@NLO", "EP");
		legend->Draw();

		// -- write down some sentences -- //
		TLatex latex;
		latex.DrawLatexNDC(0.14, 0.96, "#font[62]{#scale[0.8]{CMS}}");
		
		if( this->Type_Channel == "Muon" )
		{
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");
			latex.SetTextSize(0.045);
			latex.DrawLatexNDC(0.65, 0.85, "#gamma* /#font[122]{Z} #rightarrow #mu#mu");
			latex.DrawLatexNDC(0.65, 0.78, "#font[42]{#scale[0.5]{p_{T}^{lead}(#mu) > 22 GeV, p_{T}^{sub}(#mu) > 10 GeV}}");
			latex.DrawLatexNDC(0.65, 0.73, "#font[42]{#scale[0.5]{|#eta(#mu)| < 2.4}}");
		}
		else if( this->Type_Channel == "Electron" )
		{
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (13 TeV)}}");
			latex.SetTextSize(0.045);
			latex.DrawLatexNDC(0.65, 0.85, "#gamma* /#font[122]{Z} #rightarrow ee");
			latex.DrawLatexNDC(0.65, 0.78, "#font[42]{#scale[0.5]{p_{T}^{lead}(e) > 30 GeV, p_{T}^{sub}(e) > 10 GeV}}");
			latex.DrawLatexNDC(0.65, 0.73, "#font[42]{#scale[0.5]{|#eta(e)| < 2.5, |#eta(e)| #notin (1.44, 1.57)}}");
		}

		//////////////////////
		// -- Bottom Pad -- //
		//////////////////////
		TExec *ex = new TExec("ex","gStyle->SetErrorX(0.5);"); // -- make horizontal error bar in the plot
		ex->Draw();

		TPad *bottomPad = new TPad("bottomPad","bottomPad",0.01,0.01,0.99,0.3);
		bottomPad->Draw();
		bottomPad->cd();

		bottomPad->SetLeftMargin(0.122);
		bottomPad->SetRightMargin(0.04);
		bottomPad->SetTopMargin(0.05);
		bottomPad->SetBottomMargin(0.4);
		this->SetBasicOptions_Pad( bottomPad );

		gPad->SetLogx();

		// -- draw -- //
		this->h_FpoF_Ratio_aMCNLO->Draw("AXIS");
		this->h_FpoF_Ratio_TotUnc->Draw("E2PSAME");
		this->h_FpoF_Ratio_StatUnc->Draw("E1PSAME");
		this->h_FpoF_Ratio_aMCNLO->Draw("E2SAME");


		TLegend *legend_ratio = new TLegend(0.15, 0.80, 0.70, 0.95);
		legend_ratio->SetNColumns(3);
		legend_ratio->SetFillStyle(0);
		legend_ratio->SetBorderSize(0);
		legend_ratio->SetTextFont(62);
		legend_ratio->AddEntry( h_FpoF_Ratio_StatUnc, "Stat. unc.", "EP");
		legend_ratio->AddEntry( h_FpoF_Ratio_TotUnc, "Tot. unc.", "PF");
		legend_ratio->AddEntry( h_FpoF_Ratio_aMCNLO, "Theo. unc. (aMC@NLO)", "PF");
		legend_ratio->Draw();

		// -- General Setting -- //
		// h_ratio->SetLineColor(kBlack);
		// h_ratio->SetMarkerStyle(20);
		// h_ratio->SetMarkerSize(1);
		// h_ratio->SetMarkerColor(kBlack);
		// h_ratio->SetFillColorAlpha(kRed-9, 0.5);
		// h_ratio->SetFillStyle(1001);
		// h_ratio->SetStats(kFALSE);

		// h_ratio_StatUnc->SetLineColor(kBlue);
		// h_ratio_StatUnc->SetMarkerStyle(20);
		// h_ratio_StatUnc->SetMarkerSize(1);
		// h_ratio_StatUnc->SetMarkerColor(kBlack);
		// h_ratio_StatUnc->SetFillColorAlpha(kBlue-9, 1);
		// h_ratio_StatUnc->SetFillStyle(3144);

		// h_ratio_StatUnc->SetStats(kFALSE);

		// -- X-axis Setting -- //
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetTitle( "m [GeV]" );
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetTitleOffset( 0.9 );
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetTitleSize( 0.2 );
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetLabelColor(1);
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetLabelFont(42);
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetLabelOffset(0.007);
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetLabelSize(0.15);
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetMoreLogLabels(); 
		h_FpoF_Ratio_aMCNLO->GetXaxis()->SetNoExponent();
		
		h_FpoF_Ratio_aMCNLO->GetYaxis()->SetTitle( "aMC@NLO/Data" );
		h_FpoF_Ratio_aMCNLO->GetYaxis()->SetTitleOffset( 0.4 );
		h_FpoF_Ratio_aMCNLO->GetYaxis()->SetTitleSize( 0.1);
		h_FpoF_Ratio_aMCNLO->GetYaxis()->SetLabelSize( 0.07 );
		h_FpoF_Ratio_aMCNLO->GetYaxis()->SetRangeUser( 0.25, 1.75 );

		c->SaveAs(".pdf");
		// c->SaveAs(".C");

    Draw_RatioZoomIn( (TPad*)bottomPad->Clone(), "Local/"+Type_Channel+"_FpoF_Ratio_Data_vs_Theory_aMCNLO", 
                      this->h_FpoF_Ratio_aMCNLO->GetName(), legend_ratio->GetName() );
	}

	void Draw_Dressed()
	{
		this->Get_Histogram();

		gStyle->SetErrorX(0); // -- remove horizontal error bar in the plot

		///////////////////////
		// -- Draw Canvas -- //
		///////////////////////
		TString CanvasName = "Local/c_DXSec_"+this->Type_Channel;
		TCanvas *c = new TCanvas(CanvasName, "", 0, 0, 800, 700);
		// c->SetTopMargin(0.08);
		// c->SetBottomMargin(0.14);
		this->SetBasicOptions_Canvas( c );
		c->cd();

		gStyle->SetOptFit(1);
		gStyle->SetOptStat(0);
		gStyle->SetOptTitle(0);

		// -- Top Pad -- //
		TPad *TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
		TopPad->Draw();
		TopPad->cd();

		gPad->SetLogx();
		gPad->SetLogy();

		// -- ensure additional space at the bottom side for ratio plot -- //
		TopPad->SetLeftMargin(0.13);
		TopPad->SetRightMargin(0.05);
		TopPad->SetTopMargin(0.05);
		TopPad->SetBottomMargin(0.40);
		this->SetBasicOptions_Pad( TopPad );


		this->h_data->Draw("E1P");
		this->h_FEWZ->Draw("E1PSAME");
		this->h_aMCNLO->Draw("E1PSAME");
		this->h_data->Draw("E1PSAME");

		// -- General Setting for both plots -- //
		this->h_data->SetXTitle( "m [GeV]" );
		this->h_data->SetYTitle( "d#sigma/dm [pb/GeV]" );

		this->h_data->SetStats(kFALSE);
		this->h_FEWZ->SetStats(kFALSE);
		this->h_aMCNLO->SetStats(kFALSE);

		// -- Setting for h1 -- //
		this->h_data->SetLineColor(kStat);
		this->h_data->SetLineWidth(1);
		this->h_data->SetMarkerColor(kStat);
		this->h_data->SetMarkerSize(1);
		this->h_data->SetMarkerStyle(20);
		this->h_data->SetFillColorAlpha(kWhite, 0);

		// -- Setting for h2 -- //
		this->h_FEWZ->SetLineColor(kFEWZ);
		this->h_FEWZ->SetLineWidth(1);
		this->h_FEWZ->SetMarkerColor(kFEWZ);
		this->h_FEWZ->SetMarkerSize(1);
		this->h_FEWZ->SetMarkerStyle(32);
		this->h_FEWZ->SetFillColorAlpha(kWhite, 0);
		// this->h_FEWZ->SetFillStyle( 3144 );

		// -- Setting for h3 -- //
		this->h_aMCNLO->SetLineColor(kaMCNLO);
		this->h_aMCNLO->SetLineWidth(1);
		this->h_aMCNLO->SetMarkerColor(kaMCNLO);
		this->h_aMCNLO->SetMarkerSize(1);
		this->h_aMCNLO->SetMarkerStyle(26);
		this->h_aMCNLO->SetFillColorAlpha(kWhite, 0);
		// this->h_aMCNLO->SetFillStyle( 3144 );

		// -- X-axis Setting -- //
		this->h_data->GetXaxis()->SetLabelSize(0);
		this->h_data->GetXaxis()->SetTitleSize(0);
		this->h_data->GetXaxis()->SetNoExponent(); 
		this->h_data->GetXaxis()->SetMoreLogLabels();
		this->h_data->GetXaxis()->SetRangeUser( 15, 3000 );

		// -- Y-axis Setting -- //
		this->h_data->GetYaxis()->SetTitleSize(0.06);
		this->h_data->GetYaxis()->SetTitleOffset(0.9);
		this->h_data->SetMinimum(5e-8);
		this->h_data->SetMaximum(4000);

		// -- Add Legend -- //
		TLegend *legend = new TLegend(0.10, 0.43, 0.70, 0.70);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->SetTextFont(62);
		legend->AddEntry(h_data, "Data", "EP");
		legend->AddEntry(h_aMCNLO, "aMC@NLO", "EP");
		legend->AddEntry(h_FEWZ, "FEWZ (NNLO QCD + NLO EW)", "EP");
		legend->Draw();

		// -- write down some sentences -- //
		TLatex latex;
		latex.DrawLatexNDC(0.14, 0.96, "#font[62]{#scale[0.8]{CMS}}");
		
		if( this->Type_Channel == "Muon" )
		{
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");
			latex.SetTextSize(0.045);
			latex.DrawLatexNDC(0.65, 0.85, "#gamma* /#font[122]{Z} #rightarrow #mu#mu");
		}
		else if( this->Type_Channel == "Electron" )
		{
			latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (13 TeV)}}");
			latex.SetTextSize(0.045);
			latex.DrawLatexNDC(0.65, 0.85, "#gamma* /#font[122]{Z} #rightarrow ee");
		}

		/////////////////////////////////////
		// -- Bottom Pad1: Data/aMC@NLO -- //
		/////////////////////////////////////
		TExec *ex = new TExec("ex","gStyle->SetErrorX(0.5);"); // -- make horizontal error bar in the plot
		ex->Draw();
		
		TPad *bottomPad1 = new TPad("bottomPad1","bottomPad1",0.01,0.25,0.99,0.40);
		bottomPad1->Draw();
		bottomPad1->cd();

		bottomPad1->SetLeftMargin(0.122);
		bottomPad1->SetRightMargin(0.04);
		bottomPad1->SetTopMargin(0.05);
		bottomPad1->SetBottomMargin(0.01);
		this->SetBasicOptions_Pad( bottomPad1 );

		gPad->SetLogx();

		this->h_Ratio_aMCNLO->Draw("AXIS");
		this->h_Ratio_TotUnc->Draw("E2PSAME");
		this->h_Ratio_StatUnc->Draw("E1PSAME");
		this->h_Ratio_aMCNLO->Draw("E2PSAME");		

		TLegend *legend_aMCNLO = new TLegend(0.15, 0.69, 0.70, 0.96);
		legend_aMCNLO->SetNColumns(3);
		legend_aMCNLO->SetFillStyle(0);
		legend_aMCNLO->SetBorderSize(0);
		legend_aMCNLO->SetTextFont(62);
		legend_aMCNLO->AddEntry( h_Ratio_StatUnc, "Stat. unc.", "EP");
		legend_aMCNLO->AddEntry( h_Ratio_TotUnc, "Tot. unc.", "PF");
		legend_aMCNLO->AddEntry( h_Ratio_aMCNLO, "Theo. unc. (aMC@NLO)", "PF");
		legend_aMCNLO->Draw();

		// -- Y-axis Setting -- //
		h_Ratio_aMCNLO->GetYaxis()->SetTitle( "aMC@NLO/Data" );
		h_Ratio_aMCNLO->GetYaxis()->SetTitleOffset( 0.30 );
		h_Ratio_aMCNLO->GetYaxis()->SetTitleSize( 0.13 );
		h_Ratio_aMCNLO->GetYaxis()->SetLabelSize( 0.10 );
		h_Ratio_aMCNLO->GetYaxis()->SetRangeUser( 0.25, 1.75 );

		//////////////////////////////////
		// -- Bottom Pad2: Data/FEWZ -- //
		//////////////////////////////////
		TopPad->cd();
		TPad *bottomPad2 = new TPad("bottomPad2","bottomPad2",0.01,0.01,0.99,0.25);
		bottomPad2->Draw();
		bottomPad2->cd();
		bottomPad2->SetLeftMargin(0.122);
		bottomPad2->SetRightMargin(0.04);
		bottomPad2->SetTopMargin(0.05);
		bottomPad2->SetBottomMargin(0.4);

		gPad->SetLogx();

		this->h_Ratio_FEWZ->Draw("AXIS");
		this->h_Ratio_TotUnc->Draw("E2PSAME");
		this->h_Ratio_StatUnc->Draw("E1PSAME");
		this->h_Ratio_FEWZ->Draw("E2SAME");
		
		TLegend *legend_FEWZ = new TLegend(0.15, 0.80, 0.65, 0.96 );
		legend_FEWZ->SetNColumns(3);
		legend_FEWZ->SetFillStyle(0);
		legend_FEWZ->SetBorderSize(0);
		legend_FEWZ->SetTextFont(62);
		legend_FEWZ->AddEntry( h_Ratio_StatUnc, "Stat. unc.", "EP");
		legend_FEWZ->AddEntry( h_Ratio_TotUnc, "Tot. unc.", "PF");
		legend_FEWZ->AddEntry( h_Ratio_FEWZ, "Theo. unc. (FEWZ)", "PF");
		legend_FEWZ->Draw();

		// -- X-axis setting -- //
		h_Ratio_FEWZ->GetXaxis()->SetTitle( "m [GeV]" );
		h_Ratio_FEWZ->GetXaxis()->SetTitleOffset( 0.9 );
		h_Ratio_FEWZ->GetXaxis()->SetTitleSize( 0.2 );
		h_Ratio_FEWZ->GetXaxis()->SetLabelColor(1);
		h_Ratio_FEWZ->GetXaxis()->SetLabelFont(42);
		h_Ratio_FEWZ->GetXaxis()->SetLabelOffset(0.007);
		h_Ratio_FEWZ->GetXaxis()->SetLabelSize(0.15);
		h_Ratio_FEWZ->GetXaxis()->SetMoreLogLabels(); 
		h_Ratio_FEWZ->GetXaxis()->SetNoExponent();

		// -- Y-axis Setting -- //
		h_Ratio_FEWZ->GetYaxis()->SetTitle( "FEWZ/Data" );
		h_Ratio_FEWZ->GetYaxis()->SetTitleOffset( 0.4 );
		h_Ratio_FEWZ->GetYaxis()->SetTitleSize( 0.1);
		h_Ratio_FEWZ->GetYaxis()->SetLabelSize( 0.07 );
		h_Ratio_FEWZ->GetYaxis()->SetRangeUser( 0.25, 1.75 );

		c->SaveAs(".pdf");
		// c->SaveAs(".C");

    Draw_RatioZoomIn( (TPad*)bottomPad1->Clone(), "Local/"+Type_Channel+"_Ratio_Data_vs_Theory_aMCNLO", 
                      this->h_Ratio_aMCNLO->GetName(), legend_aMCNLO->GetName() );

    Draw_RatioZoomIn( (TPad*)bottomPad2->Clone(), "Local/"+Type_Channel+"_Ratio_Data_vs_Theory_FEWZ", 
                      this->h_Ratio_FEWZ->GetName(), legend_FEWZ->GetName() );
	}

private:
	void Init()
	{
		this->ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		this->Type_Channel= "";

		this->h_data = NULL;
		this->h_data_StatOnly = NULL;
		this->h_aMCNLO = NULL;
		this->h_FEWZ = NULL;

		this->h_Ratio_StatUnc = NULL;
		this->h_Ratio_TotUnc = NULL;

		this->h_Ratio_aMCNLO = NULL;
		this->h_Ratio_FEWZ = NULL;
	}

	void Get_Histogram()
	{
		if( this->Type_Channel == "Muon" )
		{
			TString FileName_Data = ROOTFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
			this->h_data = Get_Hist(FileName_Data, "h_DiffXsec_FSRCorr"); // -- Stat. + Syst. + Lumi -- //
			this->h_data_StatOnly = Get_Hist( FileName_Data, "h_DiffXsec_FSRCorr_StatOnly");
			this->h_aMCNLO = Get_Hist( FileName_Data, "h_DiffXsec_aMCNLO");
			this->h_FEWZ = Get_Hist( FileName_Data, "h_DiffXsec_FEWZ_NNPDF_NNLO");

			TString FileName_FpoF_Data = ROOTFilePath + "/ROOTFile_FpoF_DiffXSec_FullUnc.root";
			this->h_FpoF_data = Get_Hist(FileName_FpoF_Data, "h_FpoF_DiffXsec_Data");
			this->h_FpoF_data_StatOnly = Get_Hist(FileName_FpoF_Data, "h_FpoF_DiffXsec_Data_StatOnly");
			this->h_FpoF_aMCNLO = Get_Hist(FileName_FpoF_Data, "h_FpoF_DiffXsec_aMCNLO");
		}
		else if( this->Type_Channel == "Electron" )
		{
			// TString FileName_Data = ROOTFilePath + "/DiffXsec_Electron_v8.root";
			// TString FileName_Data = ROOTFilePath + "/DiffXsec_Electron_v9.root"; // -- update from v8: apply efficiency correction to fiducial cross section (it was missed by mistake)
			// TString FileName_Data = ROOTFilePath + "/DiffXsec_Electron_v10.root"; // -- update from v9: several bugs in data/MC are fixed
			// TString FileName_Data = ROOTFilePath + "/DiffXsec_Electron_v12_byKP.root"; // -- update from v10: diff. x-section & uncertainties from efficiency SF w/ new TnP efficiency
			TString FileName_Data = ROOTFilePath + "/DiffXsec_Electron_v13_byKP.root"; // -- update from v13: new efficiency SF
			this->h_data = Get_Hist(FileName_Data, "h_DiffXSec");

			TH1D* h_RelUnc_Stat = Get_Hist(FileName_Data, "h_RelUnc_Stat"); h_RelUnc_Stat->Scale( 0.01 );
			TH1D* h_RelUnc_Syst = Get_Hist(FileName_Data, "h_RelUnc_Syst"); h_RelUnc_Syst->Scale( 0.01 );
			TH1D* h_RelUnc_Lumi = this->MakeHist_RelLumiUnc( 0.023 );
			this->AssignTotalError( this->h_data, h_RelUnc_Stat, h_RelUnc_Syst, h_RelUnc_Lumi );

			this->h_data_StatOnly = this->MakeHist_DXSecStatOnly( this->h_data, h_RelUnc_Stat );
			this->h_aMCNLO = this->MakeHist_aMCNLO_Electron("dressed");
			this->h_FEWZ = Get_Hist( FileName_Data, "h_DiffXSec_FEWZ" );

			// -- fiducial, post-FSR -- //
			this->h_FpoF_data = Get_Hist( FileName_Data, "h_DiffXSec_Fiducial" );

			TH1D* h_FpoF_RelUnc_Stat = Get_Hist( FileName_Data, "h_RelUnc_Stat_Fiducial" ); h_FpoF_RelUnc_Stat->Scale( 0.01 );
			TH1D* h_FpoF_RelUnc_Syst = Get_Hist( FileName_Data, "h_RelUnc_Syst_Fiducial" ); h_FpoF_RelUnc_Syst->Scale( 0.01 );
			this->AssignTotalError( this->h_FpoF_data, h_FpoF_RelUnc_Stat, h_FpoF_RelUnc_Syst, h_RelUnc_Lumi );

			this->h_FpoF_data_StatOnly = this->MakeHist_DXSecStatOnly( this->h_FpoF_data, h_FpoF_RelUnc_Stat );
			
			// -- MC fiducial cross section
			// this->h_FpoF_aMCNLO = this->MakeHist_aMCNLO_Electron("FpoF"); // -- do not use my result

			this->h_FpoF_aMCNLO = Get_Hist( FileName_Data, "h_DiffXSec_MCNLO_Fiducial" );
			TString FileName_RelUnc = this->ROOTFilePath+"/ROOTFile_TheoryUnc_aMCNLO.root";
			TString HistName_RelUnc = "Electron_withinAcc/h_RelUnc_Tot";
			TH1D* h_FpoF_RelUnc_aMCNLO = Get_Hist( FileName_RelUnc, HistName_RelUnc );
			AssignErrors( this->h_FpoF_aMCNLO, h_FpoF_RelUnc_aMCNLO );
		}

		this->Set_RatioHistogram();
	}

	void Set_RatioHistogram()
	{
		TH1D* h_RelUnc_Tot = Extract_RelUnc( this->h_data );
		TH1D* h_RelUnc_Stat = Extract_RelUnc( this->h_data_StatOnly );

		this->h_Ratio_StatUnc = this->Assign_Uncertainty_Ratio1Plot( h_RelUnc_Stat );
		this->h_Ratio_TotUnc = this->Assign_Uncertainty_Ratio1Plot( h_RelUnc_Tot );

		// -- theory / data -- //
		this->h_Ratio_aMCNLO = this->MakeRatioHist_DataTheory( this->h_aMCNLO, this->h_data );
		this->h_Ratio_FEWZ = this->MakeRatioHist_DataTheory( this->h_FEWZ, this->h_data );

		// -- Set styles -- //
		this->h_Ratio_StatUnc->SetMarkerStyle(20);
		this->h_Ratio_StatUnc->SetMarkerSize(0);
		this->h_Ratio_StatUnc->SetMarkerColor(kStat);
		this->h_Ratio_StatUnc->SetLineColor(kStat);
		this->h_Ratio_StatUnc->SetFillColorAlpha(kWhite, 0);

		this->h_Ratio_TotUnc->SetMarkerColorAlpha(kWhite, 0);
		this->h_Ratio_TotUnc->SetLineColorAlpha(kWhite, 0);
		this->h_Ratio_TotUnc->SetFillColorAlpha( kStatSyst, 1 );
		this->h_Ratio_TotUnc->SetFillStyle( 3354 );

		this->h_Ratio_FEWZ->SetMarkerColor(kBlue);
		this->h_Ratio_FEWZ->SetMarkerStyle( 32 );
		// this->h_Ratio_FEWZ->SetMarkerSize( 0.8 );
		this->h_Ratio_FEWZ->SetLineColorAlpha(kWhite, 0);
		this->h_Ratio_FEWZ->SetFillColorAlpha( kFEWZ, 0.4 );
		this->h_Ratio_FEWZ->SetFillStyle( 1001 );

		this->h_Ratio_aMCNLO->SetMarkerStyle( 26 );
		this->h_Ratio_aMCNLO->SetMarkerColor(kRed);
		// this->h_Ratio_aMCNLO->SetMarkerSize( 0.8 );
		this->h_Ratio_aMCNLO->SetLineColorAlpha(kWhite, 0);
		this->h_Ratio_aMCNLO->SetFillColorAlpha( kaMCNLO, 0.4 );
		this->h_Ratio_aMCNLO->SetFillStyle( 1001 );

		// -- fiducial, post-FSR -- //
		TH1D* h_FpoF_RelUnc_Tot = Extract_RelUnc( this->h_FpoF_data );
		TH1D* h_FpoF_RelUnc_Stat = Extract_RelUnc( this->h_FpoF_data_StatOnly );

		this->h_FpoF_Ratio_StatUnc = this->Assign_Uncertainty_Ratio1Plot( h_FpoF_RelUnc_Stat );
		this->h_FpoF_Ratio_TotUnc = this->Assign_Uncertainty_Ratio1Plot( h_FpoF_RelUnc_Tot );

		// -- theory / data -- //
		this->h_FpoF_Ratio_aMCNLO = this->MakeRatioHist_DataTheory( this->h_FpoF_aMCNLO, this->h_FpoF_data );

		// -- Set styles -- //
		this->h_FpoF_Ratio_StatUnc->SetMarkerStyle(20);
		this->h_FpoF_Ratio_StatUnc->SetMarkerSize(0);
		this->h_FpoF_Ratio_StatUnc->SetMarkerColor(kStat);
		this->h_FpoF_Ratio_StatUnc->SetLineColor(kStat);
		this->h_FpoF_Ratio_StatUnc->SetFillColorAlpha(kWhite, 0);

		this->h_FpoF_Ratio_TotUnc->SetMarkerColorAlpha(kWhite, 0);
		this->h_FpoF_Ratio_TotUnc->SetLineColorAlpha(kWhite, 0);
		this->h_FpoF_Ratio_TotUnc->SetFillColorAlpha( kStatSyst, 1 );
		this->h_FpoF_Ratio_TotUnc->SetFillStyle( 3354 );

		this->h_FpoF_Ratio_aMCNLO->SetMarkerStyle( 26 );
		this->h_FpoF_Ratio_aMCNLO->SetMarkerColor(kRed);
		// this->h_FpoF_Ratio_aMCNLO->SetMarkerSize( 0.8 );
		this->h_FpoF_Ratio_aMCNLO->SetLineColorAlpha(kWhite, 0);
		this->h_FpoF_Ratio_aMCNLO->SetFillColorAlpha( kaMCNLO, 0.4 );
		this->h_FpoF_Ratio_aMCNLO->SetFillStyle( 1001 );
	}

	TH1D* Assign_Uncertainty_Ratio1Plot( TH1D* h_RelUnc )
	{
		TH1D* h_Ratio = (TH1D*)h_RelUnc->Clone();
		Int_t nBin = h_Ratio->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t RelUnc = h_RelUnc->GetBinContent(i_bin);
			h_Ratio->SetBinContent(i_bin, 1 );
			h_Ratio->SetBinError( i_bin, 1*RelUnc );
		}
		return h_Ratio;
	}

	TH1D* MakeRatioHist_DataTheory( TH1D* h_theory, TH1D* h_data )
	{
		TH1D* h_Ratio = (TH1D*)h_theory->Clone();
		TH1D* h_RelUnc_Theory = Extract_RelUnc( h_theory );

		Int_t nBin = h_theory->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t data = h_data->GetBinContent(i_bin);
			Double_t theory = h_theory->GetBinContent(i_bin);
			Double_t ratio = theory / data;

			Double_t RelUnc = h_RelUnc_Theory->GetBinContent(i_bin);
			Double_t AbsUnc_ratio = ratio * RelUnc;

			// -- set values -- //
			h_Ratio->SetBinContent(i_bin, ratio );
			h_Ratio->SetBinError(i_bin, AbsUnc_ratio );
		}

		return h_Ratio;
	}

	void SetBasicOptions_Canvas( TCanvas *c )
	{
		c->Range(0,0,1,1);
		c->SetFillColor(0);
		c->SetBorderMode(0);
		c->SetBorderSize(2);
		//c->SetGridx();
		//c->SetGridy();
		c->SetTickx(1);
		c->SetTicky(1);
		c->SetLeftMargin(0.16);
		c->SetRightMargin(0.02);
		c->SetTopMargin(0.08);
		c->SetBottomMargin(0.14);
		c->SetFrameFillStyle(0);
		c->SetFrameBorderMode(0);
	}

	void SetBasicOptions_Pad( TPad *pad )
	{
		pad->SetFrameFillStyle(0);
		pad->SetFrameBorderMode(0);
		pad->SetFrameFillStyle(0);
		pad->SetFrameBorderMode(0);
	}

	TH1D* MakeHist_DXSecStatOnly( TH1D* h_CV, TH1D* h_RelUnc_Stat )
	{
		TH1D* h_DXSecStatOnly = (TH1D*)h_CV->Clone();
		Int_t nBin = h_DXSecStatOnly->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t DXSec = h_CV->GetBinContent(i_bin);
			Double_t RelUnc_Stat = h_RelUnc_Stat->GetBinContent(i_bin);

			h_DXSecStatOnly->SetBinError( i_bin, DXSec*RelUnc_Stat );
		}

		return h_DXSecStatOnly;
	}

	TH1D* MakeHist_aMCNLO_Electron( TString type )
	{
		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		TString FileName_CV = AnalyzerPath+"/TheoryValues/PDFUnc_aMCNLO/ROOTFile_Hists_ForPDFUnc_XSec_aMCNLO_Electron.root";
		TString FileName_RelUnc = this->ROOTFilePath+"/ROOTFile_TheoryUnc_aMCNLO.root";

		TString HistName_CV = "";
		TString HistName_RelUnc = "";
		if( type == "dressed" )
		{
			HistName_CV = "h_mass_dressed_CV_All";
			HistName_RelUnc = "Electron_dressed/h_RelUnc_Tot";
		}
		else if( type == "FpoF" )
		{
			HistName_CV = "h_mass_withinAcc_CV_All";
			HistName_RelUnc = "Electron_withinAcc/h_RelUnc_Tot";
		}

		TH1D* h_CV = Get_Hist( FileName_CV, HistName_CV );
		h_CV->Scale( 1.0 / Lumi_Elec );
		h_CV = DivideEachBin_ByBinWidth( h_CV ); // -- make diff. x-section -- //
		TH1D* h_RelUnc = Get_Hist( FileName_RelUnc, HistName_RelUnc );

		AssignErrors( h_CV, h_RelUnc );

		return h_CV;
	}

	TH1D* MakeHist_RelLumiUnc( Double_t RelLumiUnc )
	{
		if( this->h_data == NULL )
		{
			cout << "[MakeHist_RelLumiUnc] h_data should be set first (to have same binning)" << endl;
			return NULL;
		}

		TH1D* h_RelLumiUnc = (TH1D*)this->h_data->Clone();
		Int_t nBin = h_RelLumiUnc->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			h_RelLumiUnc->SetBinContent(i_bin, RelLumiUnc );
			h_RelLumiUnc->SetBinError(i_bin, 0);
		}

		return h_RelLumiUnc;
	}

	void AssignTotalError( TH1D* h_CV, TH1D* h_RelUnc_Stat, TH1D* h_RelUnc_Syst, TH1D* h_RelUnc_Lumi )
	{
		Int_t nBin = h_CV->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t RelUnc_Stat = h_RelUnc_Stat->GetBinContent(i_bin);
			Double_t RelUnc_Syst = h_RelUnc_Syst->GetBinContent(i_bin);
			Double_t RelUnc_Lumi = h_RelUnc_Lumi->GetBinContent(i_bin);
			Double_t RelUnc_Tot = sqrt( RelUnc_Stat*RelUnc_Stat + RelUnc_Syst*RelUnc_Syst + RelUnc_Lumi*RelUnc_Lumi );

			Double_t DXSec = h_CV->GetBinContent(i_bin);
			h_CV->SetBinError(i_bin, DXSec*RelUnc_Tot);
		}
	}

  void Draw_RatioZoomIn(TPad *ratioPad, TString canvasName, TString histName, TString legendName )
  {
    TCanvas *c_ratio = new TCanvas(canvasName, "", 800, 800); c_ratio->cd();
    ratioPad->SetPad(0.01,0.01,0.99,0.99);
    ratioPad->SetTopMargin(0.04);
    ratioPad->SetBottomMargin(0.12);
    ratioPad->SetRightMargin(0.03);
    ratioPad->SetLeftMargin(0.12);

    TH1D* h_ratio = (TH1D*)ratioPad->GetPrimitive(histName);
    h_ratio->GetXaxis()->SetTitleOffset(1.2);
    h_ratio->GetXaxis()->SetTitleSize(0.05);
    h_ratio->GetXaxis()->SetLabelSize(0.05);
    h_ratio->GetXaxis()->SetTitle("m [GeV]");
    h_ratio->GetXaxis()->SetNoExponent(); 
    h_ratio->GetXaxis()->SetMoreLogLabels();

    h_ratio->GetYaxis()->SetRangeUser(0, 2.5);
    h_ratio->GetYaxis()->SetTitleSize(0.05);
    h_ratio->GetYaxis()->SetTitleOffset(1);
    h_ratio->GetYaxis()->SetLabelSize(0.05);

    TLegend *legend = (TLegend*)ratioPad->GetPrimitive(legendName);
    legend->SetX1NDC(0.15);
    legend->SetX2NDC(0.75);
    legend->SetY1NDC(0.84);
    legend->SetY2NDC(0.95);

    ratioPad->Draw();

    TLatex latex;
    latex.DrawLatexNDC(0.14, 0.96, "#font[62]{#scale[0.8]{CMS}}");
    
    if( this->Type_Channel == "Muon" )
    {
      latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");
      latex.SetTextSize(0.045);
      latex.DrawLatexNDC(0.17, 0.77 , "#gamma* /#font[122]{Z} #rightarrow #mu#mu");
    }
    else if( this->Type_Channel == "Electron" )
    {
      latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (13 TeV)}}");
      latex.SetTextSize(0.045);
      latex.DrawLatexNDC(0.17, 0.77, "#gamma* /#font[122]{Z} #rightarrow ee");
    }

    c_ratio->SaveAs(".pdf");
  }
};

void Comp_TheoryData_EachChannel()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	DrawingTool *tool = new DrawingTool();
	tool->Set_Channel( "Muon" );
	tool->Draw_Dressed();
	tool->Draw_FpoF();

	DrawingTool *tool_Elec = new DrawingTool();
	tool_Elec->Set_Channel( "Electron" );
	tool_Elec->Draw_Dressed();
	tool_Elec->Draw_FpoF();
}