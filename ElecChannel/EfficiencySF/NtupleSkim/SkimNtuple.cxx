#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <math.h>

Double_t deltaPhi(Double_t phi1, Double_t phi2)
{
  Double_t pi = 3.1415927;
  Double_t dphi = fabs(phi1 - phi2);
  if(dphi >= pi) dphi = 2. * pi - dphi;
  return dphi;
}

Double_t deltaEta(Double_t eta1, Double_t eta2)
{
  Double_t deta = fabs(eta1 - eta2);
  return deta;
}

Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2)
{
  Double_t dEta = deltaEta(eta1, eta2);
  Double_t dPhi = deltaPhi(phi1, phi2);
  Double_t dr = sqrt(dEta*dEta + dPhi*dPhi);
  return dr;
}

Bool_t isPassAccCondition_GenLepton_ECALGAP(double leadpt, double leadeta, double subleadpt, double subleadeta)
{
  Bool_t isPassAcc = kFALSE;

  if( leadpt > 30. && fabs(leadeta) < 2.5 && !( fabs(leadeta) > 1.4442 && fabs(leadeta) < 1.566 ) &&
      subleadpt  > 10.  && fabs(subleadeta) < 2.5 && !( fabs(subleadeta) > 1.4442 && fabs(subleadeta) < 1.566 ) )
    isPassAcc = 1;

  return isPassAcc;
}

Bool_t isPassAccCondition_Electron(double leadpt, double leadeta, double subleadpt, double subleadeta)
{
  Bool_t isPassAcc = kFALSE;
  if( leadpt > 30 && fabs(leadeta) < 2.5 && !( fabs(leadeta) > 1.4442 && fabs(leadeta) < 1.566 ) &&
      subleadpt  > 10  && fabs(subleadeta)  < 2.5 && !( fabs(subleadeta) > 1.4442 && fabs(subleadeta) < 1.566 ) )
    isPassAcc = kTRUE;

  return isPassAcc;
}

