#include <Include/SimplePlotTools.h>
#include <ElecChannel/Include/DYElectronTool.h>

class CovROOTFileMaker
{
public:
  vector<TString> vec_uncType_ = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};
  vector<TString> vec_inputFile_;

  Bool_t drawPlots_ = kTRUE;

  CovROOTFileMaker()
  {
    Init();
  }

  void SetInputFile(TString uncType, TString inputFile)
  {
    Int_t nUncType = (Int_t)vec_uncType_.size();
    for(Int_t i=0; i<nUncType; i++)
    {
      if( uncType == vec_uncType_[i] )
      {
        vec_inputFile_[i] = inputFile;

        cout << "[CovROOTFileMaker::SetInputFile] the input file for " << vec_uncType_[i] << " is set to be: " << vec_inputFile_[i] << endl;
        return;
      }
    }

    cout << "[CovROOTFileMaker::SetInputFile] uncType: " << uncType << " is not found! ... please check" << endl;
  }

  void Make()
  {
    TString fileName_output = "ROOTFile_CovM_EfficiencySF.root";
    TFile *f_output = TFile::Open(fileName_output, "RECREATE");

    vector<TString> vec_uncType_ = {"bkgChange", "sgnChange", "tagChange", "nlo", "stat"};

    vector<TH2D*> vec_cov_syst;

    Int_t nUncType = (Int_t)vec_uncType_.size();
    for(Int_t i=0; i<nUncType; i++)
    {
      TString uncType  = vec_uncType_[i];
      TString fileName = vec_inputFile_[i];

      TH2D* h_cov  = PlotTool::Get_Hist2D(fileName, "h_cov_"+uncType);
      TH2D* h_corr = PlotTool::Get_Hist2D(fileName, "h_corr_"+uncType);

      f_output->cd();
      h_cov->Write();
      h_corr->Write();

      if( uncType != "stat" )
        vec_cov_syst.push_back( h_cov );

      if( drawPlots_ )
      {
        DrawCovM(uncType, "cov",  h_cov);
        DrawCovM(uncType, "corr", h_corr);
      }
    }

    if( vec_cov_syst.size() != 4 )
    {
      cout << "vec_cov_syst.size() != 4" << endl;
      return;
    }

    TH2D* h_cov_syst_temp1 = PlotTool::Hist2DOperation("h_cov_syst_temp1", vec_cov_syst[0], vec_cov_syst[1], "+");
    TH2D* h_cov_syst_temp2 = PlotTool::Hist2DOperation("h_cov_syst_temp2", vec_cov_syst[2], vec_cov_syst[3], "+");

    TH2D* h_cov_syst  = PlotTool::Hist2DOperation("h_cov_syst", h_cov_syst_temp1, h_cov_syst_temp2, "+");
    TH2D* h_corr_syst = DYTool::Convert_CovMToCorrM(h_cov_syst, "h_corr_syst");

    if( drawPlots_ )
    {
      DrawCovM("totSyst", "cov",  h_cov_syst);
      DrawCovM("totSyst", "corr", h_corr_syst);
    }

    f_output->cd();
    h_cov_syst->Write();
    h_corr_syst->Write();

    f_output->Close();

    cout << "[CovROOTFileMaker:Make] All covariance matrices are saved in " << fileName_output << endl;
  }

  void DrawCovM(TString uncType, TString covType, TH2D* h2D )
  {
    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_"+uncType+"_"+covType, "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    c->SetLogx();
    c->SetLogy();

    TString axisTitle = "m(ee) [GeV]";

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

    if( covType == "corr" )
      h2D->GetZaxis()->SetRangeUser(-1, 1);

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

    c->SaveAs(".pdf");
  }

private:
  void Init()
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString baseDir = analyzerPath+"/ElecChannel/CovMatrix/EfficiencySF";

    Int_t nUncType = (Int_t)vec_uncType_.size();
    for(Int_t i=0; i<nUncType; i++)
    {
      TString fileName = baseDir+"/ROOTFile_CovMProducer_"+vec_uncType_[i]+".root";
      vec_inputFile_.push_back(fileName);
    }
  }
};

