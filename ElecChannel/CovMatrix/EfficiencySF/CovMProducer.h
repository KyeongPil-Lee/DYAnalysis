#include <ElecChannel/DXSec/DXSecProducer.h>
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
    uncType_ = uncType;
  }

  void Produce()
  {
    MakeCovM();

    TString histName_corr = h_cov_->GetName();
    histName_corr.ReplaceAll("h_cov", "h_corr");
    h_corr_ = DYTool::Convert_CovMToCorrM(h_cov_, histName_corr);

    Save();
  }

private:
  void Init()
  {
    h_cov_ = DYTool::MakeHist2D_DXSecBin("h_cov_"+uncType_);
    Generated_SmearedDXSec();
  }

  void MakeCovM()
  {
    Int_t nBin = h_dXSec_cv_->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec_cv_i = h_dXSec_cv_->GetBinContent(i_bin);
      Double_t dXSec_cv_j = h_dXSec_cv_->GetBinContent(j_bin);


      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t sum_ij = 0;
        for(Int_t i_map=0; i_map<nEffMap; i_map++)
        {
          Double_t dXSec_i = vec_hist_smearedDXSec_[i_map]->GetBinContent(i_bin);
          Double_t dXSec_j = vec_hist_smearedDXSec_[i_map]->GetBinContent(j_bin);

          sum_ij += dXSec_i*dXSec_j;
        }

        Double_t cov = (sum_ij - nEffMap*dXSec_cv_i*dXSec_cv_j) / (nEffMap - 1); // -- unbiased estimator for cov

        h_cov_->SetBinContent(i_bin, j_bin, cov);
        h_cov_->SetBinError(i_bin, j_bin, 0);

      } // -- j iteration
    } // -- i iteration
  }

  void Generated_SmearedDXSec()
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName_effSF = analyzerPath+"/ElecChannel/Uncertainties/EfficiencySF/ROOTFile_SmearedEffSF_perMassBin_"+uncType_+".root";

    h_effSF_cv = PlotTool::Get_Hist( fileName_effSF, "h_effSF_perMassBin_cv" );
    DXSecProducer* dXSecProducer_cv = new DXSecProducer();
    dXSecProducer_cv->UpdateEffSF( h_effSF_cv );
    dXSecProducer_cv->Produce();
    h_dXSec_cv_ = dXSecProducer_cv->DXSecHist();
    h_dXSec_cv_->SetName("h_dXSec_cv");

    for(Int_t i=0; i<nEffMap; i++)
    {
      TString numbering = TString::Format("%d", i);
      histName_smearedEffSF = "h_effSF_perMassBin_smeared_"+numbering;
      h_smearedEffSF = PlotTool::Get_Hist( fileName_effSF, histName_smearedEffSF );

      DXSecProducer* dXSecProducer = new DXSecProducer();
      dXSecProducer->UpdateEffSF( h_smearedEffSF );
      dXSecProducer->Produce();
      TH1D* h_dXSec_temp = dXSecProducer->DXSecHist();
      h_dXSec_temp->SetName("h_dXSec_smeared_"+numbering);
      vec_hist_smearedDXSec_.push_back( h_dXSec_temp );
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
