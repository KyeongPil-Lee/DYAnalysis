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
using namespace std;

#define nWeight 111

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

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

class HistContainer
{
public:
	TString Type;

	TH1D* h_mass_withinAcc;
	TH1D* h_mass_postFSR;
	TH1D* h_mass_dressed;

	HistContainer( TString _Type )
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( Bool_t Flag_Acc, GenLepton GenLep_postFSR1, GenLepton GenLep_postFSR2, GenLepton GenLep_Dressed1, GenLepton GenLep_Dressed2, Double_t weight )
	{
		Double_t M_postFSR = (GenLep_postFSR1.Momentum + GenLep_postFSR2.Momentum).M();
		Double_t M_Dressed = (GenLep_Dressed1.Momentum + GenLep_Dressed2.Momentum).M();

		this->h_mass_postFSR->Fill( M_postFSR, weight );
		if( Flag_Acc ) this->h_mass_withinAcc->Fill( M_postFSR, weight );

		this->h_mass_dressed->Fill( M_Dressed, weight );
	}

	void Scale( Double_t _scale )
	{
		h_mass_withinAcc->Scale( _scale );
		h_mass_postFSR->Scale( _scale );
		h_mass_dressed->Scale( _scale );
	}

	void Add( HistContainer* Hists )
	{
		this->h_mass_withinAcc->Add( Hists->h_mass_withinAcc );
		this->h_mass_postFSR->Add( Hists->h_mass_postFSR );
		this->h_mass_dressed->Add( Hists->h_mass_dressed );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();
		h_mass_withinAcc->Write();
		h_mass_postFSR->Write();
		h_mass_dressed->Write();
	}

protected:
	void Init_Hist()
	{
		Double_t MassBinEdges[nMassBin+1] = 
		{
			15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
			64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
			110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
			200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
			830, 1000, 1500, 3000
		};

	 	this->h_mass_withinAcc = new TH1D("h_mass_withinAcc_"+this->Type, "", nMassBin, MassBinEdges);		 
	 	this->h_mass_postFSR = new TH1D("h_mass_postFSR_"+this->Type, "", nMassBin, MassBinEdges);
		this->h_mass_dressed = new TH1D("h_mass_dressed_"+this->Type, "", nMassBin, MassBinEdges);

		this->h_mass_withinAcc->Sumw2();
		this->h_mass_postFSR->Sumw2();
		this->h_mass_dressed->Sumw2();
	}
};

class HistProducer
{
public:
	TString Flavor;
	TString SampleType;
	Int_t LeptonID;
	Double_t Luminosity;

	Double_t PtCut_Lead;
	Double_t PtCut_Sub;
	Double_t EtaCut_Lead;
	Double_t EtaCut_Sub;
	
	HistProducer()
	{
		this->Flavor = "";
	}

	HistProducer( TString _Flavor ) : HistProducer()
	{
		this->Set_Flavor( _Flavor );
	}

