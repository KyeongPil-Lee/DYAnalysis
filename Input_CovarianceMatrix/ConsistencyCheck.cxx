#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>

class CrossCheckTool
{
public:
  // -- from the ROOT file used for the paper
  TH1D* h_dXSec_ref_;
  TH1D* h_relUnc_stat_ref_;
  TH1D* h_relUnc_bkg_ref_;
  TH1D* h_relUnc_detRes_ref_;
  TH1D* h_relUnc_effSF_ref_;
  TH1D* h_relUnc_FSR_ref_;
  TH1D* h_relUnc_acc_ref_;

  // -- resutls before smoothing
  TH1D* h_relUnc_bkg_noSmooth_ref_;
  TH1D* h_relUnc_detRes_noSmooth_ref_;
  TH1D* h_relUnc_FSR_noSmooth_ref_;

  // -- from the ROOT file provided to Andrius for covariance matrices & combination
  TH1D* h_dXSec_input_;

  TH1D* h_relUnc_stat_input_;

  TH1D* h_relUnc_bkg_stat_input_;
  TH1D* h_relUnc_bkg_syst_input_;
  TH1D* h_relUnc_bkg_input_;

  TH1D* h_relUnc_detRes_stat_input_;
  TH1D* h_relUnc_detRes_syst_input_;
  TH1D* h_relUnc_detRes_input_;

  TH1D* h_relUnc_effSF_stat_input_;
  TH1D* h_relUnc_effSF_syst_input_;
  TH1D* h_relUnc_effSF_input_;

  TH1D* h_relUnc_FSR_stat_input_;
  TH1D* h_relUnc_FSR_syst_input_;
  TH1D* h_relUnc_FSR_input_;

  TH1D* h_relUnc_acc_input_;

  CrossCheckTool()
  {
    Init();
  }

  void CrossCheck()
  {
    Comparison_ref_vs_input("dXSec", h_dXSec_ref_, h_dXSec_input_);
    Comparison_ref_vs_input("relUnc_stat", h_relUnc_stat_ref_, h_relUnc_stat_input_);
    Comparison_ref_vs_input("relUnc_bkg", h_relUnc_bkg_ref_, h_relUnc_bkg_input_);
    Comparison_ref_vs_input("relUnc_detRes", h_relUnc_detRes_ref_, h_relUnc_detRes_input_);
    Comparison_ref_vs_input("relUnc_effSF", h_relUnc_effSF_ref_, h_relUnc_effSF_input_);
    Comparison_ref_vs_input("relUnc_FSR", h_relUnc_FSR_ref_, h_relUnc_FSR_input_);
    Comparison_ref_vs_input("relUnc_acc", h_relUnc_acc_ref_, h_relUnc_acc_input_);

    Comparison_ref_vs_input("relUnc_bkg    (before smoothing)", h_relUnc_bkg_noSmooth_ref_,    h_relUnc_bkg_input_);
    Comparison_ref_vs_input("relUnc_detRes (before smoothing)", h_relUnc_detRes_noSmooth_ref_, h_relUnc_detRes_input_);
    Comparison_ref_vs_input("relUnc_FSR    (before smoothing)", h_relUnc_FSR_noSmooth_ref_,    h_relUnc_FSR_input_);
  }


private:
  void Comparison_ref_vs_input(TString type, TH1D* h_ref, TH1D* h_input)
  {
    cout << "[Comparison_ref_vs_input] type = " << type << endl;

    Int_t nBin = h_ref->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t ref   = h_ref->GetBinContent(i_bin);
      Double_t input = h_input->GetBinContent(i_bin);
      Double_t relDiff = (input-ref) / ref;

      printf("[%02d bin] (ref, input, relDiff) = (%.10e, %.10e, %.10lf)\n", i_bin, ref, input, relDiff);
      if( fabs(relDiff) > 0.001 )
        cout << " ---> relDiff > 0.1%!!" << endl;
    }

