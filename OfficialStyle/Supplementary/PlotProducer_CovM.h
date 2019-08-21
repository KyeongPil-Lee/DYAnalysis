#include <Include/PlotTools.h>

class PlotProducer_CovM
{
public:
  TString fileName_;
  TString channel_;

  // -- final covariance matrix, including luminosity
  TH2D* h_cov_tot_;
  TH2D* h_corr_tot_;

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

  TH2D* h_cov_lumi_;
  TH2D* h_corr_lumi_;

  Double_t relUnc_lumi_; // -- for the covariance matrix from the luminosity uncertainty

  // -- additional matrices (between e and mu channel)
  TH2D* h_covEM_noLumi_;

  PlotProducer_CovM(TString channel) { channel_ = channel; }

  void Set_FileName(TString fileName) { fileName_ = fileName; };

  void Produce()
  {
    Init();

    if( channel_ == "muon" || channel_ == "electron" )
    {
      // DrawPlot_2D( h_cov_statYield_,  "cov",  "statYield" );
      // DrawPlot_2D( h_corr_statYield_, "corr", "statYield" );

      // DrawPlot_2D( h_cov_statNonYield_,  "cov",  "statNonYield" );
      // DrawPlot_2D( h_corr_statNonYield_, "corr", "statNonYield" );

      // DrawPlot_2D( h_cov_systAcc_,  "cov",  "systAcc" );
      // DrawPlot_2D( h_corr_systAcc_, "corr", "systAcc" );

      // DrawPlot_2D( h_cov_systNonAcc_,  "cov",  "systNonAcc" );
      // DrawPlot_2D( h_corr_systNonAcc_, "corr", "systNonAcc" );

      // DrawPlot_2D( h_cov_lumi_,  "cov",  "lumi" );
      // DrawPlot_2D( h_corr_lumi_, "corr", "lumi" );

      // DrawPlot_2D( h_cov_noLumi_,  "cov",  "noLumi" );
      // DrawPlot_2D( h_corr_noLumi_, "corr", "noLumi" );

      DrawPlot_2D( h_cov_tot_,  "cov",  "tot" );
      DrawPlot_2D( h_corr_tot_, "corr", "tot" );

      // DrawPlot_2D( h_covEM_noLumi_, "cov", "EM_noLumi" );
    }
    else if( channel_ == "combined" )
    {
      DrawPlot_2D( h_cov_tot_,  "cov",  "tot" );
      DrawPlot_2D( h_corr_tot_, "corr", "tot" );
    }
  }

private:
  void DrawPlot_2D(TH2D* h2D, TString covType, TString uncType )
  {
    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_"+covType+"_"+uncType+"_"+channel_, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    c->SetLogx();
    c->SetLogy();

    TString axisTitle = "";
    if( channel_.Contains("electron") ) axisTitle = "m(ee) [GeV]";
    if( channel_.Contains("muon") )     axisTitle = "m(#mu#mu) [GeV]";
    if( channel_.Contains("combined") ) axisTitle = "m(ll) [GeV]";

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

    if( uncType == "EM_noLumi" )
    {
      h2D->GetXaxis()->SetTitle( "m(ee) [GeV]" );
      h2D->GetYaxis()->SetTitle( "m(#mu#mu) [GeV]" );
    }

    if( covType == "corr" ) h2D->GetZaxis()->SetRangeUser(-1, 1);

    if( covType == "cov" && uncType == "tot" )
    {
      // printf("(min, max) = (%lf, %lf)\n", h2D->GetMinimum(), h2D->GetMaximum());
      h2D->GetZaxis()->SetRangeUser(-0.1, 350);
    }

    h2D->Draw("COLZ");

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
    CovInfoLatex(latex, covType, uncType);

    c->SaveAs(".pdf");
  }

