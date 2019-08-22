#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>

class ConversionTool
{
public:
  TString channel_;
  TFile *f_output_;

  TH1D* h_dXSec_;

  TH1D* h_relUnc_stat_;
  TH1D* h_absUnc_stat_;

  TH1D* h_relUnc_lumi_;
  TH1D* h_absUnc_lumi_;

  TH1D* h_relUnc_syst_eff_;
  TH1D* h_absUnc_syst_eff_;

  TH1D* h_relUnc_syst_detRes_;
  TH1D* h_absUnc_syst_detRes_;

  TH1D* h_relUnc_syst_bkg_;
  TH1D* h_absUnc_syst_bkg_;

  TH1D* h_relUnc_syst_FSR_;
  TH1D* h_absUnc_syst_FSR_;

  TH1D* h_relUnc_syst_tot_;
  TH1D* h_absUnc_syst_tot_;

  TH1D* h_relUnc_syst_accPDF_;
  TH1D* h_absUnc_syst_accPDF_;

  TH1D* h_relUnc_exp_; // -- lumi. + syst. except for accPDF
  TH1D* h_absUnc_exp_;

  TH1D* h_relUnc_tot_; // -- lumi. + stat. + syst. + accPDF
  TH1D* h_absUnc_tot_;


  TH1D* h_FpoF_dXSec_;

  TH1D* h_FpoF_relUnc_stat_;
  TH1D* h_FpoF_absUnc_stat_;

  TH1D* h_FpoF_absUnc_lumi_;
  TH1D* h_FpoF_absUnc_syst_eff_;
  TH1D* h_FpoF_absUnc_syst_detRes_;
  TH1D* h_FpoF_absUnc_syst_bkg_;

  TH1D* h_FpoF_relUnc_syst_tot_;
  TH1D* h_FpoF_absUnc_syst_tot_;

  TH1D* h_FpoF_relUnc_exp_;
  TH1D* h_FpoF_absUnc_exp_;

  TH1D* h_FpoF_relUnc_tot_;
  TH1D* h_FpoF_absUnc_tot_;

  TH2D* h_cov_noLumi_; // -- for the muon & electron channel results
  TH2D* h_cov_; // -- for the combined results

  ConversionTool(TString channel)
  {
    channel_ = channel;
    Init();
  }

  void ConvertAndSave_All()
  {
    if( channel_ == "muon" || channel_ == "electron" )
    {
      ConvertAndSave(h_dXSec_);
      ConvertAndSave(h_relUnc_stat_);
      ConvertAndSave(h_absUnc_stat_);

      ConvertAndSave(h_relUnc_lumi_);
      ConvertAndSave(h_absUnc_lumi_);

      ConvertAndSave(h_relUnc_syst_eff_);
      ConvertAndSave(h_absUnc_syst_eff_);

      ConvertAndSave(h_relUnc_syst_detRes_);
      ConvertAndSave(h_absUnc_syst_detRes_);

      ConvertAndSave(h_relUnc_syst_bkg_);
      ConvertAndSave(h_absUnc_syst_bkg_);

      ConvertAndSave(h_relUnc_syst_FSR_);
      ConvertAndSave(h_absUnc_syst_FSR_);

      ConvertAndSave(h_relUnc_syst_tot_);
      ConvertAndSave(h_absUnc_syst_tot_);

      ConvertAndSave(h_relUnc_syst_accPDF_);
      ConvertAndSave(h_absUnc_syst_accPDF_);

      ConvertAndSave(h_relUnc_exp_);
      ConvertAndSave(h_absUnc_exp_);

      ConvertAndSave(h_relUnc_tot_);
      ConvertAndSave(h_absUnc_tot_);


      ConvertAndSave(h_FpoF_dXSec_);

      ConvertAndSave(h_FpoF_relUnc_stat_);
      ConvertAndSave(h_FpoF_absUnc_stat_);

      ConvertAndSave(h_FpoF_absUnc_lumi_);
      ConvertAndSave(h_FpoF_absUnc_syst_eff_);
      ConvertAndSave(h_FpoF_absUnc_syst_detRes_);
      ConvertAndSave(h_FpoF_absUnc_syst_bkg_);

      ConvertAndSave(h_FpoF_relUnc_syst_tot_);
      ConvertAndSave(h_FpoF_absUnc_syst_tot_);

      ConvertAndSave(h_FpoF_relUnc_exp_);
      ConvertAndSave(h_FpoF_absUnc_exp_);

      ConvertAndSave(h_FpoF_relUnc_tot_);
      ConvertAndSave(h_FpoF_absUnc_tot_);

      h_cov_noLumi_->SetName("h_cov_noLumi");
      h_cov_noLumi_->Write();
    }
    else if( channel_ == "combined" )
    {
      ConvertAndSave(h_dXSec_);

      ConvertAndSave(h_relUnc_tot_);
      ConvertAndSave(h_absUnc_tot_);

      h_cov_->SetName("h_cov");
      h_cov_->Write();
    }

    f_output_->Close();
  }

private:
  void Init()
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName_input = "";
    if( channel_ == "muon" )
      fileName_input = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root";
    else if( channel_ == "electron" )
      fileName_input = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Electron.root";
    else if( channel_ == "combined" )
      fileName_input = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Combined.root";

