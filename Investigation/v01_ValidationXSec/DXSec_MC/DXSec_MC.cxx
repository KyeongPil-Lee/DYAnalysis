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

class HistProducer
{
public:
  TString sampleType_;
  Int_t leptonID_;
  TString outputName_;

  HistProducer()
  {

  }

  void Produce()
  {
    TTimeStamp ts_start;
    cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

    TStopwatch totaltime;
    totaltime.Start();

    Set_AcceptanceCut();

    DYAnalyzer *analyzer = new DYAnalyzer( "None" );

    vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
    analyzer->SetupMCsamples_v20160309_76X_MiniAODv2(sampleType_, &ntupleDirectory, &Tag, &Xsec, &nEvents);

    // -- output file
    TFile *f_output = TFile::Open(outputName_, "RECREATE");
    vector< TH1D* > vec_hist; // -- all mass histograms

    const Int_t nSample = ntupleDirectory.size();
    for(Int_t i_sample = 0; i_sample<nSample; i_sample++)
    {
      TStopwatch looptime;
      looptime.Start();

      cout << "\t<" << Tag[i_sample] << ">" << endl;

      TChain *chain = new TChain("recoTree/DYTree");
      TString dataPath = gSystem->Getenv("KP_DATA_PATH");
      chain->Add(dataPath+"/"+ntupleDirectory[i_sample]+"/ntuple_*.root");
      // chain->Add("/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/Rivet/CrossCheck/Local/ntuple_skim_377.root");

      NtupleHandle *ntuple = new NtupleHandle( chain );
      // ntuple->TurnOnBranches_GenOthers();
      ntuple->TurnOnBranches_GenLepton();

      double_t sumWeight = 0;

      TH1D* h_mass = MakeHistTemplate(Tag[i_sample]);

      Int_t nTotEvent = chain->GetEntries();
      cout << "\t[Total Events: " << nTotEvent << "]" << endl;

      for(Int_t i=0; i<nTotEvent; i++)
      {
        loadBar(i+1, nTotEvent, 100, 100);

        ntuple->GetEvent(i);

        Bool_t genFlag = kFALSE;
        genFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_sample], ntuple);

        if( genFlag == kTRUE ) // -- single flavor events
        {
          Double_t genWeight = 0;
          ntuple->GENEvt_weight < 0 ? genWeight = -1 : genWeight = 1;
          sumWeight += genWeight;

          vector<GenLepton> vec_genLepton;
          Int_t nGenLepton = ntuple->gnpair; 
          for(Int_t i_gen=0; i_gen<nGenLepton; i_gen++)
          {
            GenLepton genLepton;
            genLepton.FillFromNtuple(ntuple, i_gen);
            if( fabs(genLepton.ID) == leptonID_ && genLepton.fromHardProcessFinalState )
              vec_genLepton.push_back( genLepton );
          }

          if( int(vec_genLepton.size()) != 2 )
            printf("No 2 gen leptons with fromHardProcessFinalState!!\n");

          Bool_t isAccepted = IsAccepted( vec_genLepton[0], vec_genLepton[1] );
          if( isAccepted )
          {
            Double_t mass = (vec_genLepton[0].Momentum + vec_genLepton[1].Momentum).M();
            h_mass->Fill( mass, genWeight );
          }
        } // -- end of gen flag check
      } // -- end of event iteration

      TH1D* h_mass_unNorm = (TH1D*)h_mass->Clone();
      h_mass_unNorm->SetName("h_mass_unNorm_"+Tag[i_sample]);

      // -- normalization
      h_mass->Scale( Xsec[i_sample] / sumWeight );
      vec_hist.push_back( h_mass );

      TString histName_DXSec = h_mass->GetName();
      histName_DXSec.ReplaceAll( "h_mass", "h_DXSec" );
      TH1D* h_DXSec = (TH1D*)h_mass->Clone();
      h_DXSec->SetName(histName_DXSec);
      h_DXSec = PlotTool::DivideEachBin_ByBinWidth( h_DXSec );

      f_output->cd();
      h_mass_unNorm->Write();
      h_mass->Write();
      h_DXSec->Write();

      printf("   Total sum of weight: %.0lf\n", sumWeight);

