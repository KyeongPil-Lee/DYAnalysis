class PlotProducer
{
public:
  TString fileName_fullCorr_;
  TString fileName_noCorr_eEffSF_;

  TMatrixD* cov_ee_fullCorr_;
  TMatrixD* cov_ee_noCorr_eEffSF_;

  TH2D* h_cov_ee_fullCorr_;
  TH2D* h_cov_ee_noCorr_eEffSF_;

  TH2D* h_corr_ee_fullCorr_;
  TH2D* h_corr_ee_noCorr_eEffSF_;

  TH2D* h_relDiff_cov_;
  TH2D* h_relDiff_corr_;

  Int_t i_startZ = 9; // -- 10th bin
  Int_t i_endZ = 21; // -- 22th bin

  PlotProducer(TString fullCorr, TString noCorr_eEffSF)
  {
    fileName_fullCorr_      = fullCorr;
    fileName_noCorr_eEffSF_ = noCorr_eEffSF;
    Init();
  }

  void Produce()
  {
    DrawCovM( h_cov_ee_fullCorr_,      0, 0);
    DrawCovM( h_cov_ee_noCorr_eEffSF_, 0, 0);

    DrawCovM( h_corr_ee_fullCorr_,      -1, 1);
    DrawCovM( h_corr_ee_noCorr_eEffSF_, -1, 1);

    DrawCovM( h_cov_ee_fullCorr_,      0, 0, kTRUE);
    DrawCovM( h_cov_ee_noCorr_eEffSF_, 0, 0, kTRUE);

    DrawCovM( h_corr_ee_fullCorr_,      -0.5, 0.5, kTRUE);
    DrawCovM( h_corr_ee_noCorr_eEffSF_, -0.5, 0.5, kTRUE);

    DrawCovM( h_relDiff_cov_,  0, 0);
    DrawCovM( h_relDiff_corr_, 0, 0);

    DrawCovM( h_relDiff_cov_,  0, 0, kTRUE);
    DrawCovM( h_relDiff_corr_, 0, 0, kTRUE);
  }

private:
  void DrawCovM(TH2D* h_cov, Double_t zMin, Double_t zMax, Bool_t isZMassRange = kFALSE)
  {
    Bool_t setZRange = kTRUE;
    if( zMin == 0 && zMax == 0 ) setZRange = kFALSE;

    TString canvasName = h_cov->GetName();
    canvasName = canvasName.ReplaceAll("h_", "c_");
    if( setZRange ) canvasName = canvasName + TString::Format("_%.0lfto%.0lf", zMin, zMax);
    if( isZMassRange )  canvasName = canvasName + "_ZMassRange";

    TCanvas *c = new TCanvas(canvasName, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);

    h_cov->SetTitle("");
    h_cov->SetStats(kFALSE);
    h_cov->GetXaxis()->SetTitle( "Bin index (1-43: ee)" );
    h_cov->GetXaxis()->SetLabelSize(0.04);
    h_cov->GetXaxis()->SetTitleOffset(1.1);
    h_cov->GetXaxis()->SetTitleSize(0.05);
    h_cov->GetXaxis()->SetNoExponent(kFALSE);
    h_cov->GetXaxis()->SetMoreLogLabels(kFALSE);

    h_cov->GetYaxis()->SetTitle( "Bin index (1-43: ee)" );
    h_cov->GetYaxis()->SetTitleOffset(1);
    h_cov->GetYaxis()->SetTitleSize(0.05);
    h_cov->GetYaxis()->SetLabelSize(0.045);
    h_cov->GetYaxis()->SetNoExponent(kFALSE);
    h_cov->GetYaxis()->SetMoreLogLabels(kFALSE);

    if(setZRange) h_cov->GetZaxis()->SetRangeUser(zMin, zMax);

    if(isZMassRange)
    {
      h_cov->GetXaxis()->SetRangeUser(i_startZ, i_endZ);
      h_cov->GetYaxis()->SetRangeUser(i_startZ, i_endZ);
    }

    h_cov->Draw("COLZ");

    c->Update(); // -- without this, palette is not made...

    // TLatex latex;
    // latex.DrawLatexNDC(0.11, 0.96, "#font[62]{CMS}");
    // latex.DrawLatexNDC(0.20, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
    // latex.DrawLatexNDC(0.13, 0.91, "#font[42]{#scale[0.7]{BLUE coefficients (#lambda)}}");
    c->SaveAs(".pdf");
  }

  void Init()
  {
    gStyle->SetPalette(1);

    TFile* f_fullCorr = TFile::Open(fileName_fullCorr_);
    f_fullCorr->cd();
    cov_ee_fullCorr_  = new TMatrixD( *(TMatrixD*)f_fullCorr->Get("cov_ee") );

    f_fullCorr->Close();


    TFile* f_noCorr_eEffSF = TFile::Open(fileName_noCorr_eEffSF_);
    f_noCorr_eEffSF->cd();
    cov_ee_noCorr_eEffSF_  = new TMatrixD( *(TMatrixD*)f_noCorr_eEffSF->Get("cov_ee") );

    f_noCorr_eEffSF->Close();

    h_cov_ee_fullCorr_      = ConvertMatrixToTH2D("h_cov_ee_fullCorr", cov_ee_fullCorr_);
    h_cov_ee_noCorr_eEffSF_ = ConvertMatrixToTH2D("h_cov_ee_noCorr_eEffSF", cov_ee_noCorr_eEffSF_);

    h_corr_ee_fullCorr_      = ConvertCovToCorr(h_cov_ee_fullCorr_);
    h_corr_ee_noCorr_eEffSF_ = ConvertCovToCorr(h_cov_ee_noCorr_eEffSF_);

    h_relDiff_cov_  = MakeTH2D_RelDiff( "h_relDiff_cov",  h_cov_ee_fullCorr_,  h_cov_ee_noCorr_eEffSF_);
    h_relDiff_corr_ = MakeTH2D_RelDiff( "h_relDiff_corr", h_corr_ee_fullCorr_, h_corr_ee_noCorr_eEffSF_);
  }

  TH2D* ConvertMatrixToTH2D(TString histName, TMatrixD* m)
  {
    TH2D* h = new TH2D(*m);
    h->SetName(histName);
    return h;
  }

  TH2D* ConvertCovToCorr( TH2D* h_cov )
  {
    TH2D* h_corr = (TH2D*)h_cov->Clone();

    TString histName = h_cov->GetName();
    histName = histName.ReplaceAll("h_cov", "h_corr");
    h_corr->SetName(histName);

    Int_t nBin = h_cov->GetNbinsX();

    vector<Double_t> vec_sigma; // -- uncertainties in each bin
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t variance = h_cov->GetBinContent(i_bin, i_bin);
      Double_t sigma = sqrt(variance);
      vec_sigma.push_back(sigma);
    }

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t sigma_i = vec_sigma[i];

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;
        Double_t sigma_j = vec_sigma[j];

        Double_t cov = h_cov->GetBinContent(i_bin, j_bin);
        Double_t corr = cov / (sigma_i * sigma_j);
        h_corr->SetBinContent(i_bin, j_bin, corr);
      }
    }

    return h_corr;
  }

  TH2D* MakeTH2D_RelDiff(TString histName, TH2D* h1, TH2D* h2)
  {
    TH2D* h_relDiff = (TH2D*)h1->Clone();
    h_relDiff->SetName(histName);

    Int_t nBin = h_relDiff->GetNbinsX();
    for(Int_t i_x=0; i_x<nBin; i_x++)
    {
      Int_t i_binX = i_x+1;

      for(Int_t i_y=0; i_y<nBin; i_y++)
      {
        Int_t i_binY = i_y+1;

        Double_t value1 = h1->GetBinContent(i_binX, i_binY);
        Double_t value2 = h2->GetBinContent(i_binX, i_binY);

        Double_t relDiff = (value2 - value1) / value1;

        h_relDiff->SetBinContent(i_binX, i_binY, relDiff);
        h_relDiff->SetBinError(i_binX, i_binY, 0);
      }
    }
    return h_relDiff;
  }

  TH2D* MakeTH2D_AbsDiff(TString histName, TH2D* h1, TH2D* h2)
  {
    TH2D* h_absDiff = (TH2D*)h1->Clone();
    h_absDiff->SetName(histName);

    Int_t nBin = h_absDiff->GetNbinsX();
    for(Int_t i_x=0; i_x<nBin; i_x++)
    {
      Int_t i_binX = i_x+1;

      for(Int_t i_y=0; i_y<nBin; i_y++)
      {
        Int_t i_binY = i_y+1;

        Double_t value1 = h1->GetBinContent(i_binX, i_binY);
        Double_t value2 = h2->GetBinContent(i_binX, i_binY);

        Double_t absDiff = value2 - value1;

        h_absDiff->SetBinContent(i_binX, i_binY, absDiff);
        h_absDiff->SetBinError(i_binX, i_binY, 0);
      }
    }
    return h_absDiff;
  }

};

void DrawCovPlot()
{
  TString fileName_fullCorr      = "../ROOTFile_IntermediateResults_BLUE_FullCorr.root";
  TString fileName_noCorr_eEffSF = "../ROOTFile_IntermediateResults_BLUE_NoCorr_eEffSF.root";

  PlotProducer* producer = new PlotProducer(fileName_fullCorr, fileName_noCorr_eEffSF);
  producer->Produce();
}