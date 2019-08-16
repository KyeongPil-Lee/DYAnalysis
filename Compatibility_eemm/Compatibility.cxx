#include <Include/SimplePlotTools.h>
#define nBin 43

class Chi2CalculationTool
{
public:
  Double_t massBinEdges_[nBin+1];

  TH1D* h_dXSec_ee_;
  TH1D* h_dXSec_mm_;

  TH2D* h_cov_ee_;
  TH2D* h_cov_mm_;

  TMatrixD y_; // -- difference between ee and mm
  TMatrixD mCov_ee_;
  TMatrixD mCov_mm_;
  TMatrixD mCov_tot_;

  TH1D* h_absContToChi2_;
  TH1D* h_relContToChi2_;


  Chi2CalculationTool():
  y_(1, nBin),
  mCov_ee_(nBin, nBin),
  mCov_mm_(nBin, nBin),
  mCov_tot_(nBin, nBin)
  {
    Init();
  }

  void Calculation()
  {
    TMatrixD mCovInvert(mCov_tot_);
    mCovInvert.Invert();

    TMatrixD yT(y_);
    yT.Transpose(yT);

    TMatrixD mChi2 = (y_ * mCovInvert) * yT;
    Double_t totalChi2 = mChi2[0][0];
    Double_t normChi2 = totalChi2 / nBin;

    printf("[Calculation] Chi2/ndof = %.5lf/%02d = %.5lf\n", totalChi2, nBin, normChi2);

    Double_t totalChi2_validation = 0;
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

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
               h_dXSec_ee_->GetBinLowEdge(i_bin), h_dXSec_ee_->GetBinLowEdge(i_bin+1), cont_toChi2, ratio);

      h_absContToChi2_->SetBinContent(i_bin, cont_toChi2); h_absContToChi2_->SetBinError(i_bin, 0);
      h_relContToChi2_->SetBinContent(i_bin, ratio);       h_relContToChi2_->SetBinError(i_bin, 0);

      totalChi2_validation += cont_toChi2;
    }

    cout << "  * Validation: total chi2 by matrix multiplication, total chi2 by adding each contribution = " << totalChi2 << ", " << totalChi2_validation << endl;

    TString outputFileName = "ROOTFile_Compatibility_eemm.root";
    TFile *f_output = TFile::Open(outputFileName, "RECREATE");
    f_output->cd();
    h_absContToChi2_->Write();
    h_relContToChi2_->Write();
    f_output->Close();

    PlotTool::Print_Histogram(h_absContToChi2_);

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
    
    // TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    // TString fileName = rootFilePath +"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName = analyzerPath+"/Combination/Converter_BLUEOutput/ROOTFile_hepdata__corr12Aug_Full_newEffSF_Converted.root";

    h_dXSec_ee_ = PlotTool::Get_Hist(fileName, "ee/h_dXSec");
    h_dXSec_mm_ = PlotTool::Get_Hist(fileName, "mm/h_dXSec");

    h_cov_ee_ = PlotTool::Get_Hist2D(fileName, "ee/h_cov_tot");
    h_cov_mm_ = PlotTool::Get_Hist2D(fileName, "mm/h_cov_tot");

    // h_absContToChi2_ = (TH1D*)h_dXSec_mm_->Clone();
    // h_absContToChi2_->Reset("ICES");
    // h_absContToChi2_->SetName("h_absContToChi2");

    // h_relContToChi2_ = (TH1D*)h_dXSec_mm_->Clone();
    // h_relContToChi2_->Reset("ICES");
    // h_relContToChi2_->SetName("h_relContToChi2");

    h_absContToChi2_ = new TH1D("h_absContToChi2", "", nBin, massBinEdges_);
    h_relContToChi2_ = new TH1D("h_relContToChi2", "", nBin, massBinEdges_);

    Init_y();
    Init_mCov("ee");
    Init_mCov("mm");
    mCov_tot_ = mCov_ee_ + mCov_mm_;
    // Print_Cov();
  }

  void Init_y()
  {
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec_ee = h_dXSec_ee_->GetBinContent(i_bin);
      Double_t dXSec_mm = h_dXSec_mm_->GetBinContent(i_bin);

      y_[0][i] = dXSec_ee - dXSec_mm;
    }
  }

  void Init_mCov(TString channel)
  {
    TH2D* h_cov = nullptr;
    if     ( channel == "ee" ) h_cov = (TH2D*)h_cov_ee_->Clone();
    else if( channel == "mm" ) h_cov = (TH2D*)h_cov_mm_->Clone();
    else
    {
      cout << "[Init_mCov] No corresponding covariance matrix input!" << endl;
      return;
    }

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t cov_ij = h_cov->GetBinContent(i_bin, j_bin);

        if     ( channel == "ee" ) mCov_ee_[i][j] = cov_ij;
        else if( channel == "mm" ) mCov_mm_[i][j] = cov_ij;
      }
    }
  }

  void Print_Cov()
  {
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t cov_ee_ij = mCov_ee_[i][j];
        Double_t cov_mm_ij = mCov_mm_[i][j];
        Double_t cov_tot_ij = mCov_tot_[i][j];

        printf("(i, j) = (%02d, %02d) -> cov(ee, mm, tot) = (%.5lf, %.5lf, %.5lf)\n", i, j, cov_ee_ij, cov_mm_ij, cov_tot_ij);
      }
    }
  }

};

void Compatibility()
{
  Chi2CalculationTool* tool = new Chi2CalculationTool();
  tool->Calculation();
}