    cout << "[Input file for " << channel_ << "] " << fileName_input << endl;

    if( channel_ == "muon" || channel_ == "electron" )
    {
      h_dXSec_ = Get_Hist(fileName_input, "h_dXSec");

      h_relUnc_stat_ = Get_Hist(fileName_input, "h_relUnc_stat");
      h_absUnc_stat_ = Get_Hist(fileName_input, "h_absUnc_stat");

      h_relUnc_lumi_ = Get_Hist(fileName_input, "h_relUnc_lumi");
      h_absUnc_lumi_ = Get_Hist(fileName_input, "h_absUnc_lumi");

      h_relUnc_syst_eff_ = Get_Hist(fileName_input, "h_relUnc_syst_eff");
      h_absUnc_syst_eff_ = Get_Hist(fileName_input, "h_absUnc_syst_eff");

      h_relUnc_syst_detRes_ = Get_Hist(fileName_input, "h_relUnc_syst_detRes");
      h_absUnc_syst_detRes_ = Get_Hist(fileName_input, "h_absUnc_syst_detRes");

      h_relUnc_syst_bkg_ = Get_Hist(fileName_input, "h_relUnc_syst_bkg");
      h_absUnc_syst_bkg_ = Get_Hist(fileName_input, "h_absUnc_syst_bkg");

      h_relUnc_syst_FSR_ = Get_Hist(fileName_input, "h_relUnc_syst_FSR");
      h_absUnc_syst_FSR_ = Get_Hist(fileName_input, "h_absUnc_syst_FSR");

      h_relUnc_syst_tot_ = Get_Hist(fileName_input, "h_relUnc_syst_tot");
      h_absUnc_syst_tot_ = Get_Hist(fileName_input, "h_absUnc_syst_tot");

      h_relUnc_syst_accPDF_ = Get_Hist(fileName_input, "h_relUnc_syst_accPDF");
      h_absUnc_syst_accPDF_ = Get_Hist(fileName_input, "h_absUnc_syst_accPDF");

      h_relUnc_exp_ = Get_Hist(fileName_input, "h_relUnc_exp");
      h_absUnc_exp_ = Get_Hist(fileName_input, "h_absUnc_exp");

      h_relUnc_tot_ = Get_Hist(fileName_input, "h_relUnc_tot");
      h_absUnc_tot_ = Get_Hist(fileName_input, "h_absUnc_tot");


      h_FpoF_dXSec_ = Get_Hist(fileName_input, "h_FpoF_dXSec");

      h_FpoF_relUnc_stat_ = Get_Hist(fileName_input, "h_FpoF_relUnc_stat");
      h_FpoF_absUnc_stat_ = Get_Hist(fileName_input, "h_FpoF_absUnc_stat");

      h_FpoF_absUnc_lumi_        = Get_Hist(fileName_input, "h_FpoF_absUnc_lumi");
      h_FpoF_absUnc_syst_eff_    = Get_Hist(fileName_input, "h_FpoF_absUnc_syst_eff");
      h_FpoF_absUnc_syst_detRes_ = Get_Hist(fileName_input, "h_FpoF_absUnc_syst_detRes");
      h_FpoF_absUnc_syst_bkg_    = Get_Hist(fileName_input, "h_FpoF_absUnc_syst_bkg");

      h_FpoF_relUnc_syst_tot_ = Get_Hist(fileName_input, "h_FpoF_relUnc_syst_tot");
      h_FpoF_absUnc_syst_tot_ = Get_Hist(fileName_input, "h_FpoF_absUnc_syst_tot");

      h_FpoF_relUnc_exp_ = Get_Hist(fileName_input, "h_FpoF_relUnc_exp");
      h_FpoF_absUnc_exp_ = Get_Hist(fileName_input, "h_FpoF_absUnc_exp");

      h_FpoF_relUnc_tot_ = Get_Hist(fileName_input, "h_FpoF_relUnc_tot");
      h_FpoF_absUnc_tot_ = Get_Hist(fileName_input, "h_FpoF_absUnc_tot");

      // -- covariance matrix
      TString fileName_cov = analyzerPath+"/Combination/Converter_BLUEOutput/ROOTFile_hepdata__corr12Aug_Full_newEffSF_Converted.root";
      cout << "[Cov. Input file for " << channel_ << "] " << fileName_cov << endl;

      if( channel_ == "muon" )         h_cov_noLumi_ = Get_Hist_2D(fileName_cov, "mm/h_cov_tot"); // -- it doesn't include lumi. unc. (because the original file was made w/o lumi.)
      else if( channel_ == "electron") h_cov_noLumi_ = Get_Hist_2D(fileName_cov, "ee/h_cov_tot"); // -- it doesn't include lumi. unc. (because the original file was made w/o lumi.)
    }
    else if( channel_ == "combined" )
    {
      h_dXSec_ = Get_Hist(fileName_input, "h_dXSec");

      h_relUnc_tot_ = Get_Hist(fileName_input, "h_relUnc_tot");
      h_absUnc_tot_ = Get_Hist(fileName_input, "h_absUnc_tot");

      // -- covariance matrix
      TString fileName_cov = analyzerPath+"/Combination/ROOTFile_Combination.root";
      cout << "[Cov. Input file for " << channel_ << "] " << fileName_cov << endl;

      h_cov_ = Get_Hist_2D(fileName_cov, "ll/h_cov_tot");
      h_cov_ = Convert_TH2DToMassBinnedTH2D(h_cov_);
    }
    cout << endl;

