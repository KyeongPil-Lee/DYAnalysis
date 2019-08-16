TMatrixD Convert_TH2DtoMatrix()
{
  TH1::AddDirectory(kFALSE);

  TFile* f_input = TFile::Open("cov_ee_ElNov2017rho19_varRhoSystFileAlt2_10000-v3.root");
  f_input->cd();
  TH2D* h_cov = (TH2D*)f_input->Get("h2Cov")->Clone();
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
  TString fileName = "ROOTFile_Input_Covs_v8_20190627.root";
  gSystem->CopyFile("ROOTFile_Input_Covs_v7_20190523.root", fileName, kTRUE);
  TFile *f_input = TFile::Open("ROOTFile_Input_Covs_v8_20190627.root", "UPDATE");
  f_input->cd();
  f_input->Delete("eeCov_varRhoSystFile;1");
  f_input->Close();

  TMatrixD m_cov = Convert_TH2DtoMatrix();

  TFile *f_input2 = TFile::Open("ROOTFile_Input_Covs_v8_20190627.root", "UPDATE");
  f_input2->cd();
  m_cov.Write("eeCov_varRhoSystFile");
  f_input2->Close();
}