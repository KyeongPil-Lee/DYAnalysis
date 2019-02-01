#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>

class ValidationTool
{
public:
  TString type_;

  // -- histograms used for the input
  TH1D* h_ref_dXSec_;
  TH1D* h_ref_absUnc_stat_;
  TH1D* h_ref_absUnc_lumi_;
  TH1D* h_ref_absUnc_syst_accPDF_;
  TH1D* h_ref_absUnc_syst_eff_;
  TH1D* h_ref_absUnc_syst_detRes_;
  TH1D* h_ref_absUnc_syst_bkg_;
  TH1D* h_ref_absUnc_syst_FSR_;
  TH1D* h_ref_absUnc_tot_;

  // -- histograms from HEPData entry
  TH1D* h_HEPData_dXSec_;
  TH1D* h_HEPData_absUnc_stat_;
  TH1D* h_HEPData_absUnc_lumi_;
  TH1D* h_HEPData_absUnc_syst_accPDF_;
  TH1D* h_HEPData_absUnc_syst_eff_;
  TH1D* h_HEPData_absUnc_syst_detRes_;
  TH1D* h_HEPData_absUnc_syst_bkg_;
  TH1D* h_HEPData_absUnc_syst_FSR_;
  TH1D* h_HEPData_absUnc_tot_;

  ValidationTool(TString type)
  {
    type_ = type;
    Init();
  }

  void Validation()
  {
    Comparison_ref_vs_HEPData_All();
    if( type_ != "combined" )
      Comparison_tot_vs_quadSum();
  }

private:
  void Comparison_ref_vs_HEPData_All()
  {
    if( type_ == "muon" || type_ == "electron" )
    {
      Comparison_BinByBin("DXSec: HEPData vs. reference", h_HEPData_dXSec_, h_ref_dXSec_);

      Comparison_BinByBin("Abs. unc. (stat.): HEPData vs. reference", h_HEPData_absUnc_stat_, h_ref_absUnc_stat_);
      Comparison_BinByBin("Abs. unc. (lumi.): HEPData vs. reference", h_HEPData_absUnc_lumi_, h_ref_absUnc_lumi_);
      Comparison_BinByBin("Abs. unc. (syst_accPDF): HEPData vs. reference", h_HEPData_absUnc_syst_accPDF_, h_ref_absUnc_syst_accPDF_);
      Comparison_BinByBin("Abs. unc. (syst_eff): HEPData vs. reference", h_HEPData_absUnc_syst_eff_, h_ref_absUnc_syst_eff_);
      Comparison_BinByBin("Abs. unc. (syst_detRes): HEPData vs. reference", h_HEPData_absUnc_syst_detRes_, h_ref_absUnc_syst_detRes_);
      Comparison_BinByBin("Abs. unc. (syst_bkg): HEPData vs. reference", h_HEPData_absUnc_syst_bkg_, h_ref_absUnc_syst_bkg_);
      Comparison_BinByBin("Abs. unc. (syst_FSR): HEPData vs. reference", h_HEPData_absUnc_syst_FSR_, h_ref_absUnc_syst_FSR_);
      Comparison_BinByBin("Abs. unc. (tot): HEPData vs. reference", h_HEPData_absUnc_tot_, h_ref_absUnc_tot_);
    }

    if( type_ == "muonFid" || type_ == "electronFid" )
    {
      Comparison_BinByBin("DXSec: HEPData vs. reference", h_HEPData_dXSec_, h_ref_dXSec_);

      Comparison_BinByBin("Abs. unc. (stat.): HEPData vs. reference", h_HEPData_absUnc_stat_, h_ref_absUnc_stat_);
      Comparison_BinByBin("Abs. unc. (lumi.): HEPData vs. reference", h_HEPData_absUnc_lumi_, h_ref_absUnc_lumi_);
      Comparison_BinByBin("Abs. unc. (syst_eff): HEPData vs. reference", h_HEPData_absUnc_syst_eff_, h_ref_absUnc_syst_eff_);
      Comparison_BinByBin("Abs. unc. (syst_detRes): HEPData vs. reference", h_HEPData_absUnc_syst_detRes_, h_ref_absUnc_syst_detRes_);
      Comparison_BinByBin("Abs. unc. (syst_bkg): HEPData vs. reference", h_HEPData_absUnc_syst_bkg_, h_ref_absUnc_syst_bkg_);
      Comparison_BinByBin("Abs. unc. (tot): HEPData vs. reference", h_HEPData_absUnc_tot_, h_ref_absUnc_tot_);
    }

    if( type_ == "combined" )
    {
      Comparison_BinByBin("DXSec: HEPData vs. reference", h_HEPData_dXSec_, h_ref_dXSec_);
      Comparison_BinByBin("Abs. unc. (tot): HEPData vs. reference", h_HEPData_absUnc_tot_, h_ref_absUnc_tot_);
    }
  }

