#include <ElecChannel/Include/DYElectronTool.h>

#define nEffMap 2000

class CovMProducer
{
public:
  TString uncType_;

  TH1D* h_dXSec_cv_;
  vector<TH1D*> vec_hist_smearedDXSec_;

  TH2D* h_cov_;
  TH2D* h_corr_;

  CovMProducer(TString uncType)
  {
    cout << "[CovMProducer] uncType = " << uncType << endl;
    uncType_ = uncType;
    Init();
  }

  void Produce()
  {
    MakeCovM();

    TString histName_corr = h_cov_->GetName();
    histName_corr.ReplaceAll("h_cov", "h_corr");
    h_corr_ = DYTool::Convert_CovMToCorrM(h_cov_, histName_corr);

    Save();
  }

  void Validation()
  {
    Validation_CV_MeanOfSmearedDXSec();

  }

private:
  void Validation_CV_MeanOfSmearedDXSec()
  {
    Int_t nBin = h_dXSec_cv_->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec_cv = h_dXSec_cv_->GetBinContent(i_bin);

      Double_t sum_dXSec = 0;
      for(Int_t i_map=0; i_map<nEffMap; i_map++)
      {
        Double_t dXSec_i = vec_hist_smearedDXSec_[i_map]->GetBinContent(i_bin);
        sum_dXSec += dXSec_i;
      }

      Double_t mean_dXSec = sum_dXSec / nEffMap;

      Double_t relDiff = (mean_dXSec - dXSec_cv ) / dXSec_cv;

      printf("[%d bin] (central value, mean) = (%lf, %lf) -> rel.diff = %lf\n", i_bin, dXSec_cv, mean_dXSec, relDiff);
    }
  }

  void Init()
  {
    h_cov_ = DYTool::MakeHist2D_DXSecBin("h_cov_"+uncType_);
    Get_SmearedDXSec();
  }

  void MakeCovM()
  {
    cout << "[CovMProducer::MakeCovM] start" << endl;
    Int_t nBin = h_dXSec_cv_->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t dXSec_cv_i = h_dXSec_cv_->GetBinContent(i_bin);
        Double_t dXSec_cv_j = h_dXSec_cv_->GetBinContent(j_bin);

        Double_t sum_diff_ij = 0;
        for(Int_t i_map=0; i_map<nEffMap; i_map++)
        {
          Double_t dXSec_i = vec_hist_smearedDXSec_[i_map]->GetBinContent(i_bin);
          Double_t dXSec_j = vec_hist_smearedDXSec_[i_map]->GetBinContent(j_bin);

          Double_t diff_i = (dXSec_i - dXSec_cv_i);
          Double_t diff_j = (dXSec_j - dXSec_cv_j);

          sum_diff_ij += diff_i*diff_j;
        }

        Double_t N = nEffMap;

        Double_t cov = sum_diff_ij / (N - 1); // -- unbiased estimator for cov

        // if( i == j )
        //   cout << "i = j = " << i << " -> cov = " << cov << endl;

        h_cov_->SetBinContent(i_bin, j_bin, cov);
        h_cov_->SetBinError(i_bin, j_bin, 0);

      } // -- j iteration
    } // -- i iteration

    cout << "[CovMProducer::MakeCovM] end" << endl;
    cout << endl;
  }

  void Get_SmearedDXSec()
  {
    TString fileName = "ROOTFile_SmearedDXSecProducer_"+uncType_+".root";
    h_dXSec_cv_ = PlotTool::Get_Hist(fileName, "h_dXSec_cv");

    for(Int_t i=0; i<nEffMap; i++)
    {
      TString numbering = TString::Format("%d", i);
      TString histName_smearedDXSec = "h_dXSec_smeared_"+numbering;

      TH1D* h_dXSec_smeared = PlotTool::Get_Hist(fileName, histName_smearedDXSec);
      vec_hist_smearedDXSec_.push_back( h_dXSec_smeared );
    }
  }

  void Save()
  {
    TFile *f_output = TFile::Open("ROOTFile_CovMProducer_"+uncType_+".root", "RECREATE");
    f_output->cd();

    h_cov_->Write();
    h_corr_->Write();

    h_dXSec_cv_->Write();
    for(const auto& h_dXSec : vec_hist_smearedDXSec_ )
      h_dXSec->Write();

    f_output->Close();
  }
};
