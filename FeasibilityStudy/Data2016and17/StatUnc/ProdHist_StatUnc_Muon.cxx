#include "ProdHist_StatUnc.h"

void ProdHist_StatUnc_Muon()
{
	HistProducer *producer = new HistProducer("Muon");
	producer->Produce();
}