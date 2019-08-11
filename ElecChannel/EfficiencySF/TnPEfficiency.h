#include <Include/SimplePlotTools.h>
#include <ElecChannel/Include/DYElectronTool.h>
#include <TRandom3.h>

#define nEtaBin 10
#define nPtBin 5

class TnPEfficiency
{
public:
  vector<Double_t> vec_ptBinEdge_ = {10, 20, 30, 40, 50, 200};
  vector<Double_t> vec_etaBinEdge_ = {-2.5, -2, -1.566, -1.4442, -0.8, 0, 0.8, 1.4442, 1.566, 2, 2.5};

  TString fileName_reco_;
  TString fileName_ID_;
  TString fileName_trig_;
  Bool_t usePrivateTnP_ = kFALSE;

  Double_t eff_reco_data_[nEtaBin][nPtBin];
  Double_t eff_reco_MC_[nEtaBin][nPtBin];

  Double_t eff_ID_data_[nEtaBin][nPtBin];
  Double_t eff_ID_MC_[nEtaBin][nPtBin];

  Double_t eff_trig_data_[nEtaBin][nPtBin];
  Double_t eff_trig_MC_[nEtaBin][nPtBin];

  // -- for the uncertainty estimation:  eff(alt. value) - eff(central value) <- it should not be a abolute value!! 
  Double_t diff_reco_data_[nEtaBin][nPtBin];
  Double_t diff_reco_MC_[nEtaBin][nPtBin];

  Double_t diff_ID_data_[nEtaBin][nPtBin];
  Double_t diff_ID_MC_[nEtaBin][nPtBin];

  Double_t diff_trig_data_[nEtaBin][nPtBin];
  Double_t diff_trig_MC_[nEtaBin][nPtBin];

  // -- features for the tests
  Bool_t doFlipFlop_ = kFALSE;

  Bool_t doIgnorePtBinUnc_ = kFALSE;
  Int_t  thePtIndex_ignore_ = -1;

  Bool_t doIgnoreUnc_effType_ = kFALSE;
  TString effType_ignore_ = "";

  TnPEfficiency()
  {
    TH1::AddDirectory(kFALSE);
    if( nPtBin != vec_ptBinEdge_.size() - 1 )
      cout << "Warning: nPtBin != vec_ptBinEdge_.size() - 1" << endl;
    if( nEtaBin != vec_etaBinEdge_.size() - 1 )
      cout << "Warning: nEtaBin != vec_etaBinEdge_.size() - 1" << endl;

    doFlipFlop_ = kFALSE;

    Init();
  }

  TnPEfficiency(TString fileName_reco, TString fileName_ID, TString fileName_trig )
  {
    usePrivateTnP_ = kTRUE;

    TH1::AddDirectory(kFALSE);
    if( nPtBin != vec_ptBinEdge_.size() - 1 )
      cout << "Warning: nPtBin != vec_ptBinEdge_.size() - 1" << endl;
    if( nEtaBin != vec_etaBinEdge_.size() - 1 )
      cout << "Warning: nEtaBin != vec_etaBinEdge_.size() - 1" << endl;

    fileName_reco_ = fileName_reco;
    fileName_ID_   = fileName_ID;
    fileName_trig_ = fileName_trig;

    cout << "[TnPEfficiency::Constructor] use the private TnP root files" << endl;
    cout << "  Reconstruction efficiency: " << fileName_reco_ << endl;
    cout << "  ID efficiency: "             << fileName_ID_   << endl;
    cout << "  Trigger efficiency: "        << fileName_trig_ << endl;
    cout << endl;

    Init();
  }

  void IngorePtBinUnc(Int_t thePtIndex_ignore)
  {
    doIgnorePtBinUnc_ = kTRUE;
    thePtIndex_ignore_ = thePtIndex_ignore;
    cout << "[TnPEfficiency::IngorePtBinUnc] Activated: the uncertainty in the pT bin index = " << thePtIndex_ignore << " will be ignored" << endl;
  }

