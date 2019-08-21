#include "PlotProducer_BkgRatio.h"
#include "PlotProducer_CovM.h"
#include "PlotProducer_VariousPDFs.h"
#include "PlotProducer_RespM.h"

// -- supplementary plots
void Produce_AllPlots()
{
  // -- data vs. various PDFs
  cout << "[Plots: Data vs. theory from various PDFs]" << endl;
  // PlotProducer_VariousPDFs* plotProducer_variousPDFs_muon     = new PlotProducer_VariousPDFs("muon");
  // plotProducer_variousPDFs_muon->Produce();

  // PlotProducer_VariousPDFs* plotProducer_variousPDFs_electron = new PlotProducer_VariousPDFs("electron");
  // plotProducer_variousPDFs_electron->Produce();

  PlotProducer_VariousPDFs* plotProducer_variousPDFs_combined = new PlotProducer_VariousPDFs("combined");
  plotProducer_variousPDFs_combined->Produce();
  cout << endl;


  // -- background ratio
  cout << "[Plots: Background ratio]" << endl;
  PlotProducer_BkgRatio *plotProducer_bkgRatio_muon = new PlotProducer_BkgRatio("muon");
  plotProducer_bkgRatio_muon->Produce();

  PlotProducer_BkgRatio *plotProducer_bkgRatio_electron = new PlotProducer_BkgRatio("electron");
  plotProducer_bkgRatio_electron->Produce();
  cout << endl;


  // -- covariance matrix
  cout << "[Plots: Covariance matrices]" << endl;
  PlotProducer_CovM *plotProducer_covM_muon = new PlotProducer_CovM("muon");
  plotProducer_covM_muon->Produce();

  PlotProducer_CovM *plotProducer_covM_electron = new PlotProducer_CovM("electron");
  plotProducer_covM_electron->Produce();

  PlotProducer_CovM *plotProducer_covM_combined = new PlotProducer_CovM("combined");
  plotProducer_covM_combined->Produce();
  cout << endl;


  // -- response matrix
  cout << "[Plots: response matrices]" << endl;
  PlotProducer_RespM* plotProducer_respM_muon = new PlotProducer_RespM("muon");
  plotProducer_respM_muon->Produce();

  PlotProducer_RespM* plotProducer_respM_electron = new PlotProducer_RespM("electron");
  plotProducer_respM_electron->Produce();
  cout << endl;

  cout << "[Produce_AllPlots] Done" << endl;
  cout << endl;
}