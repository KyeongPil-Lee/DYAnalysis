#!/bin/bash

cd 1st
root -b -q 'SmearedEffSF_givenType_indexRange.cxx++("bkgChange", 0, 499)' >&SmearedEffSF_givenType_indexRange.log&
sleep 3

cd ../2nd
root -b -q 'SmearedEffSF_givenType_indexRange.cxx++("bkgChange", 500, 999)' >&SmearedEffSF_givenType_indexRange.log&
sleep 3

cd ../3rd
root -b -q 'SmearedEffSF_givenType_indexRange.cxx++("bkgChange", 1000, 1499)' >&SmearedEffSF_givenType_indexRange.log&
sleep 3

cd ../4th
root -b -q 'SmearedEffSF_givenType_indexRange.cxx++("bkgChange", 1500, 1999)' >&SmearedEffSF_givenType_indexRange.log&
sleep 3

cd ..

