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

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

class EventInfoTool
{
public:
	TString TStr_Channel;

	Double_t Luminosity;
	Double_t PtCut_Lead;
	Double_t PtCut_Sub;
	Double_t EtaCut_Lead;
	Double_t EtaCut_Sub;

	TString SampleName;
	Int_t LeptonID;

	Int_t nEvent_Test;

	EventInfoTool( TString _TStr_Channel )
	{
		this->TStr_Channel = _TStr_Channel;

		if( this->TStr_Channel != "Muon" && this->TStr_Channel != "Electron" )
		{
			cout << this->TStr_Channel << "is Wrong channel!" << endl;
			return;
		}

		this->Setup_AccCut();
		this->Setup_Param();
		this->nEvent_Test = -1;
	}

	void Set_nEvent_Test( Double_t _nEv )
	{
		this->nEvent_Test = _nEv;
	}

	void Analyze()
	{
		DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );

		TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2(this->SampleName, &ntupleDirectory, &Tag, &Xsec, &nEvents);

		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			TStopwatch looptime;
			looptime.Start();

			cout << "\t<" << Tag[i_tup] << ">" << endl;

			if( Tag[i_tup] != "DYEE_M100to200" ) continue;

			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
			
			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_GenLepton();
			ntuple->TurnOnBranches_GenOthers();

			Double_t SumWeights = 0;
			Double_t SumWeights_Separated = 0;

			Int_t nTotEvent = chain->GetEntries();
			if( this->nEvent_Test > 0 ) 
			{
				cout << "# total event: " << nTotEvent << " -> " << this->nEvent_Test << " as a test" << endl;
				nTotEvent = this->nEvent_Test;
			}
			cout << "\t[Total Events: " << nTotEvent << "]" << endl;

			Double_t norm = ( Xsec[i_tup] * this->Luminosity ) / (Double_t)nEvents[i_tup];
			cout << "\t[Normalization factor: " << norm << "]" << endl;

			Int_t nEvent_Printed = 0;
			for(Int_t i=0; i<nTotEvent; i++)
			{
				// loadBar(i+1, nTotEvent, 100, 100);

				ntuple->GetEvent(i);

				// -- Bring weights for NLO MC events -- //
				Double_t GenWeight;
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
				SumWeights += GenWeight;

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
						if( fabs(genlep.ID) == this->LeptonID && genlep.fromHardProcessFinalState )
							GenLeptonCollection.push_back( genlep );
					}

					if( (Int_t)GenLeptonCollection.size() != 2 )
					{
						cout << "# total leptons with fromHardProcessFinalState flag = " << GenLeptonCollection.size() << ": not equal to 2 !!" << endl;
						return;
					}

					Double_t dRCut = 0.1;

					// -- dressing -- //
					GenLepton genlep_postFSR1 = GenLeptonCollection[0];
					GenLepton genlep_dressed1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
					vector< GenOthers > GenPhotonCollection1;
					analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_dressed1, &GenPhotonCollection1);

					GenLepton genlep_postFSR2 = GenLeptonCollection[1];
					GenLepton genlep_dressed2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
					vector< GenOthers > GenPhotonCollection2;
					analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_dressed2, &GenPhotonCollection2);

					Double_t Pt_Lead = 0;
					Double_t Pt_Sub = 0;
					if( genlep_dressed1.Pt > genlep_dressed2.Pt )
					{
						Pt_Lead = genlep_dressed1.Pt;
						Pt_Sub = genlep_dressed2.Pt;
					}
					else
					{
						Pt_Lead = genlep_dressed2.Pt;
						Pt_Sub = genlep_dressed1.Pt;
					}

					if( Pt_Sub > 86 && Pt_Sub < 93 )
					{
						nEvent_Printed++;
						if( genlep_postFSR1.Pt > genlep_postFSR2.Pt )
							this->Print_EventInfo( ntuple, genlep_postFSR1, genlep_postFSR2 );
						else
							this->Print_EventInfo( ntuple, genlep_postFSR2, genlep_postFSR1 );

						// if( nEvent_Printed == 1000 ) break;
					}

					// if( ntuple->evtNum == 43936 )
					// {
					// 	if( genlep_postFSR1.Pt > genlep_postFSR2.Pt )
					// 		this->Print_EventInfo( ntuple, genlep_postFSR1, genlep_postFSR2 );
					// 	else
					// 		this->Print_EventInfo( ntuple, genlep_postFSR2, genlep_postFSR1 );
					// 	break;
					// }
				} // -- GenFlag == kTRUE -- //

			} // -- end of event iteration -- //

			cout << "Total Sum of Weight: " << SumWeights << endl;
			cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;

		} // -- end of sample iteration -- //
	}

