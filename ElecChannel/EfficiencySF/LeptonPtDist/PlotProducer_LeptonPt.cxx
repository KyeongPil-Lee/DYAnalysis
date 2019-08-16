#include <ElecChannel/Include/DYElectronTool.h>

#define nPtBin 5

class PlotProducer
{
public:
  Double_t arr_ptBinEdge[nPtBin+1] = {10, 20, 30, 40, 50, 200};

  Int_t nMassBin_;
  vector<Double_t> vec_massBinEdge_ = {76, 81, 86, 91, 96, 101, 106};

  vector<TH2D*> vec_hist2D_leptonPt_;

  PlotProducer()
  {
    Init();
  }

  void Produce()
  {
    ProducePtHist();
    Save();
  }

private:
  void Init()
  {
    nMassBin_ = (Int_t)vec_massBinEdge_.size() - 1;
    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      TString massBinInfo = TString::Format("m%0.lfto%.0lf", vec_massBinEdge_[i_mass], vec_massBinEdge_[i_mass+1]);
      TString histName = "h2D_leptonPt_"+massBinInfo;
      TH2D* h_temp = new TH2D(histName, "", nPtBin, arr_ptBinEdge, nPtBin, arr_ptBinEdge);

      vec_hist2D_leptonPt_.push_back( h_temp );
    }
  }

  Int_t FindMassIndex(Double_t mass)
  {
    // cout << "[FindMassIndex] mass = " << mass << endl;
    Int_t theIndex = -1;

    if( mass < vec_massBinEdge_[0] )         return theIndex;
    if( mass > vec_massBinEdge_[nMassBin_] ) return nMassBin_-1;

    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      if( vec_massBinEdge_[i_mass] <= mass && mass < vec_massBinEdge_[i_mass+1] )
      {        
        theIndex = i_mass;
        break;
      }
    }

    return theIndex;
  }

  void ProducePtHist()
  {
    TString dataPath = gSystem->Getenv("KP_DATA_PATH");
    TFile *f_ntuple = TFile::Open(dataPath+"/DY_forEff_M10to3000.root");
    Double_t pt1, pt2, eta1, eta2, diM, lumiWeight, genWeight;

    TTree *ntuple;
    ntuple = (TTree*)f_ntuple->Get("tree");
    Long64_t nTotEvent = ntuple->GetEntries();
    cout << "Total # events: "<< nTotEvent << endl;
    cout << endl;

    ntuple->SetBranchAddress("Ele1PT",&pt1);
    ntuple->SetBranchAddress("Ele1Eta",&eta1);
    ntuple->SetBranchAddress("Ele2PT",&pt2);
    ntuple->SetBranchAddress("Ele2Eta",&eta2);
    ntuple->SetBranchAddress("ZMass",&diM);
    ntuple->SetBranchAddress("lumiWeights",&lumiWeight);
    ntuple->SetBranchAddress("genWeights",&genWeight);

    for(Long64_t i_ev = 0; i_ev < nTotEvent; i_ev++)
    {
      DYTool::loadBar(i_ev+1, nTotEvent, 100, 100);

      if( genWeight < 0 ) genWeight = -1.0;
      else                genWeight = 1.0;

      ntuple->GetEntry(i_ev);

      Int_t massIndex = FindMassIndex(diM);
      if( massIndex == -1 ) continue;

      Double_t pt_lead, pt_sub;
      if( pt1 > pt2 )
      {
        pt_lead = pt1;
        pt_sub = pt2;
      }
      else
      {
        pt_lead = pt2;
        pt_sub = pt1;
      }

      vec_hist2D_leptonPt_[massIndex]->Fill( pt_lead, pt_sub, lumiWeight*genWeight);
    }

    cout << "[PlotProducer::ProducePtHist] Done" << endl;
  }

  void Save()
  {
    TString fileName = "ROOTFile_PlotProducer_LeptonPt.root";
    TFile *f_output = TFile::Open(fileName, "RECREATE");
    f_output->cd();
    for(const auto& h2D : vec_hist2D_leptonPt_ )
    {
      h2D->Write();

      TH2D* h2D_norm = (TH2D*)h2D->Clone();
      TString histName = h2D->GetName();
      histName.ReplaceAll("h2D_", "h2DNorm_");
      h2D_norm->SetName(histName);

      Double_t nTotEvent = h2D_norm->Integral();
      h2D_norm->Scale( 1.0 / nTotEvent );
      h2D_norm->Write();
    }
    f_output->Close();
    cout << "Output file: " << fileName << endl;
  }

};

void PlotProducer_LeptonPt()
{
  PlotProducer* producer = new PlotProducer();
  producer->Produce();
}