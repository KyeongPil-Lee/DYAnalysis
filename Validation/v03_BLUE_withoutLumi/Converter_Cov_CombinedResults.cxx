#include <Include/PlotTools.h>

class Converter
{
public:
  TString inputFileName_;
  TString inputFileName_2_;

  TString outputFileName_;

  TFile *f_output_;

  Converter(TString input, TString input2, TString output)
  {
    inputFileName_   = input;
    inputFileName_2_ = input2;
    outputFileName_  = output;
    f_output_ = TFile::Open(outputFileName_, "RECREATE");
  }

  void Convert()
  {
    Convert_eachChannel("ee");
    Convert_eachChannel("mm");
    Convert_eachChannel("ll");

    f_output_->Close();

    cout << "output file name: " << outputFileName_ << endl;
  }

private:
  TH2D* GetTH2DInCanvas( TFile *f_input, TString canvasName, TString padName, TString histName )
  {
    TCanvas *c = (TCanvas*)f_input->Get(canvasName)->Clone();
    c->cd();
    TPad* pad = (TPad*)c->GetPrimitive(padName)->Clone();

    return (TH2D*)pad->GetPrimitive(histName)->Clone();
  }

  void Convert_eachChannel(TString channel)
  {
    // -- make a subdirectory
    f_output_->mkdir(channel);


    TString postFix = "";
    if( channel == "ee" ) postFix = "EE";
    if( channel == "mm" ) postFix = "MM";
    if( channel == "ll" ) postFix = "LL";

    TH1D* h_dXSec = Get_Hist(inputFileName_, "h1cs"+postFix);

    // TString preFix = "h1"+channel; // -- h1ee, h1mm, h1ll

    // TH1D* h_absUnc_stat   = Get_Hist(inputFileName_, preFix+"_varYield_absUnc");
    // TH1D* h_absUnc_bkg    = Get_Hist(inputFileName_, preFix+"_varBkg_absUnc");
    // TH1D* h_absUnc_detRes = Get_Hist(inputFileName_, preFix+"_varDetRes_absUnc");
    // TH1D* h_absUnc_FSR    = Get_Hist(inputFileName_, preFix+"_varFSRRes_absUnc");
    // TH1D* h_absUnc_effSF  = Get_Hist(inputFileName_, preFix+"_varRhoFile_absUnc");

    // TH1D* h_absUnc_tot = nullptr;
    // if( channel == "ee" || channel == "mm" )
    //   h_absUnc_tot = Get_Hist(inputFileName_, preFix+"_tot_absUnc");
    // if( channel == "ll" )
    //   h_absUnc_tot = Get_Hist(inputFileName_, preFix+"_varTot_absUnc");

    // TH1D* h_absUnc_lumi   = Get_Hist(inputFileName_, preFix+"_varLumi_absUnc");
    // TH1D* h_absUnc_accPDF = Get_Hist(inputFileName_, preFix+"_varAccSyst_absUnc");

    h_dXSec->SetName("h_dXSec");
    // h_absUnc_stat->SetName("h_absUnc_stat");
    // h_absUnc_bkg->SetName("h_absUnc_bkg");
    // h_absUnc_detRes->SetName("h_absUnc_detRes");
    // h_absUnc_FSR->SetName("h_absUnc_FSR");
    // h_absUnc_effSF->SetName("h_absUnc_effSF");
    // h_absUnc_tot->SetName("h_absUnc_tot");
    // h_absUnc_lumi->SetName("h_absUnc_lumi");
    // h_absUnc_accPDF->SetName("h_absUnc_accPDF");

    f_output_->cd(channel);
    h_dXSec->Write();
    // h_absUnc_stat->Write();
    // h_absUnc_bkg->Write();
    // h_absUnc_detRes->Write();
    // h_absUnc_FSR->Write();
    // h_absUnc_effSF->Write();
    // h_absUnc_tot->Write();
    // h_absUnc_lumi->Write();
    // h_absUnc_accPDF->Write();

    delete h_dXSec;
    // delete h_absUnc_stat;
    // delete h_absUnc_bkg;
    // delete h_absUnc_detRes;
    // delete h_absUnc_FSR;
    // delete h_absUnc_effSF;
    // delete h_absUnc_tot;
    // delete h_absUnc_lumi;
    // delete h_absUnc_accPDF;

    // if( channel == "ll" )
    // {
    //   TH1D* h_relUnc_stat   = Get_Hist(inputFileName_, preFix+"_varYield_relUnc");
    //   TH1D* h_relUnc_bkg    = Get_Hist(inputFileName_, preFix+"_varBkg_relUnc");
    //   TH1D* h_relUnc_detRes = Get_Hist(inputFileName_, preFix+"_varDetRes_relUnc");
    //   TH1D* h_relUnc_FSR    = Get_Hist(inputFileName_, preFix+"_varFSRRes_relUnc");
    //   TH1D* h_relUnc_effSF  = Get_Hist(inputFileName_, preFix+"_varRhoFile_relUnc");
    //   TH1D* h_relUnc_tot    = Get_Hist(inputFileName_, preFix+"_varTot_relUnc");
    //   TH1D* h_relUnc_lumi   = Get_Hist(inputFileName_, preFix+"_varLumi_relUnc");
    //   TH1D* h_relUnc_accPDF = Get_Hist(inputFileName_, preFix+"_varAccSyst_relUnc");

    //   h_relUnc_stat->SetName("h_relUnc_stat");
    //   h_relUnc_bkg->SetName("h_relUnc_bkg");
    //   h_relUnc_detRes->SetName("h_relUnc_detRes");
    //   h_relUnc_FSR->SetName("h_relUnc_FSR");
    //   h_relUnc_effSF->SetName("h_relUnc_effSF");
    //   h_relUnc_tot->SetName("h_relUnc_tot");
    //   h_relUnc_lumi->SetName("h_relUnc_lumi");
    //   h_relUnc_accPDF->SetName("h_relUnc_accPDF");

    //   f_output_->cd(channel);
    //   h_relUnc_stat->Write();
    //   h_relUnc_bkg->Write();
    //   h_relUnc_detRes->Write();
    //   h_relUnc_FSR->Write();
    //   h_relUnc_effSF->Write();
    //   h_relUnc_tot->Write();
    //   h_relUnc_lumi->Write();
    //   h_relUnc_accPDF->Write();

    //   delete h_relUnc_stat;
    //   delete h_relUnc_bkg;
    //   delete h_relUnc_detRes;
    //   delete h_relUnc_FSR;
    //   delete h_relUnc_effSF;
    //   delete h_relUnc_tot;
    //   delete h_relUnc_lumi;
    //   delete h_relUnc_accPDF;
    // }

    ///////////////////////////////
    // -- covariance matrices -- //
    ///////////////////////////////
    TString postFixCov = channel+"Cov_onFile"; // -- e.g. mmCov_onFile

    if( channel == "mm" || channel == "ee" )
    {
      // TFile *f_input = TFile::Open(inputFileName_);
      // TH2D* h_cov_statYield    = GetTH2DInCanvas(f_input, "c2CovStatYield_"+postFixCov,    "c2CovStatYield_"+postFixCov+"_1",    "h2CovStatYield_"+postFixCov);
      // TH2D* h_cov_statNonYield = GetTH2DInCanvas(f_input, "c2CovStatNonYield_"+postFixCov, "c2CovStatNonYield_"+postFixCov+"_1", "h2CovStatNonYield_"+postFixCov);
      // TH2D* h_cov_systAcc      = GetTH2DInCanvas(f_input, "c2CovSystAcc_"+postFixCov,      "c2CovSystAcc_"+postFixCov+"_1",      "h2CovSystAcc_"+postFixCov);
      // TH2D* h_cov_systNonAcc   = GetTH2DInCanvas(f_input, "c2CovSystNonAcc_"+postFixCov,   "c2CovSystNonAcc_"+postFixCov+"_1",   "h2CovSystNonAcc_"+postFixCov);
      // TH2D* h_cov_tot          = GetTH2DInCanvas(f_input, "c2CovTot_"+postFixCov,          "c2CovTot_"+postFixCov+"_1",          "h2CovTot_"+postFixCov);
      // TH2D* h_corr_tot         = GetTH2DInCanvas(f_input, "c2CovTot_"+postFixCov,          "c2CovTot_"+postFixCov+"_2",          "h2CovTot_"+postFixCov+"_corr");

      // h_cov_statYield->SetName("h_cov_statYield");
      // h_cov_statNonYield->SetName("h_cov_statNonYield");
      // h_cov_systAcc->SetName("h_cov_systAcc");
      // h_cov_systNonAcc->SetName("h_cov_systNonAcc");
      // h_cov_tot->SetName("h_cov_tot");
      // h_corr_tot->SetName("h_corr_tot");

      // f_output_->cd(channel);
      // h_cov_statYield->Write();
      // h_cov_statNonYield->Write();
      // h_cov_systAcc->Write();
      // h_cov_systNonAcc->Write();
      // h_cov_tot->Write();
      // h_corr_tot->Write();

      // delete h_cov_statYield;
      // delete h_cov_statNonYield;
      // delete h_cov_systAcc;
      // delete h_cov_systNonAcc;
      // delete h_cov_tot;
      // delete h_corr_tot;
    }
    else if( channel == "ll" )
    {
      // TFile *f_input = TFile::Open(inputFileName_2_);

      // TH2D* h_cov_statYield    = GetTH2DInCanvas(f_input, "cCovFin_from_statYield",    "cCovFin_from_statYield_1",    "h2covFin_fromstatYield");
      // TH2D* h_cov_statNonYield = GetTH2DInCanvas(f_input, "cCovFin_from_statNonYield", "cCovFin_from_statNonYield_1", "h2covFin_fromstatNonYield");
      // TH2D* h_cov_systAcc      = GetTH2DInCanvas(f_input, "cCovFin_from_systAcc",      "cCovFin_from_systAcc_1",      "h2covFin_fromsystAcc");
      // TH2D* h_cov_systNonAcc   = GetTH2DInCanvas(f_input, "cCovFin_from_systNonAcc",   "cCovFin_from_systNonAcc_1",   "h2covFin_fromsystNonAcc");
      // TH2D* h_cov_tot_noLumi   = GetTH2DInCanvas(f_input, "cFinCovNoLumi",             "cFinCovNoLumi_1",             "h2finCovNoLumi");
      // TH2D* h_cov_tot          = GetTH2DInCanvas(f_input, "cCovFin",                   "cCovFin_1",                   "h2covFin");
      // TH2D* h_corr_tot         = GetTH2DInCanvas(f_input, "cCovFin",                   "cCovFin_2",                   "h2covFin_corr");

      // h_cov_statYield->SetName("h_cov_statYield");
      // h_cov_statNonYield->SetName("h_cov_statNonYield");
      // h_cov_systAcc->SetName("h_cov_systAcc");
      // h_cov_systNonAcc->SetName("h_cov_systNonAcc");
      // h_cov_tot_noLumi->SetName("h_cov_tot_noLumi");
      // h_cov_tot->SetName("h_cov_tot");
      // h_corr_tot->SetName("h_corr_tot");

 
      // f_output_->cd(channel);
      // h_cov_statYield->Write();
      // h_cov_statNonYield->Write();
      // h_cov_systAcc->Write();
      // h_cov_systNonAcc->Write();
      // h_cov_tot_noLumi->Write();
      // h_cov_tot->Write();
      // h_corr_tot->Write();

      // delete h_cov_statYield;
      // delete h_cov_statNonYield;
      // delete h_cov_systAcc;
      // delete h_cov_systNonAcc;
      // delete h_cov_tot_noLumi;
      // delete h_cov_tot;
      // delete h_corr_tot;
    }
  }
};

void Converter_Cov_CombinedResults()
{
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  // TString inputFileName1 = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208.root";
  TString inputFileName1 = "ROOTFile_hepdata__corrnoLumi.root";
  // TString inputFileName2 = rootFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20190208hepdata.root"; // -- have final cov. for the combined results
  TString inputFileName2 = ""; // -- have final cov. for the combined results

  // TString outputFileName = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
  TString outputFileName = "ROOTFile_hepdata__corrnoLumi_converted.root";

  Converter *converter = new Converter(inputFileName1, inputFileName2, outputFileName);
  converter->Convert();
}