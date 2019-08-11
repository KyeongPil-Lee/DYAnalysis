#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <math.h>

void mceff() {

  TString workdir;
  std::vector<TFile*> InputFiles_signal_DY;
  
  int mass[12] = {10,50,100,200,400,500,700,800,1000,1500,2000,3000};

  double xsec[11] = {18610./3,5870./3,226./3,7.67/3,0.423/3,0.24/3,0.035/3,0.03/3,0.016/3,0.002/3,0.00054/3};
  double sumofWts[11] = {771413889185.162476,144505031098.323120,219889705.060318,7008766.904321,122987.746342,69998.861766,11080.956088,9415.627747,4893.463037,664.033593,159.615701};
  
  // workdir = "/tmp/rchawla/eos/cms/store/group/phys_higgs/cmshww/arun/DYAnalysis_76X_Calibrated/DY_Signal/";
  TString dataPath = gSystem->Getenv("KP_DATA_PATH");
  workdir = dataPath + "/";

  InputFiles_signal_DY.clear();

  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_10to50.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_50toInf.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_100to200.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_200to400.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_400to500.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_500to700.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_700to800.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_800to1000.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_1000to1500.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_1500to2000.root"));
  InputFiles_signal_DY.push_back(TFile::Open(workdir+"DY_2000to3000.root"));

  unsigned int nsample = InputFiles_signal_DY.size();
  TFile* file[11];

  for(unsigned int jentry = 0; jentry < nsample; ++jentry) {
    TTree * T1 = (TTree*)InputFiles_signal_DY.at(jentry)->Get("ntupler/ElectronTree");

    vector<float>   *genPostFSR_Pt;
    vector<float>   *genPostFSR_Eta;
    vector<float>   *genPostFSR_Rap;
    vector<float>   *genPostFSR_Phi;
    vector<float>   *genPostFSR_En;
    vector<float>   *genPreFSR_Pt;
    vector<float>   *genPreFSR_Eta;
    vector<float>   *genPreFSR_Rap;
    vector<float>   *genPreFSR_Phi;
    vector<float>   *genPreFSR_En;
    Int_t           tauFlag;
    Double_t        theWeight;

    genPostFSR_Pt = 0;
    genPostFSR_Eta = 0;
    genPostFSR_Rap = 0;
    genPostFSR_Phi = 0;
    genPostFSR_En = 0;
    genPreFSR_Pt = 0;
    genPreFSR_Eta = 0;
    genPreFSR_Rap = 0;
    genPreFSR_Phi = 0;
    genPreFSR_En = 0;

    T1->SetBranchStatus("*",0);
    T1->SetBranchStatus("genPostFSR_Pt", 1);
    T1->SetBranchStatus("genPostFSR_Eta", 1);
    T1->SetBranchStatus("genPostFSR_Rap", 1);
    T1->SetBranchStatus("genPostFSR_Phi", 1);
    T1->SetBranchStatus("genPostFSR_En", 1);
    T1->SetBranchStatus("genPreFSR_Pt", 1);
    T1->SetBranchStatus("genPreFSR_Eta", 1);
    T1->SetBranchStatus("genPreFSR_Rap", 1);
    T1->SetBranchStatus("genPreFSR_Phi", 1);
    T1->SetBranchStatus("genPreFSR_En", 1);
    T1->SetBranchStatus("tauFlag", 1);
    T1->SetBranchStatus("theWeight", 1);

    T1->SetBranchAddress("genPostFSR_Pt", &genPostFSR_Pt);
    T1->SetBranchAddress("genPostFSR_Eta", &genPostFSR_Eta);
    T1->SetBranchAddress("genPostFSR_Rap", &genPostFSR_Rap);
    T1->SetBranchAddress("genPostFSR_Phi", &genPostFSR_Phi);
    T1->SetBranchAddress("genPostFSR_En", &genPostFSR_En);
    T1->SetBranchAddress("genPreFSR_Pt", &genPreFSR_Pt);
    T1->SetBranchAddress("genPreFSR_Eta", &genPreFSR_Eta);
    T1->SetBranchAddress("genPreFSR_Rap", &genPreFSR_Rap);
    T1->SetBranchAddress("genPreFSR_Phi", &genPreFSR_Phi);
    T1->SetBranchAddress("genPreFSR_En", &genPreFSR_En);
    T1->SetBranchAddress("tauFlag", &tauFlag);
    T1->SetBranchAddress("theWeight", &theWeight);

    file[jentry] = new TFile(Form("DYEE_M%dto%d.root",mass[jentry],mass[jentry+1]),"RECREATE");
    TTree *tree = new TTree("tree"," after preselections tree");

    // Branch variable declaration
    double Ele1PT;
    double Ele2PT;
    double Ele1Eta;
    double Ele2Eta;
    double ZMass;
    double lumiWeights;
    double genWeights;

    // Branch declaration
    tree->Branch("Ele1PT", &Ele1PT, "Ele1PT/D"); 
    tree->Branch("Ele2PT", &Ele2PT, "Ele2PT/D");         
    tree->Branch("Ele1Eta", &Ele1Eta, "Ele1Eta/D");         
    tree->Branch("Ele2Eta", &Ele2Eta, "Ele2Eta/D");
    tree->Branch("ZMass", &ZMass, "ZMass/D");
    tree->Branch("lumiWeights", &lumiWeights, "lumiWeights/D");
    tree->Branch("genWeights", &genWeights, "genWeights/D");

    double massGen = 0;
    TLorentzVector gen1,gen2,diGen;
    TLorentzVector ele1,ele2,dielectron;
    vector <double> newgenPt; vector <double> newgenEta; vector <double> newgenEnr; vector <double> newgenPhi;

    double lumi_Weight = xsec[jentry]/sumofWts[jentry];
    cout<<"DY Sample: "<<mass[jentry]<<"to"<<mass[jentry+1]<<endl;

    int nentries = T1->GetEntries();
    //  int nentries = 50;
    cout<<"entries: "<<nentries<<endl;
    for (unsigned int i=0; i < nentries; i++) {
      T1->GetEntry(i);

      if(i%1000000 == 0){
	cout << "Events Processed :  " << i << endl;
      }

      // sorting of gen electrons : postFSR
      int index2[genPostFSR_Pt->size()];
      float pt2[genPostFSR_Pt->size()];

      for(unsigned int b=0; b<genPostFSR_Pt->size(); b++)
      { 
	pt2[b]=genPostFSR_Pt->at(b);
      }
      int sizen = sizeof(pt2)/sizeof(pt2[0]);
      TMath::Sort(sizen,pt2,index2,true);

      // sorting of gen electrons : preFSR
      int index3[genPreFSR_Pt->size()];
      float pt3[genPreFSR_Pt->size()];
      for(unsigned int c=0; c<genPreFSR_Pt->size(); c++)
      { pt3[c]=genPreFSR_Pt->at(c);}
      int size1 = sizeof(pt3)/sizeof(pt3[0]);
      TMath::Sort(size1,pt3,index3,true);

      newgenPt.clear(); newgenEta.clear(); newgenEnr.clear(); newgenPhi.clear();

      if(genPreFSR_Pt->size() == 2){
	gen1.SetPtEtaPhiE(genPreFSR_Pt->at(0),genPreFSR_Eta->at(0),genPreFSR_Phi->at(0),genPreFSR_En->at(0));
	gen2.SetPtEtaPhiE(genPreFSR_Pt->at(1),genPreFSR_Eta->at(1),genPreFSR_Phi->at(1),genPreFSR_En->at(1));
	diGen=gen1+gen2;
	massGen=diGen.M();
      }

      if(jentry==1 && massGen >= 100.) continue;

      if(!tauFlag && genPreFSR_Pt->size() == 2){

	  for(unsigned int j=0;j<genPostFSR_Eta->size();j++){
	    if(fabs(genPostFSR_Eta->at(index2[j])) < 2.5 && !(fabs(genPostFSR_Eta->at(index2[j])) > 1.4442 && fabs(genPostFSR_Eta->at(index2[j])) < 1.566)){
	      newgenPt.push_back(genPostFSR_Pt->at(index2[j]));
	      newgenEta.push_back(genPostFSR_Eta->at(index2[j]));
	      newgenPhi.push_back(genPostFSR_Phi->at(index2[j]));
	      newgenEnr.push_back(genPostFSR_En->at(index2[j]));
	    } // eta
	  } // for loop

	//cout<<"2"<<endl;
	if(newgenPt.size() == 2) {
	  if(newgenPt.at(0) > 30. && newgenPt.at(1) > 10.){

	    Ele1PT = newgenPt.at(0);
	    Ele1Eta = newgenEta.at(0);
	    Ele2PT = newgenPt.at(1);
	    Ele2Eta = newgenEta.at(1);

	    ele1.SetPtEtaPhiE(newgenPt.at(0),newgenEta.at(0),newgenPhi.at(0),newgenEnr.at(0));
	    ele2.SetPtEtaPhiE(newgenPt.at(1),newgenEta.at(1),newgenPhi.at(1),newgenEnr.at(1));
	    dielectron=ele1+ele2;
	    ZMass = dielectron.M();
	    genWeights = theWeight;
	    lumiWeights = lumi_Weight;   

	    tree->Fill();
	  } // pt
	} // size of new gen vectors
      } // tauFlag && genPreFSR_Pt->size() == 2
    } // event Loop

    file[jentry]->Write();
    file[jentry]->Close();
    
    cout<<""<<endl;

  } // file Loop
}
