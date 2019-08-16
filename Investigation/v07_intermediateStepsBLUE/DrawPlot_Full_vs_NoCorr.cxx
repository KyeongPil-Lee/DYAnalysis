#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString fileName_fullCorr_;
  TString fileName_noCorr_;

  TMatrixD* dXSec_ll_fullCorr_;
  TMatrixD* dXSec_ll_noCorr_;

  TMatrixD* cov_ll_fullCorr_;
  TMatrixD* cov_ll_noCorr_;

  TH1D* h_dXSec_ll_fullCorr_;
  TH1D* h_dXSec_ll_noCorr_;

  vector<Double_t> vec_massBinEdge_ = {
    15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
    64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
    110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
    200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
    830, 1000, 1500, 3000 };

  PlotProducer(TString fileName_fullCorr, TString fileName_noCorr)
  {
    fileName_fullCorr_ = fileName_fullCorr;
    fileName_noCorr_ = fileName_noCorr;

    Init();
  }

  void Produce()
  {
    DrawDXSec(15, 3000);
    DrawDXSec(60, 120);
  }

private:
  void DrawDXSec(Double_t mMin, Double_t mMax)
  {
    TString canvasName = TString::Format("c_dXSec_fullCorr_vs_noCorr_M%.0lfto%.0lf", mMin, mMax);
    Bool_t isLogX = kFALSE;
    if( mMax == 3000 ) isLogX = kTRUE;

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, isLogX, 1);
    canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "Full/no corr.");

    canvas->Register( h_dXSec_ll_noCorr_,   "No correlation",   kBlack);
    canvas->Register( h_dXSec_ll_fullCorr_, "Full correlation", kBlue);

    canvas->SetRangeX(mMin, mMax);

    // canvas->SetRangeY(-10, 100);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);
    canvas->SetRangeRatio(0.94, 1.06);

    canvas->SetLegendPosition(0.60, 0.82, 0.95, 0.95);

    canvas->Latex_CMSPre();

    canvas->Draw("EPSAME");
  }

  void Init()
  {
    TFile* f_noCorr = TFile::Open(fileName_noCorr_);
    TFile* f_fullCorr = TFile::Open(fileName_fullCorr_);

    f_noCorr->cd();
    dXSec_ll_noCorr_ = new TMatrixD( *(TMatrixD*)f_noCorr->Get("dXSec_ll") );
    cov_ll_noCorr_ = new TMatrixD( *(TMatrixD*)f_noCorr->Get("cov_ll") );

    f_fullCorr->cd();
    dXSec_ll_fullCorr_ = new TMatrixD( *(TMatrixD*)f_fullCorr->Get("dXSec_ll") );
    cov_ll_fullCorr_ = new TMatrixD( *(TMatrixD*)f_fullCorr->Get("cov_ll") );

    f_noCorr->Close();
    f_fullCorr->Close();

    h_dXSec_ll_noCorr_ = ConvertMatrixToDXSec("h_dXSec_ll_noCorr", dXSec_ll_noCorr_, cov_ll_noCorr_);
    h_dXSec_ll_fullCorr_ = ConvertMatrixToDXSec("h_dXSec_ll_fullCorr", dXSec_ll_fullCorr_, cov_ll_fullCorr_);

    printf("[Combined Z cross section] (no corr, full corr) = (%lf, %lf)\n", ZCrossSection(h_dXSec_ll_noCorr_), ZCrossSection(h_dXSec_ll_fullCorr_));
    PrintCrossSectionNearZ();
  }

  Double_t ZCrossSection(TH1D* h_dXSec)
  {
    Double_t xSec_Z = 0;

    for(Int_t i=0; i<h_dXSec->GetNbinsX(); i++)
    {
      Int_t i_bin = i+1;
      Double_t binCenter = h_dXSec->GetBinCenter(i_bin);

      if( 60 < binCenter && binCenter < 120 )
      {
        Double_t dXSec = h_dXSec->GetBinContent(i_bin);
        Double_t binWidth = h_dXSec->GetBinWidth(i_bin);

        Double_t xSec = dXSec * binWidth;
        xSec_Z += xSec;
      }
      else if( binCenter > 120 )
        break;
    }

    return xSec_Z;
  }

  void PrintCrossSectionNearZ()
  {
    for(Int_t i=0; i<h_dXSec_ll_noCorr_->GetNbinsX(); i++)
    {
      Int_t i_bin = i+1;
      Double_t binCenter = h_dXSec_ll_noCorr_->GetBinCenter(i_bin);

      if( 60 < binCenter && binCenter < 120 )
      {
        Double_t binWidth = h_dXSec_ll_noCorr_->GetBinWidth(i_bin);

        Double_t binEdge_low  = h_dXSec_ll_noCorr_->GetBinLowEdge(i_bin);
        Double_t binEdge_high = h_dXSec_ll_noCorr_->GetBinLowEdge(i_bin+1);

        Double_t dXSec_ll_noCorr   = h_dXSec_ll_noCorr_->GetBinContent(i_bin);
        Double_t dXSec_ll_fullCorr = h_dXSec_ll_fullCorr_->GetBinContent(i_bin);

        Double_t xSec_ll_noCorr   = dXSec_ll_noCorr * binWidth;
        Double_t xSec_ll_fullCorr = dXSec_ll_fullCorr * binWidth;

        Double_t absDiff = xSec_ll_noCorr - xSec_ll_fullCorr;
        Double_t relDiff = absDiff / xSec_ll_fullCorr;

        printf("[%02d bin: %3.0lf < M < %3.0lf] xSec: (noCorr, fullCorr, absDiff, relDiff) = (%.3lf, %.3lf, %.3lf, %.5lf)\n", 
          i_bin, binEdge_low, binEdge_high, xSec_ll_noCorr, xSec_ll_fullCorr, absDiff, relDiff);
      }

    }

  }

  TH1D* ConvertMatrixToDXSec(TString histName, TMatrixD* m_dXSec, TMatrixD* m_cov )
  {
    TH1D* h_dXSec = CreateHist_MassBinEdge(histName);

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

void DrawPlot_Full_vs_NoCorr()
{
  TString fileName_fullCorr = "ROOTFile_IntermediateResults_BLUE_FullCorr.root";
  TString fileName_noCorr = "ROOTFile_IntermediateResults_BLUE_NoCorr.root";

  PlotProducer *producer = new PlotProducer(fileName_fullCorr, fileName_noCorr);
  producer->Produce();

}