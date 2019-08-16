// -- to compare the correlation matrix before and after update of the covariance matrix in each channel
// -- due to the inconsistency between BLUE input and the HEPData/paper

#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TH2D* h_cov_systNonAcc_ee_before_;
  TH2D* h_cov_systNonAcc_ee_after_;

  TH2D* h_cov_systNonAcc_mm_before_;
  TH2D* h_cov_systNonAcc_mm_after_;

  TH2D* h_corr_systNonAcc_ee_before_;
  TH2D* h_corr_systNonAcc_ee_after_;

  TH2D* h_corr_systNonAcc_mm_before_;
  TH2D* h_corr_systNonAcc_mm_after_;

  PlotProducer()
  {

  }

  void Produce()
  {
    Init();

    DrawPlot_CovM( "cov_systNonAcc_ee_before", h_cov_systNonAcc_ee_before_);
    DrawPlot_CovM( "cov_systNonAcc_mm_before", h_cov_systNonAcc_mm_before_);
    DrawPlot_CovM( "corr_systNonAcc_ee_before", h_corr_systNonAcc_ee_before_);
    DrawPlot_CovM( "corr_systNonAcc_mm_before", h_corr_systNonAcc_mm_before_);

    DrawPlot_CovM( "cov_systNonAcc_ee_after", h_cov_systNonAcc_ee_after_);
    DrawPlot_CovM( "cov_systNonAcc_mm_after", h_cov_systNonAcc_mm_after_);
    DrawPlot_CovM( "corr_systNonAcc_ee_after", h_corr_systNonAcc_ee_after_);
    DrawPlot_CovM( "corr_systNonAcc_mm_after", h_corr_systNonAcc_mm_after_);
  }

private:
  void Init()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName_before = rootFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";

    TFile *f_input_before = TFile::Open(fileName_before);
    h_cov_systNonAcc_ee_before_  = GetTH2DInCanvas(f_input_before, "c2CovSystNonAcc_eeCov_onFile_adj", "c2CovSystNonAcc_eeCov_onFile_adj_1", "h2CovSystNonAcc_eeCov_onFile_adj");
    h_corr_systNonAcc_ee_before_ = GetTH2DInCanvas(f_input_before, "c2CovSystNonAcc_eeCov_onFile_adj", "c2CovSystNonAcc_eeCov_onFile_adj_2", "h2CovSystNonAcc_eeCov_onFile_adj_corr");

    h_cov_systNonAcc_mm_before_  = GetTH2DInCanvas(f_input_before, "c2CovSystNonAcc_mmCov_onFile_adj", "c2CovSystNonAcc_mmCov_onFile_adj_1", "h2CovSystNonAcc_mmCov_onFile_adj");
    h_corr_systNonAcc_mm_before_ = GetTH2DInCanvas(f_input_before, "c2CovSystNonAcc_mmCov_onFile_adj", "c2CovSystNonAcc_mmCov_onFile_adj_2", "h2CovSystNonAcc_mmCov_onFile_adj_corr");


    TString fileName_after = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
    h_cov_systNonAcc_ee_after_ = PlotTool::Get_Hist2D( fileName_after, "ee/h_cov_systNonAcc");
    h_cov_systNonAcc_mm_after_ = PlotTool::Get_Hist2D( fileName_after, "mm/h_cov_systNonAcc");

    h_corr_systNonAcc_ee_after_ = Convert_CovMToCorrM(h_cov_systNonAcc_ee_after_);
    h_corr_systNonAcc_mm_after_ = Convert_CovMToCorrM(h_cov_systNonAcc_mm_after_);
  }

  TH2D* Convert_CovMToCorrM( TH2D* h_cov )
  {
    TH2D* h_corr = (TH2D*)h_cov->Clone();
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

  void DrawPlot_CovM( TString type, TH2D* h_cov )
  {
    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_"+type, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    c->SetLogx();
    c->SetLogy();

    h_cov->SetTitle("");
    h_cov->SetStats(kFALSE);
    h_cov->GetXaxis()->SetTitle( "m [GeV]" );
    h_cov->GetXaxis()->SetLabelSize(0.04);
    h_cov->GetXaxis()->SetTitleOffset(1.1);
    h_cov->GetXaxis()->SetTitleSize(0.05);
    h_cov->GetXaxis()->SetNoExponent(kFALSE);
    h_cov->GetXaxis()->SetMoreLogLabels(kFALSE);

    h_cov->GetYaxis()->SetTitle( "m [GeV]" );
    h_cov->GetYaxis()->SetTitleOffset(1.3);
    h_cov->GetYaxis()->SetTitleSize(0.05);
    h_cov->GetYaxis()->SetLabelSize(0.045);
    h_cov->GetYaxis()->SetNoExponent(kFALSE);
    h_cov->GetYaxis()->SetMoreLogLabels(kFALSE);

    if( type.Contains("corr") )
      h_cov->GetZaxis()->SetRangeUser(-1.0, 1.0);

    TLatex latex;
    latex.DrawLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+type+"}}");

    h_cov->Draw("COLZ");

    c->Update(); // -- without this, palette is not made...
    TPaletteAxis *palette = (TPaletteAxis*)h_cov->GetListOfFunctions()->FindObject("palette");
    palette->SetX1NDC(0.88);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.13);
    palette->SetY2NDC(0.95);

    c->SaveAs(".pdf");
  }

  TH2D* GetTH2DInCanvas( TFile *f_input, TString canvasName, TString padName, TString histName )
  {
    TCanvas *c = (TCanvas*)f_input->Get(canvasName)->Clone();
    c->cd();
    TPad* pad = (TPad*)c->GetPrimitive(padName)->Clone();

    return (TH2D*)pad->GetPrimitive(histName)->Clone();
  }

};

void DrawPlot_CovCorrM()
{
  PlotProducer* producer = new PlotProducer();
  producer->Produce();
}