  void IgnoreSpecificEffTypeUnc(TString effType_ignore)
  {
    if( !(effType_ignore == "reco" || effType_ignore == "ID" || effType_ignore == "trig") )
    {
      cout << "effType_ignore = " << effType_ignore << " is not supported" << endl;
      return;
    }

    doIgnoreUnc_effType_ = kTRUE;
    effType_ignore_ = effType_ignore;
  }

  void SmearingEff_GivenUncType(TString uncType)
  {
    if( !(uncType == "stat" ||
        uncType == "bkgChange" ||
        uncType == "sgnChange" ||
        uncType == "tagChange" ||
        uncType == "nlo") )
    {
      cout << "uncType = " << uncType << " is not recognizable" << endl;
      return;
    }

    if( uncType == "stat" )
      SmearingEff_Stat(); // -- smearing without correlation between bins: efficiency in each bin is randomized independently
    else
      SmearingEff_Syst(uncType); // -- smearing with the correlation considered: efficiency in each bin coherently moves
  }

  Double_t GetTnPEff(TString type, Double_t pt, Double_t eta)
  {
    Int_t i_ptBin  = FindPtBin(pt);
    Int_t i_etaBin = FindEtaBin(eta);

    // printf("(i_ptBin, i_etaBin) = (%d, %d)\n", i_ptBin, i_etaBin);

    if( type == "reco_data" ) return eff_reco_data_[i_etaBin][i_ptBin];
    if( type == "reco_MC" )   return eff_reco_MC_[i_etaBin][i_ptBin];

    if( type == "ID_data" ) return eff_ID_data_[i_etaBin][i_ptBin];
    if( type == "ID_MC" )   return eff_ID_MC_[i_etaBin][i_ptBin];

    if( type == "trig_data" ) return eff_trig_data_[i_etaBin][i_ptBin];
    if( type == "trig_MC" )   return eff_trig_MC_[i_etaBin][i_ptBin];

    cout << "type = " << type << "is not recognizable" << endl;
    return -999;
  }

