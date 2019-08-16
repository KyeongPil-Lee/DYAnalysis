#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString fileName_before_;
  TString fileName_after_;

  TMatrixD* m_dXSec_ll_before_;
  TMatrixD* m_dXSec_ll_after_;

  TMatrixD* m_cov_ll_before_;
  TMatrixD* m_cov_ll_after_;

  TH1D* h_dXSec_ll_before_;
  TH1D* h_dXSec_ll_after_;

  vector<Double_t> vec_massBinEdge_ = {
    15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
    64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
    110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
    200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
    830, 1000, 1500, 3000 };

  PlotProducer(TString fileName_before, TString fileName_after)
  {
    fileName_before_ = fileName_before;
    fileName_after_  = fileName_after;
    Init();
  }

  void Produce()
  {
    DrawDXSec(60, 120);
    DrawDXSec(15, 3000);
  }

private:
  void Init()
  {
    TH1::AddDirectory(kFALSE);

    TFile *f_before = TFile::Open(fileName_before_);
    m_dXSec_ll_before_ = new TMatrixD( *(TMatrixD*)f_before->Get("dXSec_ll") );
    m_cov_ll_before_   = new TMatrixD( *(TMatrixD*)f_before->Get("cov_ll") );
    h_dXSec_ll_before_ = ConvertMatrixToDXSec("ll", m_dXSec_ll_before_, m_cov_ll_before_);
    f_before->Close();

    TFile *f_after = TFile::Open(fileName_after_);
    m_dXSec_ll_after_ = new TMatrixD( *(TMatrixD*)f_after->Get("dXSec_ll") );
    m_cov_ll_after_   = new TMatrixD( *(TMatrixD*)f_after->Get("cov_ll") );
    h_dXSec_ll_after_ = ConvertMatrixToDXSec("ll", m_dXSec_ll_after_, m_cov_ll_after_);
    f_before->Close();
  }

  void DrawDXSec(Double_t mMin, Double_t mMax)
  {
    PlotTool::IsRatio1(h_dXSec_ll_before_, h_dXSec_ll_after_);

    TString canvasName = TString::Format("c_dXSec_before_vs_after_M%.0lfto%.0lf", mMin, mMax);
    Bool_t isLogX = kFALSE;
    if( mMax == 3000 ) isLogX = kTRUE;

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, isLogX, 1);
    canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "Blue/Black");

    canvas->Register( h_dXSec_ll_before_, "# MC for cov. M. (eff.SF. in e channel) = 2k",  kBlack);
    canvas->Register( h_dXSec_ll_after_,  "# MC for cov. M. (eff.SF. in e channel) = 10k", kBlue);

    canvas->SetRangeX(mMin, mMax);

    // canvas->SetRangeY(-10, 100);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);
    canvas->SetRangeRatio(0.9997, 1.0003);

    canvas->SetLegendPosition(0.16, 0.32, 0.70, 0.50);

    canvas->Latex_CMSPre();

    canvas->Draw("EPSAME");
  }

  TH1D* ConvertMatrixToDXSec(TString channel, TMatrixD* m_dXSec, TMatrixD* m_cov )
  {
    TH1D* h_dXSec = CreateHist_MassBinEdge("h_dXSec_"+channel);

    for(Int_t i=0; i<h_dXSec->GetNbinsX(); i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec = (*m_dXSec)[i][0];
      Double_t absUncSq = (*m_cov)[i][i];
      Double_t absUnc = sqrt(absUncSq);

      h_dXSec->SetBinContent(i_bin, dXSec);
      h_dXSec->SetBinError(i_bin, absUnc);

      // printf("[%02dth bin] (dXSec, absUnc) = (%lf, %lf)\n", i_bin, dXSec, absUnc);
    }

    return h_dXSec;
  }

  TH1D* CreateHist_MassBinEdge(TString histName = "h_temp")
  {
    // const Int_t nMassBin = vec_massBinEdge_.size() - 1;
    const Int_t nMassBin = 43;

    Double_t arr_massBinEdge[nMassBin+1] = {0};
    for(Int_t i=0; i<nMassBin+1; i++)
      arr_massBinEdge[i] = vec_massBinEdge_[i];

    return new TH1D(histName, "", nMassBin, arr_massBinEdge);
  }

};

void DrawPlot_dXSec()
{
  TString fileName_before = "../v07_intermediateStepsBLUE/ROOTFile_IntermediateResults_BLUE_FullCorr.root";
  TString fileName_after  = "ROOTFile_IntermediateResults_BLUE_nMC10000.root";

  PlotProducer *producer = new PlotProducer(fileName_before, fileName_after);
  producer->Produce();
}