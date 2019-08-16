#!/bin/bash

root -b -q 'ReplaceCov.cxx("Pt10to20", "bkgChange")'
root -b -q 'ReplaceCov.cxx("Pt10to20", "tagChange")'

root -b -q 'ReplaceCov.cxx("Pt20to30", "bkgChange")'
root -b -q 'ReplaceCov.cxx("Pt20to30", "tagChange")'

root -b -q 'ReplaceCov.cxx("Pt30to40", "bkgChange")'
root -b -q 'ReplaceCov.cxx("Pt30to40", "tagChange")'

root -b -q 'ReplaceCov.cxx("Pt40to50", "bkgChange")'
root -b -q 'ReplaceCov.cxx("Pt40to50", "tagChange")'

root -b -q 'ReplaceCov.cxx("Pt50to200", "bkgChange")'
root -b -q 'ReplaceCov.cxx("Pt50to200", "tagChange")'

echo "finished"