#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>

#include <vector>
using namespace std;

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

Bool_t CompareObject( GenLepton lep1, GenLepton lep2 )
{
  return lep1.Pt > lep2.Pt;
}

class GenPair
{
public:
  GenLepton first_;
  GenLepton second_;

  Int_t leptonID_;
  Double_t mass_;


  GenPair( GenLepton lepton1, GenLepton lepton2 )
  {
    if( lepton1.Pt > lepton2.Pt )
    {
      first_  = lepton1;
      second_ = lepton2;
    }
    else
    {
      first_  = lepton2;
      second_ = lepton1;
    }

    Init();
  }

  void Print()
  {
    printf("[Leading muon]     (pt, eta, phi) = (%7.3lf, %5.3lf, %5.3lf)\n", first_.Pt, first_.eta, first_.phi);
    printf("[Sub-Leading muon] (pt, eta, phi) = (%7.3lf, %5.3lf, %5.3lf)\n", second_.Pt, second_.eta, second_.phi);
    printf("   Dilepton mass: %.3lf\n\n", mass_);
  }

  Bool_t CheckAcceptance_EE()
  {
    Bool_t pass = kFALSE;

    Double_t ptCut_lead = 30.0;
    Double_t ptCut_sub = 10.0;
    Double_t etaCut_lead = 2.5;
    Double_t etaCut_sub = 2.5;

    if( first_.Pt  > ptCut_lead &&
        second_.Pt > ptCut_sub && 
        fabs(first_.eta)  < etaCut_lead &&
        fabs(second_.eta) < etaCut_sub &&
        !( fabs(first_.eta)  > 1.4442 && fabs(first_.eta)  < 1.566 ) &&
        !( fabs(second_.eta) > 1.4442 && fabs(second_.eta) < 1.566 ) )
      pass = kTRUE;

    return pass;
  }

private:
  void Init()
  {
    if( first_.ID == second_.ID )
      leptonID_ = first_.ID;
    else
      leptonID_ = 0;

    mass_ = (first_.Momentum + second_.Momentum).M();
  }
};

class HistContainer
{
public:
  HistContainer(TString type)
  {
    type_ = type;
    Init();
  }

  void Fill( GenPair& genPair, Double_t weight )
  {
    h_mass_->Fill( genPair.mass_, weight );

    h_ptLead_->Fill( genPair.first_.Pt, weight );
    h_ptSub_->Fill( genPair.second_.Pt, weight );

    h_etaLead_->Fill( genPair.first_.eta, weight );
    h_etaSub_->Fill( genPair.second_.eta, weight );
  }

  void Save(TFile *f_output)
  {
    f_output->cd();

    for(const auto* hist : hists_ )
      hist->Write();
  }

private:
  vector<TH1D*> hists_;
  TH1D* h_mass_;
  TH1D* h_ptLead_;
  TH1D* h_ptSub_;
  TH1D* h_etaLead_;
  TH1D* h_etaSub_;

  TString type_;

  void Init()
  {
    Double_t massBinEdges[nMassBin+1] = 
    {
      15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
      64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
      110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
      200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
      830, 1000, 1500, 3000
    };

    h_mass_ = new TH1D("h_mass_"+type_, "", nMassBin, massBinEdges);
    hists_.push_back(h_mass_);

    h_ptLead_ = new TH1D("h_ptLead_"+type_, "", 10000, 0, 10000);
    hists_.push_back(h_ptLead_);

    h_ptSub_ = new TH1D("h_ptSub_"+type_, "", 10000, 0, 10000);
    hists_.push_back(h_ptSub_);

    h_etaLead_ = new TH1D("h_etaLead_"+type_, "", 10000, 0, 10000);
    hists_.push_back(h_etaLead_);

    h_etaSub_ = new TH1D("h_etaSub_"+type_, "", 10000, 0, 10000);
    hists_.push_back(h_etaSub_);
  }
};

