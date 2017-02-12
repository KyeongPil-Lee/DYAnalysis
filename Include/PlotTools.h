#include <TH1D.h>
#include <TColor.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLatex.h>
#include <TFile.h>
#include <TPad.h>
#include <TF1.h>
#include <TGraphAsymmErrors.h>

#include <vector>


class BaseInfo
{
public:
	TString FileName; // -- including path -- //
	TString ObjectName;
	TString LegendName;
	Int_t Color;

	BaseInfo()
	{
		this->Init();
	}

	BaseInfo( Int_t _Color, TString _LegendName ): BaseInfo()
	{
		this->LegendName = _LegendName;
		this->Color = _Color;
	}

	void Init()
	{
		this->FileName = "";
		this->ObjectName = "";
		this->LegendName = "";
		this->Color = 0;
	}

	void Set_FileName_ObjectName( TString _FileName, TString _ObjectName )
	{
		this->FileName = _FileName;
		this->ObjectName = _ObjectName;
	}

	void Set_Color( Int_t _Color )
	{
		this->Color = _Color;
	}

	void Set_LegendName( TString _LegendName )
	{
		this->LegendName = _LegendName;
	}
};

class HistInfo: public BaseInfo
{
public:
	TH1D* h;
	TH1D* h_ratio;

	HistInfo()
	{
		TH1::AddDirectory( kFALSE );

		this->h = NULL;
		this->h_ratio = NULL;
	}

	HistInfo( Int_t _Color, TString _LegendName ): BaseInfo( _Color, _LegendName )
	{
		TH1::AddDirectory( kFALSE );

		this->h = NULL;
		this->h_ratio = NULL;
	}

	void Set()
	{
		if( h == NULL )
			this->Get_Histogram();
		
		this->Set_Attributes();
	}

	void Set_Histogram( TH1D* _h )
	{
		this->h = (TH1D*)_h->Clone();
	}

	void Calc_RatioHist( TH1D* h_Denom )
	{
		if( h == NULL )
		{
			cout << "Histogram is not assigned yet!" << endl;
			return;
		}

		h->Sumw2();
		h_Denom->Sumw2();

		this->h_ratio = (TH1D*)this->h->Clone();
		h_ratio->Divide( this->h, h_Denom );
	}

	// -- same with "Calc_RatioHist" -- //
	void Calc_RatioHist_Denominator( TH1D* h_Denom )
	{
		if( h == NULL )
		{
			cout << "Histogram is not assigned yet!" << endl;
			return;
		}

		h->Sumw2();
		h_Denom->Sumw2();

		this->h_ratio = (TH1D*)this->h->Clone();
		h_ratio->Divide( this->h, h_Denom );
	}

	void Calc_RatioHist_Numerator( TH1D* h_Num )
	{
		if( h == NULL )
		{
			cout << "Histogram is not assigned yet!" << endl;
			return;
		}

		h->Sumw2();
		h_Num->Sumw2();

		this->h_ratio = (TH1D*)this->h->Clone();
		h_ratio->Divide( h_Num, this->h );
	}

protected:
	void Get_Histogram()
	{
		if( this->FileName == "" || this->ObjectName == "" )
		{
			printf( "[FileName, ObjectName] = [%s, %s] ... at least one of them is not set yet\n", this->FileName.Data(), this->ObjectName.Data() );
			return;
		}

		TFile *f_input = TFile::Open( this->FileName );
		f_input->cd();

		this->h = (TH1D*)f_input->Get( this->ObjectName )->Clone();

		f_input->Close();
	}

	void Set_Attributes()
	{
		this->h->SetTitle("");
		this->h->SetStats(kFALSE);

		this->h->SetLineColor( this->Color );
		this->h->SetFillColorAlpha( kWhite, 0 );
		this->h->SetMarkerStyle( 20 );
		this->h->SetMarkerColor( this->Color );
	}

};

class GraphInfo: public BaseInfo
{
public:
	TGraphAsymmErrors* g;
	TGraphAsymmErrors* g_ratio;

	GraphInfo()
	{
		this->g = NULL;
		this->g_ratio = NULL;
	}

	GraphInfo( Int_t _Color, TString _LegendName ): BaseInfo( _Color, _LegendName )
	{
		this->g = NULL;
		this->g_ratio = NULL;
	}

	void Set()
	{
		if( g == NULL )
			this->Get_Graph();
		// this->Set_Attributes(); // -- Attributes for Graph shuold be set after drawing it -- //
	}

