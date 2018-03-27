#!/bin/bash

root -l -b << EOF

TString BasePath = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gSystem->Load(BasePath+"/libRooUnfold.so")

.x MuonResponseMatrix_1D.C++

.q

EOF

echo "finished"