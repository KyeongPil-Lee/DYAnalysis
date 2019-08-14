#include <Include/SimplePlotTools.h>
class ComparisonTool
{
public:
  TString uncType_;

  TH1D* h_dXSec_;
  TH1D* h_relUnc_;

  TH2D* h2D_cov_;

  TH1D* h_relUnc_fromCov_;


  ComparisonTool(TString uncType)
  {
    TH1::AddDirectory(kFALSE);

    uncType_ = uncType;
  }

  ComparisonTool(TString uncType, TH1D* h_dXSec, TH2D* h2D_cov, TH1D* h_relUnc): ComparisonTool(uncType)
  {
    Set_DXSecHist( h_dXSec );
    Set_CovHist( h2D_cov );
    Set_RelUncHist( h_relUnc );
  }

  void Set_DXSecHist(TH1D* h_dXSec)
  {
    h_dXSec_ = (TH1D*)h_dXSec->Clone();
  }

  void Set_RelUncHist(TH1D* h_relUnc)
  {
    h_relUnc_ = (TH1D*)h_relUnc->Clone();
  }

  void Set_CovHist(TH2D* h2D_cov)
  {
    h2D_cov_ = (TH2D*)h2D_cov->Clone();
  }


  void Comparison()
  {
    PreStep();
    DrawPlot();
  }

private:
  void DrawPlot()
  {
    TString canvasName = "c_comparison_relUnc_GaussianFit_vs_CovM_"+uncType_;
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 1, 0);
    canvas->SetTitle("m(ee) [GeV]", "Rel. unc. [%]", "Blue/Black");

    canvas->Register(h_relUnc_,         "Uncertainty from the gaussian fit",             kBlack);
    canvas->Register(h_relUnc_fromCov_, "Uncertainty from the diagonal term of cov. M.", kBlue);

    canvas->SetRangeY(0, 5);
    canvas->SetRangeRatio(0.94, 1.06);
    canvas->SetLegendPosition(0.30, 0.75, 0.95, 0.90);

    canvas->Latex_CMSPre();

    TString uncInfo = "";
    if( uncType_ == "bkgChange" ) uncInfo = "Uncertainty from the background shape";
    if( uncType_ == "sgnChange" ) uncInfo = "Uncertainty from the signal shape";
    if( uncType_ == "tagChange" ) uncInfo = "Uncertainty from the tag selection";
    if( uncType_ == "nlo" )       uncInfo = "Uncertainty from the MC sample";
    if( uncType_ == "stat" )      uncInfo = "Uncertainty from statistics";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+uncInfo+"}}");

    canvas->Draw("HISTLPSAME");
  }
  void PreStep()
  {
    MakeHist_RelUnc_FromCov();

    // -- convert to %
    h_relUnc_->Scale(100.0);
    h_relUnc_fromCov_->Scale(100.0);
  }

  void MakeHist_RelUnc_FromCov()
  {
    h_relUnc_fromCov_ = (TH1D*)h_dXSec_->Clone("h_relUnc_fromCov");

    Int_t nBin = h_dXSec_->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t dXSec = h_dXSec_->GetBinContent(i_bin);

      Double_t absUnc_fromCov = sqrt(h2D_cov_->GetBinContent(i_bin, i_bin));
      Double_t relUnc_fromCov = absUnc_fromCov / dXSec;

      h_relUnc_fromCov_->SetBinContent(i_bin, relUnc_fromCov);
      h_relUnc_fromCov_->SetBinError(i_bin, 0);
    }
  }
};

void Comparison_givenUncType(TString uncType)
{
  TString analyerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString baseDir = analyerPath+"/ElecChannel/CovMatrix/EfficiencySF/";

  TString fileName_dXSec = baseDir+"/ROOTFile_SmearedDXSecProducer_"+uncType+".root";
  TH1D* h_dXSec_cv = PlotTool::Get_Hist(fileName_dXSec, "h_dXSec_cv");

  TString fileName_cov = baseDir+"/ROOTFile_CovM_EfficiencySF.root";
  TString histName_cov = "h_cov_"+uncType;
  TH2D* h2D_cov = PlotTool::Get_Hist2D(fileName_cov, histName_cov);

  TString fileName_relUnc = analyerPath+"/ElecChannel/Uncertainties/EfficiencySF/ROOTFile_Summary_Unc_EffSF.root";
  TString histName_relUnc = "h_relUnc_"+uncType;
  TH1D* h_relUnc = PlotTool::Get_Hist(fileName_relUnc, histName_relUnc);

  ComparisonTool* tool = new ComparisonTool(uncType, h_dXSec_cv, h2D_cov, h_relUnc);
  tool->Comparison();
}

void Comparison_Unc_vs_Diag()
{
  Comparison_givenUncType("bkgChange");
  Comparison_givenUncType("sgnChange");
  Comparison_givenUncType("tagChange");
  Comparison_givenUncType("nlo");
  Comparison_givenUncType("stat");
}