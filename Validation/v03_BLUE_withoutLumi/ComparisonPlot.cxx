#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TH1D* h_dXSec_ee_;
  TH1D* h_dXSec_mm_;
  TH1D* h_dXSec_ll_;

  TH1D* h_absUnc_tot_ee_;
  TH1D* h_absUnc_tot_mm_;
  TH1D* h_absUnc_tot_ll_;

  TH1D* h_relUnc_tot_ee_;
  TH1D* h_relUnc_tot_mm_;
  TH1D* h_relUnc_tot_ll_;

  PlotProducer()
  {
    Init();
  }

  void Produce()
  {
    // CrossCheck_Uncertainty("ee");
    // CrossCheck_Uncertainty("mm");

    DrawPlot(15,  3000);
    DrawPlot(15,  60);
    DrawPlot(60,  120);
    DrawPlot(120, 3000);

    DrawPlot(120, 440);
    DrawPlot(440, 3000);

    // DrawPlot_Unc("abs");
    // DrawPlot_Unc("rel");
  }



private:
  void DrawPlot_Unc(TString uncType)
  {
    TString canvasName = TString::Format("c_%sUnc_eemmll", uncType.Data());

    TH1D* h_unc_ee = nullptr;
    TH1D* h_unc_mm = nullptr;
    TH1D* h_unc_ll = nullptr;
    TString titleY = "";
    if( uncType == "abs" )
    {
      h_unc_ee = (TH1D*)h_absUnc_tot_ee_->Clone();
      h_unc_mm = (TH1D*)h_absUnc_tot_mm_->Clone();
      h_unc_ll = (TH1D*)h_absUnc_tot_ll_->Clone();
      titleY = "Abs. unc. [pb/GeV]";
    }
    if( uncType == "rel" )
    {
      h_unc_ee = (TH1D*)h_relUnc_tot_ee_->Clone();
      h_unc_mm = (TH1D*)h_relUnc_tot_mm_->Clone();
      h_unc_ll = (TH1D*)h_relUnc_tot_ll_->Clone();

      h_unc_ee->Scale(100);
      h_unc_mm->Scale(100);
      h_unc_ll->Scale(100);
      titleY = "Rel. unc. [%]";
    }

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, 1, 1 );
    canvas->Register( h_unc_ll, "Combined result", kRed);
    canvas->Register( h_unc_mm, "#mu channel", kBlue);
    canvas->Register( h_unc_ee, "e channel", kGreen+2);
    canvas->SetTitle("m [GeV]", titleY, "ee(#mu#mu)/combined");

    canvas->SetLegendPosition( 0.60, 0.78, 0.95, 0.90 );
    if( uncType == "rel" ) canvas->SetLegendPosition( 0.16, 0.78, 0.51, 0.90 );
    // canvas->SetRangeX(minM, maxM);
    if( uncType == "abs" ) canvas->SetRangeY(1e-8, 1e+3);
    if( uncType == "rel" ) canvas->SetRangeY(2, 200);

    // canvas->SetRangeRatio(0.84, 1.16);
    // canvas->SetRangeRatio(0.7, 1.3);
    canvas->SetRangeRatio(0.6, 3.0);
    // canvas->SetRangeRatio(0, 5.0);

    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{Total uncertainty (#delta_{stat}+#delta_{exp.}+#delta_{theo.})}}");
    canvas->Latex_CMSPre();

    canvas->Draw("HISTLPSAME");
  }
  void DrawPlot(Double_t minM, Double_t maxM)
  {
    TString canvasName = TString::Format("c_dXSec_eemmll_M%.0lftoM%.0lf", minM, maxM);

    Bool_t isLogX = kFALSE;
    if( maxM == 3000 ) isLogX = kTRUE;

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, isLogX, 1 );
    canvas->Register( h_dXSec_ll_, "Combined result", kRed);
    canvas->Register( h_dXSec_mm_, "#mu channel", kBlue);
    canvas->Register( h_dXSec_ee_, "e channel", kGreen+2);
    canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "ee(#mu#mu)/combined");

    canvas->SetLegendPosition( 0.60, 0.78, 0.95, 0.91 );
    canvas->SetRangeX(minM, maxM);
    canvas->SetRangeY(1e-8, 1e+4);
    if( minM == 15 && maxM == 60 )    canvas->SetRangeY(1, 1e3);
    if( minM == 60 && maxM == 120 )   canvas->SetRangeY(0.5, 1e3);
    if( minM == 120 && maxM == 3000 ) canvas->SetRangeY(1e-8, 5);
    if( minM == 120 && maxM == 440 )  canvas->SetRangeY(1e-3, 2);
    if( minM == 440 && maxM == 3000 ) canvas->SetRangeY(1e-8, 1e-2);

    // canvas->SetRangeRatio(0.84, 1.16);
    canvas->SetRangeRatio(0.86, 1.14);
    // canvas->SetRangeRatio(0.7, 1.3);
    // canvas->SetRangeRatio(0.3, 1.7);
    // canvas->SetRangeRatio(0, 5.0);

    TString massRangeInfo = "";
    if( minM == 15 && maxM == 60 )    massRangeInfo = "Below Z peak region";
    if( minM == 60 && maxM == 120 )   massRangeInfo = "Z peak region";
    if( minM == 120 && maxM == 3000 ) massRangeInfo = "Above Z peak region";
    if( minM == 120 && maxM == 440 )  massRangeInfo = "Above Z peak region (120 < M < 440 GeV)";
    if( minM == 440 && maxM == 3000 ) massRangeInfo = "Above Z peak region (M > 440 GeV)";

    canvas->RegisterLatex(0.16, 0.91, "#font[62]{#scale[0.6]{"+massRangeInfo+"}}");
    canvas->Latex_CMSPre();

    canvas->Draw();
  }

  void CrossCheck_Uncertainty(TString channel)
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName = "";
    if( channel == "ee" )
      fileName = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Electron.root";
    if( channel == "mm" )
      fileName = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root";

    TH1D* h_relUnc_tot_ref    = PlotTool::Get_Hist(fileName, "h_relUnc_tot");

    TH1D* h_relUnc_tot_target = nullptr;
    if( channel == "ee" ) h_relUnc_tot_target = PlotTool::Extract_RelUnc(h_dXSec_ee_);
    if( channel == "mm" ) h_relUnc_tot_target = PlotTool::Extract_RelUnc(h_dXSec_mm_);
      

    h_relUnc_tot_target->Scale(100); // -- make it %

    // -- another histogram: total uncertainty from Andrius file
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName_Andrius = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
    TH1D* h_absUnc_tot_Andrius = nullptr;
    if( channel == "ee" ) h_absUnc_tot_Andrius = PlotTool::Get_Hist( fileName_Andrius, "ee/h_absUnc_tot");
    if( channel == "mm" ) h_absUnc_tot_Andrius = PlotTool::Get_Hist( fileName_Andrius, "mm/h_absUnc_tot");

    TH1D* h_relUnc_tot_Andrius = nullptr;
     if( channel == "ee" ) h_relUnc_tot_Andrius = Convert_AbsUnc_To_RelUnc( h_dXSec_ee_, h_absUnc_tot_Andrius );
     if( channel == "mm" ) h_relUnc_tot_Andrius = Convert_AbsUnc_To_RelUnc( h_dXSec_mm_, h_absUnc_tot_Andrius );
     h_relUnc_tot_Andrius->Scale(100);


    // -- plot
    TString canvasName = TString::Format("c_xCheckUnc_ref_vs_UncFromDXSec_%s", channel.Data());

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio( canvasName, 1, 1 );
    canvas->Register( h_relUnc_tot_ref, "Reference (paper)", kRed);
    canvas->Register( h_relUnc_tot_target, "Extracted from DXSec histogram", kBlack);
    canvas->Register( h_relUnc_tot_Andrius, "from Andrius' file", kBlue);
    canvas->SetTitle("m [GeV]", "Rel. unc. [%]", "target/ref.");

    canvas->SetLegendPosition( 0.45, 0.82, 0.95, 0.95 );
    canvas->SetRangeY(1e-1, 200);

    // canvas->SetRangeRatio(0.84, 1.16);
    canvas->SetRangeRatio(0.7, 1.3);

    TString channelInfo = "";
    if( channel == "ee" ) channelInfo = "e channel";
    if( channel == "mm" ) channelInfo = "#mu channel";
    canvas->RegisterLatex(0.16, 0.91, "#font[62]{#scale[0.6]{"+channelInfo+"}}");
    canvas->Latex_CMSPre();
    canvas->Draw();

    PlotTool::IsRatio1(h_relUnc_tot_ref, h_relUnc_tot_target);
  }

  void Init()
  {
    // TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    // TString fileName = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
    TString fileName = "ROOTFile_hepdata__corrnoLumi_converted.root";

    h_dXSec_ee_ = PlotTool::Get_Hist(fileName, "ee/h_dXSec");
    h_dXSec_mm_ = PlotTool::Get_Hist(fileName, "mm/h_dXSec");
    h_dXSec_ll_ = PlotTool::Get_Hist(fileName, "ll/h_dXSec");

    // h_absUnc_tot_ee_ = PlotTool::Get_Hist(fileName, "ee/h_absUnc_tot");
    // h_absUnc_tot_mm_ = PlotTool::Get_Hist(fileName, "mm/h_absUnc_tot");
    // h_absUnc_tot_ll_ = PlotTool::Get_Hist(fileName, "ll/h_absUnc_tot");

    // h_relUnc_tot_ee_ = Convert_AbsUnc_To_RelUnc(h_dXSec_ee_, h_absUnc_tot_ee_);
    // h_relUnc_tot_mm_ = Convert_AbsUnc_To_RelUnc(h_dXSec_mm_, h_absUnc_tot_mm_);
    // h_relUnc_tot_ll_ = Convert_AbsUnc_To_RelUnc(h_dXSec_ll_, h_absUnc_tot_ll_);

    // -- electron channel: uncertainty in the histogram is not the total uncertainty -> should be updated
    // Assign_AbsUnc(h_dXSec_ee_, h_absUnc_tot_ee_);
  }

  TH1D* Convert_AbsUnc_To_RelUnc( TH1D* h_cv, TH1D* h_absUnc )
  {
    TH1D* h_relUnc = (TH1D*)h_absUnc->Clone();
    TString histName = h_absUnc->GetName();
    histName.ReplaceAll("absUnc", "relUnc");
    h_relUnc->SetName(histName);

    Int_t nBin = h_cv->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t cv     = h_cv->GetBinContent(i_bin);
      Double_t absUnc = h_absUnc->GetBinContent(i_bin);
      Double_t relUnc = absUnc / cv;

      h_relUnc->SetBinContent(i_bin, relUnc);
      h_relUnc->SetBinError(i_bin, 0);
    }

    return h_relUnc;
  }

  void Assign_RelUnc(TH1D* h_cv, TH1D* h_relUnc)
  {
    Int_t nBin = h_cv->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t cv     = h_cv->GetBinContent(i_bin);
      Double_t relUnc = h_relUnc->GetBinContent(i_bin);
      Double_t absUnc = cv * relUnc;

      h_cv->SetBinError(i_bin, absUnc);
    }
  }

  void Assign_AbsUnc(TH1D* h_cv, TH1D* h_absUnc)
  {
    Int_t nBin = h_cv->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t cv     = h_cv->GetBinContent(i_bin);
      Double_t absUnc = h_absUnc->GetBinContent(i_bin);

      h_cv->SetBinError(i_bin, absUnc);
    }
  }
};

void ComparisonPlot()
{
  PlotProducer *producer = new PlotProducer();
  producer->Produce();
}