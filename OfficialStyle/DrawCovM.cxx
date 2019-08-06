// #include <TMatrixD.h>
#include <Include/PlotTools.h>

class CovMatrices
{
public:
  TString type_;
  TString fileName_;

  // -- final covariance matrix, including luminosity
  TH2D* h_cov_;
  TH2D* h_corr_;

  // -- covariance matrix without luminosity
  TH2D* h_cov_noLumi_;
  TH2D* h_corr_noLumi_;

  // -- covariance matrices from each source
  TH2D* h_cov_statYield_;
  TH2D* h_corr_statYield_;

  TH2D* h_cov_statNonYield_;
  TH2D* h_corr_statNonYield_;

  TH2D* h_cov_systAcc_;
  TH2D* h_corr_systAcc_;

  TH2D* h_cov_systNonAcc_;
  TH2D* h_corr_systNonAcc_;

  // -- additional matrices (between e and mu channel): only for combined cases
  TH2D* h_crossCovEM_;
  TH2D* h_lumiCov2NBin_;

  TH2D* h_cov_lumiOnly_;
  TH2D* h_corr_lumiOnly_;

  CovMatrices(TString type)
  {
    type_ = type;

    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    fileName_ = rootFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";

    if( type_ == "Electron" ) Init_Electron();
    if( type_ == "Muon" )     Init_Muon();
    if( type_ == "Comb" )     Init_Comb();
  }

  void DrawPlots()
  {
    // -- covariance matric (without lumi.)
    DrawPlot_givenTH2D( "cov_noLumi",  h_cov_noLumi_ );
    DrawPlot_givenTH2D( "corr_noLumi",  h_corr_noLumi_ );

    // -- individual sources
    DrawPlot_givenTH2D( "cov_statYield",  h_cov_statYield_ );
    DrawPlot_givenTH2D( "corr_statYield", h_corr_statYield_ );

    DrawPlot_givenTH2D( "cov_statNonYield",  h_cov_statNonYield_ );
    DrawPlot_givenTH2D( "corr_statNonYield", h_corr_statNonYield_ );

    DrawPlot_givenTH2D( "cov_systAcc",  h_cov_systAcc_ );
    DrawPlot_givenTH2D( "corr_systAcc", h_corr_systAcc_ );

    DrawPlot_givenTH2D( "cov_systNonAcc",  h_cov_systNonAcc_ );
    DrawPlot_givenTH2D( "corr_systNonAcc", h_corr_systNonAcc_ );

    // -- additional plots are available
    if( type_ == "Comb" )
    {
      // -- total covariance/correlation matrix including luminosity
      DrawPlot_givenTH2D( "cov", h_cov_ );
      DrawPlot_givenTH2D( "corr", h_corr_ );

      // -- others
      DrawPlot_givenTH2D( "cov_lumiOnly",  h_cov_lumiOnly_ );
      DrawPlot_givenTH2D( "corr_lumiOnly", h_corr_lumiOnly_ );

      DrawPlot_givenTH2D( "crossCovEM",   h_crossCovEM_ );

      DrawPlot_givenTH2D( "lumiCov2NBin", h_lumiCov2NBin_ );
    }

  }

