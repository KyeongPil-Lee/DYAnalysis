#include <Include/PlotTools.h>

class PlotProducer_BkgRatio
{
public:
  TString channel_ = "";
  TString fileName_ = "";

  TH1D* h_ratio_fakes_;
  TH1D* h_ratio_ttbarLike_;
  TH1D* h_ratio_ZZWZ_;
  TH1D* h_ratio_DYtautau_;
  TH1D* h_ratio_totBkg_;


  PlotProducer_BkgRatio(TString channel) {  channel_ = channel; }

  void Set_FileName(TString fileName) { fileName_ = fileName; }

  void Produce()
  {
    Init();

    gStyle->SetErrorX(0); // -- remove horizontal error bar in the plot
    
    // -- Draw canvas -- //
    HistInfo *Hist_totBkg = new HistInfo( kBlack, "Total", h_ratio_totBkg_ );
    Hist_totBkg->h->SetMarkerStyle( 20 );
    Hist_totBkg->h->SetMarkerSize( 1 );

    HistInfo *Hist_fakes = new HistInfo( kMagenta+2, "QCD + WJets", h_ratio_fakes_ );
    Hist_fakes->h->SetMarkerStyle( 21 );
    Hist_fakes->h->SetMarkerSize( 1 );

    HistInfo *Hist_ttbarLike = new HistInfo( kRed, "t#bar{t} + tW + WW", h_ratio_ttbarLike_ );
    Hist_ttbarLike->h->SetMarkerStyle(22);
    Hist_ttbarLike->h->SetMarkerSize( 1 );

    HistInfo *Hist_ZZWZ = new HistInfo( kGreen, "ZZ + WZ", h_ratio_ZZWZ_ );
    Hist_ZZWZ->h->SetMarkerStyle(23);
    Hist_ZZWZ->h->SetMarkerSize( 1 );

    HistInfo *Hist_DYtautau = new HistInfo( kBlue-9, "DY#rightarrow#tau#tau", h_ratio_DYtautau_ );
    Hist_DYtautau->h->SetMarkerStyle(29);
    Hist_DYtautau->h->SetMarkerSize( 1 );

    TString canvasName = "bkgRatio_"+channel_;
    TCanvas *c;
    SetCanvas_Square(c, canvasName, 1, 1, 800, 700);
    c->cd();

    Hist_totBkg->Draw("EPSAME");
    Hist_fakes->Draw("EPSAME");
    Hist_ttbarLike->Draw("EPSAME");
    Hist_ZZWZ->Draw("EPSAME");
    Hist_DYtautau->Draw("EPSAME");

    TLegend *legend;
    SetLegend(legend, 0.7, 0.15, 0.95, 0.4);
    Hist_totBkg->AddToLegend ( legend );
    Hist_fakes->AddToLegend ( legend );
    Hist_ttbarLike->AddToLegend ( legend );
    Hist_ZZWZ->AddToLegend ( legend );
    Hist_DYtautau->AddToLegend ( legend );
    legend->Draw();

    SetHistFormat_SinglePad( Hist_totBkg->h, "m [GeV]", "Fraction of backgrounds" );
    Hist_totBkg->h->GetYaxis()->SetRangeUser(1e-5, 1);

    TLatex latex;
    latex.DrawLatexNDC(0.14, 0.96, "#scale[0.8]{#font[62]{CMS }#font[42]{#it{Supplementary}}}");
    if( channel_ == "muon" )          latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.8 fb^{-1} (13 TeV)}}");
    else if( channel_ == "electron" ) latex.DrawLatexNDC(0.74, 0.96, "#font[62]{#scale[0.7]{2.3 fb^{-1} (13 TeV)}}");

    if( channel_ == "muon" )     latex.DrawLatexNDC(0.16, 0.91, "#scale[0.6]{#font[62]{#mu channel}}");
    if( channel_ == "electron" ) latex.DrawLatexNDC(0.16, 0.91, "#scale[0.6]{#font[62]{e channel}}");

    c->SaveAs(".pdf");
  }

