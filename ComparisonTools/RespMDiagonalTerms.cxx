#include <Include/PlotTools.h>

class CompTool
{
public:
	TH1D* h_diag_detRes_old;
	TH1D* h_diag_detRes_new;

	TH1D* h_diag_FSR_old;
	TH1D* h_diag_FSR_new;

	CompTool()
	{
		this->Init();
	}

	void Draw_All()
	{
		this->Draw(this->h_diag_detRes_old, this->h_diag_detRes_new, "DetRes");
		this->Draw(this->h_diag_FSR_old, this->h_diag_FSR_new, "FSR");
	}

private:
	void Draw( TH1D* h_old, TH1D* h_new, TString type )
	{
		// -- Draw canvas
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString canvasName = "Local/c_DiagTerm_"+type;
		SetCanvas_Ratio( c, canvasName, TopPad, BottomPad, 1, 0 );

		c->cd();
		TopPad->cd();

		h_old->Draw("EPSAME");
		h_new->Draw("EPSAME");

		h_old->SetStats(kFALSE);
		h_old->SetMarkerStyle(20);
		h_old->SetMarkerColor(kBlack);
		h_old->SetLineColor(kBlack);
		h_old->SetFillColorAlpha(kWhite, 0); 
		h_old->SetTitle("");

		h_new->SetStats(kFALSE);
		h_new->SetMarkerStyle(20);
		h_new->SetMarkerColor(kRed);
		h_new->SetLineColor(kRed);
		h_new->SetFillColorAlpha(kWhite, 0); 
		h_new->SetTitle("");

		SetHistFormat_TopPad( h_old, "Diag. term");
		h_old->GetYaxis()->SetRangeUser(0.5, 1.3);

		TLegend *legend;
		SetLegend( legend, 0.60, 0.82, 0.95, 0.95 );
		legend->AddEntry( h_old, "Before" );
		legend->AddEntry( h_new, "After" );
		legend->Draw();

		TLatex latex;
		if( type == "DetRes" )
			latex.DrawLatexNDC(0.16, 0.35, "#scale[0.6]{Response matrix for detector resolution}");
		else if( type == "FSR" )
			latex.DrawLatexNDC(0.16, 0.35, "#scale[0.6]{Response matrix for FSR correction}");

		c->cd();
		BottomPad->cd();

		TH1D* h_ratio = (TH1D*)h_old->Clone("h_ratio");
		h_ratio->Divide( h_new, h_old );

		h_ratio->Draw("EPSAME");

		SetHistFormat_BottomPad( h_ratio, "m [GeV]", "After/Before", 0.99, 1.01 );

		TF1 *f_line;
		DrawLine(f_line);

		Print_Histogram( h_ratio );

		c->SaveAs(".pdf");
	}

	void Init()
	{
		TString rootFilePath_old = "./Local/OldROOTFiles";
		TString rootFilePath_new = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString fileNameBase_detRes = "ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root";
		TString fileNameBase_FSR = "ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root";

		// -- detector resolution
		TString fileName_detRes_old = rootFilePath_old + "/" + fileNameBase_detRes;
		TString fileName_detRes_new = rootFilePath_new + "/" + fileNameBase_detRes;

		TH1D* h_truth_detRes_old = Get_Hist( fileName_detRes_old, "h_Truth_RooUnfold" );
		TH2D* h_nEvents_detRes_old = Get_Hist_2D( fileName_detRes_old, "h_RespM_RooUnfold" );
		TH2D* h_respM_detRes_old = this->CalcFractionPerBin_Transpose( h_nEvents_detRes_old, h_truth_detRes_old );
		this->h_diag_detRes_old = this->DiagHist( h_respM_detRes_old );

		TH1D* h_truth_detRes_new = Get_Hist( fileName_detRes_new, "h_Truth_RooUnfold" );
		TH2D* h_nEvents_detRes_new = Get_Hist_2D( fileName_detRes_new, "h_RespM_RooUnfold" );
		TH2D* h_respM_detRes_new = this->CalcFractionPerBin_Transpose( h_nEvents_detRes_new, h_truth_detRes_new );
		this->h_diag_detRes_new = this->DiagHist( h_respM_detRes_new );


		// -- FSR
		TString fileName_FSR_old = rootFilePath_old + "/" + fileNameBase_FSR;
		TString fileName_FSR_new = rootFilePath_new + "/" + fileNameBase_FSR;

		TH1D* h_truth_FSR_old = Get_Hist( fileName_FSR_old, "h_Truth_RooUnfold" );
		TH2D* h_nEvents_FSR_old = Get_Hist_2D( fileName_FSR_old, "h_RespM_RooUnfold" );
		TH2D* h_respM_FSR_old = this->CalcFractionPerBin_Transpose( h_nEvents_FSR_old, h_truth_FSR_old );
		this->h_diag_FSR_old = this->DiagHist( h_respM_FSR_old );

		TH1D* h_truth_FSR_new = Get_Hist( fileName_FSR_new, "h_Truth_RooUnfold" );
		TH2D* h_nEvents_FSR_new = Get_Hist_2D( fileName_FSR_new, "h_RespM_RooUnfold" );
		TH2D* h_respM_FSR_new = this->CalcFractionPerBin_Transpose( h_nEvents_FSR_new, h_truth_FSR_new );
		this->h_diag_FSR_new = this->DiagHist( h_respM_FSR_new );
	}