  // -- will be written later
  void Write(TFile *f_output)
  {

  }

private:
  void Init_Muon()
  {
    TFile *f_input = TFile::Open(fileName_);
    TMatrixD *covM_noLumi = (TMatrixD*)f_input->Get("BLUE_inp/covTotMM_noLumi");
    h_cov_noLumi_  = Convert_TMatrixDToMassBinnedTH2D(covM_noLumi, "h_covM_noLumi_"+type_);
    h_corr_noLumi_ = Convert_CovMToCorrM(h_cov_noLumi_);

    // -- individual sources
    h_cov_statYield_  = GetTH2DInCanvas(f_input, "c2CovStatYield_mmCov_onFile_adj", "c2CovStatYield_mmCov_onFile_adj_1", "h2CovStatYield_mmCov_onFile_adj");
    h_corr_statYield_ = GetTH2DInCanvas(f_input, "c2CovStatYield_mmCov_onFile_adj", "c2CovStatYield_mmCov_onFile_adj_2", "h2CovStatYield_mmCov_onFile_adj_corr");

    h_cov_statNonYield_  = GetTH2DInCanvas(f_input, "c2CovStatNonYield_mmCov_onFile_adj", "c2CovStatNonYield_mmCov_onFile_adj_1", "h2CovStatNonYield_mmCov_onFile_adj");
    h_corr_statNonYield_ = GetTH2DInCanvas(f_input, "c2CovStatNonYield_mmCov_onFile_adj", "c2CovStatNonYield_mmCov_onFile_adj_2", "h2CovStatNonYield_mmCov_onFile_adj_corr");

    h_cov_systAcc_  = GetTH2DInCanvas(f_input, "c2CovSystAcc_mmCov_onFile_adj", "c2CovSystAcc_mmCov_onFile_adj_1", "h2CovSystAcc_mmCov_onFile_adj");
    h_corr_systAcc_ = GetTH2DInCanvas(f_input, "c2CovSystAcc_mmCov_onFile_adj", "c2CovSystAcc_mmCov_onFile_adj_2", "h2CovSystAcc_mmCov_onFile_adj_corr");

    h_cov_systNonAcc_  = GetTH2DInCanvas(f_input, "c2CovSystNonAcc_mmCov_onFile_adj", "c2CovSystNonAcc_mmCov_onFile_adj_1", "h2CovSystNonAcc_mmCov_onFile_adj");
    h_corr_systNonAcc_ = GetTH2DInCanvas(f_input, "c2CovSystNonAcc_mmCov_onFile_adj", "c2CovSystNonAcc_mmCov_onFile_adj_2", "h2CovSystNonAcc_mmCov_onFile_adj_corr");

    // -- save 2D histograms (for HEPData entry creation)
    TFile *f_output = TFile::Open("ROOTFile_covM_muon.root", "RECREATE");
    f_output->cd();

    h_cov_noLumi_->SetName("h_cov_noLumi");
    h_cov_noLumi_->Write();

    h_corr_noLumi_->SetName("h_corr_noLumi");
    h_corr_noLumi_->Write();

    h_cov_statYield_->SetName("h_cov_statYield");
    h_cov_statYield_->Write();

    h_corr_statYield_->SetName("h_corr_statYield");
    h_corr_statYield_->Write();

    h_cov_statNonYield_->SetName("h_cov_statNonYield");
    h_cov_statNonYield_->Write();

    h_corr_statNonYield_->SetName("h_corr_statNonYield");
    h_corr_statNonYield_->Write();

    h_cov_systAcc_->SetName("h_cov_systAcc");
    h_cov_systAcc_->Write();

    h_corr_systAcc_->SetName("h_corr_systAcc");
    h_corr_systAcc_->Write();

    h_cov_systNonAcc_->SetName("h_cov_systNonAcc");
    h_cov_systNonAcc_->Write();

    h_corr_systNonAcc_->SetName("h_corr_systNonAcc");
    h_corr_systNonAcc_->Write();

    f_output->Close();
  }