	void Set_Graph( TGraphAsymmErrors* _g )
	{
		this->g = (TGraphAsymmErrors*)_g->Clone();
	}

	void DrawGraph( TString DrawOp )
	{
		this->g->Draw( DrawOp );
		this->Set_Attributes();
	}

	void Set_Attributes()
	{
		this->g->SetTitle( "" );

		this->g->SetLineColor( this->Color );
		this->g->SetMarkerStyle( 20 );
		this->g->SetMarkerColor( this->Color );
		this->g->SetFillColorAlpha( kWhite, 0 );

		if( g_ratio != NULL )
		{
			this->g_ratio->SetTitle( "" );

			this->g_ratio->SetLineColor( this->Color );
			this->g_ratio->SetMarkerStyle( 20 );
			this->g_ratio->SetMarkerColor( this->Color );
			this->g_ratio->SetFillColorAlpha( kWhite, 0 );
		}
	}

	void Calc_RatioGraph( TGraphAsymmErrors* g_Denom )
	{
		if( g == NULL )
		{
			cout << "Graph is not assigned yet!" << endl;
			return;
		}

		this->g_ratio = this->MakeRatioGraph( g, g_Denom );
	}

	// -- same with Calc_RatioGraph -- //
	void Calc_RatioGraph_Denominator( TGraphAsymmErrors* g_Denom )
	{
		if( g == NULL )
		{
			cout << "Graph is not assigned yet!" << endl;
			return;
		}

		this->g_ratio = this->MakeRatioGraph( g, g_Denom );
	}

	void Calc_RatioGraph_Numerator( TGraphAsymmErrors* g_Num )
	{
		if( g == NULL )
		{
			cout << "Graph is not assigned yet!" << endl;
			return;
		}

		this->g_ratio = this->MakeRatioGraph( g_Num, g );
	}

protected:
	void Get_Graph()
	{
		if( this->FileName == "" || this->ObjectName == "" )
		{
			printf( "[FileName, ObjectName] = [%s, %s] ... at least one of them is not set yet", this->FileName.Data(), this->ObjectName.Data() );
			return;
		}

		TFile *f_input = TFile::Open( this->FileName );
		f_input->cd();

		this->g = (TGraphAsymmErrors*)f_input->Get( this->ObjectName )->Clone();

		f_input->Close();
	}

	TGraphAsymmErrors* MakeRatioGraph(TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2)
	{
		g_ratio = (TGraphAsymmErrors*)g_Type2->Clone();
		g_ratio->Set(0); // --Remove all points (reset) -- //

		Int_t nPoint = g_Type1->GetN();
		Int_t nPoint_2 = g_Type2->GetN();
		if( nPoint != nPoint_2 )
			printf("# points is different bewteen two graph...be careful for the ratio plot\n");

		for(Int_t i_p=0; i_p<nPoint; i_p++)
		{
			// cout << i_p << "th Point" << endl;
			//Get Type1 point
			Double_t x_Type1, y_Type1;
			g_Type1->GetPoint(i_p, x_Type1, y_Type1);
			Double_t error_Type1 = this->ReturnLargerValue( g_Type1->GetErrorYhigh(i_p), g_Type1->GetErrorYlow(i_p) );
			// cout << "x_Type1: " << x_Type1 << " y_Type1: " << y_Type1 << " error_Type1: " << error_Type1 << " g_Type1->GetErrorYhigh: " << g_Type1->GetErrorYhigh(i_p) << " g_Type1->GetErrorYlow: " << g_Type1->GetErrorYlow(i_p) << endl;

			//Get Type2 point
			Double_t x_Type2, y_Type2;
			g_Type2->GetPoint(i_p, x_Type2, y_Type2);
			Double_t error_Type2 = this->ReturnLargerValue( g_Type2->GetErrorYhigh(i_p), g_Type2->GetErrorYlow(i_p) );
			// cout << "x_Type2: " << x_Type2 << " y_Type2: " << y_Type2 << " error_Type2: " << error_Type2 << " g_Type2->GetErrorYhigh: " << g_Type2->GetErrorYhigh(i_p) << " g_Type2->GetErrorYlow: " << g_Type2->GetErrorYlow(i_p) << endl;

			Double_t ratio;
			Double_t ratio_error;
			if( (nPoint != nPoint_2) && i_p >= nPoint_2)
			{
				ratio = 0;
				ratio_error = 0;
			}
			// else if(y_Type1 != 0 && error_Type1 != 0 && y_Type2 != 0 && error_Type2 != 0)
			else if(y_Type2 != 0)
			{
				ratio = y_Type1 / y_Type2;
				ratio_error = this->Error_PropagatedAoverB(y_Type1, error_Type1, y_Type2, error_Type2);
				//calculate Scale Factor(Type1/Type2) & error

				// cout << "ratio: " << ratio << " ratio_error: " << ratio_error << endl;
			}
			else
			{
				cout << "Denominator is 0! ... ratio and its error are set as 0" << endl;
				ratio = 0;
				ratio_error = 0;
			}

			//Set Central value
			g_ratio->SetPoint(i_p, x_Type1, ratio);

			//Set the error
			Double_t error_XLow = g_Type1->GetErrorXlow(i_p);
			Double_t error_Xhigh = g_Type1->GetErrorXhigh(i_p);
			g_ratio->SetPointError(i_p, error_XLow, error_Xhigh, ratio_error, ratio_error);

			// cout << endl;
		}

		return g_ratio;
	}

	Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B)
	{
		Double_t ratio_A = (sigma_A) / A;
		Double_t ratio_B = (sigma_B) / B;

		Double_t errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;

		return (A/B) * sqrt(errorSquare);
	}

	Double_t ReturnLargerValue(Double_t a, Double_t b)
	{
		if( a > b )
			return a;
		else
			return b;
	}
};

