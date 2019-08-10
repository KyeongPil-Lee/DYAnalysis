#include <Include/PlotTools.h>
#define nBin 43

class pValueTool
{
public:
  Bool_t useOldResult_ = kFALSE;

  Double_t massBinEdges_[nBin+1];

  TH1D* h_dXSec_data_;
  TH2D* h_cov_data_;
  TH1D* h_dXSec_theory_;

  TMatrixD y_; // y_i = data - theory
  TMatrixD mCov_data_; // -- data
  TMatrixD mCov_theory_; // -- from FEWZ, fully correlated matrix
  TMatrixD mCov_tot_; // -- (data + theory)

  TH1D* h_absContToChi2_;
  TH1D* h_relContToChi2_;

  pValueTool(Bool_t useOldResult):
  y_(1, nBin),
  mCov_data_(nBin, nBin),
  mCov_theory_(nBin, nBin),
  mCov_tot_(nBin, nBin)
  {
    UseOldResult( useOldResult );
    Init();
  }

  void UseOldResult(Bool_t flag = kTRUE)
  {
    useOldResult_ = flag;
    cout << "[UseOldResult] " << useOldResult_ << endl;
    cout << endl;
  }

  void pValue_NoCorrelation()
  {
    Double_t totalChi2 = 0;

    for(Int_t i=0; i<nBin; i++)
    {
      Double_t diff = y_[0][i];
      Double_t sigma_tot = sqrt( mCov_tot_[i][i] );
      Double_t chi2 = (diff / sigma_tot)*(diff / sigma_tot);
      totalChi2 += chi2;
    }

    Double_t pValue = ROOT::Math::chisquared_cdf_c(totalChi2, nBin);
    printf("[pValue_NoCorrelation] (total chi2, nDOF, p-value) = (%lf, %d, %lf)\n", totalChi2, nBin, pValue);

    cout << "[pValue_NoCorrelation] Bin by bin contribution" << endl;
    for(Int_t i=0; i<nBin; i++)
    {
      Double_t diff = y_[0][i];
      Double_t sigma_tot = sqrt( mCov_tot_[i][i] );
      Double_t chi2 = (diff / sigma_tot)*(diff / sigma_tot);

      Double_t ratio = (chi2/totalChi2) * 100; // -- %

      printf("[%4.0lf < M < %4.0lf] (diff, error, chi2, ratio to total chi2.) = (%.3e, %.3e, %.3lf, %.3lf %%)\n",
               massBinEdges_[i], massBinEdges_[i+1], diff, sigma_tot, chi2, ratio);
    }
    cout << endl;
  }

  void pValue_WithCorrelation()
  {
    TMatrixD mCovInvert(mCov_tot_);
    mCovInvert.Invert();

    TMatrixD yT(y_);
    yT.Transpose(yT);

    TMatrixD mChi2 = (y_ * mCovInvert) * yT;
    Double_t totalChi2 = mChi2[0][0];
    Double_t pValue = ROOT::Math::chisquared_cdf_c(totalChi2, nBin);
    printf("[pValue_WithCorrelation] (total chi2, nDOF, p-value) = (%lf, %d, %lf)\n", totalChi2, nBin, pValue);

    cout << "[pValue_WithCorrelation] bin by bin contribution" << endl;

    Double_t totalChi2_validation = 0;
    Double_t totalChi2_no830to1000Bin = 0;
    for(Int_t i=0; i<nBin; i++)
    {
      Double_t y_i = y_[0][i];
      Double_t cont_toChi2 = 0;

      for(Int_t j=0; j<nBin; j++)
      {
        Double_t y_j = y_[0][j];
        Double_t covInv_ij = mCovInvert[i][j];

        cont_toChi2 += y_i * covInv_ij * y_j;
      }

      Double_t ratio = (cont_toChi2 / totalChi2) * 100;

      printf("[%4.0lf < M < %4.0lf] (chi2, ratio to total chi2.) = (%.3lf, %.3lf %%)\n",
               massBinEdges_[i], massBinEdges_[i+1], cont_toChi2, ratio);

      Int_t i_bin = i+1;
      h_absContToChi2_->SetBinContent(i_bin, cont_toChi2); h_absContToChi2_->SetBinError(i_bin, 0);
      h_relContToChi2_->SetBinContent(i_bin, ratio);       h_relContToChi2_->SetBinError(i_bin, 0);

      totalChi2_validation += cont_toChi2;
      if( massBinEdges_[i] != 830 ) totalChi2_no830to1000Bin += cont_toChi2;
    }

    cout << "  * Validation: total chi2 by matrix multiplication, total chi2 by adding each contribution = " << totalChi2 << ", " << totalChi2_validation << endl;

    Double_t pValue_no830to1000Bin = ROOT::Math::chisquared_cdf_c(totalChi2_no830to1000Bin, nBin-1);
    printf("[p-value without 830 < M < 1000 GeV bin] (total chi2, p-value) = (%lf, %lf)\n", totalChi2_no830to1000Bin, pValue_no830to1000Bin);
    cout << endl;

    TString outputFileName = "ROOTFile_pValue_dataTheory.root";
    if( useOldResult_ ) outputFileName = outputFileName.ReplaceAll(".root", "_oldData.root");
    TFile *f_output = TFile::Open(outputFileName, "RECREATE");
    f_output->cd();
    h_absContToChi2_->Write();
    h_relContToChi2_->Write();
    f_output->Close();

    cout << outputFileName << " is created" << endl;
    cout << endl;
  }

private:
  void Init()
  {
    Double_t massBinEdges_temp[nBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
                       64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
                       110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
                       200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
                       830, 1000, 1500, 3000};
    for(Int_t i=0; i<nBin+1; i++)
      massBinEdges_[i] = massBinEdges_temp[i];

    // -- data cross section & covariance matrices
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName_data = rootFilePath + "/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
    h_dXSec_data_ = Get_Hist( fileName_data, "ll/h_dXSec");
    h_cov_data_   = Get_Hist_2D( fileName_data, "ll/h_cov_tot" );

    if( useOldResult_ )
      Init_OldDataResult();

    // -- theory cross sections
    TString fileName_theory = rootFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
    h_dXSec_theory_ = Get_Hist( fileName_theory, "h_DiffXsec_FEWZ_NNPDF_NNLO" );

    Init_y();
    Init_CovM_Data();
    Init_CovM_Theory();
    mCov_tot_ = mCov_data_ + mCov_theory_;

    h_absContToChi2_ = new TH1D("h_absContToChi2", "", nBin, massBinEdges_);
    h_relContToChi2_ = new TH1D("h_relContToChi2", "", nBin, massBinEdges_);

    // PrintCov();
  }

