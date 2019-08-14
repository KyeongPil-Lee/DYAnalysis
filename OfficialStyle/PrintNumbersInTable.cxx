#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>

class PrintTool
{
public:
  TString channel_;

  TH1D* h_diffXSec_;
  TH1D* h_relUnc_stat_;
  TH1D* h_relUnc_lumi_;

  TH1D* h_FpoF_diffXSec_;
  TH1D* h_FpoF_relUnc_stat_;
  TH1D* h_FpoF_relUnc_syst_tot_;

  // TH1D* h_relUnc_syst_momEngCorr_; // -- muon channel: momentum correction / electron channel: energy smearing correction -> not used anymore in the table
  TH1D* h_relUnc_syst_eff_;
  TH1D* h_relUnc_syst_detRes_;
  TH1D* h_relUnc_syst_bkg_;
  TH1D* h_relUnc_syst_FSR_;
  TH1D* h_relUnc_syst_tot_; // -- except for acceptance & PDFs (as same with the table)
  TH1D* h_relUnc_syst_accPDF_;

  PrintTool() {}
  PrintTool(TString channel)
  {
    channel_ = channel;
    Init();
  }

  void Print_All()
  {
    if( channel_ == "Electron" || channel_ == "Muon" )
    {
      Print_Unc();
      Print_Unc_texFormat();
      Print_DXSecEachChannel();
      Print_DXSecEachChannel_texFormat();
      Print_FpoF_DXSecEachChannel();
      Print_FpoF_DXSecEachChannel_texFormat();
    }
    else if( channel_ == "Combined" )
    {
      Print_DXSecCombined();
      Print_DXSecCombined_texFormat();
    }
  }

private:
  void Print_Unc()
  {
    printf("========== Systematic uncertainties from each source ==========\n");
    printf("                     ");
    printf("%10s", "Eff" );
    printf("%10s", "DetRes" );
    printf("%10s", "Bkg" );
    printf("%10s", "FSR" );
    printf("%10s", "Total" );
    printf("%10s", "Acc+PDF" );
    printf("\n");

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t lowerEdge = h_relUnc_syst_eff_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_relUnc_syst_eff_->GetBinLowEdge(i_bin+1);

      printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

      // printf("%10.2lf", h_relUnc_syst_momEngCorr_->GetBinContent(i_bin) );
      printf("%10.1lf", h_relUnc_syst_eff_->GetBinContent(i_bin) );
      printf("%10.1lf", h_relUnc_syst_detRes_->GetBinContent(i_bin) );
      printf("%10.1lf", h_relUnc_syst_bkg_->GetBinContent(i_bin) );
      printf("%10.1lf", h_relUnc_syst_FSR_->GetBinContent(i_bin) );
      printf("%10.1lf", h_relUnc_syst_tot_->GetBinContent(i_bin) );
      printf("%10.1lf", h_relUnc_syst_accPDF_->GetBinContent(i_bin) );
      printf("\n");
    }
  }

  void Print_Unc_texFormat()
  {
    cout << "============== Uncertainties (.tex format) ==============" << endl;    
    printf("           Eff         DetRes        Bkg          FSR         Total        Acc+PDF\n");

    // printf("%13s", "Eff" );
    // printf("%13s", "DetRes" );
    // printf("%13s", "Bkg" );
    // printf("%13s", "FSR" );
    // printf("%13s", "Total" );
    // printf("%13s", "Acc+PDF" );
    // printf("\n");


    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t lowerEdge = h_relUnc_syst_eff_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_relUnc_syst_eff_->GetBinLowEdge(i_bin+1);
      
      TString str_syst = TString::Format("%.0d--%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);
      // str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_relUnc_syst_momEngCorr_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.1lf  $", h_relUnc_syst_eff_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.1lf  $", h_relUnc_syst_detRes_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.1lf  $", h_relUnc_syst_bkg_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.1lf  $", h_relUnc_syst_FSR_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.1lf  $", h_relUnc_syst_tot_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.1lf  $", h_relUnc_syst_accPDF_->GetBinContent(i_bin) );
      str_syst = str_syst + "\\\\";

      // printf( "%s\n\\hline\n", str_syst.Data() );
      printf( "%s\n", str_syst.Data() );
    }
    cout << "===========================================================================" << endl;
  }

  void Print_DXSecEachChannel()
  {
    printf("========== Cross section + uncertainties ==========\n");
    printf("                     ");
    printf("%15s", "dSigma/dM" );
    printf("%15s", "Stat.");
    printf("%15s", "Exp.");
    printf("%15s", "Theo.");
    printf("%15s", "Total");
    printf("\n");

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t lowerEdge = h_diffXSec_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_diffXSec_->GetBinLowEdge(i_bin+1);

      printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

      Double_t diffXSec = h_diffXSec_->GetBinContent(i_bin);

      Double_t relUnc_stat = h_relUnc_stat_->GetBinContent(i_bin);
      Double_t absUnc_stat = (relUnc_stat * diffXSec) / 100.0;

      Double_t relUnc_syst_tot = h_relUnc_syst_tot_->GetBinContent(i_bin);
      Double_t relUnc_lumi = h_relUnc_lumi_->GetBinContent(i_bin);
      // -- experimental uncertainty: systematic & luminosity uncertainty
      Double_t relUnc_exp = sqrt( relUnc_syst_tot*relUnc_syst_tot + relUnc_lumi*relUnc_lumi );
      Double_t absUnc_exp = ( relUnc_exp * diffXSec ) / 100.0;

      Double_t relUnc_theo = h_relUnc_syst_accPDF_->GetBinContent(i_bin);
      Double_t absUnc_theo = ( relUnc_theo * diffXSec ) / 100.0;

      Double_t relUnc_tot = sqrt(
        relUnc_stat * relUnc_stat +
        relUnc_exp  * relUnc_exp +
        relUnc_theo * relUnc_theo );

      Double_t absUnc_tot = ( relUnc_tot * diffXSec ) / 100.0;

      printf("%15.1e", diffXSec );
      printf("%15.1e", absUnc_stat );
      printf("%15.1e", absUnc_exp );
      printf("%15.1e", absUnc_theo );
      printf("%15.1e", absUnc_tot );

      printf("\n");
    }
  }

  void Print_DXSecEachChannel_texFormat()
  {
    printf("========== Cross section + uncertainties (.tex format) ==========\n");

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t lowerEdge = h_diffXSec_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_diffXSec_->GetBinLowEdge(i_bin+1);

      Double_t diffXSec = h_diffXSec_->GetBinContent(i_bin);

      Double_t relUnc_stat = h_relUnc_stat_->GetBinContent(i_bin);
      Double_t absUnc_stat = (relUnc_stat * diffXSec) / 100.0;

      Double_t relUnc_syst_tot = h_relUnc_syst_tot_->GetBinContent(i_bin);
      Double_t relUnc_lumi = h_relUnc_lumi_->GetBinContent(i_bin);
      // -- experimental uncertainty: systematic & luminosity uncertainty
      Double_t relUnc_exp = sqrt( relUnc_syst_tot*relUnc_syst_tot + relUnc_lumi*relUnc_lumi );
      Double_t absUnc_exp = ( relUnc_exp * diffXSec ) / 100.0;

      Double_t relUnc_theo = h_relUnc_syst_accPDF_->GetBinContent(i_bin);
      Double_t absUnc_theo = ( relUnc_theo * diffXSec ) / 100.0;

      Double_t relUnc_tot = sqrt(
        relUnc_stat * relUnc_stat +
        relUnc_exp  * relUnc_exp +
        relUnc_theo * relUnc_theo );

      Double_t absUnc_tot = ( relUnc_tot * diffXSec ) / 100.0;

      // printf("%15.1e", diffXSec );
      // printf("%15.1e", absUnc_stat );
      // printf("%15.1e", absUnc_exp );
      // printf("%15.1e", absUnc_theo );
      // printf("%15.1e", absUnc_tot );

      // printf("\n");

      TString str_result = TString::Format("%.0d--%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);
      str_result = str_result + Transform_TexFormat( diffXSec );
      str_result = str_result + Transform_TexFormat( absUnc_stat );
      str_result = str_result + Transform_TexFormat( absUnc_exp );
      str_result = str_result + Transform_TexFormat( absUnc_theo );
      str_result = str_result + Transform_TexFormat( absUnc_tot );
      str_result = str_result + "\\\\";
      // printf( "%s\n\\hline\n", str_result.Data() );
      printf( "%s\n", str_result.Data() );
    }
    cout << "===========================================================================" << endl;
  }

  void Print_FpoF_DXSecEachChannel()
  {
    printf("========== Cross section + uncertainties (fiducial, post-FSR) ==========\n");
    printf("                     ");
    printf("%15s", "dSigma/dM" );
    printf("%15s", "Stat.");
    printf("%15s", "Exp.");
    printf("%15s", "Total");
    printf("\n");

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t lowerEdge = h_FpoF_diffXSec_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_FpoF_diffXSec_->GetBinLowEdge(i_bin+1);

      printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

      Double_t diffXSec = h_FpoF_diffXSec_->GetBinContent(i_bin);

      Double_t relUnc_stat = h_FpoF_relUnc_stat_->GetBinContent(i_bin);
      Double_t absUnc_stat = (relUnc_stat * diffXSec) / 100.0;

      Double_t relUnc_exp_syst = h_FpoF_relUnc_syst_tot_->GetBinContent(i_bin);
      Double_t relUnc_exp_lumi = h_relUnc_lumi_->GetBinContent(i_bin);
      // -- experimental uncertainty: systematic & luminosity uncertainty
      Double_t relUnc_exp = sqrt( relUnc_exp_syst*relUnc_exp_syst + relUnc_exp_lumi*relUnc_exp_lumi );
      Double_t absUnc_exp = ( relUnc_exp * diffXSec ) / 100.0;

      Double_t relUnc_tot = sqrt(
        relUnc_stat * relUnc_stat + 
        relUnc_exp  * relUnc_exp );

      Double_t absUnc_tot = (relUnc_tot * diffXSec) / 100.0;

      printf("%15.1e", diffXSec );
      printf("%15.1e", absUnc_stat );
      printf("%15.1e", absUnc_exp );
      printf("%15.1e", absUnc_tot );

      printf("\n");
    }
    cout << "===========================================================================" << endl;
  }

  void Print_FpoF_DXSecEachChannel_texFormat()
  {
    cout << "============== Cross section + uncertainties (fiducial, post-FSR) (.tex format) ==============" << endl;
    printf("Bin          dsigma/dm                  Stat. unc.                 Exp. unc.                  Total unc.\n");
    
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t lowerEdge = h_FpoF_diffXSec_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_FpoF_diffXSec_->GetBinLowEdge(i_bin+1);

      Double_t diffXSec = h_FpoF_diffXSec_->GetBinContent(i_bin);

      Double_t relUnc_stat = h_FpoF_relUnc_stat_->GetBinContent(i_bin);
      Double_t absUnc_stat = (relUnc_stat * diffXSec) / 100.0;

      Double_t relUnc_exp_syst = h_FpoF_relUnc_syst_tot_->GetBinContent(i_bin);
      Double_t relUnc_exp_lumi = h_relUnc_lumi_->GetBinContent(i_bin);
      // -- experimental uncertainty: systematic & luminosity uncertainty
      Double_t relUnc_exp = sqrt( relUnc_exp_syst*relUnc_exp_syst + relUnc_exp_lumi*relUnc_exp_lumi );
      Double_t absUnc_exp = ( relUnc_exp * diffXSec ) / 100.0;

      Double_t relUnc_tot = sqrt(
        relUnc_stat * relUnc_stat + 
        relUnc_exp  * relUnc_exp );

      Double_t absUnc_tot = (relUnc_tot * diffXSec) / 100.0;

      // printf("%15.1e", diffXSec );
      // printf("%15.1e", absUnc_stat );
      // printf("%15.1e", absUnc_exp );
      // printf("%15.1e", absUnc_tot );

      TString str_result = TString::Format("%.0d--%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);
      str_result = str_result + Transform_TexFormat( diffXSec );
      str_result = str_result + Transform_TexFormat( absUnc_stat );
      str_result = str_result + Transform_TexFormat( absUnc_exp );
      str_result = str_result + Transform_TexFormat( absUnc_tot );
      str_result = str_result + "\\\\";
      // printf( "%s\n\\hline\n", str_result.Data() );
      printf( "%s\n", str_result.Data() );

      // printf("15--20 & $  2.1 \times 10^{-1}  $ & $  4.8 \times 10^{-3}  $ & $  1.9 \times 10^{-2}  $ & $  2.0 \times 10^{-2}  $\\");
    }
    cout << "===========================================================================" << endl;
  }

  void Print_DXSecCombined()
  {
    printf("========== Cross section + uncertainties (combined) ==========\n");
    printf("                     ");
    printf("%15s", "dSigma/dM" );
    printf("%15s", "Total");
    printf("\n");

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t lowerEdge = h_diffXSec_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_diffXSec_->GetBinLowEdge(i_bin+1);

      printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

      Double_t diffXSec = h_diffXSec_->GetBinContent(i_bin);
      Double_t absUnc_tot = h_diffXSec_->GetBinError(i_bin);
      printf("%15.1e", diffXSec );
      printf("%15.1e", absUnc_tot );

      printf("\n");
    }
  }

  void Print_DXSecCombined_texFormat()
  {
    printf("========== Cross section + uncertainties (combined) (.tex format) ==========\n");
    printf("                     ");
    printf("%15s", "dSigma/dM" );
    printf("%15s", "Total");
    printf("\n");

    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t lowerEdge = h_diffXSec_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_diffXSec_->GetBinLowEdge(i_bin+1);

      Double_t diffXSec = h_diffXSec_->GetBinContent(i_bin);
      Double_t absUnc_tot = h_diffXSec_->GetBinError(i_bin);
      // printf("%15.1e", diffXSec );
      // printf("%15.1e", absUnc_tot );

      // printf("\n");

      TString str_result = TString::Format("%.0d--%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);
      str_result = str_result + Transform_TexFormat( diffXSec );
      str_result = str_result + Transform_TexFormat( absUnc_tot );
      str_result = str_result + "\\\\";
      // printf( "%s\n\\hline\n", str_result.Data() );
      printf( "%s\n", str_result.Data() );
    }
  }


  void Init()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

    if( channel_ == "Electron" )
    {
      // -- the column for the uncertainty from smearing correction is removed in the paper
      // -- the column for the detector reoslution uncetianty includes both smearing corr. + unfolding
      // TString fileName_smear = rootFilePath+"/ROOTFile_SmearingUnc_Electron.root";
      // TH1D* h_relUnc_syst_smear = Get_Hist(fileName_smear, "h_RelUnc_Syst_Smear");

      // TString fileName_unc = rootFilePath+"/ROOTFile_RelUnc_All_Electron_v2.root";
      // TString fileName_unc = rootFilePath+"/ROOTFile_RelUnc_All_Electron_v4.root";
      TString fileName_unc = rootFilePath+"/ROOTFile_RelUnc_All_Electron_v5_byKP.root"; // -- new differential cross secitons & uncertainties w/ new TnP efficiency once more (fix bug in the ntuple)

      h_relUnc_lumi_        = Get_Hist(fileName_unc, "h_RelLumiUnc_Percent"); 
      h_relUnc_syst_eff_    = Get_Hist(fileName_unc, "h_RelSysUnc_Eff.SF._Percent");

      h_relUnc_syst_detRes_ = Get_Hist(fileName_unc, "h_RelSysUnc_Det.Res_Percent"); // -- already includes smearing unc.
      h_relUnc_syst_bkg_    = Get_Hist(fileName_unc, "h_RelSysUnc_Bkg.Est._Percent");
      h_relUnc_syst_FSR_    = Get_Hist(fileName_unc, "h_RelSysUnc_FSR_Percent");
      h_relUnc_syst_tot_    = Get_Hist(fileName_unc, "h_RelSysUnc_Tot_Percent");
      h_relUnc_syst_accPDF_ = Get_Hist(fileName_unc, "h_RelSysUnc_Acc._Percent");

      // TString fileName_xSec = rootFilePath+"/DiffXsec_Electron_v8.root";
      // TString fileName_xSec = rootFilePath+"/DiffXsec_Electron_v10.root"; // -- after electron channel update in fiducial x-sec
      // TString fileName_xSec = rootFilePath+"/DiffXsec_Electron_v12_byKP.root"; // -- new differential cross secitons & uncertainties w/ new TnP efficiency
      TString fileName_xSec = rootFilePath+"/DiffXsec_Electron_v13_byKP.root"; // -- new differential cross secitons & uncertainties w/ new TnP efficiency once more (fix bug in the ntuple)
      h_diffXSec_    = Get_Hist(fileName_xSec, "h_DiffXSec");
      h_relUnc_stat_ = Get_Hist(fileName_xSec, "h_RelUnc_Stat");

      h_FpoF_diffXSec_        = Get_Hist(fileName_xSec, "h_DiffXSec_Fiducial");
      h_FpoF_relUnc_stat_     = Get_Hist(fileName_xSec, "h_RelUnc_Stat_Fiducial");
      h_FpoF_relUnc_syst_tot_ = Get_Hist(fileName_xSec, "h_RelUnc_Syst_Fiducial");
    }
    if( channel_ == "Muon" )
    {
      // -- the column for the uncertainty from momentum scale correction is removed in the paper
      // -- the column for the detector reoslution uncetianty includes both mom.corr + unfolding

      // TString fileName_momCorr = rootFilePath+"/ROOTFile_SysUnc_DetRes.root";
      // h_relUnc_syst_momEngCorr_ = Get_Hist(fileName_momCorr, "h_RelSysUnc_MomCorr_Percent");

      TString fileName_all = rootFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
      
      h_diffXSec_ = Get_Hist(fileName_all, "h_DiffXsec_FSRCorr");

      h_relUnc_stat_        = Get_Hist(fileName_all, "h_RelStatUnc_Percent");
      h_relUnc_lumi_        = Get_Hist(fileName_all, "h_RelLumiUnc_Percent"); 
      
      h_relUnc_syst_eff_    = Get_Hist(fileName_all, "h_RelSysUnc_Eff.SF._Percent");
      h_relUnc_syst_detRes_ = Get_Hist(fileName_all, "h_RelSysUnc_Det.Res_Percent");
      h_relUnc_syst_bkg_    = Get_Hist(fileName_all, "h_RelSysUnc_Bkg.Est._Percent");
      h_relUnc_syst_FSR_    = Get_Hist(fileName_all, "h_RelSysUnc_FSR_Percent");
      h_relUnc_syst_tot_    = Get_Hist(fileName_all, "h_RelSysUnc_Tot_Percent");
      h_relUnc_syst_accPDF_ = Get_Hist(fileName_all, "h_RelSysUnc_Acc._Percent");

      TString fileName_FpoF = rootFilePath+"/ROOTFile_FpoF_DiffXSec_FullUnc.root";
      h_FpoF_diffXSec_        = Get_Hist(fileName_FpoF, "h_FpoF_DiffXsec_Data");
      h_FpoF_relUnc_stat_     = Get_Hist(fileName_FpoF, "h_RelStatUnc_Percent");
      h_FpoF_relUnc_syst_tot_ = Get_Hist(fileName_FpoF, "h_RelSysUnc_Tot_Percent");
    }
    else if( channel_ == "Combined" )
    {
      // TString fileName = rootFilePath+"/ROOTFile_hepdata__corr_wLumi-20190208_converted.root";
      TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
      TString fileName = analyzerPath+"/Combination/ROOTFile_Combination.root";
      h_diffXSec_ = Get_Hist(fileName, "ll/h_dXSec");
    }

    SaveHistogram();
  }

  TString Transform_TexFormat( Double_t value )
  {
    Int_t Exponent = Calc_Exponent( value );
    Double_t front = value / TMath::Power(10, Exponent);
    if( front >= 9.95 && front < 10.05 ) // -- if rounded off to 10.0: change it to 1.0 x 10^1
    {
      front = 1.0;
      Exponent = Exponent + 1;
    }

    TString str = TString::Format(" & $  %.1lf \\times 10^{%d}  $", front, Exponent);
    return str;
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

  void SaveHistogram()
  {
    TFile *f_output = TFile::Open("ROOTFile_PrintNumbersInTable_"+channel_+".root", "RECREATE");
    f_output->cd();

    if( channel_ == "Combined" )
    {
      h_diffXSec_->SetName("h_dXSec");
      TH1D* h_relUnc_tot = Extract_RelUnc(h_diffXSec_, "h_relUnc_tot");
      TH1D* h_absUnc_tot = ConvertToAbsUnc(h_diffXSec_, h_relUnc_tot, kFALSE, "h_absUnc_tot");

      h_diffXSec_->Write();
      h_relUnc_tot->Write();
      h_absUnc_tot->Write();
    }
    else // -- electron or muon
    {
      h_diffXSec_->SetName("h_dXSec");
      h_relUnc_stat_->SetName("h_relUnc_stat");
      TH1D* h_absUnc_stat = ConvertToAbsUnc(h_diffXSec_, h_relUnc_stat_, kTRUE, "h_absUnc_stat");

      h_relUnc_lumi_->SetName("h_relUnc_lumi");
      TH1D* h_absUnc_lumi = ConvertToAbsUnc(h_diffXSec_, h_relUnc_lumi_, kTRUE, "h_absUnc_lumi");

      h_relUnc_syst_eff_->SetName("h_relUnc_syst_eff");
      TH1D* h_absUnc_syst_eff = ConvertToAbsUnc(h_diffXSec_, h_relUnc_syst_eff_, kTRUE, "h_absUnc_syst_eff");

      h_relUnc_syst_detRes_->SetName("h_relUnc_syst_detRes");
      TH1D* h_absUnc_syst_detRes = ConvertToAbsUnc(h_diffXSec_, h_relUnc_syst_detRes_, kTRUE, "h_absUnc_syst_detRes");

      h_relUnc_syst_bkg_->SetName("h_relUnc_syst_bkg");
      TH1D* h_absUnc_syst_bkg = ConvertToAbsUnc(h_diffXSec_, h_relUnc_syst_bkg_, kTRUE, "h_absUnc_syst_bkg");

      h_relUnc_syst_FSR_->SetName("h_relUnc_syst_FSR");
      TH1D* h_absUnc_syst_FSR = ConvertToAbsUnc(h_diffXSec_, h_relUnc_syst_FSR_, kTRUE, "h_absUnc_syst_FSR");

      h_relUnc_syst_tot_->SetName("h_relUnc_syst_tot");
      TH1D* h_absUnc_syst_tot = ConvertToAbsUnc(h_diffXSec_, h_relUnc_syst_tot_, kTRUE, "h_absUnc_syst_tot");

      h_relUnc_syst_accPDF_->SetName("h_relUnc_syst_accPDF");
      TH1D* h_absUnc_syst_accPDF = ConvertToAbsUnc(h_diffXSec_, h_relUnc_syst_accPDF_, kTRUE, "h_absUnc_syst_accPDF");

      // -- total systematic + luminosity
      vector<TH1D*> vec_relUncHist_expUnc = {
        h_relUnc_lumi_,
        h_relUnc_syst_tot_
      };
      TH1D* h_relUnc_exp = QuadSumHist("h_relUnc_exp", vec_relUncHist_expUnc);
      TH1D* h_absUnc_exp = ConvertToAbsUnc(h_diffXSec_, h_relUnc_exp, kTRUE, "h_absUnc_exp");

      // -- stat. + experimental + theory
      vector<TH1D*> vec_relUncHist_tot = {
        h_relUnc_stat_,
        h_relUnc_exp,
        h_relUnc_syst_accPDF_
      };
      TH1D* h_relUnc_tot = QuadSumHist("h_relUnc_tot", vec_relUncHist_tot);
      TH1D* h_absUnc_tot = ConvertToAbsUnc(h_diffXSec_, h_relUnc_tot, kTRUE, "h_absUnc_tot");

      // -- write
      h_diffXSec_->Write();

      h_relUnc_stat_->Write();
      h_absUnc_stat->Write();

      h_relUnc_lumi_->Write();
      h_absUnc_lumi->Write();

      h_relUnc_syst_eff_->Write();
      h_absUnc_syst_eff->Write();

      h_relUnc_syst_detRes_->Write();
      h_absUnc_syst_detRes->Write();

      h_relUnc_syst_bkg_->Write();
      h_absUnc_syst_bkg->Write();

      h_relUnc_syst_FSR_->Write();
      h_absUnc_syst_FSR->Write();

      h_relUnc_syst_tot_->Write();
      h_absUnc_syst_tot->Write();

      h_relUnc_syst_accPDF_->Write();
      h_absUnc_syst_accPDF->Write();

      h_relUnc_exp->Write();
      h_absUnc_exp->Write();

      h_relUnc_tot->Write();
      h_absUnc_tot->Write();


      // --  fiducial, post-FSR results
      h_FpoF_diffXSec_->SetName("h_FpoF_dXSec");
      h_FpoF_relUnc_stat_->SetName("h_FpoF_relUnc_stat");
      TH1D* h_FpoF_absUnc_stat = ConvertToAbsUnc(h_FpoF_diffXSec_, h_FpoF_relUnc_stat_, kTRUE, "h_FpoF_absUnc_stat");

      // -- relative uncertainty is same with the full phase space case, but absolute uncertainty is different -> need to save separately
      TH1D* h_FpoF_absUnc_lumi        = ConvertToAbsUnc(h_FpoF_diffXSec_, h_relUnc_lumi_,        kTRUE, "h_FpoF_absUnc_lumi");
      TH1D* h_FpoF_absUnc_syst_eff    = ConvertToAbsUnc(h_FpoF_diffXSec_, h_relUnc_syst_eff_,    kTRUE, "h_FpoF_absUnc_syst_eff");
      TH1D* h_FpoF_absUnc_syst_detRes = ConvertToAbsUnc(h_FpoF_diffXSec_, h_relUnc_syst_detRes_, kTRUE, "h_FpoF_absUnc_syst_detRes");
      TH1D* h_FpoF_absUnc_syst_bkg    = ConvertToAbsUnc(h_FpoF_diffXSec_, h_relUnc_syst_bkg_,    kTRUE, "h_FpoF_absUnc_syst_bkg");

      h_FpoF_relUnc_syst_tot_->SetName("h_FpoF_relUnc_syst_tot"); // -- only eff, detRes and bkg.
      TH1D* h_FpoF_absUnc_syst_tot = ConvertToAbsUnc(h_FpoF_diffXSec_, h_FpoF_relUnc_syst_tot_, kTRUE, "h_FpoF_absUnc_syst_tot");

      vector<TH1D*> vec_FpoF_relUncHist_expUnc = {
        h_relUnc_lumi_,
        h_FpoF_relUnc_syst_tot_
      };
      TH1D* h_FpoF_relUnc_exp = QuadSumHist("h_FpoF_relUnc_exp", vec_FpoF_relUncHist_expUnc);
      TH1D* h_FpoF_absUnc_exp = ConvertToAbsUnc(h_FpoF_diffXSec_, h_FpoF_relUnc_exp, kTRUE, "h_FpoF_absUnc_exp");

      vector<TH1D*> vec_FpoF_relUncHist_tot = {
        h_FpoF_relUnc_stat_,
        h_FpoF_relUnc_exp
      };
      TH1D* h_FpoF_relUnc_tot = QuadSumHist("h_FpoF_relUnc_tot", vec_FpoF_relUncHist_tot);
      TH1D* h_FpoF_absUnc_tot = ConvertToAbsUnc(h_FpoF_diffXSec_, h_FpoF_relUnc_tot, kTRUE, "h_FpoF_absUnc_tot");

      // -- write
      h_FpoF_diffXSec_->Write();

      h_FpoF_relUnc_stat_->Write();
      h_FpoF_absUnc_stat->Write();

      h_FpoF_absUnc_lumi->Write();
      h_FpoF_absUnc_syst_eff->Write();
      h_FpoF_absUnc_syst_detRes->Write();
      h_FpoF_absUnc_syst_bkg->Write();

      h_FpoF_relUnc_syst_tot_->Write();
      h_FpoF_absUnc_syst_tot->Write();

      h_FpoF_relUnc_exp->Write();
      h_FpoF_absUnc_exp->Write();

      h_FpoF_relUnc_tot->Write();
      h_FpoF_absUnc_tot->Write();
    }
  }

  TH1D* ConvertToAbsUnc( TH1D* h_cv, TH1D* h_relUnc, Bool_t isPercent = kFALSE,  TString newHistName = "" )
  {
    cout << "[ConvertToAbsUnc] converting: " << h_relUnc->GetName() << " -> " << newHistName << endl;
    TH1D* h_absUnc = (TH1D*)h_relUnc->Clone();
    if( newHistName != "" )
      h_absUnc->SetName(newHistName);

    Int_t nBin = h_absUnc->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t cv     = h_cv->GetBinContent(i_bin);
      Double_t relUnc = h_relUnc->GetBinContent(i_bin);
      if( isPercent ) relUnc = relUnc / 100.0;

      Double_t absUnc = cv * relUnc;
      // cout << "[" << i_bin << "th bin]  "
      //      << "(central value, rel.unc. -> abs. unc) "
      //      << cv << ", "
      //      << relUnc << " -> "
      //      << absUnc << endl;

      // printf("[%02d th bin] (central value, rel.unc. -> abs. unc) %.15e, %.15e -> %.15e\n",
      //          i_bin, cv, relUnc, absUnc);

      h_absUnc->SetBinContent(i_bin, absUnc);
      h_absUnc->SetBinError(i_bin, 0);
    }

    cout << endl;

    return h_absUnc;
  }

  TH1D* QuadSumHist( TString histName, vector<TH1D*> vec_hist )
  {
    TH1D* h_quadSum = (TH1D*)vec_hist[0]->Clone();
    // TString histName = "h_"+dataType_+"_relUnc_"+uncType;
    h_quadSum->SetName(histName);

    Int_t nHist = (Int_t)vec_hist.size();

    Int_t nBin = vec_hist[0]->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t quadSumSq = 0;
      for(Int_t i_hist=0; i_hist<nHist; i_hist++)
      {
        Double_t binContent = vec_hist[i_hist]->GetBinContent(i_bin);
        quadSumSq += binContent*binContent;
      }

      Double_t quadSum = sqrt(quadSumSq);

      h_quadSum->SetBinContent(i_bin, quadSum);
      h_quadSum->SetBinError(i_bin, 0);
    }

    return h_quadSum;
  }

};

void PrintNumbersInTable()
{
  cout << "================" << endl;
  cout << "Electron channel" << endl;
  cout << "================" << endl;
  PrintTool* tool_elec = new PrintTool( "Electron" );
  tool_elec->Print_All();

  cout << "============" << endl;
  cout << "Muon channel" << endl;
  cout << "============" << endl;
  PrintTool* tool_muon = new PrintTool( "Muon" );
  tool_muon->Print_All();

  cout << "===============" << endl;
  cout << "Combined result" << endl;
  cout << "===============" << endl;
  PrintTool* tool_comb = new PrintTool( "Combined" );
  tool_comb->Print_All();
}