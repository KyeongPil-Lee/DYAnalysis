#include <Include/SimplePlotTools.h>

class ComparisonTool
{
public:
  TString type_;

  vector<Double_t> vec_ptBinEdges_17001_;
  vector<Double_t> vec_etaBinEdges_17001_;

  vector<Double_t> vec_ptBinEdges_16013_;
  vector<Double_t> vec_etaBinEdges_16013_;

  TH1D* h_pt20to30_reco_17001_;
  TH1D* h_pt30to40_reco_17001_;
  TH1D* h_pt40to50_reco_17001_;
  TH1D* h_ptAbove50_reco_17001_;

  TH1D* h_pt20to30_ID_17001_;
  TH1D* h_pt30to40_ID_17001_;
  TH1D* h_pt40to50_ID_17001_;
  TH1D* h_ptAbove50_ID_17001_;

  TH1D* h_pt25to40_16013_;
  TH1D* h_pt40to55_16013_;
  TH1D* h_ptAbove55_16013_;

  ComparisonTool(TString type)
  {
    type_ = type;
    Init();
  }

  void Produce()
  {
    Comparison_Pt25to40("pt20to30");
    Comparison_Pt25to40("pt30to40");

    Comparison_Pt40to55();
    Comparison_PtAbove55();
  }

private:
  void Comparison_Pt25to40(TString ptRange_17001)
  {
    TGraphAsymmErrors* g_reco_17001 = nullptr;
    if( ptRange_17001 == "pt20to30" ) g_reco_17001 = Convert_HistToGraph_noECALGAP( h_pt20to30_reco_17001_ );
    if( ptRange_17001 == "pt30to40" ) g_reco_17001 = Convert_HistToGraph_noECALGAP( h_pt30to40_reco_17001_ );

    TGraphAsymmErrors* g_ID_17001 = nullptr;
    if( ptRange_17001 == "pt20to30" ) g_ID_17001 = Convert_HistToGraph_noECALGAP( h_pt20to30_ID_17001_ );
    if( ptRange_17001 == "pt30to40" ) g_ID_17001 = Convert_HistToGraph_noECALGAP( h_pt30to40_ID_17001_ );

    TGraphAsymmErrors* g_16013 = Convert_HistToGraph_noECALGAP( h_pt25to40_16013_ );

    TString canvasName = "c_"+type_+"_Pt25to40_vs_"+ptRange_17001;
    PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);
    canvas->SetTitle("#eta(e)", "Rel. unc.");

    TString legend_reco_17001 = "";
    if( ptRange_17001 == "pt20to30" ) legend_reco_17001 = "SMP-17-001 (Reco., 20 < p_{T} < 30 GeV)";
    if( ptRange_17001 == "pt30to40" ) legend_reco_17001 = "SMP-17-001 (Reco., 30 < p_{T} < 40 GeV)";

    TString legend_ID_17001 = "";
    if( ptRange_17001 == "pt20to30" ) legend_ID_17001 = "SMP-17-001 (ID, 20 < p_{T} < 30 GeV)";
    if( ptRange_17001 == "pt30to40" ) legend_ID_17001 = "SMP-17-001 (ID, 30 < p_{T} < 40 GeV)";

    canvas->Register( g_reco_17001, legend_reco_17001, kBlack);
    canvas->Register( g_ID_17001,   legend_ID_17001,   kBlue);
    canvas->Register( g_16013, "SMP-16-013 (Reco.+ID, 25 < p_{T} < 40 GeV)", kRed);

    canvas->SetRangeY(0.0, 0.08);
    canvas->SetLegendPosition( 0.40, 0.76, 0.95, 0.91 );

