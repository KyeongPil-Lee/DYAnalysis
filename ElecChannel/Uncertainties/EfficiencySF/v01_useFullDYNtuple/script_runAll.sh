#!/bin/bash

root -b -q SmearedEffSF_stat.cxx++ >&SmearedEffSF_stat.log&
root -b -q SmearedEffSF_bkgChange.cxx++ >&SmearedEffSF_bkgChange.log&
root -b -q SmearedEffSF_sgnChange.cxx++ >&SmearedEffSF_sgnChange.log&
root -b -q SmearedEffSF_tagChange.cxx++ >&SmearedEffSF_tagChange.log&
root -b -q SmearedEffSF_nlo.cxx++ >&SmearedEffSF_nlo.log&

echo "submission is finished"