void SetCanvas_Square( TCanvas*& c, TString CanvasName, Bool_t isLogx = kFALSE, Bool_t isLogy = kFALSE )
{
	c = new TCanvas(CanvasName, "", 800, 800);
	c->cd();
	
	c->SetTopMargin(0.05);
	c->SetLeftMargin(0.13);
	c->SetRightMargin(0.045);
	c->SetBottomMargin(0.13);

	if( isLogx )
		c->SetLogx();
	if( isLogy )
		c->SetLogy();
}

void SetCanvas_Ratio( TCanvas*& c, TString CanvasName, TPad*& TopPad, TPad*& BottomPad, Bool_t isLogx = kFALSE, Bool_t isLogy = kFALSE )
{
	c = new TCanvas(CanvasName, "", 800, 800);
	c->cd();

	TopPad = new TPad("TopPad","TopPad", 0.01, 0.01, 0.99, 0.99 );
	TopPad->Draw();
	TopPad->cd();

	TopPad->SetTopMargin(0.05);
	TopPad->SetLeftMargin(0.13);
	TopPad->SetRightMargin(0.045);
	TopPad->SetBottomMargin(0.3);

	if( isLogx )
		TopPad->SetLogx();
	if( isLogy )
		TopPad->SetLogy();

	c->cd();
	BottomPad = new TPad( "BottomPad", "BottomPad", 0.01, 0.01, 0.99, 0.28 );
	BottomPad->Draw();
	BottomPad->cd();
	BottomPad->SetGridx();
	BottomPad->SetGridy();
	BottomPad->SetBottomMargin(0.4);
	BottomPad->SetRightMargin(0.045);
	BottomPad->SetLeftMargin(0.13);

	if( isLogx )
		BottomPad->SetLogx();
}
// -- http://igotit.tistory.com/entry/C-함수-인자로-포인터-전달하고-함수내에서-동적-메모리-할당-받기-2가지-방식 -- //
void DrawLine( TF1*& f_line, Int_t color = kRed )
{
	f_line = new TF1("f_line", "1", -10000, 10000);
	f_line->SetLineColor(color);
	f_line->SetLineWidth(1);
	f_line->Draw("PSAME");
}

void Latex_Preliminary( TLatex &latex, Double_t Lumi  )
{
	latex.DrawLatexNDC(0.70, 0.96, "#font[42]{#scale[0.8]{"+TString::Format("%.2lf", Lumi)+" fb^{-1} (13 TeV)}}");
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
}

void Latex_Preliminary( TLatex &latex, Double_t Lumi, Int_t E_CM  )
{
	latex.DrawLatexNDC(0.69, 0.96, "#font[42]{#scale[0.8]{"+TString::Format("%.1lf fb^{-1} (%d TeV)", Lumi, E_CM)+"}}");
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
}

void Latex_Preliminary_NoDataInfo( TLatex &latex )
{
	// latex.DrawLatexNDC(0.69, 0.96, "#font[42]{#scale[0.8]{"+TString::Format("%.1lf fb^{-1} (%d TeV)", Lumi, E_CM)+"}}");
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
}