protected:
	void Print_EventInfo( NtupleHandle* ntuple, GenLepton genlep_postFSR1, GenLepton genlep_postFSR2 )
	{
		printf("=========================================================================================\n");
		printf("[run, lumi, event] = (%d, %d, %d)\n", ntuple->runNum, ntuple->lumiBlock, ntuple->evtNum );
		cout << endl;
		printf("[genlep_postFSR1] (Pt, eta, phi) = (%9.3lf, %9.3lf, %9.3lf)\n", genlep_postFSR1.Pt, genlep_postFSR1.eta, genlep_postFSR1.phi );
		printf("[genlep_postFSR2] (Pt, eta, phi) = (%9.3lf, %9.3lf, %9.3lf)\n", genlep_postFSR2.Pt, genlep_postFSR2.eta, genlep_postFSR2.phi );
		cout << endl;

		Double_t dRCut = 0.1;

		GenLepton genlep_dressed1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
		vector< GenOthers > GenPhotonCollection1;
		printf("##### Dressing post-FSR1 #####\n");
		this->PostToPreFSR_byDressedLepton_AllPhotons_Print(ntuple, &genlep_postFSR1, dRCut, &genlep_dressed1, &GenPhotonCollection1);

		GenLepton genlep_dressed2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
		vector< GenOthers > GenPhotonCollection2;
		printf("##### Dressing post-FSR2 #####\n");
		this->PostToPreFSR_byDressedLepton_AllPhotons_Print(ntuple, &genlep_postFSR2, dRCut, &genlep_dressed2, &GenPhotonCollection2);
		cout << endl;
		printf("[dressed lepton1] (Pt, eta, phi) = (%9.3lf, %9.3lf, %9.3lf)\n", genlep_dressed1.Pt, genlep_dressed1.eta, genlep_dressed1.phi );
		printf("[dressed lepton2] (Pt, eta, phi) = (%9.3lf, %9.3lf, %9.3lf)\n", genlep_dressed2.Pt, genlep_dressed2.eta, genlep_dressed2.phi );
		printf("=========================================================================================\n\n");
	}

	Bool_t Test_Acc( GenLepton genlep1, GenLepton genlep2 )
	{
		Bool_t Flag = kFALSE;

		Double_t Pt_Lead;
		Double_t Pt_Sub;
		Double_t Eta_Lead;
		Double_t Eta_Sub;

		if( genlep1.Pt > genlep2.Pt )
		{
			Pt_Lead = genlep1.Pt;
			Pt_Sub = genlep2.Pt;			
			Eta_Lead = genlep1.eta;
			Eta_Sub = genlep2.eta;
		}
		else
		{
			Pt_Lead = genlep2.Pt;
			Pt_Sub = genlep1.Pt;			
			Eta_Lead = genlep2.eta;
			Eta_Sub = genlep1.eta;
		}

		if( Pt_Lead > this->PtCut_Lead && Pt_Sub > this->PtCut_Sub &&
			fabs(Eta_Lead) < this->EtaCut_Lead && fabs(Eta_Sub) < this->EtaCut_Sub )
		{
			if( this->TStr_Channel == "Electron" )
			{
				if( !( fabs(genlep1.eta) > 1.4442 && fabs(genlep1.eta) < 1.566 )
				&& !( fabs(genlep2.eta) > 1.4442 && fabs(genlep2.eta) < 1.566 ) )
					Flag = kTRUE;
			}
			else
				Flag = kTRUE;
		}

		return Flag;
	}

	void Setup_AccCut()
	{
		if( this->TStr_Channel == "Muon" )
		{
			this->PtCut_Lead = 22;
			this->PtCut_Sub = 10;
			this->EtaCut_Lead = 2.4;
			this->EtaCut_Sub = 2.4;
		}
		else if( this->TStr_Channel == "Electron" )
		{
			this->PtCut_Lead = 30;
			this->PtCut_Sub = 10;
			this->EtaCut_Lead = 2.5;
			this->EtaCut_Sub = 2.5;
		}
	}

	void Setup_Param()
	{
		if( this->TStr_Channel == "Muon" )
		{
			this->SampleName = "aMCNLO_AdditionalSF_LargeN";
			this->LeptonID = 13;
			this->Luminosity = Lumi; // -- MuonPhys -- //
		}
		else if( this->TStr_Channel == "Electron" )
		{
			this->SampleName = "aMCNLO_ee_AdditionalSF_LargeN_M10to50";
			this->LeptonID = 11;
			this->Luminosity = 2257.998; // -- Golden JSON -- //
		}
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

	void PostToPreFSR_byDressedLepton_AllPhotons_Print(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection)
	{
		TLorentzVector genlep_Mom_postFSR = genlep_postFSR->Momentum;
		printf("[post-FSR] (Pt, eta, phi) = (%9.3lf, %9.3lf, %9.3lf)\n", genlep_Mom_postFSR.Pt(), genlep_Mom_postFSR.Eta(), genlep_Mom_postFSR.Phi() );

		TLorentzVector SumPhotonMom;
		SumPhotonMom.SetPxPyPzE(0,0,0,0);

		Int_t NGenOthers = ntuple->nGenOthers;
		for(Int_t i_gen=0; i_gen<NGenOthers; i_gen++)
		{
			GenOthers genlep;
			genlep.FillFromNtuple(ntuple, i_gen);

			// -- all photons within dR < 0.1 -- //
			// if( fabs(genlep.ID) == 22 && fabs(genlep.Mother) == 13)
			if( fabs(genlep.ID) == 22 )
			{
				Double_t dR = this->Calc_dR_GenLepton_GenOthers(*genlep_postFSR, genlep);
				printf("\t[Photon (dR=%5.3lf)] (Pt, eta, phi, Status, mother) = (%9.3lf, %9.3lf, %9.3lf, %d, %.0lf)", dR, genlep.Momentum.Pt(), genlep.Momentum.Eta(), genlep.Momentum.Phi(), genlep.Status, genlep.Mother);

				// -- Sum of all photon's momentum near the post-FSR muon -- //
				if( dR < dRCut )
				{
					SumPhotonMom  = SumPhotonMom + genlep.Momentum;
					GenPhotonCollection->push_back( genlep );
					printf("\t-> Sum of photon momentum: (Pt, eta, phi) = (%9.3lf, %9.3lf, %9.3lf)", SumPhotonMom.Pt(), SumPhotonMom.Eta(), SumPhotonMom.Phi() );
				}
				cout << endl;
			}
		}

		// -- Momentum(pre-FSR) = Momentum(post-FSR) + Sum of all Photon's momentum near the post-FSR muon -- //
		genlep_preFSR->Momentum = genlep_Mom_postFSR + SumPhotonMom;
		genlep_preFSR->Et = genlep_preFSR->Momentum.Et();
		genlep_preFSR->Pt = genlep_preFSR->Momentum.Pt();
		genlep_preFSR->eta = genlep_preFSR->Momentum.Eta();
		genlep_preFSR->phi = genlep_preFSR->Momentum.Phi();
		genlep_preFSR->Px = genlep_preFSR->Momentum.Px();
		genlep_preFSR->Py = genlep_preFSR->Momentum.Py();
		genlep_preFSR->Pz = genlep_preFSR->Momentum.Pz();

		printf("[dressed lepton] (Pt, eta, phi) = (%9.3lf, %9.3lf, %9.3lf)\n", genlep_preFSR->Momentum.Pt(), genlep_preFSR->Momentum.Eta(), genlep_preFSR->Momentum.Phi() );
		cout << endl;
	}

	Double_t Calc_dR_GenLepton_GenOthers( GenLepton genlep1, GenOthers genlep2 )
	{
		Double_t eta1 = genlep1.eta;
		Double_t phi1 = genlep1.phi;

		Double_t eta2 = genlep2.eta;
		Double_t phi2 = genlep2.phi;

		Double_t diff_eta = eta1 - eta2;
		Double_t diff_phi = phi1 - phi2;

		Double_t dR = sqrt( diff_eta * diff_eta + diff_phi * diff_phi );
		return dR;
	}
};

void Printout_EventInfo()
{
	EventInfoTool *tool = new EventInfoTool( "Electron" );
	// tool->Set_nEvent_Test( 100 );
	tool->Analyze();
}


