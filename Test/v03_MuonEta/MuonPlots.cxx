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
#include <TROOT.h>
#include <TApplication.h>
#include <vector>
#include <TMath.h>

// -- for Rochester Muon momentum correction -- //
#include <Include/RochesterMomCorr_76X/RoccoR.cc>
#include <Include/RochesterMomCorr_76X/rochcor2015.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

class HistContainer
{
public:
  TString type_;

  TH1D* h_pt_;
  TH1D* h_pt_lead_;
  TH1D* h_pt_subLead_;

  TH1D* h_eta_;
  TH1D* h_eta_lead_;
  TH1D* h_eta_subLead_;

  TH1D* h_phi_;
  TH1D* h_phi_lead_;
  TH1D* h_phi_subLead_;

  TH1D* h_mass_;

  vector<TH1D*> hists_;

  HistContainer(TString type)
  {
    type_ = type;
    Init();
  }

  void Fill( Muon& mu_lead, Muon& mu_subLead, Double_t weight )
  {
    if( mu_lead.Pt < mu_subLead.Pt )
    {
      cout << "Wrong input! ... leading muon pT < sub-leading muon pT!" << endl;
      return;
    }

    Double_t mass = (mu_lead.Momentum + mu_subLead.Momentum).M();
    h_mass_->Fill(mass, weight);

    h_pt_->Fill( mu_lead.Pt, weight );
    h_pt_->Fill( mu_subLead.Pt, weight );
    h_pt_lead_->Fill( mu_lead.Pt, weight );
    h_pt_subLead_->Fill( mu_subLead.Pt, weight );

    h_eta_->Fill( mu_lead.eta, weight );
    h_eta_->Fill( mu_subLead.eta, weight );
    h_eta_lead_->Fill( mu_lead.eta, weight );
    h_eta_subLead_->Fill( mu_subLead.eta, weight );

    h_phi_->Fill( mu_lead.phi, weight );
    h_phi_->Fill( mu_subLead.phi, weight );
    h_phi_lead_->Fill( mu_lead.phi, weight );
    h_phi_subLead_->Fill( mu_subLead.phi, weight );
  }

  void Save(TFile *f_output)
  {
    f_output->cd();
    for( const auto& hist : hists_ )
      hist->Write();

    cout << "All histograms are stored in " << f_output->GetName() << endl;
  }

private:
  void Init()
  {
    h_mass_        = new TH1D("h_mass_"+type_,         "", 10000, 0, 10000); hists_.push_back( h_mass_ );

    h_pt_          = new TH1D("h_pt_"+type_,           "", 10000, 0, 10000); hists_.push_back( h_pt_ );
    h_pt_lead_     = new TH1D("h_pt_lead_"+type_,      "", 10000, 0, 10000); hists_.push_back( h_pt_lead_ );
    h_pt_subLead_  = new TH1D("h_pt_subLead_"+type_,   "", 10000, 0, 10000); hists_.push_back( h_pt_subLead_ );

    h_eta_         = new TH1D("h_eta_"+type_,          "", 60, -3, 3); hists_.push_back( h_eta_ );
    h_eta_lead_    = new TH1D("h_eta_lead_"+type_,     "", 60, -3, 3); hists_.push_back( h_eta_lead_ );
    h_eta_subLead_ = new TH1D("h_eta_subLead_"+type_,  "", 60, -3, 3); hists_.push_back( h_eta_subLead_ );

    h_phi_         = new TH1D("h_phi_"+type_,          "", 80, -4, 4); hists_.push_back( h_phi_ );
    h_phi_lead_    = new TH1D("h_phi_lead_"+type_,     "", 80, -4, 4); hists_.push_back( h_phi_lead_ );
    h_phi_subLead_ = new TH1D("h_phi_subLead_"+type_,  "", 80, -4, 4); hists_.push_back( h_phi_subLead_ );
  }
};

class HistProducer
{
public:
  TString type_;
  Bool_t applyEffSF_;
  Bool_t zMassConstraint_;

  HistProducer()
  {
    type_ = "NULL";
    applyEffSF_ = kFALSE;
  }

  void Set_Type( TString type )
  {
    type_ = type;
  }

