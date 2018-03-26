#include <Include/PlotTools.h>
#include <Include/DYAnalyzer.h>

class DrawingTool
{
public:
	TH1D* h_preFSR_normToNLO;
	TH1D* h_preFSR_normToNNLO;

	TH1D* h_postFSR_normToNLO;
	TH1D* h_postFSR_normToNNLO;

	DrawingTool()
	{
		this->Init();
	}

	void DrawAll()
	{
		this->Draw();
		// this->DrawHStack( 1, "aMCNLO_NNLOxSec" );
		// this->DrawHStack( 1, "aMCNLO_AdditionalSF" );
		this->DrawHStack( 1, "aMCNLO_NLO_XSDB" );
		// this->DrawHStack( 5, "aMCNLO_NNLOxSec" );
		// this->DrawHStack( 5, "aMCNLO_AdditionalSF" );
		this->DrawHStack( 5, "aMCNLO_NLO_XSDB" );
	}

	void Draw()
	{
		HistInfo* hist_NLO = new HistInfo(kBlack, "Normalized with #sigma(NLO)", this->h_preFSR_normToNLO );
		HistInfo* hist_NNLO = new HistInfo(kRed, "Normalized with #sigma(NNLO)", this->h_preFSR_normToNNLO );
		hist_NNLO->CalcRatio_DEN( hist_NLO->h );

		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString canvasName = "c_preFSR_NLOxSec_vs_NNLOxSec";
		SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 1, 1 );
		c->cd();
		TopPad->cd();
		hist_NLO->Draw("EPSAME");
		hist_NNLO->Draw("EPSAME");
		SetHistFormat_TopPad( hist_NLO->h, "# events" );
		hist_NLO->h->GetYaxis()->SetRangeUser(1e-3, 1e8);
		hist_NLO->h->GetXaxis()->SetRangeUser(50, 3000);
		hist_NNLO->h->GetXaxis()->SetRangeUser(50, 3000);

		TLegend *legend;
		SetLegend( legend, 0.50, 0.75, 0.95, 0.95 );
		hist_NLO->AddToLegend( legend );
		hist_NNLO->AddToLegend( legend );
		legend->Draw();

		TLatex latex;
		latex.DrawLatexNDC(0.16, 0.91, "#scale[0.6]{#font[42]{pre-FSR (isHardProcess)}}");

		c->cd();
		BottomPad->cd();
		hist_NNLO->DrawRatio("EPSAME");
		SetHistFormat_BottomPad( hist_NNLO->h_ratio, "m [GeV]", "NNLO/NLO", 0.92, 1.08 );
		hist_NNLO->h_ratio->GetXaxis()->SetRangeUser(50, 3000);

		TF1 *f_line;
		DrawLine(f_line);

		c->SaveAs(".pdf");
	}

	void DrawHStack( Int_t nRebin, TString type )
	{
		vector< TString > vec_ntupleDir;
		vector< TString > vec_tag;
		vector< Double_t > vec_xSec;
		vector< Double_t > vec_nEvent;

		vector< Int_t > vec_Color;
		for(Int_t i=1; i<=9; i++)
			vec_Color.push_back( i );
		vec_Color.push_back( kOrange );
		vec_Color.push_back( kViolet );

		DYAnalyzer *analyzer = new DYAnalyzer( "None" );
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2( type, &vec_ntupleDir, &vec_tag, &vec_xSec, &vec_nEvent );

		vector< TH1D* > vec_hist;

		TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileName = rootFilePath+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root";
		TLegend *legend = new TLegend(0.55, 0.63, 0.95, 0.95);
		legend->SetBorderSize(0);
		legend->SetFillStyle(0);

		// Int_t nRebin = 1;

		Int_t nSample = (Int_t)vec_tag.size();
		for(Int_t i=0; i<nSample; i++)
		{
			TString histName = "h_GenMass_"+vec_tag[i];
			TH1D* h_temp = Get_Hist( fileName, histName );

			h_temp->SetStats(kFALSE);
			h_temp->SetFillColor( vec_Color[i] );
			h_temp->SetLineColor( vec_Color[i] );
			h_temp->SetMarkerColorAlpha( kWhite, 0 );
			Double_t Norm = (Lumi * vec_xSec[i]) / vec_nEvent[i];
			h_temp->Scale( Norm );
			h_temp->Rebin( nRebin );
			h_temp->Sumw2();
			vec_hist.push_back( h_temp );
			legend->AddEntry( h_temp, vec_tag[i] );
		}

		THStack *hs = new THStack("hs", "");
		for(Int_t i_MC=nSample-1; i_MC>=0; i_MC--)
		{
			hs->Add( vec_hist[i_MC] );
		}

		TString canvasName = TString::Format("c_GenMass_isHardProcess_%dGeV_%s", nRebin, type.Data());
		TCanvas *c_hs = new TCanvas(canvasName, "", 800, 800);
		c_hs->cd();
		gPad->SetLogy();
		gPad->SetLogx();
		gPad->SetTopMargin(0.03);
		gPad->SetRightMargin(0.05);

		TH1D* h_format = (TH1D*)vec_hist[0]->Clone();
		h_format->Reset("ICES");
		h_format->GetXaxis()->SetRangeUser(49, 4000);
		h_format->GetYaxis()->SetRangeUser(1e-4, 1e8);
		h_format->GetXaxis()->SetNoExponent();
		h_format->GetXaxis()->SetMoreLogLabels();
		h_format->GetXaxis()->SetTitle("m(#mu#mu) (isHardProcess) [GeV]");
		h_format->GetYaxis()->SetTitle(TString::Format("# Events / %d GeV", nRebin));
		h_format->GetYaxis()->SetTitleOffset(1.4);

		h_format->Draw("");
		hs->Draw("histsame");
		h_format->Draw("axissame");
		legend->Draw();

		TLatex latex;
		TString xSecInfo = "";
		if( type == "aMCNLO_AdditionalSF" ) xSecInfo = "Normalized to #sigma(NLO)";
		if( type == "aMCNLO_NNLOxSec" ) xSecInfo = "Normalized to #sigma(NNLO)";
		latex.DrawLatexNDC(0.14, 0.93, "#font[42]{#scale[0.6]{"+xSecInfo+"}}");

		c_hs->SaveAs(canvasName+".pdf");

		TH1D* h_full = (TH1D*)vec_hist[0]->Clone();
		for(Int_t i=1; i<(Int_t)vec_hist.size(); i++)
			h_full->Add( vec_hist[i] );

		this->PrintAllCriteria( canvasName, h_format, h_full, c_hs );
	}

