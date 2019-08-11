#!/bin/bash

cd 1st
root -b -q 'SmearedEffSF_GivenType_IndexRange.cxx++("nlo", 0, 499)' >&SmearedEffSF_GivenType_IndexRange.log&
sleep 3

cd ../2nd
root -b -q 'SmearedEffSF_GivenType_IndexRange.cxx++("nlo", 500, 999)' >&SmearedEffSF_GivenType_IndexRange.log&
sleep 3

cd ../3rd
root -b -q 'SmearedEffSF_GivenType_IndexRange.cxx++("nlo", 1000, 1499)' >&SmearedEffSF_GivenType_IndexRange.log&
sleep 3

cd ../4th
root -b -q 'SmearedEffSF_GivenType_IndexRange.cxx++("nlo", 1500, 1999)' >&SmearedEffSF_GivenType_IndexRange.log&
sleep 3

cd ..