  void Comparison_tot_vs_quadSum()
  {
    vector<TH1D*> vec_hist_allUnc = GetVector_allUncHist();
    TH1D* h_absUnc_quadSum = QuadSumHist("h_absUnc_quadSum", vec_hist_allUnc);
    Comparison_BinByBin("QuadSum vs. Total unc. (from HEPData)", h_absUnc_quadSum, h_HEPData_absUnc_tot_);
  }

  void Comparison_BinByBin(TString compType, TH1D* h1, TH1D* h2)
  {
    cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "[ValidationTool::Comparison_BinByBin]" << endl;
    cout << "type: " << type_ << endl;
    cout << "Comparison type: " << compType << endl;

    Int_t nBin = h1->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t value1 = h1->GetBinContent(i_bin);
      Double_t value2 = h2->GetBinContent(i_bin);
      Double_t relDiff = (value1 - value2) / value2;

      cout << "[" << i_bin << "th bin] "
           << "value1 = " << value1 << ", "
           << "value2 = " << value2
           << "-> rel. diff. = " << relDiff
           << endl;

      if( fabs(relDiff) > 0.001 ) // -- larger than 0.1%
      {
        cout << "***********************" << endl;
        cout << "   -> larger than 0.1%!" << endl;
        cout << "***********************" << endl;
      }
    }

    cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
    cout << endl;
  }

  vector<TH1D*> GetVector_allUncHist()
  {
    vector<TH1D*> vec_hist;
    vec_hist.clear();

    if( type_ == "muon" || type_ == "electron" )
    {
      vec_hist.push_back( h_HEPData_absUnc_stat_ );
      vec_hist.push_back( h_HEPData_absUnc_lumi_ );
      vec_hist.push_back( h_HEPData_absUnc_syst_accPDF_ );
      vec_hist.push_back( h_HEPData_absUnc_syst_eff_ );
      vec_hist.push_back( h_HEPData_absUnc_syst_detRes_ );
      vec_hist.push_back( h_HEPData_absUnc_syst_bkg_ );
      vec_hist.push_back( h_HEPData_absUnc_syst_FSR_ );
    }
    else if( type_ == "muonFid" || type_ == "electronFid" )
    {
      vec_hist.push_back( h_HEPData_absUnc_stat_ );
      vec_hist.push_back( h_HEPData_absUnc_lumi_ );
      vec_hist.push_back( h_HEPData_absUnc_syst_eff_ );
      vec_hist.push_back( h_HEPData_absUnc_syst_detRes_ );
      vec_hist.push_back( h_HEPData_absUnc_syst_bkg_ );
    }

    return vec_hist;
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

  void Init()
  {
    // -- initialization: some histograms will be nullptr according to type_
    h_ref_dXSec_ = nullptr;
    h_ref_absUnc_stat_ = nullptr;
    h_ref_absUnc_lumi_ = nullptr;
    h_ref_absUnc_syst_accPDF_ = nullptr;
    h_ref_absUnc_syst_eff_ = nullptr;
    h_ref_absUnc_syst_detRes_ = nullptr;
    h_ref_absUnc_syst_bkg_ = nullptr;
    h_ref_absUnc_syst_FSR_ = nullptr;
    h_ref_absUnc_tot_ = nullptr;

    h_HEPData_dXSec_ = nullptr;
    h_HEPData_absUnc_stat_ = nullptr;
    h_HEPData_absUnc_lumi_ = nullptr;
    h_HEPData_absUnc_syst_accPDF_ = nullptr;
    h_HEPData_absUnc_syst_eff_ = nullptr;
    h_HEPData_absUnc_syst_detRes_ = nullptr;
    h_HEPData_absUnc_syst_bkg_ = nullptr;
    h_HEPData_absUnc_syst_FSR_ = nullptr;
    h_HEPData_absUnc_tot_ = nullptr;


    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName_ref = "";
    if( type_.Contains("muon") )          fileName_ref = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Muon.root";
    else if( type_.Contains("electron") ) fileName_ref = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Electron.root";
    else if( type_ == "combined" )        fileName_ref = analyzerPath+"/OfficialStyle/ROOTFile_PrintNumbersInTable_Combined.root";

    TString fileName_HEPData = "./HEPData_SMP17001.root";
    TString dirName_HEPData = "";
    if( type_ == "muon" )        dirName_HEPData = "Table 1";
    if( type_ == "electron" )    dirName_HEPData = "Table 2";
    if( type_ == "muonFid" )     dirName_HEPData = "Table 3";
    if( type_ == "electronFid" ) dirName_HEPData = "Table 4";
    if( type_ == "combined" )    dirName_HEPData = "Table 5";


    if( type_ == "muon" || type_ == "electron" )
    {
      h_ref_dXSec_ = Get_Hist(fileName_ref, "h_dXSec");

      h_ref_absUnc_stat_ = Get_Hist(fileName_ref, "h_absUnc_stat");
      h_ref_absUnc_lumi_ = Get_Hist(fileName_ref, "h_absUnc_lumi");
      h_ref_absUnc_syst_accPDF_ = Get_Hist(fileName_ref, "h_absUnc_syst_accPDF");
      h_ref_absUnc_syst_eff_    = Get_Hist(fileName_ref, "h_absUnc_syst_eff");
      h_ref_absUnc_syst_detRes_ = Get_Hist(fileName_ref, "h_absUnc_syst_detRes");
      h_ref_absUnc_syst_bkg_    = Get_Hist(fileName_ref, "h_absUnc_syst_bkg");
      h_ref_absUnc_syst_FSR_    = Get_Hist(fileName_ref, "h_absUnc_syst_FSR");
      h_ref_absUnc_tot_         = Get_Hist(fileName_ref, "h_absUnc_tot");


      h_HEPData_dXSec_              = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1");
      h_HEPData_absUnc_stat_        = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e1");
      h_HEPData_absUnc_lumi_        = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e2");
      h_HEPData_absUnc_syst_accPDF_ = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e3");
      h_HEPData_absUnc_syst_eff_    = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e4");
      h_HEPData_absUnc_syst_detRes_ = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e5");
      h_HEPData_absUnc_syst_bkg_    = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e6");
      h_HEPData_absUnc_syst_FSR_    = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e7");
      h_HEPData_absUnc_tot_         = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e8");
    }
    else if( type_ == "muonFid" || type_ == "electronFid" ) // -- muon channel, fiducial cross sections
    {
      h_ref_dXSec_ = Get_Hist(fileName_ref, "h_FpoF_dXSec");

      h_ref_absUnc_stat_ = Get_Hist(fileName_ref, "h_FpoF_absUnc_stat");
      h_ref_absUnc_lumi_ = Get_Hist(fileName_ref, "h_FpoF_absUnc_lumi");
      h_ref_absUnc_syst_eff_    = Get_Hist(fileName_ref, "h_FpoF_absUnc_syst_eff");
      h_ref_absUnc_syst_detRes_ = Get_Hist(fileName_ref, "h_FpoF_absUnc_syst_detRes");
      h_ref_absUnc_syst_bkg_    = Get_Hist(fileName_ref, "h_FpoF_absUnc_syst_bkg");
      h_ref_absUnc_tot_         = Get_Hist(fileName_ref, "h_FpoF_absUnc_tot");


      h_HEPData_dXSec_              = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1");
      h_HEPData_absUnc_stat_        = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e1");
      h_HEPData_absUnc_lumi_        = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e2");
      h_HEPData_absUnc_syst_eff_    = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e3");
      h_HEPData_absUnc_syst_detRes_ = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e4");
      h_HEPData_absUnc_syst_bkg_    = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e5");
      h_HEPData_absUnc_tot_         = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e6");
    }
    else if( type_ == "combined" )
    {
      h_ref_dXSec_ = Get_Hist(fileName_ref, "h_dXSec");
      h_ref_absUnc_tot_ = Get_Hist(fileName_ref, "h_absUnc_tot");

      h_HEPData_dXSec_      = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1");
      h_HEPData_absUnc_tot_ = Get_Hist(fileName_HEPData, dirName_HEPData+"/Hist1D_y1_e1");
    }
  }

};

