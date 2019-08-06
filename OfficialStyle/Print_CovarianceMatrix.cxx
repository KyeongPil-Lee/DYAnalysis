#include <Include/SimplePlotTools.h>

void Print_2DHist(TH2D* h);
void Print_CovarianceMatrix()
{
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName = rootFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
  
  TFile *f_input = TFile::Open(fileName);
  TCanvas *c_cov = (TCanvas*)f_input->Get("cCovFin")->Clone();

  c_cov->cd();
  TPad* pad1 = (TPad*)c_cov->GetPrimitive("cCovFin_1")->Clone();
  TH2D* h_cov = (TH2D*)pad1->GetPrimitive("h2covFin")->Clone();
  Print_2DHist(h_cov);

  TPad* pad2 = (TPad*)c_cov->GetPrimitive("cCovFin_2")->Clone();
  TH2D* h_corr = (TH2D*)pad2->GetPrimitive("h2covFin_corr")->Clone();
  Print_2DHist(h_corr);
}

void Print_2DHist(TH2D* h)
{
  printf("[Print_2DHist] Starts ...\n");
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

  printf("\n[Print_2DHist] Finished\n");
}