void Latex_Simulation( TLatex &latex )
{
	latex.DrawLatexNDC(0.82, 0.96, "#font[42]{#scale[0.8]{13 TeV}}");
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.25, 0.96, "#font[42]{#it{#scale[0.8]{Simulation}}}");
}

void SetAxis_SinglePad( TAxis *X_axis, TAxis *Y_axis, TString XTitle, TString YTitle )
{
	X_axis->SetTitle( XTitle );
	X_axis->SetLabelSize(0.04);
	X_axis->SetTitleOffset(1.1);
	X_axis->SetTitleSize(0.05);

	Y_axis->SetTitle( YTitle );
	Y_axis->SetTitleSize(0.05);
	Y_axis->SetTitleOffset(1.2);
	Y_axis->SetLabelSize(0.045);
}

void SetAxis_TopPad( TAxis *X_axis, TAxis *Y_axis, TString YTitle )
{
	X_axis->SetLabelFont(42);
	X_axis->SetLabelSize(0.000);
	X_axis->SetTitleSize(0.000);

	Y_axis->SetTitleFont(42);
	Y_axis->SetTitle( YTitle );
	Y_axis->SetTitleSize(0.05);
	Y_axis->SetTitleFont(42);
	Y_axis->SetTitleOffset(1.25);
	Y_axis->SetLabelFont(42);
	Y_axis->SetLabelSize(0.04);
}

void SetAxis_BottomPad( TAxis *X_axis, TAxis *Y_axis, TString XTitle, TString YTitle, Double_t yMin = 0.51, Double_t yMax = 1.49 )
{
	X_axis->SetMoreLogLabels();
	X_axis->SetNoExponent();
	X_axis->SetTitle( XTitle );
	X_axis->SetTitleOffset( 0.85 );
	X_axis->SetTitleSize( 0.2 );
	X_axis->SetLabelColor(1);
	X_axis->SetLabelFont(42);
	X_axis->SetLabelOffset(0.01);
	X_axis->SetLabelSize(0.13);

	Y_axis->SetTitle( YTitle );
	Y_axis->SetTitleOffset( 0.55 );
	Y_axis->SetTitleSize( 0.12);
	Y_axis->SetRangeUser( yMin, yMax );
	Y_axis->SetLabelSize( 0.10 );
}


void SetHistFormat_SinglePad( TH1D* h_format, TString XTitle, TString YTitle )
{
	SetAxis_SinglePad( h_format->GetXaxis(), h_format->GetYaxis(), XTitle, YTitle );
}

void SetHistFormat_TopPad( TH1D* h_format, TString YTitle )
{
	SetAxis_TopPad( h_format->GetXaxis(), h_format->GetYaxis(), YTitle );
}

void SetHistFormat_BottomPad( TH1D* h_format, TString XTitle, TString YTitle, Double_t yMin = 0.51, Double_t yMax = 1.49 )
{
	SetAxis_BottomPad( h_format->GetXaxis(), h_format->GetYaxis(), XTitle, YTitle, yMin, yMax );
}

void SetGraphFormat_SinglePad( TGraphAsymmErrors* g_format, TString XTitle, TString YTitle )
{
	SetAxis_SinglePad( g_format->GetXaxis(), g_format->GetYaxis(), XTitle, YTitle );
}

void SetGraphFormat_TopPad( TGraphAsymmErrors* g_format, TString YTitle )
{
	SetAxis_TopPad( g_format->GetXaxis(), g_format->GetYaxis(), YTitle );
}

void SetGraphFormat_BottomPad( TGraphAsymmErrors* g_format, TString XTitle, TString YTitle, Double_t yMin = 0.51, Double_t yMax = 1.49 )
{
	SetAxis_BottomPad( g_format->GetXaxis(), g_format->GetYaxis(), XTitle, YTitle, yMin, yMax );
}

void SetHist_Color( TH1D* h, Int_t color, Bool_t isFill = kFALSE )
{
	h->SetTitle("");
	h->SetStats(kFALSE);
	h->SetLineColor( color );
	h->SetMarkerStyle( 20 );
	h->SetMarkerColor( color );

	if( isFill )
		h->SetFillColor( color );
	else
		h->SetFillColorAlpha( kWhite, 0 );
}

void Print_Histogram( TH1D* h )
{
	h->Print();
	Int_t nBin = h->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t LowerEdge = h->GetBinLowEdge(i_bin);
		Double_t UpperEdge = h->GetBinLowEdge(i_bin+1);

		Double_t value = h->GetBinContent(i_bin);
		Double_t errorAbs = h->GetBinError(i_bin);
		Double_t errorRel;
		if( value != 0 )
			errorRel = errorAbs / value;
		else
			errorRel = 0;

		printf( "%02d bin: [%6.1lf, %6.1lf] (value, error) = (%lf, %lf (%7.3lf %%))\n", 
			     i_bin, LowerEdge, UpperEdge, value, errorAbs, errorRel*100 );
	}
	printf("\n\n");
}

