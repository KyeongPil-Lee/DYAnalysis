#include <Include/SimplePlotTools.h>

class ComparisonTool
{
public:
  TH1D* h_dXSec_hybrid_;
  TH1D* h_dXSec_BLUE_;

  TH1D* h_absUnc_hybrid_;
  TH1D* h_absUnc_BLUE_;

  TH1D* h_relUnc_hybrid_;
  TH1D* h_relUnc_BLUE_;

  Double_t relUnc_lumi_;

  ComparisonTool()
  {
    TH1::AddDirectory(kFALSE);
  }

  void Set_Hybrid(TH1D* h_dXSec, TH1D* h_absUnc)
  {
    h_dXSec_hybrid_  = (TH1D*)h_dXSec->Clone();
    h_absUnc_hybrid_ = (TH1D*)h_absUnc->Clone();
  }

  void Set_BLUE(TH1D* h_dXSec, TH1D* h_absUnc)
  {
    h_dXSec_BLUE_  = (TH1D*)h_dXSec->Clone();
    h_absUnc_BLUE_ = (TH1D*)h_absUnc->Clone();
  }

  void Set_RelUnc_Lumi(Double_t relUnc_lumi )
  {
    relUnc_lumi_ = relUnc_lumi;
  }

  void Compare()
  {
    // AddLumiUnc( h_dXSec_BLUE_, h_absUnc_BLUE_ );

    h_relUnc_hybrid_ = Convert_Abs_to_Rel(h_dXSec_hybrid_, h_absUnc_hybrid_);
    h_relUnc_BLUE_   = Convert_Abs_to_Rel(h_dXSec_BLUE_,   h_absUnc_BLUE_);

    h_relUnc_hybrid_->Scale(100.0);
    h_relUnc_BLUE_->Scale(100.0);

    Comparison_DXSec();
    Comparison_AbsUnc();
    Comparison_RelUnc();
  }

private:
  void AddLumiUnc( TH1D* h_dXSec, TH1D* h_absUnc )
  {
    Int_t nBin = h_dXSec->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec             = h_dXSec->GetBinContent(i_bin);
      Double_t absUnc_tot_noLumi = h_absUnc->GetBinContent(i_bin);
      Double_t absUnc_lumi = dXSec * relUnc_lumi_;

      Double_t absUnc_tot = sqrt( absUnc_tot_noLumi*absUnc_tot_noLumi + absUnc_lumi*absUnc_lumi);
      h_absUnc->SetBinContent(i_bin, absUnc_tot);
    }
  }

  TH1D* Convert_Abs_to_Rel(TH1D* h_dXSec, TH1D* h_absUnc)
  {
    TH1D* h_relUnc = (TH1D*)h_dXSec->Clone();
    TString histName = h_absUnc->GetName();
    histName.ReplaceAll("absUnc", "relUnc");
    h_relUnc->SetName(histName);

    Int_t nBin = h_dXSec->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec  = h_dXSec->GetBinContent(i_bin);
      Double_t absUnc = h_absUnc->GetBinContent(i_bin);

      Double_t relUnc = absUnc / dXSec;

      h_relUnc->SetBinContent(i_bin, relUnc);
      h_relUnc->SetBinError(i_bin, 0);
    }

    return h_relUnc;
  }

  void Comparison_DXSec()
  {
    TString canvasName = "c_dXSec";
    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
    canvas->SetTitle("m(ll) [GeV]", "d#sigma/dm [pb/GeV]", "Hybrid/BLUE");

    canvas->Register(h_dXSec_BLUE_,   "BLUE method",   kBlue);
    canvas->Register(h_dXSec_hybrid_, "Hybrid method", kBlack);
    canvas->SetLegendPosition(0.65, 0.80, 0.95, 0.95);
    canvas->SetRangeRatio(0.88, 1.12);
    canvas->Latex_CMSPre();
    canvas->Draw("HISTLPSAME");
  }

  void Comparison_RelUnc()
  {
    TString canvasName = "c_relUnc";
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 1, 1);
    canvas->SetTitle("m(ll) [GeV]", "Rel. unc. [%]", "Hybrid/BLUE");

    canvas->Register(h_relUnc_BLUE_,   "BLUE method",   kBlue);
    canvas->Register(h_relUnc_hybrid_, "Hybrid method", kBlack);
    canvas->SetLegendPosition(0.65, 0.80, 0.95, 0.95);
    canvas->SetRangeRatio(0.82, 1.18);
    canvas->Latex_CMSPre();
    canvas->Draw("HISTLPSAME");
  }

  void Comparison_AbsUnc()
  {
    TString canvasName = "c_absUnc";
    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
    canvas->SetTitle("m(ll) [GeV]", "Abs. unc. [pb/GeV]", "Hybrid/BLUE");

    canvas->Register(h_absUnc_BLUE_,   "BLUE method",   kBlue);
    canvas->Register(h_absUnc_hybrid_, "Hybrid method", kBlack);
    canvas->SetLegendPosition(0.65, 0.80, 0.95, 0.95);
    canvas->SetRangeRatio(0.82, 1.18);
    canvas->SmartRangeY();
    canvas->Latex_CMSPre();
    canvas->Draw("HISTLPSAME");
  }

};

void Comparison_BLUE()
{
  TString fileName_hybrid = "../ROOTFile_Combination.root";
  TH1D* h_dXSec_hybrid  = PlotTool::Get_Hist(fileName_hybrid, "ll/h_dXSec");
  TH1D* h_absUnc_hybrid = PlotTool::Get_Hist(fileName_hybrid, "ll/h_absUnc_tot");

  // TString fileName_BLUE = "../Converter_BLUEOutput/ROOTFile_hepdata__corr02Aug_Full_myCov_effSFSyst_Converted.root";
  TString fileName_BLUE = "../Converter_BLUEOutput/ROOTFile_hepdata__corr12Aug_Full_newEffSF_Converted.root";
  TH1D* h_dXSec_BLUE  = PlotTool::Get_Hist(fileName_BLUE, "ll/h_dXSec");
  TH1D* h_absUnc_BLUE = PlotTool::Get_Hist(fileName_BLUE, "ll/h_absUnc_tot");

  ComparisonTool* tool = new ComparisonTool();
  tool->Set_Hybrid(h_dXSec_hybrid, h_absUnc_hybrid);
  tool->Set_BLUE(h_dXSec_BLUE,     h_absUnc_BLUE);
  // -- below line is not needed: it is already included in BLUE method result also while converting the root file
  // tool->Set_RelUnc_Lumi( 0.023 ); // -- to add the lumi. unc. in BLUE result as it doesn't have lumi. unc. yet
  tool->Compare();
}