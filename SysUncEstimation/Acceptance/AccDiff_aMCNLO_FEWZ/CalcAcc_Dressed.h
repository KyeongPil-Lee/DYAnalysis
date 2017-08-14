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


class AccTool
{
public:
	TString TStr_Channel;

	Double_t PtCut_Lead;
	Double_t PtCut_Sub;
	Double_t EtaCut_Lead;
	Double_t EtaCut_Sub;

	TString SampleName;
	Int_t LeptonID;

	Double_t MassBinEdges[nMassBin+1];

	TH1D* h_AccTotal;
	TH1D* h_AccPass;
	TEfficiency *TEff_Acc;
	TGraphAsymmErrors* g_Acc;

	Int_t nEvent_Test;

	AccTool( TString _TStr_Channel )
	{
		this->TStr_Channel = _TStr_Channel;

		if( this->TStr_Channel != "Muon" && this->TStr_Channel != "Electron" )
		{
			cout << this->TStr_Channel << "is Wrong channel!" << endl;
			return;
		}

		this->Setup_AccCut();
		this->Setup_Param();

		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		for( Int_t i=0; i<nMassBin+1; i++)
			this->MassBinEdges[i] = MassBinEdges_temp[i];

		this->h_AccTotal = new TH1D("h_AccTotal", "", nMassBin, MassBinEdges);
		this->h_AccPass = new TH1D("h_AccPass", "", nMassBin, MassBinEdges);

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

			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
			
			NtupleHandle *ntuple = new NtupleHandle( chain );
			// ntuple->TurnOnBranches_HLT();
			// ntuple->TurnOnBranches_Muon();
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

			Double_t norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
			cout << "\t[Normalization factor: " << norm << "]" << endl;

			for(Int_t i=0; i<nTotEvent; i++)
			{
				loadBar(i+1, nTotEvent, 100, 100);

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

					Double_t M_GEN = (genlep_dressed1.Momentum + genlep_dressed2.Momentum).M();

					Bool_t Flag_Acc = this->Test_Acc( genlep_dressed1, genlep_dressed2 );

					if( Flag_Acc )
					{
						h_AccTotal->Fill( M_GEN, TotWeight );
						h_AccPass->Fill( M_GEN, TotWeight );
					}
					else
					{
						h_AccTotal->Fill( M_GEN, TotWeight );
					}
				} // -- GenFlag == kTRUE -- //

			} // -- end of event iteration -- //

			cout << "Total Sum of Weight: " << SumWeights << endl;
			cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;

		} // -- end of sample iteration -- //

		this->TEff_Acc = new TEfficiency(*h_AccPass, *h_AccTotal);
		this->g_Acc = (TGraphAsymmErrors*)TEff_Acc->CreateGraph()->Clone();
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_AccTotal->SetName("h_AccTotal");
		this->h_AccTotal->Write();

		this->h_AccPass->SetName("h_AccPass");
		this->h_AccPass->Write();

		this->TEff_Acc->SetName("TEff_Acc");
		this->TEff_Acc->Write();

		this->g_Acc->SetName("g_Acc");
		this->g_Acc->Write();
	}

protected:
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
			this->SampleName = "aMCNLO_AdditionalSF";
			this->LeptonID = 13;
		}
		else if( this->TStr_Channel == "Electron" )
		{
			this->SampleName = "aMCNLO_ee_AdditionalSF";
			this->LeptonID = 11;
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
};



