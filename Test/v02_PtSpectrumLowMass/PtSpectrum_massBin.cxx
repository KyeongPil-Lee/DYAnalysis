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


class PtSpectrumTool
{
public:
	TH1D* h_leadPt_M15to20;
	TH1D* h_subPt_M15to20;

	TH1D* h_leadPt_M25to30;
	TH1D* h_subPt_M25to30;

	void PtSpectrumTool()
	{
		this->Init();
	}

	void RunEventLoop()
	{
		DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );
		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);

		const Int_t nSample = ntupleDirectory.size();
		for(Int_t i_sample = 0; i_sample<nSample; i_sample++)
		{
			cout << "\t<" << Tag[i_sample] << ">" << endl;

			if( !Tag[i_sample].Contains("M10to50") ) continue;

			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_sample]+"/ntuple_*.root");

			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_HLT();
			ntuple->TurnOnBranches_GenLepton();
			ntuple->TurnOnBranches_Muon();

			rochcor2015 *rmcor = new rochcor2015();
			analyzer->SetupPileUpReWeighting_76X( Tag[i_sample] );

			Int_t nEvent = chain->GetEntries();
			cout << "\t[Total Events: " << nEvent << "]" << endl;

			Double_t NormFactor = (Lumi * Xsec[i_sample]) / nEvents[i_sample];
			cout << "\t[Normalization factor: " << NormFactor << "]" << endl;

			for(Int_t i=0; i<nEvent; i++)
			{
				loadBar(i+1, NEvents, 100, 100);
				
				ntuple->GetEvent(i);

				Double_t GenWeight;
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

				Int_t PU = ntuple->nPileUp;
				Double_t PUWeight = analyzer->PileUpWeightValue_76X( PU );

				Double_t TotWeight = NormFactor * GenWeight * PUWeight;

				Bool_t GenFlag = kFALSE;
				GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_sample], ntuple);

				if( GenFlag == kTRUE )
				{
					/////////////////////////////////////
					// -- Generator level selection -- //
					/////////////////////////////////////
					vector< GenLepton > vec_GenLepton;

					Int_t nGenLepton = ntuple->gnpair;
					for(Int_t i_gen=0; i_gen<nGenLepton; i_gen++)
					{
						GenLepton genLepton;
						genLepton.FillFromNtuple(ntuple, i_gen);
						if( genLepton.isMuon() && genLepton.fromHardProcessFinalState )
							vec_GenLepton.push_back( genLepton );
					}

					GenLepton genLepton1 = vec_GenLepton[0];
					GenLepton genLepton2 = vec_GenLepton[1];
					Double_t gen_M = (genLepton1.Momentum + genLepton2.Momentum).M();

					Bool_t flag_passAcc = kFALSE;
					flag_passAcc = analyzer->isPassAccCondition_GenLepton(genLepton1, genLepton2);

					if( flag_passAcc )
					{
						///////////////////////////////////////////
						// -- Reconstruction level selection -- //
						///////////////////////////////////////////
						Bool_t isPassEventSelection = kFALSE;

						if( ntuple->isTriggered( analyzer->HLT ) )
						{
							// -- Collect Reconstruction level information -- //
							vector< Muon > vec_muon;
							Int_t nMuon = ntuple->nMuon;
							for(Int_t i_reco=0; i_reco<nMuon; i_reco++)
							{
								Muon mu;
								mu.FillFromNtuple(ntuple, i_reco);
								// -- Apply Rochester momentum scale correction -- //
								float qter = 1.0;
								
								if( Tag[i_sample] == "Data" )
									rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
								else
									rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

								// -- Change Muon pT, eta and phi with updated(corrected) one -- //
								mu.Pt = mu.Momentum.Pt();
								mu.eta = mu.Momentum.Eta();
								mu.phi = mu.Momentum.Phi();

								vec_muon.push_back( mu );
							}

							// -- Event Selection -- //
							vector< Muon > vec_selectedMuon;
							flag_selected = analyzer->EventSelection(vec_muon, ntuple, &vec_selectedMuon);

							if( flag_selected == kTRUE )
							{
								Muon Mu1 = vec_selectedMuon[0];
								Muon Mu2 = vec_selectedMuon[1];
								Double_t reco_M = ( Mu1.Momentum + Mu2.Momentum ).M();

								Double_t leadPt = 0;
								Double_t subPt = 0;
								if( Mu1.Pt > Mu2.Pt )
								{
									leadPt = Mu1.Pt;
									subPt = Mu2.Pt;
								}
								else
								{
									leadPt = Mu2.Pt;
									subPt = Mu1.Pt;
								}

								if( gen_M > 15 && gen_M < 20 )
								{
									this->h_leadPt_M15to20->Fill( leadPt, TotWeight );
									this->h_subPt_M15to20->Fill( subPt, TotWeight );
								}
								else if( gen_M > 25 && gen_M < 30 )
								{
									this->h_leadPt_M25to30->Fill( leadPt, TotWeight );
									this->h_subPt_M25to30->Fill( subPt, TotWeight );
								}
							} // -- if selected -- //
						} // -- if triggered -- //
					} // -- if accepted -- //
				} // -- if genFlag = TRUE -- //
			} // -- end of event iteration -- //

			TFile *f_output = TFile::Open("ROOTFile_PtSpectrum_massBin.root", "RECREATE");
			this->h_leadPt_M15to20->Write();
			this->h_subPt_M15to20->Write();

			this->h_leadPt_M25to30->Write();
			this->h_subPt_M25to30->Write();
			f_output->close();

			cout << "finished" << endl;
		}

	}

private:
	void Init()
	{
		this->h_leadPt_M15to20 = new TH1D("h_leadPt_M15to20", "", 10000, 0, 10000);
		this->h_subPt_M15to20 = new TH1D("h_subPt_M15to20", "", 10000, 0, 10000);

		this->h_leadPt_M25to30 = new TH1D("h_leadPt_M25to30", "", 10000, 0, 10000);
		this->h_subPt_M25to30 = new TH1D("h_subPt_M25to30", "", 10000, 0, 10000);
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
}

void PtSpectrum_massBin()
{
	PtSpectrumTool *tool = new PtSpectrumTool();
	tool->RunEventLoop();
}