    cout << endl;
  }

  void Init()
  {
    // -- from the files for the paper
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName_ref = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root";

    h_dXSec_ref_         = Get_Hist(fileName_ref, "h_dXSec");
    h_relUnc_stat_ref_   = Get_Hist(fileName_ref, "h_relUnc_stat");
    h_relUnc_bkg_ref_    = Get_Hist(fileName_ref, "h_relUnc_syst_bkg");
    h_relUnc_detRes_ref_ = Get_Hist(fileName_ref, "h_relUnc_syst_detRes");
    h_relUnc_effSF_ref_  = Get_Hist(fileName_ref, "h_relUnc_syst_eff");
    h_relUnc_FSR_ref_    = Get_Hist(fileName_ref, "h_relUnc_syst_FSR");
    h_relUnc_acc_ref_    = Get_Hist(fileName_ref, "h_relUnc_syst_accPDF");

    // -- convert % to nominal relative values
    h_relUnc_stat_ref_->Scale(0.01);
    h_relUnc_bkg_ref_->Scale(0.01);
    h_relUnc_detRes_ref_->Scale(0.01);
    h_relUnc_effSF_ref_->Scale(0.01);
    h_relUnc_FSR_ref_->Scale(0.01);
    h_relUnc_acc_ref_->Scale(0.01);

    // -- results before smoothing
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    h_relUnc_bkg_noSmooth_ref_    = Get_Hist(rootFilePath+"/ROOTFile_SysUnc_BkgEst.root", "h_RelUnc_Tot_Total");
    h_relUnc_detRes_noSmooth_ref_ = Get_Hist(rootFilePath+"/ROOTFile_SysUnc_DetRes.root", "h_RelSysUnc_DetRes_Percent");
    h_relUnc_FSR_noSmooth_ref_    = Get_Hist(rootFilePath+"/ROOTFile_SysUnc_FSR.root",    "h_SysUnc_Tot_Percent");

    h_relUnc_bkg_noSmooth_ref_->Scale(0.01);
    h_relUnc_detRes_noSmooth_ref_->Scale(0.01);
    h_relUnc_FSR_noSmooth_ref_->Scale(0.01);


    // -- from input file for covariance matrices & combination
    TString fileName_input = "ROOTFile_Input_Cov.root";

    h_dXSec_input_ = Get_Hist(fileName_input, "h_DiffXSec");

    h_relUnc_stat_input_ = Get_Hist(fileName_input, "h_RelStatUnc");

    h_relUnc_bkg_stat_input_ = Get_Hist(fileName_input, "h_RelUnc_Stat_BkgEst");
    h_relUnc_bkg_syst_input_ = Get_Hist(fileName_input, "h_RelUnc_Syst_BkgEst");
    vector<TH1D*> vec_hist_bkg = {h_relUnc_bkg_stat_input_, h_relUnc_bkg_syst_input_};
    h_relUnc_bkg_input_ = QuadSumHist("h_relUnc_bkg_input", vec_hist_bkg);

    h_relUnc_detRes_stat_input_ = Get_Hist(fileName_input, "h_RelUnc_Stat_DetRes");
    h_relUnc_detRes_syst_input_ = Get_Hist(fileName_input, "h_RelUnc_Syst_DetRes");
    vector<TH1D*> vec_hist_detRes = {h_relUnc_detRes_stat_input_, h_relUnc_detRes_syst_input_};
    h_relUnc_detRes_input_ = QuadSumHist("h_relUnc_detRes_input", vec_hist_detRes);

    h_relUnc_effSF_stat_input_ = Get_Hist(fileName_input, "h_RelUnc_Stat_EffSF");
    h_relUnc_effSF_syst_input_ = Get_Hist(fileName_input, "h_RelUnc_Syst_EffSF");
    vector<TH1D*> vec_hist_effSF = {h_relUnc_effSF_stat_input_, h_relUnc_effSF_syst_input_};
    h_relUnc_effSF_input_ = QuadSumHist("h_relUnc_effSF_input", vec_hist_effSF);

    h_relUnc_FSR_stat_input_ = Get_Hist(fileName_input, "h_RelUnc_Stat_FSR");
    h_relUnc_FSR_syst_input_ = Get_Hist(fileName_input, "h_RelUnc_Syst_FSR");
    vector<TH1D*> vec_hist_FSR = {h_relUnc_FSR_stat_input_, h_relUnc_FSR_syst_input_};
    h_relUnc_FSR_input_ = QuadSumHist("h_relUnc_FSR_input", vec_hist_FSR);

    h_relUnc_acc_input_ = Get_Hist(fileName_input, "h_RelUnc_Syst_Acc");
  }

  TH1D* QuadSumHist( TString histName, vector<TH1D*> vec_hist )
  {
    TH1D* h_quadSum = (TH1D*)vec_hist[0]->Clone();
    // TString histName = "h_"+dataType_+"_relUnc_"+uncType;
    h_quadSum->SetName(histName);

    Int_t nHist = (Int_t)vec_hist.size();

    Int_t nBin = vec_hist[0]->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t quadSumSq = 0;
      for(Int_t i_hist=0; i_hist<nHist; i_hist++)
      {
        Double_t binContent = vec_hist[i_hist]->GetBinContent(i_bin);
        quadSumSq += binContent*binContent;
      }

      Double_t quadSum = sqrt(quadSumSq);

      h_quadSum->SetBinContent(i_bin, quadSum);
      h_quadSum->SetBinError(i_bin, 0);
    }

    return h_quadSum;
  }

};

void ConsistencyCheck()
{
  CrossCheckTool* tool = new CrossCheckTool();
  tool->CrossCheck();
}