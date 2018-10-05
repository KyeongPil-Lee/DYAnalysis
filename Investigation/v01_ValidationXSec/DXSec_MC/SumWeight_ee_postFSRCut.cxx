#include <Include/DYAnalyzer.h>
#include <Include/SimplePlotTools.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>

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

Bool_t PassMassCut_postFSR(TString type, NtupleHandle* ntuple)
{
  Bool_t genFlag = kFALSE;

  if( type.Contains("DYEE") )
  {
    vector<GenLepton> vec_genLepton;
    Int_t nGenLepton = ntuple->gnpair;
    for(Int_t i_gen=0; i_gen<nGenLepton; i_gen++)
    {
      GenLepton genLepton;
      genLepton.FillFromNtuple(ntuple, i_gen);
      // if( genLepton.isElectron() && genLepton.isHardProcess )
      if( genLepton.isElectron() && genLepton.fromHardProcessFinalState )
        vec_genLepton.push_back( genLepton );
    }

    if( vec_genLepton.size() == 2 ) // -- Select the events containing 2 electrons from hard-process -- //
    {
      if( Tag == "DYEE_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
      {
        TLorentzVector v1 = vec_genLepton[0].Momentum;
        TLorentzVector v2 = vec_genLepton[1].Momentum;
        Double_t reco_M = (v1 + v2).M();
        if( reco_M < 200 )
          genFlag = kTRUE;
      }
      if( Tag.Contains("M50to100") ) // -- Select only evetns withtin 50 < M < 100 -- //
      {
        TLorentzVector v1 = vec_genLepton[0].Momentum;
        TLorentzVector v2 = vec_genLepton[1].Momentum;
        Double_t reco_M = (v1 + v2).M();
        if( reco_M < 100 )
          genFlag = kTRUE;
      }
      else
        genFlag = kTRUE;
    }
  }
  else
    genFlag = kTRUE;

  return genFlag;
}

void SumWeight_ee()
{
  TTimeStamp ts_start;
  cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

  TStopwatch totaltime;
  totaltime.Start();

  DYAnalyzer *analyzer = new DYAnalyzer( "None" );

  vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
  analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_ee_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);

  // -- add M10-50 high mass sample also
  ntupleDirectory.push_back("76X/DYntuple_v20170626_76X_MINIAODv2_DYLL_M10to50_25ns_LargeN");
  Tag.push_back("DYEE_M10to50_LargeN");
  Xsec.push_back( Xsec[0] ); // -- same with M10-50 sample
  nEvents.push_back( 0 ); // -- unknown yet


  const Int_t nSample = ntupleDirectory.size();
  for(Int_t i_sample = 0; i_sample<nSample; i_sample++)
  {
    TStopwatch looptime;
    looptime.Start();

    cout << "\t<" << Tag[i_sample] << ">" << endl;

    if( Tag[i_sample] != "DYEE_M50to100" ) continue;

    TChain *chain = new TChain("recoTree/DYTree");
    TString dataPath = gSystem->Getenv("KP_DATA_PATH");
    chain->Add(dataPath+"/"+ntupleDirectory[i_sample]+"/ntuple_*.root");

    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_GenLepton();

    Double_t sumWeight = 0;
    Double_t sumWeightRAW = 0;

    Double_t absWeight = 0;

    Int_t nTotEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nTotEvent << "]" << endl;

    for(Int_t i=0; i<nTotEvent; i++)
    {
      loadBar(i+1, nTotEvent, 100, 100);

      ntuple->GetEvent(i);

      Bool_t genFlag = kFALSE;
      // genFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_sample], ntuple);
      genFlag = PassMassCut_postFSR(Tag[i_sample], ntuple);

      if( genFlag ) // -- only DY->ee events
      {
        if( absWeight == 0 )
        {
          absWeight = fabs(ntuple->GENEvt_weight);
          printf("   Absolute weight for a event: %lf\n", absWeight);
        }
        else
        {
          if( absWeight != fabs(ntuple->GENEvt_weight))
          {
            printf("   GenWeight for this event is different with the nominal one!!!\n");
            Double_t absWeightThis = fabs(ntuple->GENEvt_weight);
            Double_t relDiff = (absWeightThis - absWeight) / absWeight;
            printf("      absolute weight of this event: %lf\n -> rel. diff. with nominal one: %lf\n", absWeightThis, relDiff);
          }
        }

        // -- including absolute weight value
        sumWeightRAW += ntuple->GENEvt_weight;

        // -- normalized to 1
        Double_t genWeight = 0;
        ntuple->GENEvt_weight < 0 ? genWeight = -1 : genWeight = 1;
        sumWeight += genWeight;
      } // -- end of genFlag

    } // -- end of event iteration

    printf("[Sum of weight]\n");
    printf("   RAW value: %lf\n", sumWeightRAW);
    printf("   Value when ach weights are normalized to 1: %lf\n", sumWeight);
    printf("\n");

  } // -- end of sample iteration

  Double_t TotalRunTime = totaltime.CpuTime();
  cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

  TTimeStamp ts_end;
  cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
}