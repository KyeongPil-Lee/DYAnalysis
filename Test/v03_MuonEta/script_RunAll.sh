#!bin/bash

root -l -b -q 'MuonPlots.cxx++("MC", 0)' >&MuonPlots_MC_0.log&
sleep 30

root -l -b -q 'MuonPlots.cxx++("MuonPhys", 0)' >&MuonPlots_MuonPhys_0.log&
sleep 30

root -l -b -q 'MuonPlots.cxx++("MC", 1)' >&MuonPlots_MC_1.log&
sleep 30

echo "Submission is finished"