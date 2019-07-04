#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString fileName_;

  TH1D* h_dXSec_ee_;
  TH1D* h_dXSec_mm_;
  TH1D* h_dXSec_ll_;

  PlotProducer(TString fileName)
  {
    fileName_ = fileName;
    Init();
  }

  void Produce()
  {
    Draw_DXSec(60, 120);
  }

private:
  void Init()
  {
    h_dXSec_ee_ = PlotTool::Get_Hist(fileName_, "h1csEE");
    h_dXSec_mm_ = PlotTool::Get_Hist(fileName_, "h1csMM");
    h_dXSec_ll_ = PlotTool::Get_Hist(fileName_, "h1csLL");

    Print_DXSec();
  }

  void Draw_DXSec(Double_t mMin, Double_t mMax)
  {
    TString canvasName = TString::Format("c_DXSec_M%.0lfto%.0lf", mMin, mMax);
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

  void Print_DXSec()
  {
    Int_t nBin = h_dXSec_ee_->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t minM = h_dXSec_ee_->GetBinLowEdge(i_bin);
      Double_t maxM = h_dXSec_ee_->GetBinLowEdge(i_bin+1);

      Double_t dXSec_ee = h_dXSec_ee_->GetBinContent(i_bin);
      Double_t dXSec_mm = h_dXSec_mm_->GetBinContent(i_bin);
      Double_t dXSec_ll = h_dXSec_ll_->GetBinContent(i_bin);

      printf("[%02d bin: %.0lf < M < %.0lf]", i_bin, minM, maxM);
      printf("  (ee, mm, ll) = (%lf, %lf, %lf)\n", dXSec_ee, dXSec_mm, dXSec_ll);
      printf("    -> rel.diff (ee & mm) = %lf\n", (dXSec_ee-dXSec_mm)/dXSec_mm );
      printf("    -> rel.diff (ee & ll) = %lf\n", (dXSec_ee-dXSec_ll)/dXSec_ll );
      printf("    -> rel.diff (mm & ll) = %lf\n", (dXSec_mm-dXSec_ll)/dXSec_ll );
    }
  }

};


void DrawPlot_DXSec()
{
  TString fileName = "ROOTFile_hepdata__corr10June_full.root";

  PlotProducer* producer = new PlotProducer(fileName);
  producer->Produce();

}