#pragma once

#include <TTree.h>
#include <TChain.h>
#include <vector>

class NtupleHandle_Ele
{
public:
  TChain *chain;

  Double_t runNum;
  Double_t eventNum;
  Double_t lumiBlockNum;

  Int_t   nPV;
  Float_t rho;

  Bool_t ele23_WPLoose;
  std::vector<double>* ele23Obj_pt;
  std::vector<double>* ele23Obj_eta;
  std::vector<double>* ele23Obj_phi;

  Int_t nEle;
  std::vector<float>* ele_pt;
  std::vector<float>* ele_eta;
  std::vector<float>* ele_phi;
  std::vector<float>* ele_energy;
  std::vector<float>* ele_charge;
  std::vector<float>* ele_energySC;
  std::vector<float>* ele_etSC;
  std::vector<int>*   ele_looseID;
  std::vector<float>* ele_PFIso_PU;
  std::vector<float>* ele_etaSC;
  std::vector<float>* ele_isEB;
  std::vector<float>* ele_full5x5_sigmaIEtaIEta;
  std::vector<float>* ele_phiSCWidth;
  std::vector<float>* ele_relPFIso_dBeta;
  std::vector<int>*   ele_mHit;
  std::vector<float>* ele_etaSCWidth;
  std::vector<float>* ele_PFIso_NH;
  std::vector<float>* ele_dxy;
  std::vector<float>* ele_phiSC;
  std::vector<int>*   ele_mediumID;
  std::vector<int>*   ele_ECALDrivenSeed;
  std::vector<float>* ele_ooEmooP;
  std::vector<int>*   ele_tightID;
  std::vector<float>* ele_relPFIso_rhoCorr;
  std::vector<float>* ele_dPhiIn;
  std::vector<float>* ele_dz;
  std::vector<int>*   ele_vetoID;
  std::vector<float>* ele_brem;
  std::vector<float>* ele_isEE;
  std::vector<float>* ele_PFIso_CH;
  std::vector<int>*   ele_convVeto;
  std::vector<float>* ele_r9;
  std::vector<float>* ele_PFIso_Photon;
  std::vector<float>* ele_dEtaIn;
  std::vector<float>* ele_hOverE;
  std::vector<int>*   ele_HEEPID;



  Double_t genWeight;
  Int_t    nPU;
  Int_t    nGenEle;
  std::vector<bool>    *genFlag_fromHardProcessFinalState;

  // -- gen photons with mother check (is it true?)
  std::vector<float>   *genPhoton_px;
  std::vector<float>   *genPhoton_py;
  std::vector<float>   *genPhoton_pz;
  std::vector<float>   *genPhoton_pt;
  std::vector<float>   *genPhoton_eta;
  std::vector<float>   *genPhoton_phi;
  std::vector<float>   *genPhoton_energy;

  // -- gen electrons with fromHardProcessFinalState == 1
  std::vector<float>   *genEle_postFSR_px;
  std::vector<float>   *genEle_postFSR_py;
  std::vector<float>   *genEle_postFSR_pz;
  std::vector<float>   *genEle_postFSR_pt;
  std::vector<float>   *genEle_postFSR_eta;
  std::vector<float>   *genEle_postFSR_phi;
  std::vector<float>   *genEle_postFSR_energy;

  // -- gen electrons with isHardProcess == 1 
  std::vector<float>   *genEle_preFSR_px;
  std::vector<float>   *genEle_preFSR_py;
  std::vector<float>   *genEle_preFSR_pz;
  std::vector<float>   *genEle_preFSR_pt;
  std::vector<float>   *genEle_preFSR_eta;
  std::vector<float>   *genEle_preFSR_phi;
  std::vector<float>   *genEle_preFSR_energy;

  NtupleHandle_Ele(TChain* chainPtr)
  {
    Init();
    chain = chainPtr;
    TurnOnBranches();
  }

  void GetEvent(Int_t i)
  {
      if(!chain) return;
      
      chain->GetEntry(i);
  }

