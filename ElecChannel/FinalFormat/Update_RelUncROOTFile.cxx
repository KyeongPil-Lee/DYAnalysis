#include <ElecChannel/Include/DYElectronTool.h>

class ROOTFileUpdater
{
public:
  TString fileName_old_;
  TString fileName_new_;

  TString histName_effSF_;
  TString histName_totSyst_;

  TH1D* h_relUnc_effSF_old_;
  TH1D* h_relUnc_effSF_new_;

  TH1D* h_relUnc_totSyst_old_;
  TH1D* h_relUnc_totSyst_new_;

  ROOTFileUpdater()
  {
    TH1::AddDirectory(kFALSE);

  }

  void Set_FileName(TString fileName_old, TString fileName_new)
  {
    fileName_old_ = fileName_old;
    fileName_new_ = fileName_new;
  }

  void Set_HistName_EffSF(TString histName_effSF)
  {
    histName_effSF_ = histName_effSF;
  }

  void Set_HistName_TotSyst(TString histName_totSyst)
  {
    histName_totSyst_ = histName_totSyst;
  }

  void Set_RelUncHist_EffSF_New( TH1D* h_new )
  {
    h_relUnc_effSF_new_ = (TH1D*)h_new->Clone();
  }

  void Update()
  {
    Init();

    gSystem->CopyFile(fileName_old_, fileName_new_, kTRUE);

    DYTool::ReplaceObject(fileName_new_, histName_effSF_,   h_relUnc_effSF_new_);
    DYTool::ReplaceObject(fileName_new_, histName_totSyst_, h_relUnc_totSyst_new_);
  }

  // -- bring the histogram again from the root file to confirm that it is really updated
  void Validate()
  {
    TH1D* h_relUnc_effSF_old = PlotTool::Get_Hist(fileName_old_, histName_effSF_);
    TH1D* h_relUnc_effSF_new = PlotTool::Get_Hist(fileName_new_, histName_effSF_);

    Comparison_RelUnc("effSF", h_relUnc_effSF_old, h_relUnc_effSF_new);


    TH1D* h_relUnc_totSyst_old = PlotTool::Get_Hist(fileName_old_, histName_totSyst_);
    TH1D* h_relUnc_totSyst_new = PlotTool::Get_Hist(fileName_new_, histName_totSyst_);

    Comparison_RelUnc("totSyst", h_relUnc_totSyst_old, h_relUnc_totSyst_new);
  }

private:
  void Comparison_RelUnc(TString type, TH1D* h_old, TH1D* h_new)
  {
    TString canvasName = "c_relUnc_"+type;
    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
    canvas->SetTitle("m (ee) [GeV]", "Rel. unc. [%]", "new/old");

    canvas->Register(h_old, "Old", kBlack);
    canvas->Register(h_new, "New", kRed);
    if( type == "effSF" )   canvas->SetRangeY(1, 20);
    if( type == "totSyst" ) canvas->SetRangeY(1, 200);

    canvas->Latex_CMSPre();

    canvas->Draw("HISTLPSAME");
  }
  void Init()
  {
    h_relUnc_effSF_old_ = PlotTool::Get_Hist(fileName_old_, histName_effSF_);
    h_relUnc_totSyst_old_ = PlotTool::Get_Hist(fileName_old_, histName_totSyst_);

    h_relUnc_totSyst_new_ = UpdateRelUnc_TotSyst();
  }

  TH1D* UpdateRelUnc_TotSyst()
  {
    TH1D* h_relUnc_totSyst_new = (TH1D*)h_relUnc_totSyst_old_->Clone();

    Int_t nBin = h_relUnc_totSyst_old_->GetNbinsX();

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t relUnc_effSF_old   = h_relUnc_effSF_old_->GetBinContent(i_bin);
      Double_t relUnc_effSF_new   = h_relUnc_effSF_new_->GetBinContent(i_bin);
      Double_t relUnc_totSyst_old = h_relUnc_totSyst_old_->GetBinContent(i_bin);

      Double_t relUnc_totSyst_new = sqrt( 
        relUnc_totSyst_old*relUnc_totSyst_old - 
        relUnc_effSF_old*relUnc_effSF_old +
        relUnc_effSF_new*relUnc_effSF_new ); // -- remove old eff. SF. unc & add new eff. SF. unc.

      h_relUnc_totSyst_new->SetBinContent(i_bin, relUnc_totSyst_new);
    }

    return h_relUnc_totSyst_new;
  }
};

TH1D* NewRelUncHist()
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

  // TString fileName = analyzerPath+"/ElecChannel/Uncertainties/EfficiencySF/ROOTFile_Summary_Unc_EffSF.root";
  TString fileName = analyzerPath+"/ElecChannel/Uncertainties/EfficiencySF/ROOTFile_Unc_EffSF_fromCovM.root"; // -- use the value from the cov. M.
  TString histName = "h_relUnc_tot";

  TH1D* h_relUnc_effSF = PlotTool::Get_Hist(fileName, histName);
  h_relUnc_effSF->Scale(100.0); // -- convert to %

  return h_relUnc_effSF;
}

void Update_RelUncROOTFile()
{
  TString rootfilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName_old = rootfilePath+"/ROOTFile_RelUnc_All_Electron_v4.root";
  TString fileName_new = rootfilePath+"/ROOTFile_RelUnc_All_Electron_v5_byKP.root";

  ROOTFileUpdater* tool = new ROOTFileUpdater();
  tool->Set_FileName( fileName_old, fileName_new );
  tool->Set_HistName_EffSF( "h_RelSysUnc_Eff.SF._Percent" );
  tool->Set_HistName_TotSyst( "h_RelSysUnc_Tot_Percent" );

  TH1D* h_relUnc_effSF_new = NewRelUncHist();
  tool->Set_RelUncHist_EffSF_New( h_relUnc_effSF_new );

  tool->Update();
  tool->Validate();
}