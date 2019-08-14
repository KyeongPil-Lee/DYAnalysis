// -- based on the method used in Z+jets differential cross section measurement
// -- reference: Section 8 of AN2013_049_v9

#include <Include/DYAnalyzer.h> // -- has nMassBin
#include <Include/SimplePlotTools.h>

class CombinationTool
{
public:
  // -- inputs from individual channels
  TH1D* h_dXSec_ee_;
  TH1D* h_dXSec_mm_;
  TMatrixD m_dXSec_eemm_;

  TH1D* h_absUnc_tot_ee_;
  TH1D* h_absUnc_tot_mm_;

  TH2D* h_cov_statYield_ee_;
  TH2D* h_cov_statNonYield_ee_;
  TH2D* h_cov_systAcc_ee_;
  TH2D* h_cov_systNonAcc_ee_;

  TH2D* h_cov_statYield_mm_;
  TH2D* h_cov_statNonYield_mm_;
  TH2D* h_cov_systAcc_mm_;
  TH2D* h_cov_systNonAcc_mm_;

  TH2D* h_covEM_noLumi_; // -- electron-muon channel covariance matrix. Only acceptance uncertainty is considered as correlated (as lumi. unc. is not included in this matrix)

  Double_t relUnc_lumi_; // -- used for the cov. M for the luminoisty uncertainty

  TMatrixD m_lambda_; // -- coefficient

  TMatrixD m_dXSec_ll_;
  TH1D* h_dXSec_ll_;
  TH1D* h_absUnc_stat_ll_;
  TH1D* h_relUnc_stat_ll_;
  TH1D* h_absUnc_tot_ll_;
  TH1D* h_relUnc_tot_ll_;


  // -- 2N*2N cov matrices
  TMatrixD m_cov_statYield_;
  TMatrixD m_cov_statNonYield_;
  TMatrixD m_cov_systAcc_;
  TMatrixD m_cov_systNonAcc_;
  TMatrixD m_cov_lumi_;
  TMatrixD m_cov_tot_;

  // -- 2N*2N correlation matrices
  TMatrixD m_corr_statYield_;
  TMatrixD m_corr_statNonYield_;
  TMatrixD m_corr_systAcc_;
  TMatrixD m_corr_systNonAcc_;
  TMatrixD m_corr_lumi_;
  TMatrixD m_corr_tot_;

  // -- N*N cov. & corr. matrix for the combined cross section
  TMatrixD m_cov_tot_ll_;
  TMatrixD m_corr_tot_ll_;

  CombinationTool():
  m_dXSec_eemm_(1, 2*nMassBin),
  m_lambda_(nMassBin, 2*nMassBin),
  m_dXSec_ll_(1, nMassBin),
  m_cov_statYield_(2*nMassBin, 2*nMassBin),
  m_cov_statNonYield_(2*nMassBin, 2*nMassBin),
  m_cov_systAcc_(2*nMassBin, 2*nMassBin),
  m_cov_systNonAcc_(2*nMassBin, 2*nMassBin),
  m_cov_lumi_(2*nMassBin, 2*nMassBin),
  m_cov_tot_(2*nMassBin, 2*nMassBin),
  m_corr_statYield_(2*nMassBin, 2*nMassBin),
  m_corr_statNonYield_(2*nMassBin, 2*nMassBin),
  m_corr_systAcc_(2*nMassBin, 2*nMassBin),
  m_corr_systNonAcc_(2*nMassBin, 2*nMassBin),
  m_corr_lumi_(2*nMassBin, 2*nMassBin),
  m_corr_tot_(2*nMassBin, 2*nMassBin),
  m_cov_tot_ll_(nMassBin, nMassBin), m_corr_tot_ll_(nMassBin, nMassBin)
  {
    TH1::AddDirectory(kFALSE);
  }

  void Combine()
  {
    Init();
    Calc_Coefficient();
    Calc_CombinedDXSec();
    Calc_CovM();
    AssignUncertainty();
    Calc_StatUnc();
  }

  void PrintZPeakXSec()
  {
    PrintZPeakXSec("ee", h_dXSec_ee_);
    PrintZPeakXSec("mm", h_dXSec_mm_);
    PrintZPeakXSec("ll", h_dXSec_ll_);
  }