  Double_t EfficiencySF_EventWeight(Double_t pt1, Double_t eta1, Double_t pt2, Double_t eta2)
  {
    Double_t weight;

    Double_t eff_ele1_reco_data = GetTnPEff("reco_data", pt1, eta1);
    Double_t eff_ele1_ID_data   = GetTnPEff("ID_data",   pt1, eta1);
    Double_t eff_ele1_reco_MC   = GetTnPEff("reco_MC",   pt1, eta1);
    Double_t eff_ele1_ID_MC     = GetTnPEff("ID_MC",     pt1, eta1);

    Double_t eff_ele1_data = eff_ele1_reco_data * eff_ele1_ID_data;
    Double_t eff_ele1_MC   = eff_ele1_reco_MC   * eff_ele1_ID_MC;

    Double_t eff_ele2_reco_data = GetTnPEff("reco_data", pt2, eta2);
    Double_t eff_ele2_ID_data   = GetTnPEff("ID_data",   pt2, eta2);
    Double_t eff_ele2_reco_MC   = GetTnPEff("reco_MC",   pt2, eta2);
    Double_t eff_ele2_ID_MC     = GetTnPEff("ID_MC",     pt2, eta2);

    Double_t eff_ele2_data = eff_ele2_reco_data * eff_ele2_ID_data;
    Double_t eff_ele2_MC   = eff_ele2_reco_MC   * eff_ele2_ID_MC;

    // Double_t eff_ele2_data = GetTnPEff("reco_data", pt2, eta2) * GetTnPEff("ID_data", pt2, eta2);
    // Double_t eff_ele2_MC   = GetTnPEff("reco_MC", pt2, eta2)   * GetTnPEff("ID_MC", pt2, eta2);

    Double_t eff_ele1_trig_data = GetTnPEff("trig_data", pt1, eta1);
    Double_t eff_ele2_trig_data = GetTnPEff("trig_data", pt2, eta2);
    Double_t eff_event_trig_data = eff_ele1_trig_data + eff_ele2_trig_data - eff_ele1_trig_data*eff_ele2_trig_data;

    Double_t eff_ele1_trig_MC = GetTnPEff("trig_MC", pt1, eta1);
    Double_t eff_ele2_trig_MC = GetTnPEff("trig_MC", pt2, eta2);
    Double_t eff_event_trig_MC = eff_ele1_trig_MC + eff_ele2_trig_MC - eff_ele1_trig_MC*eff_ele2_trig_MC;

    Double_t eff_all_data = eff_ele1_data * eff_ele2_data * eff_event_trig_data;
    Double_t eff_all_MC   = eff_ele1_MC   * eff_ele2_MC   * eff_event_trig_MC;

    weight = eff_all_data / eff_all_MC;

    // if( 1 )
    // {
    //   cout << "==========================================================================" << endl;
    //   printf("[electron 1] (pt, eta) = (%.3lf, %.3lf)\n", pt1, eta1);
    //   printf("   [Reco eff.]  (data, mc) = (%.3lf, %.3lf)\n", eff_ele1_reco_data, eff_ele1_reco_MC );
    //   printf("   [ID eff.]    (data, mc) = (%.3lf, %.3lf)\n", eff_ele1_ID_data,   eff_ele1_ID_MC );
    //   printf("   [Trig eff.]  (data, mc) = (%.3lf, %.3lf)\n", eff_ele1_trig_data, eff_ele1_trig_MC );
    //   printf("[electron 2] (pt, eta) = (%.3lf, %.3lf)\n", pt2, eta2);
    //   printf("   [Reco eff.]  (data, mc) = (%.3lf, %.3lf)\n", eff_ele2_reco_data, eff_ele2_reco_MC );
    //   printf("   [ID eff.]    (data, mc) = (%.3lf, %.3lf)\n", eff_ele2_ID_data,   eff_ele2_ID_MC );
    //   printf("   [Trig eff.]  (data, mc) = (%.3lf, %.3lf)\n", eff_ele2_trig_data, eff_ele2_trig_MC );
    //   printf("[Trigger] (event_data, event_MC) = (%.3lf, %.3lf)\n", eff_event_trig_data, eff_event_trig_MC);
    //   printf("[SF] (eff_all_data, eff_all_MC, SF) = (%.3lf, %.3lf, %.3lf)\n", eff_all_data, eff_all_MC, weight);
    //   cout << "==========================================================================" << endl;
    //   cout << endl;
    // }

    return weight;
  }

private:
  void Init()
  {
    // cout << "[TnPEfficiency::Init] tag change for the trigger efficiency: data in this code" << endl;
    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        eff_reco_data_[i_eta][i_pt] = 0;
        eff_reco_MC_[i_eta][i_pt] = 0;
        eff_ID_data_[i_eta][i_pt] = 0;
        eff_ID_MC_[i_eta][i_pt] = 0;
        eff_trig_data_[i_eta][i_pt] = 0;
        eff_trig_MC_[i_eta][i_pt] = 0;

        diff_reco_data_[i_eta][i_pt] = 0;
        diff_reco_MC_[i_eta][i_pt] = 0;
        diff_ID_data_[i_eta][i_pt] = 0;
        diff_ID_MC_[i_eta][i_pt] = 0;
        diff_trig_data_[i_eta][i_pt] = 0;
        diff_trig_MC_[i_eta][i_pt] = 0;
      }
    }

    if( !usePrivateTnP_ )
    {
      TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
      TString baseDir = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult";

      fileName_reco_ = baseDir+"/ROOTFile_RecoSF_April2019.root";
      fileName_ID_   = baseDir+"/ROOTFile_IDSF_April2019.root";
      fileName_trig_ = baseDir+"/ROOTFile_trigSF_SMP17001.root";
    }

    TString histName_data = "h_eff_data";
    TString histName_MC   = "h_eff_mc";

    TH2D* h_eff_reco_data = PlotTool::Get_Hist2D(fileName_reco_, histName_data);
    TH2D* h_eff_reco_MC   = PlotTool::Get_Hist2D(fileName_reco_, histName_MC);

    TH2D* h_eff_ID_data = PlotTool::Get_Hist2D(fileName_ID_, histName_data);
    TH2D* h_eff_ID_MC   = PlotTool::Get_Hist2D(fileName_ID_, histName_MC);

    TH2D* h_eff_trig_data = PlotTool::Get_Hist2D(fileName_trig_, histName_data);
    TH2D* h_eff_trig_MC   = PlotTool::Get_Hist2D(fileName_trig_, histName_MC);

    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        Int_t i_etaBin = i_eta + 1;
        Int_t i_ptBin  = i_pt + 1;

        eff_reco_data_[i_eta][i_pt] = h_eff_reco_data->GetBinContent(i_etaBin, i_ptBin);
        eff_reco_MC_[i_eta][i_pt]   = h_eff_reco_MC->GetBinContent(i_etaBin, i_ptBin);

        eff_ID_data_[i_eta][i_pt] = h_eff_ID_data->GetBinContent(i_etaBin, i_ptBin);
        eff_ID_MC_[i_eta][i_pt]   = h_eff_ID_MC->GetBinContent(i_etaBin, i_ptBin);

        eff_trig_data_[i_eta][i_pt] = h_eff_trig_data->GetBinContent(i_etaBin, i_ptBin);
        eff_trig_MC_[i_eta][i_pt]   = h_eff_trig_MC->GetBinContent(i_etaBin, i_ptBin);

        // printf("(i_eta, i_pt) = (%d, %d): (eff_trig_data, eff_trig_MC) = (%.3lf, %.3lf)\n", i_eta, i_pt, eff_trig_data_[i_eta][i_pt], eff_trig_MC_[i_eta][i_pt]);
      }
    }
  }


  void SmearingEff_Stat()
  {
    TString histName_data = "h_statUnc_data";
    TString histName_MC   = "h_statUnc_mc";

    // -- stat. unc case: diff = stat unc.
    TH2D* h_absUnc_stat_reco_data = PlotTool::Get_Hist2D(fileName_reco_, histName_data);
    TH2D* h_absUnc_stat_reco_MC   = PlotTool::Get_Hist2D(fileName_reco_, histName_MC);
    TH2D* h_absUnc_stat_ID_data   = PlotTool::Get_Hist2D(fileName_ID_, histName_data);
    TH2D* h_absUnc_stat_ID_MC     = PlotTool::Get_Hist2D(fileName_ID_, histName_MC);
    TH2D* h_absUnc_stat_trig_data = PlotTool::Get_Hist2D(fileName_trig_, histName_data);
    TH2D* h_absUnc_stat_trig_MC   = PlotTool::Get_Hist2D(fileName_trig_, histName_MC);

    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        Int_t i_etaBin = i_eta + 1;
        Int_t i_ptBin  = i_pt + 1;

        diff_reco_data_[i_eta][i_pt] = h_absUnc_stat_reco_data->GetBinContent(i_etaBin, i_ptBin);
        diff_reco_MC_[i_eta][i_pt]   = h_absUnc_stat_reco_MC->GetBinContent(i_etaBin, i_ptBin);

        diff_ID_data_[i_eta][i_pt] = h_absUnc_stat_ID_data->GetBinContent(i_etaBin, i_ptBin);
        diff_ID_MC_[i_eta][i_pt]   = h_absUnc_stat_ID_MC->GetBinContent(i_etaBin, i_ptBin);

        diff_trig_data_[i_eta][i_pt] = h_absUnc_stat_trig_data->GetBinContent(i_etaBin, i_ptBin);
        diff_trig_MC_[i_eta][i_pt]   = h_absUnc_stat_trig_MC->GetBinContent(i_etaBin, i_ptBin);
      }
    }

    // -- randomization
    TRandom3 ran;
    ran.SetSeed(0);
    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        eff_reco_data_[i_eta][i_pt] = eff_reco_data_[i_eta][i_pt] + ran.Gaus(0.0, 1.0) * diff_reco_data_[i_eta][i_pt];
        eff_reco_MC_[i_eta][i_pt]   = eff_reco_MC_[i_eta][i_pt]   + ran.Gaus(0.0, 1.0) * diff_reco_MC_[i_eta][i_pt];

        eff_ID_data_[i_eta][i_pt] = eff_ID_data_[i_eta][i_pt] + ran.Gaus(0.0, 1.0) * diff_ID_data_[i_eta][i_pt];
        eff_ID_MC_[i_eta][i_pt]   = eff_ID_MC_[i_eta][i_pt]   + ran.Gaus(0.0, 1.0) * diff_ID_MC_[i_eta][i_pt];

        eff_trig_data_[i_eta][i_pt] = eff_trig_data_[i_eta][i_pt] + ran.Gaus(0.0, 1.0) * diff_trig_data_[i_eta][i_pt];
        eff_trig_MC_[i_eta][i_pt]   = eff_trig_MC_[i_eta][i_pt]   + ran.Gaus(0.0, 1.0) * diff_trig_MC_[i_eta][i_pt];
      }
    }    
  }

  void SmearingEff_Syst(TString uncType)
  {
    FillDiffArray(uncType);
    if( doFlipFlop_ ) ModifyDiffArray_FlipFlop(uncType);
    if( doIgnorePtBinUnc_ ) ModifyDiffArray_IgnorePtBinUnc(thePtIndex_ignore_, uncType);
    if( doIgnoreUnc_effType_ ) ModifyDiffArray_IgnoreEffTypeUnc(effType_ignore_, uncType);

    Randomization_Syst(uncType);
  }

  void Randomization_Syst(TString uncType)
  {
    // -- randomization
    TRandom3 ran;
    ran.SetSeed(0);

    // -- random number: should be same for all pt-eta bins in each efficiency
    Double_t rndNum_reco = ran.Gaus(0.0, 1.0);
    Double_t rndNum_ID   = ran.Gaus(0.0, 1.0);
    Double_t rndNum_trig = ran.Gaus(0.0, 1.0);
    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        if( uncType == "bkgChange" || uncType == "sgnChange" ) // -- data variation, without trigger
        {
          eff_reco_data_[i_eta][i_pt] = eff_reco_data_[i_eta][i_pt] + rndNum_reco * diff_reco_data_[i_eta][i_pt];
          eff_ID_data_[i_eta][i_pt]   = eff_ID_data_[i_eta][i_pt]   + rndNum_ID   * diff_ID_data_[i_eta][i_pt];
        }
        else if( uncType == "nlo" ) // -- MC variation, including trigger
        {
          eff_reco_MC_[i_eta][i_pt] = eff_reco_MC_[i_eta][i_pt] + rndNum_reco * diff_reco_MC_[i_eta][i_pt];
          eff_ID_MC_[i_eta][i_pt]   = eff_ID_MC_[i_eta][i_pt]   + rndNum_ID   * diff_ID_MC_[i_eta][i_pt];
          eff_trig_MC_[i_eta][i_pt] = eff_trig_MC_[i_eta][i_pt] + rndNum_trig * diff_trig_MC_[i_eta][i_pt];
        }
        else if( uncType == "tagChange" ) // -- MC variation for reco+ID & data variation for the trigger
        {
          eff_reco_MC_[i_eta][i_pt]   = eff_reco_MC_[i_eta][i_pt]   + rndNum_reco * diff_reco_MC_[i_eta][i_pt];
          eff_ID_MC_[i_eta][i_pt]     = eff_ID_MC_[i_eta][i_pt]     + rndNum_ID   * diff_ID_MC_[i_eta][i_pt];
          eff_trig_data_[i_eta][i_pt] = eff_trig_data_[i_eta][i_pt] + rndNum_trig * diff_trig_data_[i_eta][i_pt];
        }
      }
    }
  }

  void ModifyDiffArray_FlipFlop(TString uncType)
  {
    if( !(uncType == "sgnChange" || uncType == "bkgChange") )
      cout << "uncType = " << uncType << " is not supported for doing flipflop" << endl;
    else
    {
      cout << "doFlipFlop: activated! ... uncType = " << uncType << endl;

      Int_t i_count = 0;
      for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
      {
        for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
        {
          if( i_count % 2 == 0 ) // -- set as +
          {
            diff_reco_data_[i_eta][i_pt] = fabs(diff_reco_data_[i_eta][i_pt]);
            diff_ID_data_[i_eta][i_pt]   = fabs(diff_ID_data_[i_eta][i_pt]);
          }
          else // -- set as -
          {
            diff_reco_data_[i_eta][i_pt] = (-1)*fabs(diff_reco_data_[i_eta][i_pt]);
            diff_ID_data_[i_eta][i_pt]   = (-1)*fabs(diff_ID_data_[i_eta][i_pt]);
          }

          i_count++;
        } // -- end of pt iteration
      } // -- end of eta iteration
    } // -- else statement
  }

  // -- difference = 0 for a specific pt bin for the test
  void ModifyDiffArray_IgnorePtBinUnc(Int_t thePtIndex, TString uncType)
  {
    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      if( uncType == "sgnChange" || uncType == "bkgChange" )
      {
        diff_reco_data_[i_eta][thePtIndex] = 0;
        diff_ID_data_[i_eta][thePtIndex]   = 0;
      }
      else if( uncType == "nlo" )
      {
        diff_reco_MC_[i_eta][thePtIndex] = 0;
        diff_ID_MC_[i_eta][thePtIndex]   = 0;
        diff_trig_MC_[i_eta][thePtIndex] = 0;
      }
      else if( uncType == "tagChange" )
      {
        diff_reco_MC_[i_eta][thePtIndex]   = 0;
        diff_ID_MC_[i_eta][thePtIndex]     = 0;
        diff_trig_data_[i_eta][thePtIndex] = 0;
      }
    }
  }

  // -- diff = 0 for a specific eff type (reco, ID, trig)
  void ModifyDiffArray_IgnoreEffTypeUnc(TString effType_ignore, TString uncType)
  {
    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        if( uncType == "sgnChange" || uncType == "bkgChange" )
        {
          if( effType_ignore == "reco" ) diff_reco_data_[i_eta][i_pt] = 0;
          if( effType_ignore == "ID" )   diff_ID_data_[i_eta][i_pt] = 0;
        }
        else if( uncType == "nlo" )
        {
          if( effType_ignore == "reco" ) diff_reco_MC_[i_eta][i_pt] = 0;
          if( effType_ignore == "ID" )   diff_ID_MC_[i_eta][i_pt] = 0;
          if( effType_ignore == "trig" ) diff_trig_MC_[i_eta][i_pt] = 0;
        }
        else if( uncType == "tagChange" )
        {
          if( effType_ignore == "reco" ) diff_reco_MC_[i_eta][i_pt] = 0;
          if( effType_ignore == "ID" )   diff_ID_MC_[i_eta][i_pt] = 0;
          if( effType_ignore == "trig" ) diff_trig_data_[i_eta][i_pt] = 0;
        }
      } // -- end of pt iteration
    } // -- end of eta iteration
  }

  void FillDiffArray(TString uncType)
  {
    TH2D* h_relDiff_reco = nullptr;
    TH2D* h_relDiff_ID   = nullptr;
    TH2D* h_relDiff_trig = nullptr;

    if( uncType == "bkgChange" || uncType == "sgnChange" )
    {
      TString histName = "h_relDiff_"+uncType+"_data"; // -- keep the sign information
      h_relDiff_reco = PlotTool::Get_Hist2D(fileName_reco_, histName);
      h_relDiff_ID   = PlotTool::Get_Hist2D(fileName_ID_,   histName);
    }
    else if( uncType == "nlo" )
    {
      TString histName = "h_relDiff_"+uncType+"_mc"; // -- keep the sign information
      h_relDiff_reco = PlotTool::Get_Hist2D(fileName_reco_, histName);
      h_relDiff_ID   = PlotTool::Get_Hist2D(fileName_ID_,   histName);
      h_relDiff_trig = PlotTool::Get_Hist2D(fileName_trig_, histName);
    }
    else if( uncType == "tagChange" )
    {
      TString histName_recoID = "h_relDiff_"+uncType+"_mc"; // -- keep the sign information
      TString histName_trig   = "h_relDiff_"+uncType+"_data"; // -- tag change: data variation in the trigger case
      h_relDiff_reco = PlotTool::Get_Hist2D(fileName_reco_, histName_recoID);
      h_relDiff_ID   = PlotTool::Get_Hist2D(fileName_ID_,   histName_recoID);
      h_relDiff_trig = PlotTool::Get_Hist2D(fileName_trig_, histName_trig);
    }

    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        Int_t i_etaBin = i_eta + 1;
        Int_t i_ptBin  = i_pt + 1;

        if( uncType == "bkgChange" || uncType == "sgnChange" ) // -- data variation, no trigger
        {
          diff_reco_data_[i_eta][i_pt] = h_relDiff_reco->GetBinContent(i_etaBin, i_ptBin) * eff_reco_data_[i_eta][i_pt];
          diff_ID_data_[i_eta][i_pt]   = h_relDiff_ID->GetBinContent(i_etaBin, i_ptBin)   * eff_ID_data_[i_eta][i_pt];
        }
        else if( uncType == "nlo" ) // -- MC variation, including trigger
        {
          diff_reco_MC_[i_eta][i_pt] = h_relDiff_reco->GetBinContent(i_etaBin, i_ptBin) * eff_reco_MC_[i_eta][i_pt];
          diff_ID_MC_[i_eta][i_pt]   = h_relDiff_ID->GetBinContent(i_etaBin, i_ptBin)   * eff_ID_MC_[i_eta][i_pt];
          diff_trig_MC_[i_eta][i_pt] = h_relDiff_trig->GetBinContent(i_etaBin, i_ptBin) * eff_trig_MC_[i_eta][i_pt];
        }
        else if( uncType == "tagChange" ) // -- MC variation for reco+ID & data variation for trigger
        {
          diff_reco_MC_[i_eta][i_pt]   = h_relDiff_reco->GetBinContent(i_etaBin, i_ptBin) * eff_reco_MC_[i_eta][i_pt];
          diff_ID_MC_[i_eta][i_pt]     = h_relDiff_ID->GetBinContent(i_etaBin, i_ptBin)   * eff_ID_MC_[i_eta][i_pt];
          diff_trig_data_[i_eta][i_pt] = h_relDiff_trig->GetBinContent(i_etaBin, i_ptBin) * eff_trig_data_[i_eta][i_pt];
        }
      }
    }
  }

  Int_t FindPtBin(Double_t pt )   { return FindBin(pt,  vec_ptBinEdge_ ); }
  Int_t FindEtaBin(Double_t eta ) { return FindBin(eta, vec_etaBinEdge_ ); }

  Int_t FindBin(Double_t value, vector<Double_t> vec_binEdge_ )
  {
    Int_t binNum = -1;

    Int_t nBin = (Int_t)vec_binEdge_.size() - 1;

    if( value > vec_binEdge_.back() ) // -- if the value is larger than the last bin edge
      binNum = nBin - 1;

    else if( value < vec_binEdge_[0] ) // -- if the value is smaller than the first bin edge
      binNum = 0;
    
    else
    {
      for(Int_t i=0; i<nBin; i++)
      {
        if( vec_binEdge_[i] <= value && value < vec_binEdge_[i+1] )
        {
          binNum = i;
          break;
        }
      }
    }

    if( binNum == -1 )
    {
      cout << "[TnPEfficiency::FindBin] binNum  == -1 ... Something wrong!!: value = " << value << endl;
    }

    return binNum;
  }
};