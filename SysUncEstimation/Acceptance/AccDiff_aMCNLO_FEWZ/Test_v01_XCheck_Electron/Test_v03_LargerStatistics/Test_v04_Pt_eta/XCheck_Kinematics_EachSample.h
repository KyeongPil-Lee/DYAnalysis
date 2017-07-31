#include <Include/PlotTools.h>

class XCheckTool
{
public:
	TString FileName_KP;
	TString FileName_Ridhi;

	vector< TString > Var_KP;
	vector< TString > Var_Ridhi;

	vector< TString > AccType_KP;
	vector< TString > AccType_Ridhi;

	vector< TString > MuonType_KP;
	vector< TString > MuonType_Ridhi;

	vector< TString > FSRType_KP;
	vector< TString > FSRType_Ridhi;

	vector< TString > SampleType_KP;
	vector< TString > SampleType_Ridhi;

	vector< TString > vec_HistName_KP;
	vector< TString > vec_HistName_Ridhi;

	XCheckTool()
	{
		this->Init_vectors();
		this->Construct_HistName();
	}

	void Set_FileName( TString _KP, TString _Ridhi )
	{
		this->FileName_KP = _KP;
		this->FileName_Ridhi = _Ridhi;
	}

	void DrawPlot_All()
	{
		Int_t nHist = (Int_t)vec_HistName_KP.size();
		for(Int_t i_hist=0; i_hist<nHist; i_hist++)
		{
			TString HistName_KP = vec_HistName_KP[i_hist];
			TString HistName_Ridhi = vec_HistName_Ridhi[i_hist];

			cout << "HistName_KP: " << HistName_KP << endl;
			TH1D* h_KP = Get_Hist( this->FileName_KP, HistName_KP );

			cout << "HistName_Ridhi: " << HistName_Ridhi << endl;
			TH1D* h_Ridhi = Get_Hist( this->FileName_Ridhi, HistName_Ridhi );

			Int_t nRebin = 1;
			if( HistName_KP.Contains("Eta") && HistName_KP.Contains("AccTotal") ) nRebin = 10;
			if( HistName_KP.Contains("Eta") && HistName_KP.Contains("AccPass") ) nRebin = 10;
			h_KP->Rebin( nRebin );
			h_Ridhi->Rebin( nRebin );

			this->DrawCanvas( h_KP, h_Ridhi );
		}
	}

protected:
	void Construct_HistName()
	{
		Int_t nVar = (Int_t)Var_KP.size();
		Int_t nAccType = (Int_t)AccType_KP.size();
		Int_t nMuonType = (Int_t)MuonType_KP.size();
		Int_t nFSRType = (Int_t)FSRType_KP.size();
		Int_t nSample = (Int_t)SampleType_KP.size();

		for(Int_t i_var=0; i_var<nVar; i_var++)
		{
			for(Int_t i_acc=0; i_acc<nAccType; i_acc++)
			{
				for(Int_t i_type=0; i_type<nMuonType; i_type++)
				{
					for(Int_t i_fsr=0; i_fsr<nFSRType; i_fsr++)
					{
						if( i_fsr==1 && i_acc==1) continue; // -- no plots for post-FSR, within acc -- //

						for(Int_t i_sample=0; i_sample<nSample; i_sample++)
						{
							TString HistName_KP = 
							TString::Format("h_%s_%s_%s_%s", 
								AccType_KP[i_acc].Data(), Var_KP[i_var].Data(), MuonType_KP[i_type].Data(), SampleType_KP[i_sample].Data());
							if( i_fsr == 1 ) HistName_KP = HistName_KP + "_" + FSRType_KP[i_fsr];

							TString HistName_Ridhi = 
							TString::Format("h_%s_%s_%s_%s_%s",
								Var_Ridhi[i_var].Data(), MuonType_Ridhi[i_type].Data(), AccType_Ridhi[i_acc].Data(), FSRType_Ridhi[i_fsr].Data(), SampleType_Ridhi[i_sample].Data());

							vec_HistName_KP.push_back( HistName_KP );
							vec_HistName_Ridhi.push_back( HistName_Ridhi );
						}
					}
				}
			}
		}
	}

