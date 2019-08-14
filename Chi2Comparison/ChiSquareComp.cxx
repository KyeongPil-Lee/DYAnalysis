#include <Include/PlotTools.h>
#define nBin 43


// -- calculate p-value from the chi square comparison between experimental diff.x-section and theory prediction.
// -- 
class Chi2CompTool {
public:
  Chi2CompTool():
    y_(1, nBin),
    mCov_(nBin, nBin),
    mCov_theory_(nBin, nBin),
    mCov_tot_(nBin, nBin)
  { 
    Init();
  }

  void CalcSimpleChi2() {
    Double_t simpleChi2 = 0;

    // -- sum
    for(Int_t i=0; i<nBin; i++) {
      Double_t diff = y_[0][i];
      Double_t sigma = sqrt(mCov_[i][i] + mCov_theory_[i][i]);
      simpleChi2 += (diff * diff) / sigma;
    }

    Double_t pValue = ROOT::Math::chisquared_cdf_c(simpleChi2, nBin);
    printf("[without correlation] (simpleChi2, p-value) = (%lf, %lf)\n", simpleChi2, pValue);

    cout << "Individual contribution" << endl;
    for(Int_t i=0; i<nBin; i++)
    {
      Double_t diff = y_[0][i];
      Double_t sigma = sqrt(mCov_[i][i] + mCov_theory_[i][i]);

      Double_t chi2_thisBin = (diff * diff) / sigma;
      Double_t frac_wrtTot =  chi2_thisBin / simpleChi2;

      Int_t i_bin = i+1;
      printf("[%02d bin: %.0lf < M < %.0lf] chi2 = %lf -> fraction w.r.t. total chi2 = %lf\n", i_bin, massBinEdges_[i], massBinEdges_[i+1], chi2_thisBin, frac_wrtTot);
    }
    cout << endl;
  }

  void CalcChi2() {
    mCov_tot_ = mCov_ + mCov_theory_; // -- take into account both data and theory uncertainties & their correlations
    TMatrixD mCovInvert(mCov_tot_);
    mCovInvert.Invert();

    TMatrixD yT(y_);
    yT.Transpose(yT);

    // printf("y_: nRow, nColumn = (%d, %d)\n", y_.GetNrows(), y_.GetNcols());
    // printf("yT: nRow, nColumn = (%d, %d)\n", yT.GetNrows(), yT.GetNcols());

    TMatrixD mChi2 = (y_ * mCovInvert) * yT;
    // printf("mChi2: nRow, nColumn = (%d, %d)\n", mChi2.GetNrows(), mChi2.GetNcols());

    Double_t chi2 = mChi2[0][0];
    Double_t pValue = ROOT::Math::chisquared_cdf_c(chi2, nBin);
    printf("[with correlation] (chi2, p-value) = (%lf, %lf)\n", chi2, pValue);

    // for(Int_t i=0; i<nBin; i++)
    // {
    //   Int_t i_bin = i+1;
    //   for(Int_t j=0; j<nBin; j++)
    //   {
    //     Int_t j_bin = j+1;
    //     if(i < j )
    //       printf("(%02d, %02d): cov. = %.3lf\n", i_bin, j_bin, mCov_[i][j]);
    //   }
    // }
  }

  void Test_pValueCalc() {
    cout << "============================" << endl;
    cout << "Test the p-value calculation" << endl;
    Int_t nDOF = 10;

    // -- compare with the result in the table:
    // -- https://en.wikipedia.org/wiki/Chi-squared_distribution#Table_of_%CF%872_values_vs_p-values
    vector<Double_t> vec_chi2 = {3.94, 4.87, 6.18, 18.31, 23.21, 29.59};

    for(const auto& chi2 : vec_chi2) {
      Double_t pValue = ROOT::Math::chisquared_cdf_c(chi2, nDOF);
      printf("(chi2, p-value) = (%lf, %lf)\n", chi2, pValue);
    }
    cout << "============================" << endl;
  }

