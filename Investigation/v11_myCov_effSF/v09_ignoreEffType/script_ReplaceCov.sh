#!/bin/bash

root -b -q 'ReplaceCov.cxx("reco", "bkgChange")'
root -b -q 'ReplaceCov.cxx("reco", "tagChange")'

root -b -q 'ReplaceCov.cxx("ID", "bkgChange")'
root -b -q 'ReplaceCov.cxx("ID", "tagChange")'

root -b -q 'ReplaceCov.cxx("trig", "tagChange")'

root -b -q 'ReplaceCov.cxx("recoID", "bkgChange")'
root -b -q 'ReplaceCov.cxx("recoID", "tagChange")'

echo "finished"