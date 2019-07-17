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

  Double_t eff_reco_data_[nEtaBin][nPtBin];
  Double_t eff_reco_MC_[nEtaBin][nPtBin];

  Double_t eff_ID_data_[nEtaBin][nPtBin];
  Double_t eff_ID_MC_[nEtaBin][nPtBin];

  Double_t eff_trig_data_[nEtaBin][nPtBin];
  Double_t eff_trig_MC_[nEtaBin][nPtBin];

  // -- for the uncertainty estimation:  |eff(central value) - eff(alt. value)| 
  Double_t absUnc_reco_data_[nEtaBin][nPtBin];
  Double_t absUnc_reco_MC_[nEtaBin][nPtBin];

  Double_t absUnc_ID_data_[nEtaBin][nPtBin];
  Double_t absUnc_ID_MC_[nEtaBin][nPtBin];

  Double_t absUnc_trig_data_[nEtaBin][nPtBin];
  Double_t absUnc_trig_MC_[nEtaBin][nPtBin];

  TnPEfficiency()
  {
    TH1::AddDirectory(kFALSE);
    if( nPtBin != vec_ptBinEdge_.size() - 1 )
      cout << "Warning: nPtBin != vec_ptBinEdge_.size() - 1" << endl;
    if( nEtaBin != vec_etaBinEdge_.size() - 1 )
      cout << "Warning: nEtaBin != vec_etaBinEdge_.size() - 1" << endl;

    Init();
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

        absUnc_reco_data_[i_eta][i_pt] = 0;
        absUnc_reco_MC_[i_eta][i_pt] = 0;
        absUnc_ID_data_[i_eta][i_pt] = 0;
        absUnc_ID_MC_[i_eta][i_pt] = 0;
        absUnc_trig_data_[i_eta][i_pt] = 0;
        absUnc_trig_MC_[i_eta][i_pt] = 0;
      }
    }

    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString baseDir = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult";

    fileName_reco_ = baseDir+"/ROOTFile_RecoSF_April2019.root";
    fileName_ID_   = baseDir+"/ROOTFile_IDSF_April2019.root";
    fileName_trig_ = baseDir+"/ROOTFile_trigSF_SMP17001.root";

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

        absUnc_reco_data_[i_eta][i_pt] = h_absUnc_stat_reco_data->GetBinContent(i_etaBin, i_ptBin);
        absUnc_reco_MC_[i_eta][i_pt]   = h_absUnc_stat_reco_MC->GetBinContent(i_etaBin, i_ptBin);

        absUnc_ID_data_[i_eta][i_pt] = h_absUnc_stat_ID_data->GetBinContent(i_etaBin, i_ptBin);
        absUnc_ID_MC_[i_eta][i_pt]   = h_absUnc_stat_ID_MC->GetBinContent(i_etaBin, i_ptBin);

        absUnc_trig_data_[i_eta][i_pt] = h_absUnc_stat_trig_data->GetBinContent(i_etaBin, i_ptBin);
        absUnc_trig_MC_[i_eta][i_pt]   = h_absUnc_stat_trig_MC->GetBinContent(i_etaBin, i_ptBin);
      }
    }

    // -- randomization
    TRandom3 ran;
    ran.SetSeed(0);
    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        eff_reco_data_[i_eta][i_pt] = eff_reco_data_[i_eta][i_pt] + ran.Gaus(0.0, 1.0) * absUnc_reco_data_[i_eta][i_pt];
        eff_reco_MC_[i_eta][i_pt]   = eff_reco_MC_[i_eta][i_pt]   + ran.Gaus(0.0, 1.0) * absUnc_reco_MC_[i_eta][i_pt];

        eff_ID_data_[i_eta][i_pt] = eff_ID_data_[i_eta][i_pt] + ran.Gaus(0.0, 1.0) * absUnc_ID_data_[i_eta][i_pt];
        eff_ID_MC_[i_eta][i_pt]   = eff_ID_MC_[i_eta][i_pt]   + ran.Gaus(0.0, 1.0) * absUnc_ID_MC_[i_eta][i_pt];

        eff_trig_data_[i_eta][i_pt] = eff_trig_data_[i_eta][i_pt] + ran.Gaus(0.0, 1.0) * absUnc_trig_data_[i_eta][i_pt];
        eff_trig_MC_[i_eta][i_pt]   = eff_trig_MC_[i_eta][i_pt]   + ran.Gaus(0.0, 1.0) * absUnc_trig_MC_[i_eta][i_pt];
      }
    }    
  }

  void SmearingEff_Syst(TString uncType)
  {
    TString dataType = "";
    if( uncType == "bkgChange" || uncType == "sgnChange" ) dataType = "data";
    if( uncType == "tagChange" || uncType == "nlo" )       dataType = "mc";
    TString histName = "h_absRelDiff_"+uncType+"_"+dataType;

    TH2D* h_relUnc_reco = nullptr;
    TH2D* h_relUnc_ID   = nullptr;
    TH2D* h_relUnc_trig = nullptr;

    if( dataType == "data" )
    {
      h_relUnc_reco = PlotTool::Get_Hist2D(fileName_reco_, histName);
      h_relUnc_ID   = PlotTool::Get_Hist2D(fileName_ID_,   histName);
      // -- no uncertainty on the data in the trigger efficiency (cut & count)
    }
    else if( dataType == "mc" )
    {
      h_relUnc_reco = PlotTool::Get_Hist2D(fileName_reco_, histName);
      h_relUnc_ID   = PlotTool::Get_Hist2D(fileName_ID_,   histName);
      h_relUnc_trig = PlotTool::Get_Hist2D(fileName_trig_, histName);
    }

    for(Int_t i_eta = 0; i_eta < nEtaBin; i_eta++)
    {
      for(Int_t i_pt = 0; i_pt < nPtBin; i_pt++)
      {
        Int_t i_etaBin = i_eta + 1;
        Int_t i_ptBin  = i_pt + 1;

        if( dataType == "data" )
        {
          absUnc_reco_data_[i_eta][i_pt] = h_relUnc_reco->GetBinContent(i_etaBin, i_ptBin) * eff_reco_data_[i_eta][i_pt];
          absUnc_ID_data_[i_eta][i_pt]   = h_relUnc_ID->GetBinContent(i_etaBin, i_ptBin) * eff_ID_data_[i_eta][i_pt];
        }
        else if( dataType == "mc" )
        {
          absUnc_reco_MC_[i_eta][i_pt] = h_relUnc_reco->GetBinContent(i_etaBin, i_ptBin) * eff_reco_MC_[i_eta][i_pt];
          absUnc_ID_MC_[i_eta][i_pt]   = h_relUnc_ID->GetBinContent(i_etaBin, i_ptBin) * eff_ID_MC_[i_eta][i_pt];
          absUnc_trig_MC_[i_eta][i_pt] = h_relUnc_trig->GetBinContent(i_etaBin, i_ptBin) * eff_trig_MC_[i_eta][i_pt];

          printf("[(i_eta, i_pt) = (%d, %d)] absUnc: (reco, ID, trig) = (%lf, %lf, %lf)\n", i_eta, i_pt, absUnc_reco_MC_[i_eta][i_pt], absUnc_ID_MC_[i_eta][i_pt], absUnc_trig_MC_[i_eta][i_pt]);
        }
      }
    }

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
        if( dataType == "data" )
        {
          eff_reco_data_[i_eta][i_pt] = eff_reco_data_[i_eta][i_pt] + rndNum_reco * absUnc_reco_data_[i_eta][i_pt];
          eff_ID_data_[i_eta][i_pt]   = eff_ID_data_[i_eta][i_pt]   + rndNum_ID   * absUnc_ID_data_[i_eta][i_pt];
        }
        else if( dataType == "mc" )
        {
          eff_reco_MC_[i_eta][i_pt] = eff_reco_MC_[i_eta][i_pt] + rndNum_reco * absUnc_reco_MC_[i_eta][i_pt];
          eff_ID_MC_[i_eta][i_pt]   = eff_ID_MC_[i_eta][i_pt]   + rndNum_ID   * absUnc_ID_MC_[i_eta][i_pt];
          eff_trig_MC_[i_eta][i_pt] = eff_trig_MC_[i_eta][i_pt] + rndNum_trig * absUnc_trig_MC_[i_eta][i_pt];
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