#include <Include/PlotTools.h>

void DrawCanvas( TH1D* h_KP, TH1D* h_Ridhi )
{
	HistInfo *Hist_Ridhi = new HistInfo( kGreen+2, "Ridhi", h_Ridhi );
	HistInfo *Hist_KP = new HistInfo( kBlue, "KP ", h_KP );

	TString HistName = h_KP->GetName();
	TString CanvasName = "Local/KP_vs_Ridhi_"+HistName;
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_KP, Hist_Ridhi);

	TString XTitle = "#eta(#mu)";
	if( HistName.Contains("Pt") ) XTitle = "P_{T}(#mu) [GeV]";
	canvas->SetTitle(XTitle, "# electrons", "KP/Ridhi");
	canvas->SetLegendPosition( 0.40, 0.32, 0.97, 0.45 );
	canvas->SetRatioRange( 0.88, 1.12 );
	canvas->SetLatex( "Simulation" );

	if( HistName.Contains("Pt") ) canvas->SetXRange( 0, 200 );
	canvas->Draw( 0, 1 );
	canvas->c->SaveAs(".pdf");
}

void XCheck_Kinematics()
{
	TString FileName_KP = "ROOTFile_Acc_Dressed_EE.root";
	TString FileName_Ridhi = "DYEE_AccTest_M10to3000_Ridhi_v1.root";

	vector< TString > Var_KP = {"Pt", "Eta"};
	vector< TString > Var_Ridhi = {"Pt", "Eta"};

	vector< TString > AccType_KP = {"AccTotal", "AccPass"};
	vector< TString > AccType_Ridhi = {"NoAcc", "Acc"};

	vector< TString > MuonType_KP = {"Lead", "Sub"};
	vector< TString > MuonType_Ridhi = {"Lead", "SubLead"};

	Int_t nVar = (Int_t)Var_KP.size();
	Int_t nAccType = (Int_t)AccType_KP.size();
	Int_t nMuonType = (Int_t)MuonType_KP.size();

	for(Int_t i_var=0; i_var<nVar; i_var++)
	{
		for(Int_t i_acc=0; i_acc<nAccType; i_acc++)
		{
			for(Int_t i_mutype=0; i_mutype<nMuonType; i_mutype++)
			{
				TString HistName_KP = TString::Format("h_%s_%s_%s", AccType_KP[i_acc].Data(), Var_KP[i_var].Data(), MuonType_KP[i_mutype].Data());
				TString HistName_Ridhi = TString::Format("h_%s_%s_%s", Var_Ridhi[i_var].Data(), MuonType_Ridhi[i_mutype].Data(), AccType_Ridhi[i_acc].Data());

				TH1D* h_KP = Get_Hist( FileName_KP, HistName_KP );
				TH1D* h_Ridhi = Get_Hist( FileName_Ridhi, HistName_Ridhi );

				Double_t Lumi_MuonPhys = 2759.017;
				Double_t Lumi_Golden = 2257.998;
				Double_t scale = Lumi_Golden / Lumi_MuonPhys;
				h_KP->Scale( scale );

				if( Var_KP[i_var] == "Eta" )
				{
					h_KP->Rebin(10);
					h_Ridhi->Rebin(10);
				}

				DrawCanvas( h_KP, h_Ridhi );
			}
		}
	}


}