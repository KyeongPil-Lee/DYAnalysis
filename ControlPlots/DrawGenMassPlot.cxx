#include <Include/PlotTools.h>

#define nRebin 5
void DrawGenMassPlot()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString fileName = ROOTFilePath+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root";

	vector< Int_t > vec_Color;
	for(Int_t i=1; i<=9; i++)
		vec_Color.push_back( i );
	vec_Color.push_back( kOrange );
	vec_Color.push_back( kViolet );

	vector< TString > vec_SampleType = 
	{
		"M10to50", "M50to100", "M100to200", "M200to400", "M400to500", 
		"M500to700", "M700to800", "M800to1000", "M1000to1500", "M1500to2000", "M2000to3000"
	};

	vector< Double_t > vec_XSec = 
	{
		18610.0/3.0, 5869.58346/3.0, 226/3.0, 7.67/3.0, 0.423/3.0, 
		0.24/3.0, 0.035/3.0, 0.03/3.0, 0.016/3.0, 0.002/3.0, 0.00054/3.0
	};

	vector< Double_t > vec_nEvent = 
	{
		7506956.0, 6061181.0, 227522.0, 170955.0, 50136.0, 47833.0, 44740.0, 43496.0, 40783.0, 37176.0, 23078.0
	};

	Double_t LUMI = 2759.017;

	vector< TH1D* > vec_Hist;
	TLegend *legend = new TLegend(0.55, 0.63, 0.95, 0.95);
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);

	Int_t nSample = (Int_t)vec_SampleType.size();
	for(Int_t i=0; i<nSample; i++)
	{
		TString histName = "h_GenMass_DYMuMu_"+vec_SampleType[i];
		TH1D* h_temp = Get_Hist( fileName, histName );

		h_temp->SetStats(kFALSE);
		h_temp->SetFillColor( vec_Color[i] );
		h_temp->SetLineColor( vec_Color[i] );
		h_temp->SetMarkerColorAlpha( kWhite, 0 );
		Double_t Norm = (LUMI * vec_XSec[i]) / vec_nEvent[i];
		h_temp->Scale( Norm );
		h_temp->Rebin( nRebin );

		vec_Hist.push_back( h_temp );
		legend->AddEntry( h_temp, "DYMuMu_"+vec_SampleType[i] );
	}

	THStack *hs = new THStack("hs", "");
	for(Int_t i_MC=nSample-1; i_MC>=0; i_MC--)
	{
		hs->Add( vec_Hist[i_MC] );
	}

	TString CanvasName = TString::Format("c_GenMass_isHardProcess_%dGeV", nRebin);
	TCanvas *c_hs = new TCanvas(CanvasName, "", 800, 800);
	c_hs->cd();
	gPad->SetLogy();
	gPad->SetLogx();
	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);

	TH1D* h_format = (TH1D*)vec_Hist[0]->Clone();
	h_format->Reset("ICES");
	h_format->GetXaxis()->SetRangeUser(10, 4000);
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

	c_hs->SaveAs(CanvasName+".pdf");

	h_format->GetXaxis()->SetRangeUser(1450, 1550);
	h_format->GetYaxis()->SetRangeUser(5e-3, 2e-2);
	c_hs->SetLogx(kFALSE);
	c_hs->SetLogy(kFALSE);
	c_hs->SaveAs(CanvasName+"_M1450to1550.pdf");
}