    TString fileName_output = "ROOTFile_InputForHEPData_Upto2SigDigits_"+channel_+".root";
    f_output_ = TFile::Open(fileName_output, "RECREATE");
  }

  void ConvertAndSave(TH1D* h_before)
  {
    TH1D* h_after = (TH1D*)h_before->Clone();

    Int_t nBin = h_after->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t before = h_before->GetBinContent(i_bin);

      Double_t after = Convert_TwoSignificantDigits(before);
      h_after->SetBinContent(i_bin, after);
    }

    f_output_->cd();
    h_after->Write();
  }


  Double_t Convert_TwoSignificantDigits(Double_t rawValue)
  {
    Int_t exponent = Calc_Exponent( rawValue );
    Double_t front = rawValue / TMath::Power(10, exponent);
    if( front >= 9.95 && front < 10.05 ) // -- if rounded off to 10.0: change it to 1.0 x 10^1
    {
      front = 1.0;
      exponent = exponent + 1;
    }
    // -- number: front * 10^(exponent)

    Double_t adjustedValue = Convert_KeepUptoOneDecimalPoint(front) * TMath::Power(10, exponent);

    // printf("[Convert_TwoSignificantDigits] %g -> %g\n", rawValue, adjustedValue);

    return adjustedValue;
  }

  Int_t Calc_Exponent( Double_t x )
  {
    if( x >= 1 )
      return (Int_t)TMath::Log10(x);

    else if( x > 0 && x < 1 )
      return (Int_t)TMath::Log10(x) - 1;
    
    else
    {
      cout << "Can not calculate the exponent of negative value!";
      return -999;
    }
  }

  Double_t Convert_KeepUptoOneDecimalPoint(Double_t rawValue)
  {
    Double_t temp = rawValue*10; // e.g. rawValue = 1.293 -> temp = 12.93
    Double_t temp2 = std::round(rawValue*10); // -- e.g. temp2 = 13
    Double_t returnValue = temp2 / 10.0; // -- e.g. returnValue = 1.3: what we want

    return returnValue;
  }

  TH2D* Convert_TH2DToMassBinnedTH2D(TH2D* h_before)
  {
    TString histName_before = h_before->GetName();

    // const Int_t nMassBin = 43;
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

void Convert_TwoSigDigits()
{
  ConversionTool* tool_muon = new ConversionTool("muon");
  tool_muon->ConvertAndSave_All();

  ConversionTool* tool_electron = new ConversionTool("electron");
  tool_electron->ConvertAndSave_All();

  ConversionTool* tool_combined = new ConversionTool("combined");
  tool_combined->ConvertAndSave_All();
}