  void DrawPlot()
  {
    DrawPlot_Lambda();

    // -- 2N*2N matrix for ee-mm covariance/correlation
    DrawPlot_CovM("cov_statYield", m_cov_statYield_);
    DrawPlot_CovM("cov_statNonYield", m_cov_statNonYield_);
    DrawPlot_CovM("cov_systAcc", m_cov_systAcc_);
    DrawPlot_CovM("cov_systNonAcc", m_cov_systNonAcc_);
    DrawPlot_CovM("cov_lumi", m_cov_lumi_);
    DrawPlot_CovM("cov_tot", m_cov_tot_);

    DrawPlot_CovM("corr_statYield", m_corr_statYield_);
    DrawPlot_CovM("corr_statNonYield", m_corr_statNonYield_);
    DrawPlot_CovM("corr_systAcc", m_corr_systAcc_);
    DrawPlot_CovM("corr_systNonAcc", m_corr_systNonAcc_);
    DrawPlot_CovM("corr_lumi", m_corr_lumi_);
    DrawPlot_CovM("corr_tot", m_corr_tot_);


    // -- N*N matrix for the combined cross sections
    DrawPlot_CovM("cov_tot_ll", m_cov_tot_ll_);
    DrawPlot_CovM("corr_tot_ll", m_corr_tot_ll_);

    // -- Comparison plots between channels
    DrawPlot_Unc("abs");
    DrawPlot_Unc("rel"); // -- no relative histogram in the root file

    DrawPlot_dXSec(15,  3000);
    DrawPlot_dXSec(15,  60);
    DrawPlot_dXSec(60,  120);
    DrawPlot_dXSec(120, 440);
    DrawPlot_dXSec(440, 3000);
  }

  void Save()
  {
    TFile *f_output = TFile::Open("ROOTFile_Combination.root", "RECREATE");
    f_output->cd();

    f_output->mkdir("ll");
    f_output->cd("ll");

    h_dXSec_ll_->SetName("h_dXSec");
    h_dXSec_ll_->Write();

    h_absUnc_stat_ll_->SetName("h_absUnc_stat");
    h_absUnc_stat_ll_->Write();

    h_relUnc_stat_ll_->SetName("h_relUnc_stat");
    h_relUnc_stat_ll_->Scale(100.0); // -- convert to %
    h_relUnc_stat_ll_->Write();

    h_absUnc_tot_ll_->SetName("h_absUnc_tot");
    h_absUnc_tot_ll_->Write();

    h_relUnc_tot_ll_->SetName("h_relUnc_tot");
    h_relUnc_tot_ll_->Scale(100.0); // -- convert to %
    h_relUnc_tot_ll_->Write();

    // m_cov_tot_ll_.SetName("m_cov_tot_ll");
    // m_cov_tot_ll_.Write();

    TH2D* h_cov_tot_ll = new TH2D(m_cov_tot_ll_);
    h_cov_tot_ll->SetName("h_cov_tot");
    h_cov_tot_ll->Write();
  }

  void Set_DXSec(TH1D* h_ee, TH1D* h_mm)
  {
    h_dXSec_ee_ = (TH1D*)h_ee->Clone("h_dXSec_ee");
    h_dXSec_mm_ = (TH1D*)h_mm->Clone("h_dXSec_mm");
  }

  void Set_AbsUnc_Tot(TH1D* h_ee, TH1D* h_mm)
  {
    h_absUnc_tot_ee_ = (TH1D*)h_ee->Clone("h_absUnc_tot_ee");
    h_absUnc_tot_mm_ = (TH1D*)h_mm->Clone("h_absUnc_tot_mm");
  }

  void Set_CovM_StatYield(TH2D* h_ee, TH2D* h_mm)
  {
    h_cov_statYield_ee_ = (TH2D*)h_ee->Clone("h_cov_statYield_ee");
    h_cov_statYield_mm_ = (TH2D*)h_mm->Clone("h_cov_statYield_mm");
  }

  void Set_CovM_StatNonYield(TH2D* h_ee, TH2D* h_mm)
  {
    h_cov_statNonYield_ee_ = (TH2D*)h_ee->Clone("h_cov_statNonYield_ee");
    h_cov_statNonYield_mm_ = (TH2D*)h_mm->Clone("h_cov_statNonYield_mm");
  }

  void Set_CovM_SystAcc(TH2D* h_ee, TH2D* h_mm)
  {
    h_cov_systAcc_ee_ = (TH2D*)h_ee->Clone("h_cov_systAcc_ee");
    h_cov_systAcc_mm_ = (TH2D*)h_mm->Clone("h_cov_systAcc_mm");
  }

  void Set_CovM_SystNonAcc(TH2D* h_ee, TH2D* h_mm)
  {
    h_cov_systNonAcc_ee_ = (TH2D*)h_ee->Clone("h_cov_systNonAcc_ee");
    h_cov_systNonAcc_mm_ = (TH2D*)h_mm->Clone("h_cov_systNonAcc_mm");
  }