class HistProducer
{
public:
  vector<TString> dataPaths_;
  Double_t xSec_;

  HistProducer()
  {
    xSec_ = 0;

  }

  void AddDataPath(TString path)
  {
    dataPaths_.push_back( path );
  }

  void Produce()
  {
    CheckBasics();

    TTimeStamp ts_start;
    cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

    TStopwatch totaltime;
    totaltime.Start();

    DYAnalyzer *analyzer = new DYAnalyzer( "None" );

    TChain *chain = new TChain("recoTree/DYTree");
    for(const auto& dataPath : dataPaths_ )
      chain->Add(dataPath);

    NtupleHandle *ntuple = new NtupleHandle( chain );
    // ntuple->TurnOnBranches_GenOthers();
    ntuple->TurnOnBranches_GenLepton();

    HistContainer* hists_genFlag = new HistContainer( "GenFlag" );
    HistContainer* hists_highestPt = new HistContainer( "highestPt" );

    Double_t sumWeight = 0;

    Int_t nTotEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nTotEvent << "]" << endl;

    // nTotEvent = 1000;
    // -- Event loop starts -- //
    for(Int_t i=0; i<nTotEvent; i++)
    {
      loadBar(i+1, nTotEvent, 100, 100);

      ntuple->GetEvent(i);

      if( IsDYTauTauEvent(ntuple) ) continue;

      Double_t genWeight = 0;
      ntuple->GENEvt_weight < 0 ? genWeight = -1 : genWeight = 1;
      sumWeight = sumWeight + genWeight;

      // Bool_t GenFlag = kFALSE;
      // GenFlag = analyzer->SeparateDYLLSample_isHardProcess("DYEE", ntuple);

      GenPair *genPair_genFlag = NULL;
      Bool_t isFound_genFlag = FindGenLeptonPair_GenFlag(ntuple, genPair_genFlag);
      if( isFound_genFlag )
        hists_genFlag->Fill( *genPair_genFlag, genWeight );

      GenPair *genPair_highestPt = NULL;
      Bool_t isFound_highestPt = FindGenLeptonPair_HighestPt(ntuple, genPair_highestPt);
      if( isFound_highestPt )
        hists_highestPt->Fill( *genPair_highestPt, genWeight );

      if( isFound_genFlag && !isFound_highestPt )
      {
        printf("[%d event]\n", i);
        printf("   Highest pT method cannot find correct DY->ee pair\n");
        Print_AllFinalStateLeptons(ntuple);
        printf("[genPair_genFlag]\n");
        genPair_genFlag->Print();
      }

      if( !isFound_genFlag && isFound_highestPt )
      {
        printf("[%d event]\n", i);
        printf("   Highest pT method finds wrong DY->ee pair\n");
        Print_AllFinalStateLeptons(ntuple);
        printf("[genPair_highestPt]\n");
        genPair_highestPt->Print();
      }

      if( isFound_genFlag && isFound_highestPt )
      {
        Double_t ptLead_genFlag = genPair_genFlag->first_.Pt;
        Double_t ptLead_highestPt = genPair_highestPt->first_.Pt;

        Double_t relDiff_lead = (ptLead_highestPt - ptLead_genFlag ) / ptLead_genFlag;

        Double_t ptSub_genFlag = genPair_genFlag->second_.Pt;
        Double_t ptSub_highestPt = genPair_highestPt->second_.Pt;

        Double_t relDiff_sub = (ptSub_highestPt - ptSub_genFlag ) / ptSub_genFlag;
        if( relDiff_lead > 0.1 || relDiff_sub > 0.1 )
        {
          printf("Kinematics from two different selection method is different\n");
          printf("[Leading muon]     (pt_genFlag, pt_highestPt) = (%.3lf, %.3lf) -> Rel.Diff = %.3lf\n", ptLead_genFlag, ptLead_highestPt, relDiff_lead);
          printf("[Sub-leading muon] (pt_genFlag, pt_highestPt) = (%.3lf, %.3lf) -> Rel.Diff = %.3lf\n", ptSub_genFlag, ptSub_highestPt, relDiff_sub);
        }
      }
    }

    printf("[Sum of weight: %.1lf]\n", sumWeight);

    TFile *f_output = TFile::Open("ROOTFile_FiducialXSec_DYEE.root", "RECREATE");
    hists_genFlag->Save( f_output );
    hists_highestPt->Save( f_output );

    f_output->Close();
  }

private:
  void CheckBasics()
  {
    Int_t nDataPath = (Int_t)dataPaths_.size();
    if( nDataPath == 0 )
      cout << "No data paths are found to run!" << endl;

    if( xSec_ == 0)
      cout << "No cross section is assigned" << endl;
  }