private:
	void PrintAllCriteria(TString &canvasName, TH1D* h_format, TH1D* h_full, TCanvas* c_hs )
	{
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 30, 70 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 80, 120 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 180, 220 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 380, 420 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 480, 520 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 680, 720 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 780, 820 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 980, 1020 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 1480, 1520 );
		this->PrintZoominPlot( canvasName, h_format, h_full, c_hs, 1980, 2020 );
	}

	void PrintZoominPlot( TString &canvasName, TH1D* h_format, TH1D* h_full, TCanvas* c_hs, Double_t lowerEdge, Double_t upperEdge )
	{
		h_format->GetXaxis()->SetRangeUser(lowerEdge, upperEdge);

		Double_t yMin = 0.5*this->FindBinContent(h_full, upperEdge);
		Double_t yMax = 2*this->FindBinContent(h_full, lowerEdge);
		// cout << "yMin: " << yMin << ", yMax: " << yMax << endl;
		h_format->GetYaxis()->SetRangeUser(yMin, yMax);

		c_hs->SetLogx(kFALSE);
		c_hs->SetLogy(kFALSE);
		c_hs->SaveAs(canvasName+TString::Format("_M%.0lfto%.0lf.pdf", lowerEdge, upperEdge));
	}

	Double_t FindBinContent( TH1D* h, Double_t edge )
	{
		Double_t binContent = -999;

		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			// cout << "h->GetBinLowEdge(i_bin): " << h->GetBinLowEdge(i_bin) << ", edge: " << edge << endl;
			if( h->GetBinLowEdge(i_bin) == edge )
			{
				binContent = h->GetBinContent(i_bin);
				// cout << "binContent: " << binContent << endl;
				break;
			}
		}

		if( binContent == -999 )
			cout << "no bin edge was found! ... -999 will be returned";

		return binContent;
	}
	void Init()
	{
		this->h_preFSR_normToNLO = this->MakeHist_preFSR( "aMCNLO_NLO_XSDB" );
		this->h_preFSR_normToNNLO = this->MakeHist_preFSR( "aMCNLO_NNLOxSec" );

		Int_t nRebin = 50;
 		this->h_preFSR_normToNLO->Rebin( nRebin );
		this->h_preFSR_normToNNLO->Rebin( nRebin );
	}

	TH1D* MakeHist_preFSR( TString type )
	{
		vector<TH1D*> vec_hist = this->CollectHist_preFSR( type );
		TH1D* h_full = (TH1D*)vec_hist[0]->Clone();
		for(Int_t i=1; i<(Int_t)vec_hist.size(); i++)
			h_full->Add( vec_hist[i] );

		return h_full;
	}

	vector<TH1D*> CollectHist_preFSR( TString type )
	{
		vector< TString > vec_ntupleDir;
		vector< TString > vec_tag;
		vector< Double_t > vec_xSec;
		vector< Double_t > vec_nEvent;

		DYAnalyzer *analyzer = new DYAnalyzer( "None" );
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2( type, &vec_ntupleDir, &vec_tag, &vec_xSec, &vec_nEvent );

		vector< TH1D* > vec_hist;

		TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileName = rootFilePath+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root";
		Int_t nSample = (Int_t)vec_tag.size();
		for(Int_t i=0; i<nSample; i++)
		{
			TString histName = "h_GenMass_"+vec_tag[i];
			TH1D* h_temp = Get_Hist( fileName, histName );

			h_temp->SetStats(kFALSE);
			Double_t Norm = (Lumi * vec_xSec[i]) / vec_nEvent[i];
			h_temp->Scale( Norm );
			h_temp->Sumw2();
			vec_hist.push_back( h_temp );
		}

		return vec_hist;
	}
};

void ComparisonMassDist_NLO_NNLO()
{
	DrawingTool* tool = new DrawingTool();
	tool->DrawAll();
}