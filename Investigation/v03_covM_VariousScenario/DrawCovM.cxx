#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString channel_;
  TH2D* h_cov_tot_;
  TH2D* h_corr_tot_;

  PlotProducer(TString channel)
  {
    channel_ = channel;
    Init();
  }

  void Produce()
  {
    DrawCovM("cov_tot",  h_cov_tot_);
    DrawCovM("corr_tot", h_corr_tot_);
  }
private:
  void Init()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName = rootFilePath + "/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";

    h_cov_tot_  = PlotTool::Get_Hist2D(fileName, channel_+"/h_cov_tot");
    h_corr_tot_ = PlotTool::Get_Hist2D(fileName, channel_+"/h_corr_tot");
  }

  void DrawCovM(TString histType, TH2D* h2D )
  {
    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_"+channel_+"_"+histType, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    c->SetLogx();
    c->SetLogy();
    // if( histType.Contains("cov") ) c->SetLogz();

    TString axisTitle = "m(ll) [GeV]";
    if( channel_ == "mm" ) axisTitle = "m(#mu#mu) [GeV]";
    if( channel_ == "ee" ) axisTitle = "m(ee) [GeV]";

    h2D->SetTitle("");
    h2D->SetStats(kFALSE);
    h2D->GetXaxis()->SetTitle( axisTitle );
    h2D->GetXaxis()->SetLabelSize(0.04);
    h2D->GetXaxis()->SetTitleOffset(1.1);
    h2D->GetXaxis()->SetTitleSize(0.05);
    h2D->GetXaxis()->SetNoExponent(kFALSE);
    h2D->GetXaxis()->SetMoreLogLabels(kFALSE);

    h2D->GetYaxis()->SetTitle( axisTitle );
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
    if( channel_ == "ee" )
      latex.DrawLatexNDC(0.68, 0.96, "#font[42]{#scale[0.6]{2.3 fb^{-1} (13 TeV)}}");
    else if( channel_ == "mm" )
      latex.DrawLatexNDC(0.68, 0.96, "#font[42]{#scale[0.6]{2.8 fb^{-1} (13 TeV)}}");
    else if( channel_ == "ll" )
      latex.DrawLatexNDC(0.52, 0.96, "#font[42]{#scale[0.55]{2.3 fb^{-1}(ee), 2.8 fb^{-1}(#mu#mu) (13 TeV)}}");

    c->SaveAs(".pdf");
  }
};

void DrawCovM()
{
  PlotProducer* producer_ee = new PlotProducer("ee");
  producer_ee->Produce();

  PlotProducer* producer_mm = new PlotProducer("mm");
  producer_mm->Produce();

  PlotProducer* producer_ll = new PlotProducer("ll");
  producer_ll->Produce();
}