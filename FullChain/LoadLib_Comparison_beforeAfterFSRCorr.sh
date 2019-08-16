#!/bin/bash

if [ $HOME == "/Users/KyeongPil_Lee" ];
	then
	echo "Change ROOT version: ROOT6 -> ROOT5"
	source ${KP_ROOT5_PATH}
fi

root -l -b << EOF

TString IncludePath = gSystem->Getenv("KP_ANALYZER_PATH")
gInterpreter->AddIncludePath( TString::Format("%s", IncludePath.Data()) )

TString IncludePath2 = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gInterpreter->AddIncludePath( TString::Format("%s", IncludePath2.Data()) )

.include

TString BasePath = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gSystem->Load(BasePath+"/libRooUnfold.so")

.x Comparison_beforeAfterFSRCorr.cxx++

.q

EOF

if [ $HOME == "/Users/KyeongPil_Lee" ];
	then
	echo "Return to latest ROOT version: ROOT5 -> ROOT6"
	source ${KP_ROOT6_PATH}
fi