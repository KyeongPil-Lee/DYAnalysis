#include <Include/SimplePlotTools.h>

class ValidationTool
{
public:
  TString channel_;

  TH1D* h_dXSec_ref_;
  TH1D* h_absUnc_stat_ref_;
  TH1D* h_absUnc_bkg_ref_;
  TH1D* h_absUnc_detRes_ref_;
  TH1D* h_absUnc_FSR_ref_;
  TH1D* h_absUnc_effSF_ref_;
  TH1D* h_absUnc_lumi_ref_;
  TH1D* h_absUnc_accPDF_ref_;
  TH1D* h_absUnc_tot_ref_;
  
  TH1D* h_dXSec_BLUE_;
  TH1D* h_absUnc_stat_BLUE_;
  TH1D* h_absUnc_bkg_BLUE_;
  TH1D* h_absUnc_detRes_BLUE_;
  TH1D* h_absUnc_FSR_BLUE_;
  TH1D* h_absUnc_effSF_BLUE_;
  TH1D* h_absUnc_lumi_BLUE_;
  TH1D* h_absUnc_accPDF_BLUE_;
  TH1D* h_absUnc_tot_BLUE_;

  TH2D* h_cov_tot_;


  ValidationTool(TString channel)
  {
    channel_ = channel;
    Init();
  }

  void Validate()
  {
    ComparisonUnc_ref_vs_BLUE_All();
    Comparison_Diagonal_vs_TotUnc();
  }

private:
  // -- reference vs. input for BLUE used by Andrius
  void ComparisonUnc_ref_vs_BLUE_All()
  {
    ComparisonUnc_ref_vs_BLUE( "stat",    h_absUnc_stat_ref_,    h_absUnc_stat_BLUE_ );
    ComparisonUnc_ref_vs_BLUE( "bkg",     h_absUnc_bkg_ref_,     h_absUnc_bkg_BLUE_ );
    ComparisonUnc_ref_vs_BLUE( "detRes",  h_absUnc_detRes_ref_,  h_absUnc_detRes_BLUE_ );
    ComparisonUnc_ref_vs_BLUE( "FSR",     h_absUnc_FSR_ref_,     h_absUnc_FSR_BLUE_ );
    ComparisonUnc_ref_vs_BLUE( "effSF",   h_absUnc_effSF_ref_,   h_absUnc_effSF_BLUE_ );
    ComparisonUnc_ref_vs_BLUE( "lumi",    h_absUnc_lumi_ref_,    h_absUnc_lumi_BLUE_ );
    ComparisonUnc_ref_vs_BLUE( "accPDF",  h_absUnc_accPDF_ref_,  h_absUnc_accPDF_BLUE_ );
    ComparisonUnc_ref_vs_BLUE( "tot",     h_absUnc_tot_ref_,     h_absUnc_tot_BLUE_ );
  }

  // -- cov(i,i) == unc(i) ?
  void Comparison_Diagonal_vs_TotUnc()
  {
    TH1D* h_sqrtDiag = MakeHist_sqrtDiagonal( h_cov_tot_ );
    h_sqrtDiag->SetName("h_sqrtDiag");

    // -- ee or mm: covariance matrix: without luminosity uncertainty 
    // -- -> comparison should be done with total uncertainty without luminosity uncertainty
    TH1D* h_absUnc_tot_noLumi = Subtract_Quadrature( h_absUnc_tot_ref_, h_absUnc_lumi_ref_ );
    h_absUnc_tot_noLumi->SetName("h_absUnc_tot_noLumi");


    cout << "[Comparison_Diagonal_vs_Unc]" << endl;
    TString canvasName = TString::Format("c_diag_vs_unc_%s", channel_.Data());
    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, 1, 1 );
    canvas->Register( h_absUnc_tot_noLumi, "Total uncertainty w/o luml unc.", kRed);
    canvas->Register( h_sqrtDiag,          "sqrt( cov(i,i) )",  kBlack);
    canvas->SetTitle("m [GeV]", "Unc. [pb/GeV]", "black/red");

    canvas->Latex_CMSPre();
    canvas->SetLegendPosition( 0.45, 0.82, 0.95, 0.95 );
    canvas->SetRangeY(1e-8, 1e+4);
    canvas->SetRangeRatio(0.96, 1.04);

    TString channelInfo = "";
    if( channel_ == "mm" ) channelInfo = "#mu channel";
    if( channel_ == "ee" ) channelInfo = "e channel";
    canvas->RegisterLatex(0.16, 0.91, "#font[62]{#scale[0.6]{"+channelInfo+"}}");
    canvas->Draw();