  void Init_Electron()
  {
    TFile *f_input = TFile::Open(fileName_);
    TMatrixD *covM_noLumi = (TMatrixD*)f_input->Get("BLUE_inp/covTotEE_noLumi");
    h_cov_noLumi_  = Convert_TMatrixDToMassBinnedTH2D(covM_noLumi, "h_covM_noLumi_"+type_);
    h_corr_noLumi_ = Convert_CovMToCorrM(h_cov_noLumi_);

    // -- individual sources
    h_cov_statYield_  = GetTH2DInCanvas(f_input, "c2CovStatYield_eeCov_onFile_adj", "c2CovStatYield_eeCov_onFile_adj_1", "h2CovStatYield_eeCov_onFile_adj");
    h_corr_statYield_ = GetTH2DInCanvas(f_input, "c2CovStatYield_eeCov_onFile_adj", "c2CovStatYield_eeCov_onFile_adj_2", "h2CovStatYield_eeCov_onFile_adj_corr");

    h_cov_statNonYield_  = GetTH2DInCanvas(f_input, "c2CovStatNonYield_eeCov_onFile_adj", "c2CovStatNonYield_eeCov_onFile_adj_1", "h2CovStatNonYield_eeCov_onFile_adj");
    h_corr_statNonYield_ = GetTH2DInCanvas(f_input, "c2CovStatNonYield_eeCov_onFile_adj", "c2CovStatNonYield_eeCov_onFile_adj_2", "h2CovStatNonYield_eeCov_onFile_adj_corr");

    h_cov_systAcc_  = GetTH2DInCanvas(f_input, "c2CovSystAcc_eeCov_onFile_adj", "c2CovSystAcc_eeCov_onFile_adj_1", "h2CovSystAcc_eeCov_onFile_adj");
    h_corr_systAcc_ = GetTH2DInCanvas(f_input, "c2CovSystAcc_eeCov_onFile_adj", "c2CovSystAcc_eeCov_onFile_adj_2", "h2CovSystAcc_eeCov_onFile_adj_corr");

    h_cov_systNonAcc_  = GetTH2DInCanvas(f_input, "c2CovSystNonAcc_eeCov_onFile_adj", "c2CovSystNonAcc_eeCov_onFile_adj_1", "h2CovSystNonAcc_eeCov_onFile_adj");
    h_corr_systNonAcc_ = GetTH2DInCanvas(f_input, "c2CovSystNonAcc_eeCov_onFile_adj", "c2CovSystNonAcc_eeCov_onFile_adj_2", "h2CovSystNonAcc_eeCov_onFile_adj_corr");


    // -- save 2D histograms (for HEPData entry creation)
    TFile *f_output = TFile::Open("ROOTFile_covM_electron.root", "RECREATE");
    f_output->cd();

    h_cov_noLumi_->SetName("h_cov_noLumi");
    h_cov_noLumi_->Write();

    h_corr_noLumi_->SetName("h_corr_noLumi");
    h_corr_noLumi_->Write();

    h_cov_statYield_->SetName("h_cov_statYield");
    h_cov_statYield_->Write();

    h_corr_statYield_->SetName("h_corr_statYield");
    h_corr_statYield_->Write();

    h_cov_statNonYield_->SetName("h_cov_statNonYield");
    h_cov_statNonYield_->Write();

    h_corr_statNonYield_->SetName("h_corr_statNonYield");
    h_corr_statNonYield_->Write();

    h_cov_systAcc_->SetName("h_cov_systAcc");
    h_cov_systAcc_->Write();

    h_corr_systAcc_->SetName("h_corr_systAcc");
    h_corr_systAcc_->Write();

    h_cov_systNonAcc_->SetName("h_cov_systNonAcc");
    h_cov_systNonAcc_->Write();

    h_corr_systNonAcc_->SetName("h_corr_systNonAcc");
    h_corr_systNonAcc_->Write();

    f_output->Close();
  }

