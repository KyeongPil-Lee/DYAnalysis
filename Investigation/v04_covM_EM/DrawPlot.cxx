#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TH2D* h_covEM_noLumi_;
  TH2D* h_corrEM_noLumi_;

  PlotProducer()
  {
    Init();
  }

  void Produce()
  {
    DrawCovM_EM("h_covEM", h_covEM_noLumi_);
    DrawCovM_EM("h_corrEM", h_corrEM_noLumi_);
  }

private:
  void Init()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName = rootFilePath + "/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";

    h_covEM_noLumi_  = PlotTool::Get_Hist2D(fileName, "ll/h_covEM_noLumi");

    TH2D* h_covAcc_ee = PlotTool::Get_Hist2D(fileName, "ee/h_cov_systAcc");
    TH2D* h_covAcc_mm = PlotTool::Get_Hist2D(fileName, "mm/h_cov_systAcc");

    // -- divide the diagonal terms by (unc_e * unc_mu)
    h_corrEM_noLumi_ = MakeCorrPlot( h_covEM_noLumi_, h_covAcc_ee, h_covAcc_mm);
    h_corrEM_noLumi_->SetName("h_corrEM_noLumi");

    PrintTH2D(h_covEM_noLumi_, 0);
    PrintTH2D(h_corrEM_noLumi_, 1);
  }

  void DrawCovM_EM(TString histType, TH2D* h2D )
  {
    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_"+histType, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    c->SetLogx();
    c->SetLogy();
    // if( histType.Contains("cov") ) c->SetLogz();


    h2D->SetTitle("");
    h2D->SetStats(kFALSE);
    h2D->GetXaxis()->SetTitle( "m(ee) [GeV]" );
    h2D->GetXaxis()->SetLabelSize(0.04);
    h2D->GetXaxis()->SetTitleOffset(1.1);
    h2D->GetXaxis()->SetTitleSize(0.05);
    h2D->GetXaxis()->SetNoExponent(kFALSE);
    h2D->GetXaxis()->SetMoreLogLabels(kFALSE);

    h2D->GetYaxis()->SetTitle( "m(#mu#mu) [GeV]" );
    h2D->GetYaxis()->SetTitleOffset(1.3);
    h2D->GetYaxis()->SetTitleSize(0.05);
    h2D->GetYaxis()->SetLabelSize(0.045);
    h2D->GetYaxis()->SetNoExponent(kFALSE);
    h2D->GetYaxis()->SetMoreLogLabels(kFALSE);

    if( histType.Contains("corr") )
      h2D->GetZaxis()->SetRangeUser(-1, 1);

    h2D->Draw("COLZ");

    c->Update(); // -- without this, palette is not made...
    TPaletteAxis *palette = (TPaletteAxis*)h2D->GetListOfFunctions()->FindObject("palette");
    palette->SetX1NDC(0.88);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.13);
    palette->SetY2NDC(0.95);

    TLatex latex;
    latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
    latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Supplementary}}}");
    latex.DrawLatexNDC(0.52, 0.96, "#font[42]{#scale[0.55]{2.3 fb^{-1}(ee), 2.8 fb^{-1}(#mu#mu) (13 TeV)}}");

    c->SaveAs(".pdf");
  }

  TH2D* MakeCorrPlot( TH2D* h_covEM_noLumi, TH2D* h_covAcc_ee, TH2D* h_covAcc_mm)
  {
    TH2D* h_corr = (TH2D*)h_covEM_noLumi->Clone();

    Int_t nBin = h_covEM_noLumi->GetNbinsX();

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t covEM = h_covEM_noLumi->GetBinContent(i_bin, j_bin);
        Double_t var_ee = h_covAcc_ee->GetBinContent(i_bin, j_bin); // --variance
        Double_t var_mm = h_covAcc_mm->GetBinContent(i_bin, j_bin); // --variance

        Double_t corrEM = 0;
        if( covEM == 0 )
          corrEM = 0;
        else if( var_ee == 0 || var_mm == 0 ) 
          cout << "var_ee: " << var_ee << ", var_mm" << var_mm << ": one of them are 0, but corrEM = " << corrEM << " != 0 -> need to check" << endl;
        else 
          corrEM = covEM / ( sqrt(var_ee) * sqrt(var_mm) );

        h_corr->SetBinContent(i_bin, j_bin, corrEM);
        h_corr->SetBinError(i_bin, j_bin, 0);
      }

    }

    return h_corr;
  }

  void PrintTH2D( TH2D* h, Bool_t isCorr )
  {
    cout << "[PrintTH2D] Histogram: " << h->GetName() << endl;

    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t value = h->GetBinContent(i_bin, j_bin);
        // printf("[%02d, %02d bin] value = %.3e\n", i_bin, j_bin, value);
        // if( i != j && value != 0 ) cout << "   -> value != 0!!" << endl;

        if( i != j ) continue;

        if( isCorr ) printf("%.3lf, ", value);
        else printf("%.2e, ", value);        
      }
    }

    cout << endl;
    cout << endl;
  }

};

void DrawPlot()
{
  PlotProducer* producer = new PlotProducer();
  producer->Produce();
}