	void Init_vectors()
	{
		Var_KP.push_back( "Pt"); Var_Ridhi.push_back( "Pt" );
		Var_KP.push_back( "Eta"); Var_Ridhi.push_back( "Eta" );

		AccType_KP.push_back( "AccTotal" ); AccType_Ridhi.push_back( "NoAcc" );
		AccType_KP.push_back( "AccPass" ); AccType_Ridhi.push_back( "Acc" );

		MuonType_KP.push_back( "Lead" ); MuonType_Ridhi.push_back( "Lead" );
		MuonType_KP.push_back( "Sub" ); MuonType_Ridhi.push_back( "SubLead" );

		FSRType_KP.push_back( " "); FSRType_Ridhi.push_back( "PreFSR" );
		FSRType_KP.push_back( "postFSR"); FSRType_Ridhi.push_back( "PostFSR" );

		// SampleType_Ridhi.push_back( "M10to50" );
		// SampleType_Ridhi.push_back( "M50to100" );
		// SampleType_Ridhi.push_back( "M100to200" );
		// SampleType_Ridhi.push_back( "M200to400" );
		// SampleType_Ridhi.push_back( "M400to500" );
		// SampleType_Ridhi.push_back( "M500to700" );
		// SampleType_Ridhi.push_back( "M700to800" );
		// SampleType_Ridhi.push_back( "M800to1000" );
		// SampleType_Ridhi.push_back( "M1000to1500" );
		// SampleType_Ridhi.push_back( "M1500to2000" );
		SampleType_Ridhi.push_back( "M2000to3000" );

		for(const auto& SampleType : SampleType_Ridhi )
			SampleType_KP.push_back( "DYEE_" + SampleType );
	}

	void DrawCanvas( TH1D* h_KP, TH1D* h_Ridhi )
	{
		TString HistName = h_KP->GetName();

		Int_t nRebin_Pt = 1;
		if( HistName.Contains("Pt") )
		{
			if( HistName.Contains("M500to700") ) nRebin_Pt = 5;
			if( HistName.Contains("M700to800") ) nRebin_Pt = 5;
			if( HistName.Contains("M800to1000") ) nRebin_Pt = 5;
			if( HistName.Contains("M1000to1500") ) nRebin_Pt = 10;
			if( HistName.Contains("M1500to2000") ) nRebin_Pt = 25;
			if( HistName.Contains("M2000to3000") ) nRebin_Pt = 50;

			h_KP->Rebin(nRebin_Pt);
			h_Ridhi->Rebin(nRebin_Pt);
		}

		HistInfo *Hist_Ridhi = new HistInfo( kGreen+2, "Ridhi", h_Ridhi );
		HistInfo *Hist_KP = new HistInfo( kBlue, "KP ", h_KP );

		
		TString CanvasName = "Local/v20170727_EachSample/KP_vs_Ridhi_"+HistName;
		DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_KP, Hist_Ridhi);

		TString XTitle = "#eta(e)";
		if( HistName.Contains("Pt") ) XTitle = "P_{T}(e) [GeV]";
		canvas->SetTitle(XTitle, "# electrons", "KP/Ridhi");
		canvas->SetLegendPosition( 0.40, 0.32, 0.97, 0.45 );
		canvas->SetRatioRange( 0.88, 1.12 );
		canvas->SetLatex( "Simulation" );

		if( HistName.Contains("Pt") )
		{
			canvas->SetXRange( 0, 200 );
			if( HistName.Contains("M400to500") ) canvas->SetXRange( 0, 300 );
			if( HistName.Contains("M500to700") ) canvas->SetXRange( 0, 400 );
			if( HistName.Contains("M700to800") ) canvas->SetXRange( 0, 500 );
			if( HistName.Contains("M800to1000") ) canvas->SetXRange( 0, 700 );
			if( HistName.Contains("M1000to1500") ) canvas->SetXRange( 0, 900 );
			if( HistName.Contains("M1500to2000") ) canvas->SetXRange( 0, 1400 );
			if( HistName.Contains("M2000to3000") ) canvas->SetXRange( 0, 1900 );
		}
		canvas->Draw( 0, 1 );
		canvas->c->SaveAs(".pdf");
	}

};