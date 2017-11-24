// #include </Users/KyeongPil_Lee/Physics/Include/PlotTools.h>
#include <Include/PlotTools.h>

TString FileName_output = "ROOTFile_Input_Cov.root";

TString HistName_DiffXSec = "h_DiffXSec";
TString HistName_StatUnc = "h_RelStatUnc";

TString HistName_RelUnc_Stat_BkgEst = "h_RelUnc_Stat_BkgEst";
TString HistName_RelUnc_Syst_BkgEst = "h_RelUnc_Syst_BkgEst";

TString HistName_RelUnc_Stat_DetRes = "h_RelUnc_Stat_DetRes";
TString HistName_RelUnc_Syst_DetRes = "h_RelUnc_Syst_DetRes";

TString HistName_RelUnc_Stat_EffSF = "h_RelUnc_Stat_EffSF";
TString HistName_RelUnc_Syst_EffSF = "h_RelUnc_Syst_EffSF";

TString HistName_RelUnc_Stat_FSR = "h_RelUnc_Stat_FSR";
TString HistName_RelUnc_Syst_FSR = "h_RelUnc_Syst_FSR";

TString HistName_RelUnc_Syst_Acc = "h_RelUnc_Syst_Acc";

void MakeInputFile_Cov()
{
	cout << "This code should be run after ROOTFile_DiffXSec_FullUnc.root is updated" << endl;
	// TString BasePath = "/cms/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20170106_1st_UpdateSyst_fromApproval";
	TString BasePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TH1D* h_DiffXSec = Get_Hist( BasePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_DiffXsec_FSRCorr", HistName_DiffXSec );
	TH1D* h_RelStatUnc = Get_Hist( BasePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_RelStatUnc_Percent", HistName_StatUnc );
	h_RelStatUnc->SetYTitle("");
	h_RelStatUnc->Scale( 0.01 );

	TH1D* h_RelUnc_Stat_BkgEst = Get_Hist( BasePath+"/ROOTFile_SysUnc_BkgEst.root", "h_RelUnc_Stat_Total", HistName_RelUnc_Stat_BkgEst );
	TH1D* h_RelUnc_Syst_BkgEst = Get_Hist( BasePath+"/ROOTFile_SysUnc_BkgEst.root", "h_RelUnc_Syst_Total", HistName_RelUnc_Syst_BkgEst );
	h_RelUnc_Stat_BkgEst->Scale( 0.01 ); h_RelUnc_Stat_BkgEst->SetYTitle("");
	h_RelUnc_Syst_BkgEst->Scale( 0.01 ); h_RelUnc_Syst_BkgEst->SetYTitle("");

	TH1D* h_RelUnc_Stat_DetRes = Get_Hist( BasePath+"/ROOTFile_SysUnc_DetRes.root", "h_RelUnc_Stat", HistName_RelUnc_Stat_DetRes );
	TH1D* h_RelUnc_Syst_DetRes = Get_Hist( BasePath+"/ROOTFile_SysUnc_DetRes.root", "h_RelUnc_Syst", HistName_RelUnc_Syst_DetRes );
	h_RelUnc_Stat_DetRes->Scale( 0.01 ); h_RelUnc_Stat_DetRes->SetYTitle("");
	h_RelUnc_Syst_DetRes->Scale( 0.01 ); h_RelUnc_Syst_DetRes->SetYTitle("");

	TH1D* h_RelUnc_Stat_EffSF = Get_Hist( BasePath+"/ROOTFile_SysUnc_EffSF.root", "h_RelUnc_Stat", HistName_RelUnc_Stat_EffSF );
	TH1D* h_RelUnc_Syst_EffSF = Get_Hist( BasePath+"/ROOTFile_SysUnc_EffSF.root", "h_RelUnc_Syst", HistName_RelUnc_Syst_EffSF );
	h_RelUnc_Stat_EffSF->Scale( 0.01 ); h_RelUnc_Stat_EffSF->SetYTitle("");
	h_RelUnc_Syst_EffSF->Scale( 0.01 ); h_RelUnc_Syst_EffSF->SetYTitle("");

	TH1D* h_RelUnc_Stat_FSR = Get_Hist( BasePath+"/ROOTFile_RelUnc_Stat_Syst_Tot_FSR.root", "h_RelUnc_Stat", HistName_RelUnc_Stat_FSR );
	TH1D* h_RelUnc_Syst_FSR = Get_Hist( BasePath+"/ROOTFile_RelUnc_Stat_Syst_Tot_FSR.root", "h_RelUnc_Syst", HistName_RelUnc_Syst_FSR );
	h_RelUnc_Stat_FSR->Scale( 0.01 ); h_RelUnc_Stat_FSR->SetYTitle("");
	h_RelUnc_Syst_FSR->Scale( 0.01 ); h_RelUnc_Syst_FSR->SetYTitle(""); 

	TH1D* h_RelUnc_Syst_Acc = Get_Hist( BasePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Acc._Percent", HistName_RelUnc_Syst_Acc );
	h_RelUnc_Syst_Acc->Scale( 0.01 ); h_RelUnc_Syst_Acc->SetYTitle("");

	TFile *f_output = TFile::Open(FileName_output, "RECREATE");
	f_output->cd();

	h_DiffXSec->Write();
	h_RelStatUnc->Write();

	h_RelUnc_Stat_BkgEst->Write();
	h_RelUnc_Syst_BkgEst->Write();

	h_RelUnc_Stat_DetRes->Write();
	h_RelUnc_Syst_DetRes->Write();

	h_RelUnc_Stat_EffSF->Write();
	h_RelUnc_Syst_EffSF->Write();

	h_RelUnc_Stat_FSR->Write();
	h_RelUnc_Syst_FSR->Write();

	h_RelUnc_Syst_Acc->Write();
}
