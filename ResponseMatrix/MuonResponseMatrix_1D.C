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
#include <TSystem.h>
#include <TROOT.h>
#include <TMath.h>

// -- RooUnfold package -- //
#include <src/RooUnfoldResponse.h>
#include <src/RooUnfoldBayes.h>
// #include <src/RooUnfoldInvert.h>

// -- for Rochester Muon momentum correction -- //
#include <Include/RochesterMomCorr_76X/RoccoR.cc>
#include <Include/RochesterMomCorr_76X/rochcor2015.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>
#include <vector>

static inline void loadBar(int x, int n, int r, int w);
void MuonResponseMatrix_1D(Bool_t isCorrected = kTRUE, TString Sample = "aMCNLO", TString HLTname = "IsoMu20_OR_IsoTkMu20")
{
	if( Sample != "aMCNLO" && Sample != "Powheg" )
	{
		cout << "ERROR: " << Sample << " is wrong Sample. Possible sample: aMCNLO, Powheg" << endl;
		return;
	}

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

	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

	TStopwatch totaltime;
	totaltime.Start();
	
	// gSystem->Load("/home/kplee/Unfolding/libRooUnfold.so");
	gROOT->SetBatch(kTRUE);

	DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

	TFile *f = new TFile("ROOTFile_Histogram_ResponseM_1D_" + Sample + "_" + HLTname + ".root", "RECREATE");

	// const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};

	 // const Int_t nMassBin = 45-7;
	 // Double_t MassBinEdges[nMassBin+1] = {50, 55, 60,
	 // 									 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
	 // 									 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
	 // 									 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
	 // 									 830, 1000, 1200, 1500, 2000, 3000};


	TH1D *h_GenMass = new TH1D("h_GenMass", "", nMassBin, MassBinEdges);
	TH1D *h_RecoMass = new TH1D("h_RecoMass", "", nMassBin, MassBinEdges);
	TH1D *h_Resolution = new TH1D("h_Resolution", "", 1000, -10, 10);
	TH2D *h_responseM = new TH2D("h_responseM", "", nMassBin, MassBinEdges, nMassBin, MassBinEdges);

	TH1D *h_Gen_NoReco = new TH1D("h_Gen_NoReco", "", nMassBin, MassBinEdges);
	TH1D *h_NoGen_Reco = new TH1D("h_NoGen_Reco", "", nMassBin, MassBinEdges);

	RooUnfoldResponse *UnfoldRes = new RooUnfoldResponse(h_RecoMass, h_GenMass);
	
	h_responseM->SetXTitle("Reconstruction level M(#mu#mu) [GeV]");
	h_responseM->SetYTitle("Generator level M(#mu#mu) [GeV]");
	
	TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
	//Each ntuple directory & corresponding Tags
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;

	if( Sample == "aMCNLO" )
	{
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_NNLOxSec", &ntupleDirectory, &Tag, &Xsec, &nEvents);
	}
	if( Sample == "Powheg" )
	{
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
	}

	// -- Loop for each sample -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		// if( !Tag[i_tup].Contains("DYMuMu") ) continue;

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");

		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_Muon();

		rochcor2015 *rmcor = new rochcor2015();

		analyzer->SetupPileUpReWeighting_76X( Tag[i_tup] );

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;

		Double_t NormFactor = (Lumi * Xsec[i_tup]) / nEvents[i_tup];
		cout << "\t[Normalization factor: " << NormFactor << "]" << endl;
		
		// NEvents = 10000;
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

			Int_t PU = ntuple->nPileUp;
			Double_t PUWeight = analyzer->PileUpWeightValue_76X( PU );

			Double_t TotWeight = NormFactor * GenWeight * PUWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == kTRUE )
			{
				/////////////////////////////////////
				// -- Generator level selection -- //
				/////////////////////////////////////
				vector< GenLepton > GenLeptonCollection;

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

				Bool_t isPassAcc_GenLepton = kFALSE;
				isPassAcc_GenLepton = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

				///////////////////////////////////////////
				// -- Reconstruction level selection -- //
				///////////////////////////////////////////
				Bool_t isPassEventSelection = kFALSE;

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
					isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

					if( isPassEventSelection == kTRUE )
					{
						Muon Mu1 = SelectedMuonCollection[0];
						Muon Mu2 = SelectedMuonCollection[1];
						Double_t reco_M = ( Mu1.Momentum + Mu2.Momentum ).M();

						if( isPassAcc_GenLepton == kTRUE )
						{
							h_GenMass->Fill( gen_M, TotWeight );
							h_RecoMass->Fill( reco_M, TotWeight );
							h_responseM->Fill( reco_M, gen_M, TotWeight );
							UnfoldRes->Fill( reco_M, gen_M, TotWeight );
							Double_t resol = (gen_M - reco_M) / gen_M;
							h_Resolution->Fill( resol, TotWeight );
						}
						else // -- No gen-level event within the acceptance, but reco event exists (= "Fake" events) -- //
							UnfoldRes->Fake( reco_M, TotWeight );
					}

				} // -- if( ntuple->isTriggered( analyzer->HLT ) ) -- // 

				if( isPassAcc_GenLepton == kTRUE && isPassEventSelection == kFALSE) // -- gen-level event exists in gen-level, but there is no reco-level event -- //
						h_Gen_NoReco->Fill( gen_M, TotWeight );

			} // -- End of if( GenFlag == kTRUE ) -- //

		} //End of event iteration

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;

	} //end of i_tup iteration

	f->cd();

	h_GenMass->Write();
	h_RecoMass->Write();

	h_Gen_NoReco->Write();
	h_NoGen_Reco->Write();


	TH2D* h_nEvents = (TH2D*)h_responseM->Clone();
	h_nEvents->SetName("h_nEvents");
	h_nEvents->Write();

	TCanvas *c_nEvents_Texts = new TCanvas("c_nEvents_Texts", "", 600, 600);
	c_nEvents_Texts->cd();
	h_nEvents->Draw("COLZTEXT");
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetLogz();
	c_nEvents_Texts->Write();


    TString BinName[nMassBin];
	for(Int_t i=0; i<nMassBin; i++)
		BinName[i].Form("M%dto%d", (Int_t)MassBinEdges[i], (Int_t)MassBinEdges[i+1]);

	for(Int_t i=-1; i<=nMassBin; i++) // -- Also normalize Under/Overflow bins -- //
	{
		Int_t i_genbin = i+1;
		Double_t sum = 0;

		for(Int_t j=-1; j<=nMassBin; j++) // -- Summation over all reco-bins ( including under/overflow ) -- //
		{
			Int_t i_recobin = j+1;
			sum += h_responseM->GetBinContent(i_recobin, i_genbin); //Add all i_genbin # events
		}
		Double_t UnderFlow = h_responseM->GetBinContent(0, i_genbin);
		Double_t OverFlow = h_responseM->GetBinContent(nMassBin+1, i_genbin);
		if( i == -1 )
			cout << "GenBin : Below 15GeV(UnderFlow), sum: " << sum << " UnderFlow: " << UnderFlow << " OverFlow: " << OverFlow << endl;
		else if (i == nMassBin)
			cout << "GenBin : Over 3000GeV(OverFlow), sum: " << sum << " UnderFlow: " << UnderFlow << " OverFlow: " << OverFlow << endl;
		else
			cout << "GenBin : " << BinName[i] << " sum: " << sum << " UnderFlow: " << UnderFlow << " OverFlow: " << OverFlow << endl;

		// sum += UnderFlow;
		// sum += OverFlow;

		for(Int_t j=-1; j<=nMassBin; j++) // -- Normalization will include under/overflow bins -- //
		{
			Int_t i_recobin = j+1;
			Double_t NEvent = h_responseM->GetBinContent(i_recobin, i_genbin);
			if( sum != 0 )
				h_responseM->SetBinContent(i_recobin, i_genbin, NEvent/sum ); // Normalization
			else
				h_responseM->SetBinContent(i_recobin, i_genbin, 0 );
		}
	}
	h_responseM->Write();

	TCanvas *c_responseM = new TCanvas("c_responseM", "", 600, 600);
	c_responseM->cd();
	h_responseM->SetStats(kFALSE);
	h_responseM->Draw("COLZ");
	h_responseM->SetMinimum(1e-3);
	h_responseM->SetMaximum(1);
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetLogz();
	c_responseM->Write();

	TCanvas *c_responseM_Texts = new TCanvas("c_responseM_Texts", "", 600, 600);
	c_responseM_Texts->cd();
	h_responseM->Draw("COLZTEXT");
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetLogz();
	c_responseM_Texts->Write();

	// -- Use RooUnfold Package -- //
	UnfoldRes->UseOverflow(kTRUE);
	UnfoldRes->Write();

	TH2* h_RespM = UnfoldRes->Hresponse();
	h_RespM->SetName("h_RespM_RooUnfold");
	h_RespM->Write();
	TCanvas *c_RespM = new TCanvas("c_RespM", "", 600, 600);
	c_RespM->cd();
	h_RespM->SetStats(kFALSE);
	h_RespM->Draw("COLZ");
	h_RespM->SetMinimum(1e-3);
	h_RespM->SetMaximum(1);
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetLogz();
	c_RespM->Write();

	TH1* h_Measured_RooUnfold = UnfoldRes->Hmeasured();
	h_Measured_RooUnfold->SetName("h_Measured_RooUnfold");
	h_Measured_RooUnfold->Write();

	TH1* h_Truth_RooUnfold = UnfoldRes->Htruth();
	h_Truth_RooUnfold->SetName("h_Truth_RooUnfold");
	h_Truth_RooUnfold->Write();

	// -- "Closer" Test: Re-generate Gen-level(truth) distribuion by unfolding reconstruction-level distribution of MC -- //
	// RooUnfoldInvert *UnfoldInvert = new RooUnfoldInvert(UnfoldRes, h_Measured_RooUnfold);
	// TH1D *h_unfolded = (TH1D*)UnfoldInvert->Hreco();

	RooUnfoldBayes *UnfoldBayes = new RooUnfoldBayes(UnfoldRes, h_Measured_RooUnfold);
	TH1D *h_unfolded = (TH1D*)UnfoldBayes->Hreco();
	h_unfolded->SetName("h_unfoldedMC");
	h_unfolded->Write();

	TCanvas *c_closerT = new TCanvas("c_closerT", "", 800, 600);
	c_closerT->cd();
	h_GenMass->Draw("HIST");
	h_RecoMass->Draw("HISTSAME");
	h_unfolded->Draw("EPSAME");

	h_GenMass->GetXaxis()->SetTitle("Dimuon Invariant Mass [GeV]");
	h_GenMass->GetYaxis()->SetTitle("Events");

	h_GenMass->SetStats(kFALSE);
	h_RecoMass->SetStats(kFALSE);
	h_unfolded->SetStats(kFALSE);

	h_GenMass->SetLineColor(kRed);
	h_RecoMass->SetLineColor(kBlue);
	h_unfolded->SetLineColor(kBlack);
	h_unfolded->SetMarkerColor(kBlack);
	h_unfolded->SetMarkerSize(1);
	h_unfolded->SetMarkerStyle(20);

	TLegend *legend = new TLegend(0.75, 0.75, 0.95, 0.95);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->AddEntry(h_GenMass, "Gen-Level (Truth)");
	legend->AddEntry(h_RecoMass, "Reco-Level (Measured)");
	legend->AddEntry(h_unfolded, "Unfolded (MC)");
	legend->Draw();

	c_closerT->Write();

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