    PlotTool::IsRatio1(h_absUnc_tot_noLumi, h_sqrtDiag);
  }

  void ComparisonUnc_ref_vs_BLUE(TString type, TH1D* h_unc_ref, TH1D* h_unc_BLUE)
  {
    cout << "[ComparisonUnc_ref_vs_BLUE] type = " << type << endl;
    TString canvasName = TString::Format("c_unc_%s_%s", channel_.Data(), type.Data());
    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, 1, 1 );
    canvas->Register( h_unc_ref,  "Reference (KP, paper)",    kRed);
    canvas->Register( h_unc_BLUE, "Input for BLUE (Andrius)", kBlack);
    canvas->SetTitle("m [GeV]", "Unc. [pb/GeV]", "Andrius/KP");

    canvas->Latex_CMSPre();
    canvas->SetLegendPosition( 0.45, 0.82, 0.95, 0.95 );
    canvas->SetRangeY(1e-8, 1e+4);
    canvas->SetRangeRatio(0.96, 1.04);

    TString channelInfo = "";
    if( channel_ == "mm" ) channelInfo = "#mu channel ("+type+")";
    if( channel_ == "ee" ) channelInfo = "e channel ("+type+")";
    canvas->RegisterLatex(0.16, 0.91, "#font[62]{#scale[0.6]{"+channelInfo+"}}");
    canvas->Draw();

    PlotTool::IsRatio1(h_unc_ref, h_unc_BLUE);
  }

  void Init()
  {
    Init_Ref();
    Init_BLUE();
  }

  void Init_Ref()
  {
    h_dXSec_ref_ = nullptr;
    h_absUnc_stat_ref_ = nullptr;
    h_absUnc_bkg_ref_ = nullptr;
    h_absUnc_detRes_ref_ = nullptr;
    h_absUnc_FSR_ref_ = nullptr;
    h_absUnc_effSF_ref_ = nullptr;
    h_absUnc_lumi_ref_ = nullptr;
    h_absUnc_accPDF_ref_ = nullptr;
    h_absUnc_tot_ref_ = nullptr;

    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName = "";
    if( channel_ == "ee" )
      fileName = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Electron.root";
    if( channel_ == "mm" )
      fileName = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root";

    h_dXSec_ref_ = PlotTool::Get_Hist(fileName, "h_dXSec");

    h_absUnc_stat_ref_    = PlotTool::Get_Hist(fileName, "h_absUnc_stat");
    h_absUnc_bkg_ref_     = PlotTool::Get_Hist(fileName, "h_absUnc_syst_bkg");
    h_absUnc_detRes_ref_  = PlotTool::Get_Hist(fileName, "h_absUnc_syst_detRes");
    h_absUnc_FSR_ref_     = PlotTool::Get_Hist(fileName, "h_absUnc_syst_FSR");
    h_absUnc_effSF_ref_   = PlotTool::Get_Hist(fileName, "h_absUnc_syst_eff");
    h_absUnc_lumi_ref_    = PlotTool::Get_Hist(fileName, "h_absUnc_lumi");
    h_absUnc_accPDF_ref_  = PlotTool::Get_Hist(fileName, "h_absUnc_syst_accPDF");
    h_absUnc_tot_ref_     = PlotTool::Get_Hist(fileName, "h_absUnc_tot");
  }

  void Init_BLUE()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
    
    TString dirName = channel_;

    h_dXSec_BLUE_          = PlotTool::Get_Hist(fileName, dirName+"/h_dXSec");

    h_absUnc_stat_BLUE_    = PlotTool::Get_Hist(fileName, dirName+"/h_absUnc_stat");
    h_absUnc_bkg_BLUE_     = PlotTool::Get_Hist(fileName, dirName+"/h_absUnc_bkg");
    h_absUnc_detRes_BLUE_  = PlotTool::Get_Hist(fileName, dirName+"/h_absUnc_detRes");
    h_absUnc_FSR_BLUE_     = PlotTool::Get_Hist(fileName, dirName+"/h_absUnc_FSR");
    h_absUnc_effSF_BLUE_   = PlotTool::Get_Hist(fileName, dirName+"/h_absUnc_effSF");
    h_absUnc_lumi_BLUE_    = PlotTool::Get_Hist(fileName, dirName+"/h_absUnc_lumi");
    h_absUnc_accPDF_BLUE_  = PlotTool::Get_Hist(fileName, dirName+"/h_absUnc_accPDF");
    h_absUnc_tot_BLUE_     = PlotTool::Get_Hist(fileName, dirName+"/h_absUnc_tot");

    h_cov_tot_ = PlotTool::Get_Hist2D(fileName, dirName+"/h_cov_tot");
  }

  TH1D* MakeHist_sqrtDiagonal( TH2D* h2D )
  {
    TH1D* h_sqrtDiag = (TH1D*)h_dXSec_ref_->Clone();
    h_sqrtDiag->Reset("ICES");

    Int_t nBin = h2D->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t diag     = h2D->GetBinContent(i_bin, i_bin);
      Double_t unc_diag = h2D->GetBinError(i_bin, i_bin);

      h_sqrtDiag->SetBinContent(i_bin, sqrt(diag));
      h_sqrtDiag->SetBinError(i_bin, sqrt(unc_diag));
    }

    return h_sqrtDiag;
  }

  // -- return sqrt( (h1)^2 - (h2)^2 )
  TH1D* Subtract_Quadrature( TH1D* h1, TH1D* h2 )
  {
    TH1D* h_return = (TH1D*)h1->Clone();
    h_return->Reset("ICES");

    Int_t nBin = h1->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t value1 = h1->GetBinContent(i_bin);
      Double_t value2 = h2->GetBinContent(i_bin);

      Double_t returnValue = sqrt( value1*value1 - value2*value2 );
      h_return->SetBinContent(i_bin, returnValue);
    }

    return h_return;
  }

};

void Validation()
{
  ValidationTool* tool_mm = new ValidationTool("mm");
  tool_mm->Validate();

  ValidationTool* tool_ee = new ValidationTool("ee");
  tool_ee->Validate();
}