  void Init()
  {
    relUnc_lumi_ = 0.023;

    if( fileName_ == "" )
    {
      fileName_ = DefaultFile();
      cout << "Use DEFAULT file: " << fileName_ << endl;
    }
    else
      cout << "Use PRIVATE file: " << fileName_ << endl;

    TString dirName = "";
    if( channel_ == "muon" )     dirName = "mm";
    if( channel_ == "electron" ) dirName = "ee";
    if( channel_ == "combined" ) dirName = "ll";

    if( channel_ == "electron" || channel_ == "muon" )
    {
      h_cov_statYield_  = Get_Hist_2D(fileName_, dirName+"/h_cov_statYield");
      h_corr_statYield_ = Convert_CovMToCorrM(h_cov_statYield_, "h_corr_statYield");

      h_cov_statNonYield_  = Get_Hist_2D(fileName_, dirName+"/h_cov_statNonYield");
      h_corr_statNonYield_ = Convert_CovMToCorrM(h_cov_statNonYield_, "h_corr_statNonYield");

      h_cov_systAcc_  = Get_Hist_2D(fileName_, dirName+"/h_cov_systAcc");
      h_corr_systAcc_ = Convert_CovMToCorrM(h_cov_systAcc_, "h_corr_systAcc");

      h_cov_systNonAcc_  = Get_Hist_2D(fileName_, dirName+"/h_cov_systNonAcc");
      h_corr_systNonAcc_ = Convert_CovMToCorrM(h_cov_systNonAcc_, "h_corr_systNonAcc");

      // -- luminosity: not yet added here
      h_cov_noLumi_  = Get_Hist_2D(fileName_, dirName+"/h_cov_tot");
      h_cov_noLumi_->SetName("h_cov_noLumi");
      h_corr_noLumi_ = Convert_CovMToCorrM(h_cov_noLumi_, "h_corr_noLumi");

      // -- make lumi. cov.
      TH1D* h_dXSec = Get_Hist(fileName_, dirName+"/h_dXSec");
      h_cov_lumi_  = MakeCov_Lumi(h_dXSec, relUnc_lumi_, "h_cov_lumi");
      h_corr_lumi_ = Convert_CovMToCorrM(h_cov_lumi_, "h_corr_lumi");

      // -- h_cov_noLumi + h_cov_lumi -> final cov      
      h_cov_tot_  = Hist2DOperation( "h_cov_tot", h_cov_noLumi_, h_cov_lumi_, "+" );
      h_corr_tot_ = Convert_CovMToCorrM(h_cov_tot_, "h_corr_tot");

      h_covEM_noLumi_ = Get_Hist_2D( fileName_, "ll/h_covEM_noLumi" );
      h_covEM_noLumi_ = Convert_TH2DToMassBinnedTH2D(h_covEM_noLumi_); // -- axis: bin index -> dilepton mass
    }
    else if( channel_ == "combined" )
    {
      // -- from the hybrid method: luminosity is already taken into account
      h_cov_tot_  = Get_Hist_2D(fileName_, dirName+"/h_cov_tot");
      h_cov_tot_  = Convert_TH2DToMassBinnedTH2D(h_cov_tot_);

      h_corr_tot_ = Get_Hist_2D(fileName_, dirName+"/h_corr_tot");
      h_corr_tot_ = Convert_TH2DToMassBinnedTH2D(h_corr_tot_);
    }
  }

  void DataInfoLatex(TLatex &latex)
  {
    if( channel_ == "electron" )      latex.DrawLatexNDC(0.68, 0.96, "#font[42]{#scale[0.6]{2.3 fb^{-1} (13 TeV)}}");
    else if( channel_ == "muon" )     latex.DrawLatexNDC(0.68, 0.96, "#font[42]{#scale[0.6]{2.8 fb^{-1} (13 TeV)}}");
    else if( channel_ == "combined" ) latex.DrawLatexNDC(0.52, 0.96, "#font[42]{#scale[0.55]{2.3 fb^{-1}(ee), 2.8 fb^{-1}(#mu#mu) (13 TeV)}}");
  }

