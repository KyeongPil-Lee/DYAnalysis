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

  TH1D* h_relUnc_syst_momEngCorr_; // -- muon channel: momentum correction / electron channel: energy smearing correction
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
      // Print_DXSecEachChannel();
      // Print_FpoF_DXSecEachChannel();
    }
    else if( channel_ == "Combined" )
    {
      Print_DXSecCombined();
    }
  }

private:
  void Print_Unc()
  {
    printf("========== Systematic uncertainties from each source ==========\n");
    printf("                     ");
    if( channel_ == "Muon" )
      printf("%10s", "Mom." );
    else if( channel_ == "Electron" )
      printf("%10s", "Smear." );

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
      Double_t lowerEdge = h_relUnc_syst_momEngCorr_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_relUnc_syst_momEngCorr_->GetBinLowEdge(i_bin+1);

      printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

      printf("%10.2lf", h_relUnc_syst_momEngCorr_->GetBinContent(i_bin) );
      printf("%10.2lf", h_relUnc_syst_eff_->GetBinContent(i_bin) );
      printf("%10.2lf", h_relUnc_syst_detRes_->GetBinContent(i_bin) );
      printf("%10.2lf", h_relUnc_syst_bkg_->GetBinContent(i_bin) );
      printf("%10.2lf", h_relUnc_syst_FSR_->GetBinContent(i_bin) );
      printf("%10.2lf", h_relUnc_syst_tot_->GetBinContent(i_bin) );
      printf("%10.2lf", h_relUnc_syst_accPDF_->GetBinContent(i_bin) );
      printf("\n");
    }
  }

  void Print_Unc_texFormat()
  {
    cout << "============== Uncertainties (.tex format) ==============" << endl;    
    if( channel_ == "Muon" )
      printf("           Mom.         Eff         DetRes        Bkg          FSR         Total        Acc+PDF\n");
    else if( channel_ == "Electron" )
      printf("           Smear.       Eff         DetRes        Bkg          FSR         Total        Acc+PDF\n");

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
      Double_t lowerEdge = h_relUnc_syst_momEngCorr_->GetBinLowEdge(i_bin);
      Double_t upperEdge = h_relUnc_syst_momEngCorr_->GetBinLowEdge(i_bin+1);
      
      TString str_syst = TString::Format("%.0d-%.0d", (Int_t)lowerEdge, (Int_t)upperEdge);
      str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_relUnc_syst_momEngCorr_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_relUnc_syst_eff_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_relUnc_syst_detRes_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_relUnc_syst_bkg_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_relUnc_syst_FSR_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_relUnc_syst_tot_->GetBinContent(i_bin) );
      str_syst = str_syst + TString::Format(" & $  %.2lf  $", h_relUnc_syst_accPDF_->GetBinContent(i_bin) );
      str_syst = str_syst + "\\\\";

      printf( "%s\n\\hline\n", str_syst.Data() );
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
      Double_t absUnc_Exp = ( relUnc_exp * diffXSec ) / 100.0;

      Double_t relUnc_theo = h_relUnc_syst_accPDF_->GetBinContent(i_bin);
      Double_t absUnc_theo = ( relUnc_theo * diffXSec ) / 100.0;

      Double_t relUnc_tot = sqrt(
        relUnc_stat * relUnc_stat +
        relUnc_exp  * relUnc_exp +
        relUnc_theo * relUnc_theo );

      Double_t absUnc_tot = ( relUnc_tot * diffXSec ) / 100.0;

      printf("%15.1e", diffXSec );
      printf("%15.1e", absUnc_stat );
      printf("%15.1e", absUnc_Exp );
      printf("%15.1e", absUnc_theo );
      printf("%15.1e", absUnc_tot );

      printf("\n");
    }
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


  void Init()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

    if( channel_ == "Electron" )
    {
      TString fileName_smear = rootFilePath+"/ROOTFile_SmearingUnc_Electron.root";
      h_relUnc_syst_momEngCorr_ = Get_Hist(fileName_smear, "h_RelUnc_Syst_Smear");

      TString fileName_unc = rootFilePath+"/ROOTFile_RelUnc_All_Electron_v2.root";
      h_relUnc_lumi_        = Get_Hist(fileName_unc, "h_RelLumiUnc_Percent"); 
      h_relUnc_syst_eff_    = Get_Hist(fileName_unc, "h_RelSysUnc_Eff.SF._Percent");

      h_relUnc_syst_detRes_ = Get_Hist(fileName_unc, "h_RelSysUnc_Det.Res_Percent");
      // h_relUnc_syst_detRes_ = Get_Hist(fileName_smear, "h_RelUnc_Syst_DetRes_withoutSmear");

      h_relUnc_syst_bkg_    = Get_Hist(fileName_unc, "h_RelSysUnc_Bkg.Est._Percent");
      h_relUnc_syst_FSR_    = Get_Hist(fileName_unc, "h_RelSysUnc_FSR_Percent");
      h_relUnc_syst_tot_    = Get_Hist(fileName_unc, "h_RelSysUnc_Tot_Percent");
      // RemoveLumiUnc( h_relUnc_syst_tot_, h_relUnc_lumi_ );
      h_relUnc_syst_accPDF_ = Get_Hist(fileName_unc, "h_RelSysUnc_Acc._Percent");

      TString fileName_xSec = rootFilePath+"/DiffXsec_Electron_v8.root";
      h_diffXSec_    = Get_Hist(fileName_xSec, "h_DiffXSec");
      h_relUnc_stat_ = Get_Hist(fileName_xSec, "h_RelUnc_Stat");

      h_FpoF_diffXSec_        = Get_Hist(fileName_xSec, "h_DiffXSec_Fiducial");
      h_FpoF_relUnc_stat_     = Get_Hist(fileName_xSec, "h_RelUnc_Stat_Fiducial");
      h_FpoF_relUnc_syst_tot_ = Get_Hist(fileName_xSec, "h_RelUnc_Syst_Fiducial");
    }
    if( channel_ == "Muon" )
    {
      TString fileName_momCorr = rootFilePath+"/ROOTFile_SysUnc_DetRes.root";
      h_relUnc_syst_momEngCorr_ = Get_Hist(fileName_momCorr, "h_RelSysUnc_MomCorr_Percent");

      TString fileName_unc = rootFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
      h_relUnc_lumi_        = Get_Hist(fileName_unc, "h_RelLumiUnc_Percent"); 
      h_relUnc_syst_eff_    = Get_Hist(fileName_unc, "h_RelSysUnc_Eff.SF._Percent");

      h_relUnc_syst_detRes_ = Get_Hist(fileName_unc, "h_RelSysUnc_Det.Res_Percent");
      RemoveMomCorrUnc_MuonChannel( h_relUnc_syst_detRes_, h_relUnc_syst_momEngCorr_ );

      h_relUnc_syst_bkg_    = Get_Hist(fileName_unc, "h_RelSysUnc_Bkg.Est._Percent");
      h_relUnc_syst_FSR_    = Get_Hist(fileName_unc, "h_RelSysUnc_FSR_Percent");
      h_relUnc_syst_tot_    = Get_Hist(fileName_unc, "h_RelSysUnc_Tot_Percent");
      // RemoveLumiUnc( h_relUnc_syst_tot_, h_relUnc_lumi_ );
      h_relUnc_syst_accPDF_ = Get_Hist(fileName_unc, "h_RelSysUnc_Acc._Percent");
    }
    else if( channel_ == "Combined" )
    {
      TString fileName = rootFilePath+"/dyll-combi-_corr_wLumi_inpYieldUnc-20171204.root";
      h_diffXSec_ = Get_Hist(fileName, "h1Combi");
    }
  }

  void RemoveMomCorrUnc_MuonChannel( TH1D* h_relUnc_syst_detRes, TH1D* h_relUnc_syst_momEngCorr )
  {
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t relUnc_detRes_before = h_relUnc_syst_detRes->GetBinContent(i_bin);
      Double_t relUnc_syst_momEngCorr = h_relUnc_syst_momEngCorr->GetBinContent(i_bin);
      Double_t relUnc_detRes_after = sqrt( relUnc_detRes_before*relUnc_detRes_before - relUnc_syst_momEngCorr*relUnc_syst_momEngCorr);

      h_relUnc_syst_detRes->SetBinContent(i_bin, relUnc_detRes_after);
    }
  }

  // -- total systematic uncertainty that Ridhi gave me includes luminosity uncertainty
  // -- luminosity uncertainty should be removed
  void RemoveLumiUnc( TH1D* h_relUnc_syst_tot, TH1D* h_relUnc_lumi )
  {
    for(Int_t i=0; i<nMassBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t relUnc_tot_before = h_relUnc_syst_tot->GetBinContent(i_bin);
      Double_t relUnc_lumi = h_relUnc_lumi->GetBinContent(i_bin);
      Double_t relUnc_tot_after = sqrt( relUnc_tot_before*relUnc_tot_before - relUnc_lumi*relUnc_lumi);

      h_relUnc_syst_tot->SetBinContent(i_bin, relUnc_tot_after);
    }
  }
};

void PrintNumbersInTable()
{
  PrintTool* tool_elec = new PrintTool( "Electron" );
  tool_elec->Print_All();

  PrintTool* tool_muon = new PrintTool( "Muon" );
  tool_muon->Print_All();

  PrintTool* tool_comb = new PrintTool( "Combined" );
  tool_comb->Print_All();
}