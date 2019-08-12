#include <Include/SimplePlotTools.h>

void Summary_Unc_EffSF()
{
  vector<TString> vec_uncType    = {"tot", "bkgChange", "sgnChange", "tagChange", "nlo", "stat"};
  vector<TString> vec_legendInfo = {"Total", "Background shape", "Signal shape", "Tag selection", "MC sample", "Stat."};
  vector<Int_t> vec_color        = {kRed, kOrange+2, kGreen+2, kViolet, kCyan, kBlack};

  vector<TH1D*> vec_hist_relUnc;

  Int_t nUncType = (Int_t)vec_uncType.size();
  for(Int_t i=0; i<nUncType; i++)
  {
    TString uncType = vec_uncType[i];
    if( uncType == "tot" ) continue; // -- will be inserted later

    TString fileName = "ROOTFile_UncFromGaussianFit_" + uncType + ".root";
    TH1D* h_relUnc = PlotTool::Get_Hist(fileName, "h_variance_"+uncType);
    h_relUnc->Scale(100.0); // -- convert to %

    vec_hist_relUnc.push_back( h_relUnc );
  }

  TH1D* h_relUnc_tot = PlotTool::QuadSum_HistVector( "h_relUnc_tot", vec_hist_relUnc );
  vec_hist_relUnc.insert( vec_hist_relUnc.begin(), h_relUnc_tot );

  TString canvasName = "c_summary_relUnc_effSF";
  PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 1, 0);
  canvas->SetTitle("m(ee) [GeV]", "Rel. unc. [%]");
  canvas->SetRangeY(0, 12);

  for(Int_t i=0; i<nUncType; i++)
    canvas->Register( vec_hist_relUnc[i], vec_legendInfo[i], vec_color[i] );

  canvas->Latex_CMSPre();
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{Uncertainty from the efficiency SF}}");

  canvas->SetLegendPosition( 0.50, 0.60, 0.95, 0.90 );

  canvas->Draw("HISTLP");

  // -- save in one output file
  TString fileName_output = "ROOTFile_Summary_Unc_EffSF.root";
  TFile* f_output = TFile::Open(fileName_output, "RECREATE");
  f_output->cd();
  for(const auto& h_relUnc : vec_hist_relUnc )
  {
    TString histName = h_relUnc->GetName();
    histName.ReplaceAll("h_variance", "h_relUnc");
    h_relUnc->SetName(histName);

    h_relUnc->Scale(1.0 / 100.0);

    h_relUnc->Write();
  }
  f_output->Close();
}