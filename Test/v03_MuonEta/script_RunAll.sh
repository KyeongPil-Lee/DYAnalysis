#!bin/bash

root -l -b -q 'MuonPlots.cxx++("MC", 0)' >&MuonPlots_MC_0.log&
root -l -b -q 'MuonPlots.cxx++("MuonPhys", 0)' >&MuonPlots_MuonPhys_0.log&
root -l -b -q 'MuonPlots.cxx++("MC", 1)' >&MuonPlots_MC_1.log&
root -l -b -q 'MuonPlots.cxx++("MuonPhys", 1)' >&MuonPlots_MC_1.log&

echo "Submission is finished"