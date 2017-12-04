void PrintFinalResults_texFormat( TH1D* h_DiffXSec );
void PrintFinalResults( TH1D* h_DiffXSec );
TString Transform_TexFormat( Double_t value );
Int_t Calc_Exponent( Double_t x );

void Printout_Table_Combined()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TFile *f_input = TFile::Open(ROOTFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root");
	TH1D* h_comb = (TH1D*)f_input->Get("h1Combi")->Clone();

	PrintFinalResults( h_comb );
	PrintFinalResults_texFormat( h_comb );
}

void PrintFinalResults( TH1D* h_DiffXSec )
{
	printf("                     ");
	printf("%15s", "dSigma/dM" );
	// printf("%15s", "Stat.");
	// printf("%15s", "Exp.");
	// printf("%15s", "Theo.");
	printf("%15s", "Total");
	printf("\n");

	Int_t nMassBin = h_DiffXSec->GetNbinsX();
	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t lowerEdge = h_DiffXSec->GetBinLowEdge(i_bin);
		Double_t upperEdge = h_DiffXSec->GetBinLowEdge(i_bin+1);

		printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

		Double_t DiffXsec = h_DiffXSec->GetBinContent(i_bin);

		// Double_t RelUnc_stat = h_RelStatUnc->GetBinContent(i_bin);
		// Double_t unc_stat = (RelUnc_stat * DiffXsec) / 100.0;

		// Double_t RelUnc_Exp_syst = h_RelSysUnc_Tot->GetBinContent(i_bin);
		// Double_t RelUnc_Exp_lumi = h_RelLumiUnc->GetBinContent(i_bin);
		// Double_t RelUnc_Exp = sqrt( RelUnc_Exp_syst*RelUnc_Exp_syst + RelUnc_Exp_lumi*RelUnc_Exp_lumi );
		// Double_t unc_Exp = ( RelUnc_Exp * DiffXsec ) / 100.0;

		// Double_t RelUnc_Theo = 0;
		// Int_t nSource = (Int_t)Sources.size();
		// for(Int_t i_source=0; i_source<nSource; i_source++)
		// {
		// 	if( Sources[i_source].Contains("Acc") )
		// 		RelUnc_Theo = Histos_Unc[i_source]->GetBinContent(i_bin);
		// }

		// Double_t unc_Theo = (RelUnc_Theo * DiffXsec) / 100.0;

		// Double_t RelUnc_Tot = h_RelTotUnc->GetBinContent(i_bin);
		// Double_t unc_total = (RelUnc_Tot * DiffXsec) / 100.0;

		Double_t unc_total = h_DiffXSec->GetBinError(i_bin);

		printf("%15.1e", DiffXsec );
		// printf("%15.1e", unc_stat );
		// printf("%15.1e", unc_Exp );
		// printf("%15.1e", unc_Theo );
		printf("%15.1e", unc_total );

		// printf("%15.2e", DiffXsec );
		// printf("%15.2lf", RelUnc_stat );
		// printf("%15.2lf", RelUnc_Exp );
		// printf("%15.2lf", RelUnc_Theo );
		// printf("%15.2lf", RelUnc_Tot );

		printf("\n");
	}
}

void PrintFinalResults_texFormat( TH1D* h_DiffXSec )
{
	cout << "===========================================================================" << endl;
	printf("                     ");
	printf("%15s", "dSigma/dM" );
	printf("%15s", "Total");
	printf("\n");
	
	Int_t nMassBin = h_DiffXSec->GetNbinsX();
	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t lowerEdge = h_DiffXSec->GetBinLowEdge(i_bin);
		Double_t upperEdge = h_DiffXSec->GetBinLowEdge(i_bin+1);

		TString str_result = TString::Format("%.0d-%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);

		// -- central value -- //
		Double_t DiffXsec = h_DiffXSec->GetBinContent(i_bin);

		// // -- stat. -- //
		// Double_t RelUnc_stat = h_RelStatUnc->GetBinContent(i_bin);
		// Double_t unc_stat = (RelUnc_stat * DiffXsec) / 100.0;

		// // -- Exp. -- //
		// Double_t RelUnc_Exp_syst = h_RelSysUnc_Tot->GetBinContent(i_bin);
		// Double_t RelUnc_Exp_lumi = h_RelLumiUnc->GetBinContent(i_bin);
		// Double_t RelUnc_Exp = sqrt( RelUnc_Exp_syst*RelUnc_Exp_syst + RelUnc_Exp_lumi*RelUnc_Exp_lumi );
		// Double_t unc_Exp = ( RelUnc_Exp * DiffXsec ) / 100.0;

		// // -- theory -- //
		// Double_t RelUnc_Theo = 0;
		// Int_t nSource = (Int_t)Sources.size();
		// for(Int_t i_source=0; i_source<nSource; i_source++)
		// {
		// 	if( Sources[i_source].Contains("Acc") )
		// 		RelUnc_Theo = Histos_Unc[i_source]->GetBinContent(i_bin);
		// }

		// Double_t unc_Theo = (RelUnc_Theo * DiffXsec) / 100.0;

		// -- all -- //
		// Double_t RelUnc_Tot = h_RelTotUnc->GetBinContent(i_bin);
		// Double_t unc_total = (RelUnc_Tot * DiffXsec) / 100.0;
		Double_t unc_total = h_DiffXSec->GetBinError(i_bin);

		str_result = str_result + Transform_TexFormat( DiffXsec );
		// str_result = str_result + Transform_TexFormat( unc_stat );
		// str_result = str_result + Transform_TexFormat( unc_Exp );
		// str_result = str_result + Transform_TexFormat( unc_Theo );
		str_result = str_result + Transform_TexFormat( unc_total );
		str_result = str_result + "\\\\";

		printf( "%s\n\\hline\n", str_result.Data() );
	}
	cout << "===========================================================================" << endl;
}

TString Transform_TexFormat( Double_t value )
{
	Int_t Exponent = Calc_Exponent( value );
	Double_t front = value / TMath::Power(10, Exponent);

	TString str = TString::Format(" & $  %.1lf \\times 10^{%d}  $", front, Exponent);
	return str;
}

Int_t Calc_Exponent( Double_t x )
{
	if( x >= 1 )
		return (Int_t)TMath::Log10(x);

	else if( x > 0 && x < 1 )
		return (Int_t)TMath::Log10(x) - 1;
	
	else
	{
		cout << "Can not calculate the exponent of negative value!";
		return -999;
	}
}