void SetLegend( TLegend *& legend, Double_t xMin = 0.75, Double_t yMin = 0.75, Double_t xMax = 0.95, Double_t yMax = 0.95 )
{
	legend = new TLegend( xMin, yMin, xMax, yMax );
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
}

TH1D* Get_Hist(TString FileName, TString HistName, TString HistName_New = "" )
{
	TH1::AddDirectory(kFALSE);

	TFile *f_input = TFile::Open( FileName );
	TH1D* h_temp = (TH1D*)f_input->Get(HistName)->Clone();
	if( HistName_New != "" )
		h_temp->SetName( HistName_New );

	f_input->Close();

	return h_temp;
}

TH1D* QuadSum_NoError( TH1D* h1, TH1D* h2 )
{
	TH1D* h_QuadSum = (TH1D*)h1->Clone( "h_QuadSum" );
	Int_t nBin = h1->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t value1 = h1->GetBinContent(i_bin);
		Double_t value2 = h2->GetBinContent(i_bin);

		Double_t QuadSum = sqrt( value1*value1 + value2*value2 );

		h_QuadSum->SetBinContent( i_bin, QuadSum );
		h_QuadSum->SetBinError( i_bin, 0 );
	}
	return h_QuadSum;
}

void AssignErrors( TH1D* h_cv, TH1D* h_RelUnc, Bool_t isPercent = kFALSE )
{
	Int_t nBin = h_cv->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t value = h_cv->GetBinContent(i_bin);
		Double_t RelUnc = h_RelUnc->GetBinContent(i_bin);
		if( isPercent ) RelUnc = RelUnc / 100.0;

		Double_t AbsUnc = value * RelUnc;

		h_cv->SetBinError(i_bin, AbsUnc);
	}
}

void AssignErrors_Graph( TGraphAsymmErrors* g, TH1D* h_RelUnc, Bool_t isPercent = kFALSE )
{
	Int_t nPoint = g->GetN();
	Int_t nBin = h_RelUnc->GetNbinsX();
	if( nPoint != nBin )
	{
		printf("[nPoint != nBin: %d, %d]\n", nPoint, nBin);
		return;
	}

	for(Int_t i=0; i<nPoint; i++)
	{
		Double_t x = 0;
		Double_t y = 0;
		g->GetPoint( i, x, y );

		Int_t i_bin = i+1;
		Double_t RelUnc = h_RelUnc->GetBinContent(i_bin);
		if( isPercent ) RelUnc = RelUnc / 100.0;
		
		Double_t AbsUnc = y * RelUnc;

		g->SetPointEYlow(i, AbsUnc );
		g->SetPointEYhigh(i, AbsUnc );
	}
}

TH1D* Convert_GraphToHist( TGraphAsymmErrors *g )
{
	const Int_t nBin = g->GetN();
	Double_t *BinEdges = new Double_t[nBin+1];
	Double_t *value = new Double_t[nBin];
	Double_t *error = new Double_t[nBin];

	for(Int_t i=0; i<nBin; i++)
	{
		Double_t x, y;
		g->GetPoint(i, x, y);

		// -- make BinEdges array -- //
		Double_t ErrX_Low = g->GetErrorXlow(i);
		Double_t ErrX_High = g->GetErrorXhigh(i);

		if( i == nBin-1 )
		{
			BinEdges[i] = x - ErrX_Low;
			BinEdges[i+1] = x + ErrX_High;
		}
		else
			BinEdges[i] = x - ErrX_Low;


		// -- store graph information -- //
		value[i] = y;

		Double_t ErrY_Low = g->GetErrorYlow(i);
		Double_t ErrY_High = g->GetErrorYhigh(i);

		// -- take the larger one -- //
		error[i] = ErrY_Low > ErrY_High ? ErrY_Low : ErrY_High;
	}

	TString GraphName = g->GetName();
	TH1D* h_temp = new TH1D( "h_"+GraphName, "", nBin, BinEdges );

	// -- fill this histogram using graph information -- //
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		h_temp->SetBinContent( i_bin, value[i] );
		h_temp->SetBinContent( i_bin, error[i] );
	}

	return h_temp;
}