void SkimNtuple()
{
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

  int nsample = InputFiles_signal_DY.size();
  TFile* file[11];

  for(unsigned int j = 0; j < InputFiles_signal_DY.size(); ++j )
  {
    TString analyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
    TString basePath = analyzerPath+"/ElecChannel/Include";
    TFile *f1 = TFile::Open(basePath + "/dataPUDist.root");
    TFile *f2 = TFile::Open(basePath + "/PileUp_MC.root");

    //data PU histo
    TH1F *DATA_puDist = (TH1F*)f1->Get("pileup");
    DATA_puDist->Scale(1/DATA_puDist->Integral());

    //mc PU histo
    TH1F *MC_puDist = (TH1F*)f2->Get("pileup_MC");
    TH1F *weights = (TH1F*)DATA_puDist->Clone("weights");
    weights->Divide(MC_puDist);

    TTree * tmpTree = (TTree*)InputFiles_signal_DY.at(j)->Get("ntupler/ElectronTree");

    vector<float>   *genPreFSR_Pt;
    vector<float>   *genPreFSR_Eta;
    vector<float>   *genPreFSR_Rap;
    vector<float>   *genPreFSR_Phi;
    vector<float>   *genPreFSR_En;
    vector<float>   *genPostFSR_Pt;
    vector<float>   *genPostFSR_Eta;
    vector<float>   *genPostFSR_Rap;
    vector<float>   *genPostFSR_Phi;
    vector<float>   *genPostFSR_En;
    vector<int>     *passMediumId;
    vector<int>     *passTightId;
    vector<int>     *passHEEPId;
    vector<float>   *ptElec;
    vector<float>   *etaElec;
    vector<float>   *rapElec;
    vector<float>   *phiElec;
    vector<float>   *energyElec;
    vector<float>   *etaSC;
    Int_t           tauFlag; 
    Double_t        theWeight;
    Int_t           nPUTrue;
    Bool_t          Ele23_WPLoose;
    vector<double>  *pt_Ele23;
    vector<double>  *eta_Ele23;
    vector<double>  *phi_Ele23;

    genPreFSR_Pt = 0;
    genPreFSR_Eta = 0;
    genPreFSR_Rap = 0;
    genPreFSR_Phi = 0;
    genPreFSR_En = 0;
    genPostFSR_Pt = 0;
    genPostFSR_Eta = 0;
    genPostFSR_Rap = 0;
    genPostFSR_Phi = 0;
    genPostFSR_En = 0;
    ptElec = 0;
    etaElec = 0;
    rapElec = 0;
    phiElec = 0;
    energyElec = 0;
    etaSC = 0;
    passMediumId = 0;
    passTightId = 0;
    passHEEPId = 0;
    pt_Ele23 = 0;
    eta_Ele23 = 0;
    phi_Ele23 = 0;

    tmpTree->SetBranchStatus("*", 0);
    tmpTree->SetBranchStatus("genPreFSR_Pt", 1);
    tmpTree->SetBranchStatus("genPreFSR_Eta", 1);
    tmpTree->SetBranchStatus("genPreFSR_Rap", 1);
    tmpTree->SetBranchStatus("genPreFSR_Phi", 1);
    tmpTree->SetBranchStatus("genPreFSR_En", 1);
    tmpTree->SetBranchStatus("genPostFSR_Pt", 1);
    tmpTree->SetBranchStatus("genPostFSR_Eta", 1);
    tmpTree->SetBranchStatus("genPostFSR_Rap", 1);
    tmpTree->SetBranchStatus("genPostFSR_Phi", 1);
    tmpTree->SetBranchStatus("genPostFSR_En", 1);
    tmpTree->SetBranchStatus("ptElec", 1);
    tmpTree->SetBranchStatus("etaElec", 1);
    tmpTree->SetBranchStatus("phiElec", 1);
    tmpTree->SetBranchStatus("energyElec", 1);
    tmpTree->SetBranchStatus("etaSC", 1);
    tmpTree->SetBranchStatus("passMediumId", 1);
    tmpTree->SetBranchStatus("tauFlag", 1);
    tmpTree->SetBranchStatus("theWeight", 1);
    tmpTree->SetBranchStatus("nPUTrue", 1);
    tmpTree->SetBranchStatus("pt_Ele23", 1);
    tmpTree->SetBranchStatus("eta_Ele23", 1);
    tmpTree->SetBranchStatus("phi_Ele23", 1);
    tmpTree->SetBranchStatus("Ele23_WPLoose", 1);

    tmpTree->SetBranchAddress("genPreFSR_Pt", &genPreFSR_Pt);
    tmpTree->SetBranchAddress("genPreFSR_Eta", &genPreFSR_Eta);
    tmpTree->SetBranchAddress("genPreFSR_Rap", &genPreFSR_Rap);
    tmpTree->SetBranchAddress("genPreFSR_Phi", &genPreFSR_Phi);
    tmpTree->SetBranchAddress("genPreFSR_En", &genPreFSR_En);
    tmpTree->SetBranchAddress("genPostFSR_Pt", &genPostFSR_Pt);
    tmpTree->SetBranchAddress("genPostFSR_Eta", &genPostFSR_Eta);
    tmpTree->SetBranchAddress("genPostFSR_Rap", &genPostFSR_Rap);
    tmpTree->SetBranchAddress("genPostFSR_Phi", &genPostFSR_Phi);
    tmpTree->SetBranchAddress("genPostFSR_En", &genPostFSR_En);
    tmpTree->SetBranchAddress("ptElec", &ptElec);
    tmpTree->SetBranchAddress("etaElec", &etaElec);
    tmpTree->SetBranchAddress("rapElec", &rapElec);
    tmpTree->SetBranchAddress("phiElec", &phiElec);
    tmpTree->SetBranchAddress("energyElec", &energyElec);
    tmpTree->SetBranchAddress("etaSC", &etaSC);
    tmpTree->SetBranchAddress("passMediumId", &passMediumId);
    tmpTree->SetBranchAddress("passTightId", &passTightId);
    tmpTree->SetBranchAddress("passHEEPId", &passHEEPId);
    tmpTree->SetBranchAddress("tauFlag", &tauFlag);
    tmpTree->SetBranchAddress("theWeight", &theWeight);
    tmpTree->SetBranchAddress("nPUTrue", &nPUTrue);
    tmpTree->SetBranchAddress("Ele23_WPLoose", &Ele23_WPLoose);
    tmpTree->SetBranchAddress("pt_Ele23", &pt_Ele23);
    tmpTree->SetBranchAddress("eta_Ele23", &eta_Ele23);
    tmpTree->SetBranchAddress("phi_Ele23", &phi_Ele23);

    file[j] = new TFile(Form("DYEE_M%dto%d.root",mass[j],mass[j+1]),"RECREATE");

    // -- initialization of the skimmed tree
    TTree *smallTree = new TTree("tree"," after preselections tree");

    // Branch variable declaration
    double Ele1PT;
    double Ele2PT;
    double Ele1Eta;
    double Ele2Eta;
    double ZMass;
    double lumiWeights;
    double genWeights;
    double PUWeights;

    double ZMass_reco;
    double Ele1PT_postFSR;
    double Ele2PT_postFSR;
    double Ele1Eta_postFSR;
    double Ele2Eta_postFSR;

    // Branch declaration
    smallTree->Branch("Ele1PT", &Ele1PT, "Ele1PT/D"); // -- reco-level
    smallTree->Branch("Ele2PT", &Ele2PT, "Ele2PT/D");  // -- reco-level
    smallTree->Branch("Ele1Eta", &Ele1Eta, "Ele1Eta/D");  // -- reco-level
    smallTree->Branch("Ele2Eta", &Ele2Eta, "Ele2Eta/D");   // -- reco-level
    smallTree->Branch("ZMass", &ZMass, "ZMass/D"); // -- post-FSR mass
    smallTree->Branch("lumiWeights", &lumiWeights, "lumiWeights/D");
    smallTree->Branch("genWeights", &genWeights, "genWeights/D");
    smallTree->Branch("PUWeights",  &PUWeights, "PUWeights/D");

    // -- additional info for the cross check
    smallTree->Branch("ZMass_reco",     &ZMass_reco,       "ZMass_reco/D"); 
    smallTree->Branch("Ele1PT_postFSR", &Ele1PT_postFSR,   "Ele1PT_postFSR/D"); 
    smallTree->Branch("Ele2PT_postFSR", &Ele2PT_postFSR,   "Ele2PT_postFSR/D"); 
    smallTree->Branch("Ele1Eta_postFSR", &Ele1Eta_postFSR, "Ele1Eta_postFSR/D"); 
    smallTree->Branch("Ele2Eta_postFSR", &Ele2Eta_postFSR, "Ele2Eta_postFSR/D"); 



    const Int_t nMassBin = 43;

    Double_t MassBinEdges[44] = {15,20,25,30,35,40,45,50,55,60,64,68,72,76,81,86,91,96,101,106,110,115,120,126,133,141,150,160,171,185,200,220,243,273,320,380,440,510,600,700,830,1000,1500,3000};

    TH1D *h_mass_AccTotal = new TH1D("h_mass_AccTotal", "", nMassBin, MassBinEdges);
    TH1D *h_mass_AccPass = new TH1D("h_mass_AccPass", "", nMassBin, MassBinEdges);
    TH1D *h_mass_EffTotal = new TH1D("h_mass_EffTotal", "", nMassBin, MassBinEdges);
    TH1D *h_mass_EffPass = new TH1D("h_mass_EffPass", "", nMassBin, MassBinEdges);

    double dR, massGen;
    TLorentzVector gen1,gen2,diGen;
    TLorentzVector numEle1, numEle2, numDiele, numEle1HEEP, numEle2HEEP, numDieleHEEP, denEle1, denEle2, denDiele;
    TLorentzVector genEle1, genEle2, genDiele;
    TLorentzVector ele1,ele2,dielectron;

    // for id corr
    int mediumId,passId;

    //for sorted electrons
    vector <double> newelePt; 
    vector <double> neweleEta; 
    vector <double> neweleEnr; 
    vector <double> newelePhi;
    vector <double> neweleMediumId;
    vector <double> newSCeta;

    //for gen electrons, postFSR
    vector <double> newgenPt; 
    vector <double> newgenEta; 
    vector <double> newgenEnr; 
    vector <double> newgenPhi;

    double lumiWeight = (xsec[j]/sumofWts[j])*2258.066;
    cout<<"DY Sample: "<<mass[j]<<"to"<<mass[j+1]<<endl;
    //cout << "Lumiweight = " << lumiWeight << endl;

    unsigned int nentries = tmpTree->GetEntries();
    cout<<"entries: "<<nentries<<endl;


    // -- event iteration
    for (unsigned int k=0; k < nentries; k++)
    {
      tmpTree->GetEntry(k);
      if(k%1000000 == 0)  cout << "Events Processed :  " << k << endl;

      //PUWeight
      int bin = 0;
      double puWeights = 1.0;
      bin = weights->GetXaxis()->FindBin(nPUTrue);
      puWeights = weights->GetBinContent(bin);

      // sorting of reco electrons
      int index1[ptElec->size()];
      float pt1[ptElec->size()];

      for(unsigned int el=0; el<ptElec->size(); el++)
        pt1[el]=ptElec->at(el);

      int sizer = sizeof(pt1)/sizeof(pt1[0]);
      TMath::Sort(sizer,pt1,index1,true);

      // sorting of gen electrons : postFSR
      int index2[genPostFSR_Pt->size()];
      float pt2[genPostFSR_Pt->size()];

      for(unsigned int b=0; b<genPostFSR_Pt->size(); b++)
        pt2[b]=genPostFSR_Pt->at(b);

      int sizen = sizeof(pt2)/sizeof(pt2[0]);
      TMath::Sort(sizen,pt2,index2,true);

      // sorting of gen electrons : preFSR
      int index3[genPreFSR_Pt->size()];
      float pt3[genPreFSR_Pt->size()];

      for(unsigned int c=0; c<genPreFSR_Pt->size(); c++)
        pt3[c]=genPreFSR_Pt->at(c);

      int size1 = sizeof(pt3)/sizeof(pt3[0]);
      TMath::Sort(size1,pt3,index3,true);

      //clearing of vectors
      mediumId = 0,  passId = 0;
      dR = 0.; massGen = 0.0;
      newelePt.clear(); 
      neweleEta.clear(); 
      neweleEnr.clear(); 
      newelePhi.clear(); 
      neweleMediumId.clear(); 
      newSCeta.clear();

      newgenPt.clear(); 
      newgenEta.clear(); 
      newgenEnr.clear(); 
      newgenPhi.clear();

      double postFSRmass = 0.;
      double recomass = 0.;

      if(genPreFSR_Pt->size() == 2)
      {
        gen1.SetPtEtaPhiE(genPreFSR_Pt->at(0),genPreFSR_Eta->at(0),genPreFSR_Phi->at(0),genPreFSR_En->at(0));
        gen2.SetPtEtaPhiE(genPreFSR_Pt->at(1),genPreFSR_Eta->at(1),genPreFSR_Phi->at(1),genPreFSR_En->at(1));
        diGen=gen1+gen2;
        massGen=diGen.M();
      }

      if(j==1 && massGen >= 100) continue;

      double dR1(0.),dR2(0.);

      if(!tauFlag && genPreFSR_Pt->size() == 2)
      {
        if(genPostFSR_Pt->size() >= 2.)
        {
          for(unsigned int j=0;j<genPostFSR_Eta->size();j++)
          {
            newgenPt.push_back(genPostFSR_Pt->at(index2[j]));
            newgenEta.push_back(genPostFSR_Eta->at(index2[j]));
            newgenEnr.push_back(genPostFSR_En->at(index2[j]));
            newgenPhi.push_back(genPostFSR_Phi->at(index2[j]));
          }
        }

        Bool_t Flag_PassAcc = kFALSE;
        genEle1.SetPtEtaPhiE(newgenPt.at(0),newgenEta.at(0),newgenPhi.at(0),newgenEnr.at(0));
        genEle2.SetPtEtaPhiE(newgenPt.at(1),newgenEta.at(1),newgenPhi.at(1),newgenEnr.at(1));

        genDiele = genEle1+genEle2;
        postFSRmass = genDiele.M();

        Flag_PassAcc = isPassAccCondition_GenLepton_ECALGAP(newgenPt.at(0),newgenEta.at(0),newgenPt.at(1),newgenEta.at(1));

        if(Flag_PassAcc)
        {
          h_mass_AccTotal->Fill(postFSRmass,theWeight*lumiWeight);
          h_mass_AccPass->Fill(postFSRmass,theWeight*lumiWeight);
        }
        else
          h_mass_AccTotal->Fill(postFSRmass,theWeight*lumiWeight);

        if(Flag_PassAcc == kTRUE)
        {
          Bool_t Flag_PassEff = kFALSE;

          if(ptElec->size()>=2.)
          {
            for(unsigned int i=0;i<ptElec->size();i++)
            {
              mediumId = passMediumId->at(index1[i]);
              passId = mediumId; 

              if(passId)
              {
                if(fabs(etaSC->at(index1[i])) < 2.5 && !(fabs(etaSC->at(index1[i])) > 1.4442 && fabs(etaSC->at(index1[i])) < 1.566))
                {
                  newelePt.push_back(ptElec->at(index1[i]));
                  neweleEta.push_back(etaElec->at(index1[i]));
                  neweleEnr.push_back(energyElec->at(index1[i]));
                  newelePhi.push_back(phiElec->at(index1[i]));
                  neweleMediumId.push_back(passMediumId->at(index1[i]));
                  newSCeta.push_back(etaSC->at(index1[i]));
                } // -- eta check
              } // -- end of if(passId)
            } // -- iteration on reco electron
          } // -- end of if(ptElec->size()>=2.)


          if(newelePt.size()==2)
          {
            int countTrig = 0;
            for(unsigned int j = 0; j < pt_Ele23->size(); j++)
            {
              dR1 = deltaR(neweleEta.at(0), newelePhi.at(0), eta_Ele23->at(j), phi_Ele23->at(j));
              dR2 = deltaR(neweleEta.at(1), newelePhi.at(1), eta_Ele23->at(j), phi_Ele23->at(j));
              if(dR1 < 0.1 || dR2 < 0.1) countTrig++;
            }

            Bool_t isPassAcc = kFALSE;
            isPassAcc = isPassAccCondition_Electron(newelePt.at(0),newSCeta.at(0),newelePt.at(1),newSCeta.at(1));

            ele1.SetPtEtaPhiE(newelePt.at(0),neweleEta.at(0),newelePhi.at(0),neweleEnr.at(0));
            ele2.SetPtEtaPhiE(newelePt.at(1),neweleEta.at(1),newelePhi.at(1),neweleEnr.at(1));
            dielectron=ele1+ele2;
            recomass = dielectron.M();

            if(isPassAcc == kTRUE && recomass > 10. && countTrig != 0 && Ele23_WPLoose)
              Flag_PassEff = kTRUE;
          } // -- end of if(newelePt.size()==2)

          if(Flag_PassEff == kTRUE)
          {
            h_mass_EffPass->Fill(postFSRmass,theWeight*lumiWeight*puWeights);
            h_mass_EffTotal->Fill(postFSRmass,theWeight*lumiWeight);
          }
          else
            h_mass_EffTotal->Fill(postFSRmass,theWeight*lumiWeight);


          // -- fill the tree!
          if( Flag_PassEff )
          {
            // -- reco info
            // -- eta = supercluster eta
            Ele1PT = newelePt.at(0);
            Ele2PT = newelePt.at(1);
            Ele1Eta = newSCeta.at(0);
            Ele2Eta = newSCeta.at(1);
            // -- post-FSR mass
            ZMass = postFSRmass;

            lumiWeights = lumiWeight;
            genWeights = theWeight;
            PUWeights = puWeights;

            ZMass_reco = recomass;
            Ele1PT_postFSR = newgenPt.at(0);
            Ele2PT_postFSR = newgenPt.at(1);
            Ele1Eta_postFSR = newgenEta.at(0);
            Ele2Eta_postFSR = newgenEta.at(1);

            smallTree->Fill();
          }

        } // -- end of if(Flag_PassAcc == kTRUE)

      } // -- end of if(!tauFlag && genPreFSR_Pt->size() == 2)

    } // -- end of event iteration

    file[j]->Write();    
    cout<<""<<endl;
  }

}



