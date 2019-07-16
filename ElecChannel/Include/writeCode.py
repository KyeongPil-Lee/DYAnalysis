dic_branch_var = {
    "enSC" : ["vector<float>*", "ele_energySC"],
    "etSC" : ["vector<float>*", "ele_etSC"],
    "etaSC" : ["vector<float>*", "ele_etaSC"],
    "phiSC" : ["vector<float>*", "ele_phiSC"],
    "full5x5_sigmaIetaIeta" : ["vector<float>*", "ele_full5x5_sigmaIEtaIEta"],
    "hOverE" : ["vector<float>*", "ele_hOverE"],
    "etaScWidth" : ["vector<float>*", "ele_etaSCWidth"],
    "phiScWidth" : ["vector<float>*", "ele_phiSCWidth"],
    "r9" : ["vector<float>*", "ele_r9"],
    "dEtaIn" : ["vector<float>*", "ele_dEtaIn"],
    "dPhiIn" : ["vector<float>*", "ele_dPhiIn"],
    "isoChargedHadrons" : ["vector<float>*", "ele_PFIso_CH"],
    "isoNeutralHadrons" : ["vector<float>*", "ele_PFIso_NH"],
    "isoPhotons" :        ["vector<float>*", "ele_PFIso_Photon"],
    "isoChargedFromPU" :  ["vector<float>*", "ele_PFIso_PU"],
    "isoDeltaBeta" : ["vector<float>*", "ele_relPFIso_dBeta"],
    "isoRho" : ["vector<float>*", "ele_relPFIso_rhoCorr"],

    "ooEmooP" : ["vector<float>*", "ele_ooEmooP"],
    "d0" : ["vector<float>*", "ele_dxy"],
    "dz" : ["vector<float>*", "ele_dz"],

    "expectedMissingInnerHits" : ["vector<int>*", "ele_mHit"],
    "passConversionVeto" : ["vector<int>*", "ele_convVeto"],

    "brem" : ["vector<float>*", "ele_brem"],

    "passVetoId" :   ["vector<int>*", "ele_vetoID"],
    "passLooseId" :  ["vector<int>*", "ele_looseID"],
    "passMediumId" : ["vector<int>*", "ele_mediumID"],
    "passTightId" :  ["vector<int>*", "ele_tightID"],
    "passHEEPId" :   ["vector<int>*", "ele_HEEPID"],

    "eleEcalDrivenSeed" : ["vector<int>*", "ele_ECALDrivenSeed"],
    "eleInBarrel" : ["vector<float>*", "ele_isEB"],
    "eleInEndcap" : ["vector<float>*", "ele_isEE"],

    # "asdf" : ["vector<float>*", "asdf"],
    # "asdf" : ["vector<float>*", "asdf"],
    # "asdf" : ["vector<float>*", "asdf"],
    # "asdf" : ["vector<float>*", "asdf"],
    # "asdf" : ["vector<float>*", "asdf"],
}

for key in dic_branch_var.keys():
    branch  = key
    cppType = dic_branch_var[key][0]
    var     = dic_branch_var[key][1]

    code = """
chain->SetBranchStatus("{branch_}", 1);
chain->SetBranchAddress("{branch_}", &{var_});""".format(branch_=key, var_=var)

    print code

print '\n'

for key in dic_branch_var.keys():
    branch  = key
    cppType = dic_branch_var[key][0]
    var     = dic_branch_var[key][1]

    code = """{cppType_} {var_};""".format(cppType_=cppType, var_=var)

    print code


# -- for DYElectron.h
list_var = [
"ele_pt",
"ele_eta",
"ele_phi",
"ele_energy",
"ele_charge",
"ele_energySC",
"ele_etSC",
"ele_looseID",
"ele_PFIso_PU",
"ele_etaSC",
"ele_isEB",
"ele_full5x5_sigmaIEtaIEta",
"ele_phiSCWidth",
"ele_relPFIso_dBeta",
"ele_mHit",
"ele_etaSCWidth",
"ele_PFIso_NH",
"ele_dxy",
"ele_phiSC",
"ele_mediumID",
"ele_ECALDrivenSeed",
"ele_ooEmooP",
"ele_tightID",
"ele_relPFIso_rhoCorr",
"ele_dPhiIn",
"ele_dz",
"ele_vetoID",
"ele_brem",
"ele_isEE",
"ele_PFIso_CH",
"ele_convVeto",
"ele_r9",
"ele_PFIso_Photon",
"ele_dEtaIn",
"ele_hOverE",
"ele_HEEPID",
]

for var in list_var:
    varInClass = var.split("ele_")[-1]
    code = "Double_t %s;" % (varInClass)
    print code

print '\n'

for var in list_var:
    varInClass = var.split("ele_")[-1]
    code = "%s = ntuple->%s->At(index);" % (varInClass, var)
    print code


