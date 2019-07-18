#include <ElecChannel/DXSec/DXSecProducer.h>

void MakeDXSec()
{
  DXSecProducer *producer = new DXSecProducer();
  producer->Produce();
  TH1D* h_dXSec_KP = producer->DXSecHist();

  TFile *f_output = TFile::Open("ROOTFile_dXSec.root", "RECREATE");
  f_output->cd();
  h_dXSec_KP->Write();
  f_output->Close();

  // TString fileName_Ridhi = "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/ElecChannel/Code_Ridhi/CentralValue/Dressed/FinalCorr_v11.root";
  // TString histName_Ridhi = "h_xSec_dM_FSRCorr";
  // TH1D* h_dXSec_Ridhi = PlotTool::Get_Hist(fileName_Ridhi, histName_Ridhi);

  // TString canvasName = "c_dXSec_KP_vs_Ridhi";
  // PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
  // canvas->SetTitle("m(ee) [GeV]", "d#sigma/dm [pb/GeV]", "KP/Ridhi");

  // canvas->Register(h_dXSec_Ridhi, "Ridhi", kBlack);
  // canvas->Register(h_dXSec_KP,    "KP",    kBlue);

  // canvas->SetRangeY(1e-7, 1e3);
  // canvas->SetLegendPosition(0.75, 0.75, 0.95, 0.95);

  // canvas->Latex_CMSPre();

  // canvas->Draw();
}