  void TurnOnBranches()
  {
    chain->SetBranchStatus("RunNo", 1);
    chain->SetBranchAddress("RunNo", &runNum);

    chain->SetBranchStatus("Lumi", 1);
    chain->SetBranchAddress("Lumi", &lumiBlockNum);

    chain->SetBranchStatus("EvtNo", 1);
    chain->SetBranchAddress("EvtNo", &eventNum);

    chain->SetBranchStatus("theWeight", 1);
    chain->SetBranchAddress("theWeight", &genWeight);

    chain->SetBranchStatus("nPV", 1);
    chain->SetBranchAddress("nPV", &nPV);

    chain->SetBranchStatus("nPUTrue", 1);
    chain->SetBranchAddress("nPUTrue", &nPU);

    chain->SetBranchStatus("rho", 1);
    chain->SetBranchAddress("rho", &rho);

    chain->SetBranchStatus("Ele23_WPLoose", 1);
    chain->SetBranchAddress("Ele23_WPLoose", &ele23_WPLoose);

    chain->SetBranchStatus("pt_Ele23", 1);
    chain->SetBranchAddress("pt_Ele23", &ele23Obj_pt);

    chain->SetBranchStatus("eta_Ele23", 1);
    chain->SetBranchAddress("eta_Ele23", &ele23Obj_eta);

    chain->SetBranchStatus("phi_Ele23", 1);
    chain->SetBranchAddress("phi_Ele23", &ele23Obj_phi);

    chain->SetBranchStatus("nEle", 1);
    chain->SetBranchAddress("nEle", &nEle);

    chain->SetBranchStatus("nGenEle", 1);
    chain->SetBranchAddress("nGenEle", &nGenEle);

    chain->SetBranchStatus("fromHProcessFinalState", 1);
    chain->SetBranchAddress("fromHProcessFinalState", &genFlag_fromHardProcessFinalState);


    chain->SetBranchStatus("genPhoton_Px", 1);
    chain->SetBranchAddress("genPhoton_Px", &genPhoton_px);

    chain->SetBranchStatus("genPhoton_Py", 1);
    chain->SetBranchAddress("genPhoton_Py", &genPhoton_py);

    chain->SetBranchStatus("genPhoton_Pz", 1);
    chain->SetBranchAddress("genPhoton_Pz", &genPhoton_pz);

    chain->SetBranchStatus("genPhoton_Pt", 1);
    chain->SetBranchAddress("genPhoton_Pt", &genPhoton_pt);

    chain->SetBranchStatus("genPhoton_Eta", 1);
    chain->SetBranchAddress("genPhoton_Eta", &genPhoton_eta);

    chain->SetBranchStatus("genPhoton_Phi", 1);
    chain->SetBranchAddress("genPhoton_Phi", &genPhoton_phi);

    chain->SetBranchStatus("genPhoton_En", 1);
    chain->SetBranchAddress("genPhoton_En", &genPhoton_energy);


    chain->SetBranchStatus("genPostFSR_Px", 1);
    chain->SetBranchAddress("genPostFSR_Px", &genEle_postFSR_px);

    chain->SetBranchStatus("genPostFSR_Py", 1);
    chain->SetBranchAddress("genPostFSR_Py", &genEle_postFSR_py);

    chain->SetBranchStatus("genPostFSR_Pz", 1);
    chain->SetBranchAddress("genPostFSR_Pz", &genEle_postFSR_pz);

    chain->SetBranchStatus("genPostFSR_Pt", 1);
    chain->SetBranchAddress("genPostFSR_Pt", &genEle_postFSR_pt);

    chain->SetBranchStatus("genPostFSR_Eta", 1);
    chain->SetBranchAddress("genPostFSR_Eta", &genEle_postFSR_eta);

    chain->SetBranchStatus("genPostFSR_Phi", 1);
    chain->SetBranchAddress("genPostFSR_Phi", &genEle_postFSR_phi);

    chain->SetBranchStatus("genPostFSR_En", 1);
    chain->SetBranchAddress("genPostFSR_En", &genEle_postFSR_energy);


    chain->SetBranchStatus("genPreFSR_Px", 1);
    chain->SetBranchAddress("genPreFSR_Px", &genEle_preFSR_px);

    chain->SetBranchStatus("genPreFSR_Py", 1);
    chain->SetBranchAddress("genPreFSR_Py", &genEle_preFSR_py);

    chain->SetBranchStatus("genPreFSR_Pz", 1);
    chain->SetBranchAddress("genPreFSR_Pz", &genEle_preFSR_pz);

    chain->SetBranchStatus("genPreFSR_Pt", 1);
    chain->SetBranchAddress("genPreFSR_Pt", &genEle_preFSR_pt);

    chain->SetBranchStatus("genPreFSR_Eta", 1);
    chain->SetBranchAddress("genPreFSR_Eta", &genEle_preFSR_eta);

    chain->SetBranchStatus("genPreFSR_Phi", 1);
    chain->SetBranchAddress("genPreFSR_Phi", &genEle_preFSR_phi);

    chain->SetBranchStatus("genPreFSR_En", 1);
    chain->SetBranchAddress("genPreFSR_En", &genEle_preFSR_energy);

    chain->SetBranchStatus("ptElec", 1);
    chain->SetBranchAddress("ptElec", &ele_pt);

    chain->SetBranchStatus("etaElec", 1);
    chain->SetBranchAddress("etaElec", &ele_eta);

    chain->SetBranchStatus("phiElec", 1);
    chain->SetBranchAddress("phiElec", &ele_phi);

    chain->SetBranchStatus("energyElec", 1);
    chain->SetBranchAddress("energyElec", &ele_energy);

    chain->SetBranchStatus("chargeElec", 1);
    chain->SetBranchAddress("chargeElec", &ele_charge);

    chain->SetBranchStatus("etSC", 1);
    chain->SetBranchAddress("etSC", &ele_etSC);

    chain->SetBranchStatus("passLooseId", 1);
    chain->SetBranchAddress("passLooseId", &ele_looseID);

    chain->SetBranchStatus("isoChargedFromPU", 1);
    chain->SetBranchAddress("isoChargedFromPU", &ele_PFIso_PU);

    chain->SetBranchStatus("etaSC", 1);
    chain->SetBranchAddress("etaSC", &ele_etaSC);

    chain->SetBranchStatus("eleInBarrel", 1);
    chain->SetBranchAddress("eleInBarrel", &ele_isEB);

    chain->SetBranchStatus("full5x5_sigmaIetaIeta", 1);
    chain->SetBranchAddress("full5x5_sigmaIetaIeta", &ele_full5x5_sigmaIEtaIEta);

    chain->SetBranchStatus("phiScWidth", 1);
    chain->SetBranchAddress("phiScWidth", &ele_phiSCWidth);

    chain->SetBranchStatus("isoDeltaBeta", 1);
    chain->SetBranchAddress("isoDeltaBeta", &ele_relPFIso_dBeta);

    chain->SetBranchStatus("expectedMissingInnerHits", 1);
    chain->SetBranchAddress("expectedMissingInnerHits", &ele_mHit);

    chain->SetBranchStatus("etaScWidth", 1);
    chain->SetBranchAddress("etaScWidth", &ele_etaSCWidth);

    chain->SetBranchStatus("isoNeutralHadrons", 1);
    chain->SetBranchAddress("isoNeutralHadrons", &ele_PFIso_NH);

    chain->SetBranchStatus("d0", 1);
    chain->SetBranchAddress("d0", &ele_dxy);

    chain->SetBranchStatus("phiSC", 1);
    chain->SetBranchAddress("phiSC", &ele_phiSC);

    chain->SetBranchStatus("passMediumId", 1);
    chain->SetBranchAddress("passMediumId", &ele_mediumID);

    chain->SetBranchStatus("eleEcalDrivenSeed", 1);
    chain->SetBranchAddress("eleEcalDrivenSeed", &ele_ECALDrivenSeed);

    chain->SetBranchStatus("ooEmooP", 1);
    chain->SetBranchAddress("ooEmooP", &ele_ooEmooP);

    chain->SetBranchStatus("passTightId", 1);
    chain->SetBranchAddress("passTightId", &ele_tightID);

    chain->SetBranchStatus("isoRho", 1);
    chain->SetBranchAddress("isoRho", &ele_relPFIso_rhoCorr);

    chain->SetBranchStatus("dPhiIn", 1);
    chain->SetBranchAddress("dPhiIn", &ele_dPhiIn);

    chain->SetBranchStatus("dz", 1);
    chain->SetBranchAddress("dz", &ele_dz);

    chain->SetBranchStatus("passVetoId", 1);
    chain->SetBranchAddress("passVetoId", &ele_vetoID);

    chain->SetBranchStatus("brem", 1);
    chain->SetBranchAddress("brem", &ele_brem);

    chain->SetBranchStatus("eleInEndcap", 1);
    chain->SetBranchAddress("eleInEndcap", &ele_isEE);

    chain->SetBranchStatus("isoChargedHadrons", 1);
    chain->SetBranchAddress("isoChargedHadrons", &ele_PFIso_CH);

    chain->SetBranchStatus("passConversionVeto", 1);
    chain->SetBranchAddress("passConversionVeto", &ele_convVeto);

    chain->SetBranchStatus("r9", 1);
    chain->SetBranchAddress("r9", &ele_r9);

    chain->SetBranchStatus("isoPhotons", 1);
    chain->SetBranchAddress("isoPhotons", &ele_PFIso_Photon);

    chain->SetBranchStatus("dEtaIn", 1);
    chain->SetBranchAddress("dEtaIn", &ele_dEtaIn);

    chain->SetBranchStatus("hOverE", 1);
    chain->SetBranchAddress("hOverE", &ele_hOverE);

    chain->SetBranchStatus("enSC", 1);
    chain->SetBranchAddress("enSC", &ele_energySC);

    chain->SetBranchStatus("passHEEPId", 1);
    chain->SetBranchAddress("passHEEPId", &ele_HEEPID);
  }

