#include <Include/SimplePlotTools.h>

// -- central value of the efficiency: old
// -- uncertainty on the efficiency: use same relative uncertainty with the new efficiency
// -- keep histograms used in TnPEfficiency.h only
// ---- h_eff_data, h_eff_mc
// ---- h_statUnc_data, h_statUnc_mc
// ---- h_relDiff_bkgChange_data
// ---- h_relDiff_sgnChange_data
// ---- h_relDiff_tagChange_mc
// ---- h_relDiff_nlo_mc

class ConvertTool
{
public:
  TString effType_;

  TString fileName_old_;
  TString fileName_new_;

  TFile *f_output_;

  ConvertTool(TString effType, TString fileName_old, TString fileName_new)
  {
    effType_ = effType;

    fileName_old_ = fileName_old;
    fileName_new_ = fileName_new;
  }

  void Convert()
  {
    TString fileName_output = "ROOTFile_OldEff_NewUnc_"+effType_+".root";
    f_output_ = TFile::Open(fileName_output, "RECREATE");

    Save_CentralValue();
    Save_Uncertainty();

    cout << "[ConvertTool::Convert] file = " << fileName_output << " is created" << endl;
  }

private:
  void Save_CentralValue()
  {
    TH2D* h_eff_data = PlotTool::Get_Hist2D(fileName_old_, "h_eff_data");
    TH2D* h_eff_mc   = PlotTool::Get_Hist2D(fileName_old_, "h_eff_mc");

    f_output_->cd();
    h_eff_data->Write();
    h_eff_mc->Write();
  }

  void Save_Uncertainty()
  {
    // -- convert the absolute uncertainty to have same relative uncertainty from the new eff. results
    TH2D* h_absUnc_stat_data = Convert_NewRelUnc("data", "h_statUnc_data");
    TH2D* h_absUnc_stat_mc   = Convert_NewRelUnc("mc", "h_statUnc_mc");

    TH2D* h_relDiff_bkgChange_data = PlotTool::Get_Hist2D(fileName_new_, "h_relDiff_bkgChange_data");
    TH2D* h_relDiff_sgnChange_data = PlotTool::Get_Hist2D(fileName_new_, "h_relDiff_sgnChange_data");
    TH2D* h_relDiff_tagChange_mc   = PlotTool::Get_Hist2D(fileName_new_, "h_relDiff_tagChange_mc");
    TH2D* h_relDiff_nlo_mc         = PlotTool::Get_Hist2D(fileName_new_, "h_relDiff_nlo_mc");

    f_output_->cd();
    h_absUnc_stat_data->Write();
    h_absUnc_stat_mc->Write();
    h_relDiff_bkgChange_data->Write();
    h_relDiff_sgnChange_data->Write();
    h_relDiff_tagChange_mc->Write();
    h_relDiff_nlo_mc->Write();
  }

  TH2D* Convert_NewRelUnc(TString dataType, TString histName)
  {
    TH2D* h_eff_old = PlotTool::Get_Hist2D(fileName_old_, "h_eff_"+dataType);
    TH2D* h_eff_new = PlotTool::Get_Hist2D(fileName_new_, "h_eff_"+dataType);
    TH2D* h_absUnc_stat_old = PlotTool::Get_Hist2D(fileName_old_, "h_statUnc_"+dataType);
    TH2D* h_absUnc_stat_new = PlotTool::Get_Hist2D(fileName_new_, "h_statUnc_"+dataType);

    TH2D* h_absUnc_stat_return = (TH2D*)h_eff_old->Clone(histName);

    Int_t nEtaBin = h_eff_old->GetNbinsX();
    Int_t nPtBin = h_eff_old->GetNbinsY();

    for(Int_t i_eta=0; i_eta<nEtaBin; i_eta++)
    {
      Int_t i_etaBin = i_eta+1;

      for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
      {
        Int_t i_ptBin = i_pt+1;

        Double_t eff_old = h_eff_old->GetBinContent(i_etaBin, i_ptBin);
        Double_t eff_new = h_eff_new->GetBinContent(i_etaBin, i_ptBin);
        Double_t absUnc_stat_old = h_absUnc_stat_old->GetBinContent(i_etaBin, i_ptBin);
        Double_t absUnc_stat_new = h_absUnc_stat_new->GetBinContent(i_etaBin, i_ptBin);

        Double_t relUnc_new = absUnc_stat_new / eff_new;
        Double_t absUnc_wrtOld = eff_old * relUnc_new;

        printf("[(eta bin, pt bin) = (%d, %d)] stat. unc.: %lf -> %lf (rel. unc. = %lf)\n", i_etaBin, i_ptBin, absUnc_stat_old, absUnc_wrtOld, relUnc_new);

        h_absUnc_stat_return->SetBinContent(i_etaBin, i_ptBin, absUnc_wrtOld);
        h_absUnc_stat_return->SetBinError(i_etaBin, i_ptBin, 0);
      }
    }

    return h_absUnc_stat_return;
  }
};

void Convert_RecoEff()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_old = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult/ROOTFile_RecoSF_SMP17001.root";
  TString fileName_new = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult/ROOTFile_RecoSF_April2019.root";

  ConvertTool* tool = new ConvertTool("reco", fileName_old, fileName_new);
  tool->Convert();
}

void Convert_IDEff()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName_old = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult/ROOTFile_IDSF_SMP17001.root";
  TString fileName_new = analyzerPath+"/ElecChannel/EfficiencySF/TnPResult/ROOTFile_IDSF_April2019.root";

  ConvertTool* tool = new ConvertTool("ID", fileName_old, fileName_new);
  tool->Convert();
}

void ConvertROOTFile_OldEff()
{
  Convert_RecoEff();
  Convert_IDEff();
}


