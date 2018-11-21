#include <Include/PlotTools.h>

class CovDrawingTool
{
public:
  TString type_;
  TH2D* h_cov_; // -- covariance matrix
  TH2D* h_corr_; // -- correlation matrix

  Bool_t isCorrAvail;

  CovDrawingTool(TString type, TH2D* h_cov)
  {
    type_ = type;
    h_cov_  = (TH2D*)h_cov->Clone();
    isCorrAvail = kFALSE;
    h_corr_ = nullptr;
  }

  CovDrawingTool(TString type, TH2D* h_cov, TH2D* h_corr)
  {
    type_ = type;
    h_cov_  = (TH2D*)h_cov->Clone();
    isCorrAvail = kTRUE;
    h_corr_ = (TH2D*)h_corr->Clone();
  }

  void DrawAllPlots()
  {
    DrawPlot_givenTH2D( "cov", h_cov_ );
    cout << "Details numbers for covariance matrix (type = " << type_ << ")" << endl;
    Print_2DHist( h_cov_ );

    if( isCorrAvail )
    { 
      DrawPlot_givenTH2D( "corr", h_corr_ );
      cout << "Details numbers for correlation matrix (type = " << type_ << ")" << endl;
      Print_2DHist( h_corr_ );
    }
  }

private:
  void DrawPlot_givenTH2D(TString histType, TH2D* h2D )
  {
    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_"+histType+"_"+type_, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    c->SetLogx();
    c->SetLogy();

    TString axisTitle = "";
    if( type_.Contains("Electron") ) axisTitle = "m(ee) [GeV]";
    if( type_.Contains("Muon") )     axisTitle = "m(#mu#mu) [GeV]";
    if( type_.Contains("Comb") )     axisTitle = "m(ll) [GeV]";

    h2D->SetTitle("");
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

    h2D->Draw("COLZ");

    TPaletteAxis *palette = (TPaletteAxis*)h2D->GetListOfFunctions()->FindObject("palette");
    palette->SetX1NDC(0.88);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.13);
    palette->SetY2NDC(0.95);

    TLatex latex;
    latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
    latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Supplementary}}}");
    DataInfoLatex(latex);

    c->SaveAs(".pdf");
  }

  void DataInfoLatex(TLatex &latex)
  {
    if(type_.Contains("Electron"))
      latex.DrawLatexNDC(0.70, 0.96, "#font[42]{#scale[0.6]{2.3 fb^{-1} (13 TeV)}}");
    else if( type_.Contains("Muon") )
      latex.DrawLatexNDC(0.70, 0.96, "#font[42]{#scale[0.6]{2.8 fb^{-1} (13 TeV)}}");
    else if( type_.Contains("Comb") )
      latex.DrawLatexNDC(0.52, 0.96, "#font[42]{#scale[0.55]{2.3 fb^{-1}(ee), 2.8 fb^{-1}(#mu#mu) (13 TeV)}}");
  }

  void Print_2DHist(TH2D* h)
  {
    printf("[CovDrawingTool::Print_2DHist] Starts ...\n");
    Int_t nBinX = h->GetNbinsX();
    Int_t nBinY = h->GetNbinsY();

    for(Int_t i_y=0; i_y<nBinY; i_y++)
    {
      Int_t i_binY = i_y+1;
      printf("[%02d binY]  ", i_binY);
   
      for(Int_t i_x=0; i_x<nBinX; i_x++)
      {
        Int_t i_binX = i_x+1;

        Double_t value = h->GetBinContent(i_binX, i_binY);
        printf("%.3e  ", value);
      }
      printf("\n");
    }

    printf("\n[CovDrawingTool::Print_2DHist] Finished\n");
  }

};

void GetTH2D_CombinedResult( TH2D*& h_cov, TH2D*& h_corr)
{
  // -- covariance matrix & correlation matrix for combined result
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName = rootFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
  
  TFile *f_input = TFile::Open(fileName);
  TCanvas *c_cov = (TCanvas*)f_input->Get("cCovFin")->Clone();

  c_cov->cd();
  TPad* pad1 = (TPad*)c_cov->GetPrimitive("cCovFin_1")->Clone();
  h_cov = (TH2D*)pad1->GetPrimitive("h2covFin")->Clone();

  TPad* pad2 = (TPad*)c_cov->GetPrimitive("cCovFin_2")->Clone();
  h_corr = (TH2D*)pad2->GetPrimitive("h2covFin_corr")->Clone();
}

void DrawCovM()
{
  TH2D* h_cov_comb;
  TH2D* h_corr_comb;
  GetTH2D_CombinedResult( h_cov_comb, h_corr_comb );

  CovDrawingTool* tool_comb = new CovDrawingTool( "Comb", h_cov_comb, h_corr_comb );
  tool_comb->DrawAllPlots();
}