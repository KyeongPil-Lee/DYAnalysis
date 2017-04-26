#include <Include/PlotTools.h>

void Latex_Info( TLatex &latex, TString Type, TString region );

Double_t CustomizedBkg(Double_t *x, Double_t* par)
{
	Double_t logx = TMath::Log(x[0]);
	Double_t arg1 = par[1];
	Double_t arg2 = par[2]*logx;
	// Double_t arg3 = par[3]*logx*logx;
	// Double_t arg4 = par[4]*logx*logx*logx;

	// return par[0]*TMath::Power(x[0], arg1 + arg2 + arg3 + arg4);
	// return par[0]*TMath::Power(x[0], arg1 + arg2 + arg3);
	return par[0]*TMath::Power(x[0], arg1 + arg2);
}

// Double_t FitFunc(Double_t* x, Double_t* par)
// {
// 	Double_t a = par[0];
// 	Double_t b = par[1];

// 	return TMath::Exp(a + b*x[0]);
// }

Double_t FitFunc(Double_t* x, Double_t* par)
{
	Double_t a = par[0];
	Double_t b = par[1];
	Double_t c = par[2];

	return TMath::Exp(a + b*x[0] + c*x[0]*x[0]);
}

void Fit_DataMCRatio()
{
	gStyle->SetOptFit(111);

	TString InputName = "./ROOTFile_DataMC_Ratio.root";
	vector< TString	> vec_Region;
	vec_Region.push_back( "All" );
	vec_Region.push_back( "BB" );
	vec_Region.push_back( "BEEE" );

	TFile *f_output = TFile::Open("ROOTFile_FitResults.root", "RECREATE");
	for( const auto &Region : vec_Region )
	{
		TGraphAsymmErrors *g = Get_Graph( InputName, "g_ratio_"+Region );

		////////////////////
		// -- Fitting -- //
		///////////////////
		TF1 *func = new TF1("CustomizedBkg", CustomizedBkg, 60, 2500, 3);
		func->SetParameter(0, 1); func->SetParName(0, "p0");
		func->SetParameter(1, -0.001); func->SetParName(1, "p1");
		func->SetParameter(2, 0.001); func->SetParName(2, "p2");
		// func->SetParameter(3, -0.001); func->SetParName(3, "p3");

		if( Region == "BB" )
		{
			func->SetParameter(0, 0.1);
			func->SetParameter(1, -0.03);
			func->FixParameter(2, 0);
		}

		g->Fit( "CustomizedBkg", "R", "", 60, 2500);

		TCanvas *c;
		SetCanvas_Square( c, "c_Fit_"+Region );
		c->cd();
		g->Draw("APSAME");
		g->GetYaxis()->SetRangeUser(0.9, 1.1);

		SetGraphFormat_SinglePad( g, "m [GeV]", "data/MC" );
		TLatex latex;
		Latex_Info( latex, "", Region );

		c->SaveAs(".pdf");

		f_output->cd();
		c->Write();
		g->Write("g_ratio_"+Region);
	}

	f_output->Close();
}

void Latex_Info( TLatex &latex, TString Type, TString region )
{
	TString TStr_Type = "";
	if( Type.Contains("DEN") ) TStr_Type = "Denominator";
	else if( Type == "" ) TStr_Type = "";
	else TStr_Type = "Numerator";

	TString TStr_Region = "";
	if( region == "BB" ) TStr_Region = "BB";
	if( region == "BEEE" ) TStr_Region = "BE+EE";
	if( region == "All" ) TStr_Region = "All";

	latex.DrawLatexNDC(0.15, 0.91, "#font[42]{#scale[0.8]{"+TStr_Region+"}}");
	latex.DrawLatexNDC(0.15, 0.88, "#font[42]{#scale[0.6]{"+TStr_Type+"}}");
}