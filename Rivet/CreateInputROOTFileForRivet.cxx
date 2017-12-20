#include <Include/PlotTools.h>

void CreateInputROOTFileForRivet()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString fileName_ll = ROOTFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
	TString fileName_mumu = ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
	TString fileName_mumuFiducial = ROOTFilePath+"/ROOTFile_FpoF_DiffXSec_FullUnc.root";
	TString fileName_ee = ROOTFilePath+"/DiffXsec_Electron_v8.root";

	cout << "Input file Name for combined result: \n" << fileName_ll << endl;
	cout << "\nInput file Name for muon result: \n" << fileName_mumu << ", \n" << fileName_mumuFiducial << endl;
	cout << "\nInput file Name for electron result: \n" << fileName_ee << endl;

	TH1D* h_DXSec_ll = Get_Hist(fileName_ll, "h1Combi" );
	TH1D* h_DXSec_mumu = Get_Hist(fileName_mumu, "h_DiffXsec_FSRCorr");
	TH1D* h_DXSecFiducial_mumu = Get_Hist(fileName_mumuFiducial, "h_FpoF_DiffXsec_Data");
	TH1D* h_DXSec_ee = Get_Hist(fileName_ee, "h_DiffXSec");
	TH1D* h_DXSecFiducial_ee = Get_Hist(fileName_ee, "h_DiffXSec_Fiducial");


	TString outputFileName = "ROOTFile_InputForRivet.root";
	TFile *f_output = TFile::Open(outputFileName, "RECREATE");
	f_output->cd();
	h_DXSec_ll->SetName("h_DXSec_ll");
	h_DXSec_ll->Write();

	h_DXSec_mumu->SetName("h_DXSec_mumu");
	h_DXSec_mumu->Write();

	h_DXSecFiducial_mumu->SetName("h_DXSecFiducial_mumu");
	h_DXSecFiducial_mumu->Write();

	h_DXSec_ee->SetName("h_DXSec_ee");
	h_DXSec_ee->Write();

	h_DXSecFiducial_ee->SetName("h_DXSecFiducial_ee");
	h_DXSecFiducial_ee->Write();

	f_output->Close();

	cout << endl;
	cout << outputFileName << " is created" << endl;
}