  void Set_CovM_EM(TH2D* h_em )
  {
    h_covEM_noLumi_ = (TH2D*)h_em->Clone("h_covEM_noLumi");
  }

  void Set_RelUnc_Lumi( Double_t relUnc_lumi )
  {
    relUnc_lumi_ = relUnc_lumi;
  }


private:
  void Init()
  {
    SetError_dXSec();
    InitMatrix_dXSec();

    InitCovMatrix_noCorrBetweenChannels(m_cov_statYield_,    m_corr_statYield_,    h_cov_statYield_ee_,    h_cov_statYield_mm_);
    InitCovMatrix_noCorrBetweenChannels(m_cov_statNonYield_, m_corr_statNonYield_, h_cov_statNonYield_ee_, h_cov_statNonYield_mm_);
    InitCovMatrix_noCorrBetweenChannels(m_cov_systNonAcc_,   m_corr_systNonAcc_,   h_cov_systNonAcc_ee_,   h_cov_systNonAcc_mm_);

    InitCovMatrix_CorrBetweenChannels(m_cov_systAcc_, m_corr_systAcc_, h_cov_systAcc_ee_, h_cov_systAcc_mm_, h_covEM_noLumi_);
    InitCovMatrix_Lumi(); // -- fully correlated between bins & channels

    // -- init. with empty histograms
    h_dXSec_ll_ = (TH1D*)h_dXSec_ee_->Clone();
    h_dXSec_ll_->Reset("ICES");
    h_absUnc_stat_ll_ = (TH1D*)h_dXSec_ll_->Clone();
    h_relUnc_stat_ll_ = (TH1D*)h_dXSec_ll_->Clone();
    h_absUnc_tot_ll_  = (TH1D*)h_dXSec_ll_->Clone();
    h_relUnc_tot_ll_  = (TH1D*)h_dXSec_ll_->Clone();
  }

