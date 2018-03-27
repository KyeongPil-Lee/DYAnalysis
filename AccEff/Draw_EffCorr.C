#include "/home/kplee/CommonCodes/DrellYanAnalysis/MyCanvas.C"

void Draw_EffCorr()
{
	TFile *f_input = TFile::Open("ROOTFile_AccEff.root"); f_input->cd();
	TGraphAsymmErrors *g_EffSF_HLTv4p2 = (TGraphAsymmErrors*)f_input->Get("g_EffCorr_HLTv4p2")->Clone();
	TGraphAsymmErrors *g_EffSF_HLTv4p3 = (TGraphAsymmErrors*)f_input->Get("g_EffCorr_HLTv4p3")->Clone();

	vector< TGraphAsymmErrors* > Graphs; vector< TString > Names;
	Graphs.push_back( g_EffSF_HLTv4p2 ); Names.push_back( "EffSF_HLTv4p2");
	Graphs.push_back( g_EffSF_HLTv4p3 ); Names.push_back( "EffSF_HLTv4p3");

	MyCanvas *myc = new MyCanvas("c_EffSF_HLTv4p2_vs_HLTv4p3", "Dimuon Mass [GeV]", "Efficiency SF");
	myc->SetLogx(1);
	myc->SetYRange(0.8, 1.1);
	myc->Colors[0] = kOrange;
	myc->Colors[1] = kGreen+1;
	myc->CanvasWithMultipleGraphs( Graphs, Names, "EP");
	myc->PrintCanvas();
}