      Double_t LoopRunTime = looptime.CpuTime();
      cout << "\tLoop RunTime(" << Tag[i_sample] << "): " << LoopRunTime << " seconds\n" << endl;

    } // -- end of sample iteration

    TH1D* h_mass_merged = NULL;
    for(const auto& h : vec_hist )
    {
      if( h_mass_merged == NULL )
        h_mass_merged = (TH1D*)h->Clone("h_mass_merged");
      else
        h_mass_merged->Add( h );
    }

    TString histName_DXSec = h_mass_merged->GetName();
    histName_DXSec.ReplaceAll( "h_mass", "h_DXSec" );
    TH1D* h_DXSec_merged = (TH1D*)h_mass_merged->Clone();
    h_DXSec_merged->SetName(histName_DXSec);
    h_DXSec_merged = PlotTool::DivideEachBin_ByBinWidth( h_DXSec_merged );

    f_output->cd();
    h_mass_merged->Write();
    h_DXSec_merged->Write();
    f_output->Close();

    Double_t TotalRunTime = totaltime.CpuTime();
    cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

    TTimeStamp ts_end;
    cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;

  }

private:
  Double_t ptCut_lead_;
  Double_t ptCut_sub_;
  Double_t etaCut_lead_;
  Double_t etaCut_sub_;

  void Set_AcceptanceCut()
  {
    if( leptonID_ == 13 )
    {
      ptCut_lead_ = 22;
      ptCut_sub_ = 10;
      etaCut_lead_ = 2.4;
      etaCut_sub_ = 2.4;
    }
    else if(leptonID_ == 11 )
    {
      ptCut_lead_ = 30;
      ptCut_sub_ = 10;
      etaCut_lead_ = 2.5;
      etaCut_sub_ = 2.5;
    }
    else
    {
      printf("[Set_AcceptanceCut] LeptonID = %d: not available\n", leptonID_);
      ptCut_lead_ = 0;
      ptCut_sub_ = 0;
      etaCut_lead_ = 0;
      etaCut_sub_ = 0;
    }
  }

  Bool_t ECALGapSafe(Double_t eta)
  {
    Bool_t isSafe = kFALSE;
    if( !( fabs(eta) > 1.4442 && fabs(eta) < 1.566 ) ) isSafe = kTRUE;
    return isSafe;
  }

  Bool_t IsAccepted( GenLepton& lepton1, GenLepton& lepton2 )
  {
    Bool_t isAccepted = kFALSE;

    Double_t ptLead = 0;
    Double_t ptSub = 0;
    Double_t etaLead = 0;
    Double_t etaSub = 0;

    if( lepton1.Pt > lepton2.Pt )
    {
      ptLead = lepton1.Pt;
      etaLead = lepton1.eta;

      ptSub = lepton2.Pt;
      etaSub = lepton2.eta;
    }
    else
    {
      ptLead = lepton2.Pt;
      etaLead = lepton2.eta;

      ptSub = lepton1.Pt;
      etaSub = lepton1.eta;
    }

    if( ptLead > ptCut_lead_ &&
        ptSub  > ptCut_sub_ &&
        fabs(etaLead) < etaCut_lead_ &&
        fabs(etaSub) < etaCut_sub_ )
    {
      if( leptonID_ == 11 )
      {
        if( ECALGapSafe(etaLead) && ECALGapSafe(etaSub) )
          isAccepted = kTRUE;          
      }
      else
        isAccepted = kTRUE;
    }

    return isAccepted;
  }

  TH1D* MakeHistTemplate(TString type)
  {
    Double_t arr_massBinEdge[nMassBin+1] = 
    {
      15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
      64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
      110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
      200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
      830, 1000, 1500, 3000
    };

    TH1D* h_temp = new TH1D("h_mass_"+type, "", nMassBin, arr_massBinEdge);
    return h_temp;
  }

};

void DXSec_MC()
{
  HistProducer producer;
  producer.sampleType_ = "aMCNLO_ee_AdditionalSF";
  producer.leptonID_ = 11;
  producer.outputName_ = "ROOTFile_DXSec_MC_aMCNLO_ee_AdditionalSF.root";
  producer.Produce();
}