    canvas->Latex_CMSPre();
    TString uncInfo = "";
    if( type_ == "sgnChange" ) uncInfo = "Uncertainty from signal shape";
    if( type_ == "bkgChange" ) uncInfo = "Uncertainty from background shape";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+uncInfo+"}}");

    canvas->Draw("LP");
  }

  void Comparison_Pt40to55()
  {
    TGraphAsymmErrors* g_reco_17001 = Convert_HistToGraph_noECALGAP( h_pt40to50_reco_17001_ );
    TGraphAsymmErrors* g_ID_17001   = Convert_HistToGraph_noECALGAP( h_pt40to50_ID_17001_ );
    TGraphAsymmErrors* g_16013 = Convert_HistToGraph_noECALGAP( h_pt40to55_16013_ );

    TString canvasName = "c_"+type_+"_Pt40to55_vs_Pt40to50";
    PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);
    canvas->SetTitle("#eta(e)", "Rel. unc.");

    canvas->Register( g_reco_17001, "SMP-17-001 (Reco., 40 < p_{T} < 50 GeV)",    kBlack);
    canvas->Register( g_ID_17001,   "SMP-17-001 (ID, 40 < p_{T} < 50 GeV)",       kBlue);
    canvas->Register( g_16013,      "SMP-16-013 (Reco.+ID, 40 < p_{T} < 55 GeV)", kRed);

    canvas->SetRangeY(0.0, 0.08);
    canvas->SetLegendPosition( 0.40, 0.76, 0.95, 0.91 );

    canvas->Latex_CMSPre();
    TString uncInfo = "";
    if( type_ == "sgnChange" ) uncInfo = "Uncertainty from signal shape";
    if( type_ == "bkgChange" ) uncInfo = "Uncertainty from background shape";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+uncInfo+"}}");

    canvas->Draw("LP");
  }

  void Comparison_PtAbove55()
  {
    TGraphAsymmErrors* g_reco_17001 = Convert_HistToGraph_noECALGAP( h_ptAbove50_reco_17001_ );
    TGraphAsymmErrors* g_ID_17001   = Convert_HistToGraph_noECALGAP( h_ptAbove50_ID_17001_ );
    TGraphAsymmErrors* g_16013 = Convert_HistToGraph_noECALGAP( h_ptAbove55_16013_ );

    TString canvasName = "c_"+type_+"_PtAbove55_vs_PtAbove50";
    PlotTool::GraphCanvas *canvas = new PlotTool::GraphCanvas(canvasName, 0, 0);
    canvas->SetTitle("#eta(e)", "Rel. unc.");

    canvas->Register( g_reco_17001, "SMP-17-001 (Reco., p_{T} > 50 GeV)",    kBlack);
    canvas->Register( g_ID_17001,   "SMP-17-001 (ID, p_{T} > 50 GeV)",       kBlue);
    canvas->Register( g_16013,      "SMP-16-013 (Reco.+ID, p_{T} > 55 GeV)", kRed);

    canvas->SetRangeY(0, 0.08);
    canvas->SetLegendPosition( 0.40, 0.76, 0.95, 0.91 );

    canvas->Latex_CMSPre();

    TString uncInfo = "";
    if( type_ == "sgnChange" ) uncInfo = "Uncertainty from signal shape";
    if( type_ == "bkgChange" ) uncInfo = "Uncertainty from background shape";
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+uncInfo+"}}");

    canvas->Draw("LP");
  }

  void Init()
  {
    Init_SMP17001();
    Init_SMP16013();
  }

  void Init_SMP17001()
  {
    TString fileName_reco = "ROOTFile_RecoSF_Electron.root";
    TString fileName_ID   = "ROOTFile_IDSF_Electron.root";

    vector<Double_t> vec_ptBinEdges_temp = {10, 20, 30, 40, 50, 2000.0};
    vec_ptBinEdges_17001_ = vec_ptBinEdges_temp;

    vector<Double_t> vec_etaBinEdges_temp = {-2.5, -2.0, -1.566, -1.4442, -0.800, 0, 0.800, 1.4442, 1.566, 2.0, 2.5};
    vec_etaBinEdges_17001_ = vec_etaBinEdges_temp;

    TString histName_base = "h_relDiff_"+type_;

    h_pt20to30_reco_17001_ = PlotTool::Get_Hist(fileName_reco, histName_base+"_data_pt20to30");
    TakeAbsoluteValue( h_pt20to30_reco_17001_ );

    h_pt30to40_reco_17001_ = PlotTool::Get_Hist(fileName_reco, histName_base+"_data_pt30to40");
    TakeAbsoluteValue( h_pt30to40_reco_17001_ );

    h_pt40to50_reco_17001_ = PlotTool::Get_Hist(fileName_reco, histName_base+"_data_pt40to50");
    TakeAbsoluteValue( h_pt40to50_reco_17001_ );

    h_ptAbove50_reco_17001_ = PlotTool::Get_Hist(fileName_reco, histName_base+"_data_pt50to2000");
    TakeAbsoluteValue( h_ptAbove50_reco_17001_ );

    h_pt20to30_ID_17001_ = PlotTool::Get_Hist(fileName_ID, histName_base+"_data_pt20to30");
    TakeAbsoluteValue( h_pt20to30_ID_17001_ );

    h_pt30to40_ID_17001_ = PlotTool::Get_Hist(fileName_ID, histName_base+"_data_pt30to40");
    TakeAbsoluteValue( h_pt30to40_ID_17001_ );

    h_pt40to50_ID_17001_ = PlotTool::Get_Hist(fileName_ID, histName_base+"_data_pt40to50");
    TakeAbsoluteValue( h_pt40to50_ID_17001_ );

    h_ptAbove50_ID_17001_ = PlotTool::Get_Hist(fileName_ID, histName_base+"_data_pt50to2000");
    TakeAbsoluteValue( h_ptAbove50_ID_17001_ );
  }

  void TakeAbsoluteValue( TH1D* h )
  {
    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t value = h->GetBinContent(i_bin);
      if( value < 0 ) h->SetBinContent(i_bin, fabs(value));
    }
  }

  void Init_SMP16013()
  {
    TString fileName = "SMP16013/ROOTFile_SMP16013_EffUnc_eChannel.root";

    vector<Double_t> vec_ptBinEdges_temp = {25., 40., 55, 8000.};
    vec_ptBinEdges_16013_ = vec_ptBinEdges_temp;

    vector<Double_t> vec_etaBinEdges_temp = {-2.5, -2.0, -1.566, -1.4442, -1., -0.5, 0, 0.5, 1., 1.4442, 1.566, 2.0, 2.5};
    vec_etaBinEdges_16013_ = vec_etaBinEdges_temp;

    TString histName_base = "h_relUnc_"+type_;

    h_pt25to40_16013_  = PlotTool::Get_Hist(fileName, histName_base+"_pt25to40");
    h_pt40to55_16013_  = PlotTool::Get_Hist(fileName, histName_base+"_pt40to55");
    h_ptAbove55_16013_ = PlotTool::Get_Hist(fileName, histName_base+"_pt55to8000");
  }

  TGraphAsymmErrors* Convert_HistToGraph_noECALGAP(TH1D* h)
  {
    TGraphAsymmErrors* g = new TGraphAsymmErrors();

    Int_t nBin = h->GetNbinsX();

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t binLowEdge = h->GetBinLowEdge(i_bin);
      if( binLowEdge == -1.566 || binLowEdge == 1.4442 ) continue; // -- do not include ECAL gap: meaningless values

      Double_t x     = h->GetBinCenter(i_bin);
      Double_t xErr  = h->GetBinWidth(i_bin) / 2.0;

      Double_t y     = h->GetBinContent(i_bin);
      Double_t yErr  = h->GetBinError(i_bin);

      // printf("(i, x, y) = (%d, %lf, %lf)\n", i, x, y);

      g->SetPoint(g->GetN(), x, y);
      g->SetPointError(g->GetN()-1, xErr, xErr, yErr, yErr);
    }

    return g;
  }

};

void Comparison_SMP17001_16013()
{
  ComparisonTool* tool_sgnChange = new ComparisonTool("sgnChange");
  tool_sgnChange->Produce();

  ComparisonTool* tool_bkgChange = new ComparisonTool("bkgChange");
  tool_bkgChange->Produce();
}