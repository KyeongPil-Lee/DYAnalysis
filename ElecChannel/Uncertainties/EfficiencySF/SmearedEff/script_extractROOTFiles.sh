#!bin/bash

hadd ROOTFile_SmearedEffSF_perMassBin_bkgChange.root \
bkgChange/1st/*.root \
bkgChange/2nd/*.root \
bkgChange/3rd/*.root \
bkgChange/4th/*.root

hadd ROOTFile_SmearedEffSF_perMassBin_sgnChange.root \
sgnChange/1st/*.root \
sgnChange/2nd/*.root \
sgnChange/3rd/*.root \
sgnChange/4th/*.root

hadd ROOTFile_SmearedEffSF_perMassBin_tagChange.root \
tagChange/1st/*.root \
tagChange/2nd/*.root \
tagChange/3rd/*.root \
tagChange/4th/*.root

hadd ROOTFile_SmearedEffSF_perMassBin_nlo.root \
nlo/1st/*.root \
nlo/2nd/*.root \
nlo/3rd/*.root \
nlo/4th/*.root

hadd ROOTFile_SmearedEffSF_perMassBin_stat.root \
stat/1st/*.root \
stat/2nd/*.root \
stat/3rd/*.root \
stat/4th/*.root

echo "finished"