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
#include <stdio.h>

#include <vector>

//Customized header files
#include <Include/DYAnalyzer.h>

static inline void loadBar(int x, int n, int r, int w);
void ProdHist_MassWithinAcc( TString Sample = "aMCNLO", TString HLTname = "IsoMu20_OR_IsoTkMu20" )
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000}; // -- high mass bins are merged -- //

	TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");

	// -- Each ntuple directory & corresponding Tags -- //
	// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;

	if( Sample == "aMCNLO" )
	{
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);
	}
	if( Sample == "Powheg" )
	{
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
	}

	TFile *f = new TFile("ROOTFile_Histograms_Mass_WithinAcc_" + Sample + ".root", "RECREATE");
	TH1D *h_mass_preFSR_tot = new TH1D("h_mass_preFSR_tot", "", nMassBin, MassBinEdges);
	TH1D *h_mass_postFSR_tot = new TH1D("h_mass_postFSR_tot", "", nMassBin, MassBinEdges);
	TH1D *h_mass_preFSR_WithinAcc_tot = new TH1D("h_mass_preFSR_WithinAcc_tot", "", nMassBin, MassBinEdges);
	TH1D *h_mass_postFSR_WithinAcc_tot = new TH1D("h_mass_postFSR_WithinAcc_tot", "", nMassBin, MassBinEdges);

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		TH1D *h_mass_preFSR = new TH1D("h_mass_preFSR_"+Tag[i_tup], "", nMassBin, MassBinEdges);
		TH1D *h_mass_postFSR = new TH1D("h_mass_postFSR_"+Tag[i_tup], "", nMassBin, MassBinEdges);
		TH1D *h_mass_preFSR_WithinAcc = new TH1D("h_mass_preFSR_WithinAcc_"+Tag[i_tup], "", nMassBin, MassBinEdges);
		TH1D *h_mass_postFSR_WithinAcc = new TH1D("h_mass_postFSR_WithinAcc_"+Tag[i_tup], "", nMassBin, MassBinEdges);

		// -- ntuple Setting -- //
		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_GenOthers();

		// analyzer->SetupPileUpReWeighting( Tag[i_tup] );

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Double_t SumWeights = 0;
		Double_t SumWeights_Separated = 0;
		Int_t SumEvents_Separated = 0;

		Int_t nTotEvent = chain->GetEntries();

		Double_t norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
		cout << "\t[Normalization factor: " << norm << "]" << endl;

		// -- Event loop starts -- //
		for(Int_t i=0; i<nTotEvent; i++)
		{
			// printf("[%dth Event]\n", i);

			loadBar(i+1, nTotEvent, 100, 100);

			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			if( isNLO == 1 )
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
			else
				GenWeight = 1;

			SumWeights += GenWeight;

			Int_t PU = ntuple->nPileUp;
			// Double_t PUWeight = analyzer->PileUpWeightValue( PU );

			// Double_t TotWeight = norm * GenWeight * PUWeight;
			Double_t TotWeight = norm * GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == kTRUE )
			{
				SumWeights_Separated += GenWeight;
				SumEvents_Separated++;

				vector<GenLepton> GenLeptonCollection_FinalState;
				Int_t NGenLeptons = ntuple->gnpair;
				for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
				{
					GenLepton genlep;
					genlep.FillFromNtuple(ntuple, i_gen);
					if( genlep.isMuon() && genlep.fromHardProcessFinalState )
					{
						GenLeptonCollection_FinalState.push_back( genlep );

						if( (Int_t)GenLeptonCollection_FinalState.size() == 2 )
							break;
					}
				}


				/////////////////////////////////////////////////////
				// -- Obtain pre-FSR momentum by dressed lepton -- //
				/////////////////////////////////////////////////////
				Double_t dRCut = 0.1;

				GenLepton genlep_postFSR1 = GenLeptonCollection_FinalState[0];
				GenLepton genlep_preFSR1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
				vector< GenOthers > GenPhotonCollection1;
				analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_preFSR1, &GenPhotonCollection1);

				GenLepton genlep_postFSR2 = GenLeptonCollection_FinalState[1];
				GenLepton genlep_preFSR2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
				vector< GenOthers > GenPhotonCollection2;
				analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_preFSR2, &GenPhotonCollection2);

				// -- Mass Calculation -- //
				Double_t M_preFSR = ( genlep_preFSR1.Momentum + genlep_preFSR2.Momentum ).M();
				Double_t M_postFSR = ( genlep_postFSR1.Momentum + genlep_postFSR2.Momentum ).M();

				h_mass_preFSR->Fill( M_preFSR, TotWeight );
				h_mass_postFSR->Fill( M_postFSR, TotWeight );

				h_mass_preFSR_tot->Fill( M_preFSR, TotWeight );
				h_mass_postFSR_tot->Fill( M_postFSR, TotWeight );

				Bool_t isPassAcc_preFSR = analyzer->isPassAccCondition_GenLepton(genlep_preFSR1, genlep_preFSR2);
				Bool_t isPassAcc_postFSR = analyzer->isPassAccCondition_GenLepton(genlep_postFSR1, genlep_postFSR2);

				if( isPassAcc_preFSR )
				{
					h_mass_preFSR_WithinAcc->Fill( M_preFSR, TotWeight );
					h_mass_preFSR_WithinAcc_tot->Fill( M_preFSR, TotWeight );
				}

				if( isPassAcc_postFSR )
				{
					h_mass_postFSR_WithinAcc->Fill( M_postFSR, TotWeight );
					h_mass_postFSR_WithinAcc_tot->Fill( M_postFSR, TotWeight );
				}

			} // -- End of if( GenFlag == kTRUE ) -- //

		} //End of event iteration

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;
		cout << "\tSum of Events of Separated Events: " << SumEvents_Separated << endl;

		f->cd();
		h_mass_preFSR->Write();
		h_mass_postFSR->Write();
		h_mass_preFSR_WithinAcc->Write();
		h_mass_postFSR_WithinAcc->Write();

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();
	h_mass_preFSR_tot->Write();
	h_mass_postFSR_tot->Write();
	h_mass_preFSR_WithinAcc_tot->Write();
	h_mass_postFSR_WithinAcc_tot->Write();

	Double_t TotalRunTime = totaltime.CpuTime();
	cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

	TTimeStamp ts_end;
	cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
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