  void Init()
  {
    chain = nullptr;

    runNum = -999;
    eventNum = -999;
    lumiBlockNum = -999;

    nPV = -999;
    rho = -999;

    ele23_WPLoose = kFALSE;

    // -- without vector initialization, the code makes segmentation error
    ele23Obj_pt = 0;
    ele23Obj_eta = 0;
    ele23Obj_phi = 0;

    nEle = -999;
    ele_pt = 0;
    ele_eta = 0;
    ele_phi = 0;
    ele_energy = 0;
    ele_charge = 0;
    ele_energySC = 0;
    ele_etSC = 0;
    ele_looseID = 0;
    ele_PFIso_PU = 0;
    ele_etaSC = 0;
    ele_isEB = 0;
    ele_full5x5_sigmaIEtaIEta = 0;
    ele_phiSCWidth = 0;
    ele_relPFIso_dBeta = 0;
    ele_mHit = 0;
    ele_etaSCWidth = 0;
    ele_PFIso_NH = 0;
    ele_dxy = 0;
    ele_phiSC = 0;
    ele_mediumID = 0;
    ele_ECALDrivenSeed = 0;
    ele_ooEmooP = 0;
    ele_tightID = 0;
    ele_relPFIso_rhoCorr = 0;
    ele_dPhiIn = 0;
    ele_dz = 0;
    ele_vetoID = 0;
    ele_brem = 0;
    ele_isEE = 0;
    ele_PFIso_CH = 0;
    ele_convVeto = 0;
    ele_r9 = 0;
    ele_PFIso_Photon = 0;
    ele_dEtaIn = 0;
    ele_hOverE = 0;
    ele_HEEPID = 0;

    genWeight = -999;
    nPU = -999;
    nGenEle = -999;
    genFlag_fromHardProcessFinalState = 0;

    genPhoton_px = 0;
    genPhoton_py = 0;
    genPhoton_pz = 0;
    genPhoton_pt = 0;
    genPhoton_eta = 0;
    genPhoton_phi = 0;
    genPhoton_energy = 0;

    genEle_postFSR_px = 0;
    genEle_postFSR_py = 0;
    genEle_postFSR_pz = 0;
    genEle_postFSR_pt = 0;
    genEle_postFSR_eta = 0;
    genEle_postFSR_phi = 0;
    genEle_postFSR_energy = 0;

    genEle_preFSR_px = 0;
    genEle_preFSR_py = 0;
    genEle_preFSR_pz = 0;
    genEle_preFSR_pt = 0;
    genEle_preFSR_eta = 0;
    genEle_preFSR_phi = 0;
    genEle_preFSR_energy = 0;

  }


private:
};