	void Set_Flavor( TString _Flavor )
	{
		this->Flavor = _Flavor;
		if( Flavor != "Muon" && Flavor != "Electron" )
			cout << this->Flavor << " is WRONG! ... Muon or Electron are available" << endl;

		this->Setup();
	}
	void Produce()
	{
		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

		TStopwatch totaltime;
		totaltime.Start();

		DYAnalyzer *analyzer = new DYAnalyzer( "None" );

		TFile *f_output = new TFile("ROOTFile_Hists_ForPDFUnc_XSec_aMCNLO_"+this->Flavor+".root", "RECREATE");

		HistContainer* Hists_CV_All = new HistContainer( "CV_All" );
		HistContainer* Hists_Weighted_All[nWeight];
		for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
		{
			TString TStr_Number = TString::Format("%03d", i_weight);
			Hists_Weighted_All[i_weight] = new HistContainer( TStr_Number+"_All" );
		}

		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2(this->SampleType, &ntupleDirectory, &Tag, &Xsec, &nEvents);

		// -- Loop for all samples -- //
		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			TStopwatch looptime;
			looptime.Start();

			cout << "\t<" << Tag[i_tup] << ">" << endl;

			// if( Tag[i_tup] == "DYMuMu_M10to50" || Tag[i_tup] == "DYMuMu_M50to100" ) continue;

			TChain *chain = new TChain("recoTree/DYTree");
			TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");

			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_GenOthers();
			ntuple->TurnOnBranches_GenLepton();

			std::vector<double> *PDFWeights; PDFWeights = 0; // -- if it is not initialized, then seg. fault will occur! -- //
			chain->SetBranchStatus("PDFWeights", 1);
			chain->SetBranchAddress("PDFWeights", &PDFWeights);

			// -- Histograms for this sample -- //
			HistContainer* Hists_CV = new HistContainer( "CV_"+Tag[i_tup] );
			HistContainer* Hists_Weighted[nWeight];
			for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
			{
				TString TStr_Number = TString::Format("%03d", i_weight);
				Hists_Weighted[i_weight] = new HistContainer( TStr_Number+"_"+Tag[i_tup] );
			}

			// -- for x-check -- //
			TH1D* h_RelDiff_SumWeights = new TH1D("h_RelDiff_SumWeights_"+Tag[i_tup], "", 10000, -10, 10);

			Double_t SumWeights_CV = 0;
			Double_t SumWeights_Weighted[nWeight] = {0};

			Int_t nTotEvent = chain->GetEntries();
			cout << "\t[Total Events: " << nTotEvent << "]" << endl; 

			// nTotEvent = 1000;
			// -- Event loop starts -- //
			for(Int_t i=0; i<nTotEvent; i++)
			{
				loadBar(i+1, nTotEvent, 100, 100);

				ntuple->GetEvent(i);

				Bool_t GenFlag = kFALSE;
				GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

				if( GenFlag == kTRUE ) // -- DY->mumu events -- //
				{
					// -- Bring weights for NLO MC events -- //
					Double_t GenWeight = 0;
					ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
					SumWeights_CV += GenWeight;

					Double_t GenWeight_Weighted[nWeight];
					for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
					{
						GenWeight_Weighted[i_weight] = GenWeight*PDFWeights->at(i_weight);
						SumWeights_Weighted[i_weight] += GenWeight_Weighted[i_weight];
					}

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

					GenLepton genlep_postFSR1 = GenLeptonCollection[0];
					GenLepton genlep_postFSR2 = GenLeptonCollection[1];
					Bool_t Flag_PassAcc = this->Test_Acc( genlep_postFSR1, genlep_postFSR2 );

					// -- construct dressed leptons -- //
					Double_t dRCut = 0.1;
					GenLepton genlep_dressed1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
					vector< GenOthers > GenPhotonCollection1;
					analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_dressed1, &GenPhotonCollection1);

					GenLepton genlep_dressed2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
					vector< GenOthers > GenPhotonCollection2;
					analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_dressed2, &GenPhotonCollection2);

					// -- fill the histograms -- //
					Hists_CV->Fill( Flag_PassAcc, genlep_postFSR1, genlep_postFSR2, genlep_dressed1, genlep_dressed2, GenWeight );
					for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
						Hists_Weighted[i_weight]->Fill( Flag_PassAcc, genlep_postFSR1, genlep_postFSR2, genlep_dressed1, genlep_dressed2, GenWeight_Weighted[i_weight] );

				} // -- End of if( GenFlag == kTRUE )

			} // -- End of event iteration

			if( SumWeights_CV != (Double_t)nEvents[i_tup] )
				cout << "Sum of weights in DYAnalyzer (" << (Double_t)nEvents[i_tup] << ") is different with the direct calculation in this code (" << SumWeights_CV << ")!" << endl;

			Double_t Norm_CV = ( Xsec[i_tup] * this->Luminosity ) / SumWeights_CV;
			printf("[Central value] Sum of weights: %.1lf -> norm.factor = %lf\n", SumWeights_CV, Norm_CV);
			Hists_CV->Scale( Norm_CV );
			Hists_CV_All->Add( Hists_CV );
	
			for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
			{
				Double_t Norm_Weighted = ( Xsec[i_tup] * this->Luminosity ) / SumWeights_Weighted[i_weight];
				printf("\t[%03d-th PDF weight] Sum of weights: %.1lf -> norm.factor = %lf\n", i_weight, SumWeights_Weighted[i_weight], Norm_Weighted );
				Hists_Weighted[i_weight]->Scale( Norm_Weighted );
				Hists_Weighted_All[i_weight]->Add( Hists_Weighted[i_weight] );
			}

			for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
			{
				Double_t RelDiff = (SumWeights_Weighted[i_weight] - SumWeights_CV) / SumWeights_CV;
				h_RelDiff_SumWeights->Fill( RelDiff );
			}

			f_output->cd();
			h_RelDiff_SumWeights->Write();

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
		} //end of i_tup iteration

		Hists_CV_All->Save( f_output );
		for(Int_t i_weight=0; i_weight<nWeight; i_weight++) Hists_Weighted_All[i_weight]->Save( f_output );

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
	}

protected:
	void Setup()
	{
		if( this->Flavor == "Muon" )
		{
			this->SampleType = "aMCNLO_AdditionalSF_PDFUnc";
			this->LeptonID = 13;
			this->Luminosity = Lumi;

			this->PtCut_Lead = 22;
			this->PtCut_Sub = 10;
			this->EtaCut_Lead = 2.4;
			this->EtaCut_Sub = 2.4;
		}
		else if( this->Flavor == "Electron" )
		{
			this->SampleType = "aMCNLO_ee_AdditionalSF_PDFUnc";
			this->LeptonID = 11;
			this->Luminosity = 2257.998; // -- Golden JSON -- //

			this->PtCut_Lead = 30;
			this->PtCut_Sub = 10;
			this->EtaCut_Lead = 2.5;
			this->EtaCut_Sub = 2.5;
		}
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
			if( this->Flavor == "Electron" )
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

};