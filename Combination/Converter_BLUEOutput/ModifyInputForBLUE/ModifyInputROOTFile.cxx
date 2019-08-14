#include <ElecChannel/Include/DYElectronTool.h>

TMatrixD Convert_TH2DtoMatrix(TString histName)
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString covFilePath = analyzerPath+"/ElecChannel/CovMatrix/EfficiencySF/ROOTFile_CovM_EfficiencySF.root";

  TFile* f_input = TFile::Open(covFilePath);
  f_input->cd();
  TH2D* h_cov = (TH2D*)f_input->Get(histName)->Clone();
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

TH1D* GetDXSec_ee()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName = analyzerPath+"/ElecChannel/DXSec/ROOTFile_dXSec.root";

  return PlotTool::Get_Hist(fileName, "h_dXSec");
}

TMatrixD* GetCov_SFApplied(TString fileName, TString covName, TH1D* h_dXSec_new)
{
  TH1D* h_dXSec_old = PlotTool::Get_Hist(fileName, "h_DiffXSec_ee");
  TH1D* h_SF = (TH1D*)h_dXSec_old->Clone();
  h_SF->Divide( h_dXSec_new, h_dXSec_old );

  TFile *f_input = TFile::Open(fileName);
  f_input->cd();
  TMatrixD* m_cov = (TMatrixD*)f_input->Get(covName)->Clone();

  for(Int_t i=0; i<43; i++)
  {
    Int_t i_bin = i+1;
    Double_t SF_i = h_SF->GetBinContent(i_bin);

    for(Int_t j=0; j<43; j++)
    {
      Int_t j_bin = j+1;
      Double_t SF_j = h_SF->GetBinContent(j_bin);

      Double_t cov_ij = (*m_cov)[i][j];

      Double_t cov_SFApplied_ij = cov_ij * SF_i * SF_j;

      (*m_cov)[i][j] = cov_SFApplied_ij;
    }
  }

  return m_cov;
}

void ModifyInputROOTFile()
{
  TH1::AddDirectory(kFALSE);

  TString fileName_ref = "/Users/KyeongPil_Lee/Physics/DYCodeForComb/DYee2015_newEffSF/Covariance/ROOTFile_Input_Covs_v7_20190523.root";
  // TString fileName_new = "./ROOTFile_Input_Covs_v20190809_myDXSecEE_myCovEffSF_SFAppliedToCov.root";
  TString fileName_new = "./ROOTFile_Input_Covs_v20190812_newEffSF.root";
  gSystem->CopyFile(fileName_ref, fileName_new, kTRUE);


  // -- new diff. x-section
  TH1D* h_dXSec_ee = GetDXSec_ee();

  // -- efficiency SF
  TMatrixD m_cov_effStat = Convert_TH2DtoMatrix("h_cov_stat");
  TMatrixD m_cov_effSyst = Convert_TH2DtoMatrix("h_cov_syst");

  // -- cov. w/ SF applied
  TMatrixD* m_cov_stat    = GetCov_SFApplied( fileName_ref, "eeCov_varYield",      h_dXSec_ee );
  TMatrixD* m_cov_bkgStat = GetCov_SFApplied( fileName_ref, "eeCov_varBkg",        h_dXSec_ee );
  TMatrixD* m_cov_detStat = GetCov_SFApplied( fileName_ref, "eeCov_varDetRes",     h_dXSec_ee );
  TMatrixD* m_cov_varEff  = GetCov_SFApplied( fileName_ref, "eeCov_varEff",        h_dXSec_ee );
  TMatrixD* m_cov_varAcc  = GetCov_SFApplied( fileName_ref, "eeCov_varAcc",        h_dXSec_ee );
  TMatrixD* m_cov_bkgSyst = GetCov_SFApplied( fileName_ref, "eeCov_varBkgSyst",    h_dXSec_ee );
  TMatrixD* m_cov_detSyst = GetCov_SFApplied( fileName_ref, "eeCov_varDetResSyst", h_dXSec_ee );
  TMatrixD* m_cov_FSRSyst = GetCov_SFApplied( fileName_ref, "eeCov_varFSRResSyst", h_dXSec_ee );
  TMatrixD* m_cov_accSyst = GetCov_SFApplied( fileName_ref, "eeCov_varAccSyst",    h_dXSec_ee );

  // -- cov. for FSR stat.: from the other root file
  TString fileName_FSRStat = "/Users/KyeongPil_Lee/Physics/DYCodeForComb/DYee2015_newEffSF/Covariance/ROOTFile_Input_Covs_v6_HepData20190205.root";
  TMatrixD* m_cov_FSRStat = GetCov_SFApplied( fileName_FSRStat, "eeCov_varFSRRes", h_dXSec_ee ); // -- different SF with the others


  DYTool::ReplaceObject( fileName_new, "h_DiffXSec_ee",        h_dXSec_ee );
  DYTool::ReplaceObject( fileName_new, "eeCov_varRhoFile",     &m_cov_effStat );
  DYTool::ReplaceObject( fileName_new, "eeCov_varRhoSystFile", &m_cov_effSyst );

  DYTool::ReplaceObject( fileName_new, "eeCov_varYield",      m_cov_stat );
  DYTool::ReplaceObject( fileName_new, "eeCov_varBkg",        m_cov_bkgStat );
  DYTool::ReplaceObject( fileName_new, "eeCov_varDetRes",     m_cov_detStat );
  DYTool::ReplaceObject( fileName_new, "eeCov_varEff",        m_cov_varEff );
  DYTool::ReplaceObject( fileName_new, "eeCov_varAcc",        m_cov_varAcc );
  DYTool::ReplaceObject( fileName_new, "eeCov_varBkgSyst",    m_cov_bkgSyst );
  DYTool::ReplaceObject( fileName_new, "eeCov_varDetResSyst", m_cov_detSyst );
  DYTool::ReplaceObject( fileName_new, "eeCov_varFSRResSyst", m_cov_FSRSyst );
  DYTool::ReplaceObject( fileName_new, "eeCov_varAccSyst",    m_cov_accSyst );
  DYTool::ReplaceObject( fileName_new, "eeCov_varFSRRes",     m_cov_FSRStat );
}