/////////////////////////////////
// -- full list of branches -- //
/////////////////////////////////
// -- included in this class -- //
// Double_t        RunNo;
// Double_t        EvtNo;
// Double_t        Lumi;
// Double_t        theWeight;
// Int_t           nPV;
// Int_t           nPUTrue;
// Float_t         rho;
// Bool_t          Ele23_WPLoose;
// vector<double>  *pt_Ele23;
// vector<double>  *eta_Ele23;
// vector<double>  *phi_Ele23;
// Int_t           nEle;
// Int_t           nGenEle;
// vector<bool>    *fromHProcessFinalState;
// vector<float>   *genPhoton_Px;
// vector<float>   *genPhoton_Py;
// vector<float>   *genPhoton_Pz;
// vector<float>   *genPhoton_Pt;
// vector<float>   *genPhoton_Eta;
// vector<float>   *genPhoton_Phi;
// vector<float>   *genPhoton_En;
// vector<float>   *genPostFSR_Px;
// vector<float>   *genPostFSR_Py;
// vector<float>   *genPostFSR_Pz;
// vector<float>   *genPostFSR_Pt;
// vector<float>   *genPostFSR_Eta;
// vector<float>   *genPostFSR_Phi;
// vector<float>   *genPostFSR_En;
// vector<float>   *genPreFSR_Px;
// vector<float>   *genPreFSR_Py;
// vector<float>   *genPreFSR_Pz;
// vector<float>   *genPreFSR_Pt;
// vector<float>   *genPreFSR_Eta;
// vector<float>   *genPreFSR_Rap;
// vector<float>   *genPreFSR_Phi;
// vector<float>   *genPreFSR_En;
// vector<float>   *ptElec;
// vector<float>   *etaElec;
// vector<float>   *phiElec;
// vector<float>   *energyElec;
// vector<float>   *chargeElec;
// vector<float>   *enSC;
// vector<float>   *etSC;
// vector<float>   *etaSC;
// vector<float>   *phiSC;
// vector<float>   *full5x5_sigmaIetaIeta;
// vector<float>   *hOverE;
// vector<float>   *etaScWidth;
// vector<float>   *phiScWidth;
// vector<float>   *r9;
// vector<float>   *dEtaIn;
// vector<float>   *dPhiIn;
// vector<float>   *isoChargedHadrons;
// vector<float>   *isoNeutralHadrons;
// vector<float>   *isoPhotons;
// vector<float>   *isoChargedFromPU;
// vector<float>   *isoDeltaBeta;
// vector<float>   *isoRho;
// vector<float>   *ooEmooP;
// vector<float>   *d0;
// vector<float>   *dz;
// vector<int>     *expectedMissingInnerHits;
// vector<int>     *passConversionVeto;
// vector<float>   *brem;
// vector<int>     *passVetoId;
// vector<int>     *passLooseId;
// vector<int>     *passMediumId;
// vector<int>     *passTightId;
// vector<int>     *passHEEPId;
// vector<int>     *eleEcalDrivenSeed;
// vector<float>   *eleInBarrel;
// vector<float>   *eleInEndcap;