  void InitMatrix_dXSec()
  {
    // -- m_dXSec_eemm_:
    // ---- 0 ... nMassBin-1: dXSec for e channel
    // ---- nMassBin ... 2*nMassBin-1: dXSec for mu channel
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec_ee = h_dXSec_ee_->GetBinContent(i_bin);
      m_dXSec_eemm_[0][i] = dXSec_ee;

      Double_t dXSec_mm = h_dXSec_mm_->GetBinContent(i_bin);
      m_dXSec_eemm_[0][i+nMassBin] = dXSec_mm;
    }
  }

  void SetError_dXSec()
  {
    // -- h_dXSec_ee: assigned uncertainty is not the total uncertainty -> need to update
    // -- do same thing for the muon channel to be sure
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t absUnc_tot_ee = h_absUnc_tot_ee_->GetBinContent(i_bin);
      h_dXSec_ee_->SetBinError(i_bin, absUnc_tot_ee);

      Double_t absUnc_tot_mm = h_absUnc_tot_mm_->GetBinContent(i_bin);
      h_dXSec_mm_->SetBinError(i_bin, absUnc_tot_mm);
    }
  }

  // -- fill m_lambda_ with the coefficient values (simple weighted mean, same with the case without any correlation)
  // -- 1...N: electron channel, N+1 ... 2N: muon channel
  void Calc_Coefficient()
  {
    // -- initialization: set as 0 for all elements
    for(Int_t i=0; i<nMassBin; i++)
    {
      for(Int_t j=0; j<2*nMassBin; j++)
      {
        m_lambda_[i][j] = 0;
      }
    }

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t unc_ee = h_absUnc_tot_ee_->GetBinContent(i_bin);
      Double_t unc_mm = h_absUnc_tot_mm_->GetBinContent(i_bin);

      Double_t num_ee = (1.0/unc_ee)*(1.0/unc_ee);
      Double_t num_mm = (1.0/unc_mm)*(1.0/unc_mm);
      Double_t den = num_ee + num_mm;

      Double_t coeff_ee = num_ee / den;
      Double_t coeff_mm = num_mm / den;

      m_lambda_[i][i]          = coeff_ee; // -- 1, 2, ... N: electron channel
      m_lambda_[i][i+nMassBin] = coeff_mm; // -- N+1, N+2, ... 2N: muon channel
    }
  }

  // -- combine two results using the coefficient derived by weighted mean
  void Calc_CombinedDXSec()
  {
    TMatrixD m_dXSec_eemm_T = TMatrixD(m_dXSec_eemm_);
    m_dXSec_eemm_T.Transpose(m_dXSec_eemm_T);

    TMatrixD m_dXSec_ll_T = m_lambda_ * m_dXSec_eemm_T; // -- nMassBin * 1 matrix
    m_dXSec_ll_.Transpose(m_dXSec_ll_T); // -- 1 * nMassBin matrix

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;

      h_dXSec_ll_->SetBinContent(i_bin, m_dXSec_ll_[0][i]);
      h_dXSec_ll_->SetBinError(i_bin, 0); // -- no error yet. It will be filled after the covariance matrix for the combiend result is constructed
    }
  }

  void Calc_CovM()
  {
    // -- make total ee-mm covariance matrix by the sum of individual cov. matrices
    m_cov_tot_ = m_cov_statYield_ + m_cov_statNonYield_ + m_cov_systNonAcc_ + m_cov_systAcc_ + m_cov_lumi_;
    Convert_Cov_to_Corr( m_cov_tot_, m_corr_tot_ );

    // if( DoVal_withAndrius_ )
    // {
    //   m_cov_tot_ = m_cov_statYield_ + m_cov_statNonYield_ + m_cov_systNonAcc_ + m_cov_systAcc_; // -- remove lumi. unc.
    //   RemoveOffDiagonalTerms( m_cov_tot_ ); // -- remove all correlations
    //   Convert_Cov_to_Corr( m_cov_tot_, m_corr_tot_ );
    // }

    TMatrixD m_lambdaT(m_lambda_);
    m_lambdaT.Transpose(m_lambdaT);
    m_cov_tot_ll_ = (m_lambda_ * m_cov_tot_) * m_lambdaT;
    Convert_Cov_to_Corr( m_cov_tot_ll_, m_corr_tot_ll_, kFALSE );
  }

  // -- no correlation between channels:
  // ---- (1...N * 1...N) block: cov. for e channel
  // ---- (N+1...2N * N+1...2N) block: cov. for mu channel
  // ---- Other blocks (1...N * N+1...2N, N+1...2N*1...N): empty (uncorrelated)
  void InitCovMatrix_noCorrBetweenChannels(TMatrixD& m_cov, TMatrixD& m_corr, TH2D* h_cov_ee_, TH2D* h_cov_mm_)
  {
    // -- initialization: set as 0 for all elements
    for(Int_t i=0; i<2*nMassBin; i++)
    {
      for(Int_t j=0; j<2*nMassBin; j++)
      {
        m_cov[i][j] = 0;
      }
    }

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      for(Int_t j=0; j<nMassBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t cov_ij_ee = h_cov_ee_->GetBinContent(i_bin, j_bin);
        m_cov[i][j] = cov_ij_ee;

        Double_t cov_ij_mm = h_cov_mm_->GetBinContent(i_bin, j_bin);
        m_cov[i+nMassBin][j+nMassBin] = cov_ij_mm;
      }
    }

    Convert_Cov_to_Corr(m_cov, m_corr);
  }

  void InitCovMatrix_CorrBetweenChannels(TMatrixD& m_cov, TMatrixD& m_corr, TH2D* h_cov_ee_, TH2D* h_cov_mm_, TH2D* h_covEM )
  {
    // -- initialization: set as 0 for all elements
    for(Int_t i=0; i<2*nMassBin; i++)
    {
      for(Int_t j=0; j<2*nMassBin; j++)
      {
        m_cov[i][j] = 0;
      }
    }

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      for(Int_t j=0; j<nMassBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t cov_ij_ee = h_cov_ee_->GetBinContent(i_bin, j_bin);
        m_cov[i][j] = cov_ij_ee;

        Double_t cov_ij_mm = h_cov_mm_->GetBinContent(i_bin, j_bin);
        m_cov[i+nMassBin][j+nMassBin] = cov_ij_mm;
      }
    }

    // -- correlation part bewteen two channels
    // -- h_covEM: x-axis = m(ee) bin, y-axis = m(mumu) bin
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      for(Int_t j=0; j<nMassBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t covEM_ij = h_covEM->GetBinContent(i_bin, j_bin);
        m_cov[i][j+nMassBin] = covEM_ij;
        m_cov[j+nMassBin][i] = covEM_ij;
      }
    }

    Convert_Cov_to_Corr(m_cov, m_corr);
  }

  void InitCovMatrix_Lumi()
  {
    vector<Double_t> vec_absUnc_lumi_ee;
    vector<Double_t> vec_absUnc_lumi_mm;

    vec_absUnc_lumi_ee.clear();
    vec_absUnc_lumi_mm.clear();

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec_ee       = h_dXSec_ee_->GetBinContent(i_bin);
      Double_t absUnc_lumi_ee = dXSec_ee * relUnc_lumi_;
      vec_absUnc_lumi_ee.push_back( absUnc_lumi_ee );

      Double_t dXSec_mm       = h_dXSec_mm_->GetBinContent(i_bin);
      Double_t absUnc_lumi_mm = dXSec_mm * relUnc_lumi_;
      vec_absUnc_lumi_mm.push_back( absUnc_lumi_mm );
    }

    for(Int_t i=0; i<2*nMassBin; i++)
    {
      Double_t sigma_i = 0;
      if( i < nMassBin ) sigma_i = vec_absUnc_lumi_ee[i];
      else               sigma_i = vec_absUnc_lumi_mm[i-nMassBin]; // -- nMassBin ... 2*nMassBin-1: muon

      for(Int_t j=0; j<2*nMassBin; j++)
      {
        Double_t sigma_j = 0;
        if( j < nMassBin ) sigma_j = vec_absUnc_lumi_ee[j];
        else               sigma_j = vec_absUnc_lumi_mm[j-nMassBin]; // -- nMassBin ... 2*nMassBin-1: muon

        Double_t cov_ij = sigma_i*sigma_j; // -- fully correlated

        m_cov_lumi_[i][j] = cov_ij;
      }
    }

    Convert_Cov_to_Corr(m_cov_lumi_, m_corr_lumi_);
  }

  // -- convert 2N*2N cov matrix to corr matrix
  void Convert_Cov_to_Corr(TMatrixD& m_cov, TMatrixD& m_corr, Bool_t is2NcovM = kTRUE)
  {
    Int_t nBin = 0;
    if( is2NcovM ) nBin = 2*nMassBin;
    else           nBin = nMassBin;

    // -- initialization: set as 0 for all elements
    for(Int_t i=0; i<nBin; i++)
    {
      for(Int_t j=0; j<nBin; j++)
      {
        m_corr[i][j] = 0;
      }
    }

    for(Int_t i=0; i<nBin; i++)
    {
      for(Int_t j=0; j<nBin; j++)
      {
        Double_t cov_ij = m_cov[i][j];
        Double_t var_i  = m_cov[i][i];
        Double_t var_j  = m_cov[j][j];

        Double_t corr_ij = cov_ij / sqrt(var_i*var_j);
        m_corr[i][j] = corr_ij;
        // printf("(i, j) = (%d, %d) -> (cov, var_i, var_j, corr) = (%.3e, %.3e, %.3e, %.3e)\n", i, j, cov_ij, var_i, var_j, corr_ij);
      }
    }
  }

  void AssignUncertainty()
  {
    vector<Double_t> vec_absUnc;
    for(Int_t i=0; i<nMassBin; i++)
      vec_absUnc.push_back( sqrt(m_cov_tot_ll_[i][i]) ); // -- take the uncertainty from the diagonal terms of total cov. matrix

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec = h_dXSec_ll_->GetBinContent(i_bin);
      Double_t absUnc = vec_absUnc[i];
      Double_t relUnc = absUnc / dXSec;

      h_dXSec_ll_->SetBinError(i_bin, absUnc);

      h_absUnc_tot_ll_->SetBinContent(i_bin, absUnc);
      h_absUnc_tot_ll_->SetBinError(i_bin, 0);

      h_relUnc_tot_ll_->SetBinContent(i_bin, relUnc);
      h_relUnc_tot_ll_->SetBinError(i_bin, 0);
    }
  }

  void Calc_StatUnc()
  {
    // -- make total ee-mm covariance matrix using only the cov from stat. unc.
    TMatrixD m_lambdaT(m_lambda_);
    m_lambdaT.Transpose(m_lambdaT);

    TMatrixD m_cov_statYield_ll = (m_lambda_ * m_cov_statYield_) * m_lambdaT;

    vector<Double_t> vec_absUnc;
    for(Int_t i=0; i<nMassBin; i++)
      vec_absUnc.push_back( sqrt(m_cov_statYield_ll[i][i]) ); // -- take the uncertainty from the diagonal terms of the cov. matrix

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec = h_dXSec_ll_->GetBinContent(i_bin);
      Double_t absUnc = vec_absUnc[i];
      Double_t relUnc = absUnc / dXSec;

      h_absUnc_stat_ll_->SetBinContent(i_bin, absUnc);
      h_absUnc_stat_ll_->SetBinError(i_bin, 0);

      h_relUnc_stat_ll_->SetBinContent(i_bin, relUnc);
      h_relUnc_stat_ll_->SetBinError(i_bin, 0);
    }
  }

  void DrawPlot_Lambda()
  {
    // -- 2D histogram
    TH2D* h2D = new TH2D(m_lambda_);

    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_lambda", "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    // c->SetLogx();
    // c->SetLogy();

    h2D->SetTitle("");
    h2D->SetStats(kFALSE);
    h2D->GetXaxis()->SetTitle( "bin index (1-43: ee, 44-86: #mu#mu)" );
    h2D->GetXaxis()->SetLabelSize(0.04);
    h2D->GetXaxis()->SetTitleOffset(1.1);
    h2D->GetXaxis()->SetTitleSize(0.05);
    h2D->GetXaxis()->SetNoExponent(kFALSE);
    h2D->GetXaxis()->SetMoreLogLabels(kFALSE);

    h2D->GetYaxis()->SetTitle( "bin index (1-43)" );
    h2D->GetYaxis()->SetTitleOffset(1.3);
    h2D->GetYaxis()->SetTitleSize(0.05);
    h2D->GetYaxis()->SetLabelSize(0.045);
    h2D->GetYaxis()->SetNoExponent(kFALSE);
    h2D->GetYaxis()->SetMoreLogLabels(kFALSE);

    h2D->GetZaxis()->SetRangeUser(0, 1.01);

    h2D->Draw("COLZ");

    c->SaveAs(".pdf");

    // -- 1D histogram for the comparison of the coefficient on the electron and muon channel as 1D

    TH1D* h_coeff_ee = new TH1D("h_coeff_ee", "", nMassBin, 0, nMassBin);
    TH1D* h_coeff_mm = new TH1D("h_coeff_mm", "", nMassBin, 0, nMassBin);
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t coeff_ee = m_lambda_[i][i];
      Double_t coeff_mm = m_lambda_[i][i+nMassBin];

      h_coeff_ee->SetBinContent(i_bin, coeff_ee);
      h_coeff_ee->SetBinError(i_bin, 0);

      h_coeff_mm->SetBinContent(i_bin, coeff_mm);
      h_coeff_mm->SetBinError(i_bin, 0);
    }

    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas("c_lambda_ee_mm", 0, 0);
    canvas->SetTitle("Bin index", "Coefficients (#lambda)");

    canvas->Register( h_coeff_mm, "#mu channel", kBlue);
    canvas->Register( h_coeff_ee, "e channel",   kGreen+2);

    canvas->SetLegendPosition(0.60, 0.84, 0.97, 0.95);

    canvas->SetRangeY(0, 1.2);

    canvas->Latex_CMSPre();
    // canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{Combined cross sections}}");
    canvas->Draw("HISTLPSAME");
  }

  void DrawPlot_CovM( TString type, TMatrixD m_cov )
  {
    TH2D* h2D = new TH2D(m_cov);

    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_"+type, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    // c->SetLogx();
    // c->SetLogy();

    TString axisTitle = "bin index (1-43: ee, 44-86: #mu#mu)";
    if( type.Contains("ll") ) axisTitle = "bin index (ll)";

    h2D->SetTitle("");
    h2D->SetStats(kFALSE);
    h2D->GetXaxis()->SetTitle( axisTitle );
    h2D->GetXaxis()->SetLabelSize(0.04);
    h2D->GetXaxis()->SetTitleOffset(1.1);
    h2D->GetXaxis()->SetTitleSize(0.05);
    h2D->GetXaxis()->SetNoExponent(kFALSE);
    h2D->GetXaxis()->SetMoreLogLabels(kFALSE);

    h2D->GetYaxis()->SetTitle( axisTitle );
    h2D->GetYaxis()->SetTitleOffset(1.3);
    h2D->GetYaxis()->SetTitleSize(0.05);
    h2D->GetYaxis()->SetLabelSize(0.045);
    h2D->GetYaxis()->SetNoExponent(kFALSE);
    h2D->GetYaxis()->SetMoreLogLabels(kFALSE);

    if( type.Contains("corr") )
      h2D->GetZaxis()->SetRangeUser(-1.0, 1.0);

    TLatex latex;
    latex.DrawLatexNDC(0.16, 0.91, "#font[42]{#scale[0.6]{"+type+"}}");

    h2D->Draw("COLZ");

    c->Update(); // -- without this, palette is not made...
    TPaletteAxis *palette = (TPaletteAxis*)h2D->GetListOfFunctions()->FindObject("palette");
    palette->SetX1NDC(0.88);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.13);
    palette->SetY2NDC(0.95);


    c->SaveAs(".pdf");
  }

  void DrawPlot_dXSec(Double_t minM, Double_t maxM)
  {
    TString canvasName = TString::Format("c_dXSec_eemmll_M%.0lftoM%.0lf", minM, maxM);

    Bool_t isLogX = kFALSE;
    if( maxM == 3000 ) isLogX = kTRUE;

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, isLogX, 1 );
    canvas->Register( h_dXSec_ll_, "Combined result", kRed);
    canvas->Register( h_dXSec_mm_, "#mu channel", kBlue);
    canvas->Register( h_dXSec_ee_, "e channel", kGreen+2);
    canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "ee(#mu#mu)/combined");

    canvas->SetLegendPosition( 0.60, 0.78, 0.95, 0.91 );
    canvas->SetRangeX(minM, maxM);
    canvas->SetRangeY(1e-8, 1e+4);
    if( minM == 15 && maxM == 60 )    canvas->SetRangeY(1, 1e3);
    if( minM == 60 && maxM == 120 )   canvas->SetRangeY(0.5, 1e3);
    if( minM == 120 && maxM == 3000 ) canvas->SetRangeY(1e-8, 5);
    if( minM == 120 && maxM == 440 )  canvas->SetRangeY(1e-3, 2);
    if( minM == 440 && maxM == 3000 ) canvas->SetRangeY(1e-8, 1e-2);

    // canvas->SetRangeRatio(0.84, 1.16);
    canvas->SetRangeRatio(0.86, 1.14);
    // canvas->SetRangeRatio(0.7, 1.3);
    // canvas->SetRangeRatio(0.3, 1.7);
    // canvas->SetRangeRatio(0, 5.0);

    TString massRangeInfo = "";
    if( minM == 15 && maxM == 60 )    massRangeInfo = "Below Z peak region";
    if( minM == 60 && maxM == 120 )   massRangeInfo = "Z peak region";
    if( minM == 120 && maxM == 3000 ) massRangeInfo = "Above Z peak region";
    if( minM == 120 && maxM == 440 )  massRangeInfo = "Above Z peak region (120 < M < 440 GeV)";
    if( minM == 440 && maxM == 3000 ) massRangeInfo = "Above Z peak region (M > 440 GeV)";

    canvas->RegisterLatex(0.16, 0.91, "#font[62]{#scale[0.6]{"+massRangeInfo+"}}");
    canvas->Latex_CMSPre();

    canvas->Draw();
  }

  void DrawPlot_Unc(TString uncType)
  {
    TString canvasName = TString::Format("c_%sUnc_eemmll", uncType.Data());

    TH1D* h_unc_ee = nullptr;
    TH1D* h_unc_mm = nullptr;
    TH1D* h_unc_ll = nullptr;
    TString titleY = "";
    if( uncType == "abs" )
    {
      h_unc_ee = (TH1D*)h_absUnc_tot_ee_->Clone();
      h_unc_mm = (TH1D*)h_absUnc_tot_mm_->Clone();
      h_unc_ll = (TH1D*)h_absUnc_tot_ll_->Clone();
      titleY = "Abs. unc. [pb/GeV]";
    }
    if( uncType == "rel" )
    {
      h_unc_ee = PlotTool::Extract_RelUnc(h_dXSec_ee_);
      h_unc_mm = PlotTool::Extract_RelUnc(h_dXSec_mm_);
      h_unc_ll = (TH1D*)h_relUnc_tot_ll_->Clone();

      h_unc_ee->Scale(100);
      h_unc_mm->Scale(100);
      h_unc_ll->Scale(100);
      titleY = "Rel. unc. [%]";
    }

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, 1, 1 );
    canvas->Register( h_unc_ll, "Combined result", kRed);
    canvas->Register( h_unc_mm, "#mu channel", kBlue);
    canvas->Register( h_unc_ee, "e channel", kGreen+2);
    canvas->SetTitle("m [GeV]", titleY, "ee(#mu#mu)/combined");

    canvas->SetLegendPosition( 0.60, 0.78, 0.95, 0.90 );
    if( uncType == "rel" ) canvas->SetLegendPosition( 0.16, 0.78, 0.51, 0.90 );
    // canvas->SetRangeX(minM, maxM);
    if( uncType == "abs" ) canvas->SetRangeY(1e-8, 1e+3);
    if( uncType == "rel" ) canvas->SetRangeY(2, 200);

    // canvas->SetRangeRatio(0.84, 1.16);
    // canvas->SetRangeRatio(0.7, 1.3);
    canvas->SetRangeRatio(0.95, 1.2);
    // canvas->SetRangeRatio(0.96, 1.04);
    // canvas->SetRangeRatio(0, 5.0);

    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{Total uncertainty (#delta_{stat}+#delta_{exp.}+#delta_{theo.})}}");
    canvas->Latex_CMSPre();

    canvas->Draw("HISTLPSAME");
  }

  void PrintZPeakXSec(TString channel, TH1D* h_dXSec)
  {
    Double_t xSec_ZPeak = 0;
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binCenter = h_dXSec->GetBinCenter(i_bin);

      if( 60 < binCenter && binCenter < 120 )
      {
        Double_t dXSec    = h_dXSec->GetBinContent(i_bin);
        Double_t binWidth = h_dXSec->GetBinWidth(i_bin);

        Double_t xSec = dXSec*binWidth;
        xSec_ZPeak += xSec;
      }
    }

    printf("[channel = %s] Z cross section (60 < M < 120 GeV) = %lf pb\n\n", channel.Data(), xSec_ZPeak);
  }
};