private:
  void Init()
  {
    if( fileName_ == "" )
    {
      TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

      if( channel_ == "muon" )          fileName_ = rootFilePath+"/ROOTFile_Histograms_DimuonMassSpectrum_All.root";
      else if( channel_ == "electron" ) fileName_ = rootFilePath+"/ROOTFile_Histograms_DielectronMassSpectrum.root";
      else
      {
        cout << "Channel = " << channel_ << " is not recognizable" << endl;
        return;
      }

      cout << "Use DEFAULT root file: " << fileName_ << endl;
    }
    else
      cout << "Use PRIVATE root file: " << fileName_ << endl;


    TH1D* h_data = Get_Hist( fileName_, "h_data" );
    h_data->Sumw2();

    // -- h_fakes -- //
    TH1D* h_WJets = Get_Hist( fileName_, "h_WJets_FR" );
    h_WJets->Sumw2();
    TH1D* h_QCD = Get_Hist( fileName_, "h_diJet_FR" );
    h_QCD->Sumw2();
    TH1D* h_fakes = (TH1D*)h_WJets->Clone();
    h_fakes->Add( h_QCD );

    // -- h_ttbarLike -- //
    TH1D* h_ttbar = Get_Hist( fileName_, "h_ttbar_emu" );
    h_ttbar->Sumw2();
    TH1D* h_tW_emu = Get_Hist( fileName_, "h_tW_emu");
    h_tW_emu->Sumw2();
    TH1D* h_WW = Get_Hist( fileName_, "h_WW_emu" );
    h_WW->Sumw2();
    HotFix_ElectronChannel( h_tW_emu, h_WW );

    TH1D* h_ttbarLike = (TH1D*)h_ttbar->Clone();
    h_ttbarLike->Add( h_tW_emu );
    h_ttbarLike->Add( h_WW );

    // -- h_ZZWZ -- //
    TH1D* h_WZ = Get_Hist( fileName_, "h_WZ" );
    h_WZ->Sumw2();
    TH1D* h_ZZ = Get_Hist( fileName_, "h_ZZ" );
    h_ZZ->Sumw2();
    TH1D* h_ZZWZ = (TH1D*)h_WZ->Clone();
    h_ZZWZ->Add( h_ZZ );

    // -- DY->tautau -- //
    TH1D* h_DYtautau = Get_Hist( fileName_, "h_DYTauTau_emu" );
    h_DYtautau->Sumw2();


    // -- total bkg. -- //
    TH1D* h_totBkg = (TH1D*)h_fakes->Clone();
    h_totBkg->Add( h_ttbarLike );
    h_totBkg->Add( h_ZZWZ );
    h_totBkg->Add( h_DYtautau );

    // -- bkg. ratio -- //
    h_ratio_totBkg_ = (TH1D*)h_data->Clone();
    h_ratio_totBkg_->Divide( h_totBkg, h_data );

    h_ratio_fakes_ = (TH1D*)h_data->Clone();
    h_ratio_fakes_->Divide( h_fakes, h_data );

    h_ratio_ttbarLike_ = (TH1D*)h_data->Clone();
    h_ratio_ttbarLike_->Divide( h_ttbarLike, h_data );

    h_ratio_ZZWZ_ = (TH1D*)h_data->Clone();
    h_ratio_ZZWZ_->Divide( h_ZZWZ, h_data );

    h_ratio_DYtautau_ = (TH1D*)h_data->Clone();
    h_ratio_DYtautau_->Divide( h_DYtautau, h_data );
  }

  void HotFix_ElectronChannel( TH1D* h_tW_emu, TH1D* h_WW )
  {
    if( channel_ == "electron" )
    {
      h_tW_emu->SetBinError(42, 0 );
      h_WW->SetBinError(42, 0 );
    }
  }
};

