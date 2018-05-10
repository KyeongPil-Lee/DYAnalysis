#include <Include/PlotTools.h>
#define nBin 43


// -- calculate p-value from the chi square comparison between experimental diff.x-section and theory prediction.
// -- 
class Chi2CompTool {
public:
  Chi2CompTool():
    y_(1, nBin),
    mCov_(nBin, nBin) 
  { 
    Init();
  }

  void CalcSimpleChi2() {
    Double_t simpleChi2 = 0;

    // -- sum
    for(Int_t i=0; i<nBin; i++) {
      Double_t diff = y_[0][i];
      Double_t sigma = sqrt(mCov_[i][i]);
      simpleChi2 += (diff * diff) / sigma;
    }

    Double_t pValue = ROOT::Math::chisquared_cdf_c(simpleChi2, nBin);
    printf("[without correlation] (simpleChi2, p-value) = (%lf, %lf)\n", simpleChi2, pValue);
  }

  void CalcChi2() {
    TMatrixD mCovInvert(mCov_);
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
  TH1D* hData_;
  // -- no corresponding covariance matrix for each channel in Andrius' input file?
  // TH1D* hData_elec;
  // TH1D* hData_mu;
  TH1D* hTheory_;
  TH2D* h2DCov_;

  TMatrixD y_; // y_i = x_i - mu_i
  TMatrixD mCov_;

  void Init() {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

    // -- Data: combined result
    TString fileName_data = rootFilePath + "/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
    // -- convert TH1F to TH1D -- //
    TFile *f_input = TFile::Open( fileName_data );
    f_input->cd();
    TH1F* h_temp = (TH1F*)f_input->Get( "h1Combi" )->Clone();
    hData_ = new TH1D();
    h_temp->Copy( *hData_ );
    hData_->SetTitle("");

    // // -- Data: muon
    // TString fileName_mu = rootFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
    // hData_mu = Get_Hist(fileName_mu, "h_DiffXsec_FSRCorr");

    // // -- Data: electron
    // TString fileName_elec = rootFilePath + "/DiffXsec_Electron_v8.root";
    // hData_elec = Get_Hist(fileName_elec, "h_DiffXSec");


    // -- covariance matrix
    TCanvas *c2DCov = (TCanvas*)f_input->Get("cCovFin")->Clone();
    TPad* pad = (TPad*)c2DCov->FindObject("cCovFin_1");
    h2DCov_ = (TH2D*)pad->GetPrimitive("h2covFin")->Clone();

    // -- Theory
    TString fileName_theory = rootFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
    hTheory_ = Get_Hist( fileName_theory, "h_DiffXsec_FEWZ_NNPDF_NNLO" );

    InitVector();
    InitCovMatrix();
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

      for(Int_t j=0; j<=i; j++) {
        Int_t j_bin = j+1;

        Double_t covValue_ij = h2DCov_->GetBinContent(i_bin, j_bin);

        // printf("(%02d, %02d): cov. = %.3lf\n", i_bin, j_bin, covValue_ij);
        mCov_[i][j] = covValue_ij;
      }
    }
  }
};


void ChiSquareComp() {
  Chi2CompTool *tool = new Chi2CompTool();
  // tool->ValidateCovMatrix();
  // tool->Test_pValueCalc();
  tool->CalcSimpleChi2();
  tool->CalcChi2();
}