  void Init_Comb()
  {
    TFile *f_input = TFile::Open(fileName_);
    h_cov_  = GetTH2DInCanvas(f_input, "cCovFin", "cCovFin_1", "h2covFin");
    h_corr_ = GetTH2DInCanvas(f_input, "cCovFin", "cCovFin_2", "h2covFin_corr");

    // -- no luminosity
    h_cov_noLumi_ = GetTH2DInCanvas(f_input, "cFinCovNoLumi", "cFinCovNoLumi_1", "h2finCovNoLumi");
    h_cov_noLumi_ = Convert_TH2DToMassBinnedTH2D(h_cov_noLumi_);

    h_corr_noLumi_ = GetTH2DInCanvas(f_input, "cFinCovNoLumi", "cFinCovNoLumi_2", "h2finCovNoLumi_corr");
    h_corr_noLumi_ = Convert_TH2DToMassBinnedTH2D(h_corr_noLumi_);

    // -- individual sources
    h_cov_statYield_  = GetTH2DInCanvas(f_input, "cCovFin_from_statYield", "cCovFin_from_statYield_1", "h2covFin_fromstatYield");
    h_corr_statYield_ = GetTH2DInCanvas(f_input, "cCovFin_from_statYield", "cCovFin_from_statYield_2", "h2covFin_fromstatYield_corr");

    h_cov_statNonYield_  = GetTH2DInCanvas(f_input, "cCovFin_from_statNonYield", "cCovFin_from_statNonYield_1", "h2covFin_fromstatNonYield");
    h_corr_statNonYield_ = GetTH2DInCanvas(f_input, "cCovFin_from_statNonYield", "cCovFin_from_statNonYield_2", "h2covFin_fromstatNonYield_corr");

    h_cov_systAcc_  = GetTH2DInCanvas(f_input, "cCovFin_from_systAcc", "cCovFin_from_systAcc_1", "h2covFin_fromsystAcc");
    h_corr_systAcc_ = GetTH2DInCanvas(f_input, "cCovFin_from_systAcc", "cCovFin_from_systAcc_2", "h2covFin_fromsystAcc_corr");

    h_cov_systNonAcc_  = GetTH2DInCanvas(f_input, "cCovFin_from_systNonAcc", "cCovFin_from_systNonAcc_1", "h2covFin_fromsystNonAcc");
    h_corr_systNonAcc_ = GetTH2DInCanvas(f_input, "cCovFin_from_systNonAcc", "cCovFin_from_systNonAcc_2", "h2covFin_fromsystNonAcc_corr");

    // -- covariance matrix due to luminosity uncertainty
    h_cov_lumiOnly_  = GetTH2DInCanvas(f_input, "cLumiContrCov", "cLumiContrCov_1", "h2lumiContrCov");
    h_corr_lumiOnly_ = GetTH2DInCanvas(f_input, "cLumiContrCov", "cLumiContrCov_2", "h2lumiContrCov_corr");

    // -- cross covariance between two channels
    TMatrixD *covM_crossCovEM = (TMatrixD*)f_input->Get("BLUE_inp/covTotEM_noLumi");
    h_crossCovEM_ = Convert_TMatrixDToMassBinnedTH2D(covM_crossCovEM, "h_covM_crossCovEM_"+type_);

    // -- fully correlated covariance due to the luminosity uncertainty (1-43: electron channel, 44-86: muon channel)
    TMatrixD *covM_lumiCov2NBin = (TMatrixD*)f_input->Get("BLUE_inp/covTotLumi");
    h_lumiCov2NBin_ = new TH2D(*covM_lumiCov2NBin);
    h_lumiCov2NBin_->SetName("h_lumiCov2NBin");


    // -- save 2D histograms (for HEPData entry creation)
    TFile *f_output = TFile::Open("ROOTFile_covM_combined.root", "RECREATE");
    f_output->cd();

    h_cov_->SetName("h_cov");
    h_cov_->Write();

    h_corr_->SetName("h_corr");
    h_corr_->Write();

    h_cov_noLumi_->SetName("h_cov_noLumi");
    h_cov_noLumi_->Write();

    h_corr_noLumi_->SetName("h_corr_noLumi");
    h_corr_noLumi_->Write();

    h_cov_statYield_->SetName("h_cov_statYield");
    h_cov_statYield_->Write();

    h_corr_statYield_->SetName("h_corr_statYield");
    h_corr_statYield_->Write();

    h_cov_statNonYield_->SetName("h_cov_statNonYield");
    h_cov_statNonYield_->Write();

    h_corr_statNonYield_->SetName("h_corr_statNonYield");
    h_corr_statNonYield_->Write();

    h_cov_systAcc_->SetName("h_cov_systAcc");
    h_cov_systAcc_->Write();

    h_corr_systAcc_->SetName("h_corr_systAcc");
    h_corr_systAcc_->Write();

    h_cov_systNonAcc_->SetName("h_cov_systNonAcc");
    h_cov_systNonAcc_->Write();

    h_corr_systNonAcc_->SetName("h_corr_systNonAcc");
    h_corr_systNonAcc_->Write();

    h_cov_lumiOnly_->SetName("h_cov_lumiOnly");
    h_cov_lumiOnly_->Write();

    h_corr_lumiOnly_->SetName("h_corr_lumiOnly");
    h_corr_lumiOnly_->Write();

    h_crossCovEM_->SetName("h_crossCovEM");
    h_crossCovEM_->Write();

    h_lumiCov2NBin_->SetName("h_lumiCov2NBin");
    h_lumiCov2NBin_->Write();

    f_output->Close();
  }

  TH2D* GetTH2DInCanvas( TFile *f_input, TString canvasName, TString padName, TString histName )
  {
    TCanvas *c = (TCanvas*)f_input->Get(canvasName)->Clone();
    c->cd();
    TPad* pad = (TPad*)c->GetPrimitive(padName)->Clone();

    return (TH2D*)pad->GetPrimitive(histName)->Clone();
  }

