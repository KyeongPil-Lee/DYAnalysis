void AddTemporaryWWUnc()
{
	TH1::AddDirectory(kFALSE);
	TFile *f_input = TFile::Open("../ROOTFile_Bkg_DataDrivenMethod_Old.root");
	f_input->cd();
	TH1D* WW_stat = (TH1D*)f_input->Get("WW_stat");
	TH1D* WW_systematic = (TH1D*)f_input->Get("WW_systematic");
	f_input->Close();

	TFile *f_output = TFile::Open("ROOTFile_Bkg_DataDrivenMethod.root", "UPDATE"); f_output->cd();
	// WW_stat->Write();
	WW_systematic->Write();
	f_output->Close();
}