  Bool_t IsDYTauTauEvent( NtupleHandle* ntuple )
  {
    Bool_t isFound = kFALSE;
    vector<GenLepton> genLeptons;
    for(Int_t i_gen=0; i_gen<ntuple->gnpair; i_gen++)
    {
      GenLepton genLepton;
      genLepton.FillFromNtuple(ntuple, i_gen);
      if( abs(genLepton.ID) == 15 && genLepton.isHardProcess )
        genLeptons.push_back( genLepton );
    }

    Int_t nGenLepton = (Int_t)genLeptons.size();
    if( nGenLepton == 2 )
      isFound = kTRUE;

    return isFound;
  }

  void Print_AllFinalStateLeptons(NtupleHandle* ntuple)
  {
    printf("[Print_AllFinalStateLeptons]\n");
    Int_t i_lepton = 0;
    for(Int_t i_gen=0; i_gen<ntuple->gnpair; i_gen++)
    {
      GenLepton genLepton;
      genLepton.FillFromNtuple(ntuple, i_gen);
      if( abs(genLepton.ID) == 11 || abs(genLepton.ID) == 13 || abs(genLepton.ID) == 15 )
      {
        if( genLepton.Status == 1 )
        {
          printf("   [%02d lepton] (ID, pt, eta, phi, fromHardProcessFinalState) = (%d, %7.3lf, %5.3lf, %5.3lf, %d)\n", 
                 i_lepton, genLepton.ID, genLepton.Pt, genLepton.eta, genLepton.phi, genLepton.fromHardProcessFinalState);
          i_lepton++;
        }
      }
    }
    printf("\n");
  }

  Bool_t FindGenLeptonPair_GenFlag( NtupleHandle* ntuple, GenPair*& genPair )
  {
    Bool_t isFound = kFALSE;
    vector<GenLepton> genLeptons;
    for(Int_t i_gen=0; i_gen<ntuple->gnpair; i_gen++)
    {
      GenLepton genLepton;
      genLepton.FillFromNtuple(ntuple, i_gen);
      if( abs(genLepton.ID) == 11 && genLepton.fromHardProcessFinalState )
        genLeptons.push_back( genLepton );
    }

    Int_t nGenLepton = (Int_t)genLeptons.size();
    if( nGenLepton == 2 )
    {
      genPair = new GenPair( genLeptons[0], genLeptons[1] );

      if( genPair->CheckAcceptance_EE() )
        isFound = kTRUE;
      else
        delete genPair;
    }

    return isFound;
  }

  Bool_t FindGenLeptonPair_HighestPt( NtupleHandle* ntuple, GenPair*& genPair )
  {
    Bool_t isFound = kFALSE;

    vector<GenLepton> genLeptons;
    for(Int_t i_gen=0; i_gen<ntuple->gnpair; i_gen++)
    {
      GenLepton genLepton;
      genLepton.FillFromNtuple(ntuple, i_gen);
      if( abs(genLepton.ID) == 11 && genLepton.Status == 1 ) // -- final state electrons
        genLeptons.push_back( genLepton );
    }

    // -- sort by pT
    std::sort(genLeptons.begin(), genLeptons.end(), CompareObject);

    int index_lepton1 = -1;
    int index_lepton2 = -1;
    FindPFSLeptonPair_HighestPtWithinAcc( genLeptons, 11, index_lepton1, index_lepton2 );
    if( index_lepton1 != -1 && index_lepton2 != -1 )
    {
      isFound = kTRUE;
      genPair = new GenPair( genLeptons[index_lepton1], genLeptons[index_lepton2] );
    }

    return isFound;
  }