// -- not included -- //
// Double_t        Bunch;
// Int_t           pvNTracks;
// Int_t           nPU;
// Bool_t          Ele27_WP85;
// Bool_t          IsoMu20;
// Bool_t          Ele17_Ele12;
// Bool_t          Mu8_Ele17;
// Bool_t          Mu17_Ele12;
// Bool_t          Mu8_Ele23;
// vector<double>  *etPhoton;
// Int_t           singlePhoton;
// Int_t           prescalePhoton;
// Int_t           nGenTau;
// Int_t           tauFlag;
// vector<bool>    *fromHProcessDecayed;
// vector<float>   *genPhoton_Rap;
// vector<float>   *genPostFSR_Rap;
// vector<float>   *rapElec;
// vector<float>   *massElec;
// vector<float>   *preEnSC;
// vector<float>   *rawEnSC;
// vector<float>   *E1x5;
// vector<float>   *E2x5;
// vector<float>   *E5x5;
// vector<int>     *isTrue;
// vector<int>     *isPassMedium_NoPt;
// vector<int>     *isPassMedium_NoScEta;
// vector<int>     *isPassMedium_NoDEta;
// vector<int>     *isPassMedium_NoDPhi;
// vector<int>     *isPassMedium_NoSigmaEtaEta;
// vector<int>     *isPassMedium_NoHOverE;
// vector<int>     *isPassMedium_NoDxy;
// vector<int>     *isPassMedium_NoDz;
// vector<int>     *isPassMedium_NoEInvP;
// vector<int>     *isPassMedium_NoPFIso;
// vector<int>     *isPassMedium_NoConVeto;
// vector<int>     *isPassMedium_NoMissHits;
// Int_t           nMuons;
// vector<bool>    *isLooseMuon;
// vector<bool>    *isTightMuon;
// vector<bool>    *isHighPtMuon;
// vector<double>  *ptMuon;
// vector<double>  *etaMuon;
// vector<double>  *phiMuon;
// vector<double>  *energyMuon;
// vector<double>  *chargeMuon;
// vector<double>  *isoChargedHadronPfR04Muon;
// vector<double>  *isoNeutralHadronPfR04Muon;
// vector<double>  *isoGammaPfR04Muon;
// vector<double>  *isoChargedFromPUMuon;
// vector<double>  *isoPFMuon;
// vector<double>  *isoTrkMuon;
// vector<double>  *metPt;
// vector<double>  *metPhi;
// vector<double>  *metSumEt;
// Int_t           nPhotons;
// vector<float>   *ptPhoton;
// vector<float>   *etaPhoton;
// vector<float>   *phiPhoton;