  void Init_OldDataResult()
  {
    delete h_dXSec_data_;
    delete h_cov_data_;

    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

    // -- Data: combined result
    TString fileName_data = rootFilePath + "/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
    // -- convert TH1F to TH1D -- //
    TFile *f_input = TFile::Open( fileName_data );
    f_input->cd();
    TH1F* h_temp = (TH1F*)f_input->Get( "h1Combi" )->Clone();
    h_dXSec_data_ = new TH1D();
    h_temp->Copy( *h_dXSec_data_ );
    h_dXSec_data_->SetTitle("");


    TCanvas *c2DCov = (TCanvas*)f_input->Get("cCovFin")->Clone();
    TPad* pad = (TPad*)c2DCov->FindObject("cCovFin_1");
    h_cov_data_ = (TH2D*)pad->GetPrimitive("h2covFin")->Clone();
  }

  void PrintCov()
  {
    cout << "[PrintCov]" << endl;
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        printf("[%02d bin, %02d bin] covariance: (data, theory, total) = (%.5lf, %.5lf, %.5lf)\n", i_bin, j_bin, mCov_data_[i][j], mCov_theory_[i][j], mCov_tot_[i][j]);
      }
    }

    cout << endl;
  }

  void Init_y()
  {
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      
      Double_t dXSec_data   = h_dXSec_data_->GetBinContent(i_bin);
      Double_t dXSec_theory = h_dXSec_theory_->GetBinContent(i_bin);

      // -- y_i = x_i - mu_i
      y_[0][i] = dXSec_data - dXSec_theory;
    }
  }

  void Init_CovM_Data()
  {
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t cov_ij = h_cov_data_->GetBinContent(i_bin, j_bin);
        mCov_data_[i][j] = cov_ij;
      }
    }
  }

  void Init_CovM_Theory()
  {
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t sigma_i = h_dXSec_theory_->GetBinError(i_bin);

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;
        Double_t sigma_j = h_dXSec_theory_->GetBinError(j_bin);

        Double_t cov_ij = sigma_i * sigma_j;
        mCov_theory_[i][j] = cov_ij;

        // if( i == j )
        // {
        //   Double_t dXSec = h_dXSec_theory_->GetBinContent(i_bin);
        //   Double_t relUnc = ( sigma_i / dXSec ) * 100; // --  %
        //   printf("[Init_CovM_Theory: %02d bin] error = %.3lf (%.3lf %%) -> cov. = %.3lf\n", i_bin, sigma_i, relUnc, mCov_theory_[i][i]);
        // }
      }
    }
  }
};


void pValue_dataTheory()
{
  pValueTool* tool_old = new pValueTool(kTRUE);
  tool_old->pValue_NoCorrelation();
  tool_old->pValue_WithCorrelation();

  pValueTool* tool_new = new pValueTool(kFALSE);
  tool_new->pValue_NoCorrelation();
  tool_new->pValue_WithCorrelation();

}