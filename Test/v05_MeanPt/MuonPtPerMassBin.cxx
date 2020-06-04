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

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>
static inline void loadBar(int x, int n, int r, int w);

void MuonPtPerMassBin()
{
  // -- histogram definitions
  TH1D* h_sumWeight = new TH1D("h_sumWeight", "", 1, 0, 1);
  vector<Double_t> vec_massBinEdge = {10, 15, 20, 25, 30, 35, 40, 45, 50};
  vector<TH1D*> vec_hist_pt;
  vector<TH1D*> vec_hist_pt_lead;
  vector<TH1D*> vec_hist_pt_sub;

  Int_t nMassBin = (Int_t)vec_massBinEdge.size() - 1;
  for(Int_t i=0; i<nMassBin; i++)
  {
    Double_t minM = vec_massBinEdge[i];
    Double_t maxM = vec_massBinEdge[i+1];

    TString tstr_massRange = TString::Format("M%.0lfto%.0lf", minM, maxM);

    TH1D* h_temp_pt      = new TH1D("h_pt_"+tstr_massRange, "", 10000, 0, 10000);
    TH1D* h_temp_pt_lead = new TH1D("h_pt_lead_"+tstr_massRange, "", 10000, 0, 10000);
    TH1D* h_temp_pt_sub  = new TH1D("h_pt_sub_"+tstr_massRange, "", 10000, 0, 10000);

    vec_hist_pt.push_back( h_temp_pt );
    vec_hist_pt_lead.push_back( h_temp_pt_lead );
    vec_hist_pt_sub.push_back( h_temp_pt_sub );
  }

  TString ntupleDir = "/data7/Users/kplee/backup/data4/Users/kplee/DYntuple/76X/v20160304_76X_MINIAODv2_DYLL_M10to50_25ns";
  TString tag = "DYMuMu_M10to50";
  Double_t xSec = 18610.0/3.0;
  Double_t sumWeight = 7506956.0;

  DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );

  TChain *chain = new TChain("recoTree/DYTree");
  chain->Add(ntupleDir+"/ntuple_*.root");

  NtupleHandle *ntuple = new NtupleHandle( chain );
  ntuple->TurnOnBranches_GenLepton();
  // ntuple->TurnOnBranches_HLT();
  // ntuple->TurnOnBranches_Muon();

  Double_t sumWeight_tot = 0;
  Double_t sumWeight_M40to50 = 0;

  Int_t nEvent = chain->GetEntries();
  cout << "\t[Total Events: " << nEvent << "]" << endl;
  for(Int_t i=0; i<nEvent; i++)
  {
    loadBar(i+1, nEvent, 100, 100);

    ntuple->GetEvent(i);

    Bool_t isMuMuEvent = kFALSE;
    isMuMuEvent = analyzer->SeparateDYLLSample_isHardProcess(tag, ntuple);

    if( isMuMuEvent )
    {
      Double_t genWeight = ntuple->GENEvt_weight < 0 ? -1 : 1;
      h_sumWeight->Fill(0.5, genWeight);

      vector<GenLepton> vec_genLepton;
      Int_t nGenLepton = ntuple->gnpair;
      for(Int_t i_gen=0; i_gen<nGenLepton; i_gen++)
      {
        GenLepton genLepton;
        genLepton.FillFromNtuple(ntuple, i_gen);
        if( genLepton.isMuon() && genLepton.fromHardProcessFinalState )
          vec_genLepton.push_back( genLepton );
      }

      Int_t nGenLeptonHP = (Int_t)vec_genLepton.size();
      if( nGenLeptonHP != 2 )
      {
        cout << "nGenLeptonHP = " << nGenLeptonHP << " != 2 ... something wrong!!" << endl;
        return;
      }

      Double_t diMuM = (vec_genLepton[0].Momentum + vec_genLepton[1].Momentum).M();

      Double_t pt_lead, pt_sub;
      if( vec_genLepton[0].pt > vec_genLepton[1].pt )
      {
        pt_lead = vec_genLepton[0].pt;
        pt_sub  = vec_genLepton[1].pt;
      }
      else
      {
        pt_lead = vec_genLepton[1].pt;
        pt_sub  = vec_genLepton[0].pt;
      }

      if( pt_lead > 22.0 && pt_sub > 10.0 && 
          fabs(vec_genLepton[0].eta) < 2.4 && fabs(vec_genLepton[1].eta) < 2.4 ) // -- events only passing acceptance condition
      {
        for(Int_t i=0; i<nMassBin; i++)
        {
          Double_t minM = vec_massBinEdge[i];
          Double_t maxM = vec_massBinEdge[i+1];

          if( minM < diMuM && diMuM < maxM )
          {
            vec_hist_pt[i]->Fill( pt_lead, genWeight );
            vec_hist_pt[i]->Fill( pt_sub, genWeight );

            vec_hist_pt_lead[i]->Fill( pt_lead, genWeight );
            vec_hist_pt_sub[i]->Fill( pt_sub, genWeight );
          }
        }
      } // -- acceptance condition
    } // -- isMuMu event

  } // -- event iteration

  TFile *f_output = TFile::Open("ROOTFile_MuonPtPerMassBin.root", "RECREATE");
  f_output->cd();
  h_sumWeight->Write();
  for(Int_t i=0; i<nMassBin; i++)
  {
    vec_hist_pt[i]->Write();
    vec_hist_pt_lead[i]->Write();
    vec_hist_pt_sub[i]->Write();
  }
  f_output->Close();
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