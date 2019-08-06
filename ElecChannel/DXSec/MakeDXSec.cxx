#include <ElecChannel/DXSec/DXSecProducer.h>

void MakeDXSec()
{
  DXSecProducer *producer = new DXSecProducer();
  producer->Produce();
  TH1D* h_dXSec_KP      = producer->DXSecHist();
  TH1D* h_FpoF_dXSec_KP = producer->FpoF_DXSecHist();

  TFile *f_output = TFile::Open("ROOTFile_dXSec.root", "RECREATE");
  f_output->cd();
  h_dXSec_KP->Write("h_dXSec");
  h_FpoF_dXSec_KP->Write("h_FpoF_dXSec");
  f_output->Close();
}