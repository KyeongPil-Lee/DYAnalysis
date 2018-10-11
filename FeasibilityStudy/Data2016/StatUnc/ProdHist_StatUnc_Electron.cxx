#include "ProdHist_StatUnc.h"

void ProdHist_StatUnc_Electron()
{
	HistProducer *producer = new HistProducer("Electron");
	producer->Produce();
}