  void Set_ApplyEffSF(Bool_t apply )
  {
    applyEffSF_ = apply;
  }

  void Set_ZMassConstraint( Bool_t flag )
  {
    zMassConstraint_ = flag;
  }

  void Produce()
  {
    if( applyEffSF_ && (type_ == "MuonPhys" || type_ == "Golden") )
    {
      printf("applyEffSF == True for the data ... it is ignored and SF will not be applied!\n");
      applyEffSF_ = kFALSE;
    }

    TTimeStamp ts_start;
    cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

    TStopwatch totaltime;
    totaltime.Start();

    DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );
    analyzer->SetupEfficiencyScaleFactor( "ROOTFile_TagProbeEfficiency_76X_v20160502.root" );

    TString dataPath = gSystem->Getenv("KP_DATA_PATH");

    vector<TString> ntuplePath; 
    vector<TString> tag;
    vector<Double_t> xSec;
    vector<Double_t> nEvent;

    if( type_ == "MC" )
    {
      analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntuplePath, &tag, &xSec, &nEvent);
    }
    else if( type_ == "MuonPhys" )
    {
      ntuplePath.push_back( "76X/v20160205_SingleMuon_RunC_Rereco_MuonPhys" ); // -- Run2015C. D will be added later -- //
      tag.push_back( "Data" ); 
    }
    else
    {
      cout << "Unknown type: " << type_ << endl;
      return;
    }

    TString applyEffSFStr = "";
    if( applyEffSF_ ) applyEffSFStr = "withSF";
    else              applyEffSFStr = "withoutSF";

    TString fileName = "ROOTFile_MuonPlot_"+type_+"_"+applyEffSFStr+".root";
    if( type_ == "MuonPhys" || type_ == "Golden" ) // -- data
      fileName = "ROOTFile_MuonPlot_"+type_+".root";

    TFile *f_output = TFile::Open(fileName, "RECREATE");

    // -- sample iterations
    const Int_t nSample = (Int_t)ntuplePath.size();
    for(Int_t i_sample = 0; i_sample<nSample; i_sample++)
    {
      TStopwatch looptime;
      looptime.Start();

      printf("[%s]\n", tag[i_sample].Data());

      Bool_t isMC;
      tag[i_sample] == "Data" ? isMC = kFALSE : isMC = kTRUE;

      TChain *chain = new TChain("recoTree/DYTree");
      chain->Add(dataPath + "/" + ntuplePath[i_sample]+"/ntuple_*.root");
      if( type_ == "MuonPhys" )
        chain->Add(dataPath+"/76X/v20160303_SingleMuon_RunD_Rereco_MuonPhys/*.root"); // -- add RunD dataset

      NtupleHandle *ntuple = new NtupleHandle( chain );
      ntuple->TurnOnBranches_HLT();
      ntuple->TurnOnBranches_Muon();
      if( isMC ) ntuple->TurnOnBranches_GenLepton();

      // -- for the momentum correction
      rochcor2015 *rmcor = new rochcor2015();

      // -- for the PU reweighting (works only for MC)
      analyzer->SetupPileUpReWeighting_76X( isMC );

      Bool_t isNLO = 0;
      if( tag[i_sample].Contains("DYMuMu") || tag[i_sample].Contains("DYTauTau") || tag[i_sample] == "WJets" )
      {
        isNLO = 1;
        cout << "\t" << tag[i_sample] << ": generated with NLO mode - Weights are applied" << endl;
      }

      Double_t sumW = 0;
      Double_t sumW_separated = 0;

      HistContainer *histContainer = new HistContainer( tag[i_sample] );

      Int_t nTotEvent = chain->GetEntries();
      cout << "\t[Total Events: " << nTotEvent << "]" << endl;

      // -- event iterations
      for(Int_t i=0; i<nTotEvent; i++)
      {
        loadBar(i+1, nTotEvent, 100, 100);

        ntuple->GetEvent(i);

        /////////////////////////////
        // -- Bring the weights -- //
        /////////////////////////////

        // -- Gen-weights
        Double_t genWeight;
        if( isNLO == 1 )
          ntuple->GENEvt_weight < 0 ? genWeight = -1 : genWeight = 1;
        else
          genWeight = 1;

        sumW += genWeight;

        // -- Pileup-Reweighting
        Double_t PUWeight = analyzer->PileUpWeightValue_76X( ntuple->nPileUp );

        Bool_t genFlag = kFALSE;
        genFlag = analyzer->SeparateDYLLSample_isHardProcess(tag[i_sample], ntuple);

        if( genFlag == 1 )
          sumW_separated += genWeight;

        // -- reco-level
        if( ntuple->isTriggered( analyzer->HLT ) && genFlag)
        {
          vector< Muon > muons;
          Int_t nLepton = ntuple->nMuon;
          for(Int_t i_reco=0; i_reco<nLepton; i_reco++)
          {
            Muon mu;
            mu.FillFromNtuple(ntuple, i_reco);

            // -- momentum correction part
            float qter = 1.0;
            
            if( tag[i_sample] == "Data" )
              rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
            else
              rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

            // -- Change Muon pT, eta and phi with updated(corrected) one -- //
            mu.Pt = mu.Momentum.Pt();
            mu.eta = mu.Momentum.Eta();
            mu.phi = mu.Momentum.Phi();

            muons.push_back( mu );
          }

          // -- Event Selection -- //
          vector< Muon > selectedMuons;
          Bool_t passSel = kFALSE;
          passSel = analyzer->EventSelection(muons, ntuple, &selectedMuons);

          if( passSel )
          {
            // -- check Z mass constraint
            Bool_t passZMassCut = kFALSE;
            if( zMassConstraint_ )
            {
              Double_t mass = (selectedMuons[0].Momentum + selectedMuons[1].Momentum).M();
              if( mass > 60 && mass < 120 )
                passZMassCut = kTRUE;
            }
            else
              passZMassCut = kTRUE; // -- always true

            if( passZMassCut )
            {
              Double_t totWeight = genWeight * PUWeight;

              if( applyEffSF_ )
              {
                Double_t effSF_HLTv4p2 = analyzer->EfficiencySF_EventWeight_HLTv4p2( selectedMuons[0], selectedMuons[1] );
                Double_t effSF_HLTv4p3 = analyzer->EfficiencySF_EventWeight_HLTv4p3( selectedMuons[0], selectedMuons[1] );

                Double_t lumiFrac_HLTv4p2 = Lumi_HLTv4p2 / Lumi;
                Double_t lumiFrac_HLTv4p3 = 1.0 - lumiFrac_HLTv4p2;

                // -- weighted sum with corresponding luminosity
                Double_t effSFWeight = effSF_HLTv4p2*lumiFrac_HLTv4p2 + effSF_HLTv4p3*lumiFrac_HLTv4p3;

                // -- update total weight by multiplying the weight from the efficiency SF
                totWeight = totWeight * effSFWeight;
              }

              if( selectedMuons[0].Pt > selectedMuons[1].Pt )
                histContainer->Fill( selectedMuons[0], selectedMuons[1], totWeight );
              else
                histContainer->Fill( selectedMuons[1], selectedMuons[0], totWeight );

            } // -- end of passZMasCut

          } // -- end of passSel

        } // -- end of isTriggered & genFlag

      } // -- end of event iteration

      histContainer->Save( f_output );
      delete histContainer;

      printf("[Total sum of weights: %.0lf\n]", sumW );
      printf("\t[Total sum of weights (separated): %.0lf\n]", sumW_separated );

      Double_t LoopRunTime = looptime.CpuTime();
      cout << "\tLoop RunTime(" << tag[i_sample] << "): " << LoopRunTime << " seconds\n" << endl;

    } // -- end of sample iterations

    f_output->Close();

    Double_t TotalRunTime = totaltime.CpuTime();
    cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

    TTimeStamp ts_end;
    cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
  }

private:
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

void MuonPlots(TString type, Bool_t applyEffSF )
{
  HistProducer *producer = new HistProducer();
  producer->Set_Type( type );
  producer->Set_ApplyEffSF( applyEffSF );
  producer->Set_ZMassConstraint( kTRUE );
  producer->Produce();
}