  void ValidateCovMatrix() {
    cout << "==========================" << endl;
    cout << "Validate covariance matrix" << endl;
    // -- sqrt(cov(i, i)) should be same with the uncertainty of i-th bin
    for(Int_t i=0; i<nBin; i++) {
      Int_t i_bin = i+1;

      Double_t unc_i = hData_->GetBinError(i_bin);
      Double_t cov_i = h2DCov_->GetBinContent(i_bin, i_bin);
      Double_t sqrtCov_i = sqrt(cov_i);

      printf("[%02d bin] (unc_i, sqrtCov_i) = (%.6lf, %.6lf) -> Diff = %lf %%\n", \
               i_bin, unc_i, sqrtCov_i, ((sqrtCov_i-unc_i)/unc_i)*100 );
    }
    cout << "==========================" << endl;
    cout << endl;
  }

private:
  Double_t massBinEdges_[nBin+1];
  
  TH1D* hData_;

  TH1D* hTheory_;
  TH2D* h2DCov_;

  TMatrixD y_; // y_i = x_i - mu_i
  TMatrixD mCov_; // -- data
  TMatrixD mCov_theory_; // -- from FEWZ, fully correlated matrix
  TMatrixD mCov_tot_; // -- (data + theory)

  void Init() {
    Double_t massBinEdges_temp[nBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
                       64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
                       110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
                       200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
                       830, 1000, 1500, 3000};
    for(Int_t i=0; i<nBin+1; i++)
      massBinEdges_[i] = massBinEdges_temp[i];
    
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

    // -- Data: combined result
    TString fileName_data = rootFilePath + "/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
    hData_ = Get_Hist( fileName_data, "ll/h_dXSec");

    // // -- Data: muon
    // TString fileName_mu = rootFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
    // hData_mu = Get_Hist(fileName_mu, "h_DiffXsec_FSRCorr");

    // // -- Data: electron
    // TString fileName_elec = rootFilePath + "/DiffXsec_Electron_v8.root";
    // hData_elec = Get_Hist(fileName_elec, "h_DiffXSec");


    // -- covariance matrix
    h2DCov_ = Get_Hist_2D( fileName_data, "ll/h_cov_tot" );

    // -- Theory
    TString fileName_theory = rootFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
    hTheory_ = Get_Hist( fileName_theory, "h_DiffXsec_FEWZ_NNPDF_NNLO" );

    InitVector();
    InitCovMatrix();
    InitCovMatrix_Theory();
  }

  void InitVector() {    
    for(Int_t i=0; i<nBin; i++) {
      Int_t i_bin = i+1;
      
      Double_t dxSec_data = hData_->GetBinContent(i_bin);
      Double_t dxSec_theory = hTheory_->GetBinContent(i_bin);

      // -- y_i = x_i - mu_i
      y_[0][i] = dxSec_data - dxSec_theory;
      // printf("[%02d bin] (data, theory, diff) = (%.3e, %.3e, %.3e (%.3lf %%))\n", \
      //         i_bin, dxSec_data, dxSec_theory, y_[0][i], (y_[0][i]/dxSec_theory)*100 );
    }

    // printf("y_: nRow, nColumn = (%d, %d)\n", y_.GetNrows(), y_.GetNcols());
  }

  void InitCovMatrix() {
    for(Int_t i=0; i<nBin; i++) {
      Int_t i_bin = i+1;

      for(Int_t j=0; j<nBin; j++) {
        Int_t j_bin = j+1;

        Double_t covValue_ij = h2DCov_->GetBinContent(i_bin, j_bin);

        // printf("(%02d, %02d): cov. = %.3lf\n", i_bin, j_bin, covValue_ij);
        mCov_[i][j] = covValue_ij;
      }
    }
  }

  void InitCovMatrix_Theory()
  {
    cout << "[InitCovMatrix_Theory]" << endl;
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t sigma_i = hTheory_->GetBinError(i_bin); // -- absolute error

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;
        Double_t sigma_j = hTheory_->GetBinError(j_bin); // -- absolute error

        mCov_theory_[i][j] = sigma_i*sigma_j; // -- fully correlated between bins
        printf("(%02d, %02d): sigma_i = %.3lf, sigma_j = %.3lf -> cov. = %.3lf\n", i_bin, j_bin, sigma_i, sigma_j, mCov_theory_[i][j]);
      }
    }

    cout << endl;
  }
};


void ChiSquareComp() {
  Chi2CompTool *tool = new Chi2CompTool();
  // tool->ValidateCovMatrix();
  // tool->Test_pValueCalc();
  tool->CalcSimpleChi2();
  tool->CalcChi2();
}