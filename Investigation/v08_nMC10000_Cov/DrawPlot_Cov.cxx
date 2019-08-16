#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString fileName_before_;
  TString fileName_after_;

  TMatrixD* m_cov_before_;
  TMatrixD* m_cov_after_;

  TH2D* h_cov_before_;
  TH2D* h_cov_after_;

  TH2D* h_corr_before_;
  TH2D* h_corr_after_;

  TH2D* h_relDiff_cov_;
  TH2D* h_relDiff_corr_;


  PlotProducer(TString fileName_before, TString fileName_after)
  {
    fileName_before_ = fileName_before;
    fileName_after_  = fileName_after;

    Init();
  }

  void Produce()
  {
    DrawTH2D("corr_before",   h_corr_before_, -1, 1);
    DrawTH2D("corr_after",    h_corr_after_,  -1, 1);

    DrawTH2D("relDiff_cov",   h_relDiff_cov_,  -0.05, 0.05);
    DrawTH2D("relDiff_corr",  h_relDiff_corr_, -0.05, 0.05);
  }

private:
  void Init()
  {
    TFile *f_before = TFile::Open(fileName_before_);
    m_cov_before_ = new TMatrixD( *(TMatrixD*)f_before->Get("eeCov_varRhoSystFile") );
    f_before->Close();

    TFile *f_after = TFile::Open(fileName_after_);
    m_cov_after_ = new TMatrixD( *(TMatrixD*)f_after->Get("eeCov_varRhoSystFile") );
    f_after->Close();

    h_cov_before_ = ConvertMatrixToTH2D(m_cov_before_, "h_cov_before");
    h_cov_after_  = ConvertMatrixToTH2D(m_cov_after_, "h_cov_after");

    h_corr_before_ = Convert_CovMToCorrM(h_cov_before_, "h_corr_before");
    h_corr_after_  = Convert_CovMToCorrM(h_cov_after_, "h_corr_after");

    h_relDiff_cov_  = RelDiff(h_cov_before_,  h_cov_after_,  "h_relDiff_cov");
    h_relDiff_corr_ = RelDiff(h_corr_before_, h_corr_after_, "h_relDiff_corr");
  }

  void DrawTH2D(TString type, TH2D* h2D, Double_t zMin, Double_t zMax)
  {
    gStyle->SetPalette(1);

    Bool_t setZRange = kTRUE;
    if( zMin == 0 && zMax == 0 ) setZRange = kFALSE;

    TString canvasName = "c_"+type;
    if( setZRange ) canvasName = canvasName + TString::Format("_%.0lfto%.0lf", zMin, zMax);

    TCanvas *c = new TCanvas(canvasName, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);

    h2D->SetTitle("");
    h2D->SetStats(kFALSE);
    h2D->GetXaxis()->SetTitle( "Bin index (1-43, ee)" );
    h2D->GetXaxis()->SetLabelSize(0.04);
    h2D->GetXaxis()->SetTitleOffset(1.1);
    h2D->GetXaxis()->SetTitleSize(0.05);
    h2D->GetXaxis()->SetNoExponent(kFALSE);
    h2D->GetXaxis()->SetMoreLogLabels(kFALSE);

    h2D->GetYaxis()->SetTitle( "Bin index (1-43, ee)" );
    h2D->GetYaxis()->SetTitleOffset(1.3);
    h2D->GetYaxis()->SetTitleSize(0.05);
    h2D->GetYaxis()->SetLabelSize(0.045);
    h2D->GetYaxis()->SetNoExponent(kFALSE);
    h2D->GetYaxis()->SetMoreLogLabels(kFALSE);

    if(setZRange) h2D->GetZaxis()->SetRangeUser(zMin, zMax);

    h2D->Draw("COLZ");

    c->Update(); // -- without this, palette is not made...

    // TLatex latex;
    // latex.DrawLatexNDC(0.11, 0.96, "#font[62]{CMS}");
    // latex.DrawLatexNDC(0.20, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
    // latex.DrawLatexNDC(0.13, 0.91, "#font[42]{#scale[0.7]{BLUE coefficients (#lambda)}}");

    c->SaveAs(".pdf");
  }

  TH2D* ConvertMatrixToTH2D(TMatrixD* m, TString histName)
  {
    TH2D* h = new TH2D(*m);
    h->SetName(histName);
    return h;
  }

  TH2D* Convert_CovMToCorrM( TH2D* h_cov, TString histName )
  {
    TH2D* h_corr = (TH2D*)h_cov->Clone();
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
  
  TH2D* RelDiff(TH2D* h1, TH2D* h2, TString histName)
  {
    TH2D* h_relDiff = (TH2D*)h1->Clone();
    h_relDiff->SetName(histName);
    Int_t nBin = h1->GetNbinsX();

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
      }
    }

    return h_relDiff;
  }
};

void DrawPlot_Cov()
{
  TString fileName_before = "ROOTFile_Input_Covs_v7_20190523.root";
  TString fileName_after  = "ROOTFile_Input_Covs_v8_20190627.root";
  PlotProducer* producer = new PlotProducer(fileName_before, fileName_after);
  producer->Produce();
}