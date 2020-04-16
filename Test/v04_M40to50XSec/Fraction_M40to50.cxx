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

void Fraction_M40to50()
{
  TH1D* h_diMuM = new TH1D("h_diMuM", "", 10000, 0, 10000);

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

      vector<GenLepton> vec_genLepton;
      Int_t nGenLepton = ntuple->gnpair;
      for(Int_t i_gen=0; i_gen<nGenLepton; i_gen++)
      {
        GenLepton genLepton;
        genLepton.FillFromNtuple(ntuple, i_gen);
        if( genLepton.isMuon() && genLepton.isHardProcess )
          vec_genLepton.push_back( genLepton );
      }

      Int_t nGenLeptonHP = (Int_t)vec_genLepton.size();
      if( nGenLeptonHP != 2 )
      {
        cout << "nGenLeptonHP = " << nGenLeptonHP << " != 2 ... something wrong!!" << endl;
        return;
      }

      Double_t diMuM = (vec_genLepton[0].Momentum + vec_genLepton[1].Momentum).M();
      h_diMuM->Fill( diMuM, genWeight );

      sumWeight_tot += genWeight;
      if( 40.0 <= diMuM && diMuM <= 50.0 ) sumWeight_M40to50 += genWeight;

    }

  } // -- event iteration

  Double_t frac_M40to50 = sumWeight_M40to50 / sumWeight_tot;

  printf("[sum of weight] (total, 40 < M < 50 GeV) = (%.1lf, %.1lf)\n", sumWeight_M40to50, sumWeight_tot);
  printf("   ---> fraction = %lf\n", frac_M40to50);

  TFile *f_output = TFile::Open("ROOTFile_GenDimuonMass.root", "RECREATE");
  f_output->cd();
  h_diMuM->Write();
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