#include <Include/PlotTools.h>

void AssignUncertainty_ElectronChannel( TH1D* h_DXSec, TH1D* h_RelUnc_Stat, TH1D* h_RelUnc_Syst, Double_t RelUnc_Lumi );
void CreateInputROOTFileForRivet()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString fileName_ll = ROOTFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
	TString fileName_mumu = ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
	TString fileName_mumuFiducial = ROOTFilePath+"/ROOTFile_FpoF_DiffXSec_FullUnc.root";
	// TString fileName_ee = ROOTFilePath+"/DiffXsec_Electron_v8.root";
	// TString fileName_ee = ROOTFilePath+"/DiffXsec_Electron_v9.root"; // -- fiducial cross section is updated
  TString fileName_ee = ROOTFilePath+"/DiffXsec_Electron_v10.root"; // -- small bugs are fixed w.r.t. v9

	cout << "Input file Name for combined result: \n" << fileName_ll << endl;
	cout << "\nInput file Name for muon result: \n" << fileName_mumu << ", \n" << fileName_mumuFiducial << endl;
	cout << "\nInput file Name for electron result: \n" << fileName_ee << endl;

	TH1D* h_DXSec_ll = Get_Hist(fileName_ll, "h1Combi" );
	TH1D* h_DXSec_mumu = Get_Hist(fileName_mumu, "h_DiffXsec_FSRCorr");
	TH1D* h_DXSecFiducial_mumu = Get_Hist(fileName_mumuFiducial, "h_FpoF_DiffXsec_Data");
	TH1D* h_DXSec_ee = Get_Hist(fileName_ee, "h_DiffXSec");
	TH1D* h_DXSecFiducial_ee = Get_Hist(fileName_ee, "h_DiffXSec_Fiducial");

	//////////////////////////////////////////////////////////////
	// -- update the uncertainty of electron channel results -- //
	TH1D* h_RelUnc_Stat = Get_Hist(fileName_ee, "h_RelUnc_Stat");
	TH1D* h_RelUnc_Syst = Get_Hist(fileName_ee, "h_RelUnc_Syst");
	AssignUncertainty_ElectronChannel( h_DXSec_ee, h_RelUnc_Stat, h_RelUnc_Syst, 0.023 );

	TH1D* h_RelUnc_Stat_Fiducial = Get_Hist(fileName_ee, "h_RelUnc_Stat_Fiducial");
	TH1D* h_RelUnc_Syst_Fiducial = Get_Hist(fileName_ee, "h_RelUnc_Syst_Fiducial");
	AssignUncertainty_ElectronChannel( h_DXSecFiducial_ee, h_RelUnc_Stat_Fiducial, h_RelUnc_Syst_Fiducial, 0.023 );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// -- multiply bin width to each bin content because root2yoda automatically divide bin content by bin width :( -- //
	h_DXSec_ll = MultiplyEachBin_byBinWidth( h_DXSec_ll );
	h_DXSec_mumu = MultiplyEachBin_byBinWidth( h_DXSec_mumu );
	h_DXSecFiducial_mumu = MultiplyEachBin_byBinWidth( h_DXSecFiducial_mumu );
	h_DXSec_ee = MultiplyEachBin_byBinWidth( h_DXSec_ee );
	h_DXSecFiducial_ee = MultiplyEachBin_byBinWidth( h_DXSecFiducial_ee );

	// -- 25 Oct.: full phase space result for electron channel & combine results will not be included in Rivet
	// -- reference: https://gitlab.cern.ch/cms-gen/Rivet/merge_requests/127
	TString outputFileName = "ROOTFile_InputForRivet.root";
	TFile *f_output = TFile::Open(outputFileName, "RECREATE");
	f_output->cd();
	// h_DXSec_ll->SetName("h_DXSec_ll");
	// h_DXSec_ll->Write();

	h_DXSec_mumu->SetName("h_DXSec_mumu");
	h_DXSec_mumu->Write();

	h_DXSecFiducial_mumu->SetName("h_DXSecFiducial_mumu");
	h_DXSecFiducial_mumu->Write();

	// h_DXSec_ee->SetName("h_DXSec_ee");
	// h_DXSec_ee->Write();

	h_DXSecFiducial_ee->SetName("h_DXSecFiducial_ee");
	h_DXSecFiducial_ee->Write();

	f_output->Close();

	cout << endl;
	cout << outputFileName << " is created" << endl;
}

void AssignUncertainty_ElectronChannel( TH1D* h_DXSec, TH1D* h_RelUnc_Stat, TH1D* h_RelUnc_Syst, Double_t RelUnc_Lumi )
{
	Int_t nBin = h_DXSec->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t DXSec = h_DXSec->GetBinContent(i_bin);

		Double_t RelUnc_Stat = h_RelUnc_Stat->GetBinContent(i_bin) / 100.0;
		Double_t RelUnc_Syst = h_RelUnc_Syst->GetBinContent(i_bin) / 100.0;
		Double_t RelUnc_Tot = sqrt( RelUnc_Stat*RelUnc_Stat + RelUnc_Syst*RelUnc_Syst + RelUnc_Lumi*RelUnc_Lumi );
		Double_t AbsUnc_Tot = DXSec * RelUnc_Tot;

		h_DXSec->SetBinError(i_bin, AbsUnc_Tot);
	}
}