class CovMValidationTool
{
public:
  TString type_;

  TH2D* h_ref_cov_;
  TH2D* h_HEPData_cov_;

  CovMValidationTool(TString type)
  {
    type_ = type;
    Init();
  }

  void Validation()
  {
    Comparison2D_BinByBin("Total covariance: HEPData vs. Reference", h_ref_cov_, h_HEPData_cov_);
  }

private:
  void Init()
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString fileName_ref = "";
    if( type_.Contains("muon") )          fileName_ref = analyzerPath+"/OfficialStyle/ROOTFile_covM_muon.root";
    else if( type_.Contains("electron") ) fileName_ref = analyzerPath+"/OfficialStyle/ROOTFile_covM_electron.root";
    else if( type_ == "combined" )        fileName_ref = analyzerPath+"/OfficialStyle/ROOTFile_covM_combined.root";

    TString fileName_HEPData = "./HEPData_SMP17001.root";
    TString dirName_HEPData = "";
    if( type_ == "muon" )        dirName_HEPData = "Table 6";
    if( type_ == "electron" )    dirName_HEPData = "Table 7";
    if( type_ == "combined" )    dirName_HEPData = "Table 8";

    if( type_ == "muon" || type_ == "electron" )
    {
      h_ref_cov_     = Get_Hist_2D(fileName_ref, "h_cov_noLumi");
      h_HEPData_cov_ = Get_Hist_2D(fileName_HEPData, dirName_HEPData+"/Hist2D_y1");
    }
    else if( type_ == "combined" )
    {
      h_ref_cov_     = Get_Hist_2D(fileName_ref, "h_cov");
      h_HEPData_cov_ = Get_Hist_2D(fileName_HEPData, dirName_HEPData+"/Hist2D_y1");
    }
  }

  void Comparison2D_BinByBin(TString compType, TH2D* h1, TH2D* h2)
  {
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "[CovMValidationTool::Comparison2D_BinByBin]" << endl;
    cout << "type: " << type_ << endl;
    cout << "Comparison type: " << compType << endl;

    Int_t nBinX = h1->GetNbinsX();
    Int_t nBinY = h1->GetNbinsY();
    for(Int_t i_x=0; i_x<nBinX; i_x++)
    {
      Int_t i_binX = i_x+1;

      for(Int_t i_y=0; i_y<nBinY; i_y++)
      {
        Int_t i_binY = i_y+1;

        Double_t value1 = h1->GetBinContent(i_binX, i_binY);
        Double_t value2 = h2->GetBinContent(i_binX, i_binY);
        Double_t relDiff = (value1 - value2) / value2;

        cout << "[(" << i_binX << ", " << i_binY << ") bin] "
             << "value1 = " << value1 << ", "
             << "value2 = " << value2
             << " -> rel. diff. = " << relDiff
             << endl;

        if( fabs(relDiff) > 0.001 ) // -- larger than 0.1%
        {
          cout << "***********************" << endl;
          cout << "   -> larger than 0.1%!" << endl;
          cout << "***********************" << endl;
        }

      }
    }

    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << endl;
  }

};

void Validation_HEPDataEntry()
{
  ValidationTool* tool_muon = new ValidationTool("muon");
  tool_muon->Validation();

  ValidationTool* tool_electron = new ValidationTool("electron");
  tool_electron->Validation();

  ValidationTool* tool_muonFid = new ValidationTool("muonFid");
  tool_muonFid->Validation();

  ValidationTool* tool_electronFid = new ValidationTool("electronFid");
  tool_electronFid->Validation();

  ValidationTool* tool_combined = new ValidationTool("combined");
  tool_combined->Validation();


  // -- covariance matrices
  CovMValidationTool* toolCov_muon = new CovMValidationTool("muon");
  toolCov_muon->Validation();

  CovMValidationTool* toolCov_electron = new CovMValidationTool("electron");
  toolCov_electron->Validation();

  CovMValidationTool* toolCov_combined = new CovMValidationTool("combined");
  toolCov_combined->Validation();
}