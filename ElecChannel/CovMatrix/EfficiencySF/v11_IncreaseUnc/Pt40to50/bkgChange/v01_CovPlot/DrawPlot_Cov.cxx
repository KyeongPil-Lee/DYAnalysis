#include <Include/SimplePlotTools.h>
#include <ElecChannel/Include/DYElectronTool.h>

class CovPlotProducer
{
public:
  TString type_; // -- for the canvas name

  TH2D* h_cov_;
  TH2D* h_corr_;

  CovPlotProducer(TString type, TH2D* h_cov)
  {
    type_ = type;

    TH1::AddDirectory(kFALSE);
    h_cov_ = (TH2D*)h_cov->Clone();

    TString histName_corr = h_cov_->GetName();
    histName_corr.ReplaceAll("h_cov", "h_corr");
    h_corr_ = DYTool::Convert_CovMToCorrM(h_cov_, histName_corr);
  }

  void Produce()
  {
    DrawPlot("cov",  h_cov_);
    DrawPlot("corr", h_corr_, 0.8, 1);
    DrawPlot("corr", h_corr_, 0.5, 1);
    DrawPlot("corr", h_corr_, -1, 1);
  }

private:
  void DrawPlot(TString covType, TH2D* h2D, Double_t zMin = 0, Double_t zMax = 0)
  {
    gStyle->SetPalette(1);

    Bool_t setZRange = kTRUE;
    if( zMin == 0 && zMax == 0 ) setZRange = kFALSE;

    TString canvasName = "c_" + covType + "_" + type_;
    if( setZRange ) canvasName = canvasName + TString::Format("_%.1lfto%.1lf", zMin, zMax);

    TCanvas *c = new TCanvas(canvasName, "", 800, 800);
    c->cd();

    c->SetLogx();
    c->SetLogy();

    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);

    h2D->SetTitle("");
    h2D->SetStats(kFALSE);
    h2D->GetXaxis()->SetTitle( "m(ee) [GeV]" );
    h2D->GetXaxis()->SetLabelSize(0.04);
    h2D->GetXaxis()->SetTitleOffset(1.1);
    h2D->GetXaxis()->SetTitleSize(0.05);
    h2D->GetXaxis()->SetNoExponent(kTRUE);
    h2D->GetXaxis()->SetMoreLogLabels(kTRUE);

    h2D->GetYaxis()->SetTitle( "m(ee) [GeV]" );
    h2D->GetYaxis()->SetTitleOffset(1.3);
    h2D->GetYaxis()->SetTitleSize(0.05);
    h2D->GetYaxis()->SetLabelSize(0.045);
    h2D->GetYaxis()->SetNoExponent(kTRUE);
    h2D->GetYaxis()->SetMoreLogLabels(kTRUE);

    if(setZRange) h2D->GetZaxis()->SetRangeUser(zMin, zMax);

    h2D->Draw("COLZ");

    c->Update(); // -- without this, palette is not made...

    // TLatex latex;
    // latex.DrawLatexNDC(0.11, 0.96, "#font[62]{CMS}");
    // latex.DrawLatexNDC(0.20, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
    // latex.DrawLatexNDC(0.13, 0.91, "#font[42]{#scale[0.7]{BLUE coefficients (#lambda)}}");

    c->SaveAs(".pdf");
  }

};

void DrawPlot_bkgChange_Ref()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName = analyzerPath+"/ElecChannel/CovMatrix/EfficiencySF/ROOTFile_CovM_EfficiencySF.root";
  TString histName = "h_cov_bkgChange";
  TH2D* h_cov = PlotTool::Get_Hist2D(fileName, histName);

  CovPlotProducer* producer = new CovPlotProducer("bkgChange_ref", h_cov);
  producer->Produce();
}

void DrawPlot_totalSyst_Ref()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName = analyzerPath+"/ElecChannel/CovMatrix/EfficiencySF/ROOTFile_CovM_EfficiencySF.root";
  TString histName = "h_cov_syst";
  TH2D* h_cov = PlotTool::Get_Hist2D(fileName, histName);

  CovPlotProducer* producer = new CovPlotProducer("totSyst_ref", h_cov);
  producer->Produce();
}

void DrawPlot_bkgChange_Target()
{
  TString fileName = "../ROOTFile_CovM_EfficiencySF.root";
  TString histName = "h_cov_bkgChange";
  TH2D* h_cov = PlotTool::Get_Hist2D(fileName, histName);

  CovPlotProducer* producer = new CovPlotProducer("bkgChange_target", h_cov);
  producer->Produce();
}

void DrawPlot_totalSyst_Target()
{
  TString fileName = "../ROOTFile_CovM_EfficiencySF.root";
  TString histName = "h_cov_syst";
  TH2D* h_cov = PlotTool::Get_Hist2D(fileName, histName);

  CovPlotProducer* producer = new CovPlotProducer("totSyst_target", h_cov);
  producer->Produce();
}

void DrawPlot_Cov()
{
  DrawPlot_bkgChange_Ref();
  DrawPlot_bkgChange_Target();

  DrawPlot_totalSyst_Ref();
  DrawPlot_totalSyst_Target();
}