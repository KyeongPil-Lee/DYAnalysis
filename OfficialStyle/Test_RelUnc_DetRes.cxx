#include <Include/SimplePlotTools.h>

void Smoothing( TH1D* h, Double_t lowerEdge, Double_t upperEdge );
TH1D* QuadSum( TH1D* h1, TH1D* h2 );
void Test_RelUnc_DetRes()
{
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName = rootFilePath+"/ROOTFile_SysUnc_DetRes.root";
  TH1D* h_relUnc_syst_unfold    = PlotTool::Get_Hist(fileName, "h_RelSysUnc_Unfolding_Percent");
  TH1D* h_relUnc_syst_momCorr   = PlotTool::Get_Hist(fileName, "h_RelSysUnc_MomCorr_Percent");
  TH1D* h_relUnc_syst_detRes    = PlotTool::Get_Hist(fileName, "h_RelSysUnc_DetRes_Percent");

  PlotTool::HistCanvaswRatio *c_detResAll = new PlotTool::HistCanvaswRatio( "c_relUnc_detResAll_noSmoothing", 1, 1 );
  c_detResAll->Register( h_relUnc_syst_detRes, "Quad. sum (unfolding & mom. corr.)", kRed);
  c_detResAll->Register( h_relUnc_syst_unfold, "Unfolding", kGreen+2);
  c_detResAll->Register( h_relUnc_syst_momCorr, "Momentum Correction", kBlue);
  c_detResAll->SetTitle( "m [GeV]", "Uncertainty (%)", "rato to total");
  c_detResAll->SetLegendPosition( 0.50, 0.32, 0.95, 0.45 );
  c_detResAll->SetRangeY( 1e-1, 2e2 );
  c_detResAll->Latex_CMSPre();
  c_detResAll->Draw("LPSAME");

  TH1D* h_relUnc_syst_unfold_smth = (TH1D*)h_relUnc_syst_unfold->Clone();
  Smoothing(h_relUnc_syst_unfold_smth, 15, 700);

  PlotTool::HistCanvaswRatio *c_unfold = new PlotTool::HistCanvaswRatio( "c_relUnc_unfold_smoothing", 1, 1 );
  c_unfold->Register( h_relUnc_syst_unfold, "Before smoothing", kBlack);
  c_unfold->Register( h_relUnc_syst_unfold_smth, "After smoothing", kRed);
  c_unfold->SetTitle( "m [GeV]", "Unc. from unfolding (%)", "after/before");
  c_unfold->SetLegendPosition( 0.50, 0.32, 0.95, 0.45 );
  c_unfold->SetRangeY( 1e-1, 2e2 );
  c_unfold->Latex_CMSPre();
  c_unfold->Draw("LPSAME");


  TH1D* h_relUnc_syst_momCorr_smth = (TH1D*)h_relUnc_syst_momCorr->Clone();
  Smoothing(h_relUnc_syst_momCorr_smth, 15, 700);

  PlotTool::HistCanvaswRatio *c_momCorr = new PlotTool::HistCanvaswRatio( "c_relUnc_momCorr_smoothing", 1, 1 );
  c_momCorr->Register( h_relUnc_syst_momCorr, "Before smoothing", kBlack);
  c_momCorr->Register( h_relUnc_syst_momCorr_smth, "After smoothing", kRed);
  c_momCorr->SetTitle( "m [GeV]", "Unc. from mom. corr. (%)", "after/before");
  c_momCorr->SetLegendPosition( 0.50, 0.32, 0.95, 0.45 );
  c_momCorr->SetRangeY( 1e-1, 2e2 );
  c_momCorr->Latex_CMSPre();
  c_momCorr->Draw("LPSAME");

  TH1D* h_relUnc_syst_detRes_smth = (TH1D*)h_relUnc_syst_detRes->Clone();
  Smoothing(h_relUnc_syst_detRes_smth, 15, 700);

  PlotTool::HistCanvaswRatio *c_detRes = new PlotTool::HistCanvaswRatio( "c_relUnc_detRes_smoothing", 1, 1 );
  c_detRes->Register( h_relUnc_syst_detRes, "Before smoothing", kBlack);
  c_detRes->Register( h_relUnc_syst_detRes_smth, "After smoothing", kRed);
  c_detRes->SetTitle( "m [GeV]", "Unc. from det. res. (%)", "after/before");
  c_detRes->SetLegendPosition( 0.50, 0.32, 0.95, 0.45 );
  c_detRes->SetRangeY( 1e-1, 2e2 );
  c_detRes->Latex_CMSPre();
  c_detRes->Draw("LPSAME");


  // TH1D* h_quadSumFromSmth = QuadSum( h_relUnc_syst_unfold_smth, h_relUnc_syst_momCorr_smth );
  TH1D* h_quadSumFromSmth = QuadSum( h_relUnc_syst_unfold_smth, h_relUnc_syst_momCorr ); // -- no smoothing on mom. corr.

  PlotTool::HistCanvaswRatio *c_detResAll_UnfoldSmth = new PlotTool::HistCanvaswRatio( "c_relUnc_detResAll_UnfoldSmth", 1, 1 );
  c_detResAll_UnfoldSmth->Register( h_quadSumFromSmth, "Quad. sum (unfolding(smoothed) + mom. corr.)", kRed);
  c_detResAll_UnfoldSmth->Register( h_relUnc_syst_unfold_smth, "Unfolding (smoothed)", kGreen+2);
  c_detResAll_UnfoldSmth->Register( h_relUnc_syst_momCorr, "Momentum Correction", kBlue);
  c_detResAll_UnfoldSmth->SetTitle( "m [GeV]", "Uncertainty (%)", "rato to total");
  c_detResAll_UnfoldSmth->SetLegendPosition( 0.15, 0.74, 0.90, 0.95 );
  c_detResAll_UnfoldSmth->SetRangeY( 1e-1, 2e2 );
  c_detResAll_UnfoldSmth->Latex_CMSPre();
  c_detResAll_UnfoldSmth->Draw("LPSAME");

  // -- quad. sum first & smoothing it vs. smoothing unfolding unc. first & quad. sum.
  PlotTool::HistCanvaswRatio *c_detRes_withquadSumwSmth = new PlotTool::HistCanvaswRatio( "c_relUnc_detRes_SmoothingQuadSum", 1, 1 );
  c_detRes_withquadSumwSmth->Register( h_relUnc_syst_detRes_smth, "Quad. sum first & Smoothing it", kGreen+2);
  c_detRes_withquadSumwSmth->Register( h_quadSumFromSmth, "Smoothing unfolding unc. only & Quad. sum", kBlue);

  c_detRes_withquadSumwSmth->SetTitle( "m [GeV]", "Unc. from det. res. (%)", "Blue/Green");
  c_detRes_withquadSumwSmth->SetLegendPosition( 0.15, 0.32, 0.95, 0.45 );
  c_detRes_withquadSumwSmth->SetRangeY( 1e-1, 2e2 );
  c_detRes_withquadSumwSmth->Latex_CMSPre();
  c_detRes_withquadSumwSmth->Draw("LPSAME");


  TString fileName_unc = rootFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
  TH1D* h_relUnc_syst_eff = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Eff.SF._Percent");
  TH1D* h_relUnc_syst_bkg = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Bkg.Est._Percent");
  TH1D* h_relUnc_syst_FSR = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_FSR_Percent");
  TH1D* h_relUnc_syst_tot = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Tot_Percent");
  
  TH1D* h_relUnc_syst_accPDF = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Acc._Percent");
  TH1D* h_relUnc_lumi        = PlotTool::Get_Hist(fileName_unc, "h_RelLumiUnc_Percent");
  TH1D* h_relUnc_stat        = PlotTool::Get_Hist(fileName_unc, "h_RelStatUnc_Percent");
  TH1D* h_diffXSec           = PlotTool::Get_Hist(fileName_unc, "h_DiffXsec_FSRCorr");


  TH1D* h_relUnc_syst_tot_wQuadSumFromSmth = (TH1D*)h_relUnc_syst_tot->Clone();

  TH1D* h_relTotUnc                  = (TH1D*)h_relUnc_syst_tot->Clone();
  TH1D* h_relTotUnc_wQuadSumFromSmth = (TH1D*)h_relUnc_syst_tot->Clone();

  Int_t nBin = h_relUnc_syst_tot->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    Double_t relUnc_syst_eff = h_relUnc_syst_eff->GetBinContent(i_bin);
    Double_t relUnc_syst_bkg = h_relUnc_syst_bkg->GetBinContent(i_bin);
    Double_t relUnc_syst_FSR = h_relUnc_syst_FSR->GetBinContent(i_bin);

    Double_t relUnc_detRes_quadSumFromSmth = h_quadSumFromSmth->GetBinContent(i_bin);


    // -- new total systematic: switch detector resolution uncertainty with the one after smoothing individual sources
    Double_t quadSum_wQuadSumFromSmth = sqrt(
      relUnc_syst_eff*relUnc_syst_eff +
      relUnc_syst_bkg*relUnc_syst_bkg +
      relUnc_syst_FSR*relUnc_syst_FSR + 
      relUnc_detRes_quadSumFromSmth*relUnc_detRes_quadSumFromSmth );

    h_relUnc_syst_tot_wQuadSumFromSmth->SetBinContent(i_bin, quadSum_wQuadSumFromSmth);

    // -- x-check: relUnc_syst_tot should be same with quadSum_wSmthDetRes
    Double_t relUnc_detRes_smth = h_relUnc_syst_detRes_smth->GetBinContent(i_bin);

    Double_t quadSum_wSmthDetRes = sqrt(
      relUnc_syst_eff*relUnc_syst_eff +
      relUnc_syst_bkg*relUnc_syst_bkg +
      relUnc_syst_FSR*relUnc_syst_FSR + 
      relUnc_detRes_smth*relUnc_detRes_smth );

    Double_t relUnc_syst_tot = h_relUnc_syst_tot->GetBinContent(i_bin);
    Double_t relDiff = ((quadSum_wSmthDetRes - relUnc_syst_tot) / relUnc_syst_tot) * 100;

    // printf("[%02d bin] (syst_tot, quadSum_wSmthDetRes) = (%5.2lf, %5.2lf) -> diff = %lf\n",
    //   i_bin, relUnc_syst_tot, quadSum_wSmthDetRes, relDiff);


    // -- total uncertianty including all uncertainties
    Double_t relUnc_accPDF = h_relUnc_syst_accPDF->GetBinContent(i_bin);
    Double_t relUnc_lumi = h_relUnc_lumi->GetBinContent(i_bin);
    Double_t relUnc_stat = h_relUnc_stat->GetBinContent(i_bin);

    Double_t quadSum_totalUnc = sqrt(
      relUnc_accPDF*relUnc_accPDF +
      relUnc_lumi*relUnc_lumi +
      relUnc_stat*relUnc_stat +
      relUnc_syst_tot*relUnc_syst_tot
      );

    h_relTotUnc->SetBinContent(i_bin, quadSum_totalUnc);

    Double_t diffXSec = h_diffXSec->GetBinContent(i_bin);
    Double_t absTotUnc = (diffXSec * quadSum_totalUnc) / 100.0;
    printf("[%02d bin] (diff. x-sec, abs. total unc.) = (%.1e, %.1e)\n", i_bin, diffXSec, absTotUnc);

    // -- new total unc.: switch detector resolution uncertainty with the one after smoothing individual sources
    Double_t quadSum_totalUnc_wQuadSumFromSmth = sqrt(
      relUnc_accPDF*relUnc_accPDF +
      relUnc_lumi*relUnc_lumi +
      relUnc_stat*relUnc_stat +
      quadSum_wQuadSumFromSmth*quadSum_wQuadSumFromSmth
      );

    h_relTotUnc_wQuadSumFromSmth->SetBinContent(i_bin, quadSum_totalUnc_wQuadSumFromSmth);
  }

  PlotTool::HistCanvaswRatio *c_systTot = new PlotTool::HistCanvaswRatio( "c_relUnc_systTot_differentDetRes", 1, 1 );
  c_systTot->Register( h_relUnc_syst_tot, "Det. Res: Quad. sum first & Smoothing it", kGreen+2);
  c_systTot->Register( h_relUnc_syst_tot_wQuadSumFromSmth, "Det. Res.: Smoothing unfodling unc. first & Quad. sum", kBlue);
  c_systTot->SetTitle( "m [GeV]", "Total systematic (%)", "Blue/Green");
  c_systTot->SetLegendPosition( 0.15, 0.32, 0.95, 0.45 );
  c_systTot->SetRangeY( 1e-1, 2e2 );
  c_systTot->Latex_CMSPre();
  c_systTot->Draw("LPSAME");

  PlotTool::HistCanvaswRatio *c_totUnc = new PlotTool::HistCanvaswRatio( "c_relUnc_totUnc_differentDetRes", 1, 1 );
  c_totUnc->Register( h_relTotUnc, "Det. Res: Quad. sum first & Smoothing it", kGreen+2);
  c_totUnc->Register( h_relTotUnc_wQuadSumFromSmth, "Det. Res.: Smoothing unfolding unc. first & Quad. sum", kBlue);
  c_totUnc->SetTitle( "m [GeV]", "Total uncertainty (%)", "Blue/Green");
  c_totUnc->SetLegendPosition( 0.15, 0.32, 0.95, 0.45 );
  c_totUnc->SetRangeY( 1e-1, 2e2 );
  c_totUnc->Latex_CMSPre();
  c_totUnc->Draw("LPSAME");










  // TString fileName_unc = rootFilePath+"/ROOTFile_DiffXSec_FullUnc.root";
  // TH1D* h_relUnc_syst_detResFromUnc    = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Det.Res_Percent");

  // TString fileName_uncNew = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/Combination/Local/ROOTFile_DiffXSec_FullUnc.root";
  // TH1D* h_relUnc_syst_detResFromUncNew    = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Det.Res_Percent");

  // Int_t nBin = h_relUnc_syst_unfold->GetNbinsX();
  // for(Int_t i=0; i<nBin; i++)
  // {
  //   Int_t i_bin = i+1;

  //   Double_t relUnc_unfold                = h_relUnc_syst_unfold->GetBinContent(i_bin);
  //   Double_t relUnc_momCorr               = h_relUnc_syst_momCorr->GetBinContent(i_bin);
  //   Double_t relUnc_detRes                = h_relUnc_syst_detRes->GetBinContent(i_bin);
  //   Double_t relUnc_detResFromUnc         = h_relUnc_syst_detResFromUnc->GetBinContent(i_bin);
  //   Double_t relUnc_syst_detResFromUncNew = h_relUnc_syst_detResFromUncNew->GetBinContent(i_bin);
  //   printf("[%02d bin] (unfold, momCorr, detRes, detResFromUnc, detResFromUncNew) = (%5.2lf, %5.2lf, %5.2lf, %5.2lf, %5.2lf)\n",
  //     i_bin, relUnc_unfold, relUnc_momCorr, relUnc_detRes, relUnc_detResFromUnc, relUnc_syst_detResFromUncNew);
  // }

  // for(Int_t i=0; i<nBin; i++)
  // {
  //   Int_t i_bin = i+1;

  //   Double_t relUnc_unfold  = h_relUnc_syst_unfold->GetBinContent(i_bin);
  //   Double_t relUnc_momCorr = h_relUnc_syst_momCorr->GetBinContent(i_bin);
  //   Double_t relUnc_quadSum = sqrt( relUnc_unfold*relUnc_unfold + relUnc_momCorr*relUnc_momCorr );

  //   Double_t relUnc_detRes  = h_relUnc_syst_detRes->GetBinContent(i_bin);
  //   Double_t relDiff = ((relUnc_quadSum - relUnc_detRes ) / relUnc_quadSum) * 100;

  //   // printf("[%02d bin] (unfold, momCorr, quadSum, detRes, relDiff(%%)) = (%5.2lf, %5.2lf, %5.2lf, %5.2lf, %lf)\n",
  //   //   i_bin, relUnc_unfold, relUnc_momCorr, relUnc_quadSum, relUnc_detRes, relDiff);
  // }

  // // -- individual smoothing vs. smoothing total only
  // Smoothing( h_relUnc_syst_unfold, 15, 700 );
  // Smoothing( h_relUnc_syst_momCorr, 15, 700 );

  // for(Int_t i=0; i<nBin; i++)
  // {
  //   Int_t i_bin = i+1;

  //   Double_t relUnc_unfold  = h_relUnc_syst_unfold->GetBinContent(i_bin);
  //   Double_t relUnc_momCorr = h_relUnc_syst_momCorr->GetBinContent(i_bin);
  //   Double_t relUnc_quadSum = sqrt( relUnc_unfold*relUnc_unfold + relUnc_momCorr*relUnc_momCorr );

  //   Double_t relUnc_detResSmth = h_relUnc_syst_detResFromUnc->GetBinContent(i_bin);

  //   Double_t relDiff = ((relUnc_detResSmth - relUnc_quadSum ) / relUnc_detResSmth) * 100;

  //   // printf("[%02d bin] (quadSum after smoothing, smoothing quad sum, relDiff(%%)) = (%5.2lf, %5.2lf, %lf)\n",
  //   //   i_bin, relUnc_quadSum, relUnc_detResSmth, relDiff);
  // }

}

void Smoothing( TH1D* h, Double_t lowerEdge, Double_t upperEdge )
{
  h->GetXaxis()->SetRangeUser(lowerEdge, upperEdge);
  h->Smooth(10, "R");

  h->GetXaxis()->SetRangeUser(15, 3000);
}

TH1D* QuadSum( TH1D* h1, TH1D* h2 )
{
  TH1D* h_result = (TH1D*)h1->Clone();

  Int_t nBin = h1->GetNbinsX();
  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;
    Double_t value1 = h1->GetBinContent(i_bin);
    Double_t value2 = h2->GetBinContent(i_bin);

    Double_t quadSum = sqrt( value1*value1 + value2*value2 );

    // printf("[%02d bin] (value1, value2, quadSum) = (%5.2lf, %5.2lf, %5.2lf)\n",
    //   i_bin, value1, value2, quadSum);

    h_result->SetBinContent(i_bin, quadSum);
  }

  return h_result;
}