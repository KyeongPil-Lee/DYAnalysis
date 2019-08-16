TMatrixD Convert_TH2DtoMatrix()
{
  TH1::AddDirectory(kFALSE);

  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString covFilePath = analyzerPath+"/ElecChannel/CovMatrix/EfficiencySF/ROOTFile_CovM_EfficiencySF.root";

  TFile* f_input = TFile::Open(covFilePath);
  f_input->cd();
  TH2D* h_cov = (TH2D*)f_input->Get("h_cov_syst")->Clone();
  f_input->Close();

  TMatrixD m_cov(43, 43);

  for(Int_t i_x=0; i_x<43; i_x++)
  {
    Int_t i_binX = i_x + 1;
    for(Int_t i_y=0; i_y<43; i_y++)
    {
      Int_t i_binY = i_y + 1;

      Double_t cov = h_cov->GetBinContent(i_binX, i_binY);
      m_cov[i_x][i_y] = cov;
    }
  }

  return m_cov;
}

void ReplaceCov()
{
  TString fileName_ref = "/Users/KyeongPil_Lee/Physics/DYCodeForComb/DYee2015_newEffSF/Covariance/ROOTFile_Input_Covs_v7_20190523.root";
  TString fileName_new = "./ROOTFile_Input_Covs_v9_20190718_myCov_effSFSyst.root";

  gSystem->CopyFile(fileName_ref, fileName_new, kTRUE);

  TFile *f_input = TFile::Open(fileName_new, "UPDATE");
  f_input->cd();
  f_input->Delete("eeCov_varRhoSystFile;1");
  f_input->Close();

  TMatrixD m_cov = Convert_TH2DtoMatrix();

  TFile *f_input2 = TFile::Open(fileName_new, "UPDATE");
  f_input2->cd();
  m_cov.Write("eeCov_varRhoSystFile");
  f_input2->Close();
}