	TH1D* DiagHist( TH2D* h_respM )
	{
		const Int_t nBin = h_respM->GetNbinsX();
		Double_t arr_binEdge[nBin+1];

		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t edge = h_respM->GetXaxis()->GetBinLowEdge(i_bin);
			arr_binEdge[i] = edge;
		}
		arr_binEdge[nBin] = h_respM->GetXaxis()->GetBinLowEdge(nBin+1); // -- last bin


		TH1D* h_diag = new TH1D("h_diag", "", nBin, arr_binEdge);

		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t value = h_respM->GetBinContent(i_bin, i_bin);
			Double_t error= h_respM->GetBinError(i_bin, i_bin);

			h_diag->SetBinContent(i_bin, value);
			h_diag->SetBinError(i_bin, error);
		}

		return h_diag;
	}

	TH2D* CalcFractionPerBin_Transpose(TH2D *h_nEvents, TH1* h_Truth )
	{
		TH2D *h_response = (TH2D*)h_nEvents->Clone();

		Int_t nBins = h_Truth->GetNbinsX();
		for(Int_t i_genbin=0; i_genbin <= nBins+1; i_genbin++) // -- Include under/overflow -- //
		{
			Double_t SumN_truth = h_Truth->GetBinContent(i_genbin);

			for(Int_t i_recobin=0; i_recobin <= nBins+1; i_recobin++) // -- Include under/overflow -- //
			{
				Double_t nEvent = h_nEvents->GetBinContent(i_recobin, i_genbin);
				Double_t error = h_nEvents->GetBinError(i_recobin, i_genbin);

				Double_t fraction = 0;
				Double_t err_fraction = 0;
				if( SumN_truth == 0 )
				{
					fraction = 0;
					err_fraction = 0;
				}
				else
				{
					fraction = nEvent / SumN_truth;
					err_fraction = error / SumN_truth;
				}
				
				if( fraction < 0 && fabs(fraction) < 1e-3 )
				{
					fraction = 0;
					err_fraction = 0;
				}

				h_response->SetBinContent( i_recobin, i_genbin, fraction );
				h_response->SetBinError( i_recobin, i_genbin, err_fraction );
			}
		}

		return this->Transpose( h_response );
	}

	TH2D* Transpose( TH2D* h_2D )
	{
		TH2D* h_2D_Transposed = (TH2D*)h_2D->Clone();
		h_2D_Transposed->SetDirectory(0);

		Int_t nBin_X = h_2D->GetNbinsX();
		Int_t nBin_Y = h_2D->GetNbinsY();

		if(nBin_X != nBin_Y )
		{
			printf("(nBin_X, nBin_Y) = (%d, %d): this is not squared matrix! ... transpose for this matrix is not provided by this function\n", nBin_X, nBin_Y);
			return NULL;
		}

		for(Int_t iter_x=0; iter_x<nBin_X; iter_x++)
		{
			Int_t i_binX = iter_x+1;
			for(Int_t iter_y=0; iter_y<nBin_Y; iter_y++)
			{
				Int_t i_binY = iter_y+1;

				Double_t value_before = h_2D->GetBinContent(i_binX, i_binY);

				h_2D_Transposed->SetBinContent(i_binY, i_binX, value_before);
			}
		}

		return h_2D_Transposed;
	}

};


void RespMDiagonalTerms()
{
	CompTool *tool = new CompTool();
	tool->Draw_All();
}