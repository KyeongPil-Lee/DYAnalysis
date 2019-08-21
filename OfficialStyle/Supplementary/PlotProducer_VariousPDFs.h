#include <Include/PlotTools.h>

class PlotProducer_VariousPDFs
{
public:
  TString channel_;
  TString fileName_ = "";

  TH1D* h_dXSec_;
  TH1D* h_relUnc_stat_;
  TH1D* h_relUnc_tot_;

  TH1D* h_dXSec_NNPDF_;
  TH1D* h_relUnc_tot_NNPDF_;

  TH1D* h_dXSec_CT_;
  TH1D* h_relUnc_tot_CT_;

  TH1D* h_dXSec_MMHT_;
  TH1D* h_relUnc_tot_MMHT_;

  TH1D* h_dXSec_HERA_;
  TH1D* h_relUnc_tot_HERA_;

  TH1D* h_dXSec_ABM_;
  TH1D* h_relUnc_tot_ABM_;



  PlotProducer_VariousPDFs(TString channel)
  {
    channel_ = channel;
  }

  PlotProducer_VariousPDFs(TString channel, TString fileName)
  {
    channel_  = channel;
    fileName_ = fileName;
  }

  void Produce()
  {
    Init();
    DrawPlot();
  }

private:
  void DrawPlot()
  {
    // -- parameter settings
    Int_t color_NNPDF = kRed;
    Int_t color_CT = kViolet;
    Int_t color_MMHT = kBlue;
    Int_t color_HERAPDF = kGreen+2;
    Int_t color_ABM = kOrange+2;

    Double_t small = 0.000001; // -- margin between Pads -- //

    Double_t yTitleOffset = 0.35;
    Double_t yLabelOffset_pixel = 1;

    // -- data x-section with stat. unc. only -- //
    HistInfo *Hist_Data_StatUnc = new HistInfo( kBlack, "Data", h_dXSec_ );
    Hist_Data_StatUnc->CalcRatio_DEN( Hist_Data_StatUnc->h ); // -- ratio = 1 -- //

    AssignErrors( Hist_Data_StatUnc->h,       h_relUnc_stat_, kFALSE);
    AssignErrors( Hist_Data_StatUnc->h_ratio, h_relUnc_stat_, kFALSE);
    Hist_Data_StatUnc->h_ratio->SetMarkerStyle(20);
    // Hist_Data_StatUnc->h_ratio->SetMarkerSize(0.8);
    Hist_Data_StatUnc->h_ratio->SetMarkerSize(0);
    Hist_Data_StatUnc->h_ratio->SetLineWidth(1);


    // -- data x-section with total unc. -- //
    HistInfo *Hist_Data_TotUnc = new HistInfo( kGray+1, "Data", h_dXSec_ );
    Hist_Data_TotUnc->CalcRatio_DEN( Hist_Data_TotUnc->h ); // -- ratio = 1 -- //

    AssignErrors( Hist_Data_TotUnc->h,       h_relUnc_tot_, kFALSE );
    AssignErrors( Hist_Data_TotUnc->h_ratio, h_relUnc_tot_, kFALSE );
    Hist_Data_TotUnc->h_ratio->SetMarkerColorAlpha( kGray+1, 0 );
    Hist_Data_TotUnc->h_ratio->SetFillColorAlpha( kGray+1, 1 );
    gStyle->SetHatchesSpacing( 1.5 );
    Hist_Data_TotUnc->h_ratio->SetFillStyle( 3354 );


    // -- NNPDF 3.0 -- //
    HistInfo *Hist_NNPDF = new HistInfo( color_NNPDF, "NNPDF 3.0", h_dXSec_NNPDF_ );
    Hist_NNPDF->CalcRatio_DEN( Hist_Data_TotUnc->h );

    AssignErrors( Hist_NNPDF->h_ratio, h_relUnc_tot_NNPDF_, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
    // Hist_NNPDF->h_ratio->SetMarkerColorAlpha( color_NNPDF, 0 );
    Hist_NNPDF->h_ratio->SetMarkerStyle( kOpenTriangleDown );
    Hist_NNPDF->h_ratio->SetMarkerColor( color_NNPDF );
    Hist_NNPDF->h_ratio->SetLineColorAlpha( color_NNPDF, 0 );
    Hist_NNPDF->h_ratio->SetFillColorAlpha( color_NNPDF, 0.4 );
    Hist_NNPDF->h_ratio->SetFillStyle( 1001 );


    // -- CT14 -- //
    HistInfo *Hist_CT = new HistInfo( color_CT, "CT14", h_dXSec_CT_ );
    Hist_CT->CalcRatio_DEN( Hist_Data_TotUnc->h );

    AssignErrors( Hist_CT->h_ratio, h_relUnc_tot_CT_, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
    // Hist_CT->h_ratio->SetMarkerColorAlpha( color_CT, 0 );
    Hist_CT->h_ratio->SetMarkerStyle( kOpenTriangleUp );
    Hist_CT->h_ratio->SetMarkerColor( color_CT );
    Hist_CT->h_ratio->SetLineColorAlpha( color_CT, 0 );
    Hist_CT->h_ratio->SetFillColorAlpha( color_CT, 0.4 );


    // -- MMHT2014 -- //
    HistInfo *Hist_MMHT = new HistInfo( color_MMHT, "MMHT2014", h_dXSec_MMHT_ );
    Hist_MMHT->CalcRatio_DEN( Hist_Data_TotUnc->h );

    AssignErrors( Hist_MMHT->h_ratio, h_relUnc_tot_MMHT_, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
    // Hist_MMHT->h_ratio->SetMarkerColorAlpha( color_MMHT, 0 );
    Hist_MMHT->h_ratio->SetMarkerStyle( kOpenCircle );
    Hist_MMHT->h_ratio->SetMarkerColor( color_MMHT );
    Hist_MMHT->h_ratio->SetLineColorAlpha( color_MMHT, 0 );
    Hist_MMHT->h_ratio->SetFillColorAlpha( color_MMHT, 0.4 );


    // -- HERAPDF15 -- //
    HistInfo *Hist_HERAPDF = new HistInfo( color_HERAPDF, "HERAPDF15", h_dXSec_HERA_ );
    Hist_HERAPDF->CalcRatio_DEN( Hist_Data_TotUnc->h );

    AssignErrors( Hist_HERAPDF->h_ratio, h_relUnc_tot_HERA_, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
    // Hist_HERAPDF->h_ratio->SetMarkerColorAlpha( color_HERAPDF, 0 );
    Hist_HERAPDF->h_ratio->SetMarkerStyle( kOpenSquare );
    Hist_HERAPDF->h_ratio->SetMarkerColor( color_HERAPDF );
    Hist_HERAPDF->h_ratio->SetLineColorAlpha( color_HERAPDF, 0 );
    Hist_HERAPDF->h_ratio->SetFillColorAlpha( color_HERAPDF, 0.4 );


    // -- ABM12LHC -- //
    HistInfo *Hist_ABM = new HistInfo( color_ABM, "ABM12LHC", h_dXSec_ABM_ );
    Hist_ABM->CalcRatio_DEN( Hist_Data_TotUnc->h );

    AssignErrors( Hist_ABM->h_ratio, h_relUnc_tot_ABM_, kFALSE ); // -- uncertainty on the ratio: theo. unc. only -- //
    Hist_ABM->h_ratio->SetMarkerColorAlpha( color_ABM, 0 );
    Hist_ABM->h_ratio->SetMarkerStyle( kOpenDiamond );
    Hist_ABM->h_ratio->SetMarkerColor( color_ABM );
    // Hist_ABM->h_ratio->SetMarkerSize( 1 );
    Hist_ABM->h_ratio->SetLineColorAlpha( color_ABM, 0 );
    Hist_ABM->h_ratio->SetFillColorAlpha( color_ABM, 0.4 );


    // -- Draw Canvas -- //
    TCanvas *c;
    TString canvasName = TString::Format("RatioWithData_VariousPDF_%s", channel_.Data());
    SetCanvas_Square( c, canvasName );

    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);

    // -- just for drawing latex -- //
    TPad* BigPad = new TPad("BigPad","BigPad", 0.01, 0.01, 0.99, 0.99 );
    BigPad->Draw();
    BigPad->cd();
    BigPad->SetTopMargin(0.05);
    BigPad->SetBottomMargin(0.05);
    // BigPad->SetFillStyle(4050);


    Double_t UpperMargin = 0.05;
    Double_t LowerMargin = 0.1;

    // -- ySize_LastPad * LowerMargin_LastPad = LowerMargin -- //
    Double_t ySize_Pads = 0;
    Double_t ySize_LastPad = 0;
    Double_t LowerMargin_LastPad = 0;

    // -- 5*ySize_Pads = 1-UpperMargin-LowerMargin -- //
    ySize_Pads = ( 1 - UpperMargin - LowerMargin ) / 5.0;

    ySize_LastPad = ySize_Pads + LowerMargin;

    // -- ySize_LastPad * LowerMargin_LastPad = LowerMargin -- //
    LowerMargin_LastPad = LowerMargin / ySize_LastPad;

    printf("[ySize] (Pads, LastPads) = (%.3lf, %.3lf), [LowerMargin_LastPad = %.3lf]\n", ySize_Pads, ySize_LastPad, LowerMargin_LastPad);


    // -- inner pad with top and bottom margin -- //
    TPad *MultiPad = new TPad("MultiPad", "MultiPad", 0.01, ySize_LastPad, 0.99, 1-UpperMargin );
    MultiPad->Draw();
    MultiPad->cd();
    // MultiPad->SetBottomMargin(0.1);
    MultiPad->Divide(1, 4, small, small);

    MultiPad->cd(1);
    DrawOnPad_Data_vs_MC( gPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_NNPDF, small, yTitleOffset );

    MultiPad->cd(2);
    DrawOnPad_Data_vs_MC( gPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_CT, small, yTitleOffset );

    MultiPad->cd(3);
    DrawOnPad_Data_vs_MC( gPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_MMHT, small, yTitleOffset );

    MultiPad->cd(4);
    DrawOnPad_Data_vs_MC( gPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_HERAPDF, small, yTitleOffset );

    BigPad->cd();
    TPad *LastPad = new TPad( "LastPad", "LastPad", 0.01, 0.00, 0.99, ySize_LastPad );
    LastPad->Draw();
    LastPad->cd();
    DrawOnPad_Data_vs_MC( LastPad, Hist_Data_StatUnc, Hist_Data_TotUnc, Hist_ABM, small, yTitleOffset, kTRUE );
    LastPad->SetBottomMargin( LowerMargin_LastPad );
    Hist_ABM->h_ratio->GetYaxis()->SetTitleOffset( yTitleOffset * (ySize_LastPad / ySize_Pads) );
    Hist_ABM->h_ratio->GetXaxis()->SetTitleSize( 0.2 );
    Hist_ABM->h_ratio->GetXaxis()->SetTitleOffset( 0.8 );
    Hist_ABM->h_ratio->GetXaxis()->SetTitle( "m [GeV]" );
    TLegend *legend_lastPad = (TLegend*)LastPad->GetPrimitive("TPave");
    legend_lastPad->SetY1NDC(0.3);
    legend_lastPad->SetY2NDC(0.6);

    BigPad->cd();
    TLatex latex;
    Double_t lumi = 2.8;
    if( channel_ == "electron" ) lumi = 2.3;
    Int_t E_CM = 13;


    // latex.DrawLatexNDC(0.09, 0.96, "#font[62]{CMS}");
    // latex.DrawLatexNDC(0.19, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
    latex.DrawLatexNDC(0.09, 0.96, "#scale[0.8]{#font[62]{CMS }#font[42]{#it{Supplementary}}}");
    // latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");

    if( channel_ == "muon" || channel_ == "electron" )
      latex.DrawLatexNDC(0.72, 0.96, "#font[62]{#scale[0.7]{"+TString::Format("%.1lf fb^{-1} (%d TeV)", lumi, E_CM)+"}}");
    else if( channel_ == "combined" )
      latex.DrawLatexNDC( 0.46, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (ee)  2.8 fb^{-1} (#mu#mu) (13 TeV)}}");

    // latex.DrawLatexNDC( 0.76, 0.004, "#font[42]{#scale[0.8]{m (#mu#mu) [GeV]}}");

    c->SaveAs(".pdf");
  }

  void Init()
  {
    Init_DataResult();
    Init_NNPDF();
    Init_OtherPDFs();
  }

  void Init_NNPDF()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    TString fileName = rootFilePath+"/ROOTFile_DiffXSec_FullUnc.root";

    h_dXSec_NNPDF_      = Get_Hist(fileName, "h_DiffXsec_FEWZ_NNPDF_NNLO");  
    h_relUnc_tot_NNPDF_ = Extract_RelUnc( h_dXSec_NNPDF_ );

    h_dXSec_NNPDF_      = Rebin_MassBinEdges_Above200( h_dXSec_NNPDF_ );
    h_relUnc_tot_NNPDF_ = Rebin_MassBinEdges_Above200( h_relUnc_tot_NNPDF_ );
  }

  void Init_OtherPDFs()
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString basePath = analyzerPath+"/TheoryValues/NNLO_VariousPDF_Above200GeV";

    h_dXSec_CT_ = Get_Hist(basePath+"/CT14/ROOTFile_DY_FEWZ_NNLO_CT14nnlo.root", "h_DiffXsec");
    h_dXSec_CT_ = MergeLastTwoBins( h_dXSec_CT_ );
    AssignScaleUnc_3Percent( h_dXSec_CT_ );
    h_relUnc_tot_CT_ = Extract_RelUnc( h_dXSec_CT_ );

    h_dXSec_MMHT_ = Get_Hist(basePath+"/MMHT2014/ROOTFile_DY_FEWZ_NNLO_MMHT2014nnlo68cl.root", "h_DiffXsec");
    h_dXSec_MMHT_ = MergeLastTwoBins( h_dXSec_MMHT_ );
    AssignScaleUnc_3Percent( h_dXSec_MMHT_ );
    h_relUnc_tot_MMHT_ = Extract_RelUnc( h_dXSec_MMHT_ );

    h_dXSec_HERA_ = Get_Hist(basePath+"/HERAPDF/ROOTFile_DY_FEWZ_NNLO_HERAPDF15NNLO_EIG.root", "h_DiffXsec");
    h_dXSec_HERA_ = MergeLastTwoBins( h_dXSec_HERA_ );
    AssignScaleUnc_3Percent( h_dXSec_HERA_ );
    h_relUnc_tot_HERA_ = Extract_RelUnc( h_dXSec_HERA_ );

    h_dXSec_ABM_ = Get_Hist(basePath+"/ABM12LHC/ROOTFile_DY_FEWZ_NNLO_abm12lhc_5_nnlo.root", "h_DiffXsec");
    h_dXSec_ABM_ = MergeLastTwoBins( h_dXSec_ABM_ );
    AssignScaleUnc_3Percent( h_dXSec_ABM_ );
    h_relUnc_tot_ABM_ = Extract_RelUnc( h_dXSec_ABM_ );
  }

  void Init_DataResult()
  {
    if( fileName_ == "" )
    {
      fileName_ = DefaultFile();
      cout << "Use DEFAULT file: " << fileName_ << endl;
    }
    else
      cout << "Use PRIVATE file: " << fileName_ << endl;


    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    if( channel_ == "muon" )
    {      
      h_dXSec_       = Get_Hist(fileName_, "h_dXSec");
      h_relUnc_stat_ = Get_Hist(fileName_, "h_relUnc_stat");
      h_relUnc_tot_  = Get_Hist(fileName_, "h_relUnc_tot"); // -- stat. + exp. + theory

      h_relUnc_stat_->Scale(0.01);
      h_relUnc_tot_->Scale(0.01);

      h_dXSec_       = Rebin_MassBinEdges_Above200( h_dXSec_ );
      h_relUnc_stat_ = Rebin_MassBinEdges_Above200( h_relUnc_stat_ );
      h_relUnc_tot_  = Rebin_MassBinEdges_Above200( h_relUnc_tot_ );


    }
    else if( channel_ == "electron" )
    {
      h_dXSec_       = Get_Hist(fileName_, "h_dXSec");
      h_relUnc_stat_ = Get_Hist(fileName_, "h_relUnc_stat");
      h_relUnc_tot_  = Get_Hist(fileName_, "h_relUnc_tot"); // -- stat. + exp. + theory

      h_relUnc_stat_->Scale(0.01);
      h_relUnc_tot_->Scale(0.01);

      h_dXSec_       = Rebin_MassBinEdges_Above200( h_dXSec_ );
      h_relUnc_stat_ = Rebin_MassBinEdges_Above200( h_relUnc_stat_ );
      h_relUnc_tot_  = Rebin_MassBinEdges_Above200( h_relUnc_tot_ );
    }
    else if( channel_ == "combined" )
    {
      h_dXSec_       = Get_Hist(fileName_, "ll/h_dXSec");
      h_relUnc_stat_ = Get_Hist(fileName_, "ll/h_relUnc_stat");
      h_relUnc_tot_  = Get_Hist(fileName_, "ll/h_relUnc_tot"); // -- stat. + exp. + theory

      h_relUnc_stat_->Scale(0.01);
      h_relUnc_tot_->Scale(0.01);

      h_dXSec_       = Rebin_MassBinEdges_Above200( h_dXSec_ );
      h_relUnc_stat_ = Rebin_MassBinEdges_Above200( h_relUnc_stat_ );
      h_relUnc_tot_  = Rebin_MassBinEdges_Above200( h_relUnc_tot_ );
    }
  }

  TH1D* Rebin_MassBinEdges_Above200( TH1D* h )
  {
    const Int_t _nMassBin = 13;
    Double_t MassBinEdges[_nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

    return (TH1D*)h->Rebin(_nMassBin, h->GetName(), MassBinEdges);
  }

  TString DefaultFile()
  {
    TString fileName = "";

    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    if( channel_ == "muon" )
      fileName = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root";
    else if( channel_ == "electron" )
      fileName = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Electron.root";
    else if( channel_ == "combined" )
      fileName = analyzerPath+"/Combination/ROOTFile_Combination.root";
    else
      cout << "No default file corresponding to " << channel_ << "is found" << endl;

    return fileName;
  }

  TH1D* MergeLastTwoBins( TH1D* h )
  {
    // const Int_t _nMassBin = 15;
    // Double_t MassBinEdges[_nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1200, 1500, 2000, 3000};

    const Int_t _nMassBin = 13;
    Double_t MassBinEdges[_nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

    TString HistName = h->GetName();
    // TH1D* h_Merge = new TH1D(HistName, "", _nMassBin, MassBinEdges);
    TH1D* h_Merge = (TH1D*)h->Clone(); // -- in order to maintain attributes -- //
    h_Merge = Rebin_MassBinEdges_Above200( h_Merge );

    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t BinCenter = h->GetBinCenter(i_bin);

      if( BinCenter < 1000 )
      {
        Double_t value = h->GetBinContent(i_bin);
        Double_t error = h->GetBinError(i_bin);

        h_Merge->SetBinContent(i_bin, value);
        h_Merge->SetBinError(i_bin, error);
      }
    }

    Double_t XSec_M1000to1200 = h->GetBinContent(12) * h->GetBinWidth(12);
    Double_t XSecErr_M1000to1200 = h->GetBinError(12) * h->GetBinWidth(12);

    Double_t XSec_M1200to1500 = h->GetBinContent(13) * h->GetBinWidth(13);
    Double_t XSecErr_M1200to1500 = h->GetBinError(13) * h->GetBinWidth(13);

    Double_t XSec_M1500to2000 = h->GetBinContent(14) * h->GetBinWidth(14);
    Double_t XSecErr_M1500to2000 = h->GetBinError(14) * h->GetBinWidth(14);

    Double_t XSec_M2000to3000 = h->GetBinContent(15) * h->GetBinWidth(15);
    Double_t XSecErr_M2000to3000 = h->GetBinError(15) * h->GetBinWidth(15);

    Double_t DiffXSec_M1000to1500 = (XSec_M1000to1200 + XSec_M1200to1500) / 500.0;
    Double_t DiffXSecErr_M1000to1500 = QuadratureSum( XSecErr_M1000to1200, XSecErr_M1200to1500 ) / 500.0; // -- assume no correlation -- //

    Double_t DiffXSec_M1500to3000 = (XSec_M1500to2000 + XSec_M2000to3000) / 1500.0;
    Double_t DiffXSecErr_M1500to3000 = QuadratureSum( XSecErr_M1500to2000, XSecErr_M2000to3000 ) / 1500.0;

    h_Merge->SetBinContent( 12, DiffXSec_M1000to1500 );
    h_Merge->SetBinError( 12, DiffXSecErr_M1000to1500 );

    h_Merge->SetBinContent( 13, DiffXSec_M1500to3000 );
    h_Merge->SetBinError( 13, DiffXSecErr_M1500to3000 );

    return h_Merge;
  }

  Double_t QuadratureSum( Double_t a, Double_t b )
  {
    return sqrt( a*a + b*b );
  }

  void AssignScaleUnc_3Percent( TH1D* h )
  {
    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t DiffXSec = h->GetBinContent(i_bin);
      Double_t Err_before = h->GetBinError(i_bin);
      Double_t Err_ScaleVar = DiffXSec * 0.03; // -- 3% for all mass bins -- //

      Double_t Err_after = sqrt(Err_before*Err_before + Err_ScaleVar*Err_ScaleVar);
      h->SetBinError(i_bin, Err_after);
    }
  }

  void DrawOnPad_Data_vs_MC( TVirtualPad *Pad, HistInfo *Hist_Data_StatUnc, HistInfo *Hist_Data_TotUnc, HistInfo *Hist_Theory, Double_t small, Double_t yTitleOffset, Bool_t isLast = kFALSE )
  {
    Double_t yTitleSize_pixel = 15;
    Double_t yLabelSize_pixel = 19;

    Pad->SetTopMargin(small);
    Pad->SetBottomMargin(small);
    Pad->SetLeftMargin(0.08);
    Pad->SetRightMargin(0.045);
    Pad->SetLogx();
    Pad->SetTickx();
    Hist_Theory->h_ratio->Draw("E2PSAME");
    Hist_Data_TotUnc->h_ratio->Draw("E2PSAME");
    Hist_Data_StatUnc->h_ratio->Draw("E1PSAME");

    TAxis *X_axis = Hist_Theory->h_ratio->GetXaxis();
    X_axis->SetMoreLogLabels();
    X_axis->SetNoExponent();
    X_axis->SetTitle( "" );
    X_axis->SetTitleOffset( 0.0 );
    X_axis->SetTitleSize( 0.0 );
    X_axis->SetLabelColor(1);
    X_axis->SetLabelFont(42);
    X_axis->SetLabelOffset(0.01);
    X_axis->SetLabelSize(0.13);
    // X_axis->SetTickLength( xTickWidth );

    TAxis *Y_axis = Hist_Theory->h_ratio->GetYaxis();
    Double_t ratio_Min = 0.31;
    Double_t ratio_Max = 1.69;
    Y_axis->SetTitle( Hist_Theory->LegendName+"/Data" );
    Y_axis->CenterTitle();
    Y_axis->SetTitleOffset( 0.45 );
    Y_axis->SetTitleSize( yTitleSize_pixel / (Pad->GetWh()*Pad->GetAbsHNDC()) );
    // Y_axis->SetTitleOffset( yTitleOffset_pixel / (Pad->GetWh()*Pad->GetAbsHNDC()) );
    Y_axis->SetTitleOffset( yTitleOffset );

    Y_axis->SetLabelOffset(0.005);
    Y_axis->SetLabelSize( yLabelSize_pixel / (Pad->GetWh()*Pad->GetAbsHNDC()) );
    Y_axis->SetRangeUser( ratio_Min, ratio_Max );
    Y_axis->SetNdivisions( 505 );
    // Hist_Theory->h_ratio->SetMinimum( 0.45 );
    // Hist_Theory->h_ratio->SetMaximum( 1.55 );
    // Y_axis->SetRangeUser( 0.45, 1.55 );

    TLegend *legend;
    if( isLast )
      SetLegend( legend, 0.10, 0.39, 0.50, 0.52);
    else
      SetLegend( legend, 0.10, 0.05, 0.50, 0.25);
    legend->SetTextFont(62);
    legend->AddEntry( Hist_Theory->h_ratio, Hist_Theory->LegendName );
    legend->Draw();
    
  }

};