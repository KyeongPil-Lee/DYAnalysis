#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
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

  Int_t theBinIndex_;

  PlotProducer(TString fileName)
  {
    fileName_ = fileName;
    Init();
  }

  void SetBinIndex(Int_t i_bin)
  {
    theBinIndex_ = i_bin;
  }

  void Produce()
  {
    if( theBinIndex_ == -1 )
    {
      cout << "[Produce] SetBinIndex is needed first" << endl;
      return;
    }

    Validation();
    DrawDXSec();
    DrawLambda1D();
    // DrawDXSec_eemm();
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

    // cout << "nRow: " << lambda_->GetNrows() << ", nCol: " << lambda_->GetNcols() << endl;

    h_dXSec_ee_ = ConvertMatrixToDXSec("ee", dXSec_ee_, cov_ee_);
    h_dXSec_mm_ = ConvertMatrixToDXSec("mm", dXSec_mm_, cov_mm_);
    h_dXSec_ll_ = ConvertMatrixToDXSec("ll", dXSec_ll_, cov_ll_);

    printf("[Z cross section] (ee, mm, ll) = (%lf, %lf,  %lf)\n", ZCrossSection(h_dXSec_ee_), ZCrossSection(h_dXSec_mm_), ZCrossSection(h_dXSec_ll_));

    theBinIndex_ = -1;
    // PrintCrossSectionInfo_NearZ();


    // for(Int_t i=0; i<43; i++)
    //   printf("[%d bin] dXSec(ll) = %lf\n", i, (*dXSec_ll_)[i][0]);

    // Double_t dXSec_16th_ = 0;
    // for(Int_t i=0; i<86; i++)
    // {
    //   Double_t lambda = (*lambda_)[16-1][i];
    //   Double_t dXSec_eemm = (*dXSec_eemm_)[i][0];
    //   Double_t dXSec_partial_ll = lambda * dXSec_eemm;

    //   dXSec_16th_ += dXSec_partial_ll;

    //   printf("[%d bin index] (lambda, dXSec_eemm, dXSec_partial_ll, dXSec_16th_) = (%lf, %lf, %lf, %lf)\n", 
    //     i, lambda, dXSec_eemm, dXSec_partial_ll, dXSec_16th_);
    // }

    // PlotTool::Print_Histogram(h_dXSec_ee_);
    // PlotTool::Print_Histogram(h_dXSec_mm_);
  }

  void Validation()
  {
    Validation_ll_vs_lambdaTimesdXSec();
  }

  void Validation_ll_vs_lambdaTimesdXSec()
  {
    cout << "[Validation] dXSec_ll vs. lambda * dXSec_eemm" << endl;
    TMatrixD* m_dXSec_ll_2nd = new TMatrixD((*lambda_)*(*dXSec_eemm_));

    Int_t nBin = m_dXSec_ll_2nd->GetNrows();
    for(Int_t i=0; i<nBin; i++)
    {
      Double_t dXSec_ll     = (*dXSec_ll_)[i][0];
      Double_t dXSec_ll_2nd = (*m_dXSec_ll_2nd)[i][0];

      Double_t absDiff = dXSec_ll - dXSec_ll_2nd;
      Double_t relDiff = absDiff / dXSec_ll;

      if( relDiff > 0.001 ) // -- 0.1%
      {
        printf("  (dXSec_ll, dXSec_ll_2nd, relDiff) = (%lf, %lf, %lf)\n", dXSec_ll, dXSec_ll_2nd, relDiff);
        return;
      }
    }

    cout << "[Validation] Done." << endl;
  }

  void DrawDXSec()
  {
    DrawDXSec(60, 120);
    DrawDXSec(15, 3000);

    DrawRelUnc(60, 120);
    DrawRelUnc(15, 3000);

    DrawAbsUnc(60, 120);
    DrawAbsUnc(15, 3000);
  }

  void DrawDXSec(Double_t mMin, Double_t mMax)
  {
    TString canvasName = TString::Format("c_dXSec_M%.0lfto%.0lf", mMin, mMax);
    Bool_t isLogX = kFALSE;
    if( mMax == 3000 ) isLogX = kTRUE;

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, isLogX, 1);
    canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "e(#mu)/combined");

    canvas->Register( h_dXSec_ll_, "Combined",    kRed);
    canvas->Register( h_dXSec_ee_, "e channel",   kGreen+2);
    canvas->Register( h_dXSec_mm_, "#mu channel", kBlue);

    canvas->SetRangeX(mMin, mMax);

    // canvas->SetRangeY(-10, 100);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);
    canvas->SetRangeRatio(0.94, 1.06);
    // canvas->SetRangeRatio(0.7, 1.3);

    canvas->SetLegendPosition(0.60, 0.82, 0.95, 0.95);

    canvas->Latex_CMSPre();

    canvas->Draw("EPSAME");
  }

  void DrawRelUnc(Double_t mMin, Double_t mMax)
  {
    TH1D* h_relUnc_ee = PlotTool::Extract_RelUnc(h_dXSec_ee_, "h_relUnc_ee", kTRUE);
    TH1D* h_relUnc_mm = PlotTool::Extract_RelUnc(h_dXSec_mm_, "h_relUnc_mm", kTRUE);
    TH1D* h_relUnc_ll = PlotTool::Extract_RelUnc(h_dXSec_ll_, "h_relUnc_ll", kTRUE);

    TString canvasName = TString::Format("c_relUnc_M%.0lfto%.0lf", mMin, mMax);
    Bool_t isLogX = kFALSE;
    if( mMax == 3000 ) isLogX = kTRUE;

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, isLogX, 0);
    canvas->SetTitle("m [GeV]", "Rel. unc. (%)", "e(#mu)/combined");

    canvas->Register( h_relUnc_ll, "Combined",    kRed);
    canvas->Register( h_relUnc_ee, "e channel",   kGreen+2);
    canvas->Register( h_relUnc_mm, "#mu channel", kBlue);

    canvas->SetRangeX(mMin, mMax);
    if( mMin == 60 && mMax == 120 )
      canvas->SetRangeY(2, 10);

    // canvas->SetRangeY(-10, 100);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);
    canvas->SetRangeRatio(0.6, 3.0);

    canvas->SetLegendPosition(0.60, 0.82, 0.95, 0.95);

    canvas->Latex_CMSPre();

    canvas->Draw("HISTLPSAME");
  }

  void DrawAbsUnc(Double_t mMin, Double_t mMax)
  {
    TH1D* h_absUnc_ee = PlotTool::Extract_AbsUnc(h_dXSec_ee_, "h_absUnc_ee");
    TH1D* h_absUnc_mm = PlotTool::Extract_AbsUnc(h_dXSec_mm_, "h_absUnc_mm");
    TH1D* h_absUnc_ll = PlotTool::Extract_AbsUnc(h_dXSec_ll_, "h_absUnc_ll");

    TString canvasName = TString::Format("c_absUnc_M%.0lfto%.0lf", mMin, mMax);
    Bool_t isLogX = kFALSE;
    if( mMax == 3000 ) isLogX = kTRUE;

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, isLogX, 1);
    canvas->SetTitle("m [GeV]", "Abs. unc. [pb/GeV]", "e(#mu)/combined");

    canvas->Register( h_absUnc_ll, "Combined",    kRed);
    canvas->Register( h_absUnc_ee, "e channel",   kGreen+2);
    canvas->Register( h_absUnc_mm, "#mu channel", kBlue);

    canvas->SetRangeX(mMin, mMax);
    // if( mMin == 60 && mMax == 120 )
    //   canvas->SetRangeY(2, 10);

    // canvas->SetRangeY(-10, 100);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);
    canvas->SetRangeRatio(0.6, 3.0);

    canvas->SetLegendPosition(0.60, 0.82, 0.95, 0.95);

    canvas->Latex_CMSPre();

    canvas->Draw("HISTLPSAME");
  }


  void DrawLambda()
  {
    DrawLambda(0, 0);
    DrawLambda(-1, 1);
    DrawLambda(-2, 2);
    DrawLambda(-10, 10);
    DrawLambda(-100, 100);
    DrawLambda(-1000, 1000);
    DrawLambda(-10000, 10000);
  }

  void DrawLambda(Double_t zMin, Double_t zMax)
  {
    Bool_t setZRange = kTRUE;
    if( zMin == 0 && zMax == 0 ) setZRange = kFALSE;

    TH2D* h_lambda = ConvertMatrixToTH2D(lambda_);

    TString canvasName = "c_lambda";
    if( setZRange ) canvasName = canvasName + TString::Format("_%.0lfto%.0lf", zMin, zMax);

    TCanvas *c = new TCanvas(canvasName, "", 800, 600);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.10);
    c->SetRightMargin(0.15);
    c->SetBottomMargin(0.13);

    h_lambda->SetTitle("");
    h_lambda->SetStats(kFALSE);
    h_lambda->GetXaxis()->SetTitle( "Bin index (1-43: ee, 44-86: #mu#mu)" );
    h_lambda->GetXaxis()->SetLabelSize(0.04);
    h_lambda->GetXaxis()->SetTitleOffset(1.1);
    h_lambda->GetXaxis()->SetTitleSize(0.05);
    h_lambda->GetXaxis()->SetNoExponent(kFALSE);
    h_lambda->GetXaxis()->SetMoreLogLabels(kFALSE);

    h_lambda->GetYaxis()->SetTitle( "Bin index (ll)" );
    h_lambda->GetYaxis()->SetTitleOffset(1);
    h_lambda->GetYaxis()->SetTitleSize(0.05);
    h_lambda->GetYaxis()->SetLabelSize(0.045);
    h_lambda->GetYaxis()->SetNoExponent(kFALSE);
    h_lambda->GetYaxis()->SetMoreLogLabels(kFALSE);

    if(setZRange) h_lambda->GetZaxis()->SetRangeUser(zMin, zMax);

    h_lambda->Draw("COLZ");

    c->Update(); // -- without this, palette is not made...

    TLatex latex;
    latex.DrawLatexNDC(0.11, 0.96, "#font[62]{CMS}");
    latex.DrawLatexNDC(0.20, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
    latex.DrawLatexNDC(0.13, 0.91, "#font[42]{#scale[0.7]{BLUE coefficients (#lambda)}}");

    c->SaveAs(".pdf");

    delete h_lambda;
  }

  void DrawDXSec_eemm()
  {
    dXSec_eemm_->Draw();
  }

  void DrawLambda1D()
  {
    DrawLambda1D(theBinIndex_);
  }

  void DrawLambda1D(Int_t binIndex) // -- binIndex: start from 1
  {
    TH1D* h_lambda_ee = LambdaCoeff_OneBin(binIndex, "ee");
    TH1D* h_lambda_mm = LambdaCoeff_OneBin(binIndex, "mm");

    // PlotTool::Print_Histogram(h_lambda_ee);
    // PlotTool::Print_Histogram(h_lambda_mm);

    TString canvasName = "c_lambda1D_"+TString::Format("%dbinIndex", binIndex);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);
    canvas->SetTitle("Bin index (1-43)", "Coefficient (#lambda)");

    canvas->Register( h_lambda_ee, "coefficients on ee result",     kGreen+2);
    canvas->Register( h_lambda_mm, "coefficients on #mu#mu result", kBlue);

    // canvas->SetRangeY(-1e4, 1e4);
    // canvas->SetRangeY(-1, 1);
    canvas->SetRangeY(-7, 7);

    canvas->SetLegendPosition(0.50, 0.82, 0.95, 0.95);

    canvas->Latex_CMSPre();
    TString massRangeInfo = TString::Format("%.0lf < M < %.0lf GeV bin", vec_massBinEdge_[binIndex-1], vec_massBinEdge_[binIndex]);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.7]{"+massRangeInfo+"}}");

    canvas->Draw("HISTLPSAME");

    UnBiasedCheck( h_lambda_ee, h_lambda_mm );

    DrawLambdaTimesDXSec1D(binIndex, h_lambda_ee, h_lambda_mm);

    PrintLambdaDXSec(binIndex, h_lambda_ee, h_lambda_mm);

    delete h_lambda_ee;
    delete h_lambda_mm;
  }

  void PrintLambdaDXSec(Int_t binIndex, TH1D* h_lambda_ee, TH1D* h_lambda_mm)
  {
    cout << "[PrintLambdaDXSec] bin index = " << binIndex << endl;

    Double_t dXSec_ll = 0;
    for(Int_t i=0; i<h_lambda_ee->GetNbinsX(); i++)
    {
      Int_t i_bin = i+1;

      Double_t minM = vec_massBinEdge_[i];
      Double_t maxM = vec_massBinEdge_[i+1];

      Double_t lambda_ee = h_lambda_ee->GetBinContent(i_bin);
      Double_t lambda_mm = h_lambda_mm->GetBinContent(i_bin);

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

  void DrawLambdaTimesDXSec1D(Int_t binIndex, TH1D* h_lambda_ee, TH1D* h_lambda_mm)
  {
    TH1D* h_ee = MultiplyTwoHists(h_lambda_ee, h_dXSec_ee_);
    TH1D* h_mm = MultiplyTwoHists(h_lambda_mm, h_dXSec_mm_);

    cout << "Integral: ee = " << h_ee->Integral() << ", mm = " << h_mm->Integral() << endl;

    TString canvasName = "c_lambdaTimesDXSec1D_"+TString::Format("%dbinIndex", binIndex);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);
    canvas->SetTitle("Bin index (1-43)", "#lambda #times (d#sigma/dm) [pb/GeV]");

    canvas->Register( h_ee, "ee contribution (#lambda#timesd#sigma/dm) from each bin", kGreen+2);
    canvas->Register( h_mm, "#mu#mu contribution (#lambda#timesd#sigma/dm) from each bin", kBlue);

    canvas->SetRangeY(-15, 15);
    if( binIndex == 14 ) canvas->SetRangeY(-2, 10);
    if( binIndex == 15 ) canvas->SetRangeY(-2, 20);
    if( binIndex == 16 ) canvas->SetRangeY(-10, 90);
    if( binIndex == 18 ) canvas->SetRangeY(-2, 10);
    if( binIndex == 19 ) canvas->SetRangeY(-1, 5);
    // canvas->SetRangeY(-20, 120);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);

    canvas->SetLegendPosition(0.40, 0.82, 0.95, 0.95);

    canvas->Latex_CMSPre();
    TString massRangeInfo = TString::Format("%.0lf < M < %.0lf GeV bin", vec_massBinEdge_[binIndex-1], vec_massBinEdge_[binIndex]);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.7]{"+massRangeInfo+"}}");

    canvas->Draw("HISTLPSAME");

    DrawLambdaTimesDXSec1D_Sum_eemm(binIndex, h_ee, h_mm);

    // PlotTool::Print_Histogram(h_ee);
    // PlotTool::Print_Histogram(h_mm);

  }

  void DrawLambdaTimesDXSec1D_Sum_eemm(Int_t binIndex, TH1D* h_ee, TH1D* h_mm)
  {
    TH1D* h_sum = SumTwoHists(h_ee, h_mm, "h_sum");

    TString canvasName = "c_lambdaTimesDXSec1D_Sum_eemm_"+TString::Format("%dbinIndex", binIndex);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);
    canvas->SetTitle("Bin index (1-43)", "#lambda #times (d#sigma/dm) [pb/GeV]");

    canvas->Register( h_sum, "ee+mm contribution (#lambda#timesd#sigma/dm) from each bin", kBlack);

    canvas->SetRangeY(-2, 0.5);
    if( binIndex == 14 ) canvas->SetRangeY(-0.1, 0.1);
    if( binIndex == 15 ) canvas->SetRangeY(-0.1, 0.1);
    if( binIndex == 16 ) canvas->SetRangeY(-1, 0.5);
    if( binIndex == 18 ) canvas->SetRangeY(-0.2, 0.1);
    if( binIndex == 19 ) canvas->SetRangeY(-0.1, 0.1);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);

    canvas->SetLegendPosition(0.30, 0.15, 0.95, 0.30);

    canvas->Latex_CMSPre();
    TString massRangeInfo = TString::Format("%.0lf < M < %.0lf GeV bin", vec_massBinEdge_[binIndex-1], vec_massBinEdge_[binIndex]);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.7]{"+massRangeInfo+"}}");

    canvas->Draw("HISTLPSAME");

    DrawLambdaTimesDXSec1D_Sum_eemm_Relative(binIndex, h_sum);



    // PlotTool::Print_Histogram(h_sum);

    vector<Double_t> vec_plus;
    vector<Double_t> vec_minus;
    Double_t sum_plus  = 0;
    Double_t sum_minus = 0;
    for(Int_t i=0; i<h_sum->GetNbinsX(); i++)
    {
      Int_t i_bin = i+1;

      Double_t value = h_sum->GetBinContent(i_bin);

      if( value > 0 ) sum_plus  += value;
      else            sum_minus += value;

      if( value > 0 ) vec_plus.push_back( value );
      else            vec_minus.push_back( value );
    }

    printf("\n[DrawLambdaTimesDXSec1D_Sum_eemm] (sum_plus, sum_minus, sum_all) = (%lf, %lf, %lf)\n", sum_plus, sum_minus, sum_plus+sum_minus);

    std::sort(vec_plus.begin(),  vec_plus.end(), std::greater<Double_t>());
    // std::sort(vec_minus.begin(), vec_minus.end(), std::greater<Double_t>());
    std::sort(vec_minus.begin(), vec_minus.end());

    printf("First 5 positive contribution: %lf, %lf, %lf, %lf, %lf\n", vec_plus[0],  vec_plus[1],  vec_plus[2],  vec_plus[3],  vec_plus[4]);
    printf("First 5 negative contribution: %lf, %lf, %lf, %lf, %lf\n", vec_minus[0], vec_minus[1], vec_minus[2], vec_minus[3], vec_minus[4]);
    printf("\n");
  }

  void DrawLambdaTimesDXSec1D_Sum_eemm_Relative(Int_t binIndex, TH1D* h_sum)
  {
    TH1D* h_relSum = (TH1D*)h_sum->Clone();
    Double_t dXSec_ll_theBin = h_dXSec_ll_->GetBinContent(binIndex);
    h_relSum->Scale( 100.0 / dXSec_ll_theBin ); // % -- conversion to %

    TString canvasName = "c_lambdaTimesDXSec1D_Sum_eemm_Relative_"+TString::Format("%dbinIndex", binIndex);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);
    canvas->SetTitle("Bin index (1-43)", "(#lambda #times (d#sigma/dm)) / (d#sigma/dm)^{comb.} [%]");

    canvas->Register( h_relSum, "ee+mm contribution (#lambda#timesd#sigma/dm) from each bin", kBlack);

    canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-1, 1);
    // canvas->SetRangeY(-5, 5);

    canvas->SetLegendPosition(0.30, 0.15, 0.95, 0.30);

    canvas->Latex_CMSPre();
    TString massRangeInfo = TString::Format("%.0lf < M < %.0lf GeV bin", vec_massBinEdge_[binIndex-1], vec_massBinEdge_[binIndex]);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.7]{"+massRangeInfo+"}}");

    canvas->Draw("HISTLPSAME");
  }

  TH1D* LambdaCoeff_OneBin(Int_t binIndex, TString channel) // -- binIndex: start from 1
  {
    Int_t nBin = lambda_->GetNrows();

    TH1D* h_lambda = new TH1D("h_lambda_"+channel, "", nBin, 0, nBin);

    Int_t i_start = 0;
    if( channel == "mm" ) i_start = 43;

    Int_t i_bin = 1;
    for(Int_t i_col=i_start; i_col<i_start+nBin; i_col++)
    {
      Double_t lambda = (*lambda_)[binIndex-1][i_col];

      h_lambda->SetBinContent(i_bin, lambda);
      h_lambda->SetBinError(i_bin, 0);
      i_bin++;
    }

    return h_lambda;
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

  TH2D* ConvertMatrixToTH2D(TMatrixD* m)
  {
    return new TH2D(*m);
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

  // -- check the sum of coefficient == 1.0
  void UnBiasedCheck( TH1D* h_ee, TH1D* h_mm )
  {
    Double_t sum_coeff = 0;

    Int_t nBin = h_ee->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t coeff_ee = h_ee->GetBinContent(i_bin);
      Double_t coeff_mm = h_mm->GetBinContent(i_bin);

      sum_coeff += coeff_ee;
      sum_coeff += coeff_mm;
    }

    cout << "sum of coefficient: " << sum_coeff << endl;
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

  // void PrintCrossSectionInfo_NearZ()
  // {
  //   for(Int_t i=0; i<h_dXSec_ee_->GetNbinsX(); i++)
  //   {
  //     Int_t i_bin = i+1;
  //     Double_t binCenter = h_dXSec->GetBinCenter(i_bin);

  //     if( 60 < binCenter && binCenter < 120 )
  //     {
  //       Double_t binEdge_low  = h_dXSec_ee_->GetBinLowEdge(i_bin);
  //       Double_t binEdge_high = h_dXSec_ee_->GetBinLowEdge(i_bin+1);

  //       Double_t dXSec_ee = h_dXSec_ee_->GetBinContent(i_bin);
  //       Double_t dXSec_mm = h_dXSec_mm_->GetBinContent(i_bin);
  //       Double_t dXSec_ll = h_dXSec_ll_->GetBinContent(i_bin);        
  //     }
  //   }
  // }

};


void DrawPlots()
{
  TString fileName = "ROOTFile_IntermediateResults_BLUE_FullCorr.root";
  // TString fileName = "ROOTFile_IntermediateResults_BLUE_noCorr_eEffSF.root";
  // TString fileName = "ROOTFile_IntermediateResults_BLUE_NoCorr.root";

  PlotProducer* producer = new PlotProducer(fileName);
  producer->SetBinIndex(19);
  producer->Produce();
}