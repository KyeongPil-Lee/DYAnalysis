#include <Include/PlotTools.h>

TH1D* MakeHist_StatUnc( TH1D* h )
{
	TH1D *h_RelStatUnc = (TH1D*)h->Clone();
	Int_t nBin = h_RelStatUnc->GetNbinsX();

	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t nEvent = h->GetBinContent(i_bin);
		Double_t RelStatUnc = 1. / sqrt(nEvent);

		h_RelStatUnc->SetBinContent(i_bin, RelStatUnc);
		h_RelStatUnc->SetBinError(i_bin, 0);
	}

	return h_RelStatUnc;
}

void Calc_StatUnc( TString Flavor )
{
	TFile *f_output = TFile::Open("ROOTFile_RelStatUnc_2016and17_"+Flavor+".root", "RECREATE");

	TString FileName = "ROOTFile_Histogram_1D2D_"+Flavor+".root";
	vector< TString > vec_HistName =
	{
		"h_mass_DY",
		"h_absdiRap_M20to30_DY",
		"h_absdiRap_M30to45_DY",
		"h_absdiRap_M45to60_DY",
		"h_absdiRap_M60to120_DY",
		"h_absdiRap_M120to200_DY",
		"h_absdiRap_M200to1500_DY",
	};

	for(const auto& HistName : vec_HistName )
	{
		TH1D* h = Get_Hist( FileName, HistName );
		TH1D* h_StatUnc = MakeHist_StatUnc( h );

		TString HistName_StatUnc = h->GetName();
		HistName_StatUnc.ReplaceAll( "h_", "h_RelStatUnc_");
		h_StatUnc->SetName( HistName_StatUnc );

		f_output->cd();
		h_StatUnc->Write();
	}
}

void Calc_StatUnc()
{
	Calc_StatUnc( "Electron" );
	Calc_StatUnc( "Muon" );
}