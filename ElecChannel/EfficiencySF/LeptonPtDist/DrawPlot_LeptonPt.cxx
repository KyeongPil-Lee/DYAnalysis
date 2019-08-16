#include <ElecChannel/Include/DYElectronTool.h>

#define nPtBin 5

class FractionPerBin
{
public:
  Double_t arr_ptBinEdge[nPtBin+1] = {10, 20, 30, 40, 50, 200};

  Double_t frac_;
  Int_t binIndex_lead_;
  Int_t binIndex_sub_;

  FractionPerBin( Double_t frac, Int_t binIndex_lead, Int_t binIndex_sub)
  {
    frac_ = frac;
    binIndex_lead_ = binIndex_lead;
    binIndex_sub_  = binIndex_sub;
  }

  void Print()
  {
    Double_t minPt_sub = arr_ptBinEdge[binIndex_sub_];
    Double_t maxPt_sub = arr_ptBinEdge[binIndex_sub_+1];

    Double_t minPt_lead = arr_ptBinEdge[binIndex_lead_];
    Double_t maxPt_lead = arr_ptBinEdge[binIndex_lead_+1];

    // printf("[%.0lf < pt(lead) < %.0lf, %.0lf < pt(sub) < %.0lf GeV] frac = %lf\n",
    //   minPt_lead, maxPt_lead, minPt_sub, maxPt_sub, frac_);

    printf("[%.0lf, %.0lf]\t[%.0lf, %.0lf]\t%lf\n",
      minPt_lead, maxPt_lead, minPt_sub, maxPt_sub, frac_);
  }

private:

};

Bool_t Compare_LargerFrac( FractionPerBin frac1, FractionPerBin frac2 )
{
  return frac1.frac_ > frac2.frac_; 
}

class PlotProducer
{
public:
  TString fileName_;

  Int_t nMassBin_;
  vector<Double_t> vec_massBinEdge_ = {76, 81, 86, 91, 96, 101, 106};

  vector<TH2D*> vec_hist2D_leptonPt_;

  PlotProducer(TString fileName)
  {
    fileName_ = fileName;
    Init();
  }

  void Produce()
  {
    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      Double_t minM = vec_massBinEdge_[i_mass];
      Double_t maxM = vec_massBinEdge_[i_mass+1];
      DrawPlot(vec_hist2D_leptonPt_[i_mass], minM, maxM);

      printf("[PlotProducer::Produce] %.0lf < m < %.0lf\n", minM, maxM);
      SortFractionAndPrint(vec_hist2D_leptonPt_[i_mass]);
    }
  }

private:
  void DrawPlot(TH2D* h2D, Double_t minM, Double_t maxM)
  {
    gStyle->SetPalette(1);

    TString massInfo = TString::Format("m%.0lfto%.0lf", minM, maxM);

    TString canvasName = "c2DNorm_leptonPt_"+massInfo;

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
    h2D->GetXaxis()->SetTitle( "leading p_{T}(e)" );
    h2D->GetXaxis()->SetLabelSize(0.04);
    h2D->GetXaxis()->SetTitleOffset(1.1);
    h2D->GetXaxis()->SetTitleSize(0.05);
    h2D->GetXaxis()->SetNoExponent(kTRUE);
    h2D->GetXaxis()->SetMoreLogLabels(kTRUE);

    h2D->GetYaxis()->SetTitle( "sub-leading p_{T}(e)" );
    h2D->GetYaxis()->SetTitleOffset(1.25);
    h2D->GetYaxis()->SetTitleSize(0.05);
    h2D->GetYaxis()->SetLabelSize(0.045);
    h2D->GetYaxis()->SetNoExponent(kTRUE);
    h2D->GetYaxis()->SetMoreLogLabels(kTRUE);

    h2D->GetZaxis()->SetRangeUser(0, 0.5);

    h2D->Draw("COLZ");

    c->Update(); // -- without this, palette is not made...

    TLatex latex;
    latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
    latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Simulation}}}");

    TString latex_massInfo = TString::Format("%.0lf < m(ee) < %.0lf GeV", minM, maxM);
    latex.DrawLatexNDC(0.15, 0.91, "#font[42]{#scale[0.7]{"+latex_massInfo+"}}");

    c->SaveAs(".pdf");
  }

  void SortFractionAndPrint(TH2D* h2D)
  {
    vector<FractionPerBin> vec_frac;

    if( nPtBin != h2D->GetNbinsX() )
    {
      cout << "nPtBin != h2D->GetNbinsX()" << endl;
      return;
    }

    Double_t sum_frac = 0;
    for(Int_t i_lead=0; i_lead<nPtBin; i_lead++)
    {
      Int_t i_leadBin = i_lead+1;

      for(Int_t i_sub=0; i_sub<nPtBin; i_sub++)
      {
        Int_t i_subBin = i_sub+1;

        Double_t frac = h2D->GetBinContent(i_leadBin, i_subBin);
        vec_frac.push_back( FractionPerBin(frac, i_lead, i_sub) );

        sum_frac += frac;
      }
    }

    cout << "sum of fraction = " << sum_frac << endl;
    cout << endl;

    std::sort(vec_frac.begin(), vec_frac.end(), Compare_LargerFrac);

    Int_t nFrac = vec_frac.size();
    for(Int_t i=0; i<nFrac; i++)
    {
      vec_frac[i].Print();
      if( i == 4 ) break;
    }

    cout << endl;
  }

  void Init()
  {
    nMassBin_ = (Int_t)vec_massBinEdge_.size() - 1;

    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      TString massBinInfo = TString::Format("m%0.lfto%.0lf", vec_massBinEdge_[i_mass], vec_massBinEdge_[i_mass+1]);
      TString histName = "h2DNorm_leptonPt_"+massBinInfo;
      TH2D* h_temp = PlotTool::Get_Hist2D(fileName_, histName);
      vec_hist2D_leptonPt_.push_back( h_temp );
    }
  }

};

void DrawPlot_LeptonPt()
{
  TString fileName = "ROOTFile_PlotProducer_LeptonPt.root";
  PlotProducer* producer = new PlotProducer(fileName);
  producer->Produce();
}