  void CovInfoLatex(TLatex& latex, TString covType, TString uncType)
  {
    if( uncType == "tot" )
    {
      if( covType == "cov" )  latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Total covariance}}");
      if( covType == "corr" ) latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Total correlation}}");
    }

    if( uncType == "noLumi" )
    {
      if( covType == "cov" )  latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Total covariance}}");
      if( covType == "corr" ) latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Total correlation}}");
      latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{without luminosity uncertainty}}");
    }

    if( uncType == "lumi" )
    {
      if( covType == "cov" )  latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Covariance of the luminosity uncertainty}}");
      if( covType == "corr" ) latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Correlation of the luminosity uncertainty}}");        
    }

    if( uncType == "statYield" || uncType == "statNonYield" || uncType == "systAcc" || uncType == "systNonAcc" )
    {
      if( covType == "cov" )  latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Covariance of the uncertainty}}");
      if( covType == "corr" ) latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{Correlation of the uncertainty}}");

      if( uncType == "statYield" )    latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{from the statistical yield of the measurement}}");
      if( uncType == "statNonYield" ) latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{from the statistical sources not related to the yield of the measurement}}");
      if( uncType == "systAcc" )      latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{from the systematic uncertainty on the acceptance}}");
      if( uncType == "systNonAcc" )   latex.DrawLatexNDC(0.16, 0.87, "#font[62]{#scale[0.5]{from the systematic uncertainty except for the acceptance}}");
    }

    if( uncType == "EM_noLumi" )
      latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.5]{cross-covariance between two channels}}");

  }

  TH2D* MakeCov_Lumi(TH1D* h_dXSec, Double_t relUnc_lumi, TString histName)
  {
    TH2D* h_cov_lumi = MakeHist2D_DXSecBin(histName);

    Int_t nBin = h_dXSec->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t dXSec_i = h_dXSec->GetBinContent(i_bin);
      Double_t absUnc_lumi_i = dXSec_i * relUnc_lumi;

      for(Int_t j=0; j<nBin; j++)
      {
        Int_t j_bin = j+1;
        Double_t dXSec_j = h_dXSec->GetBinContent(j_bin);
        Double_t absUnc_lumi_j = dXSec_j * relUnc_lumi;

        Double_t cov_ij = absUnc_lumi_i*absUnc_lumi_j; // -- as corr = 1 -> cov = corr * sigma_i * sigma_j = sigma_i * sigma_j

        h_cov_lumi->SetBinContent(i_bin, j_bin, cov_ij);
        h_cov_lumi->SetBinError(i_bin, 0);
      }
    }
    cout << "[PlotProducer_CovM::MakeCov_Lumi] The covariance from the luminosity uncertainty (" << relUnc_lumi << ") is made" << endl;

    return h_cov_lumi;
  }

  TString DefaultFile()
  {
    TString fileName = "";

    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

    if( channel_ == "muon" )
      fileName = analyzerPath+"/Combination/Converter_BLUEOutput/ROOTFile_hepdata__corr12Aug_Full_newEffSF_Converted.root";

    else if( channel_ == "electron" )
      fileName = analyzerPath+"/Combination/Converter_BLUEOutput/ROOTFile_hepdata__corr12Aug_Full_newEffSF_Converted.root";

    else if ( channel_ == "combined" )
      fileName = analyzerPath+"/Combination/ROOTFile_Combination.root";

    else
    {
      cout << "no corresponding fileName is found for " << channel_ << endl;
      fileName = "";
    }

    return fileName;
  }

  TH2D* Convert_CovMToCorrM( TH2D* h_cov, TString histName )
  {
    TH2D* h_corr = (TH2D*)h_cov->Clone();
    h_corr->SetName(histName);
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
        if( (corr - 1.0) > 1e-3 )
        {
          cout << "Correlation = " << corr << " exceeds 1 !!! ... something wrong happens" << endl;
          printf("(%d, %d) (cov, sigma_i, sigma_j, sigma_i*sigma_j) = (%lf,  %lf, %lf, %lf) -> corr = %lf\n",
                   i, j, cov, sigma_i, sigma_j, sigma_i*sigma_j, corr);
          return nullptr;
        }

        // printf("(%d, %d) (cov, sigma_i, sigma_j, sigma_i*sigma_j) = (%lf,  %lf, %lf, %lf) -> corr = %lf\n",
        //          i, j, cov, sigma_i, sigma_j, sigma_i*sigma_j, corr);

        // if( i == j )
        //   cout << "i = j = " << i << " -> corr = " << corr << endl;

        h_corr->SetBinContent(i_bin, j_bin, corr);
      }
    }

    return h_corr;
  }

  TH2D* Hist2DOperation(TString histName, TH2D* h1, TH2D* h2, TString operation)
  {
    if( !(operation == "+" || operation == "-" || operation == "*" || operation == "/") )
    {
      cout << "[HistOperation] operation = " << operation << " is not supported ... return nullptr" << endl;
      return nullptr;
    }

    Int_t nBinX1 = h1->GetNbinsX();
    Int_t nBinX2 = h2->GetNbinsX();
    if( nBinX1 != nBinX2 )
    {
      printf("[HistOperation] (nBinX1, nBinX2) = (%d, %d): not same ... return nullptr\n", nBinX1, nBinX2);
      return nullptr;
    }

    Int_t nBinY1 = h1->GetNbinsY();
    Int_t nBinY2 = h2->GetNbinsY();
    if( nBinY1 != nBinY2 )
    {
      printf("[HistOperation] (nBinY1, nBinY2) = (%d, %d): not same ... return nullptr\n", nBinY1, nBinY2);
      return nullptr;
    }

    TH2D* h_return = (TH2D*)h1->Clone();
    h_return->SetName(histName);

    for(Int_t i_x=0; i_x<nBinX1; i_x++)
    {
      Int_t i_binX = i_x+1;

      for(Int_t i_y=0; i_y<nBinY1; i_y++)
      {
        Int_t i_binY = i_y+1;

        Double_t value1 = h1->GetBinContent(i_binX, i_binY);
        Double_t value2 = h2->GetBinContent(i_binX, i_binY);

        Double_t value_return = -1;

        if( operation == "+" ) value_return = value1 + value2;
        if( operation == "-" ) value_return = value1 - value2;
        if( operation == "*" ) value_return = value1 * value2;
        if( operation == "/" ) value_return = value1 / value2;

        h_return->SetBinContent(i_binX, i_binY, value_return);
        h_return->SetBinError(i_binX, i_binY, 0); // -- no error propagation considered for now
      }
    }

    return h_return;
  }

  // -- used for cov. M
  TH2D* MakeHist2D_DXSecBin(TString histName)
  {
    Double_t arr_massBinEdge[44] = {15,20,25,30,35,40,45,50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,126,133,141,150,160,171,185,200,220,243,273,320,380,440,510,600,700,830,1000,1500,3000};
    return new TH2D(histName, "", 43, arr_massBinEdge, 43, arr_massBinEdge);
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

};
