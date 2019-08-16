#include <Include/SimplePlotTools.h>


class LambdaInfo
{
public:
  TString type_;

  TMatrixD* lambda_;
  TMatrixD* dXSec_eemm_; // -- 0-42 row: ee, 43-85 row: mm

  TMatrixD* dXSec_ee_;
  TMatrixD* dXSec_mm_;
  TMatrixD* dXSec_ll_;

  TMatrixD* cov_ee_;
  TMatrixD* cov_mm_;
  TMatrixD* cov_ll_;

  TString fileName_;

  vector<Double_t> vec_massBinEdge_ = {
    15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
    64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
    110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
    200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
    830, 1000, 1500, 3000 };

  TH1D* h_dXSec_ee_;
  TH1D* h_dXSec_mm_;
  TH1D* h_dXSec_ll_;

  Int_t binIndex_;

  // -- lambda distribution in 1D for the binIndex bin
  TH1D* h_lambda_ee_;
  TH1D* h_lambda_mm_;

  LambdaInfo(TString type, TString fileName, Int_t binIndex)
  {
    type_ = type;
    fileName_ = fileName;
    binIndex_ = binIndex;
    Init();
  }

  void DrawPlot()
  {
    DrawLambda1D();
    DrawLambdaTimesDXSec1D();
    PrintLambdaDXSec();
  }

private:
  void Init()
  {
    gStyle->SetPalette(1);

    TFile *f_input = TFile::Open(fileName_);

    lambda_     = new TMatrixD( *(TMatrixD*)f_input->Get("lambda") );
    dXSec_eemm_ = new TMatrixD( *(TMatrixD*)f_input->Get("dXSec_eemm") );
    // dXSec_ll_   = new TMatrixD((*lambda_)*(*dXSec_eemm_));

    dXSec_ee_ = new TMatrixD( *(TMatrixD*)f_input->Get("dXSec_ee") );
    dXSec_mm_ = new TMatrixD( *(TMatrixD*)f_input->Get("dXSec_mm") );
    dXSec_ll_ = new TMatrixD( *(TMatrixD*)f_input->Get("dXSec_ll") );

    cov_ee_ = new TMatrixD( *(TMatrixD*)f_input->Get("cov_ee") );
    cov_mm_ = new TMatrixD( *(TMatrixD*)f_input->Get("cov_mm") );
    cov_ll_ = new TMatrixD( *(TMatrixD*)f_input->Get("cov_ll") );

    h_dXSec_ee_ = ConvertMatrixToDXSec("ee", dXSec_ee_, cov_ee_);
    h_dXSec_mm_ = ConvertMatrixToDXSec("mm", dXSec_mm_, cov_mm_);
    h_dXSec_ll_ = ConvertMatrixToDXSec("ll", dXSec_ll_, cov_ll_);

    h_lambda_ee_ = LambdaCoeff_OneBin("ee");
    h_lambda_mm_ = LambdaCoeff_OneBin("mm");
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

  void PrintLambdaDXSec()
  {
    cout << "[PrintLambdaDXSec] bin index = " << binIndex_ << endl;

    Double_t dXSec_ll = 0;
    for(Int_t i=0; i<h_lambda_ee_->GetNbinsX(); i++)
    {
      Int_t i_bin = i+1;

      Double_t minM = vec_massBinEdge_[i];
      Double_t maxM = vec_massBinEdge_[i+1];

      Double_t lambda_ee = h_lambda_ee_->GetBinContent(i_bin);
      Double_t lambda_mm = h_lambda_mm_->GetBinContent(i_bin);

      Double_t dXSec_ee = h_dXSec_ee_->GetBinContent(i_bin);
      Double_t dXSec_mm = h_dXSec_mm_->GetBinContent(i_bin);

      printf("[%02d bin: %4.0lf < M < %4.0lf]\n", i_bin, minM, maxM);
      printf("  [ee channel] (lambda, dsigma/dm, lambda*dsigma/dm) = (%lf, %lf, %lf)\n", lambda_ee, dXSec_ee, lambda_ee*dXSec_ee);
      printf("  [mm channel] (lambda, dsigma/dm, lambda*dsigma/dm) = (%lf, %lf, %lf)\n", lambda_mm, dXSec_mm, lambda_mm*dXSec_mm);
      printf("    [Sum of lambda*dsigma/dm] %lf\n", lambda_ee*dXSec_ee + lambda_mm*dXSec_mm);

      dXSec_ll += lambda_ee*dXSec_ee + lambda_mm*dXSec_mm;
    }

    printf("dXSec_ll = %lf\n", dXSec_ll);
    printf("[PrintLambdaDXSec: Done]\n");
  }

  void DrawLambda1D() // -- binIndex: start from 1
  {
    TString canvasName = "c_lambda1D_"+TString::Format("%dbinIndex_%s", binIndex_, type_.Data());
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);
    canvas->SetTitle("Bin index (1-43)", "Coefficient (#lambda)");

    canvas->Register( h_lambda_ee_, "coefficients on ee result",     kGreen+2);
    canvas->Register( h_lambda_mm_, "coefficients on #mu#mu result", kBlue);

    // canvas->SetRangeY(-1e4, 1e4);
    // canvas->SetRangeY(-1, 1);
    canvas->SetRangeY(-7, 7);
    if( binIndex_ == 41 ) canvas->SetRangeY(-1e-6, 1e-6);
    if( binIndex_ == 42 ) canvas->SetRangeY(-1e-6, 1e-6);
    if( binIndex_ == 43 ) canvas->SetRangeY(-1e-7, 1e-7);

    canvas->SetLegendPosition(0.50, 0.82, 0.95, 0.95);

    // canvas->Latex_CMSPre();
    TString massRangeInfo = TString::Format("%.0lf < M < %.0lf GeV bin", vec_massBinEdge_[binIndex_-1], vec_massBinEdge_[binIndex_]);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.7]{"+massRangeInfo+"}}");

    canvas->Draw("HISTLPSAME");
  }

  void DrawLambdaTimesDXSec1D()
  {
    TH1D* h_ee = MultiplyTwoHists(h_lambda_ee_, h_dXSec_ee_);
    TH1D* h_mm = MultiplyTwoHists(h_lambda_mm_, h_dXSec_mm_);

    TString canvasName = "c_lambdaTimesDXSec1D_"+TString::Format("%dbinIndex_%s", binIndex_, type_.Data());
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);
    canvas->SetTitle("Bin index (1-43)", "#lambda #times (d#sigma/dm) [pb/GeV]");

    canvas->Register( h_ee, "ee contribution (#lambda#timesd#sigma/dm) from each bin", kGreen+2);
    canvas->Register( h_mm, "#mu#mu contribution (#lambda#timesd#sigma/dm) from each bin", kBlue);

    canvas->SetRangeY(-15, 15);
    if( binIndex_ == 14 ) canvas->SetRangeY(-2, 10);
    if( binIndex_ == 15 ) canvas->SetRangeY(-2, 20);
    if( binIndex_ == 16 ) canvas->SetRangeY(-10, 90);
    if( binIndex_ == 18 ) canvas->SetRangeY(-2, 10);
    if( binIndex_ == 19 ) canvas->SetRangeY(-1, 5);
    if( binIndex_ == 41 ) canvas->SetRangeY(-3e-6, 3e-6);
    if( binIndex_ == 42 ) canvas->SetRangeY(-3e-6, 3e-6);
    if( binIndex_ == 43 ) canvas->SetRangeY(-1e-7, 3e-7);
    // canvas->SetRangeY(-20, 120);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);

    canvas->SetLegendPosition(0.40, 0.82, 0.95, 0.95);

    // canvas->Latex_CMSPre();
    TString massRangeInfo = TString::Format("%.0lf < M < %.0lf GeV bin", vec_massBinEdge_[binIndex_-1], vec_massBinEdge_[binIndex_]);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.7]{"+massRangeInfo+"}}");

    canvas->Draw("HISTLPSAME");

    DrawLambdaTimesDXSec1D_Sum_eemm(h_ee, h_mm);
  }

  void DrawLambdaTimesDXSec1D_Sum_eemm(TH1D* h_ee, TH1D* h_mm)
  {
    TH1D* h_sum = SumTwoHists(h_ee, h_mm, "h_sum");

    TString canvasName = "c_lambdaTimesDXSec1D_Sum_eemm_"+TString::Format("%dbinIndex_%s", binIndex_, type_.Data());
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);
    canvas->SetTitle("Bin index (1-43)", "#lambda #times (d#sigma/dm) [pb/GeV]");

    canvas->Register( h_sum, "ee+mm contribution (#lambda#timesd#sigma/dm) from each bin", kBlack);

    canvas->SetRangeY(-2, 0.5);
    if( binIndex_ == 14 ) canvas->SetRangeY(-0.1, 0.1);
    if( binIndex_ == 15 ) canvas->SetRangeY(-0.1, 0.1);
    if( binIndex_ == 16 ) canvas->SetRangeY(-1, 0.5);
    if( binIndex_ == 18 ) canvas->SetRangeY(-0.2, 0.1);
    if( binIndex_ == 19 ) canvas->SetRangeY(-0.1, 0.1);
    if( binIndex_ == 41 ) canvas->SetRangeY(-4e-7, 4e-7);
    if( binIndex_ == 42 ) canvas->SetRangeY(-3e-7, 3e-7);
    if( binIndex_ == 43 ) canvas->SetRangeY(-1e-8, 1e-8);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);

    canvas->SetLegendPosition(0.30, 0.15, 0.95, 0.30);

    // canvas->Latex_CMSPre();
    TString massRangeInfo = TString::Format("%.0lf < M < %.0lf GeV bin", vec_massBinEdge_[binIndex_-1], vec_massBinEdge_[binIndex_]);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.7]{"+massRangeInfo+"}}");

    canvas->Draw("HISTLPSAME");
  }

  TH1D* MultiplyTwoHists(TH1D* h1, TH1D* h2, TString histName = "h_temp" )
  {
    if( h1->GetNbinsX() != h2->GetNbinsX() )
    {
      cout << "[MultiplyTwoHists] h1->GetNbinsX() != h2->GEtNbinsX()" << endl;
      return nullptr;
    }

    TH1D* h_return = (TH1D*)h1->Clone();
    h_return->SetName(histName);

    for(Int_t i=0; i<h1->GetNbinsX(); i++)
    {
      Int_t i_bin = i+1;

      Double_t value1 = h1->GetBinContent(i_bin);
      Double_t value2 = h2->GetBinContent(i_bin);

      h_return->SetBinContent(i_bin, value1*value2);
      h_return->SetBinError(i_bin, 0); // -- set error to 0 for all bins
    }

    h_return->SetBinContent(0, 0);
    h_return->SetBinContent(h1->GetNbinsX()+1, 0);

    return h_return;
  }

  TH1D* LambdaCoeff_OneBin(TString channel) // -- binIndex: start from 1
  {
    Int_t nBin = lambda_->GetNrows();

    TH1D* h_lambda = new TH1D("h_lambda_"+channel, "", nBin, 0, nBin);

    Int_t i_start = 0;
    if( channel == "mm" ) i_start = 43;

    Int_t i_bin = 1;
    for(Int_t i_col=i_start; i_col<i_start+nBin; i_col++)
    {
      Double_t lambda = (*lambda_)[binIndex_-1][i_col];

      h_lambda->SetBinContent(i_bin, lambda);
      h_lambda->SetBinError(i_bin, 0);
      i_bin++;
    }

    return h_lambda;
  }

  TH1D* SumTwoHists(TH1D* h1, TH1D* h2, TString histName = "h_temp" )
  {
    if( h1->GetNbinsX() != h2->GetNbinsX() )
    {
      cout << "[SumTwoHists] h1->GetNbinsX() != h2->GEtNbinsX()" << endl;
      return nullptr;
    }

    TH1D* h_return = (TH1D*)h1->Clone();
    h_return->SetName(histName);

    for(Int_t i=0; i<h1->GetNbinsX(); i++)
    {
      Int_t i_bin = i+1;

      Double_t value1 = h1->GetBinContent(i_bin);
      Double_t value2 = h2->GetBinContent(i_bin);

      h_return->SetBinContent(i_bin, value1+value2);
      h_return->SetBinError(i_bin, 0); // -- set error to 0 for all bins
    }

    h_return->SetBinContent(0, 0);
    h_return->SetBinContent(h1->GetNbinsX()+1, 0);

    return h_return;
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

class PlotProducer
{
public:
  TString fileName_fullCorr_;
  TString fileName_noCorrEffSFe_;
  TString fileName_noCorr_;

  LambdaInfo* lambdaInfo_fullCorr_;
  LambdaInfo* lambdaInfo_noCorrEffSFe_;
  LambdaInfo* lambdaInfo_noCorr_;

  Int_t binIndex_;

  PlotProducer(TString fileName_fullCorr, TString fileName_noCorrEffSFe, TString fileName_noCorr, Int_t binIndex )
  {
    fileName_fullCorr_     = fileName_fullCorr;
    fileName_noCorrEffSFe_ = fileName_noCorrEffSFe;
    fileName_noCorr_       = fileName_noCorr;

    binIndex_ = binIndex;

    lambdaInfo_fullCorr_     = new LambdaInfo("fullCorr",     fileName_fullCorr_,     binIndex_);
    lambdaInfo_noCorrEffSFe_ = new LambdaInfo("noCorrEffSFe", fileName_noCorrEffSFe_, binIndex_);
    lambdaInfo_noCorr_       = new LambdaInfo("noCorr",       fileName_noCorr_,       binIndex_);
  }

  void SetBinIndex(Int_t i_bin)
  {
    binIndex_ = i_bin;
  }

  void Produce()
  {
    if( binIndex_ == -1 )
    {
      cout << "binIndex_ = -1" << endl;
      return;
    }

    lambdaInfo_fullCorr_->DrawPlot();
    lambdaInfo_noCorrEffSFe_->DrawPlot();
    lambdaInfo_noCorr_->DrawPlot();
  }

private:


};

void DrawPlots_Lambda()
{
  TString fileName_fullCorr = "../ROOTFile_IntermediateResults_BLUE_FullCorr.root";
  TString fileName_noCorrEffSFe = "../ROOTFile_IntermediateResults_BLUE_NoCorr_eEffSF.root";
  TString fileName_noCorr = "../ROOTFile_IntermediateResults_BLUE_NoCorr.root";
  Int_t binIndex = 41;

  PlotProducer *producer = new PlotProducer(fileName_fullCorr, fileName_noCorrEffSFe, fileName_noCorr, binIndex);
  producer->Produce();
}
