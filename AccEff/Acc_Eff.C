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

#include <vector>

// -- for Rochester Muon momentum correction -- //
#include <Include/RochesterMomCorr_76X/RoccoR.cc>
#include <Include/RochesterMomCorr_76X/rochcor2015.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

static inline void loadBar(int x, int n, int r, int w);
void Acc_Eff(Bool_t isCorrected = kTRUE, TString Sample = "aMCNLO", TString HLTname = "IsoMu20_OR_IsoTkMu20" )
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Sample: " << Sample << endl;

	TString isApplyMomCorr = "";
	if( isCorrected == kTRUE )
	{
		cout << "Apply Roochester Muon Momentum Correction..." << endl;
		isApplyMomCorr = "MomCorr";
	}
	else
	{
		cout << "DO *NOT* Apply Roochester Muon Momentum Correction..." << endl;
		isApplyMomCorr = "MomUnCorr";
	}

	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( HLTname );
	analyzer->SetupEfficiencyScaleFactor( "ROOTFile_TagProbeEfficiency_76X_v20160502.root" );

	TFile *f = new TFile("ROOTFile_Histogram_Acc_Eff_" + Sample + "_" + HLTname + ".root", "RECREATE");

	// const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

 	TH1D *h_mass_AccTotal = new TH1D("h_mass_AccTotal", "", nMassBin, MassBinEdges);
	TH1D *h_mass_AccPass = new TH1D("h_mass_AccPass", "", nMassBin, MassBinEdges);
	TH1D *h_mass_EffTotal = new TH1D("h_mass_EffTotal", "", nMassBin, MassBinEdges);
	TH1D *h_mass_EffPass = new TH1D("h_mass_EffPass", "", nMassBin, MassBinEdges);	 

	// -- After applying efficiency correction -- //
	TH1D *h_mass_EffPass_Corr_HLTv4p2 = new TH1D("h_mass_EffPass_Corr_HLTv4p2", "", nMassBin, MassBinEdges);
	TH1D *h_mass_EffPass_Corr_HLTv4p3 = new TH1D("h_mass_EffPass_Corr_HLTv4p3", "", nMassBin, MassBinEdges);

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

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;
		TH1D* h_mass = new TH1D("h_mass_"+Tag[i_tup], "", 800, 0, 8000);

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_GenLepton();

		rochcor2015 *rmcor = new rochcor2015();

		Bool_t isMC;
		Tag[i_tup] == "Data" ? isMC = kFALSE : isMC = kTRUE;
		analyzer->SetupPileUpReWeighting_76X( isMC );

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Double_t SumWeights = 0;
		Double_t SumWeights_Separated = 0;

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl; 

		Double_t norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
		cout << "\t[Normalization factor: " << norm << "]" << endl;

		// NEvents = 1000;
		// -- Event loop starts -- //
		for(Int_t i=0; i<NEvents; i++)
		{
			loadBar(i+1, NEvents, 100, 100);

			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			if( isNLO == 1 )
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
			else
				GenWeight = 1;

			SumWeights += GenWeight;

			Int_t PU = ntuple->nPileUp;
			Double_t PUWeight = analyzer->PileUpWeightValue_76X( PU );

			Double_t TotWeight = norm * GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == kTRUE )
			{
				SumWeights_Separated += GenWeight;

				// -- Collect gen-level information -- //
				vector<GenLepton> GenLeptonCollection;
				Int_t NGenLeptons = ntuple->gnpair; 
				for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
				{
					GenLepton genlep;
					genlep.FillFromNtuple(ntuple, i_gen);
					if( genlep.isMuon() && genlep.fromHardProcessFinalState )
						GenLeptonCollection.push_back( genlep );
				}
				GenLepton genlep1 = GenLeptonCollection[0];
				GenLepton genlep2 = GenLeptonCollection[1];
				Double_t gen_M = (genlep1.Momentum + genlep2.Momentum).M();

				// -- Flags -- //
				Bool_t Flag_PassAcc = kFALSE;
				Bool_t Flag_PassAccEff = kFALSE;

				// -- Fill the mass histograms -- //
				h_mass->Fill( gen_M, TotWeight );
				h_mass_tot->Fill( gen_M, TotWeight );

				Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

				// -- Acceptance Calculation -- //
				if( Flag_PassAcc == kTRUE ) 
				{
					h_mass_AccTotal->Fill( gen_M, TotWeight );
					h_mass_AccPass->Fill( gen_M, TotWeight );
				}
				else
				{
					h_mass_AccTotal->Fill( gen_M, TotWeight );
				} 	

				Double_t Eff_SF_HLTv4p2 = -999; // -- Efficiency correction factor -- //
				Double_t Eff_SF_HLTv4p3 = -999; // -- Efficiency correction factor -- //

				// -- Calculate the efficiency among the events passing acceptance condition -- //
				if( Flag_PassAcc == kTRUE )
				{
					Bool_t Flag_PassEff = kFALSE;

					if( ntuple->isTriggered( analyzer->HLT ) )
					{
						// -- Collect Reconstruction level information -- //
						vector< Muon > MuonCollection;
						Int_t NLeptons = ntuple->nMuon;
						for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
						{
							Muon mu;
							mu.FillFromNtuple(ntuple, i_reco);
							// -- Apply Rochester momentum scale correction -- //
							if( isCorrected == kTRUE )
							{
								float qter = 1.0;
								
								if( Tag[i_tup] == "Data" )
									rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
								else
									rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

								// -- Change Muon pT, eta and phi with updated(corrected) one -- //
								mu.Pt = mu.Momentum.Pt();
								mu.eta = mu.Momentum.Eta();
								mu.phi = mu.Momentum.Phi();
							}
							
							MuonCollection.push_back( mu );
						}

						// -- Event Selection -- //
						vector< Muon > SelectedMuonCollection;
						Bool_t isPassEventSelection = kFALSE;
						isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

						if( isPassEventSelection == kTRUE )
						{
							Eff_SF_HLTv4p2 = analyzer->EfficiencySF_EventWeight_HLTv4p2( SelectedMuonCollection[0], SelectedMuonCollection[1] );
							Eff_SF_HLTv4p3 = analyzer->EfficiencySF_EventWeight_HLTv4p3( SelectedMuonCollection[0], SelectedMuonCollection[1] );

							// printf("( SF_HLTv4p2, SF_HLTv4p3 ) = (%.5lf, %.5lf)\n", Eff_SF_HLTv4p2, Eff_SF_HLTv4p3);

							Flag_PassEff = kTRUE;
							Flag_PassAccEff = kTRUE;
						}

					} // -- End of if( ntuple->isTriggered( HLT ) ) -- //

					// -- Efficiency Calculation -- //
					if( Flag_PassEff == kTRUE)
					{
						h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );
						h_mass_EffPass->Fill( gen_M, TotWeight * PUWeight );
						h_mass_EffPass_Corr_HLTv4p2->Fill( gen_M, TotWeight  * PUWeight * Eff_SF_HLTv4p2 );
						h_mass_EffPass_Corr_HLTv4p3->Fill( gen_M, TotWeight  * PUWeight * Eff_SF_HLTv4p3 );
					}
					else
						h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );

				} // -- End of if( Flag_PassAcc )


				// // -- Acceptance * Efficiency Calculation -- //
				// if( Flag_PassAccEff == kTRUE)
				// {
				// 	AccEff_Mass->FillWeighted(1, TotWeight, gen_M);
				// 	h_mass_AccEffPass_Corr->Fill( gen_M, TotWeight * Eff_SF );
				// }
				// else 
				// 	AccEff_Mass->FillWeighted(0, TotWeight, gen_M);

			} // -- End of if( GenFlag == kTRUE )

		} //End of event iteration

		f->cd();
		h_mass->Write();

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();
	h_mass_tot->Write();

	h_mass_AccTotal->Write();
	h_mass_AccPass->Write();
	h_mass_EffTotal->Write();
	h_mass_EffPass->Write();
	h_mass_EffPass_Corr_HLTv4p2->Write();
	h_mass_EffPass_Corr_HLTv4p3->Write();

	TEfficiency *Acc_Mass = new TEfficiency(*h_mass_AccPass, *h_mass_AccTotal);
	Acc_Mass->SetName("TEff_Acc_Mass");

	TEfficiency *Eff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_EffTotal);
	Eff_Mass->SetName("TEff_Eff_Mass");

	// TEfficiency *AccEff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_AccTotal);
	// AccEff_Mass->SetName("TEff_AccEff_Mass");

	TEfficiency *Eff_Mass_Corr_HLTv4p2 = new TEfficiency(*h_mass_EffPass_Corr_HLTv4p2, *h_mass_EffTotal);
	Eff_Mass_Corr_HLTv4p2->SetName("TEff_Eff_Mass_Corr_HLTv4p2");

	// TEfficiency *AccEff_Mass_Corr_HLTv4p2 = new TEfficiency(*h_mass_EffPass_Corr_HLTv4p2, *h_mass_AccTotal);
	// AccEff_Mass_Corr_HLTv4p2->SetName("TEff_AccEff_Mass_Corr_HLTv4p2");

	TEfficiency *Eff_Mass_Corr_HLTv4p3 = new TEfficiency(*h_mass_EffPass_Corr_HLTv4p3, *h_mass_EffTotal);
	Eff_Mass_Corr_HLTv4p3->SetName("TEff_Eff_Mass_Corr_HLTv4p3");

	// TEfficiency *AccEff_Mass_Corr_HLTv4p3 = new TEfficiency(*h_mass_EffPass_Corr_HLTv4p3, *h_mass_AccTotal);
	// AccEff_Mass_Corr_HLTv4p3->SetName("TEff_AccEff_Mass_Corr_HLTv4p3");

	Acc_Mass->Write();
	Eff_Mass->Write();
	// AccEff_Mass->Write();
	Eff_Mass_Corr_HLTv4p2->Write();
	// AccEff_Mass_Corr_HLTv4p2->Write();
	Eff_Mass_Corr_HLTv4p3->Write();
	// AccEff_Mass_Corr_HLTv4p3->Write();

	TCanvas *c_Acc_Mass = new TCanvas("c_Acc_Mass", "", 800, 600);
	c_Acc_Mass->cd();
	Acc_Mass->Draw("AP");
	c_Acc_Mass->Write();

	TCanvas *c_Eff_Mass = new TCanvas("c_Eff_Mass", "", 800, 600);
	c_Eff_Mass->cd();
	Eff_Mass->Draw("AP");
	c_Eff_Mass->Write();

	// TCanvas *c_AccEff_Mass = new TCanvas("c_AccEff_Mass", "", 800, 600);
	// c_AccEff_Mass->cd();
	// AccEff_Mass->Draw("AP");
	// c_AccEff_Mass->Write();

	TCanvas *c_Eff_Mass_Corr_HLTv4p2 = new TCanvas("c_Eff_Mass_Corr_HLTv4p2", "", 800, 600);
	c_Eff_Mass_Corr_HLTv4p2->cd();
	Eff_Mass_Corr_HLTv4p2->Draw("AP");
	c_Eff_Mass_Corr_HLTv4p2->Write();

	// TCanvas *c_AccEff_Mass_Corr_HLTv4p2 = new TCanvas("c_AccEff_Mass_Corr_HLTv4p2", "", 800, 600);
	// c_AccEff_Mass_Corr_HLTv4p2->cd();
	// AccEff_Mass_Corr_HLTv4p2->Draw("AP");
	// c_AccEff_Mass_Corr_HLTv4p2->Write();

	TCanvas *c_Eff_Mass_Corr_HLTv4p3 = new TCanvas("c_Eff_Mass_Corr_HLTv4p3", "", 800, 600);
	c_Eff_Mass_Corr_HLTv4p3->cd();
	Eff_Mass_Corr_HLTv4p3->Draw("AP");
	c_Eff_Mass_Corr_HLTv4p3->Write();

	// TCanvas *c_AccEff_Mass_Corr_HLTv4p3 = new TCanvas("c_AccEff_Mass_Corr_HLTv4p3", "", 800, 600);
	// c_AccEff_Mass_Corr_HLTv4p3->cd();
	// AccEff_Mass_Corr_HLTv4p3->Draw("AP");
	// c_AccEff_Mass_Corr_HLTv4p3->Write();

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