  TH2D* Convert_TMatrixDToMassBinnedTH2D(TMatrixD* covM, TString histName)
  {
    cout << "CovM" << endl;
    cout << "  # row: " << covM->GetNrows() << endl;
    cout << "  # col: " << covM->GetNcols() << endl;
    TH2D* h_fromTMatrixD = new TH2D(*covM); // -- it is not mass binned
    h_fromTMatrixD->SetName(histName+"_fromTMatrixD");

    const Int_t nMassBin = 43;
    Double_t massBinEdges[nMassBin+1] = {
      15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
      64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
      110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
      200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
      830, 1000, 1500, 3000};

    TH2D* h_template = new TH2D(histName, "", nMassBin, massBinEdges, nMassBin, massBinEdges);

    // -- copy values
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      for(Int_t j=0; j<nMassBin; j++)
      {
        Int_t j_bin = j+1;

        // Double_t value = covM[i][j]; // -- it doesn't work, why?
        Double_t value = h_fromTMatrixD->GetBinContent(i_bin, j_bin);
        h_template->SetBinContent(i_bin, j_bin, value);
        h_template->SetBinError(i_bin, j_bin, 0);
      }
    }

    return h_template;
  }

  TH2D* Convert_TH2DToMassBinnedTH2D(TH2D* h_before)
  {
    TString histName_before = h_before->GetName();

    const Int_t nMassBin = 43;
    Double_t massBinEdges[nMassBin+1] = {
      15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
      64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
      110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
      200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
      830, 1000, 1500, 3000};

    TH2D* h_template = new TH2D(histName_before+"_massBinned", "", nMassBin, massBinEdges, nMassBin, massBinEdges);

    // -- copy values
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      for(Int_t j=0; j<nMassBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t value = h_before->GetBinContent(i_bin, j_bin);
        h_template->SetBinContent(i_bin, j_bin, value);
        h_template->SetBinError(i_bin, j_bin, 0);
      }
    }

    return h_template;
  }

  void DrawPlot_givenTH2D(TString histType, TH2D* h2D )
  {
    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_"+type_+"_"+histType, "", 800, 800);
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
    h2D->SetStats(kFALSE);
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

    if( histType.Contains("corr") )
      h2D->GetZaxis()->SetRangeUser(-1, 1);

    h2D->Draw("COLZ");

    // -- special treatment is needed as the axis of this matrix is just bin number, not mass
    if( histType == "lumiCov2NBin" )
    {
      c->SetLogx(kFALSE);
      c->SetLogy(kFALSE);

      h2D->GetXaxis()->SetTitle("");
      h2D->GetYaxis()->SetTitle("");

      TLine grid;
      grid.SetLineColor(kRed);
      grid.SetLineStyle(2);

      Double_t x1 = h2D->GetXaxis()->GetBinLowEdge(42);
      grid.DrawLine(43, 0, 43, 86);
      grid.DrawLine(0, 43, 86, 43);

      TLatex latex;
      latex.DrawLatexNDC(0.25, 0.03, "#font[42]{#scale[0.8]{m(ee) bin}}");
      latex.DrawLatexNDC(0.60, 0.03, "#font[42]{#scale[0.8]{m(#mu#mu) bin}}");
      // grid.DrawLineNDC(0.2, 0.2, 0.5, 0.5);

      latex.SetTextAngle(90);
      latex.DrawLatexNDC(0.04, 0.25, "#font[42]{#scale[0.8]{m(ee) bin}}");
      latex.DrawLatexNDC(0.04, 0.70, "#font[42]{#scale[0.8]{m(#mu#mu) bin}}");
    }

    c->Update(); // -- without this, palette is not made...
    TPaletteAxis *palette = (TPaletteAxis*)h2D->GetListOfFunctions()->FindObject("palette");
    palette->SetX1NDC(0.88);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.13);
    palette->SetY2NDC(0.95);

    TLatex latex;
    latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
    latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Supplementary}}}");
    DataInfoLatex(latex);

    if( histType == "cov" )  latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Total covariance}}");
    if( histType == "corr" ) latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Total correlation}}");

    if( histType.Contains("noLumi") )
    {
      if( histType.Contains("corr") )
        latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Total correlation}}");
      else
        latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Total covariance}}");

      latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{without luminosity uncertainty}}");
    }

    if( histType.Contains("lumiOnly") )
    {
      if( histType.Contains("corr") )
        latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Correlation of luminosity uncertainty}}");
      else
        latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Covariance of luminosity uncertainty}}");
    }

    if( histType.Contains("_statYield") || histType.Contains("_statNonYield") || 
        histType.Contains("_systAcc")   || histType.Contains("_systNonAcc") )
    {
      if( histType.Contains("corr") )
        latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Correlation of uncertainty}}");
      else
        latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Covariance of uncertainty}}");

      if( histType.Contains("_statYield") )
        latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{from statistical yield of the measurement}}");

      if( histType.Contains("_statNonYield") )
        latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.4]{from statistical sources not related to yield of the measurement}}");

      if( histType.Contains("_systAcc") )
        latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{from systematic uncertainty on acceptance}}");

      if( histType.Contains("_systNonAcc") )
        latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{from systematic uncertainty except for acceptance}}");
    }

    if( histType == "crossCovEM" )
      latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{cross-covariance between two channels}}");

    if( histType == "lumiCov2NBin" )
      latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{covariance of luminosity uncertainty between two channels}}");

    c->SaveAs(".pdf");
  }

  void DataInfoLatex(TLatex &latex)
  {
    if(type_.Contains("Electron"))
      latex.DrawLatexNDC(0.68, 0.96, "#font[42]{#scale[0.6]{2.3 fb^{-1} (13 TeV)}}");
    else if( type_.Contains("Muon") )
      latex.DrawLatexNDC(0.68, 0.96, "#font[42]{#scale[0.6]{2.8 fb^{-1} (13 TeV)}}");
    else if( type_.Contains("Comb") )
      latex.DrawLatexNDC(0.52, 0.96, "#font[42]{#scale[0.55]{2.3 fb^{-1}(ee), 2.8 fb^{-1}(#mu#mu) (13 TeV)}}");
  }

  void Print_2DHist(TH2D* h)
  {
    printf("[CovMatrices::Print_2DHist] Starts ...\n");
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

    printf("\n[CovMatrices::Print_2DHist] Finished\n");
  }

  TH2D* Convert_CovMToCorrM( TH2D* h_cov )
  {
    TH2D* h_corr = (TH2D*)h_cov->Clone();
    Int_t nBin = h_cov->GetNbinsX();

    vector<Double_t> vec_sigma; // -- uncertainties in each bin
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t variance = h_cov->GetBinContent(i_bin, i_bin);
      Double_t sigma = sqrt(variance);
      vec_sigma.push_back(sigma);
    }

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t sigma_i = vec_sigma[i];

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;
        Double_t sigma_j = vec_sigma[j];

        Double_t cov = h_cov->GetBinContent(i_bin, j_bin);
        Double_t corr = cov / (sigma_i * sigma_j);
        h_corr->SetBinContent(i_bin, j_bin, corr);
      }
    }

    return h_corr;
  }

  void Comparison_TH2D(TH2D* h1, TH2D* h2)
  {
    Int_t nBin = h1->GetNbinsX();

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;

        Double_t value1 = h1->GetBinContent(i_bin, j_bin);
        Double_t value2 = h2->GetBinContent(i_bin, j_bin);
        Double_t relDiff = (value2 - value1) / value1;

        cout << "[i_bin, j_bin] = [" << i_bin << ", " << j_bin << "]"
             << "  (h1, h2, rel.diff) = (" 
             << value1 << ", "
             << value2 << ", "
             << relDiff*100 << "%)" << endl;

        if( relDiff*100 > 0.001 ) // -- 0.001%
        {
          cout << "**********************************************" << endl;
          cout << "  Large (>0.001%) relative difference is found" <<endl;
          cout << "**********************************************" << endl;
        }
        // cout << endl;
      }
    }
  }

};

void DrawCovM()
{
  CovMatrices* covM_muon = new CovMatrices("Muon");
  covM_muon->DrawPlots();

  CovMatrices* covM_electron = new CovMatrices("Electron");
  covM_electron->DrawPlots();

  CovMatrices* covM_comb = new CovMatrices("Comb");
  covM_comb->DrawPlots();
}