#include <Include/PlotTools.h>

class PlotProducer_RespM
{
public:
  TString channel_;

  TH2D* h_respM_detRes_;
  TH2D* h_respM_FSR_;

  PlotProducer_RespM(TString channel) { channel_ = channel; }

  void Produce()
  {
    Init();
    DrawPlot_2D( "detRes", h_respM_detRes_ );
    DrawPlot_2D( "FSR",    h_respM_FSR_ );
  }

private:
  void DrawPlot_2D( TString type, TH2D* h_respM )
  {
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(1);

    TString xTitle = "";
    TString yTitle = "";
    if( type == "detRes" )
    {
      xTitle = "generated m (post-FSR) [GeV]";
      yTitle = "reconstructed m [GeV]";
    }
    else if( type == "FSR" )
    {
      xTitle = "generated m (dressed lepton) [GeV]";
      yTitle = "generated m (post-FSR) [GeV]";
    }

    TString canvasName = "RespM_"+type+"_"+channel_;
    TCanvas *c = new TCanvas(canvasName, "", 0, 0, 800, 800);
    c->Range(0.7092156,0.9116051,4.044042,3.556467);
    c->SetLogx();
    c->SetLogy();
    c->SetLogz();
    c->SetGridx();
    c->SetGridy();
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetLeftMargin(0.14);
    c->SetRightMargin(0.12);
    c->SetTopMargin(0.05);

    h_respM->Draw("COLZ");

    h_respM->SetMinimum(0.001);
    h_respM->SetMaximum(1);

    h_respM->GetXaxis()->SetTitle(xTitle);
    h_respM->GetXaxis()->SetMoreLogLabels();
    h_respM->GetXaxis()->SetNoExponent();
    h_respM->GetXaxis()->SetLabelFont(42);
    h_respM->GetXaxis()->SetLabelSize(0.035);
    h_respM->GetXaxis()->SetTitleSize(0.035);
    h_respM->GetXaxis()->SetTitleFont(42);

    h_respM->GetYaxis()->SetTitle(yTitle);
    h_respM->GetYaxis()->SetMoreLogLabels();
    h_respM->GetYaxis()->SetNoExponent();
    h_respM->GetYaxis()->SetLabelFont(42);
    h_respM->GetYaxis()->SetLabelSize(0.035);
    h_respM->GetYaxis()->SetTitleSize(0.035);
    h_respM->GetYaxis()->SetTitleOffset(2);
    h_respM->GetYaxis()->SetTitleFont(42);

    h_respM->GetZaxis()->SetLabelFont(42);
    h_respM->GetZaxis()->SetLabelSize(0.035);
    h_respM->GetZaxis()->SetTitleSize(0.035);
    h_respM->GetZaxis()->SetTitleFont(42);

    TLatex latex;
    latex.DrawLatexNDC(0.14, 0.96, "#scale[0.8]{#font[62]{CMS }#font[42]{#it{Simulation Supplementary}}}");
    latex.DrawLatexNDC(0.78, 0.96, "#font[62]{#scale[0.7]{13 TeV}}");

    if( channel_ == "muon" )     latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.6]{#mu channel}}");
    if( channel_ == "electron" ) latex.DrawLatexNDC(0.16, 0.91, "#font[62]{#scale[0.6]{e channel}}");

    c->SaveAs(".pdf");
  }

  void Init()
  {
    TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
    if( channel_ == "muon" )
    {
      TString fileName_detRes = rootFilePath+"/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root";
      TString fileName_FSR = rootFilePath+"/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root";

      cout << "[FileName]" << endl;
      cout << "  Det. res.: " << fileName_detRes << endl;
      cout << "  FSR:       " << fileName_FSR << endl;
      cout << endl;

      // -- det. res. -- //
      TH1D* h_truth_detRes   = Get_Hist( fileName_detRes, "h_Truth_RooUnfold" );
      TH2D* h_nEvents_detRes = Get_Hist_2D( fileName_detRes, "h_RespM_RooUnfold" );
      h_respM_detRes_ = CalcFractionPerBin_Transpose( h_nEvents_detRes, h_truth_detRes );

      // -- FSR -- //      
      TH1D* h_truth_FSR   = Get_Hist( fileName_FSR, "h_Truth_RooUnfold" );
      TH2D* h_nEvents_FSR = Get_Hist_2D( fileName_FSR, "h_RespM_RooUnfold" );
      h_respM_FSR_ = CalcFractionPerBin_Transpose( h_nEvents_FSR, h_truth_FSR );
    }
    else if( channel_ == "electron" )
    {
      TString fileName = rootFilePath+"/ROOTFile_RespM_Electron.root";

      cout << "[FileName]" << endl;
      cout << "  Det. res.: " << fileName << endl;
      cout << "  FSR:       " << fileName << endl;
      cout << endl;

      h_respM_detRes_ = Get_Hist_2D( fileName, "h_respM_detRes" );
      h_respM_FSR_    = Get_Hist_2D( fileName, "h_respM_FSR" );
    }
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

        Double_t fraction = 0;
        if( SumN_truth == 0 )
          fraction = 0;
        else
          fraction = nEvent / SumN_truth;
        
        if( fraction < 0 && fabs(fraction) < 1e-3 ) fraction = 0;

        h_response->SetBinContent( i_recobin, i_genbin, fraction );
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

