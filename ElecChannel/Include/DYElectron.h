#pragma once

#include <ElecChannel/Include/NtupleHandle_Ele.h>
#include <TLorentzVector.h>

#define EleMass 0.000510998 // -- GeV -- //

class DYElectron
{
public:
  Double_t pt;
  Double_t eta;
  Double_t phi;
  Double_t energy;
  Double_t charge;
  Double_t energySC;
  Double_t etSC;
  Int_t    looseID;
  Double_t PFIso_PU;
  Double_t etaSC;
  Double_t isEB;
  Double_t full5x5_sigmaIEtaIEta;
  Double_t phiSCWidth;
  Double_t relPFIso_dBeta;
  Int_t    mHit;
  Double_t etaSCWidth;
  Double_t PFIso_NH;
  Double_t dxy;
  Double_t phiSC;
  Int_t    mediumID;
  Int_t    ECALDrivenSeed;
  Double_t ooEmooP;
  Int_t    tightID;
  Double_t relPFIso_rhoCorr;
  Double_t dPhiIn;
  Double_t dz;
  Int_t    vetoID;
  Double_t brem;
  Double_t isEE;
  Double_t PFIso_CH;
  Int_t    convVeto;
  Double_t r9;
  Double_t PFIso_Photon;
  Double_t dEtaIn;
  Double_t hOverE;
  Int_t    HEEPID;

  TLorentzVector vec_P;

  DYElectron()
  {

  }

  DYElectron(NtupleHandle_Ele *ntuple, Int_t index)
  {
    FillFromNtuple(ntuple, index);
  }

private:
  void FillFromNtuple(NtupleHandle_Ele *ntuple, Int_t index)
  {
    pt                    = ntuple->ele_pt->at(index);
    eta                   = ntuple->ele_eta->at(index);
    phi                   = ntuple->ele_phi->at(index);
    energy                = ntuple->ele_energy->at(index);
    charge                = ntuple->ele_charge->at(index);
    energySC              = ntuple->ele_energySC->at(index);
    etSC                  = ntuple->ele_etSC->at(index);
    looseID               = ntuple->ele_looseID->at(index);
    PFIso_PU              = ntuple->ele_PFIso_PU->at(index);
    etaSC                 = ntuple->ele_etaSC->at(index);
    isEB                  = ntuple->ele_isEB->at(index);
    full5x5_sigmaIEtaIEta = ntuple->ele_full5x5_sigmaIEtaIEta->at(index);
    phiSCWidth            = ntuple->ele_phiSCWidth->at(index);
    relPFIso_dBeta        = ntuple->ele_relPFIso_dBeta->at(index);
    mHit                  = ntuple->ele_mHit->at(index);
    etaSCWidth            = ntuple->ele_etaSCWidth->at(index);
    PFIso_NH              = ntuple->ele_PFIso_NH->at(index);
    dxy                   = ntuple->ele_dxy->at(index);
    phiSC                 = ntuple->ele_phiSC->at(index);
    mediumID              = ntuple->ele_mediumID->at(index);
    ECALDrivenSeed        = ntuple->ele_ECALDrivenSeed->at(index);
    ooEmooP               = ntuple->ele_ooEmooP->at(index);
    tightID               = ntuple->ele_tightID->at(index);
    relPFIso_rhoCorr      = ntuple->ele_relPFIso_rhoCorr->at(index);
    dPhiIn                = ntuple->ele_dPhiIn->at(index);
    dz                    = ntuple->ele_dz->at(index);
    vetoID                = ntuple->ele_vetoID->at(index);
    brem                  = ntuple->ele_brem->at(index);
    isEE                  = ntuple->ele_isEE->at(index);
    PFIso_CH              = ntuple->ele_PFIso_CH->at(index);
    convVeto              = ntuple->ele_convVeto->at(index);
    r9                    = ntuple->ele_r9->at(index);
    PFIso_Photon          = ntuple->ele_PFIso_Photon->at(index);
    dEtaIn                = ntuple->ele_dEtaIn->at(index);
    hOverE                = ntuple->ele_hOverE->at(index);
    HEEPID                = ntuple->ele_HEEPID->at(index);

    vec_P.SetPtEtaPhiM(pt, eta, phi, EleMass);
  }
};

class DYGenEle_postFSR
{
public:
  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t pt;
  Double_t eta;
  Double_t phi;
  Double_t energy;

  TLorentzVector vec_P;

  DYGenEle_postFSR() {}
  DYGenEle_postFSR(NtupleHandle_Ele *ntuple, Int_t index)
  {
    FillFromNtuple(ntuple, index);
  }

private:
  void FillFromNtuple(NtupleHandle_Ele *ntuple, Int_t index)
  {
    px     = ntuple->genEle_postFSR_px->at(index);
    py     = ntuple->genEle_postFSR_py->at(index);
    pz     = ntuple->genEle_postFSR_pz->at(index);
    pt     = ntuple->genEle_postFSR_pt->at(index);
    eta    = ntuple->genEle_postFSR_eta->at(index);
    phi    = ntuple->genEle_postFSR_phi->at(index);
    energy = ntuple->genEle_postFSR_energy->at(index);

    vec_P.SetPxPyPzE(px, py, pz, energy);
  }  
};

class DYGenEle_preFSR
{
public:
  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t pt;
  Double_t eta;
  Double_t phi;
  Double_t energy;

  TLorentzVector vec_P;

  DYGenEle_preFSR() {}
  DYGenEle_preFSR(NtupleHandle_Ele *ntuple, Int_t index)
  {
    FillFromNtuple(ntuple, index);
  }

private:
  void FillFromNtuple(NtupleHandle_Ele *ntuple, Int_t index)
  {
    px     = ntuple->genEle_preFSR_px->at(index);
    py     = ntuple->genEle_preFSR_py->at(index);
    pz     = ntuple->genEle_preFSR_pz->at(index);
    pt     = ntuple->genEle_preFSR_pt->at(index);
    eta    = ntuple->genEle_preFSR_eta->at(index);
    phi    = ntuple->genEle_preFSR_phi->at(index);
    energy = ntuple->genEle_preFSR_energy->at(index);

    vec_P.SetPxPyPzE(px, py, pz, energy);
  }  
};