  void FindPFSLeptonPair_HighestPtWithinAcc(vector<GenLepton> leptonsByPt, int leptonID, int& index_lepton1, int& index_lepton2)
  {
    double pTCut_lead = 0;
    if( leptonID == 11 ) pTCut_lead = 30.0;
    if( leptonID == 13 ) pTCut_lead = 22.0;
    double pTCut_sub = 10.0; // same for both channel

    double etaCut_lead = 0;
    if( leptonID == 11 ) etaCut_lead = 2.5;
    if( leptonID == 13 ) etaCut_lead = 2.4;
    double etaCut_sub = etaCut_lead;

    int nLepton = int(leptonsByPt.size());
    for(int i=0; i<nLepton; i++) {
      auto& lepton = leptonsByPt[i];
      if( lepton.Pt > pTCut_lead && fabs(lepton.eta) < etaCut_lead ) {
        if( leptonID == 11 ) { // electron channel: check ECAL gap
          if( !(fabs(lepton.eta) > 1.4442 && fabs(lepton.eta) < 1.566) ) {
            index_lepton1 = i;
            break;
          }
        }
        else{ // muon channel
          index_lepton1 = i;
          break;
        }
      }
    } // end of lepton iteration

    // if no lepton is found in the leptons_PFS
    if( index_lepton1 < 0 ) {
      index_lepton1 = -1;
      index_lepton2 = -1;
      return;
    }

    // 2nd lepton: lepton with highest-pT among the leptons with the opposite sign with 1st lepton
    int leptonID_lepton1 = leptonsByPt[index_lepton1].ID;
    for(int i=index_lepton1+1; i<nLepton; i++) { // starting after lepton1
      auto& lepton = leptonsByPt[i];
      if( lepton.ID == (-1)*leptonID_lepton1 && 
          lepton.Pt > pTCut_sub && fabs(lepton.eta) < etaCut_sub ) {
        if( leptonID == 11 ) { // electron channel: check ECAL gap
          if( !(fabs(lepton.eta) > 1.4442 && fabs(lepton.eta) < 1.566) ) {
            index_lepton2 = i;
            break;
          }
        }
        else { // muon channel
          index_lepton2 = i;
          break;
        }
      }
    } // end of lepton iteration
  }

  static inline void loadBar(int x, int n, int r, int w)
  {
      // Only update r times.
      if( x == n )
        cout << endl;

      if ( x % (n/r +1) != 0 ) return;

   
      // Calculuate the ratio of complete-to-incomplete.
      float ratio = x/(float)n;
      int   c     = ratio * w;
   
      // Show the percentage complete.
      printf("%3d%% [", (int)(ratio*100) );
   
      // Show the load bar.
      for (int x=0; x<c; x++) cout << "=";
   
      for (int x=c; x<w; x++) cout << " ";
   
      // ANSI Control codes to go back to the
      // previous line and clear it.
    cout << "]\r" << flush;

  }
};

void FiducialXSec_DYEE()
{
  TString dataPathBase = gSystem->Getenv("KP_DATA_PATH");

  HistProducer *producer = new HistProducer();
  producer->AddDataPath(dataPathBase+"/76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns/ntuple_*.root");
  // producer->AddDataPath("./Local/ntuple_skim_377.root");
  producer->xSec_ = 18610.0 / 3.0;

  producer->Produce();
}