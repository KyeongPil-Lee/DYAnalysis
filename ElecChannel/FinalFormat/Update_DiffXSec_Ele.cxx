#include <ElecChannel/Include/DYElectronTool.h>

TH1D* NewDXSec(TString histName)
{
  TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
  TString fileName = analyzerPath+"/ElecChannel/DXSec/ROOTFile_dXSec.root";

  return PlotTool::Get_Hist(fileName, histName);
}

void ComparisonPlot(TString type, TH1D* h_old, TH1D* h_new)
{
  TString canvasName = "c_old_vs_new_"+type;

  TString titleY = "";
  if( type.Contains("dXSec") )  titleY = "d#sigma/dm [pb/GeV]";
  if( type.Contains("relUnc") ) titleY = "Rel. unc [%]";

  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  canvas->SetTitle("m(ee) [GeV]", titleY, "new/old");

  canvas->Register( h_old, "old", kBlack);
  canvas->Register( h_new, "new", kBlue);

  if( type.Contains("relUnc") ) canvas->SetRangeY(1, 150);

  canvas->Draw("HISTLPSAME");
}

void Update_DiffXSec_Ele()
{
  TString rootFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
  TString fileName_old = rootFilePath+"/DiffXsec_Electron_v12_byKP.root";
  TString fileName_new = rootFilePath+"/DiffXsec_Electron_v13_byKP.root";

  gSystem->CopyFile(fileName_old, fileName_new, kTRUE);

  ///////////////////////////////////////////////////
  // -- replace the differential cross sections -- //
  ///////////////////////////////////////////////////
  TH1D* h_dXSec      = NewDXSec("h_dXSec");
  TH1D* h_FpoF_dXSec = NewDXSec("h_FpoF_dXSec");

  DYTool::ReplaceObject(fileName_new, "h_DiffXSec",          h_dXSec);
  DYTool::ReplaceObject(fileName_new, "h_DiffXSec_Fiducial", h_FpoF_dXSec);

  /////////////////////////////////////
  // -- replace the uncertainties -- //
  /////////////////////////////////////
  TString fileName_unc   = rootFilePath+"/ROOTFile_RelUnc_All_Electron_v5_byKP.root"; // -- latest uncertainty w/ new efficinecy SF unc.

  TH1D* h_relUnc_detRes = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Det.Res_Percent");
  TH1D* h_relUnc_bkg    = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Bkg.Est._Percent");
  TH1D* h_relUnc_FSR    = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_FSR_Percent");
  TH1D* h_relUnc_acc    = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Acc._Percent");
  TH1D* h_relUnc_effSF  = PlotTool::Get_Hist(fileName_unc, "h_RelSysUnc_Eff.SF._Percent");

  vector<TH1D*> vec_relUnc_syst = {h_relUnc_detRes, h_relUnc_bkg, h_relUnc_FSR, h_relUnc_acc, h_relUnc_effSF};
  TH1D* h_relUnc_syst = PlotTool::QuadSum_HistVector( "h_relUnc_syst", vec_relUnc_syst );

  vector<TH1D*> vec_FpoF_relUnc_syst = {h_relUnc_detRes, h_relUnc_bkg, h_relUnc_effSF};
  TH1D* h_FpoF_relUnc_syst = PlotTool::QuadSum_HistVector( "h_FpoF_relUnc_syst", vec_FpoF_relUnc_syst );

  DYTool::ReplaceObject(fileName_new, "h_RelUnc_Syst",          h_relUnc_syst);
  DYTool::ReplaceObject(fileName_new, "h_RelUnc_Syst_Fiducial", h_FpoF_relUnc_syst);


  // -- comparison plots
  TH1D* h_dXSec_old      = PlotTool::Get_Hist(fileName_old, "h_DiffXSec");
  TH1D* h_dXSec_new      = PlotTool::Get_Hist(fileName_new, "h_DiffXSec");
  ComparisonPlot("dXSec", h_dXSec_old, h_dXSec_new);

  TH1D* h_FpoF_dXSec_old = PlotTool::Get_Hist(fileName_old, "h_DiffXSec_Fiducial");
  TH1D* h_FpoF_dXSec_new = PlotTool::Get_Hist(fileName_new, "h_DiffXSec_Fiducial");
  ComparisonPlot("FpoF_dXSec", h_FpoF_dXSec_old, h_FpoF_dXSec_new);

  TH1D* h_relUnc_syst_old = PlotTool::Get_Hist(fileName_old, "h_RelUnc_Syst");
  TH1D* h_relUnc_syst_new = PlotTool::Get_Hist(fileName_new, "h_RelUnc_Syst");
  ComparisonPlot("relUnc_syst", h_relUnc_syst_old, h_relUnc_syst_new);

  TH1D* h_FpoF_relUnc_syst_old = PlotTool::Get_Hist(fileName_old, "h_RelUnc_Syst_Fiducial");
  TH1D* h_FpoF_relUnc_syst_new = PlotTool::Get_Hist(fileName_new, "h_RelUnc_Syst_Fiducial");
  ComparisonPlot("FpoF_relUnc_syst", h_FpoF_relUnc_syst_old, h_FpoF_relUnc_syst_new);
}

  // KEY: TH1D h_DiffXSec;1  
  // KEY: TH1D h_RelUnc_Stat;1 
  // KEY: TH1D h_RelUnc_Syst;1 
  // KEY: TH1D h_DiffXSec_FEWZ;1 
  // KEY: TH1D h_DiffXSec_MCNLO;1  
  // KEY: TH1D h_DiffXSec_Fiducial;1 
  // KEY: TH1D h_RelUnc_Stat_Fiducial;1  
  // KEY: TH1D h_RelUnc_Syst_Fiducial;1  
  // KEY: TH1D h_DiffXSec_MCNLO_Fiducial;1