void Combination_Hybrid()
{
  // -- Get input histograms
  // TString fileName = "./Converter_BLUEOutput/ROOTFile_hepdata__corr02Aug_Full_Converted.root";
  // TString fileName = "./Converter_BLUEOutput/ROOTFile_hepdata__corr02Aug_Full_myCov_effSFSyst_Converted.root"; // -- switch the cov. M for eff.SF.syst to my result
  // TString fileName = "./Converter_BLUEOutput/ROOTFile_hepdata__corr08Aug_Full_myDXSecEE_myCovEffSF_CovSFApplied_Converted.root"; // -- SF applied
  TString fileName = "./Converter_BLUEOutput/ROOTFile_hepdata__corr12Aug_Full_newEffSF_Converted.root"; // -- SF applied

  TH1D* h_dXSec_ee = PlotTool::Get_Hist(fileName, "ee/h_dXSec");
  TH1D* h_dXSec_mm = PlotTool::Get_Hist(fileName, "mm/h_dXSec");

  TH1D* h_absUnc_tot_ee = PlotTool::Get_Hist(fileName, "ee/h_absUnc_tot");
  TH1D* h_absUnc_tot_mm = PlotTool::Get_Hist(fileName, "mm/h_absUnc_tot");

  TH2D* h_cov_statYield_ee = PlotTool::Get_Hist2D(fileName, "ee/h_cov_statYield");
  TH2D* h_cov_statYield_mm = PlotTool::Get_Hist2D(fileName, "mm/h_cov_statYield");

  TH2D* h_cov_statNonYield_ee = PlotTool::Get_Hist2D(fileName, "ee/h_cov_statNonYield");
  TH2D* h_cov_statNonYield_mm = PlotTool::Get_Hist2D(fileName, "mm/h_cov_statNonYield");

  TH2D* h_cov_systAcc_ee = PlotTool::Get_Hist2D(fileName, "ee/h_cov_systAcc");
  TH2D* h_cov_systAcc_mm = PlotTool::Get_Hist2D(fileName, "mm/h_cov_systAcc");

  TH2D* h_cov_systNonAcc_ee = PlotTool::Get_Hist2D(fileName, "ee/h_cov_systNonAcc");
  TH2D* h_cov_systNonAcc_mm = PlotTool::Get_Hist2D(fileName, "mm/h_cov_systNonAcc");

  TH2D* h_covEM_noLumi_ = PlotTool::Get_Hist2D(fileName, "ll/h_covEM_noLumi");

  Double_t relUnc_lumi = 0.023;

  // -- combine
  CombinationTool* tool = new CombinationTool();

  tool->Set_DXSec(h_dXSec_ee, h_dXSec_mm);
  tool->Set_AbsUnc_Tot(h_absUnc_tot_ee, h_absUnc_tot_mm);

  tool->Set_CovM_StatYield(   h_cov_statYield_ee,    h_cov_statYield_mm);
  tool->Set_CovM_StatNonYield(h_cov_statNonYield_ee, h_cov_statNonYield_mm);
  tool->Set_CovM_SystAcc(     h_cov_systAcc_ee,      h_cov_systAcc_mm);
  tool->Set_CovM_SystNonAcc(  h_cov_systNonAcc_ee,   h_cov_systNonAcc_mm);

  tool->Set_CovM_EM( h_covEM_noLumi_ );

  tool->Set_RelUnc_Lumi( relUnc_lumi );

  tool->Combine();
  tool->DrawPlot();
  tool->PrintZPeakXSec();
  tool->Save();
}